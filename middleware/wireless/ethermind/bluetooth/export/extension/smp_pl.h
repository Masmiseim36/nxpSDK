
/**
 *  \file smp_pl.h
 *
 *  This Header File contains the platform specific interfaces
 *  exported by EtherMind Bluetooth Stack for the SMP
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SMP_PL_
#define _H_SMP_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_smp_api.h"

#ifdef SMP_HAVE_TBX_PL_ECDH
#include "crypto.h"
#endif /* SMP_HAVE_TBX_PL_ECDH */

/**
 * \defgroup smp_pl_module SMP Platform (Security Manager Protocol Platform)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Security Manager Protocol (SMP) platform module to the Application and the
 *  core SMP layer.
 */

/**
 * \defgroup smp_pl_defines Defines
 * \{
 * This section describes the defines for SMP Platform
 */
/* --------------------------------------------- Global Definitions */

/**
 * \defgroup smp_pl_iocaps SMP Platform IO Capabilities
 * \{
 *  This section lists the commands of SMP IO Capabilities defined by
 *  specification, and the default IO capability used by the platform
 */

/**
 * To indicate platform supports only a mechanism to display/conway 6 didgit
 * number to user.
 */
#define SMP_IO_CAPABILITY_DISPLAY_ONLY             0x00

/**
 * To indiate platform supports at least two buttons that are mapped easily to
 * 'yes' and 'no' or the device has a mechanism whereby the user can indicate
 * either 'yes' or 'no' and a mechanism through which 6 didgit numeric value
 * can be displayed/cowayed to the user.
 */
#define SMP_IO_CAPABILITY_DISPLAY_YESNO            0x01

/**
 * To indicate platform supports only numeric keyboard that can input the
 * numbers '0' through '9'.
 */
#define SMP_IO_CAPABILITY_KEYBOARD_ONLY            0x02

/**
 * To indicate platform does not have the ability to display or communicate
 * a 6 digit decimal number.
 */
#define SMP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT       0x03

/**
 * To indicate platform supports a mechanism through which 6 didgit numeric
 * value can be displayed and numeric keyboard that can input the numbers '0'
 * through '9'.
 */
#define SMP_IO_CAPABILITY_KEYBOARD_DISPLAY         0x04

/** Default IO capability */
#define SMP_DEFAULT_IO_CAPABILITY  SMP_IO_CAPABILITY_KEYBOARD_DISPLAY

/** \} */

/**
 * \defgroup smp_pl_constants SMP Platform Constants
 * \{
 *  This section lists the constants used by the SMP Platform
 */

#ifdef SMP_HAVE_OOB_SUPPORT
/**
 * OOB Blob Data size - should be configured for
 * the specific platform or usecase
 */
#define SMP_OOB_BLOB_DATA_SIZE                     50

/** OOB Temporary key size. Specification defined */
#define SMP_OOB_TEMP_KEY_SIZE                      16

#ifdef SMP_LESC
/** OOB Confirm Value Size. Specification defined */
#define SMP_OOB_CONFIRM_VAL_SIZE                   16

/** OOB Confirm Value Size. Specification defined */
#define SMP_OOB_RANDOM_VAL_SIZE                    16

/* OOB Public Key X coordinate Size. Specification defined */
#define SMP_OOB_PUBLIC_KEY_X_SIZE                  32

/* OOB Public Key Size. Specification defined */
#define SMP_OOB_PUBLIC_KEY_SIZE                    64

/** OOB Data Generation States */
#define SMP_OOB_STATE_IDLE                         0x01
#define SMP_OOB_STATE_RAND_GEN                     0x02
#define SMP_OOB_STATE_PUB_KEY_GEN                  0x04
#define SMP_OOB_STATE_CONF_VAL_GEN                 0x08

/* State Handling Macros */
#define SMP_LESC_OOB_SET_STATE(x, s)          (x)->state = (s)

#endif /* SMP_LESC */
#endif /* SMP_HAVE_OOB_SUPPORT */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
/* Mode of Cross Transport Key Generation */
#define SMP_LESC_LK_FROM_LTK   0x01
#define SMP_LESC_LTK_FROM_LK   0x02

