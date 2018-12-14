#include "BinaryWriter.hpp"

#include <fstream>

struct BinaryWriter_private
{
	std::ofstream file;
};

template <class type> void type_to_bytes(BinaryWriter* self, type value)
{
	/*
	uint8_t buf[sizeof(type)];
	for(uint_fast8_t a = 0; a < sizeof(type); ++a)
	{
		buf[a] = static_cast<uint8_t>(value >> (a * 8));
	}
	self->WriteBytes(buf, sizeof(buf));
	*/
	self->WriteBytes(reinterpret_cast<uint8_t*>(&value), sizeof(type));
}

/**
	BinaryWriter.cpp
	This class is my implementation of Microsoft's C# BinaryWriter
	It writes the following to a file:
	 - the bytes in a signed or unsigned integer (8-bit, 16-bit, 32-bit, 64-bit, or 128-bit)
	 - single characters (8-bit int) or strings of characters (in char*)
	 - Microsoft-style strings (one byte before the string specifies the length)

	@todo		Make an exception class to throw when errors occur instead of using std::string
	@arg s		The file to write
	@arg bak	If this is true, the pre-existing file (if there is one) with the name
				specified in the previous argument will have .bak appended to its name
*/

BinaryWriter::BinaryWriter(const std::string& filename, bool bak)
{
	this->privates = new BinaryWriter_private();

	if(bak && BinaryLibUtil::file_exists(filename.c_str()))
	{
		std::string s_bak = filename + ".bak";
		BinaryLibUtil::move_file(filename.c_str(), s_bak.c_str(), true);
	}
	this->privates->file.open(filename.c_str(), std::ios::binary | std::ios::trunc);
	if(!this->privates->file.good())
	{
		throw ("BinaryWriter: error opening \"" + filename + "\"");
	}
}

BinaryWriter::~BinaryWriter()
{
	delete this->privates;
}

void BinaryWriter::WriteBool(bool b)
{
	this->WriteUInt8(b ? 1 : 0);
}

#define X(name, type)\
void BinaryWriter::Write##name(type i)\
{\
	type_to_bytes<type>(this, i);\
}

BINARYWRITER_TYPES

#if __SIZEOF_INT128__ == 16
X(Int128, __int128)
X(UInt128, unsigned __int128)
#else
#pragma message("BinaryWriter::Write(U)Int128 not included")
#endif

#ifdef FLOAT16
X(Float128, FLOAT16)
#else
#pragma message("BinaryWriter::WriteFloat128 not included")
#endif

#undef X

void BinaryWriter::WriteChars(const char* c, uint_fast64_t len)
{
	this->WriteBytes(reinterpret_cast<const uint8_t*>(c), len);
}

void BinaryWriter::WriteBytes(const uint8_t* c, uint_fast64_t len)
{
	if(!this->privates->file.is_open())
	{
		throw std::string("BinaryWriter: tried to write, but no file is open");
	}

	this->privates->file.write(reinterpret_cast<const char*>(c), len);
	if(!this->privates->file.good())
	{
		throw std::string("BinaryWriter: write failed");
	}
}

void BinaryWriter::WriteBytes(const std::vector<uint8_t>& bytes)
{
	if(bytes.size() > 0)
	{
		this->WriteBytes(&bytes[0], bytes.size());
	}
}

void BinaryWriter::WriteChars(const std::vector<char>& bytes)
{
	if(bytes.size() > 0)
	{
		this->WriteChars(&bytes[0], bytes.size());
	}
}

void BinaryWriter::WriteChars(const std::string& s)
{
	this->WriteChars(s.c_str(), s.length());
}

// WARNING: this might not be accurate for large values!
void BinaryWriter::Write7BitEncodedInt(uint64_t value)
{
	while(value >= 128)
	{
		this->WriteUInt8(static_cast<uint8_t>(value | 0x80));
		value >>= 7;
	}
	this->WriteUInt8(static_cast<uint8_t>(value));
}

void BinaryWriter::WriteStringMS(const std::string& s)
{
	size_t len = s.length();
	this->Write7BitEncodedInt(len);
	this->WriteChars(s.c_str(), len);
}
