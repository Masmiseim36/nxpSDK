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
\file       pdum_gen.c
\brief      PDU Manager Configuration
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "jendefs.h"
#include "pdum_nwk.h"
#include "pdum_apl.h"
#include "app_pdum_cfg.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

struct pdum_tsAPdu_tag {
    struct pdum_tsAPduInstance_tag *psAPduInstances;
    uint16 u16FreeListHeadIdx;
    uint16 u16Size;
    uint16 u16NumInstances;
};

struct pdum_tsAPduInstance_tag {
    uint8 *au8Storage;
    uint16 u16Size;
    uint16 u16NextAPduInstIdx;
    uint16 u16APduIdx;
};

typedef struct pdum_tsAPduInstance_tag pdum_tsAPduInstance;
typedef struct pdum_tsAPdu_tag pdum_tsAPdu;


/****************************************************************************/
/***        Function Prototypes                                           ***/
/****************************************************************************/

PRIVATE void PDUM_vInitPools(void);

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/* Macro definition for Ex: PUBLIC pdum_tsAPduInstance s_asapduZDPInstances[6]; */
#define _pdum_handler_name_(a)  PUBLIC pdum_tsAPduInstance s_asa##a##Instances
#define _pdum_block_size_  [0*
#define _pdum_queue_size_ +
#define _eol_  ];

/* APDU Instances */
/* PUBLIC pdum_tsAPduInstance s_asapduZDPInstances[6] */
PdumsDetails_c

#undef _pdum_handler_name_
#undef _pdum_queue_size_
#undef _pdum_block_size_
#undef _eol_

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/* Macro definition for Ex: struct pdum_tsAPdu_tag pdum_apduZDP = {s_asapduZDPInstances, 0, 100, 6};*/
#define _pdum_handler_name_(a)  struct pdum_tsAPdu_tag  a =  {s_asa##a##Instances, 0
#define _pdum_block_size_ ,
#define _pdum_queue_size_ ,
#define _eol_  };

/* APDU Instances Containers */
PdumsDetails_c


#undef _pdum_handler_name_
#undef _pdum_queue_size_
#undef _pdum_block_size_
#undef _eol_


#define _pdum_handler_name_(a)  & a
#define _pdum_block_size_(a) 
#define _pdum_queue_size_(a)
#define _eol_  ,

pdum_tsAPdu *s_asAPduPool[] =  
{
    /* APDU pools */
    /* &pdum_apduZDP */
    PdumsDetails_c
};

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

extern void pdum_vAPduInit (pdum_tsAPdu *asAPduPool[], uint16 u16NumAPdus);

PUBLIC void PDUM_vInit(void)
{

    PDUM_vInitPools();
    pdum_vAPduInit(s_asAPduPool, sizeof(s_asAPduPool)/sizeof(pdum_tsAPdu *));
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

PRIVATE void PDUM_vInitPools(void)
{
    uint8_t indexPools;
    uint8_t indexInstances;
    
    for(indexPools = 0; indexPools < sizeof(s_asAPduPool)/sizeof(pdum_tsAPdu *); indexPools++)
    {
        for(indexInstances = 0; indexInstances <  s_asAPduPool[indexPools]->u16NumInstances; indexInstances++)
        {
            s_asAPduPool[indexPools]->psAPduInstances[indexInstances].au8Storage = NULL;
            s_asAPduPool[indexPools]->psAPduInstances[indexInstances].u16Size = 0;
            s_asAPduPool[indexPools]->psAPduInstances[indexInstances].u16NextAPduInstIdx = 0;
            s_asAPduPool[indexPools]->psAPduInstances[indexInstances].u16APduIdx = indexInstances;
        }
    } 
    
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
