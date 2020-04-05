#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <errno.h>
#include "bitmap.h"

struct bloom_filter_header
{
	uint32_t magic;
	uint32_t k;
	uint64_t count;
	char __buf[496];
} __attribute__ ((packed));
typedef struct bloom_filter_header bloom_filter_header;

typedef struct
{
	bloom_filter_header *header;
	bloom_bitmap *map;
	uint64_t offset;
	uint64_t bitmap_size;
} bloom_filter;

typedf struct
{
	uint64_t bytes;
	uint32_t k;
	uint64_t capacity;
	double fp_probability;
} bloom_filter_params;

int bf_from_bitmap(bloom_bitmap *map, uint32_t k, int new_filter, bloom_filter *filter);

int bf_add(bloom_filter *filter, char *key);
int bf_contains(bloom_filter *filter, char *key);
uint64_t bf_size(bloom_filter *filter);

int bf_flush(bloom_filter *filter);
int bf_close(bloom_filter *filter);

void bf_compute_hashes(uint32_t k, char *key, uint64_t *hashes);

int bf_params_for_capacity(bloom_filter_params *params);
int bf_size_for_capacity_prob(bloom_filter_params *params);

int bf_fp_probability_for_capacity_size(bloom_filter_params *params);

int bf_capacity_for_size_prob(bloom_filter_params *params);

int bf_ideal_k(bloom_filter_params *params);

#endif
