#pragma once

#include <fstream> // std::ifstream, std::remove, and rename

//#define FLOAT16 long double

// not supported by clang
//#define FLOAT16 __float128

class BinaryLibUtil
{
	public:
		static bool file_exists(const char* filename);
		static void delete_file(const char* filename);
		static void move_file(const char* src, const char* dst, const bool overwrite);

	private:
		void static_asserts()
		{
			static_assert(sizeof(char) == 1, "char must be 1 byte");

			static_assert(sizeof(int8_t) == 1, "something is wrong with stdint");
			static_assert(sizeof(uint8_t) == 1, "something is wrong with stdint");

			static_assert(sizeof(int16_t) == 2, "something is wrong with stdint");
			static_assert(sizeof(uint16_t) == 2, "something is wrong with stdint");

			static_assert(sizeof(int32_t) == 4, "something is wrong with stdint");
			static_assert(sizeof(uint32_t) == 4, "something is wrong with stdint");

			static_assert(sizeof(int64_t) == 8, "something is wrong with stdint");
			static_assert(sizeof(uint64_t) == 8, "something is wrong with stdint");

			#if __SIZEOF_INT128__ == 16
			static_assert(sizeof(__int128) == 16, "__int128 must be 16 bytes");
			static_assert(sizeof(unsigned __int128) == 16, "unsigned __int128 must be 16 bytes");
			#endif

			static_assert(sizeof(float) == 4, "float must be 4 bytes");
			static_assert(sizeof(double) == 8, "double must be 8 bytes");
			#ifdef FLOAT16
			static_assert(sizeof(FLOAT16) == 16, "FLOAT16 must be 16 bytes");
			#endif
		}
};