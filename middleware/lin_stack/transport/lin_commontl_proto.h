/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LIN_COMMONTL_PROTO_H
#define LIN_COMMONTL_PROTO_H

#include "lin.h"
#if (SUPPORT_TRANSPORT_LAYER == 1U)
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* PCI type */
#define PCI_SF 0x00U /*!< Single Frame */
#define PCI_FF 0x01U /*!< First Frame */
#define PCI_CF 0x02U /*!< Consecutive Frame */

/* COMMON FUNCTIONS */
/*
 * @brief Copy the specified raw data to the specified queue.
 *
 * @param[in] iii Interface name
 * @param[in] data Buffer for the data
 * @param[in] *queue Queue for containing data
 * @param[in] direction Direction
 * @return void
 */
void tl_put_raw(l_ifc_handle iii, const l_u8 *const data, lin_transport_layer_queue_t *queue, l_u8 direction);

/*
 * @brief Get the raw data from the specified queue to
 * the specified raw data
 *
 * @param[in] iii Interface name
 * @param[in] data Buffer for the data
 * @param[in] *queue Queue for containing data
 * @param[in] direction Direction
 * @return void
 */
void tl_get_raw(l_ifc_handle iii, l_u8 *const data, lin_transport_layer_queue_t *queue, l_u8 direction);

#endif /*end (SUPPORT_TRANSPORT_LAYER == 1U) */
#endif /* End of LIN_COMMONTL_PROTO_H */

/*! @} */

/******************************************************************************/
/* EOF */
/******************************************************************************/
