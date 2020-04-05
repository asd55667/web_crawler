#ifndef HASH_H
#define HASH_H



static inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
	  return (x << r) | (x >> (32 - r));
}

static inline uint32_t fmix(uint32_t h)
{
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

uint32_t murmur3_32(const void *key, size_t len, uint32_t seed);

uint32_t fnv1a(const void *key, size_t len); 


#endif

