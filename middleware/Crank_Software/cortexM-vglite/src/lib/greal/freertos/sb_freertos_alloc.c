#include <gre/sdk/greal.h>



#if defined(GRE_TARGET_TOOLCHAIN_mplabx)
#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>

size_t xGetPtrSize(void *pv);

void *
greal_malloc(size_t size) {
	return pvPortMalloc(size);
}

void 
greal_free(void *ptr) {
	vPortFree(ptr);
}

void *
greal_realloc(void *ptr, size_t size) {
    size_t old_size = xGetPtrSize(ptr); 
    
    if (size == 0) {
        greal_free(ptr);
        return NULL;
    } else if (!ptr) {
        return greal_calloc(1, size);
    } else if (size <= old_size) {
        return ptr;
    } else {
        if((ptr) && (size > old_size)) {
            void *ptrNew = greal_calloc(1, size);
            if (ptrNew) {
                memcpy(ptrNew, ptr, old_size);
                greal_free(ptr);
            }
            return ptrNew;
        } else {
            return NULL;
        }
    }
}

void *
greal_calloc(size_t nelem, size_t elsize) {
    void *tmp_ptr = pvPortMalloc(nelem * elsize);
    memset(tmp_ptr, 0x0, nelem * elsize);
    return tmp_ptr; 
}

char *
greal_strdup(const char *s1) {
	int len; 
    void *new_string; 
    //strdup doesn't account for null being OK as input
	if(!s1) {
		return NULL;
	}
    len = strlen(s1);
    new_string = greal_calloc(1, len + 1);
    strcpy(new_string, s1);
	return new_string;
}
#endif