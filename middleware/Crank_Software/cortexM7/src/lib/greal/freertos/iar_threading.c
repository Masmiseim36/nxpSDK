#ifdef GRE_TARGET_TOOLCHAIN_iar

#include <FreeRTOS.h>
#include <yfuns.h>
#include "task.h"
#include <gre/sdk/greal.h>

void
__iar_system_Mtxinit(__iar_Rmtx* lock)    // Initialize a system lock
{
}

void
__iar_system_Mtxdst(__iar_Rmtx* lock)     // Destroy a system lock
{
}

void
__iar_system_Mtxlock(__iar_Rmtx* lock)    // Lock a system lock
{
    vTaskSuspendAll();
}

void
__iar_system_Mtxunlock(__iar_Rmtx* lock)  // Unlock a system lock
{
    xTaskResumeAll();
}


void
__iar_file_Mtxinit(__iar_Rmtx* lock)    // Initialize a system lock
{
}

void
__iar_file_Mtxdst(__iar_Rmtx* lock)     // Destroy a system lock
{
}

void
__iar_file_Mtxlock(__iar_Rmtx* lock)    // Lock a system lock
{
    vTaskSuspendAll();
}

void
__iar_file_Mtxunlock(__iar_Rmtx* lock)  // Unlock a system lock
{
   xTaskResumeAll();
}


long __lseek(int handle, long offset, int whence)
{
    return -1;
}
int __close(int handle)
{
    return 0;
}
int remove(const char* filename)
{
    return 0;
}
size_t __read(int handle, unsigned char * buffer, size_t size)
{
    return _LLIO_ERROR;
}
int __open(const char* filename, int mode)
{
    return -1;
}

size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    return _LLIO_ERROR;
}
#endif
