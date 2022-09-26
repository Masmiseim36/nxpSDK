/** @file mlan_11h.h
 *
 *  @brief This header file contains data structures and
 *  function declarations of 802.11h
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
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
extern void wlan_11h_init(mlan_adapter *adapter);

/** Initialize the 11h interface structure */
extern void wlan_11h_priv_init(mlan_private *pmpriv);


/** Check if radar detection is required on the specified channel */
extern t_bool wlan_11h_radar_detect_required(mlan_private *priv, t_u8 channel);



#endif /*_MLAN_11H_ */
