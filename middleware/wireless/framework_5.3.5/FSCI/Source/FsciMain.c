/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the main source file for the FSCI module
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "EmbeddedTypes.h"
#include "FsciInterface.h"
#include "FsciCommands.h"
#include "FsciCommunication.h"
#include "MemManager.h"
#include "Messaging.h"

#if gFsciIncluded_c
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
gFsciOpGroup_t *FSCI_GetReqOpGroup(opGroup_t OG, uint8_t fsciInterface);


/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t  mFsciSrcInterface = mFsciInvalidInterface_c;
gFsciOpGroup_t  gReqOpGroupTable[gFsciMaxOpGroups_c];
uint8_t         gNumberOfOG = 0;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief   This function initializes the FSCI module
*
* \param[in] argument pointer to a initialization structure
*
********************************************************************************** */
void FSCI_Init( void* argument  )
{
    /* Initialize the communication interface */
    FSCI_commInit( argument );
}

/*! *********************************************************************************
* \brief   Sends a message to the FSCI module
*
* \param[in] pPacket a pointer to the message payload
* \param[in] fsciInterface the interface on which the data was received
*
* \return the status of the operation
*
********************************************************************************** */
gFsciStatus_t FSCI_ProcessRxPkt (clientPacket_t* pPacket, uint32_t fsciInterface)
{
    gFsciStatus_t status = gFsciSuccess_c;   
    
    switch(pPacket->structured.header.opGroup)
    {
#if gFsciLenHas2Bytes_c
    case gFSCI_ReservedOpGroup_c:
        pPacket->structured.header.opGroup = gFSCI_ReservedOpGroup_c;
        pPacket->structured.header.opCode = 0x02;
        pPacket->structured.header.len = 0x00;
        FSCI_transmitFormatedPacket( pPacket, fsciInterface );
        break;
#endif
    case gFSCI_ReqOpcodeGroup_c:
        fsciMsgHandler(pPacket, fsciInterface);
        break;
    case gFSCI_CnfOpcodeGroup_c:
        MEM_BufferFree(pPacket);
        break;        
    default:
        status = FSCI_CallRegisteredFunc(pPacket->structured.header.opGroup,
                                         pPacket,
                                         fsciInterface);
        break;
    }

    if ( (gFsciSuccess_c != status) &&  (gFsciSAPHook_c != status) )
    {
        FSCI_Error(status, fsciInterface);
        MEM_BufferFree(pPacket);
    }
    return status;
}
#endif

/*! *********************************************************************************
* \brief   This calls the handler for a specific OpGroup
*
* \param[in] opGroup the OpGroup of the message
* \param[in] pData a pointer to the message payload
* \param[in] param a pointer to a parameter to be passed to the handler function
* \param[in] fsciInterface the interface on which the data should be printed
*
* \return Returns the status of the call process.
*
********************************************************************************** */
gFsciStatus_t FSCI_Monitor (opGroup_t opGroup, void *pData, void* param, uint32_t fsciInterface)
{
    gFsciStatus_t status = gFsciSuccess_c;
#if gFsciIncluded_c
    gFsciOpGroup_t *p;

    /* Skip if the request originated in FSCI */
    if( mFsciSrcInterface == fsciInterface )
    {
        mFsciSrcInterface = mFsciInvalidInterface_c;
    }
    else
    {
        /* Search for the registered calback function */
        p = FSCI_GetReqOpGroup(opGroup, fsciInterface);
        
        if ( NULL == p ) /* The OpGroup was not found */
        {
            status = gFsciSAPInfoNotFound_c;
        }
        else if ( gFsciDisableMode_c == p->mode) /* The SAP is disabed */
        {
            status = gFsciSAPDisabled_c;
        }
        else
        {
            
            /* Execute request */
            if ( p->pfOpGroupHandler )
            {
                p->pfOpGroupHandler( pData, param, fsciInterface );
            }
            
            if ( gFsciHookMode_c == p->mode ) /* The SAP operates in Hook mode */
            {
                status = gFsciSAPHook_c;
            }
        }
    }
#endif /* gFsciIncluded_c */
    return status;
}

