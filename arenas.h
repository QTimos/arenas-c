#ifndef ARENAS_H
#define ARENAS_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef enum
{
	SUCCESS = 0,
	CREATION_ERROR = 1,
	FILE_FAILURE_ERROR = 2,
} ARENAError;

typedef struct sArena
{
	uint8_t *start;
	size_t size;
	size_t offset;
} tArena;

tArena *arena_create(size_t size, size_t offset);

/* thread-local error state */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define HT_THREAD_LOCAL _Thread_local
#  define HT_THREAD_SAFE_ERROR_STATE 1
#elif defined(__GNUC__)
#  define HT_THREAD_LOCAL __thread
#  define HT_THREAD_SAFE_ERROR_STATE 1
#elif defined(_MSC_VER)
#  define HT_THREAD_LOCAL __declspec(thread)
#  define HT_THREAD_SAFE_ERROR_STATE 1
#else
#  define HT_THREAD_LOCAL /* empty / not thread safe branch */
#  define HT_THREAD_SAFE_ERROR_STATE 0
#endif

#if !HT_THREAD_SAFE_ERROR_STATE && !defined(HT_ACCEPT_UNSAFE_THREAD_STATE)
#  if defined(__GNUC__) || defined(__clang__) || \
      (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L)
#    warning "HT: error state is not thread-local on this toolchain (not thread-safe). Define HT_ACCEPT_UNSAFE_THREAD_STATE to silence."
#  elif defined(_MSC_VER)
#    pragma message("HT: error state is not thread-local on this toolchain (not thread-safe). Define HT_ACCEPT_UNSAFE_THREAD_STATE to silence.")
#  endif
#endif



#ifdef ARENAS_IMPLEMENTATION

static HT_THREAD_LOCAL uint8_t arena_error_state = 0;
static uint8_t *get_err(void)
{
	return &arena_error_state;
}

#define _ARENAERROR (get_error())
#define ARENAERROR (*_ARENAERROR)
fopen

tArena *arena_create(size_t size, size_t offset, size_t alignment, const char *file_path)
{
	ARENAERROR = SUCCESS;
	if (size % alignment != 0 || offset % alignment != 0)
		dprintf(1, "WARNING: size or offset of creating arena is not aligned to %zu.\n", alignment);
	tArena *arena = malloc(sizeof(tArena));
	if (!arena)
	{
		ARENAERROR = CREATION_ERROR;
		return NULL;
	}
	if (!file_path)
	{
#if defined(MMAP_ALLOC)
		void *pages = mmap(
				NULL, size, PROT_READ | PROT_WRITE,
				MAP_HUGETLB | MAP_HUGE_2MB
				);
		if ((int)pages == -1)
		{
			ARENAERROR = CREATION_ERROR;
			return NULL;
		}
#elif defined(ALIGNED_ALLOC)
		void *pages = aligned_alloc(alignment, size);
		if (!pages)
		{
			ARENAERROR = CREATION_ERROR;
			return NULL;
		}
#else // defalut using malloc
		void *pages = malloc(size);
		if (!pages)
		{
			ARENAERROR = CREATION_ERROR;
			return NULL;
		}
#endif
	}
	else
	{
#define MMAP_ALLOC
		int fd = open(file_path, O_RDONLY);
		if (fd == -1)
		{
			ARENAERROR = FILE_FAILURE_ERROR:
			return NULL;
		}
		void *pages = mmap(
				NULL, size, PROT_READ | PROT_WRITE,
				MAP_HUGETLB | MAP_HUGE_2MB | MAP_FILE, fd, 0
				);
		if ((int)pages == -1)
		{
			ARENAERROR = CREATION_ERROR;
			return NULL;
		}
	}
}

#endif
#endif
