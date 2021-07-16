/*
 * Copyright 2016, Crank Software Inc. All Rights Reserved.
 * 
 * For more information email info@cranksoftware.com.
 */
/** \File: greal_alloc.c
 * This is a series of cover functions to allow the traceability and replacement 
 * of the standard C library memory allocation routines.
 */
#include <gre/sdk/greal.h>

#include <stdlib.h>
#include <string.h>

//RJD:  Any of the implementations below can be overridden by placing that define in a header file that will get included in greal.h.  This
// allows for a particular greal implementation to cover the functions and then place the defines in their own implementation header files and 
// overide the functionality.  


/**
 * The memory metrics implementation adds an additional header on top of the allocation
 * for the purposes of tracking the allocation patterns.
 */
//#define GREAL_MEMORY_METRICS 1

#if defined(GREAL_MEMORY_METRICS)
#include <stdio.h>

//Use this define to enable the tracing of malloc/free calls
#define MEM_METRICS_TRACE

//Use this define to enable the periodic output of memory allocation statistics
//#define MEM_METRICS_STATS

//After this many combined allocation or free operations the stats will be dumped
#define MEM_METRICS_STATS_FREQUENCY 100

//Sanity check on the overallocated pointer that we create for book-keeping
#define MEM_PTR_SIGNATURE 0xdeadbeef

typedef struct greal_mem_ptr {
	int32_t		nbytes;			//Number of bytes user requested
	int32_t		sig;			//MEM_PTR_SIGNATURE
	char		data[0];		//User data follows this
} greal_mem_ptr_t;

int _gr_alloc_count = 0;		//Number of malloc() calls
int _gr_free_count = 0;			//Number of free() calls
int _gr_active_bytes = 0;		//Number of active bytes

static greal_mem_ptr_t *
get_mem_ptr(void *ptr) {
	greal_mem_ptr_t *mp;

	if(!ptr) {
		return NULL;
	}
	
	mp = (greal_mem_ptr_t *)(ptr - offsetof(greal_mem_ptr_t, data));
	if(mp->sig != MEM_PTR_SIGNATURE) {
		return NULL;
	}

	return mp;
}

//Dump the statistics every 100 operations
static void 
dump_stats() {
#if defined(MEM_METRICS_STATS)		
	static uint64_t base_ms = 0;
	
	if(((_gr_alloc_count + _gr_free_count) % MEM_METRICS_STATS_FREQUENCY) == 0) {
		struct timespec the_time;
		uint64_t		the_ms;
		clock_gettime(CLOCK_REALTIME, &the_time);
		the_ms = (the_time.tv_sec * 1000) + (the_time.tv_nsec / 1000000);
		if(base_ms == 0) {
			base_ms = the_ms;
			the_ms = 0;
		} else {
			the_ms -= base_ms;
		}
		printf("MEM,%lld,%d,%d,%d,%d\n", the_ms, _gr_alloc_count, _gr_free_count, _gr_alloc_count - _gr_free_count, _gr_active_bytes);
	}
#endif
}

static void 
trace_operation(void *ptr, int requested, int isfree) {
#if defined(MEM_METRICS_TRACE)		
	greal_mem_ptr_t *mp;
		char 			 op;

	mp = get_mem_ptr(ptr);
	if(!mp) {
		return;
	}

	op = (isfree) ? '-' : '+';
	printf("MT,%c,%p,%d,%d\n", op, ptr, requested, mp->nbytes);
#endif
}

#define _GREAL_MALLOC_
void *
greal_malloc(size_t size) {
	greal_mem_ptr_t *mp;
	void 			*ret;

	if(!size) {
		return NULL;
	}

	mp = malloc(size + sizeof(*mp));
	if(!mp) {
		return NULL;
	}
	ret = (void *)mp + offsetof(greal_mem_ptr_t, data);

	mp->nbytes = size;
	mp->sig = MEM_PTR_SIGNATURE;

	_gr_alloc_count++;
	_gr_active_bytes += size;
	
	dump_stats();
	trace_operation(ret, size, 0);

	return ret;
}

#define _GREAL_FREE_
void 
greal_free(void *ptr) {
	greal_mem_ptr_t *mp;

	if(!ptr) {
		return;
	}

	trace_operation(ptr, 0, 1);
	
	mp = get_mem_ptr(ptr);
	if(!mp) {
		printf("Mis-matched free! \n");
		free(ptr);
	} else {
		_gr_free_count++;
		_gr_active_bytes -= mp->nbytes;
		free(mp);
	}

	dump_stats();
}

//
// To simplify tracing, we implement realloc/calloc/strdup in terms of greal_malloc/greal_free
// This could affect performance (realloc) but is a good sanity exercise
//

#define _GREAL_REALLOC_
void *
greal_realloc(void *ptr, size_t size) {
	greal_mem_ptr_t *mp;
	void 			*nptr;
	int 			old_size;
	
	//No size, act the same as greal_free()
	if(size == 0) {
		greal_free(ptr);
		return NULL;
	} 
	
	//No pointer, act the same as greal_malloc()
	if(!ptr) {
		return greal_malloc(size);
	} 

	//Retreive our cover header, if we don't have one, we're in trouble
	mp = get_mem_ptr(ptr);
	if(!mp) {
		return realloc(ptr, size);	
	} 
	
	//Shrinking, don't bother doing anything
	old_size = mp->nbytes;
	if(size <= old_size) {
		return ptr;
	}
	
	//Growing, allocate a new buffer and copy the data, drop the old pointer
	nptr = greal_malloc(size);
	if(!nptr) {
		return NULL;
	}
	memcpy(nptr, ptr, old_size);
	greal_free(ptr);
	
	return nptr;
}

#define _GREAL_CALLOC_
void *
greal_calloc(size_t nelem, size_t elsize) {
	void *ptr;
	int  nbytes;

	nbytes = nelem * elsize;
	ptr = greal_malloc(nbytes);
	if(!ptr) {
		return NULL;
	}
	memset(ptr, 0, nbytes);

	return ptr;
}

#define _GREAL_STRDUP_
char *
greal_strdup(const char *s1) {
	void *ptr;
	int  nbytes;
	
	if(!s1) {
		return NULL;
	}

	nbytes = strlen(s1) + 1;
	ptr = greal_malloc(nbytes);
	if(!ptr) {
		return NULL;
	}
	memcpy(ptr, s1, nbytes);

	return ptr;
}
#endif	// GREAL_MEMORY_METRICS

/**
 * Generic implementation on top of STD C calls
 */

#ifndef _GREAL_MALLOC_
void *
greal_malloc(size_t size) {
	return malloc(size);
}
#endif

#ifndef _GREAL_FREE_
void 
greal_free(void *ptr) {
	free(ptr);
}
#endif

#ifndef _GREAL_REALLOC_
void *
greal_realloc(void *ptr, size_t size) {
	return realloc(ptr, size);
}
#endif

#ifndef _GREAL_CALLOC_
void *
greal_calloc(size_t nelem, size_t elsize) {
	return calloc(nelem, elsize);
}
#endif

#ifndef _GREAL_STRDUP_
char *
greal_strdup(const char *s1) {
	//strdup doesn't account for null being OK as input
	if(!s1) {
		return NULL;
	}
	return strdup(s1);
}
#endif

