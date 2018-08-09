/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
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
/*!
**  @addtogroup thread_app_config
**  @{
*/
#ifndef _APP_MAC_CONFIG_H_
#define _APP_MAC_CONFIG_H_

/*!=================================================================================================
\file       app_mac_config.h
\brief      This file is a for mac/phy configuration of all thread demo applications.
            If it is required to configure just one application use the appllication config. file.
            Ex: for thread router application use thread_router_config.h
==================================================================================================*/


/*!=================================================================================================
    CONFIG Mac/Phy
==================================================================================================*/
/*! FCSI uses MAC commands */
#ifndef gTHCI_IncludeMacCommands_c
  #define gTHCI_IncludeMacCommands_c 0
#endif
/*! Number of PANs that can be managed */
#ifndef gMpmMaxPANs_c
    #define gMpmMaxPANs_c       1
#endif
/*! Number of MAC instances */
#ifndef gMacInstancesCnt_c
    #define gMacInstancesCnt_c  1
#endif
/*! Number of MAC Key Table entries (see IEEE 802.15.4 spec) */
#ifndef gNumKeyTableEntries_c
    #define gNumKeyTableEntries_c        5  /* MLE_KEY_DESCRIPTOR_TABLE_SIZE +
                                              one Key for KEK  (see Meshcop)
                                              one Key for Discovery (MAC key ID mode 2) */
#endif
/*! Number of MAC KeyId Lookup list entries (see IEEE 802.15.4 spec)*/
#ifndef gNumKeyIdLookupListEntries_c
    #define gNumKeyIdLookupListEntries_c 1
#endif
/*! Number of MAC Key Device list entries (see IEEE 802.15.4 spec).
 *  This should be equal with the maximum number of neighbors plus 2 entries(1 for the KEK and 1 for
 *  the Discovery MAC security configuration). */
#ifndef gNumKeyDeviceListEntries_c
    #define gNumKeyDeviceListEntries_c   (THR_MAX_NEIGHBORS + 2)
#endif
/*! Number of MAC Key Usage list entries (see IEEE 802.15.4 spec) */
#ifndef gNumKeyUsageListEntries_c
    #define gNumKeyUsageListEntries_c    2
#endif
/*! Number of MAC Device Table entries (see IEEE 802.15.4 spec) */
#ifndef gNumDeviceTableEntries_c
    #define gNumDeviceTableEntries_c     ( ((gNumKeyTableEntries_c-2) * gNumKeyDeviceListEntries_c) + \
                                          1 +   /*one entry for KEK (MAC key ID mode 0 )*/  \
                                          1  )  /*one entry for Discovery (MAC key ID mode 2) */
#endif
/*! The MAC stack size */
#ifndef gMacTaskStackSize_c
    #define gMacTaskStackSize_c 1200
#endif
/*! The MAC memory pool ID */
#ifndef gMacPoolId_d
    #define  gMacPoolId_d 1
#endif
/*! The PHY memory pool ID */
#ifndef gPhyPoolId_d
  #define    gPhyPoolId_d 1
#endif

/*! The number of children that can be handled by PHY neighbor table */
#ifndef gPhyNeighborEntries_d
#define gPhyNeighborEntries_d          THR_MAX_SLEEPY_ED_NEIGHBORS
#endif

/*! The neighbor table size from PHY. This table is used to generate the ACK with Frame Pending bit
    set. When a poll request is received from a sleepy child which is not in this table, an ACK with
    Frame Pending bit set will be automatically generated. A neighbor requires 2 entries, one for
    the short address and another one for the extended address. This feature is no longer required
    for Thread.
*/
#ifndef gPhyNeighborTableSize_d
    #define gPhyNeighborTableSize_d       0
#endif

/* If gPhyRxPBTransferThereshold_d is enabled, the packet is transfered in RAM
            when the specified threshold is reached. Phy optimisation for thread*/
#ifndef gPhyRxPBTransferThereshold_d
    #define gPhyRxPBTransferThereshold_d  (1)
#endif

/* Enable RF Coexistence */
#ifndef gMWS_UseCoexistence_d
    #define gMWS_UseCoexistence_d 0
#endif

#endif /* _APP_MAC_CONFIG_H_   */
