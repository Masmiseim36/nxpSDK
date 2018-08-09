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
     
/*!=============================================================================
\file       zcl_buffer.c
\brief      ZCL Buffer Stack Wrapper.
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>

#include "zcl.h"
#include "zcl_common.h"
#include "zcl_internal.h"
#include "pdum_apl.h"
#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Imported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       hZCL_AllocateAPduInstance
 **
 ** DESCRIPTION:
 ** ZCL wrapper to get a stack buffer
 **
 ** PARAMETERS:                 Name                      Usage
 ** Nothing
 **
 ** RETURN:
 ** PDUM_thAPduInstance
 **
 ****************************************************************************/

PUBLIC PDUM_thAPduInstance hZCL_AllocateAPduInstance(void)
{
    tsZCL_CallBackEvent sZCL_CallBackEvent;

    PDUM_thAPduInstance pdum_tsAPduInstance;
    
    pdum_tsAPduInstance = PDUM_hAPduAllocateAPduInstance(psZCL_Common->hZCL_APdu);

    if(pdum_tsAPduInstance==PDUM_INVALID_HANDLE)
    {
        // fill in non-attribute specific values in callback event structure
        FLib_MemSet(&sZCL_CallBackEvent, 0, sizeof(tsZCL_CallBackEvent));
        sZCL_CallBackEvent.eEventType = E_ZCL_CBET_ERROR;
        sZCL_CallBackEvent.eZCL_Status = E_ZCL_ERR_ZBUFFER_FAIL;
        // general c/b
        psZCL_Common->pfZCLinternalCallBackFunction(&sZCL_CallBackEvent);
    }

    return pdum_tsAPduInstance;
}

/****************************************************************************
 **
 ** NAME:       hZCL_GetBufferPoolHandle
 **
 ** DESCRIPTION:
 ** Returns number of ZCL buffer pool handle
 **
 ** PARAMETERS:               Name                    Usage
 ** none
 **
 ** RETURN:
 ** PDUM_thAPdu
 **
 ****************************************************************************/

PUBLIC PDUM_thAPdu hZCL_GetBufferPoolHandle(void)
{
    PDUM_thAPdu hZCL_APdu;

	#ifndef COOPERATIVE
    // get ZCL mutex
        vZCL_GetInternalMutex();
    #endif

    hZCL_APdu = psZCL_Common->hZCL_APdu;
    #ifndef COOPERATIVE
        vZCL_ReleaseInternalMutex();
    #endif
    return(hZCL_APdu);

}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
