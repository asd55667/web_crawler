#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bloom_filter.h"

bloom_filter *bf_init(uint32_t k, uint32_t nboxes)
{
	bloom_filter *bf;
	bf = malloc(sizeof *bf);
	bf->bits = malloc(((nboxes >> 3) + 1) * sizeof bf->bits);

    bf->n = 0;
    bf->k = k;
	bf->size = nboxes;
	bf->offset = nboxes / k;
	return bf;
}

int bf_search(bloom_filter *bf, uint32_t *hashes)
{
	uint32_t i, idx;
	for(i = 0; i < bf->k; i++)
	{
		idx = i * bf->offset + hashes[i] % bf->offset;
		if(bf_getbit(bf, idx))
			return 1;
	}
	return 0;
}

void bf_hash(uint32_t *hashes, uint32_t k, char *key)
{
	size_t len = strlen(key);
	hashes[0] = murmur3_32(key, len, 0);
	hashes[1] = fnv1a(key, len);

	for(uint32_t i = 4; i < k; i++)
		hashes[i] = hashes[1] + ((i + hashes[0]) % 18446744073709551557U);
}

int bf_add(bloom_filter *bf, char *key)
{
	uint32_t *hashes = alloca(bf->k * sizeof(uint32_t));	
	
	bf_hash(hashes, bf->k, key);

	if(bf_search(bf, hashes))
		return 0;

	uint32_t i, idx;
	for(i = 0; i < bf->k; i++)
	{
		idx = i * bf->offset + hashes[i] % bf->offset;	
		bf_setbit(bf, idx);
	}
    bf->n += 1;
    return 1;
}

int bf_contain(bloom_filter *bf, char *key)
{
	uint32_t *hashes;
	hashes = alloca(bf->k * sizeof *hashes);
	bf_hash(hashes, bf->k, key);
	return bf_search(bf, hashes);
}

void free_bf(bloom_filter *bf)
{
    free(bf->bits);
    free(bf);
}

void print_bf(bloom_filter *bf)
{
    printf("k: %u n: %u size: %u\n", bf->k, bf->n, bf->size);
    uint32_t n = (bf->size > 30) ? 30 : bf->size;
    for(uint32_t i = 0; i < n; i++)
    {
        if((i % 8) == 0)
            printf(" ");
        printf("%d", (bf->bits[i >> 3] >> (7 - i % 8)) & 0x1);
    }
    printf("\n");
    
}


int test_bf()
{
    bloom_filter *bf = bf_init(2, 20);
    print_bf(bf);
    bf_setbit(bf, 2);
    if(!bf_getbit(bf, 2))
        printf("bf_getbit fail!\n");
    bf_add(bf, "ASDJ");
    
    uint32_t *hs = malloc(2 * sizeof(uint32_t *));
    bf_hash(hs, 2, "ASDJ");
    bf_search(bf, hs);
    
    print_bf(bf);
    if(!bf_contain(bf, "ASDJ"))
        printf("add fail!\n");
    
    print_bf(bf);
    
    free(hs);
    free_bf(bf);
    
    getchar();
    return 0;
}
//
//k: 2 n: 0 size: 20
//00000000 00000000 0000
//k: 2 n: 1 size: 20
//00100001 00000000 0001
