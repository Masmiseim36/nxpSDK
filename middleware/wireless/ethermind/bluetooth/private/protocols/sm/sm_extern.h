
/**
 *  \file sm_extern.h
 *
 *  This header file contains declarations of all global variables
 *  used in the SM module. This file will be included by all the
 *  source files except the sm_init.c file.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SM_EXTERN_
#define _H_SM_EXTERN_

/* ----------------------------------------- External Global Variables */

/** Security Manager Global Mutex */
BT_DEFINE_MUTEX_TYPE (extern, sm_mutex)

/** The UI Notification Callback */
extern SM_UI_NOTIFY_CB sm_ui_notify;

#ifndef BT_SSP
/** Default Security Mode */
extern UCHAR sm_security_mode;

/* Encryption Mode in Security Mode 3 */
extern UCHAR sm_encryption_mode;
#endif  /* BT_SSP */

/**
 *  Default/Global PIN Code
 *  This is also the PIN Code to be used when remote clients
 *  connect to the local Server Profiles/Applications
 */
extern UCHAR sm_default_pin [];
extern UCHAR sm_default_pin_length;

/**
 *  Pairable & Bondable mode values.
 *  - Bit 0: Pairable
 *  - Bit 1: Bondable
 *
 *  Pairing is the process of generating the common Link Key for a remote
 *  device, whereas Bonding is the process of remembering the generated
 *  Link Key from the Pairing process.
 *
 *  Hence, if the device needs to be Bondable, it must also be Pairable.
 *  If the device is set to Non-Pairable, then it is also Non-Bondable.
 *
 *  Defined Values (see BT_sm_api.h)
 *  - SM_NON_PAIRABLE: 0x00
 *  - SM_PAIRABLE_AND_NON_BONDABLE: 0x01
 *  - SM_PAIRABLE_AND_BONDABLE: 0x03
 */
extern UCHAR sm_pairable_mode;

/** Security Manager Device Database */
extern SM_DEVICE_ENTITY sm_devices [];

/** ACL Connection Handle for which Authentication started */
extern UINT16 sm_authentication_handle;

/** ACL Connection Handle for which Encryption started */
extern UINT16 sm_encryption_handle;

#ifdef BT_SSP
/** The Service for which Authentication started */
extern UCHAR sm_authentication_service;
#endif /* BT_SSP */

#ifdef SM_HAVE_MODE_2
/** The Security Manager Service Database */
extern SM_SERVICE_ENTITY sm_services [];

/** Security Manager Security Access Contexts */
extern SM_CONTEXT_ENTITY sm_contexts [];
#endif /* SM_HAVE_MODE_2 */

#ifdef BT_BRSC
/** Secure Connections Only mode values */
extern UCHAR sm_secure_connections_only_mode;
#endif /* BT_BRSC */

#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
/**
 * Bit field representing valid device queue entries w.r.t. Security Manager.
 * Used in consistency check during startup after restoring data structures
 * from persistent storage.
 */
extern UINT32   sm_dq_valid_entries;
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

#endif /* _H_SM_EXTERN_ */