/* States of Cross Transport Key Generation */
#define SMP_LESC_LK_LTK_IDLE   0x00
#define SMP_LESC_TEMP1_GEN     0x01
#define SMP_LESC_LK_GEN        0x02
#define SMP_LESC_TEMP2_GEN     0x10
#define SMP_LESC_LTK_GEN       0x20

#define SMP_LESC_LK_LTK_SET_STATE(x,s)        (x)->state = (s)
#define SMP_LESC_LK_LTK_GET_STATE(x)          (x)->state
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/** Max Encryption key size in platform */
#define SMP_MAX_ENCRYPTION_KEY_SIZE_PL             SMP_MAX_ENCRYPTION_KEY_SIZE

/** Invalid platform attribute value */
#define SMP_INVALID_ATTRIBUTE_PL                   0x00

/** Device platform attribute update operations */
#define SMP_DEVICE_ATTR_PL_DELETE                  0x01
#define SMP_DEVICE_ATTR_PL_CONNECTION_COMPLETE     0x02
#define SMP_DEVICE_ATTR_PL_AUTHENTICATION_COMPLETE 0x03

/** Platform device list size */
#define SMP_PL_DEVICE_LIST_SIZE                    2

/** Platform MITM policy */
#define SMP_PL_SEC_MITM_BEST_EFFORT                0
#define SMP_PL_SEC_MITM_STRICT                     1

#ifdef SMP_LESC
#define SMP_PL_LESC_BEST_EFFORT                    0
#define SMP_PL_LESC_STRICT                         1

#define SMP_PL_LESC_KEY_PRESS_NTF_RESET            0
#define SMP_PL_LESC_KEY_PRESS_NTF_SET              1

/* LESC KeyPress Notification Values */
#define SMP_LESC_PASSKEY_ENTRY_STARTED             0x00
#define SMP_LESC_PASSKEY_DIGIT_ENTERED             0x01
#define SMP_LESC_PASSKEY_DIGIT_ERASED              0x02
#define SMP_LESC_PASSKEY_CLEARED                   0x03
#define SMP_LESC_PASSKEY_ENTRY_COMPLETED           0x04
#endif /* SMP_LESC */

/** \} */

/** \} */


/**
 * \defgroup smp_pl_structures SMP Platform Data Structures
 * \{
 * This section defines the data structures used in SMP Pl.
 */
/* --------------------------------------------- Structures/Data Types */

/** SMP Platform device attribute */
typedef UCHAR SMP_DEVICE_ATTR_PL;

#ifdef SMP_HAVE_OOB_SUPPORT
/** SMP Platform device */
typedef struct _SMP_DEVICE_PL
{
    /* Peer device address */
    BT_DEVICE_ADDR bd_addr;

    /* Validity */
    UCHAR used;

    /* OOB flag */
    UCHAR oob_flag;

    /* Temporary Key - 16 Octets */
    UCHAR tk[SMP_OOB_TEMP_KEY_SIZE];

#ifdef SMP_LESC
    /*
     * SMP LESC Specific Peer OOB Details
     * 1. Confirm Value
     * 2. Random number 'r'
     */
    /* Confirm Value - 16 Octets */
    UCHAR cnf_val[SMP_OOB_CONFIRM_VAL_SIZE];

    /* Random Value - 16 Octets */
    UCHAR rand[SMP_OOB_RANDOM_VAL_SIZE];

#endif /* SMP_LESC */

    /*
     * OOB Blob Data - to be taken care by platform specific implementation.
     * In the default implementaiton, this is only can be stored and retrived.
     */
    UCHAR blob[SMP_OOB_BLOB_DATA_SIZE];

} SMP_DEVICE_PL;

/** SMP Data Structure for Application OOB set and get Interface */
typedef struct _SMP_OOB_DATA
{
    /* Temporary Key - 16 Octets */
    UCHAR temp_key[SMP_OOB_TEMP_KEY_SIZE];

#ifdef SMP_LESC
    /*
     * SMP LESC Specific Peer OOB Details
     * 1. Confirm Value
     * 2. Random number 'r'
     */
    /* Confirm Value - 16 Octets */
    UCHAR lesc_cnf_val[SMP_OOB_CONFIRM_VAL_SIZE];

    /* Random Value - 16 Octets */
    UCHAR lesc_rand[SMP_OOB_RANDOM_VAL_SIZE];
#endif /* SMP_LESC */

} SMP_OOB_DATA;

