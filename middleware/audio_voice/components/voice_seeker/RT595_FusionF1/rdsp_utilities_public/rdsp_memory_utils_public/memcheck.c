/*
 * Copyright 2021 by Retune DSP
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/
#include "memcheck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* Example usage showing file, line, function that allocated the corrupted memory array
* #undef malloc
* #undef free
* #define malloc(size) memcheck_malloc(size, __FILE__, __LINE__, __FUNCTION__)
* #define free(x) memcheck_free(x)
*
* If no additional information is to be provided, the memcheck_malloc() function can replace malloc(size) as
* memcheck_malloc(size, NULL, 0, NULL)
*/

#define MAX_NODES 1000				// Maximum number of allocations possible
const char pattern[] = "XYXYXYXY";	// Guard pattern to insert around the allocated memory
#define NUM_PATTERN_REPETITIONS 2	// Number of time the pattern is repeated on each side of the memory array

typedef struct memcheck_node {
  void * addr;
  size_t size;
  int line;
  const char * file;
  const char *function;
} memcheck_node_t;

static memcheck_node_t m_ctx[MAX_NODES];

void* memcheck_malloc_align(size_t size, size_t alignment, const char* file, int line, const char* function) {
    void* p1; // original block
    void** p2;// aligned block
    size_t offset = alignment - 1 + sizeof(void*);
    if ((p1 = memcheck_malloc(size + offset, file, line, function)) == NULL ) {
        return NULL;
    }
    p2 = (void**)(((size_t)p1 + offset) & ~(alignment - 1));
    p2[-1] = p1;
    return p2;
}

void memcheck_free_align(void *data) {
	memcheck_free(((void**) data)[-1]);
}

// Memcheck replacement function for malloc()
void* memcheck_malloc(size_t size, const char* file, int line, const char* function) {
  int i, r;
  char* addr = NULL;
  static int is_initialized = 0;

  // Initialize the first time it is called
  if (is_initialized == 0) {
	  for (i = 0; i < MAX_NODES; i++) {
		  (void)memset(&m_ctx[i], 0, sizeof(memcheck_node_t));
	  }
	  is_initialized = 1;
  }

  if (size <= 0) {
    return NULL;
  }

  for (i = 0; i < MAX_NODES; i++) {
    if(NULL == (char*) (m_ctx[i].addr) ) {
		addr = (char*) malloc(size + 2 * NUM_PATTERN_REPETITIONS * sizeof(pattern));
		if (addr == NULL) {
		  return addr;
		}

		char* current_pattern = addr;
		for (r = 0; r < NUM_PATTERN_REPETITIONS; r++) {
			(void)memcpy(current_pattern, pattern, sizeof(pattern));
			(void)memcpy(current_pattern + NUM_PATTERN_REPETITIONS * sizeof(pattern) + size, pattern, sizeof(pattern));
			current_pattern += sizeof(pattern);		// Increment to next pattern
		}
		m_ctx[i].addr = addr;
		m_ctx[i].size = size;
		m_ctx[i].line = line;
		m_ctx[i].file = file;
		m_ctx[i].function = function;
		break;
    } else {
    	if (i == MAX_NODES - 1) {
    		(void)printf("No free memcheck nodes, increase MAX_NODES\n");
    		return NULL;
    	}
    }
  }
  return (addr + NUM_PATTERN_REPETITIONS * sizeof(pattern));
}

// Memcheck replacement function for free()
void memcheck_free(void* addr) {
  int i;
  char* addr_c = (char*) addr;

  if (addr == NULL) {
    return;
  }

  addr_c -= NUM_PATTERN_REPETITIONS * sizeof(pattern);
  for (i = 0; i < MAX_NODES; i++) {
    if(addr_c == (char*)(m_ctx[i].addr)) {
      free(addr_c);
      m_ctx[i].addr = NULL;
      m_ctx[i].size = 0;
      m_ctx[i].line = -1;
      m_ctx[i].file = NULL;
      m_ctx[i].function = NULL;
      break;
    }
  }
}

// Function that checks all dynamic memory allocations at run-time
void memcheck_check(void) {
  int i, r;
  char* addr = NULL;

  for (i = 0; i < MAX_NODES; i++) {
    if(NULL != (char*) (m_ctx[i].addr)) {
    	addr = m_ctx[i].addr;

    	int current_node_corrupt = 0;
    	char* current_pattern = addr;
		for (r = 0; r < NUM_PATTERN_REPETITIONS; r++) {
			if (memcmp(current_pattern, pattern, sizeof(pattern)) || memcmp(current_pattern + NUM_PATTERN_REPETITIONS * sizeof(pattern) + m_ctx[i].size, pattern, sizeof(pattern))) {
				current_node_corrupt = 1;
			}
			current_pattern += sizeof(pattern);		// Increment to next pattern
		}

    	if (current_node_corrupt != 0) {
    		if (m_ctx[i].file != NULL && m_ctx[i].function != NULL) {
    			(void)printf("Address %p, allocated at %s:%d:%s() is corrupted. ", m_ctx[i].addr, m_ctx[i].file, m_ctx[i].line, m_ctx[i].function);
    		} else {
    			(void)printf("Address %p is corrupted. ", m_ctx[i].addr);
    		}
      }
    }
  }
}
