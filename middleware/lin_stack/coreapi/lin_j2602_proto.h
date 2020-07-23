/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
/*!
 * @file lin_j2602_proto.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012: Repeated include file
 * All the library headers are already guarded using macro.
 */

#ifndef LIN_J2602_PROTO_H
#define LIN_J2602_PROTO_H

#include "lin.h"

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Update date word status.
 *
 * @param[in] iii Interface name
 * @return void
 */
void lin_update_word_status_j2602(l_ifc_handle iii);

#if (SUPPORT_SLAVE_MODE == 1U)
#if (SUPPORT_PROTOCOL_J2602 == 1U)
/*!
 * @brief The function is to perform the update of error signal in J2602 system.
 *
 * @param[in] iii Interface name
 * @param[in] event_id Event id
 * @return void
 */
void lin_update_status_byte(l_ifc_handle iii, lin_lld_event_id_t event_id);

#endif /* End (SUPPORT_PROTOCOL_J2602 == 1U)*/
#endif /* End (SUPPORT_SLAVE_MODE == 1U) */
#endif /* ifndef LIN_J2602_PROTO_H */
/*! @} */
/******************************************************************************/
/* EOF */
/******************************************************************************/