#ifdef SMP_LESC
/** SMP LESC specific OOB related data struct */
typedef struct _SMP_LESC_OOB_DATA_PL
{
    UCHAR state;

    /*
     * SMP LESC Specific Peer OOB Details
     * 1. Confirm Value
     * 2. Random number 'r'
     */
    /* Confirm Value - 16 Octets */
    UCHAR cnf_val[SMP_OOB_CONFIRM_VAL_SIZE];

    /* Random Value - 16 Octets */
    UCHAR rand[SMP_OOB_RANDOM_VAL_SIZE];

    /* Public Key - 64 Octets */
    UCHAR pkey[SMP_OOB_PUBLIC_KEY_SIZE];

} SMP_LESC_OOB_DATA_PL;

#ifdef SMP_SAVE_REMOTE_IOCAP
/** IO Capabilities of a device */
typedef struct _SMP_IOCAPS
{
    /** I/O Capability */
    UCHAR iocap;

    /** OOB Availability */
    UCHAR oob;

    /** Max Keysize exchanged */
    UCHAR ekey_size;

} SMP_IOCAPS;
#endif /* SMP_SAVE_REMOTE_IOCAP */

/**
 * SMP LESC OOB data generation complete callback.
 * Security Manager Protocol PL calls the registered callback
 * to indicate LESC specific OOB data generation completion.
 *
 * \param bd_addr Pointer to peer device address that is resolved
 */
typedef void (* SMP_LESC_OOB_GEN_COMPLETE_CB)
             (
                 /* OUT */ SMP_LESC_OOB_DATA_PL * lesc_oob
             ) DECL_REENTRANT;
#endif /* SMP_LESC */
#endif /* SMP_HAVE_OOB_SUPPORT */

#ifdef SMP_LESC
#ifdef SMP_HAVE_TBX_PL_ECDH
/* Data Type Defines for PL ECDH Utility */
typedef ssp_prkey_t SMP_PL_ECDH_PRIV_KEY;
typedef ssp_pukey_t SMP_PL_ECDH_PUB_KEY;
typedef ssp_dhkey_t SMP_PL_ECDH_DHKEY;
#endif /* SMP_HAVE_TBX_PL_ECDH */
#endif /* SMP_LESC */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
/** SMP LESC specific Cross Transport Key derivation related data struct */
typedef struct _SMP_LESC_LK_LTK_GEN_PL
{
    UCHAR state;

    /* lk Octets */
    UCHAR lk[SMP_LK_SIZE];

    /* LTK 16 Octets */
    UCHAR ltk[SMP_LTK_SIZE];

    /* Temp Key 16 Octets */
    UCHAR temp[SMP_AES_CMAC_MAC_SIZE];

} SMP_LESC_LK_LTK_GEN_PL;

/**
 * SMP LESC LK LTK key generation complete callback.
 * Security Manager Protocol PL calls the registered callback
 * to indicate LESC specific LK LTK key generation completion.
 *
 * \param bd_addr Pointer to peer device address that is resolved
 */
typedef void (* SMP_LESC_LK_LTK_GEN_COMPLETE_CB)
             (
                 /* OUT */ SMP_LESC_LK_LTK_GEN_PL * lk_ltk
             ) DECL_REENTRANT;
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/** \} */

/* --------------------------------------------- Macros */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define BT_smp_get_lk_from_ltk_pl(k,h,ct) \
        BT_smp_get_lk_ltk_pl(SMP_LESC_LK_FROM_LTK, (k), (h), (ct))

#define BT_smp_get_ltk_from_lk_pl(k,h,ct) \
        BT_smp_get_lk_ltk_pl(SMP_LESC_LTK_FROM_LK, (k), (h), (ct))
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/**
 * \defgroup smp_pl_internal_defs Platform Internal Definitions
 * \{
 *  This section describes Internal functions used by SMP core.
 */
