/**
 *  \file appl_smp.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_SMP_
#define _H_APPL_SMP_

/* --------------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#include "BT_sm_api.h"
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

#ifdef APPL_FSM_SUPPORT
#include "appl_fsm.h"
#endif /* APPL_FSM_SUPPORT */

#include "smp_pl.h"


/* Repeat Tracking Feature Enable */
/* #define HAVE_SMP_REPEAT_TRACKING */

/* Parallel Authentication Feature Enable - 4.0 and 4.1 */
/* #define APPL_SMP_PARALLEL_AUTH */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
#ifdef HAVE_SMP_REPEAT_TRACKING
extern BT_timer_handle smp_repeat_timer;
#endif /* HAVE_SMP_REPEAT_TRACKING */

/* --------------------------------------------- Static Global Variables */
void main_smp_operations (void);
void appl_smp_reset (void);
void appl_smp_generate_sign_data (UCHAR * data, UINT16 datalen);
void appl_smp_verify_sign_data (UCHAR * data, UINT16 datalen);

void smp_pl_operations(void);

API_RESULT appl_smp_cb
           (
                SMP_BD_HANDLE * bd_handle,
                UCHAR         event,
                API_RESULT    status,
                void        * event_data,
                UINT16        data_len
           );


void appl_smp_register (void);

void appl_smp_configure_platform (void);

#ifndef SMP
#define appl_smp_assert_security(a, b, c) API_SUCCESS
#else /* SMP */
API_RESULT appl_smp_assert_security
           (
               SMP_BD_HANDLE * handle,
               UCHAR           security,
               UCHAR           ekey_size
           );
#endif /* SMP */

API_RESULT appl_smp_check_security_access
          (
              SMP_BD_HANDLE * handle,
              UCHAR sec_level,
              UCHAR ekey_size
          );

API_RESULT appl_smp_security_request
          (
              SMP_BD_HANDLE * handle,
              SMP_AUTH_INFO * auth
          );

API_RESULT appl_smp_get_bonded_device_list
           (
               /* OUT */ UCHAR              * count,
               /* OUT */   SMP_BD_HANDLE    * dev_handle
           );

#ifdef SMP_LESC
#ifdef SMP_HAVE_OOB_SUPPORT
void appl_smp_lesc_oob_data_complete(SMP_LESC_OOB_DATA_PL * appl_lesc_oob);
#endif /* SMP_HAVE_OOB_SUPPORT */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
void appl_smp_lesc_txp_key_gen_complete(SMP_LESC_LK_LTK_GEN_PL * appl_txp_key);
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

#endif /* _H_APPL_SMP_ */

