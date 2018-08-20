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

#ifndef DLIST_H_INCLUDED
#define DLIST_H_INCLUDED

/*!
\file       dlist.h
\brief      Doubly Linked list definitions
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* DLNODE - Doubly linked-list node */
typedef struct tagDNODE DNODE;

struct tagDNODE
{
  DNODE *psNext;
  DNODE *psPrevious;
};

/* DLIST - Doubly-linked-list header */
typedef struct tagDLIST 
{
  DNODE *psFirst;  /* First node in list (Points to NULL if none) */
  DNODE *psLast;   /* Last node in list (Points to NULL if none) */
} DLIST;

typedef bool (boDLISTtest)(void *pvSearchParam, void *psNodeUnderTest);

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/* Linked-list initialisation */
PUBLIC void vDLISTinitialise( DLIST *psList );

/* Search operations */
PUBLIC void *psDLISTgetHead( DLIST *psList );
PUBLIC void *psDLISTgetTail( DLIST *psList );

PUBLIC void *psDLISTgetNext( DNODE *psCurrentNode );
PUBLIC void *psDLISTgetPrevious( DNODE *psCurrentNode );

/* Addition operations */
PUBLIC void vDLISTaddToHead( DLIST *psList, DNODE *psNewNode );
PUBLIC void vDLISTinsertBefore( DLIST  *psList,
                                DNODE *psCurrentNode,
                                DNODE *psNewNode);
PUBLIC void vDLISTinsertAfter( DLIST  *psList,
                               DNODE *psCurrentNode,
                               DNODE *psNewNode);
PUBLIC void vDLISTaddToTail( DLIST *psList, DNODE *psNewNode );

/* Removal operations */
PUBLIC DNODE *psDLISTremove( DLIST *psList, DNODE *psNode );
PUBLIC DNODE *psDLISTremoveFromTail( DLIST *psList );
PUBLIC DNODE *psDLISTremoveFromHead( DLIST *psList );

/* Searching operations */
extern PUBLIC void *psDLISTsearchFromHead( DLIST *psList,
                                            boDLISTtest *prTest,
                                            void *pvSearchElement);
extern PUBLIC void *psDLISTsearchForward( DNODE *psCurrentNode,
                                           boDLISTtest *prTest,
                                           void *pvSearchElement);
extern PUBLIC void *psDLISTsearchBackward( DNODE *psCurrentNode,
                                            boDLISTtest *prTest,
                                            void *pvSearchElement);
extern PUBLIC void *psDLISTsearchFromTail( DLIST *psList,
                                            boDLISTtest *prTest,
                                            void *pvSearchElement);

/* Status operations */
PUBLIC int iDLISTnumberOfNodes( DLIST *psList );
PUBLIC bool boDLISTisEmpty( DLIST *psList );

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DLIST_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
