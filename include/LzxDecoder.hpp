#pragma once

#include <array>
#include <string>

#include <BinaryReader.hpp>
#include <BinaryWriter.hpp>
#include "BitBuffer.hpp"

#define MIN_MATCH					2
#define MAX_MATCH					257
#define NUM_CHARS					256
#define PRETREE_NUM_ELEMENTS		20
#define ALIGNED_NUM_ELEMENTS		8
#define NUM_PRIMARY_LENGTHS			7
#define NUM_SECONDARY_LENGTHS		249

const uint16_t PRETREE_MAXSYMBOLS = PRETREE_NUM_ELEMENTS;
const uint16_t MAINTREE_MAXSYMBOLS = NUM_CHARS + 50*8;
const uint16_t LENGTH_MAXSYMBOLS = NUM_SECONDARY_LENGTHS + 1;
const uint16_t ALIGNED_MAXSYMBOLS = ALIGNED_NUM_ELEMENTS;

const uint8_t PRETREE_TABLEBITS = 6;
const uint8_t MAINTREE_TABLEBITS = 12;
const uint8_t LENGTH_TABLEBITS = 12;
const uint8_t ALIGNED_TABLEBITS = 7;

class LzxDecoder
{
	public:
		explicit LzxDecoder(const uint_fast16_t window_bits);
		LzxDecoder(const LzxDecoder&) = delete;
		~LzxDecoder();
		void Decompress(const uint8_t* inBuf, const uint_fast32_t inLen, uint8_t* outBuf, const uint_fast32_t outLen);

	private:
		void MakeDecodeTable(uint16_t nsyms, uint8_t nbits, uint8_t* length, uint16_t* table);
		void ReadLengths(uint8_t* lens, uint_fast32_t first, uint_fast32_t last, BitBuffer& bitbuf);
		uint32_t ReadHuffSym(const uint16_t* table, const uint8_t* lengths, uint32_t nsyms, uint8_t nbits, BitBuffer& bitbuf);

		std::array<uint32_t, 51> position_base;
		std::array<uint8_t, 52> extra_bits;

		enum class BLOCKTYPE : uint8_t
		{
			INVALID = 0,
			VERBATIM = 1,
			ALIGNED = 2,
			UNCOMPRESSED = 3,
		};
		static std::string to_string(BLOCKTYPE);

		struct
		{
			uint8_t*			window;
			uint_fast32_t		window_size;
			uint_fast32_t		window_posn;

			uint_fast32_t		R0, R1, R2; 		// for the LRU offset system
			uint16_t			main_elements;		// number of main tree elements
			bool				header_read;		// have we started decoding at all yet?
			BLOCKTYPE			block_type;			// type of this block
			uint32_t			block_length;		// uncompressed length of this block
			uint32_t			block_remaining;	// uncompressed bytes still left to decode

			std::array<uint8_t,  PRETREE_MAXSYMBOLS>  PRETREE_len;
			std::array<uint8_t, MAINTREE_MAXSYMBOLS> MAINTREE_len;
			std::array<uint8_t,   LENGTH_MAXSYMBOLS>   LENGTH_len;
			std::array<uint8_t,  ALIGNED_MAXSYMBOLS>  ALIGNED_len;

			std::array<uint16_t, (1 <<  PRETREE_TABLEBITS) + ( PRETREE_MAXSYMBOLS * 2)>  PRETREE_table;
			std::array<uint16_t, (1 << MAINTREE_TABLEBITS) + (MAINTREE_MAXSYMBOLS * 2)> MAINTREE_table;
			std::array<uint16_t, (1 <<   LENGTH_TABLEBITS) + (  LENGTH_MAXSYMBOLS * 2)>   LENGTH_table;
			std::array<uint16_t, (1 <<  ALIGNED_TABLEBITS) + ( ALIGNED_MAXSYMBOLS * 2)>  ALIGNED_table;

		} state;
};
