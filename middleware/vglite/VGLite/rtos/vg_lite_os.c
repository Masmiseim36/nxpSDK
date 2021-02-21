#include "vg_lite_os.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "vg_lite_hw.h"
#include "vg_lite_hal.h"

/* If bit31 is activated this indicates a bus error */
#define IS_AXI_BUS_ERR(x) ((x)&(1U << 31))

#define ISR_WAIT_TIME   0x1FFFF
#define MAX_MUTEX_TIME  100
#define TASK_WAIT_TIME  20

/* command queue task parameter */
#define QUEUE_TASK_NAME  "queue_task"
#ifndef QUEUE_TASK_PRIO
#define QUEUE_TASK_PRIO  (configMAX_PRIORITIES - 1)
#endif /* QUEUE_TASK_PRIO */
#define QUEUE_TASK_SIZE  1024
#define QUEUE_LENGTH     8
#define MAX_QUEUE_WAIT_NUM  10

typedef struct vg_lite_queue{
    uint32_t  cmd_physical;
    uint32_t  cmd_offset;
    uint32_t  cmd_size;
    uint32_t* signal;
    uint32_t  semaphore_id;
}
vg_lite_queue_t;

typedef struct vg_lite_os{
    TaskHandle_t     task_hanlde;
    QueueHandle_t    queue_handle;
}
vg_lite_os_t;

static SemaphoreHandle_t mutex;
static vg_lite_os_t os_obj = {0};

SemaphoreHandle_t semaphore[TASK_LENGTH] = {NULL};
SemaphoreHandle_t command_semaphore = NULL;
SemaphoreHandle_t int_queue;
volatile uint32_t int_flags;

void __attribute__((weak)) vg_lite_bus_error_handler()
{
    /*
     * Default implementation of the bus error handler does nothing. Application
     * should override this handler if it requires to be notified when a bus
     * error event occurs.
     */
     return;
}

/* command queue function */
void command_queue(void * parameters)
{
    vg_lite_queue_t* peek_queue;
    uint32_t  even_got;
    BaseType_t  ret;

    os_obj.queue_handle = xQueueCreate(QUEUE_LENGTH, sizeof(vg_lite_queue_t * ));
    if(os_obj.queue_handle == NULL)
    {
        /*command queue create fail, delete queue task */
        vTaskDelete(NULL);
    }

    command_semaphore = xSemaphoreCreateCounting(30,0);
    while(1)
    {
        even_got = 0;
        if (xSemaphoreTake((SemaphoreHandle_t)command_semaphore, portMAX_DELAY) == pdTRUE) {
            if(uxQueueMessagesWaiting(os_obj.queue_handle))
            {
                ret = xQueueReceive(os_obj.queue_handle,
                                  (void*) &peek_queue,
                                  TASK_WAIT_TIME/portTICK_PERIOD_MS);
                if(ret == pdPASS)
                {
                    vg_lite_hal_poke(VG_LITE_HW_CMDBUF_ADDRESS, peek_queue->cmd_physical + peek_queue->cmd_offset);
                    vg_lite_hal_poke(VG_LITE_HW_CMDBUF_SIZE, (peek_queue->cmd_size +7)/8 );

                    if(vg_lite_hal_wait_interrupt(ISR_WAIT_TIME, (uint32_t)~0, &even_got))
                        *(peek_queue->signal) = VG_LITE_HW_FINISHED;
                    else
                        /* wait timeout */
                        *(peek_queue->signal) = VG_LITE_IDLE;
                    if(semaphore[peek_queue->semaphore_id]){
                        xSemaphoreGive(semaphore[peek_queue->semaphore_id]);
                    }

                    vg_lite_os_free((void *) peek_queue);
                }
            }
        }
    }
}

OsSemaphoreHandle_t vg_lite_os_create_semaphore(void)
{
    return (OsSemaphoreHandle_t)xSemaphoreCreateBinary();
}

void vg_lite_os_release_semaphore(OsSemaphoreHandle_t semaphore)
{
    xSemaphoreGive((SemaphoreHandle_t)semaphore);
}

long vg_lite_os_take_semaphore( OsSemaphoreHandle_t semaphore, uint32_t xTicksToWait)
{
    return xSemaphoreTake((SemaphoreHandle_t)semaphore, xTicksToWait);
}

void vg_lite_os_delete_semaphore(OsSemaphoreHandle_t semaphore)
{
    vSemaphoreDelete((SemaphoreHandle_t)semaphore);
}

void vg_lite_os_set_semaphore(uint32_t id, OsSemaphoreHandle_t Semaphore)
{
    semaphore[id] = (SemaphoreHandle_t)Semaphore;
}

OsSemaphoreHandle_t vg_lite_os_get_semaphore(uint32_t id)
{
    return (OsSemaphoreHandle_t)semaphore[id];
}

void vg_lite_os_delete_semaphore_by_id(uint32_t id)
{
    if(semaphore[id]){
        vSemaphoreDelete(semaphore[id]);
        semaphore[id] = NULL;
    }
}

