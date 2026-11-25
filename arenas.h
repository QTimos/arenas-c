#ifndef ARENAS_H
#define ARENAS_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>
// tArena *arena_create(size_t size, size_t alignment, const char *file_path, tAlocationBackend allocation);

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


#endif
#endif
