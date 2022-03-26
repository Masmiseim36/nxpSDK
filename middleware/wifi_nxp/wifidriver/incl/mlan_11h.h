/** @file mlan_11h.h
 *
 *  @brief This header file contains data structures and
 *  function declarations of 802.11h
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

/*************************************************************
Change Log:
    03/26/2009: initial creation
*************************************************************/

#ifndef _MLAN_11H_
#define _MLAN_11H_

/** 11H OID bitmasks */
#define ENABLE_11H_MASK       MBIT(0)
#define MASTER_RADAR_DET_MASK MBIT(1)
#define SLAVE_RADAR_DET_MASK  MBIT(2)

/** DFS Master Radar Detect global enable */
#define DFS_MASTER_RADAR_DETECT_EN (MTRUE)
/** DFS Slave Radar Detect global enable */
#define DFS_SLAVE_RADAR_DETECT_EN (MFALSE)

/**
 *  11H APIs
 */


/** Initialize the 11h device structure */
extern void wlan_11h_init(mlan_adapter *pmadapter);

/** Initialize the 11h interface structure */
extern void wlan_11h_priv_init(mlan_private *pmpriv);


/** Check if radar detection is required on the specified channel */
extern t_bool wlan_11h_radar_detect_required(mlan_private *priv, t_u8 channel);



#endif /*_MLAN_11H_ */
