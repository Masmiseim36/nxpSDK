
/**
 *  \file aes_cmac_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_AES_CMAC_PL_
#define _H_AES_CMAC_PL_

/* --------------------------------------------- Header File Inclusion */
#ifndef AES_CMAC_PERFORMANCE_ANALYSIS
#include "BT_common.h"
#include "BT_hci_api.h"
#else /* AES_CMAC_PERFORMANCE_ANALYSIS */
#include "aes_cmac_os.h"
#endif /* AES_CMAC_PERFORMANCE_ANALYSIS */
#include "smp_pl.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
/* Hack. TBD */
#define EM_MODULE_ID_AES_CMAC  0

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
/* Platform Init */
void aes_cmac_init_pl (void);

/* Platform Encrypt */
API_RESULT aes_cmac_128B_encrypt_pl (UCHAR * key, UCHAR * data, UCHAR * encout);
void aes_cmac_128B_encrypt_complete_pl(UCHAR index, UCHAR status, UCHAR * data, UINT16 length);

#endif /* _H_AES_CMAC_PL_ */

