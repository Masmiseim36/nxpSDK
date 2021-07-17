/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************************************
 * xf-component.h
 *
 * Xtensa processing framework component definition
 *******************************************************************************/

#ifndef __XF_H
#error "xf-component.h mustn't be included directly"
#endif

/*******************************************************************************
 * Types definitions
 ******************************************************************************/

/* ...component descriptor (base structure) */
typedef struct xf_component
{
    /* ...scheduler node */
    xf_task_t               task;

    /* ...component id */
    UWORD32                     id;

    /* ...component processing priority */
    UWORD32                 priority;

    /* ...message-processing function */
    int                   (*entry)(struct xf_component *, xf_message_t *);

    /* ...component destructor function */
    int                   (*exit)(struct xf_component *, xf_message_t *);
    
#ifndef XA_DISABLE_EVENT
    /* ...component error handler function */
    int                   (*error_handler)(struct xf_component *, XA_ERRORCODE);
#endif
}   xf_component_t;

/*******************************************************************************
 * Helpers
 ******************************************************************************/

/* ...return core-id of the component */
static inline UWORD32 xf_component_core(xf_component_t *component)
{
    return XF_PORT_CORE(component->id);
}

/* ...schedule component execution */
#define xf_component_schedule(c, dts)                                       \
({                                                                          \
    xf_sched_t *__sched = &XF_CORE_DATA(xf_component_core((c)))->sched;     \
    xf_sched_put(__sched, &(c)->task, (dts)); \
    xf_ipi_resume_dsp_isr(xf_component_core(c));                            \
})

/* ...cancel component execution */
#define xf_component_cancel(c)                                          \
({                                                                      \
    xf_sched_t *__sched = &XF_CORE_DATA(xf_component_core((c)))->sched; \
    xf_sched_cancel(__sched, &(c)->task);                               \
})
    
/*******************************************************************************
 * API functions
 ******************************************************************************/

/* ...component factory */
extern xf_component_t * xf_component_factory(UWORD32 core, xf_id_t id, UWORD32 length);
