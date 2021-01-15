
/**
 *  \file BT_sm_api.h
 *
 *  This file lists all the APIs that are exported to the profiles
 *  and applications that intend to use the Security Manager for
 *  all Security related procedures.
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_SM_API_
#define _H_BT_SM_API_

/** ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_hci_api.h"
#include "sm_pl.h"

#ifdef BT_SSP
#include "sm_ssp_pl.h"
#endif /* BT_SSP */


/** ----------------------------------------- Global Definitions */
/** Security Manager Default Configuration Constants */
#ifndef BT_SSP
#define SM_SET_SECURITY_MODE                    0x01
#endif  /* BT_SSP */

/** Security Mode Definitions */
#ifndef BT_SSP
#define SM_SECURITY_MODE_1                      0x01
#define SM_SECURITY_MODE_2                      0x02
#define SM_SECURITY_MODE_3                      0x03
#endif  /* BT_SSP */

/** Security Manager Generic Operation Types */
#define SM_OPERATION_SET                        0x01
#define SM_OPERATION_RESET                      0x02
#define SM_OPERATION_GET                        0x03

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
 */
#define SM_NON_PAIRABLE                         0x00
#define SM_PAIRABLE_AND_NON_BONDABLE            0x01
#define SM_PAIRABLE_AND_BONDABLE                0x03

/** Security Manager UI Notification Callback Event Types */
#define SM_ACL_CONNECT_REQUEST_NTF              0x01
#define SM_AUTHORIZATION_REQUEST_NTF            0x02
#define SM_PIN_CODE_REQUEST_NTF                 0x03
#define SM_LINK_KEY_REQUEST_NTF                 0x04

#ifndef BT_SSP
#define SM_AUTHORIZATION_REQUEST_NTF_MODE_1_3   0x05
#endif  /* BT_SSP */

#ifdef BT_SSP_NC
#define SM_USER_CONF_REQUEST_NTF                0x06
#endif /* BT_SSP_NC */

#ifdef BT_SSP_PE
#define SM_USER_PASSKEY_REQUEST_NTF             0x07
#define SM_USER_PASSKEY_NTF                     0x08
#endif /* BT_SSP_PE */

/** Filtering Codes for Searching Device List */
#define SM_ANY_LIST                             0x01
#define SM_PAIR_LIST                            0x02
#define SM_TRUSTED_LIST                         0x03

/** Flag values for 'authenticate' parameter of SM_SERVICE */
#define SM_SERVICE_SECURITY_LEVEL_0             0x00
#define SM_SERVICE_SECURITY_LEVEL_1             0x01
#define SM_SERVICE_SECURITY_LEVEL_2             0x02
#define SM_SERVICE_SECURITY_LEVEL_3             0x03

#ifdef BT_BRSC
/**
 * BR/EDR Security Level 4 is introduced in Core v4.1.
 * For Security Mode 4, Level 4 or device in Secure
 * Connections Only Mode:
 * - MITM is required
 * - SSP (P-256) is required
 */
#define SM_SERVICE_SECURITY_LEVEL_4             0x04
#endif /* BT_BRSC */

/** Flag values for 'authorize' parameter of SM_SERVICE */
#define SM_SERVICE_AUTHORIZATION_NONE           0x00
#ifdef BT_SSP
#define SM_SERVICE_AUTHORIZATION_REQUIRED       0x01
#else  /* BT_SSP */
#define SM_SERVICE_AUTHORIZATION_MODE_2         0x01
#define SM_SERVICE_AUTHORIZATION_MODE_1_3       0x02
#endif /* BT_SSP */

/** Codes for Setting Variable PIN and Fixed PIN */
#define SM_FIXED_PIN                            1
#define SM_VARIABLE_PIN                         0


/** ----------------------------------------- Structures/Data Types */
/** SM User Interface Notification Callback */
typedef API_RESULT (* SM_UI_NOTIFY_CB)
                   (UCHAR event, UCHAR *bd_addr, UCHAR *data);


#ifdef SM_HAVE_MODE_2

