
/**
 *  \file aes_cmac_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "aes_cmac.h"
#include "aes_cmac_pl.h"

#ifdef AES_CMAC_UTIL

/*
 * NOTE:
 *
 * This flag is to be defined to simulate encryption return from API even though
 * controller encrypt is called. Hence this uses Wait & Signal and shall be used
 * only when testing the AES by calling from a task other than the
 * EtherMind Read Task
 */
#ifndef AES_CMAC_PERFORMANCE_ANALYSIS
/* #define AES_HAVE_ENC_RETURN */
#endif /* AES_CMAC_PERFORMANCE_ANALYSIS */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
#ifdef AES_HAVE_ENC_RETURN
BT_DEFINE_MUTEX(aes_mutex)
BT_DEFINE_COND(aes_cond)

UCHAR aes_encout[AES_CMAC_KEY_SIZE];
UCHAR aes_encstatus;
#endif /* AES_HAVE_ENC_RETURN */

/* --------------------------------------------- Functions */

void aes_cmac_init_pl (void)
{
#ifndef AES_CMAC_PERFORMANCE_ANALYSIS
#ifdef AES_HAVE_ENC_RETURN
    BT_MUTEX_INIT_VOID(aes_mutex, AES_PL);
    BT_COND_INIT_VOID(aes_cond, AES_PL);
#endif /* AES_HAVE_ENC_RETURN */
#endif /* AES_CMAC_PERFORMANCE_ANALYSIS */
}

/**
 *  \fn aes_cmac_128B_encrypt_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param key
 *  \param data
 *  \param encout
 *
 *  \return
 */
API_RESULT aes_cmac_128B_encrypt_pl (UCHAR * key, UCHAR * data, UCHAR * encout)
{
#ifndef AES_CMAC_PERFORMANCE_ANALYSIS
    API_RESULT retval;
    UCHAR counter;

    /* Init */
    retval = API_SUCCESS;

    BT_debug_trace(EM_MODULE_ID_AES_CMAC, "[AES_CMAC] Enc Input.\n");
    BT_debug_dump_bytes(EM_MODULE_ID_AES_CMAC, data, 16U);

    BT_debug_trace(EM_MODULE_ID_AES_CMAC, "[AES_CMAC] Enc Key.\n");
    BT_debug_dump_bytes(EM_MODULE_ID_AES_CMAC, key, 16U);

    /* Invoke HCI Encrypt */
    retval = BT_smp_128B_encrypt_pl (key, data, &counter);

    if (API_SUCCESS == retval)
    {
#ifdef AES_HAVE_ENC_RETURN

        BT_MUTEX_LOCK_VOID(aes_mutex, AES_PL);

        BT_thread_cond_wait(&aes_cond, &aes_mutex);
        BT_mem_copy (encout, aes_encout, AES_CMAC_KEY_SIZE);
        retval = aes_encstatus;

        BT_MUTEX_UNLOCK_VOID(aes_mutex, AES_PL);
#else /* AES_HAVE_ENC_RETURN */

        *encout = counter;
        retval = AES_CMAC_PROC_PENDING; /* return AES_CMAC_PROC_PENDING; */
#endif /* AES_HAVE_ENC_RETURN */
    }

    return retval;
#else /* AES_CMAC_PERFORMANCE_ANALYSIS */
    BT_IGNORE_UNUSED_PARAM(key);
    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(encout);

    return API_SUCCESS;
#endif /* AES_CMAC_PERFORMANCE_ANALYSIS */
}

#ifndef AES_CMAC_PERFORMANCE_ANALYSIS
void aes_cmac_128B_encrypt_complete_pl (UCHAR index, UCHAR status, UCHAR * data, UINT16 length)
{
#ifdef AES_HAVE_ENC_RETURN

    BT_MUTEX_LOCK_VOID(aes_mutex, AES_PL);

    /* Store and signal */
    aes_encstatus = status;
    BT_mem_copy (aes_encout, data, AES_CMAC_KEY_SIZE);
    BT_thread_cond_signal(&aes_cond);

    BT_MUTEX_UNLOCK_VOID(aes_mutex, AES_PL);
#else /* AES_HAVE_ENC_RETURN */

    BT_debug_trace(EM_MODULE_ID_AES_CMAC, "[AES_CMAC] Enc Out.\n");
    BT_debug_dump_bytes(EM_MODULE_ID_AES_CMAC, data, 16U);

    aes_cmac_aes_128_encryption_complete (index, status, data, length);
#endif /* AES_HAVE_ENC_RETURN */
}
#endif /* AES_CMAC_PERFORMANCE_ANALYSIS */

#endif /* AES_CMAC_UTIL */
