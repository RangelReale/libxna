#pragma once

#include "BinaryLibUtil.hpp"

#include <cstdint>
#include <vector>

#define BINARYWRITER_TYPES\
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

struct BinaryWriter_private;
class BinaryWriter
{
	public:
		BinaryWriter(const std::string& filename, bool bak = false);
		BinaryWriter(const BinaryWriter&) = delete;
		~BinaryWriter();

		void WriteBool(bool b);

		#define X(name, type) void Write##name(type);

		BINARYWRITER_TYPES

		#if __SIZEOF_INT128__ == 16
		X(Int128, __int128)
		X(UInt128, unsigned __int128)
		#endif

		#ifdef FLOAT16
		X(Float128, FLOAT16)
		#endif

		#undef X

		void WriteChars(const char*, uint_fast64_t len);
		void WriteBytes(const uint8_t*, uint_fast64_t len);
		void WriteBytes(const std::vector<uint8_t>&);
		void WriteChars(const std::vector<char>&);
		void WriteChars(const std::string&);

		void Write7BitEncodedInt(uint64_t value);
		void WriteStringMS(const std::string& s);

	private:
		BinaryWriter_private* privates;
};