/* --------------------------------------------- Internal Functions */

/**
 * \brief To perform platform initializations for SMP
 *
 * \par Description:
 *      This internal platform API is used to make platform
 *      specific initializations during the initialization of
 *      internal SMP module
 *
 * \return void
 */
void smp_init_pl (void);

/**
 * \brief To purge internal device list
 *
 * \par Description:
 *      This internal platform API is used to purge and manage the internal
 *      device list. This API is called when SMP device database is full
 *      and there is no space to add new device.
 *
 * \param [out] free_index  Index of freed device in database
 *
 * \return API_RESULT:
 *         - API_SUCCESS: If successful.
 *         - Error Codes: An error code describing the cause of failure.
 */
API_RESULT smp_purge_device_list_pl(UCHAR *free_index);

/**
 * \brief To update the platform specific device attrbute.
 *
 * \par Description:
 *      This is internal platform API to update/delte platform specific device.
 *      This API is called whenever there connection/reconnection or
 *      pairing/authentication or disconnection.
 *
 * \param [in] operation  The operation to be perfomred on the device.
 *        - \ref SMP_DEVICE_ATTR_PL_DELETE To deleted platform specific
 *               device attribute.
 *        - \ref SMP_DEVICE_ATTR_PL_CONNECTION_COMPLETE To indicate connection
 *               is been establish to remote device and needs update platform
 *               specifci attribute.
 *        - \ref SMP_DEVICE_ATTR_PL_AUTHENTICATION_COMPLETE To indicate
 *
 * \param [in] di  The remote device database index.
 *
 * \return void
 */
void smp_update_device_attr_pl (UCHAR operation, UCHAR di);

/**
 * \brief To check for security requirement
 *
 * \par Description:
 *      This internal platform API is used to assert for required security for
 *      a link during SMP authentication procedure.
 *
 * \param [in] auth_req  Required Authentication, as per auth_req format in spec
 * \param [in] auth_poss  Possible Authentication, as per auth_req format in spec
 * \param [in] assoc  IOCAP Association model possible
 *
 * \return API_SUCCESS if requirement met, API_FAILURE if requirement failed to
 * meet
 *
 * \note Currently, implementation does checking only for MITM
 */
API_RESULT smp_check_pairing_security_pl
           (
               UCHAR auth_req,
               UCHAR auth_poss,
               UCHAR assoc
           );

#ifdef SMP_LESC
API_RESULT smp_get_lesc_key_ntf_pl(void);
#endif /* SMP_LESC */

#ifdef SMP_HAVE_OOB_SUPPORT
/**
 * \brief To get OOB availability for a peer device
 *
 * \par Description:
 *      This API retrieves OOB presence flag for the given peer device.
 *      Used by SMP, during authentication procedure.
 *
 * \param [in] di  Peer device for which the OOB availability to be check.
 * \param [out] oob  Pointer to OOB data presence flag
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
void smp_get_peer_oob_availability_pl
     (
         /* IN */  SMP_BD_HANDLE * di,
         /* OUT */ UCHAR         * oob
     );

/**
 * \brief To retrieve the OOB Temporary Key of a peer device.
 *
 * \par Description:
 *      This API retrieves OOB Temporary Key of a peer device.
 *      Used by SMP, during authentication procedure.
 *
 * \param [in] di  Peer device for which the OOB Temporary Key is requested.
 * \param [out] temp_key  Pointer to hold 16 Octet, OOB Temporary Key.
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT smp_get_oob_tk_pl
           (
               /* IN */  SMP_BD_HANDLE * di,
               /* OUT */ UCHAR         * temp_key
           );

#ifdef SMP_LESC
void smp_get_local_oob_public_key_pl( /* OUT */ UCHAR * l_pub_key);
void smp_get_lesc_oob_l_cnf_val_pl( /* OUT */ UCHAR * l_cnf_val);
void smp_get_lesc_oob_l_rand_pl( /* OUT */ UCHAR * l_rand);
API_RESULT smp_get_lesc_oob_pl
           (
               /* IN */  SMP_BD_HANDLE * di,
               /* OUT */ UCHAR         * r_conf_val,
               /* OUT */ UCHAR         * r_rand
           );