vg_lite_error_t vg_lite_os_set_tls(void* tls)
{
    if(tls == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    vTaskSetThreadLocalStoragePointer(NULL, 0, tls);
    return VG_LITE_SUCCESS;
}

void * vg_lite_os_get_tls( )
{
    return pvTaskGetThreadLocalStoragePointer(NULL, 0);
}

void * vg_lite_os_malloc(uint32_t size)
{
    return pvPortMalloc(size);
}

void vg_lite_os_free(void * memory)
{
    vPortFree(memory);
}

void vg_lite_os_reset_tls()
{
    vTaskSetThreadLocalStoragePointer(NULL, 0, NULL);
}

void vg_lite_os_sleep(uint32_t msec)
{
    vTaskDelay((configTICK_RATE_HZ * msec + 999)/ 1000);
}

vg_lite_error_t vg_lite_os_initialize(void)
{
    static int task_number = 0;
    BaseType_t ret;

    int_queue = xSemaphoreCreateBinary();
    int_flags = 0;

    if(mutex == NULL)
    {
        mutex = xSemaphoreCreateMutex();
        if(mutex == NULL)
            return VG_LITE_MULTI_THREAD_FAIL;
    }

    if(task_number == 0)
    {
        if(xSemaphoreTake(mutex, TASK_WAIT_TIME/portTICK_PERIOD_MS) == pdTRUE)
        {
            if(os_obj.task_hanlde == NULL)
            {
                ret = xTaskCreate(command_queue, QUEUE_TASK_NAME, QUEUE_TASK_SIZE, NULL, QUEUE_TASK_PRIO, &os_obj.task_hanlde);
                if(ret != pdPASS)
                {
                    /* command queue task create fail */
                    xSemaphoreGive(mutex);
                    return VG_LITE_MULTI_THREAD_FAIL;
                }
            }
            task_number++;
            xSemaphoreGive(mutex);
            return VG_LITE_SUCCESS;
        }
    }
    return VG_LITE_SUCCESS;
}

void vg_lite_os_deinitialize(void)
{
    /* TODO: Remove clock. */
    vSemaphoreDelete(mutex);
    mutex = 0;
    vSemaphoreDelete(int_queue);
    /* TODO: Remove power. */
}

vg_lite_error_t vg_lite_os_lock()
{
    if(mutex == NULL)
        return VG_LITE_NOT_SUPPORT;

    if(xSemaphoreTake(mutex, MAX_MUTEX_TIME/portTICK_PERIOD_MS) != pdTRUE)
        return VG_LITE_MULTI_THREAD_FAIL;

    return VG_LITE_SUCCESS;
}

vg_lite_error_t vg_lite_os_unlock()
{
    if(xSemaphoreGive(mutex) != pdTRUE)
        return VG_LITE_MULTI_THREAD_FAIL;

    return VG_LITE_SUCCESS;
}

vg_lite_error_t vg_lite_os_submit(uint32_t physical, uint32_t offset, uint32_t size,  uint32_t * signal, uint32_t semaphore_id)
{
    vg_lite_queue_t* queue_node;

    if(os_obj.queue_handle == NULL)
        return VG_LITE_NOT_SUPPORT;

    queue_node = (vg_lite_queue_t*) vg_lite_os_malloc(sizeof(vg_lite_queue_t));
    if(queue_node == NULL)
        return VG_LITE_MULTI_THREAD_FAIL;

    queue_node->cmd_physical = physical;
    queue_node->cmd_offset = offset;
    queue_node->cmd_size = size;
    queue_node->signal = signal;
    queue_node->semaphore_id = semaphore_id;

    if(xQueueSend(os_obj.queue_handle,
                  (void *) &queue_node,
                  ISR_WAIT_TIME/portTICK_PERIOD_MS) != pdTRUE)
        return VG_LITE_MULTI_THREAD_FAIL;

    /* Current command buffer has been sent to the command queue. */
    *signal = VG_LITE_IN_QUEUE;

    if (xSemaphoreTake((SemaphoreHandle_t)semaphore[semaphore_id], portMAX_DELAY) == pdTRUE) {
        if(xSemaphoreGive(command_semaphore) != pdTRUE)
            return VG_LITE_MULTI_THREAD_FAIL;
        return VG_LITE_SUCCESS;
    }

    return VG_LITE_MULTI_THREAD_FAIL;
}

vg_lite_error_t vg_lite_os_wait(uint32_t timeout, uint32_t * signal ,OsSemaphoreHandle_t semaphore)
{
    if(semaphore) {
        if (xSemaphoreTake((SemaphoreHandle_t)semaphore, portMAX_DELAY) == pdTRUE) {
            if(*signal == VG_LITE_HW_FINISHED){
                xSemaphoreGive((SemaphoreHandle_t)semaphore);
                return VG_LITE_SUCCESS;
            }
            else{
                xSemaphoreGive((SemaphoreHandle_t)semaphore);
                return VG_LITE_TIMEOUT;
            }
        }
    }
    return VG_LITE_TIMEOUT;
}

void vg_lite_os_IRQHandler(void)
{
    uint32_t flags = vg_lite_hal_peek(VG_LITE_INTR_STATUS);
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (flags) {
        /* Combine with current interrupt flags. */
        int_flags |= flags;

        /* Wake up any waiters. */
        if(int_queue){
            xSemaphoreGiveFromISR(int_queue, &xHigherPriorityTaskWoken);
            if(xHigherPriorityTaskWoken != pdFALSE )
            {
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }
    }
}

int32_t vg_lite_os_wait_interrupt(uint32_t timeout, uint32_t mask, uint32_t * value)
{
#if _BAREMETAL
    uint32_t int_status=0;
    int_status = vg_lite_hal_peek(VG_LITE_INTR_STATUS);
    (void)value;

    while (int_status==0){
        int_status = vg_lite_hal_peek(VG_LITE_INTR_STATUS);
        usleep(1);
    }

    if (IS_AXI_BUS_ERR(*value))
    {
        vg_lite_bus_error_handler();
    }
    return 1;
#else /*for rt500*/
    if(int_queue) {
        if (xSemaphoreTake(int_queue, timeout / portTICK_PERIOD_MS) == pdTRUE) {
            if (value != NULL) {
               *value = int_flags & mask;
                if (IS_AXI_BUS_ERR(*value))
                {
                    vg_lite_bus_error_handler();
                }
            }
            int_flags = 0;

            return 1;
        }
    }
    return 0;
#endif
}
