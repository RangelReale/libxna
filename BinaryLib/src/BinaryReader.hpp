#pragma once

#include "BinaryLibUtil.hpp"

#include <cstdint>
#include <string>
#include <memory>

#define BINARYREADER_TYPES\
	X(Int8, int8_t)\
	X(UInt8, uint8_t)\
	X(Int16, int16_t)\
	X(UInt16, uint16_t)\
	X(Int32, int32_t)\
	X(UInt32, uint32_t)\
	X(Int64, int64_t)\
	X(UInt64, uint64_t)\
	X(Float32, float)\
	X(Float64, double)

struct BinaryReader_private;
class BinaryReader
{
	public:
		explicit BinaryReader(const std::string& s);
		BinaryReader(std::unique_ptr<uint8_t[]> data, const uint_fast64_t size);
		BinaryReader(const BinaryReader&) = delete;
		~BinaryReader();

		void ChangeFile(const std::string& s);
		void ChangeFile(std::unique_ptr<uint8_t[]> data, const uint_fast64_t size);
		void Close();

		uint_fast64_t GetFileSize() const;
		uint_fast64_t GetPosition() const;
		void Seek(uint_fast64_t pos);

		bool ReadBool();

		#define X(name, type) type Read##name();

		BINARYREADER_TYPES

		#if __SIZEOF_INT128__ == 16
		X(Int128, __int128)
		X(UInt128, unsigned __int128)
		#endif

		#ifdef FLOAT16
		X(Float128, FLOAT16)
		#endif

		#undef X

		std::unique_ptr<char[]> ReadChars(uint_fast64_t bytes);
		std::unique_ptr<uint8_t[]> ReadBytes(uint_fast64_t bytes);

		std::string ReadString(uint_fast64_t length);

		uint_fast64_t Read7BitEncodedInt();
		std::string ReadStringMS();

	private:
		BinaryReader_private* privates;
};