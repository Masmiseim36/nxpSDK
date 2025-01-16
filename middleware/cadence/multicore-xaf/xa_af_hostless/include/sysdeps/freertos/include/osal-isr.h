/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
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
#ifndef _OSAL_ISR_H
#define _OSAL_ISR_H

#include <xtensa_api.h>
#include <xtensa/hal.h>

typedef void xf_isr(void *arg);

/*
 * Set ISR and threaded handler for an IRQ.
 *
 * ISR runs in interrupt context. It should verify that IRQ came from
 * the hardware and clear interrupt request in the hardware if appropriate.
 * All other work should be done in the threaded handler.
 *
 * Threaded handler runs in a thread context. It can interact with the
 * hardware as well as with the rest of XAF.
 *
 * Either irq_handler or threaded_handler may be NULL if not needed.
 *
 * arg is an arbitrary pointer passed to both irq_handler and
 * threaded_handler.
 */
int __xf_set_threaded_irq_handler(int irq,
                                  xf_isr *irq_handler,
                                  xf_isr *threaded_handler,
                                  void *arg);

int __xf_unset_threaded_irq_handler(int irq);

static inline unsigned long __xf_disable_interrupts(void)
{
    return xthal_disable_interrupts();
}

static inline void __xf_restore_interrupts(unsigned long prev)
{
    xthal_restore_interrupts(prev);
}

static inline void __xf_enable_interrupt(int irq)
{
    xt_interrupt_enable(irq);
}

static inline void __xf_disable_interrupt(int irq)
{
    xt_interrupt_disable(irq);
}

#endif