#if gFsciIncluded_c
/*! *********************************************************************************
* \brief   This calls the handler for a specific OpGroup
*
* \param[in] opGroup the OpGroup of the message
* \param[in] pData a pointer to the message payload
* \param[in] fsciInterface the interface on which the data should be printed
*
* \return Returns the status of the call process.
*
********************************************************************************** */
gFsciStatus_t FSCI_CallRegisteredFunc( opGroup_t opGroup, void *pData, uint32_t fsciInterface )
{
    gFsciOpGroup_t *pOGtable;
    gFsciStatus_t status = gFsciSuccess_c;
    extern uint8_t mFsciErrorReported;

    mFsciErrorReported = FALSE;

    /* Search for the OpGroup */
    pOGtable = FSCI_GetReqOpGroup( opGroup, fsciInterface );

    if ( NULL == pOGtable )  /* The OpGroup was not found */
    {
        status = gFsciUnknownOpcodeGroup_c;
    }
    else if ( gFsciDisableMode_c == pOGtable->mode ) /* The SAP is disabed */
    {
        status = gFsciSAPDisabled_c;
    }
    else
    {
        /* Execute request */
        mFsciSrcInterface = fsciInterface;
        if ( pOGtable->pfOpGroupHandler )
        {
            pOGtable->pfOpGroupHandler( pData, pOGtable->param, fsciInterface );
        }
        mFsciSrcInterface = mFsciInvalidInterface_c;
    }
 
    return status;
}
#endif /* gFsciIncluded_c */
/*! *********************************************************************************
* \brief   This function registers the handler function for a specific OpGroup
*
* \param[in] OG the OpGroup to be registered
* \param[in] mode the operating mode for the OpGroup
* \param[in] pfHandler pointer to the message handler function
* \param[in] param a pointer to a parameter to be passed to the handler function
* \param[in] fsciInterface the interface on which the data should be printed
*
* \return Returns the status of the registration process.
*
********************************************************************************** */
gFsciStatus_t FSCI_RegisterOpGroup (opGroup_t opGroup,
                                    gFsciMode_t mode,
                                    pfMsgHandler_t pfHandler,
                                    void* param,
                                    uint32_t fsciInterface)
{
    gFsciStatus_t status = gFsciSuccess_c;
#if gFsciIncluded_c
    if ( (FSCI_GetReqOpGroup(opGroup, fsciInterface)) || (gNumberOfOG >= gFsciMaxOpGroups_c) || 
         (fsciInterface >= gFsciMaxInterfaces_c) )
    {
        status = gFsciError_c;
    }
    else
    {
        gReqOpGroupTable[gNumberOfOG].opGroup = opGroup;
        gReqOpGroupTable[gNumberOfOG].mode = mode;
        gReqOpGroupTable[gNumberOfOG].pfOpGroupHandler = pfHandler;
        gReqOpGroupTable[gNumberOfOG].param = param;
        gReqOpGroupTable[gNumberOfOG].fsciInterfaceId = fsciInterface;
        gNumberOfOG++;
    }
#endif /* gFsciIncluded_c */
    return status;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
#if gFsciIncluded_c
/*! *********************************************************************************
* \brief   This function searches for an OpGroup in the gReqOpGroupTable
*
* \param[in]  OG the OpGroup to be found
* \param[in]  intf the interface on which the handler was registered
*
* \return  Returns a pointer to the corresponding OpGroup entry.
*          If no entry was found the function returns NULL
*
********************************************************************************** */
gFsciOpGroup_t *FSCI_GetReqOpGroup( opGroup_t OG, uint8_t fsciInterface )
{
    uint32_t index;
    gFsciOpGroup_t *p = NULL;

    for ( index = 0; index < gNumberOfOG; index++ )
    {
        if ( (gReqOpGroupTable[index].opGroup == OG) &&
             (gReqOpGroupTable[index].fsciInterfaceId == fsciInterface) )
        {
            p = (gFsciOpGroup_t*)&gReqOpGroupTable[index];
            break;
        }
    }

    return p;
}
#endif /* gFsciIncluded_c */