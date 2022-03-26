/** @file mlan_wmm.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of wmm functionalities
 *
 *  Copyright 2008-2022 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */

/****************************************************
Change log:
    10/24/2008: initial version
****************************************************/

#ifndef _MLAN_WMM_H_
#define _MLAN_WMM_H_

/**
 *  @brief This function gets the TID
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param ptr          A pointer to RA list table
 *
 *  @return 	    TID
 */
static INLINE int wlan_get_tid(pmlan_adapter pmadapter, raListTbl *ptr)
{
    pmlan_buffer mbuf;

    ENTER();
    mbuf = (pmlan_buffer)(void *)util_peek_list(pmadapter->pmoal_handle, &ptr->buf_head, MNULL, MNULL);
    LEAVE();

    return mbuf->priority;
}

/**
 *  @brief This function gets the length of a list
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param head         A pointer to mlan_list_head
 *
 *  @return 	    Length of list
 */
static INLINE int wlan_wmm_list_len(pmlan_adapter pmadapter, pmlan_list_head head)
{
    pmlan_linked_list pos;
    int count = 0;

    ENTER();

    pos = head->pnext;

    while (pos != (pmlan_linked_list)(void *)head)
    {
        ++count;
        pos = pos->pnext;
    }

    LEAVE();
    return count;
}

/** Initialize WMM */
t_void wlan_wmm_init(pmlan_adapter pmadapter);
/* Setup default queues */
void wlan_wmm_default_queue_priorities(pmlan_private priv);

/** Process WMM association request */
extern t_u32 wlan_wmm_process_association_req(pmlan_private priv,
                                              t_u8 **ppAssocBuf,
                                              IEEEtypes_WmmParameter_t *pWmmIE,
                                              IEEEtypes_HTCap_t *pHTCap);


/** WMM DELTS request command handler */
extern mlan_status wlan_cmd_wmm_delts_req(IN pmlan_private pmpriv, OUT HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf);

#endif /* !_MLAN_WMM_H_ */