#endif /* SMP_LESC */

#endif /* SMP_HAVE_OOB_SUPPORT */

#ifdef HOST_RESOLVE_PVT_ADDR
/**
 *  \brief To resolve the connected device if Resolvable Private Address
 *
 *  \par Description:
 *  Default implementation makes use of SMP resolution procedure if only
 *  storage of remote keys in SMP is enabled by defining SMP_NO_STORED_KEYS
 *  flag.
 *  This can be updated to make use of application resolution procedure
 *  in case when SMP_NO_STORED_KEYS is undefined or in case when application
 *  desires to use its own remote database.
 *
 *  Post the resolution procedure from the application with all devices
 *  in database in a chained manner, the application shall
 *  notify the procedure result using the BT_smp_notify_address_resolution API.
 *
 *  \param [in] bdaddr   Peer device address to be resolved that has connected
 *  \param [in] handler  Private Address resolution complete handler
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT smp_resolve_connection_address_pl
           (
               BT_DEVICE_ADDR * bdaddr,
               SMP_PVT_ADDR_RESOLV_COMPLETE_CB handler
           );
#endif /* HOST_RESOLVE_PVT_ADDR */

#ifdef SMP_PL_ENABLE_KEY_GENERATION
/**
 * \brief Rand Complete handler
 *
 * \par Description:
 *      This handles the RAND complete HCI event.
 *
 * \param [in] data_param  Random Number
 * \param [in] datalen  Random Number size
 *
 * \return void
 */
void smp_rand_complete_pl (UCHAR * data_param, UINT16 datalen);

/**
 * \brief Encryption Complete handler
 *
 * \par Description:
 *      This handles the Encryption complete HCI event.
 *
 * \param [in] data_param  Encrypted data
 * \param [in] datalen  Encrypted data size
 *
 * \return void
 */
void smp_encryption_complete_pl (UCHAR * data_param, UINT16 datalen);
#endif /* SMP_PL_ENABLE_KEY_GENERATION */

#if (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT)
void smp_lesc_oob_handle_cmd_complete(UCHAR * data, UINT16 datalen);
void smp_lesc_oob_cnf_val_gen_complete(void);
#endif /* (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT) */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
void smp_lesc_lk_ltk_complete (void);
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/** \} */

/**
 * \defgroup smp_pl_api_defs Platform API Definitions
 * \{
 *  This section describes application API for SMP Platform.
 */
/* --------------------------------------------- API Declarations */

/**
 * \brief Add device to platform database
 *
 * \par Description:
 *      This function adds the given device to the platform database
 *
 * \param [in] bd_addr  Peer bluetooth device address
 *
 * \return API_SUCCESS or Error Code stating the failure reason
 */
API_RESULT BT_smp_add_device_pl (/* IN */ BT_DEVICE_ADDR * bd_addr);

/**
 * \brief Delete device from platform database
 *
 * \par Description:
 *      This function removes the given device from the platform database
 *
 * \param [in] bd_addr  Peer bluetooth device address
 *
 * \return API_SUCCESS or Error Code stating the failure reason
 */
API_RESULT BT_smp_remove_device_pl (/* IN */ BT_DEVICE_ADDR * bd_addr);

/**
 * \brief To get local platform capabilities
 *
 * \par Description:
 *      This API retrieves local platform IO capabilities,
 *      key distribution setting and maximum encryption key size.
 *
 * \param [out] iocap  Pointer to IO Capability supported
 * \param [out] keys  Pointer to key distribution setting flag
 * \param [out] ekey_size  Pointer to max. encryption size
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_local_capability_pl
           (
               /* OUT */ UCHAR  * iocap,
               /* OUT */ UCHAR  * keys,
               /* OUT */ UCHAR  * ekey_size
           );

