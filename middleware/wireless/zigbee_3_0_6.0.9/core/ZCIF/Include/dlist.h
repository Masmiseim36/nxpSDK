/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
