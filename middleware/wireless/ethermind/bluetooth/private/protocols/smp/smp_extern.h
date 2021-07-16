
/**
 *  \file smp_extern.h
 *
 *  This file conatains the external global varibale definition.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SMP_EXTERN_
#define _H_SMP_EXTERN_

/** ============================================= External Global Variables */

/** SMP FSM Identifier index */
extern UCHAR smp_fsm_id;

/** Security Manager Protocol User Notification callback */
extern SMP_UI_NOTIFY_CB  smp_ui_notify;

/** Security Manager Protocol Device database */
extern SMP_DEVICE_ENTITY smp_devices[SMP_MAX_DEVICES];

/** Security Manager Protocol Contexts */
extern SMP_CONTEXT       smp_context[SMP_MAX_CONTEXTS];

/** Security Manager Global Mutex */
BT_DEFINE_MUTEX_TYPE (extern, smp_mutex)

/** Private Address Procedure Information */
extern SMP_PVT_ADDR smp_pvt_addr;

/** SMP Toolbox context */
extern SMP_TBX_CONTEXT tbx_context;

#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
extern SMP_APPL_AES_CMAC_UTILITY_CB smp_appl_aes_cb;
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */

#ifdef HOST_RESOLVE_PVT_ADDR
/* Resolution Address */
extern UCHAR smp_resolution_addr[BT_BD_ADDR_SIZE];

/* Resolution Procedure Index */
extern UCHAR smp_resolution_index;

/* Address resolution complete handler */
extern SMP_PVT_ADDR_RESOLV_COMPLETE_CB smp_rpa_resolution_handler;
#endif /* HOST_RESOLVE_PVT_ADDR */

#if ((defined SMP_DATA_SIGNING) || (defined SMP_LESC))
extern SMP_AES_CONTEXT smp_aes_context;
#endif /* ((defined SMP_DATA_SIGNING) || (defined SMP_LESC)) */

#ifdef SMP_LESC
/* SMP Tool box Related LESC Constant Defines */
extern DECL_CONST UCHAR smp_lesc_keygen_id[SMP_LESC_KEYID_SIZE];
extern DECL_CONST UCHAR smp_lesc_kygen_salt[SMP_LESC_KEYID_SALT_SIZE];
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
extern DECL_CONST UCHAR smp_lesc_lk_tmp1[SMP_LESC_KEYID_SIZE];
extern DECL_CONST UCHAR smp_lesc_ltk_tmp2[SMP_LESC_KEYID_SIZE];
extern DECL_CONST UCHAR smp_lesc_lk_lebr[SMP_LESC_KEYID_SIZE];
extern DECL_CONST UCHAR smp_lesc_ltk_brle[SMP_LESC_KEYID_SIZE];

extern DECL_CONST UCHAR smp_lesc_lk_salt[SMP_LESC_KEYID_SALT_SIZE];
extern DECL_CONST UCHAR smp_lesc_ltk_salt[SMP_LESC_KEYID_SALT_SIZE];
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
/**
 * Bit field representing valid device queue entries w.r.t. SMP.
 * Used in consistency check during startup after restoring data structures
 * from persistent storage.
 */
extern UINT32   smp_dq_valid_entries;
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

#endif /* _H_SMP_EXTERN_ */

