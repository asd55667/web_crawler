#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stdio.h>
#include "hashlib.h"

typedef struct
{
	uint32_t n;
	uint32_t k;
	uint32_t size;
    uint32_t offset;
	unsigned char *bits;
} bloom_filter;



static inline int bf_getbit(bloom_filter *bf, uint32_t idx)
{
	return (bf->bits[idx >> 3] >> (7 - idx % 8)) & 0x1;
}

static inline void bf_setbit(bloom_filter *bf, uint32_t idx)
{
	unsigned char byte = bf->bits[idx >> 3];
	unsigned char offset = 7 - idx % 8;
	byte |= 1 << offset;
	bf->bits[idx >> 3] = byte;
}

bloom_filter *bf_init(uint32_t k, uint32_t nboxes);
void bf_hash(uint32_t *hashes, uint32_t k, char *key);
int bf_search(bloom_filter *bf, uint32_t *hashes);

int bf_add(bloom_filter *bf, char *key);
int bf_contain(bloom_filter *bf, char *key);

void free_bf(bloom_filter *bf);
void print_bf(bloom_filter *bf);


#endif

