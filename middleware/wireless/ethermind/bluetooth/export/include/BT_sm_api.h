
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
#define SM_SET_SECURITY_MODE                    0x01U
#endif  /* BT_SSP */

/** Security Mode Definitions */
/**
 * @name  Security Mode Definitions
 *
 * Constant Definitions for Security Mode Definitions
 */
/*@{*/
#ifndef BT_SSP
#define SM_SECURITY_MODE_1                      0x01U
#define SM_SECURITY_MODE_2                      0x02U
#define SM_SECURITY_MODE_3                      0x03U
#endif  /* BT_SSP */
/*@}*/
/** Security Manager Generic Operation Types */
/**
 * @name  Security Manager Generic Operation Types
 *
 * Constant Definitions for Security Manager Generic Operation Types
 */
/*@{*/
#define SM_OPERATION_SET                        0x01U
#define SM_OPERATION_RESET                      0x02U
#define SM_OPERATION_GET                        0x03U
/*@}*/
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
#define SM_NON_PAIRABLE                         0x00U
#define SM_PAIRABLE_AND_NON_BONDABLE            0x01U
#define SM_PAIRABLE_AND_BONDABLE                0x03U

/** Security Manager UI Notification Callback Event Types */
/**
 * @name  Security Manager UI Notification Callback Event Types
 *
 * Constant Definitions for Security Manager UI Notification Callback Event Types
 */
/*@{*/
#define SM_ACL_CONNECT_REQUEST_NTF              0x01U
#define SM_AUTHORIZATION_REQUEST_NTF            0x02U
#define SM_PIN_CODE_REQUEST_NTF                 0x03U
#define SM_LINK_KEY_REQUEST_NTF                 0x04U

#ifndef BT_SSP
#define SM_AUTHORIZATION_REQUEST_NTF_MODE_1_3   0x05U
#endif  /* BT_SSP */

#ifdef BT_SSP_NC
#define SM_USER_CONF_REQUEST_NTF                0x06U
#endif /* BT_SSP_NC */

#ifdef BT_SSP_PE
#define SM_USER_PASSKEY_REQUEST_NTF             0x07U
#define SM_USER_PASSKEY_NTF                     0x08U
#endif /* BT_SSP_PE */
/*@}*/
/** Filtering Codes for Searching Device List */
/**
 * @name  Filtering Codes for Searching Device List
 *
 * Constant Definitions for Filtering Codes for Searching Device List
 */
/*@{*/
#define SM_ANY_LIST                             0x01U
#define SM_PAIR_LIST                            0x02U
#define SM_TRUSTED_LIST                         0x03U
/*@}*/
/** Flag values for 'authenticate' parameter of SM_SERVICE */
/**
 * @name  Flag values for 'authenticate' parameter of SM_SERVICE
 *
 * Constant Definitions for Flag values for 'authenticate' parameter of SM_SERVICE
 */
/*@{*/
#define SM_SERVICE_SECURITY_LEVEL_0             0x00U
#define SM_SERVICE_SECURITY_LEVEL_1             0x01U
#define SM_SERVICE_SECURITY_LEVEL_2             0x02U
#define SM_SERVICE_SECURITY_LEVEL_3             0x03U
/*@}*/
#ifdef BT_BRSC
/**
 * BR/EDR Security Level 4 is introduced in Core v4.1.
 * For Security Mode 4, Level 4 or device in Secure
 * Connections Only Mode:
 * - MITM is required
 * - SSP (P-256) is required
 */
#define SM_SERVICE_SECURITY_LEVEL_4             0x04U
#endif /* BT_BRSC */

/** Flag values for 'authorize' parameter of SM_SERVICE */
#define SM_SERVICE_AUTHORIZATION_NONE           0x00U
#ifdef BT_SSP
#define SM_SERVICE_AUTHORIZATION_REQUIRED       0x01U
#else  /* BT_SSP */
#define SM_SERVICE_AUTHORIZATION_MODE_2         0x01U
#define SM_SERVICE_AUTHORIZATION_MODE_1_3       0x02U
#endif /* BT_SSP */

/** Codes for Setting Variable PIN and Fixed PIN */
#define SM_FIXED_PIN                            1U
#define SM_VARIABLE_PIN                         0U


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
    /** Encryption key size required */
    UCHAR ekey_size;
#endif /* BTSIG_ERRATA_11838 */

} SM_SERVICE;

#endif /* SM_HAVE_MODE_2 */

/** Peer device information */
typedef struct
{
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
} SM_PEER_INFO;

#ifdef BTSIG_ERRATA_11838
/** Current security state information */
typedef struct _SM_DEVICE_STATE
{
    /* Encryption key Size */
    UCHAR ekey_size;

} SM_DEVICE_STATE;
#endif /* BTSIG_ERRATA_11838 */

