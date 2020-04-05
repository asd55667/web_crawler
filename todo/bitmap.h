#ifndef BLOOM_BITMAP_H
#define BLOOM_BITMAP_H
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>

typedef enum
{
	SHARED 		= 1;
	PERSISTENT	= 2;
	ANONYMOUS	= 4;
	NEW_BITMAP	= 8;
} bitmap_mode;

typedef struct
{
	bimap_mode mode;
	int fd;
	uint64_t size;
	unsigned char *mmap;
	unsigned char *dirty_pages;
} bloom_bitmap;

int bitmap_from_file(int fd, uint64_t len, bitmap_mode mode, bloom_bitmap *map);

int bitmap_from_filename(char *filename, uint64_t len, int create, bitmap_mode mode, bloom_bitmap *map);

int bitmap_flush(bloom_bitmap *map);
int bitmap_close(bloom_bitmap *map);

inline int bitmap_getbit(bloom_bitmap *map, uint64_t idx)
{
	return (map->mmap[idx >> 3] >> (7 - (idx % 8))) & 0x1;
}

inline void bitmap_setbit(bloom_bitmap *map, uint64_t idx)
{
	unsigned char byte = map->mmap[idx >> 3];
	unsigned char byte_off = 7 - idx % 8;
	byte |= 1 << byte_off;
	map->mmap[idx >> 3] = byte;

	if(map->mode == PERSISTENT)
	{
		uint64_t page = idx >> 15;
		byte = map->dirty_pages[page >> 3];
		byte_off = 7 - page % 8;
		byte |= 1 << byte_off;
		map->dirty_pages[page >> 3] = byte;
	}
}

#endif
