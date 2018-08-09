/*! *********************************************************************************
 * \addtogroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is a configuration file for the nwk_ip module.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _NWK_IP_CONFIG_H_
#define _NWK_IP_CONFIG_H_


#define ND_PIB                   .messages = ND_PIB_MSG_RS_RCV_MASK | ND_PIB_MSG_RA_SEND_MASK | ND_PIB_MSG_NS_RCV_MASK |ND_PIB_MSG_NA_SEND_MASK, \
                                 .options = ND_PIB_OPT_PREFIX_INFO_SEND_MASK | ND_PIB_OPT_AR_RCV_MASK | \
                                            ND_PIB_OPT_CTXT_SEND_MASK, \
                                 .constants = { \
                                    .maxRtrSolicitationDelay = ND6_MAX_RTR_SOLICITATION_DELAY, \
                                    .rtrSolicitationInterval = ND6_RTR_SOLICITATION_INTERVAL, \
                                    .maxRtrSolicitations = ND6_MAX_RTR_SOLICITATIONS, \
                                    .reachableTime = ND6_REACHABLE_TIME, \
                                    .retransTime = ND6_RETRANS_TIMER, \
                                    .delayFirstProbeTime = ND6_DELAY_FIRST_PROBE_TIME, \
                                    .maxRtrSolicitationInterval = ND6_MAX_RTR_SOLICITATIONS}, \
                                .devType =  gNd6DevBorderRouter_c,

#define GLOBAL_PREFIX_INIT \
        {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}                

#define GLOBAL_PREFIX_INIT_LEN  64
#endif /* _NWK_IP_CONFIG_H_ */
/*!
** @}
*/
