/*
* The Clear BSD License
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
