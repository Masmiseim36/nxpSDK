#ifndef _VG_LITE_OS_H
#define _VG_LITE_OS_H

#include <stdint.h>

#include "vg_lite_kernel.h"

typedef void * OsSemaphoreHandle_t;

/*!
@brief  Set the value in a task’s thread local storage array.
*/
vg_lite_error_t vg_lite_os_set_tls(void* tls);

/*!
@brief  Get the current task’s thread local storage array.
*/
void * vg_lite_os_get_tls( );

/*!
@brief  Memory allocate.
*/
void * vg_lite_os_malloc(uint32_t size);

/*!
@brief  Memory free.
*/
void vg_lite_os_free(void * memory);

/*!
@brief  Reset the value in a task’s thread local storage array.
*/
void vg_lite_os_reset_tls();


/*!
@brief  sleep a number of milliseconds.
*/
void vg_lite_os_sleep(uint32_t msec);

/*!
@brief  initialize the os parameters.
*/
vg_lite_error_t vg_lite_os_initialize();

/*!
@brief  deinitialize the os parameters.
*/
void vg_lite_os_deinitialize();

/*!
@brief  Mutex semaphore take.
*/
vg_lite_error_t vg_lite_os_lock();

/*!
@brief  Mutex semaphore give.
*/
vg_lite_error_t vg_lite_os_unlock();

/*!
@brief  Submit the current command buffer to the command queue.
*/
vg_lite_error_t vg_lite_os_submit(uint32_t physical, uint32_t offset, uint32_t size,  uint32_t * signal, uint32_t semaphore_id);

/*!
@brief  Wait for the current command buffer to be executed.
*/
vg_lite_error_t vg_lite_os_wait(uint32_t timeout, uint32_t * signal ,OsSemaphoreHandle_t semaphore);

/*!
@brief  Create semaphore.
*/
OsSemaphoreHandle_t vg_lite_os_create_semaphore(void);

/*!
@brief  Set semaphore.
*/
void vg_lite_os_set_semaphore(uint32_t id, OsSemaphoreHandle_t semaphore);

/*!
@brief  Get semaphore.
*/
OsSemaphoreHandle_t vg_lite_os_get_semaphore(uint32_t id);

/*!
@brief  Delete semaphore by id.
*/
void vg_lite_os_delete_semaphore_by_id(uint32_t id);

/*!
@brief  Release semaphore.
*/
void vg_lite_os_release_semaphore(OsSemaphoreHandle_t semaphore);

/*!
@brief  Take semaphore.
*/
long vg_lite_os_take_semaphore( OsSemaphoreHandle_t semaphore, uint32_t xTicksToWait);

/*!
@brief  Delete semaphore.
*/
void vg_lite_os_delete_semaphore(OsSemaphoreHandle_t semaphore);

/*!
@brief  IRQ Handler.
*/
void vg_lite_os_IRQHandler();

/*!
@brief  Wait until an interrupt from the VGLite graphics hardware has been received.
*/
int32_t vg_lite_os_wait_interrupt(uint32_t timeout, uint32_t mask, uint32_t * value);

#endif