/** SM Service Database Attributes for Higher Level Service */
typedef struct
{
    /** Profiles SM callback */
    SM_UI_NOTIFY_CB service_sm_cb;

    /** L2CAP PSM Value of the Protocol providing the Service */
    UINT16 psm;

    /** RFCOMM Server Channel (only if PSM is RFCOMM) */
    UCHAR  server_ch;

    /** Authentication required? */
    UCHAR authenticate;

    /** Authorization required? */
    UCHAR authorize;

    /** Encryption required? */
    UCHAR encrypt;

#ifdef BT_SSP
    /** Set 'No Bonding' in IO Cap for this service */
    UCHAR no_bonding;
#endif /* BT_SSP */

#ifdef BTSIG_ERRATA_11838
    /* Encryption key size required */
    UCHAR ekey_size;
#endif /* BTSIG_ERRATA_11838 */

} SM_SERVICE;

#endif /* SM_HAVE_MODE_2 */


typedef struct
{
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
} SM_PEER_INFO;


#ifdef __cplusplus
extern "C"{
#endif

/** ----------------------------------------- Internal Functions */
/*
 *  These functions are not APIs and should not be called by Applications
 *  or Profiles.
 */

/** To Set/Get/Reset Device Link Key */
API_RESULT sm_device_link_key
           (UCHAR op_type, UCHAR *bd_addr, UCHAR *link_key);

#ifdef SM_HAVE_MODE_2
/** To modify/retrieve Service Attributes */
API_RESULT sm_service_attributes
           (UCHAR op_type, UCHAR service_id, SM_SERVICE *serv_attr);
#endif /* SM_HAVE_MODE_2 */

/** To reply to SM UI Notification Callback Requests */
API_RESULT sm_ui_notification_request_reply
           (UCHAR* bd_addr, UCHAR accept, UCHAR reason, UCHAR event_type);


/** ----------------------------------------- API Declarations */

#ifndef BT_SSP
/** To set default Security Mode */
API_RESULT BT_sm_default_security_mode
           (UCHAR security_mode, UCHAR encrypt_flag);

/** To get default Security Mode */
API_RESULT BT_sm_get_security_mode
           (UCHAR* security_mode, UCHAR *encrypt_flag);
#endif  /* BT_SSP */

/** To set Default/Global PIN Code */
API_RESULT BT_sm_default_pin_code (UCHAR *pin, UCHAR pin_length);

/** To get Default/Global PIN Code */
API_RESULT BT_sm_get_pin (UCHAR* pin, UCHAR* pin_length);

/** To set Pairable/Bondable Mode */
API_RESULT BT_sm_set_pairable (UCHAR mode);

/** To add a remote Bluetooth device to the SM Device Database */
API_RESULT BT_sm_add_device (UCHAR *bd_addr);

/** To set a remote Bluetooth device as 'Trusted' or 'Untrusted' */
API_RESULT BT_sm_set_device_trusted (UCHAR *bd_addr, UCHAR flag);

/** To set name for a remote Bluetooth device */
API_RESULT BT_sm_set_device_name
           (UCHAR *bd_addr, UCHAR *name, UCHAR name_length);

/** To get name for a remote Bluetooth device */
API_RESULT BT_sm_get_device_name
           (UCHAR *bd_addr, UCHAR *name, UCHAR *name_length);

/** To set Bluetooth PIN (Passkey) for a remote Bluetooth device */
API_RESULT BT_sm_set_device_pin_code
           (UCHAR *bd_addr, UCHAR *pin, UCHAR pin_length);

/** To get Bluetooth PIN (Passkey) for a remote Bluetooth device */
API_RESULT BT_sm_get_device_pin_code
           (UCHAR *bd_addr, UCHAR *pin, UCHAR *pin_length);

/** To Set Link Key for a remote Bluetooth device */
#ifndef SM_LITE
#define BT_sm_set_device_link_key(bd, lk) \
        sm_device_link_key(SM_OPERATION_SET, (bd), (lk))

/** To Get Link Key (if any) for a remote Bluetooth device */
#define BT_sm_get_device_link_key(bd, lk) \
        sm_device_link_key(SM_OPERATION_GET, (bd), (lk))

/** To Delete Link Key (if any) for a remote Bluetooth device */
#define BT_sm_delete_device_link_key(bd) \
        sm_device_link_key(SM_OPERATION_RESET, (bd), NULL)
#else  /* SM_LITE */
/** To Get Link Key (if any) for a remote Bluetooth device */
API_RESULT BT_sm_get_device_link_key
           (
               /* IN */    UCHAR *    bd_addr,
               /* INOUT */ UCHAR *    link_key
           );
#endif /* SM_LITE */

/**
 *  To retrieve various attributes of a Device Database entry of a
 *  remote Bluetooth device
 */
API_RESULT BT_sm_get_device_attributes (UCHAR *bd_addr, UCHAR *device_attr);

/* To delete devices from SM Device Database based on Operation Types */
API_RESULT BT_sm_delete_device (UCHAR *bd_addr, UCHAR op_type);

/** To get a list of Device Database entries based on Operation Types */
API_RESULT BT_sm_get_device_list
           (SM_PEER_INFO *peer_info, UCHAR op_type, UCHAR *count);

#ifdef SM_HAVE_MODE_2
/** To register Security requirements of a Service */
API_RESULT BT_sm_add_service
           (
               SM_SERVICE *    serv_attr,
               UCHAR *         service_id
           );

/** To de-register Security requirements of a Service */
API_RESULT BT_sm_delete_service (UCHAR service_id);

/** To set Security requirements of a Service */
#define BT_sm_set_service_attributes(sid, attr) \
        sm_service_attributes(SM_OPERATION_SET, (sid), (attr))

/** To get Security requirements of a Service */
#define BT_sm_get_service_attributes(sid, attr) \
        sm_service_attributes(SM_OPERATION_GET, (sid), (attr))

#endif /* SM_HAVE_MODE_2 */

/** To register presence of a manual User Interface */
API_RESULT BT_sm_register_user_interface
           (
               SM_UI_NOTIFY_CB    notify_cb
           );

/** To reply to a Connection Request from the SM */
#define BT_sm_connection_request_reply(bd_addr, accept, reason) \
        sm_ui_notification_request_reply \
        ((bd_addr), (accept), (reason), SM_ACL_CONNECT_REQUEST_NTF)

#ifdef SM_HAVE_MODE_2
/** To reply to a Authorisation Request event callback from the SM */
#define BT_sm_authorization_request_reply(bd_addr, accept) \
        sm_ui_notification_request_reply \
        ((bd_addr), (accept), 0x0, SM_AUTHORIZATION_REQUEST_NTF)

/** To reply to a Authorisation Request (Mode 1/3) event callback from the SM */
#define BT_sm_authorization_request_reply_mode_1_3(bd_addr, accept) \
        sm_ui_notification_request_reply \
        ((bd_addr), (accept), 0x0, SM_AUTHORIZATION_REQUEST_NTF_MODE_1_3)
#endif /* SM_HAVE_MODE_2 */

/** To reply to a Bluetooth PIN Code Request event callback from the SM */
#define BT_sm_pin_code_request_reply(bd, p, pl) \
        hci_sm_pin_code_request_reply((bd), (p), (pl))

/** To reply to a Bluetooth Link Key Request event callback from SM */
#define BT_sm_link_key_request_reply(bd, lk, lkp) \
        hci_sm_link_key_request_reply((bd), (lk), (lkp))

/** To change Link Key of the ACL Connection with a remote device */
API_RESULT BT_sm_change_connection_link_key (UCHAR *bd_addr);

/* To request Authentication of the ACL Connection with a remote device */
API_RESULT BT_sm_authentication_request (UCHAR *bd_addr);

/* To enable/disable Encryption of data on the ACL Connection */
API_RESULT BT_sm_encryption_request (UCHAR *bd_addr, UCHAR enable);

/* To set the PIN (Variable/Fixed) Type */
#define BT_sm_set_pin_type(pt)      hci_sm_set_pin_type((pt))

/** To change Master Link Key */
#define BT_sm_master_link_key(kf)   hci_sm_master_link_key((kf))

#ifdef BT_SSP_UC
#define BT_sm_user_conf_request_reply(bd, at) \
        hci_sm_user_confirmation_request_reply((bd), (at))
#endif /* BT_SSP_UC */

#ifdef BT_SSP_PE
#define BT_sm_user_passkey_request_reply(bd, pk) \
        hci_sm_user_passkey_request_reply((bd), (pk))
#endif /* BT_SSP_PE */

#ifdef BT_BRSC
/** To enable/disable the Secure Connections Only Mode */
API_RESULT BT_sm_set_secure_connections_only_mode(UCHAR enable);

/** To get current Secure Connections Only Mode */
API_RESULT BT_sm_get_secure_connections_only_mode(UCHAR * enable);
#endif /* BT_BRSC */

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_SM_API_ */


