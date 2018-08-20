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

#ifndef _zps_nwk_slist_h_
#define _zps_nwk_slist_h_

/*!
\file       zps_nwk_slist.h
\brief      ZPS NWK Single Linked List
*/
     
#ifndef MODULE
#define MODULE /* no modifier implies global */
#endif

/***********************/
/**** INCLUDE FILES ****/
/***********************/

#include "jendefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************/
/**** MACROS/DEFINES ****/
/************************/

#define ZPS_NWK_SLIST_LIMBO_NODE ((zps_tsNwkSlistNode *)0xFFFFFFFF)

/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/

/**
 * ZPS NWK Single linked list node
 * @ingroup g_zps_nwk_slist
 * @note
 * Based on Jeneric JennicStdLib but without the unnecessary stuff
 */
typedef struct zps_tsNwkSlistNode_tag zps_tsNwkSlistNode;
struct zps_tsNwkSlistNode_tag
{
    zps_tsNwkSlistNode *psNext;
};

/**
 * ZPS NWK Single linked list
 * @ingroup g_zps_nwk_slist
 * @note
 * Based on Jeneric JennicStdLib but without the unnecessary stuff
 */
typedef struct zps_tsNwkSlist_tag
{
    zps_tsNwkSlistNode *psHead;     /**< Head node in list (NULL if empty) */
    zps_tsNwkSlistNode *psTail;     /**< Tail node in list (NULL if empty) */
} zps_tsNwkSlist;

/****************************/
/**** IMPORTED FUNCTIONS ****/
/****************************/

/**************************/
/**** MODULE VARIABLES ****/
/**************************/

/****************************/
/**** EXPORTED FUNCTIONS ****/
/****************************/

MODULE void
zps_vNwkSlistInit(zps_tsNwkSlist *psList);

MODULE void
zps_vNwkSlistAddToHead(zps_tsNwkSlist *psList,
                       zps_tsNwkSlistNode *psNode);

MODULE void
zps_vNwkSlistAddToTail(zps_tsNwkSlist *psList,
                       zps_tsNwkSlistNode *psNode);

MODULE void
zps_vNwkSlistInsertAfter(zps_tsNwkSlist *psList,
                         zps_tsNwkSlistNode *psCurrentNode,
                         zps_tsNwkSlistNode *psNewNode);

MODULE zps_tsNwkSlistNode *
zps_psNwkSlistRemoveFromHead(zps_tsNwkSlist *psList);

MODULE zps_tsNwkSlistNode *
zps_psNwkSlistRemove(zps_tsNwkSlist *psList,
                     zps_tsNwkSlistNode *psNode);

MODULE void
zps_vNwkSlistMakeLimboNode(zps_tsNwkSlistNode *psNode);

MODULE bool_t
zps_bNwkSlistIsLimboNode(zps_tsNwkSlistNode *psNode);

#ifdef __cplusplus
};
#endif

#endif /* _mac_prv_h_ */

/* End of file $Id: zps_nwk_slist.h 77413 2016-02-26 15:34:22Z nxp29772 $ *******************************************/