#ifdef SMP_SAVE_REMOTE_IOCAP
/**
 * \brief To get remote platform capabilities
 *
 * \par Description:
 *      This API retrieves remote platform IO capabilities,
 *      OOB Support and maximum encryption key size.
 *
 * \param [in] bd_addr  Pointer to Peer device address
 * \param [out] iocaps  Pointer to SMP_IOCAPS structure
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_remote_iocaps_pl
           (
               /* IN */  BT_DEVICE_ADDR * bd_addr,
               /* OUT */ SMP_IOCAPS     * iocaps
           );
#endif /* SMP_SAVE_REMOTE_IOCAP */

/**
 * \brief To get local keys for exchange
 *
 * \par Description:
 *      This API retrieves local keys to be exchanged with peer.
 *
 * \param [out] key_info  Pointer to \ref SMP_KEY_DIST
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_key_exchange_info_pl(SMP_KEY_DIST ** key_info);

/**
 * \brief To mark a device untrusted
 *
 * \par Description:
 *      This API marks the device specified by the handle as untrusted.
 *
 * \param [in] bd_handle  SMP BD Handle of peer device
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_mark_device_untrusted_pl (/* IN */ SMP_BD_HANDLE * bd_handle);

/**
 * \brief To get the long term key
 *
 * \par Description:
 *      This API retrieves the long term key for a given combination of RAND and
 *      EDIV.
 *
 * \param [in] rnd  8 octet Random Number
 * \param [in] ediv  Encrypted Diversifier
 * \param [out] ltk  Pointer to long term key to be retrieved
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_long_term_key_pl
           (
               /* IN */  UCHAR  * rnd,
               /* IN */  UINT16   ediv,
               /* OUT */ UCHAR  * ltk
           );

/**
 * \brief To set local IO capability
 *
 * \par Description:
 *      This API sets the local platform IO capability that will be used during
 *      SMP authentication procedure.
 *
 * \param [in] io_cap  IO Capability as in \ref smp_pl_iocaps
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_set_io_cap_pl (/* IN */ UCHAR io_cap);

/**
 * \brief To set local and remote key distribution support
 *
 * \par Description:
 *      This API sets the local and remote keys to be exchanged, that will be
 *      used during SMP authentication procedure.
 *
 * \param [in] key_dist  Key distribution setting flag
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_set_key_distribution_flag_pl (/* IN */ UCHAR key_dist);

/**
 * \brief To set platform Max Encryption key size
 *
 * \par Description:
 *     This API sets the max encryption key size of local platform.
 *
 * \param [in] key_size  Max Encryption size
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_set_max_encryption_key_size_pl (/* IN */ UCHAR key_size);

/**
 * \brief To get platform Max Encryption key size
 *
 * \par Description:
 *     This API retrieves the max encryption key size of local platform.
 *
 * \return Platform Max Encryption key size
 */
UCHAR BT_smp_get_max_encryption_key_size_pl (void);

#ifdef SMP_HAVE_OOB_SUPPORT
/**
 * \brief To set the platform OOB data
 *
 * \par Description:
 *      This API sets local platform OOB presence flag and data.
 *
 * \param [in] bd_addr  Peer device for which the OOB configuration is to be used
 * \param [in] oob_flag  OOB data presence flag
 * \param [in] oob Pointer to OOB authentication information of type \ref SMP_OOB_DATA
 * \param [in] blob  OOB blob data
 * \param [in,out] blob_len  OOB blob data length.
 *                         Actual blob length stored will be returned.
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 *
 * \note Before calling this API to set the OOB data, application must
 *       add the peer device to the platform database by calling
 *       BT_smp_add_device_pl().
 */
API_RESULT BT_smp_set_oob_data_pl
           (
               /* IN */    BT_DEVICE_ADDR * bd_addr,
               /* IN */    UCHAR            oob_flag,
               /* IN */    SMP_OOB_DATA   * oob,
               /* IN */    UCHAR          * blob,
               /* INOUT */ UCHAR          * blob_len
           );