#ifdef __cplusplus
extern "C"{
#endif

/** ----------------------------------------- Internal Functions */
/*
 *  These functions are not APIs and should not be called by Applications
 *  or Profiles.
 */

/** To Set/Get/Reset Device Link Key */
/**
 *
 *  \brief To get/set/reset Link Key for a remote Bluetooth device
 *
 *  \par Description:
 *       This internal function sets/gets/resets the Link Key for a specified
 *       remote Bluetooth device.
 *
 *  \param [in] op_type
 *         Identifies the kind of operation
 *  \param [in] bd_addr
 *         The BD_ADDR of the remote Bluetooth Device
 *  \param [in,out] link_key
 *         The Link Key for the remote Bluetooth Device
 *  \param [in,out] link_key_type
 *         The Link Key Type for the remote Bluetooth Device
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT sm_device_link_key_and_type
           (UCHAR op_type, UCHAR *bd_addr, UCHAR *link_key, UCHAR *link_key_type);

#ifdef SM_HAVE_MODE_2
/** To modify/retrieve Service Attributes */
/**
 *  \brief To set/get various Service DB Attributes
 *
 *  \par Description:
 *       This API sets/gets security requirements for a higher
 *       level service (Profile/Application).
 *
 *  \param [in] op_type
 *         Identifies the kind of operation
 *  \param [in] si
 *         Identifies a registered Service
 *  \param [in,out] serv_attr
 *         Contains security details of the Service
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT sm_service_attributes
           (UCHAR op_type, UCHAR si, SM_SERVICE *serv_attr);
#endif /* SM_HAVE_MODE_2 */

/** To reply to SM UI Notification Callback Requests */
/**
 *
 *  \brief To handle the reply from application/profile in response to
 *         invokation of SM UI Notification Callback
 *
 *  \par Description:
 *       This API allows the user to reply to Connection/Authorization Request
 *       when the SM is in Manual mode of operation
 *
 *  \param [in] bd_addr
 *         The BD_ADDR of the remote device
 *
 *  \param [in] accept
 *         Flag to accept or reject connection
 *
 *  \param [in] reason
 *         Reason flag for Connection being rejected
 *
 *  \param [in] event_type
 *         Defines the Event Type of original SM UI Notification Callback
 *         for which this reply function is invoked
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT sm_ui_notification_request_reply
           (UCHAR* bd_addr, UCHAR accept, UCHAR reason, UCHAR event_type);


/** ----------------------------------------- API Declarations */

#ifndef BT_SSP
/** To set default Security Mode */
/**
 *  \brief To set the default Security Mode
 *
 *  \par Description:
 *       This API configures the Security Manager in one of the three Security
 *       Modes, as defined in Bluetooth Generic Access Profile (GAP).
 *
 *  \param [in] security_mode
 *         This parameter defines the Security Mode that should be configured
 *         for the Security Manager. The acceptable values, as defined in the
 *         Security Modes section, are:
 *         SM_SECURITY_MODE_1
 *         SM_SECURITY_MODE_2
 *         SM_SECURITY_MODE_3
 *
 *
 *  \param [in] encrypt_flag
 *         This parameter defines the default encryption mode when the mode
 *         parameter is set to SM_SECURITY_MODE_3. The valid values are:
 *         0 - No Encryption
 *         1 - Encryption Enabled
 *         This parameter is ignored if mode is either SM_SECURITY_MODE_1 or SM_SECURITY_MODE_2.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sm_default_security_mode
           (UCHAR security_mode, UCHAR encrypt_flag);

/** To get default Security Mode */
/**
 *  \brief To get the current Security Mode of the Security Manager.
 *
 *  \par Description:
 *       This API returns the security mode and encryption mode of the Security
 *       Manager.
 *
 *  \param [out] security_mode
 *         Pointer to a caller allocated UCHAR variable onto which the Security
 *         Mode set for the Security Manager will be copied.
 *
 *  \param [out] encrypt_flag
 *         Pointer to a caller allocated UCHAR variable onto which the default
 *         Encryption Mode set for the Security Manager will be copied.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_get_security_mode
           (UCHAR* security_mode, UCHAR *encrypt_flag);
#endif  /* BT_SSP */

/** To set Default/Global PIN Code */
/**
 *  \brief To set the default Bluetooth PIN Code
 *
 *  \par Description:
 *       This API configures the default Bluetooth PIN Code that the Security
 *       Manager will use under the following conditions:
 *       When PIN Code Request comes for a remote Bluetooth device, for which
 *       ACL connection has not been initiated locally. That is, it is a remote
 *       connection to the local device.
 *
 *  \param [in] pin
 *         This parameter holds the Bluetooth PIN, and it must be a pointer to
 *         an UCHAR buffer. Application can pass NULL only if the pin_length is
 *         Zero.
 *  \param [in] pin_length
 *         This parameter specifies the length of the PIN Code - size of the
 *         UCHAR buffer as specified in the parameter pin. The length of PIN
 *         Code can be Zero, but must not be more than PIN_CODE_SIZE octets.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_default_pin_code (UCHAR *pin, UCHAR pin_length);

/** To get Default/Global PIN Code */
/**
 *  \brief To get the default Bluetooth PIN Code
 *
 *  \par Description:
 *       This API returns the default Bluetooth PIN Code set for the Security
 *       Manager.
 *
 *  \param [out] pin
 *         Caller allocated UCHAR array of at least PIN_CODE_SIZE octets onto
 *         which the default PIN Code will be copied.
 *
 *  \param [out] pin_length
 *         Pointer to a caller allocated UCHAR variable onto which the length of
 *         the default Bluetooth PIN Code will be copied.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_get_pin (UCHAR* pin, UCHAR* pin_length);

/** To set Pairable/Bondable Mode */
/**
 *  \brief To set the pairability/bondability of the stack.
 *
 *  \par Description:
 *       This API sets the pairable/non-pairable/bondable/non-bondable mode
 *       of the stack.
 *
 *  \param [in] mode
 *         This parameter specifies the mode to be set.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_set_pairable (UCHAR mode);

/** To add a remote Bluetooth device to the SM Device Database */
/**
 *  \brief To add a remote Bluetooth device into SM's Device Database.
 *
 *  \par Description:
 *       This API enables the application to add a remote Bluetooth device, as
 *       identified by its Bluetooth Device Address, to the Security Manager's
 *       Device Database if it does not exist.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device which needs to be
 *         added to the SM Device Database.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_add_device (UCHAR *bd_addr);

/** To set a remote Bluetooth device as 'Trusted' or 'Untrusted' */
/**
 *  \brief To mark a remote Bluetooth device as "Trusted".
 *
 *  \par Description:
 *       This API enables the application to control the trust-level of a remote
 *       Bluetooth device, as identified by its Bluetooth Device Address. The
 *       specified remote Bluetooth device must exist in the device database.
 *
 *  \param [in] bd_addr
 *
 *  \param [in] flag
 *         This parameter specifies what should be the new trust-level for the
 *         specified device.
 *         The values are:
 *         0x00 - Untrusted.
 *         0x01 - Trusted.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_set_device_trusted (UCHAR *bd_addr, UCHAR flag);

/** To set name for a remote Bluetooth device */
/**
 *  \brief To specify name for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to specify name for a remote Bluetooth
 *       device. The name may be obtained by application using HCI Remote Name
 *       Request procedure or by any other means.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which name to be set.
 *
 *  \param [in] name
 *          This parameter holds the name, and it must be a pointer to an
 *          UCHAR buffer. Application can pass NULL only if the name_length is
 *          Zero.
 *
 *  \param [in] name_length
 *         This parameter specifies the size of the name - size of the
 *         UCHAR buffer as specified in the parameter name. The length of name
 *         can be Zero, but must not be more than SM_DEVICE_NAME_SIZE octets.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_set_device_name
           (UCHAR *bd_addr, UCHAR *name, UCHAR name_length);

/** To get name for a remote Bluetooth device */
/**
 *  \brief To get specified name for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to get the name for a remote device,
 *       stored in the Device Database.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which name to
 *         be get.
 *
 *  \param [out] name
 *         Caller allocated UCHAR array of at least SM_DEVICE_NAME_SIZE octets
 *         onto which the device's name will be copied.
 *
 *  \param [out] name_length
 *         Pointer to a caller allocated UCHAR variable onto which the size of
 *         the device's name will be copied.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_get_device_name
           (UCHAR *bd_addr, UCHAR *name, UCHAR *name_length);

/** To set Bluetooth PIN (Passkey) for a remote Bluetooth device */
/**
 *  \brief To specify Bluetooth PIN Code for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to specify Bluetooth PIN Code for a remote
 *       Bluetooth device, which will be used by the Security Manager for
 *       authentication procedure when the local Bluetooth device attempts to
 *       connect (either link-level or service-level) to the specified remote
 *       device. The specified remote Bluetooth device must exist in the device
 *       database.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which Bluetooth
 *         PIN Code to be set.
 *
 *  \param [in] pin
 *         This parameter holds the Bluetooth PIN, and it must be a pointer to
 *         an UCHAR buffer. Application can pass NULL only if the pin_length is
 *         Zero.
 *
 *  \param [in] pin_length
 *         This parameter specifies the length of the PIN Code - size of the
 *         UCHAR buffer as specified in the parameter pin. The length of PIN
 *         Code can be Zero, but must not be more than PIN_CODE_SIZE octets.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_set_device_pin_code
           (UCHAR *bd_addr, UCHAR *pin, UCHAR pin_length);

/** To get Bluetooth PIN (Passkey) for a remote Bluetooth device */
/**
 *  \brief To get Bluetooth PIN Code for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to get the Bluetooth PIN Code for a remote
 *       device, stored in the Device Database.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which Bluetooth
 *         PIN Code to be get.
 *
 *  \param [out] pin
 *         Caller allocated UCHAR array of at least PIN_CODE_SIZE octets onto
 *         which the device's PIN Code will be copied.
 *
 *  \param [out] pin_length
 *         Pointer to a caller allocated UCHAR variable onto which the length of
 *         the device's Bluetooth PIN Code will be copied.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sm_get_device_pin_code
           (UCHAR *bd_addr, UCHAR *pin, UCHAR *pin_length);

/** To Set Link Key for a remote Bluetooth device */
#ifndef SM_LITE
/**
 *  \brief  To specify Bluetooth Link Key for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to specify Link Key for a remote Bluetooth
 *       device, which will be used by the Security Manager for link
 *       authentication procedure during link-level or service-level
 *       connection to/from the specified remote device. The specified remote
 *       Bluetooth device must exist in the device database.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which Bluetooth
 *         Link Key to be set.
 *
 *  \param [in] link_key
 *          This parameter holds the Link Key to be set, and it must be a
 *          pointer to an UCHAR buffer of 16 octets. NULL is not an acceptable
 *          parameter.
 */
#define BT_sm_set_device_link_key(bd, lk) \
        sm_device_link_key_and_type(SM_OPERATION_SET, (bd), (lk), NULL)

/**
 *  \brief  To specify Bluetooth Link Key and type for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to specify Link Key and type for a remote Bluetooth
 *       device, which will be used by the Security Manager for link
 *       authentication procedure during link-level or service-level
 *       connection to/from the specified remote device. The specified remote
 *       Bluetooth device must exist in the device database.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which Bluetooth
 *         Link Key to be set.
 *
 *  \param [in] link_key
 *          This parameter holds the Link Key to be set, and it must be a
 *          pointer to an UCHAR buffer of 16 octets. NULL is not an acceptable
 *          parameter.
 *
 *  \param [in] link_key_type
 *          This parameter holds the Link Key Type to be set
 */
#define BT_sm_set_device_link_key_and_type(bd, lk, type) \
        sm_device_link_key_and_type(SM_OPERATION_SET, (bd), (lk), (type))

/**
 *  \brief To get Bluetooth Link Key for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to retrieve Link Key for a remote
 *       Bluetooth device, which is be used by the Security Manager for
 *       authentication procedure during link-level or service-level connection
 *       to the specified remote device.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which Bluetooth
 *         Link Key to be get.
 *
 *  \param [out] link_key
 *         Caller allocated UCHAR array of 16 octets onto which the device's
 *         Link Key will be copied. NULL is not an acceptable parameter.
 */
#define BT_sm_get_device_link_key(bd, lk) \
        sm_device_link_key_and_type(SM_OPERATION_GET, (bd), (lk), NULL)

/**
 *  \brief  To get Bluetooth Link Key and type for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to retrieve Link Key and type for a remote Bluetooth
 *       device, which is used by the Security Manager for link
 *       authentication procedure during link-level or service-level
 *       connection to/from the specified remote device. The specified remote
 *       Bluetooth device must exist in the device database.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which Bluetooth
 *         Link Key to be set.
 *
 *  \param [in] link_key
 *          This parameter holds the Link Key to be set, and it must be a
 *          pointer to an UCHAR buffer of 16 octets. NULL is not an acceptable
 *          parameter.
 *
 *  \param [in] link_key_type
 *          This parameter holds the Link Key Type to be set
 */
#define BT_sm_get_device_link_key_and_type(bd, lk, type) \
        sm_device_link_key_and_type(SM_OPERATION_GET, (bd), (lk), (type))

/** To Delete Link Key (if any) for a remote Bluetooth device */
/**
 *  \brief To delete Bluetooth Link Key of a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to delete the Link Key for a remote
 *       Bluetooth device.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the device for which Link Key needs
 *         to be deleted.
 */
#define BT_sm_delete_device_link_key(bd) \
        sm_device_link_key_and_type(SM_OPERATION_RESET, (bd), NULL, NULL)

#ifdef BTSIG_ERRATA_11838
/**
 *  \brief To get the encryption key size with a specific device
 *
 *  \par Description:
 *  This API enables application to get the encryption key size for
 *  a remote device, stored in the Device Database.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device
 *         for which encryption key size to be get
 *  \param [out] state
 *         Pointer to a caller allocated memory of \ref SM_DEVICE_STATE
 *         onto which the encryption key size will be copied
 *
 *  \return API_RESULT:
 *          - API_SUCCESS: If successful.
 *          - Error Codes: An error code describing the cause of failure.
 */
API_RESULT BT_sm_get_device_security_state
           (
               /* IN */  UCHAR *    bd_addr,
               /* OUT */ SM_DEVICE_STATE * state
           );
#endif /* BTSIG_ERRATA_11838 */

#else  /* SM_LITE */
API_RESULT BT_sm_get_device_link_key_and_type
           (
               /* IN */    UCHAR *    bd_addr,
               /* INOUT */ UCHAR *    link_key,
               /* OUT   */ UCHAR *    link_key_type
           );

/** To Get Link Key (if any) for a remote Bluetooth device */
/**
 *  \brief To get Bluetooth Link Key for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to retrieve Link Key for a remote
 *       Bluetooth device, which is be used by the Security Manager for
 *       authentication procedure during link-level or service-level connection
 *       to the specified remote device.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which Bluetooth
 *         Link Key to be get.
 *
 *  \param [in] link_key
 *          Caller allocated UCHAR array of 16 octets onto which the device's
 *          Link Key will be copied. NULL is not an acceptable parameter.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_get_device_link_key(bd, lk)
        BT_sm_get_device_link_key_and_type((bd), (lk), NULL)
#endif /* SM_LITE */

/**
 *  To retrieve various attributes of a Device Database entry of a
 *  remote Bluetooth device
 */
/**
 *  \brief To get Security Manager's Device Database attributes of a remote Bluetooth device
 *
 *  \par Description:
 *       This API enables application to retrieve various attributes of a remote
 *       Bluetooth device from the Security Manager's Device Database entry.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which device
 *         attributes to be retrieved.
 *
 *  \param [in] dev_attr
 *         This parameter is a bit-field, each bit of which represents a
 *         specific detail about the device in the device database.
 *         The caller must allocate memory for this parameter and pass the
 *         pointer here. The meaning of each bit is given below:
 *         Bit 0: Trust Level
 *         0 - The device is not trusted.
 *         1 - The device is trusted.
 *         But 1: Authentication of the link-level connection
 *         0 - The link-level connection is not authenticated.
 *         1 - The link-level connection is authenticated.
 *         Bit 2: Encryption of the link-level connection
 *         0 - The link-level connection is not encrypted.
 *         1 - The link-level connection is encrypted.
 *         Bit 3: Link Key Availability
 *         0 - Link Key is not available for the device.
 *         1 - Link Key is available for the device.
 *         Bit 4: PIN Code Availability
 *         0 - The Bluetooth PIN is not configured for the device.
 *         1 - The Bluetooth PIN is configured for the device.
 *         Bit 5-7: Unused.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_sm_get_device_attributes (UCHAR *bd_addr, UCHAR *dev_attr);

/* To delete devices from SM Device Database based on Operation Types */
/**
 *  \brief To delete a Security Manager Device Database entry for a remote
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API deletes a device from the Device Database for a specific
 *       remote Bluetooth device, as identified by its Bluetooth Device Address,
 *       and according to the based on the operation type.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device for which Device
 *         Database entry to be deleted.
 *
 *  \param [in] op_type
 *         This parameter specifies how deletion operation to be performed.
 *         The valid values and their meaning are specified below:
 *         SM_PAIR_LIST: Delete a "Paired" device
 *         SM_TRUSTED_LIST : Delete a "Trusted" dveice
 *         SM_ANY_LIST: Force deletion of a device from the database,
 *         irrespective of whether the device is "Paired", "Trusted" or "Untrusted"
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_delete_device (UCHAR *bd_addr, UCHAR op_type);

/** To get a list of Device Database entries based on Operation Types */
/**
 *  \brief To get the list of "Paired" and "Trusted" device lists from the
 *         Device Database
 *
 *  \par Description:
 *       This function returns the list of "Paired" and/or "Trusted" remote
 *       Bluetooth devices available in the Security Manager's Device Database.
 *
 *
 *  \param [in] op_type
 *         This parameter specifies which list of devices to be returned.
 *         The valid values and their meaning are specified below:
 *         SM_PAIR_LIST: To get list of "Paired" devices
 *         SM_TRUSTED_LIST: To get list of "Trusted" devices
 *         SM_ANY_LIST: To get list of devices that are present in the Device Database
 *
 *  \param [out] peer_info
 *         Pointer to a caller allocated SM_PEER_INFO Array, of size count,
 *         variable where the list of devices, as requested by the op_type
 *         parameter, will be returned.
 *
 *  \param [in,out] count
 *         Number of devices found in the database. This is an INOUT parameter.
 *         On input, it should contain the size of the SM_PEER_INFO Array
 *         allocated for peer_info. On output, it contains actual number of
 *         device information returned in peer_info.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_get_device_list
           (SM_PEER_INFO *peer_info, UCHAR op_type, UCHAR *count);

#ifdef SM_HAVE_MODE_2
/** To register Security requirements of a Service */
/**
 *  \brief To add a Service, and its security needs, in the SM Service Database.
 *
 *  \par Description:
 *       This API enables Profiles/applications to register security related
 *       requirements for a service. Upon successful registration,
 *       Security Manager allocates a unique Service Identifier and returns to
 *       the caller, which can later be used to refer to the service.
 *
 *  \param [in] service_attr
 *         This parameter specifies the security attributes of the service that
 *         needs to be registered. The caller must allocate a SM_SERVICE data
 *         type, populate the required fields, and pass the pointer of it.
 *
 *  \param [out] service_id
 *         This parameter specifies the unique Service Identifier, allocated by
 *         the Security Manager and returned. The profiles/applications can use
 *         this value for further operations on the registered service
 *         (for example, delete a service from the service database,
 *         change its security attributes etc.). The caller must allocate a
 *         variable of type UCHAR and pass pointer of it.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Normally, the Service Database is used to implement Service-level
 *        security (Security Mode 2). However, it is possible to use SM Service
 *        Database for Service Authorization in Security Mode 1 or 3.
 *        For details, see SM Service Authorization in Security Mode 1 and 3.
 *
 */
API_RESULT BT_sm_add_service
           (
               SM_SERVICE *    serv_attr,
               UCHAR *         service_id
           );

/** To de-register Security requirements of a Service */
/**
 *  \brief To delete a Service, and its security needs, from the SM's Service Database.
 *
 *  \par Description:
 *       This API enables profiles/applications to de-register security related
 *       requirements for a service, as identified by its uniquely allocated
 *       Service Identifier.
 *
 *  \param [in] si
 *         This parameter specifies the unique Service Identifier, identifying
 *         the service, which needs to de-register. This value must have been
 *         obtained using BT_sm_add_service() API.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *
 */
API_RESULT BT_sm_delete_service (UCHAR si);

/** To set Security requirements of a Service */
/**
 *  \brief To set/change security attributes of a registered service in the SM's
 *         Service Database.
 *
 *  \par Description:
 *       This API enables profiles/applications to set/change service level
 *       security requirement for a registered service.
 *
 *  \param [in] service_id
 *         This parameter specifies the unique Service Identifier, identifying
 *         the service. This value must have been obtained using
 *         BT_sm_add_service() API.
 *
 *  \param [in] service_attr
 *         This parameter specifies the security attributes of the specified
 *         service. The caller must allocate a SM_SERVICE data type, populate
 *         the required fields, and pass the pointer of it. Note that the psm
 *         and server_ch fields of service_attr will be ignored, as they cannot
 *         be changed for a registered service.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_set_service_attributes(sid, attr) \
        sm_service_attributes(SM_OPERATION_SET, (sid), (attr))

/** To get Security requirements of a Service */
/**
 *  \brief To get the security attributes of a registered service from the SM's
 *         Service Database.
 *
 *  \par Description:
 *       This API enables profiles/applications to retrieve service level
 *       security requirement for a registered service.
 *
 *  \param [in] service_id
 *         This parameter specifies the unique Service Identifier, identifying
 *         the service. This value must have been obtained using
 *         BT_sm_add_service() API.
 *
 *  \param [out] service_attr
 *         The caller must allocate a SM_SERVICE data type, and pass the pointer
 *         of it. The service attributes will be copied onto this variable on return.
 */
#define BT_sm_get_service_attributes(sid, attr) \
        sm_service_attributes(SM_OPERATION_GET, (sid), (attr))

#endif /* SM_HAVE_MODE_2 */

/** To register presence of a manual User Interface */
/**
 *  \brief To register the UI Notification Callback with SM
 *
 *  \par Description:
 *       This API enables applications to register the callback function with
 *       Security Manager to receive the events as explained above. It is
 *       mandatory to register this callback.
 *
 *  \param [in] notify_cb
 *         This parameter specifies the SM User Interface Callback function tha
 *         the Security Manager should call to notify various events that
 *         requires user/application's intervention. This parameter cannot be NULL.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_register_user_interface
           (
               SM_UI_NOTIFY_CB    notify_cb
           );

/** To reply to a Connection Request from the SM */
/**
 *  \brief To reply to the SM Connection Request Event
 *
 *  \par Description:
 *       This API enables applications to accept/reject an incoming link-level
 *       connection from a "Untrusted" remote Bluetooth device.
 *       This API must be used to reply when Security Manager calls the
 *       registered UI Notification Callback with SM_CONNECTION_REQUEST_NTF
 *       as the event identifier.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote "Untrusted" Bluetooth device.
 *
 *
 *  \param [in] accept
 *         This parameter specifies whether the incoming link-level
 *         (ACL) connection from the specified remote Bluetooth device needs
 *         to be accepted or rejected.
 *         The valid values are:
 *         0x00 - reject the incoming link-level (ACL) connection.
 *         0x01 - accept the incoming link-level (ACL) connection.
 *
 *  \param [in] reason
 *         This parameter specifies the reason for rejecting a connection.
 *         This parameter must be set to one of the Host Controller Error Codes
 *         as described in the Specification of
 *         the Bluetooth System, v1.2, Vol. 2, Part D - Error Codes.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_connection_request_reply(bd_addr, accept, reason) \
        sm_ui_notification_request_reply \
        ((bd_addr), (accept), (reason), SM_ACL_CONNECT_REQUEST_NTF)

#ifdef SM_HAVE_MODE_2
/** To reply to a Authorisation Request event callback from the SM */
/**
 *  \brief To reply to the SM Authorization Request Event (in Security Mode 2)
 *
 *  \par Description:
 *       This API enables applications to accept/reject (authorize) an incoming
 *       service-level connection from a device (un-trusted).
 *       This API must be used to reply when Security Manager calls the
 *       registered UI Notification Callback with SM_AUTHORIZATION_REQUEST_NTF
 *       as the event identifier.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote "Untrusted" Bluetooth device.
 *
 *  \param [in] accept
 *         This parameter specifies whether the incoming service-level
 *         connection from the specified remote Bluetooth device needs to be
 *         rejected or accepted (authorized).
 *         The valid values are:
 *         0x00 - reject the incoming service-level connection.
 *         0x01 - accept the incoming service-level connection.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_authorization_request_reply(bd_addr, accept) \
        sm_ui_notification_request_reply \
        ((bd_addr), (accept), 0x0U, SM_AUTHORIZATION_REQUEST_NTF)

/** To reply to a Authorisation Request (Mode 1/3) event callback from the SM */
/**
 *  \brief To reply to the SM Authorization Request Event in Security Mode 1 or 3.
 *
 *  \par Description:
 *       This API enables applications to accept/reject (authorize) an incoming
 *       service-level connection from a device, in Security Mode 1 or 3.
 *       This API must be used to reply when Security Manager calls the
 *       registered UI Notification Callback with
 *       SM_AUTHORIZATION_REQUEST_NTF_MODE_1_3 as the event identifier.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote Bluetooth device.
 *
 *  \param [in] accept
 *         This parameter specifies whether the incoming service-level
 *         connection from the specified remote Bluetooth device needs to be
 *         rejected or accepted (authorized).
 *         The valid values are:
 *         0x00 - reject the incoming service-level connection.
 *         0x01 - accept the incoming service-level connection.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_authorization_request_reply_mode_1_3(bd_addr, accept) \
        sm_ui_notification_request_reply \
        ((bd_addr), (accept), 0x0U, SM_AUTHORIZATION_REQUEST_NTF_MODE_1_3)
#endif /* SM_HAVE_MODE_2 */

/** To reply to a Bluetooth PIN Code Request event callback from the SM */
/**
 *  \brief To reply to the SM PIN Code Request Event
 *
 *  \par Description:
 *       This API enables applications to furnish Bluetooth PIN for
 *       authentication procedure on outgoing link-level connection to a device.
 *       This API must be used to reply when Security Manager calls
 *       the registered UI Notification Callback with SM_PIN_CODE_REQUEST_NTF
 *       as the event identifier. The application can send either a positive
 *       or a negative reply to a PIN Code request from
 *       the local Bluetooth device (controller).
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device.
 *
 *  \param [in] pin
 *         This parameter holds the Bluetooth PIN, and it must be a pointer to
 *         an UCHAR buffer. Application can pass NULL only if the pin_length
 *         is Zero.
 *
 *  \param [in] pin_length
 *         This parameter specifies the length of the PIN Code - size of the
 *         UCHAR buffer as specified in the parameter pin. The length of
 *         PIN Code can be Zero, but must not be more than 16 octets.
 *         To send a PIN Code Negative Reply, the application must
 *         call this API with pin_length as 0.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_sm_pin_code_request_reply(bd, p, pl) \
        hci_sm_pin_code_request_reply((bd), (p), (pl))

/** To reply to a Bluetooth Link Key Request event callback from SM */
/**
 *  \brief To reply to the SM Link Key Request Event
 *
 *  \par Description:
 *       This API enables applications to furnish the Link Key for
 *       authentication procedure on the link-level connection to a device.
 *       This API must be used to reply when Security Manager calls the
 *       registered UI Notification Callback with SM_LINK_KEY_REQUEST_NTF
 *       as the event identifier. The application can send either a positive
 *       or a negative reply to a Link Key request from the
 *       local Bluetooth device (controller).
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote (un-trusted) device.
 *
 *  \param [in] link_key
 *         This parameter holds the Bluetooth Link Key. It must be a pointer to
 *         an UCHAR buffer of size 16 octets. NULL is an acceptable value,
 *         only if the link_key_present is 0.
 *
 *  \param [in] link_key_present
 *         The application can set this flag to 0, if the Bluetooth Link Key,
 *         for the specified remote Bluetooth device, is not available,
 *         to generate the Link Key Request Negative Reply message to the
 *         local Bluetooth device. If link key is available,
 *         this flag must be set to 1.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *
 */
#define BT_sm_link_key_request_reply(bd, lk, lkp) \
        hci_sm_link_key_request_reply((bd), (lk), (lkp))

/** To change Link Key of the ACL Connection with a remote device */
/**
 *  \brief To request changing the ACL connection Link Key
 *
 *  \par Description:
 *       This API enables application to change the Link Key for a device,
 *       after an ACL connection has been established.
 *       This link key will be used for subsequent authentication requests.
 *       The local Bluetooth device starts the Bluetooth Change Link Key
 *       process for the ACL link on reception of the HCI Change Connection
 *       Link Key Command, and sends a Command Status Event to the HCI.
 *       At the end of the Encryption process,
 *       the Change Connection Link Key Complete Event will be received.
 *
 *  \param [in] bd_addr
 *         The address of the remote device for which the link key should be
 *         changed.
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *
 */
API_RESULT BT_sm_change_connection_link_key (UCHAR *bd_addr);

/* To request Authentication of the ACL Connection with a remote device */
/**
 *  \brief To request for link-level Authentication of a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to initiate ACL link Authentication
 *       procedures, after an ACL link has been established with
 *       a remote Bluetooth device.
 *       This API is useful when the Security Mode of the Security Manager is
 *       either 1 or 2. In addition, this API can also be used under
 *       the following circumstances:
 *       i)When the ACL link Authentication needs to be delayed, so that initial
 *       control information can be exchanged with the remote Bluetooth device
 *       (for example, SDP queries)
 *       ii)When it needs to be ensured that the ACL link is really authenticated.
 *       Several profiles need to ensure that the link is authenticated before
 *       starting data transfer.
 *       The local Bluetooth device starts the Bluetooth Authentication process
 *       for the ACL link on reception of the HCI Authentication Requested Command,
 *       and sends a Command Status Event to the HCI. At the end of
 *       the Authentication process, the Authentication Complete Event
 *       will be received.
 *       Before receiving the Authentication Complete Event, Bluetooth Security
 *       related events (Link Key Request Event and/or PIN Code Request Event)
 *       might be received, depending on the security settings on the local
 *       and/or remote Bluetooth device.
 *       For details on the Authentication procedure, refer to the Specification
 *       of the Bluetooth System, v1.2, Vol. 2, Part F - Message Sequence Charts.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote Bluetooth device for which
 *         the ACL link should be authenticated.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_authentication_request (UCHAR *bd_addr);

/* To enable/disable Encryption of data on the ACL Connection */
/**
 *  \brief To request for link-level Encryption for a remote Bluetooth device.
 *
 *  \par Description:
 *       This API enables application to initiate ACL link Encryption procedures,
 *       after an ACL link has been established with a remote Bluetooth device.
 *       This API is useful when the Security Mode of the Security Manager is
 *       either 1 or 2.
 *       In addition, this API can also be used under the following circumstances:
 *       i) When the ACL link Encryption needs to be delayed, so that initial
 *       control information can be exchanged with the remote Bluetooth device
 *       (for example, SDP queries)
 *       ii)When it needs to be ensured that the ACL link is really encrypted.
 *       Several profiles need to ensure that the link is encrypted before
 *       starting data transfer.
 *       The local Bluetooth device starts the Bluetooth Encryption process for
 *       the ACL link on reception of the HCI Change Connection Encryption Command,
 *       and sends a Command Status Event to the HCI.
 *       At the end of the Encryption process, the Encryption Change Event
 *       will be received.
 *       For details on the Encryption procedure, refer to the Specification of
 *       the Bluetooth System, v1.2, Vol. 2, Part F - Message Sequence Charts.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote Bluetooth device for which
 *         the ACL link should be encrypted.
 *
 *  \param [in] enable
 *          This parameter defines the kind of encryption to be requested.
 *          The valid values are:
 *          0 - no encryption
 *          1 - enable point-to-point encryption
 *          2 - enable broadcast encryption
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_sm_encryption_request (UCHAR *bd_addr, UCHAR enable);

/* To set the PIN (Variable/Fixed) Type */
/**
 *  \brief To set the Variable/Fixed PIN Type in the Bluetooth hardware.
 *
 *  \par Description:
 *       This function can be used to configure the local Bluetooth device to
 *       use a Variable or Fixed PIN Code, when replying to a PIN Code Request event.
 *
 *  \param [in] pin_type
 *         The PIN Type to be configured: SM_VARIABLE_PIN or SM_FIXED_PIN
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_set_pin_type(pt)      hci_sm_set_pin_type((pt))

/** To change Master Link Key */
/**
 *  \brief To change the Link Key of the Piconet Master Bluetooth device.
 *
 *  \par Description:
 *       This API can be used to force the device that is master of the Piconet
 *       to use the temporary link key of the master device,
 *       or the semi-permanent link keys. The temporary link key is used for
 *       encryption of broadcast messages within a Piconet,
 *       and the semi-permanent link keys are used for private encrypted
 *       point-to-point communication.
 *
 *  \param [in] key_flag
 *         This is to indicate which Link Key (temporary link key of the Master,
 *         or the semi-permanent link keys) shall be used.
 *         Value 0x00: Use semi-permanent Link Key
 *         Value 0x01: Use Temporary Link
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_master_link_key(kf)   hci_sm_master_link_key((kf))

#ifdef BT_SSP_UC
/**
 *  \brief To reply to the SM User Confirmation Request Event
 *
 *  \par Description:
 *       This API enables applications to accept/reject User Confirmation request,
 *       during link authentication using Secure Simple Pairing procedure
 *       (Numeric Comparison association model).
 *       This API must be used to reply when Security Manager calls the
 *       registered UI Notification Callback with SM_USER_CONF_REQUEST_NTF
 *       as the event identifier. The application can send either accept or
 *       reject to a User Conf request from the local Bluetooth device (controller).
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device.
 *
 *  \param [in] accept
 *          This parameter specifies whether the User Confirmation request for
 *          the specified remote Bluetooth device needs to be accepted or rejected.
 *          The valid values are:
 *          0x00 - reject the User Confirmation request.
 *          0x01 - accept the User Confirmation request.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_user_conf_request_reply(bd, at) \
        hci_sm_user_confirmation_request_reply((bd), (at))
#endif /* BT_SSP_UC */

#ifdef BT_SSP_PE
/**
 *  \brief To reply to the SM User Passkey Request Event
 *
 *  \par Description:
 *       This API enables applications to furnish the Passkey, during link
 *       authentication using Secure Simple Pairing procedure
 *       (Passkey Entry association model).
 *       This API must be used to reply when Security Manager calls the
 *       registered UI Notification Callback with SM_USER_PASSKEY_REQUEST_NTF
 *       as the event identifier.
 *       The application can send either accept or reject to a User Conf request
 *       from the local Bluetooth device (controller).
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote device.
 *
 *  \param [in] passkey
 *         This parameter holds the Passkey, and it must be a pointer to an
 *         UCHAR buffer. If application wishes to reject the Passkey Request,
 *         it may pass NULL to this parameter.
 *
 *  \return
 *         API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
#define BT_sm_user_passkey_request_reply(bd, pk) \
        hci_sm_user_passkey_request_reply((bd), (pk))
#endif /* BT_SSP_PE */

#ifdef BT_BRSC
/** To enable/disable the Secure Connections Only Mode */
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return None
 *
 *  \note
 *
 */
API_RESULT BT_sm_set_secure_connections_only_mode(UCHAR enable);

/** To get current Secure Connections Only Mode */
/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param None
 *
 *  \return None
 *
 *  \note
 *
 */
API_RESULT BT_sm_get_secure_connections_only_mode(UCHAR * enable);
#endif /* BT_BRSC */

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_SM_API_ */


