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

#ifndef LIN_COMMON_PROTO_H
#define LIN_COMMON_PROTO_H

#include "lin_types.h"
#include "lin_cfg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAKE_UNCONDITIONAL_FRAME 0U   /*!< Make unconditional frame */
#define UPDATE_UNCONDITIONAL_FRAME 1U /*!< Update unconditional frame */

/*******************************************************************************
 * API
 ******************************************************************************/

#if (SUPPORT_MASTER_MODE == 1U)
/*
 * @brief Check to see if the current schedule table is needed to change
 *  or not. Used in tick callback function.
 *
 * @param[in] iii Interface name
 * @return void
 */
void lin_switch_sch_table(l_ifc_handle iii);

/*
 * @brief Checks if the associated flags to a sporadic frame have been updated
 *  and a transmission is needed
 *
 * @param[in] iii Interface name
 * @param[in] frm_id Index of frame
 * @return l_u8
 */
l_u8 lin_check_sporadic_update(l_ifc_handle iii, l_u8 frm_id);

#endif /* End (SUPPORT_MASTER_MODE == 1U) */

/*
 * @brief This function packs signals associated with unconditional frame
 *  into buffer.
 *
 * @param[in] iii Interface name
 * @param[in] id ID to process
 * @param[in] type Make or update
 * @return void
 */
void lin_process_uncd_frame(l_ifc_handle iii, l_u8 id, l_u8 type);

/*
 * @brief This function returns the index of a frame in frame list
 * or 0xFF if not found.
 *
 * @param[in] iii Interface name
 * @param[in] id ID to process
 * @return l_u8
 */
l_u8 lin_get_frame_index(l_ifc_handle iii, l_u8 id);

#endif /* LIN_COMMON_PROTO_H */

/******************************************************************************/
/* EOF */
/******************************************************************************/
