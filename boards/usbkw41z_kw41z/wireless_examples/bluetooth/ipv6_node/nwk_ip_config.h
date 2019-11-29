/*! *********************************************************************************
 * \addtogroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is a configuration file for the nwk_ip module.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _NWK_IP_CONFIG_H_
#define _NWK_IP_CONFIG_H_


#define ND_PIB                   .messages = ND_PIB_MSG_RS_SEND_MASK | ND_PIB_MSG_RA_RCV_MASK | ND_PIB_MSG_NS_SEND_MASK |ND_PIB_MSG_NA_RCV_MASK, \
                                 .options = ND_PIB_OPT_PREFIX_INFO_RCV_MASK | ND_PIB_OPT_AR_SEND_MASK | \
                                            ND_PIB_OPT_CTXT_RCV_MASK, \
                                 .constants = { \
                                    .maxRtrSolicitationDelay = ND6_MAX_RTR_SOLICITATION_DELAY, \
                                    .rtrSolicitationInterval = ND6_RTR_SOLICITATION_INTERVAL, \
                                    .maxRtrSolicitations = ND6_MAX_RTR_SOLICITATIONS, \
                                    .reachableTime = ND6_REACHABLE_TIME, \
                                    .retransTime = ND6_RETRANS_TIMER, \
                                    .delayFirstProbeTime = ND6_DELAY_FIRST_PROBE_TIME, \
                                    .maxRtrSolicitationInterval = ND6_MAX_RTR_SOLICITATIONS}, \
                                .devType =  gNd6DevHost_c,

#define GLOBAL_PREFIX_INIT \
        {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}                

#define GLOBAL_PREFIX_INIT_LEN  64
#endif /* _NWK_IP_CONFIG_H_ */
/*!
** @}
*/