/**
 * \brief To get the platform OOB data
 *
 * \par Description:
 *      This API retrieves peer device OOB information.
 *
 * \param [in] bd_addr  Peer device for which the OOB configuration is to be retrieved
 * \param [out] oob_flag  OOB data presence flag
 * \param [out] oob  OOB data pointer of type \ref SMP_OOB_DATA
 * \param [out] blob  OOB blob data
 * \param [in,out] blob_len  OOB blob data length.
 *                         Actual blob length copied will be returned.
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_oob_data_pl
           (
               /* IN */    BT_DEVICE_ADDR * bd_addr,
               /* OUT */   UCHAR          * oob_flag,
               /* OUT */   SMP_OOB_DATA   * oob,
               /* OUT */   UCHAR          * blob,
               /* INOUT */ UCHAR          * blob_len
           );
#endif /* SMP_HAVE_OOB_SUPPORT */

#if (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT)
API_RESULT BT_smp_generate_lesc_oob_local_data_pl
           (
               SMP_LESC_OOB_GEN_COMPLETE_CB handler
           );
#endif /* (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT) */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
API_RESULT BT_smp_get_lk_ltk_pl
           (
               UCHAR                           mode,
               UCHAR                           * key,
               SMP_LESC_LK_LTK_GEN_COMPLETE_CB handler,
               UCHAR                           ct2
           );
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/**
 * \brief To set the platform MITM policy
 *
 * \par Description:
 *      This API updates the platform MITM policy to be either
 *      Strict(SMP_PL_SEC_MITM_STRICT) or Easy(SMP_PL_SEC_MITM_BEST_EFFORT).
 *      When Strict, JustWorks association will be disallowed if MITM is
 *      requested by either of the devices in pairing.
 *
 * \param [in] policy  MITM Policy \ref SMP_PL_SEC_MITM_BEST_EFFORT or
 *                    \ref SMP_PL_SEC_MITM_STRICT
 *
 * \return None.
 */
void BT_smp_set_mitm_policy_pl (UCHAR policy);

/**
 * \brief To get the platform MITM policy
 *
 * \par Description:
 *      This API retrieves the platform MITM policy to be either
 *      Strict(SMP_PL_SEC_MITM_STRICT) or Easy(SMP_PL_SEC_MITM_BEST_EFFORT).
 *      When Strict, JustWorks association will be disallowed if MITM is
 *      requested by either of the devices in pairing.
 *
 * \return policy  MITM Policy \ref SMP_PL_SEC_MITM_BEST_EFFORT or
 *                    \ref SMP_PL_SEC_MITM_STRICT
 */
UCHAR BT_smp_get_mitm_policy_pl(void);

#ifdef SMP_LESC
/**
 * \brief To set the platform LE secure connections policy
 *
 * \par Description:
 *      This API updates the platform LESC policy to be either
 *      Strict(SMP_PL_LESC_STRICT) or Easy(SMP_PL_LESC_BEST_EFFORT).
 *      When Strict, Just LE LEGACY pairing will be disallowed if LESC is
 *      requested by either of the devices in pairing.
 *
 * \param [in] policy  LESC Policy \ref SMP_PL_LESC_BEST_EFFORT or
 *                    \ref SMP_PL_LESC_STRICT
 *
 * \return None.
 */
void BT_smp_set_lesc_policy_pl (UCHAR policy);

/**
 * \brief To get the platform LE secure connections policy
 *
 * \par Description:
 *      This API retrieves the current platform LESC policy and returns the same
 *      to the application. The policy value can be either
 *      Strict(SMP_PL_LESC_STRICT) or Easy(SMP_PL_LESC_BEST_EFFORT).
 *      When Strict, Just LE LEGACY pairing will be disallowed if LESC is
 *      requested by either of the devices in pairing.
 *
 * \return LESC Policy \ref SMP_PL_LESC_BEST_EFFORT or
 *                    \ref SMP_PL_LESC_STRICT
 */
UCHAR BT_smp_get_lesc_policy_pl (void);

/**
 * \brief To set the key press notifications of LE secure connections
 *
 * \par Description:
 *      This API either enables or disables the LE Secure connections
 *      key press notification support which is used during PASSKEY pairing
 *      association model.
 *
 * \param policy (IN) flag \ref SMP_PL_LESC_KEY_PRESS_NTF_RESET or
 *                    \ref SMP_PL_LESC_KEY_PRESS_NTF_SET
 *
 * \return None.
 */
