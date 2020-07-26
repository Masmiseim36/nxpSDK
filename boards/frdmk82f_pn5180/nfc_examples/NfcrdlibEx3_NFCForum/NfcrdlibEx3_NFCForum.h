/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* NFC Type 4A Card Emulation application.
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 6170 $ (v06.01.00)
* $Date: 2016-09-23 20:23:49 +0530 (Fri, 23 Sep 2016) $
*/

#ifndef EX3_NFCFORUM_H
#define EX3_NFCFORUM_H

/**
* Reader Library Headers
*/
#include <ph_Status.h>

#if defined (NXPBUILD__PHHAL_HW_PN5180)   || \
    defined (NXPBUILD__PHHAL_HW_RC663)    || \
    defined (NXPBUILD__PHHAL_HW_PN7462AU)
        #define PH_EXAMPLE3_LPCD_ENABLE             /* If LPCD needs to be configured and used over HAL or over DiscLoop */
#endif



/*******************************************************************************
**  Macros
*******************************************************************************/

/* Print system critical errors (with which execution can't proceed further)
 * and halt the execution. */
#define ERROR_CRITICAL(...) DEBUG_PRINTF(__VA_ARGS__); while(1) { /* Nothing to do */ }

#ifdef PH_OSAL_FREERTOS


#ifdef NXPBUILD__PHHAL_HW_PN5180
#  define RDLIB_TASK_STACK              (2000/4)
#  define SNEP_SERVER_TASK_STACK        (1000/4)
#  define SNEP_CLIENT_TASK_STACK        (1000/4)
#else
/* NXPBUILD__PHHAL_HW_PN7462AU */
#  define RDLIB_TASK_STACK              (1400/4)
#  define SNEP_SERVER_TASK_STACK         (600/4)
#  define SNEP_CLIENT_TASK_STACK         (500/4)
#endif


#define RDLIB_TASK_PRIO                4
#define SNEP_SERVER_TASK_PRIO          3
#define SNEP_CLIENT_TASK_PRIO          3

#elif defined PH_OSAL_LINUX

#define RDLIB_TASK_STACK               0x20000
#define RDLIB_TASK_PRIO                      0
#define SNEP_SERVER_TASK_STACK         0x20000
#define SNEP_SERVER_TASK_PRIO                0
#define SNEP_CLIENT_TASK_STACK         0x20000
#define SNEP_CLIENT_TASK_PRIO                0

#endif /* PH_OSAL_FREERTOS || PH_OSAL_LINUX */

#define LISTEN_PHASE_TIME_MS          300     /**< Listen Phase TIME */

/* Enable Discovery Loop Active mode. */
#define ACTIVE_MODE

/* Enable Discovery Loop Passive mode. */
#define PASSIVE_MODE

/* Enable initiator/reader mode polling configuration in Discovery Loop. */
#define INITIATOR_MODE

/* Enable listen mode configuration (target/card mode) in Discovery Loop. */
#define TARGET_MODE

/* Enable SNEP client PUT operation in this example. */
#define SNEP_CLIENT

/* Enable SNEP server to receive a SNEP PUT message in this example. */
#define SNEP_SERVER

/* Un-comment following line to enable SNEP PUT of 1K text message else SNEP PUT of URI (www.nxp.com) will be enabled by default.  */
/* #define NDEF_TEXT_1K */


