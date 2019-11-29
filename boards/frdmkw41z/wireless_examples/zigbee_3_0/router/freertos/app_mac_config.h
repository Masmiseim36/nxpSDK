/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_MAC_CONFIG_H_
#define _APP_MAC_CONFIG_H_

/*!
\file       app_mac_config.h
\brief      This file is a for mac/phy configuration of all zigbee demo applications.
            If it is required to configure just one application use the appllication config. file.
            Ex: for zigbee router application use the specific config.h
*/
/*!=================================================================================================
    CONFIG Mac/Phy
==================================================================================================*/
/*! For CPU_MKW41Z512VHT4 enable ZigBee PRO customizations Cortex M0+*/         
#ifndef gMacFeatureSet_d
  #define gMacFeatureSet_d gMacFeatureSet_ZPM0_d
#endif
    
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

/*! The MAC stack size */
#ifndef gMacTaskStackSize_c
    #define gMacTaskStackSize_c 1300
#endif
/*! The MAC memory pool ID */
#ifndef gMacPoolId_d
    #define  gMacPoolId_d 0
#endif
/*! The PHY memory pool ID */
#ifndef gPhyPoolId_d
  #define    gPhyPoolId_d 0
#endif

/* Enable RF Coexistence */
#ifndef gMWS_UseCoexistence_d
    #define gMWS_UseCoexistence_d 0
#endif

#endif /* _APP_MAC_CONFIG_H_   */
