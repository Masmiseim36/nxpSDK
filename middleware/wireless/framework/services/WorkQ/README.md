# WorkQ

## Overview

This service implements a simple workqueue API. A **workqueue** is an object that uses a dedicated thread to process
work items, in a first in, first out manner. Each work item is processed by calling the handler registered in this work.
A workqueue is typically used by an ISR or a high priority thread to offload non-urgent processing to a lower priority
thread to limit impacting time sensitive activities.

A workqueue is composed by the following:
* A **queue** of work items that have been queued but not yet processed.
* A **thread** that processes the work items added to the queue.
* Information used internally by the service.

A workqueue is defined using the variable type `fwk_workq_t`. The workqueue thread must be defined by the user using
the `FWK_WORKQ_THREAD_DEFINE()` macro. The workqueue must be started before being able to submit work items to it, this
is done by calling `WORKQ_Start()`. The macro `FWK_WORKQ_THREAD()` can be used in second parameter to `WORKQ_Start()`
to provide a pointer to the workqueue thread definition.

## Work items

Any number of work items can be defined and added to a queue, the only limiting factor is the available RAM.
The work items are referenced by their memory address which must be provided by the user. Either static or dynamic
allocation are permitted.

A work item is composed of:
* A **handler** function, which is executed by the workqueue's thread when the work is processed. This function takes
a single parameter which is a pointer to the work item itself.
* Information used internally by the service.

A work item can be initialized using `FWK_WORK_DEFINE` to assign the function handler at compile time.
This can be done runtime too by changing the `handler` member from the `fwk_work_t` structure.

A work item can be **queued** by submitting it to a workqueue from any context (an ISR or a thread) using the
`WORKQ_SubmitToQueue()` function. Submitting a work item appends it to the workqueue's queue. The workqueue's queue is
processed in a FIFO manner, so the work item will be processed only when all preceding work items have been processed.
The work items are removed from the queue when processed, so if the user needs the work to run again, then it must be
submitted again. The time required before the work item is processed depends on the workqueue's thread priority
(which can be configured). If a work item is submitted while already queued, then the submission won't have any effect.

A work item enters the **running** state when it's being processed by the workqueue. If a work item is submitted while
already running, then the work item is added to the same queue that it is running on. This mechanism exists to avoid
re-entry issue.

It is **not** possible to cancel or delay a work item with this implementation of the workqueue.

If the handler function requires additional information about the work to perform, the work item can be embedded in a
larger data structure. Then, the handler function can use the `CONTAINER_OF` macro from `fwk_hal_macros.h` to retrieve
address of the whole data structure.

Once submitted, a work item **must not** be altered in any way by the user, otherwise the user is exposed to data
corruption and undefined behavior.

## System workqueue

The WorkQ service defines a workqueue known as the **system workqueue**, which is an instance of a workqueue available
to any software module running as part of the application. This workqueue is like any other workqueue, but the service
provides a simplified API which removes the need to specify the queue address, as it's handled by the system workqueue
implementation. The system workqueue is simple to use and should be convenient for most needs, it also allows to reduce
the memory footprint by allowing to share the same thread for any work item. But, keep in mind the work handlers should
avoid being blocking as it would delay other queued work items.

The system workqueue must be initialized before being used, this is done by calling `WORKQ_InitSysWorkQ()`.

Work items can be submitted to the system workqueue through the `WORKQ_Submit()` function.

The system worqueue stack size and thread priority can be configured with `CONFIG_FWK_SYSWORKQ_STACK_SIZE` and
`CONFIG_FWK_SYSWORKQ_PRIO` macros.

## Example of use

The following example demonstrates how to use the workqueue service using the system workqueue. In this example, an ISR
uses the system workqueue to print a counter value.

>Note: this example is very simple, but if the ISR fires while the previous work item is still queued
> (meaning waiting to be processed), then the new submission would be ignored, and only the latest value of the counter
> would be printed once. We kept the example simple just to illustrate how the workqueues can be used.

```C
#include "fwk_workq.h"
#include "fwl_hal_macros.h"
...

struct counter_info {
    fwk_wort_t work;
    uint32_t value;
} my_counter;

void counter_isr()
{
    ...
    my_counter.value++;
    WORKQ_Submit(&my_counter.work);
    ...
}

void print_counter(fwk_work_t *work_item)
{
    struct counter_info *counter = CONTAINER_OF(work_item, struct counter_info, work);
    PRINTF("Counter value: %u\n", counter->value);
}

void main()
{
    /* initialize the work initial state */
    my_counter.value = 0U;
    my_counter.work.handler = print_counter;
    /* start the system workqueue */
    int ret = WORKQ_InitSysWorkQ();
    assert(ret == 0);

    /* initialize the counter isr, start the OS, etc ... */
    ...
}
```
