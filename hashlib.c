#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashlib.h"

static uint32_t murmur32_scramble(uint32_t k)
{
	k *= 0xcc9e2d51;
	k = rotl32(k, 15);
	k *= 0x1b873593;
	return k;
}

uint32_t murmur3_32(const void *key, size_t len, uint32_t seed)
{
    uint8_t *data = (uint8_t *)key;
	uint32_t h = seed;
	uint32_t k;
	
	for(size_t i = len >> 2; i; i--)
	{
		k = *((uint32_t *)data);
		data += sizeof(uint32_t);

		h ^= murmur32_scramble(k);
		h = rotl32(h, 13);
		h = h * 5 + 0xe6546b64;
	}

	k = 0;
	for(size_t i = len & 3; i; i--)
	{
		k <<= 8;
		k |= data[i - 1];
	}

	h ^= murmur32_scramble(k);
	h ^= len;
    
	h = fmix(h);
	return h;
}

uint32_t fnv1a(const void *key, size_t len)
{
	uint8_t *data = (uint8_t *)key;

	uint32_t h = 0x811c9dc5;  // offset
	uint32_t k;

	for(size_t i = 0; i < len; i++)
	{
		k = *data;
		data += sizeof *data;
		
		h *= 0x01000193; // prime
		h ^= k;
	}
	return h;
}

	
// murmur 				output
// // A  S  D  J    %u
// // 65 83 68 74  3398122917
//
// k val first loop
// 1245991745
// // 0b100 1010 0100 0100 0101 0011 0100 0001
//
// fnv1a 				output
// // A  S  D  J    %u	
// // 65 83 68 74  60015639  
