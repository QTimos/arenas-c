#define ARENA_IMPLEMENTATION
#include "arenas.h"
#include <stdio.h>
#include <string.h>

typedef enum
{
	SUCCESS = 0,
	CREATION_ERROR = 1,
	FILE_FAILURE_ERROR = 2,
	ALLIGNMENT_ERROR = 3
} ARENAError;

typedef enum
{
	MALLOC = 0,
	MMAP_ALLOC = 1,
	ALIGNED_ALLOC = 2
} tAlocationBackend;

typedef struct sArena
{
	void *memory;
	size_t capacity;

	size_t base_alignment;
	size_t block_size;
	size_t block_count;

	uint64_t *bitmap;
	tAlocationBackend allocation;
} tArena;


void release_memory(void *memory, size_t size, tAlocationBackend allocation)
{
	if (!memory) return;
	switch (allocation)
	{
		case MMAP_ALLOC:
			munmap(memory, size);
			return ;
		case ALIGNED_ALLOC:
		case MALLOC:
			free(memory);
			return ;
	}
}

void *get_memory(size_t size, size_t alignment, tAlocationBackend allocation)
{
	void *pages;

	if (!size) return NULL;
	switch (allocation)
	{
		case MMAP_ALLOC:
			pages = mmap(
					NULL, size, PROT_READ | PROT_WRITE,
					MAP_PRIVATE | MAP_ANON, -1, 0
					);
			if (pages == MAP_FAILED)
				return NULL;
			return pages;
		case ALIGNED_ALLOC:
			if (!alignment || (size % alignment) || (alignment & (alignment - 1)))
				return NULL;
			return aligned_alloc(alignment, size);
		case MALLOC:
			return malloc(size);
		default:
			return NULL;
	}
}

tArena *arena_create(size_t , size_t alignment, const char *file_path, tAlocationBackend allocation)
{
	if (size == 0)
		return NULL;
	if (alignment == 0)
		return NULL;
	tArena *arena = malloc(sizeof(tArena));
	if (!arena)
		return NULL;
	arena->memory = NULL;
	arena->base_alignment = alignment;
	arena->block_size = ;
	return arena;
}


#define SIZE 4096
int main(int argc, char **argv)
{
	tArena *arena = arena_create(SIZE, 64, NULL, MMAP_ALLOC);
	if (!arena)
	{
		dprintf(2, "Failed to create arena\n");
		return 1;
	}
	float arr[16] = {
		0.417022f, 0.720324f, 0.000114f, 0.302333f,
		0.146756f, 0.092339f, 0.186260f, 0.345561f,
		0.396767f, 0.538817f, 0.419195f, 0.685220f,
		0.204452f, 0.878117f, 0.027388f, 0.670468f
	};
	return (0);
}