#ifdef NDEF_TEXT_1K
/* SNEP PUT text message used in this example. */
static const uint8_t baSnepAppBuf[] = {
    0xC1,                   /* NDEF Record Structure */
    0x01,                   /* TYPE LENGTH */
    0x00, 0x00, 0x04, 0x01, /* PAYLOAD LENGTH */
    0x54,                   /* TYPE TEXT */
    0x02,                   /* ID UTF8 */
    0x65, 0x6E,             /* LOCALE EN */
    'N','X','P',' ','S','e','m','i','c','o','n','d','u','c','t','o','r','s',' ','c','r','e','a','t','e','s',' ','s','o','l'
    ,'u','t','i','o','n','s',' ','t','h','a','t',' ','e','n','a','b','l','e',' ','s','e','c','u','r','e',' ','c','o','n','n'
    ,'e','c','t','i','o','n','s',' ','f','o','r',' ','a',' ','s','m','a','r','t','e','r',' ','w','o','r','l','d','.',' ','B'
    ,'u','i','l','d','i','n','g',' ','o','n',' ','i','t','s',' ','e','x','p','e','r','t','i','s','e',' ','i','n',' ','H','i'
    ,'g','h',' ','P','e','r','f','o','r','m','a','n','c','e',' ','M','i','x','e','d',' ','S','i','g','n','a','l',' ','e','l'
    ,'e','c','t','r','o','n','i','c','s',',',' ','N','X','P',' ','i','s',' ','d','r','i','v','i','n','g',' ','i','n','n','o'
    ,'v','a','t','i','o','n',' ','i','n',' ','t','h','e',' ','a','u','t','o','m','o','t','i','v','e',',',' ','i','d','e','n'
    ,'t','i','f','i','c','a','t','i','o','n',' ','a','n','d',' ','m','o','b','i','l','e',' ','i','n','d','u','s','t','r','i'
    ,'e','s',',',' ','a','n','d',' ','i','n',' ','a','p','p','l','i','c','a','t','i','o','n',' ','a','r','e','a','s',' ','i'
    ,'n','c','l','u','d','i','n','g',' ','w','i','r','e','l','e','s','s',' ','i','n','f','r','a','s','t','r','u','c','t','u'
    ,'r','e',',',' ','l','i','g','h','t','i','n','g',',',' ','h','e','a','l','t','h','c','a','r','e',',',' ','i','n','d','u'
    ,'s','t','r','i','a','l',',',' ','c','o','n','s','u','m','e','r',' ','t','e','c','h',' ','a','n','d',' ','c','o','m','p'
    ,'u','t','i','n','g','.',' ','F','o','r',' ','a','p','p','l','i','c','a','t','i','o','n','s',' ','d','e','m','a','n','d'
    ,'i','n','g',' ','t','h','e',' ','h','i','g','h','e','s','t',' ','s','e','c','u','r','i','t','y',' ','a','n','d',' ','r'
    ,'e','l','i','a','b','i','l','i','t','y',' ','(','e','.','g','.',' ','s','m','a','r','t',' ','c','a','r','d','s',',',' '
    ,'s','e','c','u','r','e',' ','d','o','n','g','l','e','s',',',' ','S','A','M',' ','m','o','d','u','l','e','s','.','.','.'
    ,')',' ','w','e',' ','p','r','o','v','i','d','e',' ','d','i','f','f','e','r','e','n','t',' ','p','l','a','t','f','o','r'
    ,'m','s',' ','a','c','c','o','r','d','i','n','g',' ','t','o',' ','a','p','p','l','i','c','a','t','i','o','n',' ','n','e'
    ,'e','d','s',' ','r','a','n','g','i','n','g',' ','f','r','o','m',' ','o','u','r',' ','w','e','l','l',' ','e','s','t','a'
    ,'b','l','i','s','h','e','d',' ','W','E','-','c','o','n','t','a','c','t',' ','c','o','n','t','r','o','l','l','e','r',' '
    ,'f','a','m','i','l','y',' ','t','o',' ','o','u','r',' ','S','m','a','r','t','M','X',' ','a','n','d',' ','S','m','a','r'
    ,'t','X','A',' ','d','e','v','i','c','e','s','.',' ','O','u','r',' ','M','I','F','A','R','E',' ','c','o','n','t','a','c'
    ,'t','l','e','s','s',' ','s','m','a','r','t',' ','c','a','r','d',' ','I','C','s',' ','p','r','o','v','i','d','e',' ','t'
    ,'h','e',' ','m','o','s','t',' ','a','d','v','a','n','c','e','d',' ','c','o','m','b','i','n','a','t','i','o','n',' ','o'
    ,'f',' ','s','e','c','u','r','i','t','y',' ','a','n','d',' ','c','o','n','v','e','n','i','e','n','c','e',' ','f','o','r'
    ,' ','c','o','n','t','a','c','t','l','e','s','s',' ','i','n','t','e','r','f','a','c','i','n','g',' ','i','n',' ','a','c'
    ,'c','o','r','d','a','n','c','e',' ','w','i','t','h',' ','t','h','e',' ','I','S','O',' ','1','4','4','4','3','A',' ','i'
    ,'n','t','e','r','f','a','c','e',' ','s','t','a','n','d','a','r','d','.',' ','O','u','r',' ','I','C','O','D','E',' ','a'
    ,'n','d',' ','U','C','O','D','E',' ','f','a','m','i','l','i','e','s',' ','o','f',' ','R','F','I','D',' ','I','C','s',' '
    ,'a','r','e',' ','s','p','e','c','i','f','i','c','a','l','l','y',' ','d','e','s','i','g','n','e','d',' ','f','o','r',' '
    ,'h','i','g','h','-','v','o','l','u','m','e',' ','s','m','a','r','t',' ','l','a','b','e','l',' ','a','p','p','l','i','c'
    ,'a','t','i','o','n','s','.',' ','I','n',' ','a','d','d','i','t','i','o','n',' ','o','u','r',' ','H','I','T','A','G',' '
    ,'f','a','m','i','l','y',' ','o','f',' ','I','C','s',',',' ','w','i','t','h',' ','a','n',' ','o','p','e','r','a','t','i'
    ,'n','g',' ','r','a','n','g','e',' ','o','f',' ','m','o','r','e',' ','t','h','a','n',' ','o','n','e',' ','m','e','t','e'
    ,'r','.'
};
#else
/* Short SNEP PUT URI message used in this example. */
static const uint8_t baSnepAppBuf[] = {
    0xC1,                   /* NDEF record structure */
    0x01,                   /* TYPE LENGTH */
    0x00, 0x00, 0x00, 0x08, /* PAYLOAD LENGTH */
    0x55,                   /* TYPE TEXT */
    0x01,                   /* ID UTF8 */
    'n','x','p','.','c','o','m'
};
#endif /* NDEF_TEXT_1K */

#define SNEP_PUT               0x01
#define SNEP_RECEIVED_PUT      0x80

/**
 * Read NDEF message from detected tag.
 * */
phStatus_t ReadNdefMessage(
    uint8_t TopTagType);      /* [in] Tag type from which to read NDEF */

/**
 * Reader library thread. This is the main thread which starts discovery loop
 * in listen mode and poll mode (one at a time) and manages switching between
 * both listen and poll modes.
 * */
void TReaderLibrary(
    void *pDataParams);        /* [in] Input parameter to thread. */

/**
* SNEP Server Task. This Task registers SNEP Server and waits for  Remote Client
* to PUT SNEP Message.
* Refer phnpSnep.h for more info.
* APIs which does PAL or HAL exchange should not be called from application
* thread.
*/
void TSNEPServer(
    void * pLlcp             /* [in] Input parameter to thread. */
    );

/**
* SNEP Client Task. This Task registers SNEP Client and waits till Remote Server
* accepts connection and then perform SNEP PUT action.
* Refer phnpSnep.h for more info.
* APIs which does PAL or HAL exchange should not be called from application
* thread.
*/
void TSNEPClient(
    void * pLlcp             /* [in] Input parameter to thread. */
    );


#endif /* EX3_NFCFORUM_H */