void BT_smp_set_key_press_ntf_pl (UCHAR flag);
#endif /* SMP_LESC */

/**
 * \brief To set local Identity Address
 *
 * \par Description:
 *      This API sets the local Identity Address that will be used during
 *      SMP authentication Key Exchange procedure.
 *
 * \param [in] bd_addr Pointer to Identity Address to be set  \ref SMP_BD_ADDR
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_set_local_identity_addr(/* IN */ SMP_BD_ADDR * bd_addr);

#ifdef SMP_PL_ENABLE_KEY_GENERATION
/**
 * \brief To generate the SMP platform keys
 *
 * \par Description:
 *      This utility platform API helps to generate different SMP keys upon
 *      giving a diversifier value, that may be specific to a peer device.
 *
 * \param [in] div  Diversifier specific to a device
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_keys_pl (UINT16 div);

/**
 * \brief To generate the SMP Encrypted Diversifier
 *
 * \par Description:
 *      This utility platform API helps to generate Encrypted Diversifier.
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_ediv_pl (void);

/**
 * \brief To generate the SMP Encryption key
 *
 * \par Description:
 *      This utility platform API helps to generate Encryption key
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_ltk_pl (void);

/**
 * \brief To generate the SMP signature key
 *
 * \par Description:
 *      This utility platform API helps to generate signature key
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_csrk_pl (void);


/**
 * \brief To generate the SMP Identity key
 *
 * \par Description:
 *      This utility platform API helps to generate identity resolving key
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_irk_pl (void);

/**
 * \brief To generate the SMP DHK
 *
 * \par Description:
 *      This utility platform API helps to generate DHK
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_dhk_pl (void);
#endif /* SMP_PL_ENABLE_KEY_GENERATION */

#ifdef SMP_HAVE_TBX_PL_ENC
API_RESULT BT_smp_encrypt_pl
           (
               UCHAR  * key,
               UCHAR  * text,
               UINT16 len,
               UCHAR  * cmd_counter
           );
void smp_assign_encrypt_cmd_counter_pl (UCHAR * cmd_cntr);
void smp_reverse_octet_stream_pl(UCHAR * buffer, UINT16 buffer_len);
#endif /* SMP_HAVE_TBX_PL_ENC */

#ifdef SMP_HAVE_TBX_PL_ECDH
API_RESULT smp_gen_public_key_pl(void);
API_RESULT smp_gen_dh_key_pl (UCHAR * r_pub_key);
#endif /* SMP_HAVE_TBX_PL_ECDH */

#ifdef SMP_HAVE_TBX_PL_ECDH
#define BT_smp_read_p256_pubkey_pl()            smp_gen_public_key_pl()
#define BT_smp_gen_dhkey_pl(pub_key)            smp_gen_dh_key_pl(pub_key)
#else /* SMP_HAVE_TBX_PL_ECDH */
#define BT_smp_read_p256_pubkey_pl()            BT_hci_le_read_local_p256_public_key()
#define BT_smp_gen_dhkey_pl(pub_key)            BT_hci_le_generate_dhkey(pub_key)
#endif /* SMP_HAVE_TBX_PL_ECDH */

#ifdef SMP_HAVE_TBX_PL_ENC
API_RESULT BT_smp_128B_encrypt_pl (UCHAR * key, UCHAR * data, UCHAR * encout);
#else /* SMP_HAVE_TBX_PL_ENC */
#define BT_smp_128B_encrypt_pl(key, data, count)       BT_hci_le_encrypt((key), (data), (count))
#endif /* SMP_HAVE_TBX_PL_ENC */

/** To get bit field representation of valid device queue entries */
#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
API_RESULT smp_get_valid_dq_entries_pl
           (
               /* OUT */ UINT32 *    dq_entries
           );
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

/* Platform Passkey Generated related declarations */
API_RESULT BT_smp_set_passkey_revamp_mode_pl (/* IN */ UCHAR mode);
API_RESULT smp_revamp_passkey_pl
           (
               /* INOUT */ UCHAR  * p_key_ptr,
               /* INOUT */ UINT16 * p_key_len
           );

/** \} */

/** \} */

#endif /* _H_SMP_PL_ */

