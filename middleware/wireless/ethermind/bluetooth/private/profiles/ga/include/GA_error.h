
/**
 *  \file GA_error.h
 *
*  \brief This file contains all the error codes defined for GA profiles.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_ERROR_
#define _H_GA_ERROR_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_ga_core
 * \{
 */

/**
 * \addtogroup ga_cap_module
 * \{
 */

/**
 * \addtogroup ga_bap_module
 * \{
 */

/**
 * \addtogroup ga_bap_defines
 * \{
 */

/**
 * \defgroup ga_cap_error_code_def Error Code Definitions
 * \{
 * \brief This sections describes all the Error ID for GA Layer and
 * Error Codes exposed and used by the EtherMind GA CAP Layer.
 */

/**
 * \defgroup ga_cap_error_group Error Grouping
 * \{
 * \brief This sections describes various Error Code Groupings exposed by the
 * EtherMind GA Layer.
 */

/**
 * \name Common Errors
 * \{
 * \brief Set of common error codes across all profiles
 */

#define GA_MUTEX_INIT_FAILED                       0x0001
#define GA_COND_INIT_FAILED                        0x0002
#define GA_MUTEX_LOCK_FAILED                       0x0003
#define GA_MUTEX_UNLOCK_FAILED                     0x0004
#define GA_MEMORY_ALLOCATION_FAILED                0x0005

/** \} */

/**
 * \name Profile Error ID
 * \{
 * \brief This section describes GA Profile Module Error ID.
 */

 /** Module Error Code ID - Common Audio Profile */
#define GA_CAP_ERR_ID                                   0x0100U
 /** Module Error Code ID - Basic Audio Profile */
#define GA_BAP_ERR_ID                                   0x0200U
/** Module Error Code ID - Volume Control Profile */
#define GA_VCP_ERR_ID                                   0x0300U
/** Module Error Code ID - Microphone Control Profile */
#define GA_MICP_ERR_ID                                  0x0400U
/** Module Error Code ID - Coordinated Set Identification Profile */
#define GA_CSIP_ERR_ID                                  0x0500U
/** Module Error Code ID - Call Control Profile */
#define GA_CCP_ERR_ID                                   0x0600U
/** Module Error Code ID - Media Control Profile */
#define GA_MCP_ERR_ID                                   0x0700U
/** Module Error Code ID - Object Transfer Profile */
#define GA_OTP_ERR_ID                                   0x0800U
/** Module Error Code ID - Hearing Aid Profile */
#define GA_HAP_ERR_ID                                   0x0900U
/** Module Error Code ID - Telephony & Media Audio Profile */
#define GA_TMAP_ERR_ID                                  0x0A00U
/** Module Error Code ID - Broadcast Audio Scann Service & Profile */
#define GA_BASS_ERR_ID                                  0x0B00U
/** Module Error Code ID - GA BRR PL */
#define GA_BRR_ERR_ID                                   0x0C00U
/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

/**
* \defgroup ga_brr_error_code Error Codes
* \{
* \brief This section lists the Error Codes that are provided as part of
* return of functionalities of BRR.
*/

#define BRR_MUTEX_INIT_FAILED                    \
        (GA_BRR_ERR_ID | GA_MUTEX_INIT_FAILED)
#define BRR_COND_INIT_FAILED                     \
        (GA_BRR_ERR_ID | GA_COND_INIT_FAILED)
#define BRR_MUTEX_LOCK_FAILED                    \
        (GA_BRR_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define BRR_MUTEX_UNLOCK_FAILED                  \
        (GA_BRR_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define BRR_MEMORY_ALLOCATION_FAILED             \
        (GA_BRR_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)
/** \} */

/**
* \addtogroup ga_bap_module
* \{
*/

/**
* \addtogroup ga_bap_defines
* \{
*/

/**
* \defgroup ga_cap_error_code Error Codes
* \{
* \brief This section lists the Error Codes that are provided as part of
* return of functionalities of CAP \ref ga_cap_api_defs.
*/

#define CAP_MUTEX_INIT_FAILED                    \
        (GA_CAP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define CAP_COND_INIT_FAILED                     \
        (GA_CAP_ERR_ID | GA_COND_INIT_FAILED)
#define CAP_MUTEX_LOCK_FAILED                    \
        (GA_CAP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define CAP_MUTEX_UNLOCK_FAILED                  \
        (GA_CAP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define CAP_MEMORY_ALLOCATION_FAILED             \
        (GA_CAP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/** Volume Control Profile Error Code ID - Invalid State to perform the operation */
#define GA_CAP_INVALID_STATE                            (GA_CAP_ERR_ID | 0x0011U)
/** Volume Control Profile Error Code ID - Invalid Parameters provided */
#define GA_CAP_INVALID_PARAMS                           (GA_CAP_ERR_ID | 0x0012U)
/** Volume Control Profile Error Code ID - New Context allocation failed */
#define GA_CAP_CNTX_ALLOC_FAILED                        (GA_CAP_ERR_ID | 0x0013U)
/** Volume Control Profile Error Code ID - Another operation already in progress */
#define GA_CAP_IN_BUSY_STATE                            (GA_CAP_ERR_ID | 0x0014U)
/** Volume Control Profile Error Code ID - Invalid Handle to perform the operation */
#define GA_CAP_INVALID_HANDLE                           (GA_CAP_ERR_ID | 0x0015U)

/** \} */
/** \} */
/** \} */

/**
* \addtogroup ga_bap_module
* \{
*/

/**
* \addtogroup ga_bap_defines
* \{
*/

/**
* \defgroup ga_bap_error_code Error Codes
* \{
* \brief This section lists the Error Codes that are provided as part of
* return of functionalities of BAP \ref ga_bap_api_defs.
*/

#define BAP_MUTEX_INIT_FAILED                    \
        (GA_BAP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define BAP_COND_INIT_FAILED                     \
        (GA_BAP_ERR_ID | GA_COND_INIT_FAILED)
#define BAP_MUTEX_LOCK_FAILED                    \
        (GA_BAP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define BAP_MUTEX_UNLOCK_FAILED                  \
        (GA_BAP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define BAP_MEMORY_ALLOCATION_FAILED             \
        (GA_BAP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/** Volume Control Profile Error Code ID - Invalid State to perform the operation */
#define GA_BAP_INVALID_STATE                            (GA_BAP_ERR_ID | 0x0011U)
/** Volume Control Profile Error Code ID - Invalid Parameters provided */
#define GA_BAP_INVALID_PARAMS                           (GA_BAP_ERR_ID | 0x0012U)
/** Volume Control Profile Error Code ID - New Context allocation failed */
#define GA_BAP_CNTX_ALLOC_FAILED                        (GA_BAP_ERR_ID | 0x0013U)
/** Volume Control Profile Error Code ID - Another operation already in progress */
#define GA_BAP_IN_BUSY_STATE                            (GA_BAP_ERR_ID | 0x0014U)
/** Volume Control Profile Error Code ID - Invalid Handle to perform the operation */
#define GA_BAP_INVALID_HANDLE                           (GA_BAP_ERR_ID | 0x0015U)

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_vcp_module
 * \{
 */

/**
 * \addtogroup ga_vcp_defines
 * \{
 */

/**
 * \defgroup ga_vcp_error_code Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of VCP \ref ga_vcp_api_defs.
 */

#define VCP_MUTEX_INIT_FAILED                    \
        (GA_VCP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define VCP_COND_INIT_FAILED                     \
        (GA_VCP_ERR_ID | GA_COND_INIT_FAILED)
#define VCP_MUTEX_LOCK_FAILED                    \
        (GA_VCP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define VCP_MUTEX_UNLOCK_FAILED                  \
        (GA_VCP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define VCP_MEMORY_ALLOCATION_FAILED             \
        (GA_VCP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/** Volume Control Profile Error Code ID - Invalid State to perform the operation */
#define GA_VCP_INVALID_STATE                            (GA_VCP_ERR_ID | 0x0011U)
/** Volume Control Profile Error Code ID - Invalid Parameters provided */
#define GA_VCP_INVALID_PARAMS                           (GA_VCP_ERR_ID | 0x0012U)
/** Volume Control Profile Error Code ID - New Context allocation failed */
#define GA_VCP_CNTX_ALLOC_FAILED                        (GA_VCP_ERR_ID | 0x0013U)
/** Volume Control Profile Error Code ID - Another operation already in progress */
#define GA_VCP_IN_BUSY_STATE                            (GA_VCP_ERR_ID | 0x0014U)
/** Volume Control Profile Error Code ID - Invalid Handle to perform the operation */
#define GA_VCP_INVALID_HANDLE                           (GA_VCP_ERR_ID | 0x0015U)
/**
 * Volume Control Profile Error Code ID - Operation Not Supported
 * Property not set for this Char by the remote.
 */
#define GA_VCP_OPERATION_NOT_SUPPORTED_BY_REMOTE_DEVICE (GA_VCP_ERR_ID | 0x0016U)
/**
 * Volume Control Profile Error Code ID - Service not registered by Local
 * Server.
 */
#define GA_VCP_SERVICE_ID_NOT_FOUND                     (GA_VCP_ERR_ID | 0x0017U)
/**
 * Volume Control Profile Error Code ID - Unknown Service Type other than
 * VCS, VOCS, AICS.
 */
#define GA_VCP_INVALID_SERVICE_TYPE                     (GA_VCP_ERR_ID | 0x0018U)

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_micp_module
 * \{
 */

/**
 * \addtogroup ga_micp_defines
 * \{
 */

/**
 * \defgroup ga_micp_error_code Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of MICP \ref ga_micp_api_defs.
 */

#define MICP_MUTEX_INIT_FAILED                    \
        (GA_MICP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define MICP_COND_INIT_FAILED                     \
        (GA_MICP_ERR_ID | GA_COND_INIT_FAILED)
#define MICP_MUTEX_LOCK_FAILED                    \
        (GA_MICP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define MICP_MUTEX_UNLOCK_FAILED                  \
        (GA_MICP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define MICP_MEMORY_ALLOCATION_FAILED             \
        (GA_MICP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/**
 * Microphone Control Profile Error Code ID - Invalid State to perform the
 * operation
 */
#define GA_MICP_INVALID_STATE                            (GA_MICP_ERR_ID | 0x0011U)
/** Microphone Control Profile Error Code ID - Invalid Parameters provided */
#define GA_MICP_INVALID_PARAMS                           (GA_MICP_ERR_ID | 0x0012U)
/** Microphone Control Profile Error Code ID - New Context allocation failed */
#define GA_MICP_CNTX_ALLOC_FAILED                        (GA_MICP_ERR_ID | 0x0013U)
/**
 * Microphone Control Profile Error Code ID - Another operation already in
 * progress.
 */
#define GA_MICP_IN_BUSY_STATE                            (GA_MICP_ERR_ID | 0x0014U)
/**
 * Microphone Control Profile Error Code ID - Invalid Handle to perform the
 * operation.
 */
#define GA_MICP_INVALID_HANDLE                           (GA_MICP_ERR_ID | 0x0015U)
/**
 * Microphone Control Profile Error Code ID - Operation Not Supported
 * Property not set for this Char by the remote.
 */
#define GA_MICP_OPERATION_NOT_SUPPORTED_BY_PEER_DEVICE   (GA_MICP_ERR_ID | 0x0016U)
/**
 * Microphone Control Profile Error Code ID - Service not registered by Local
 * Server.
 */
#define GA_MICP_SERVICE_ID_NOT_FOUND                     (GA_MICP_ERR_ID | 0x0017U)
/**
 * Microphone Control Profile Error Code ID - Unknown Service Type other than
 * MICS, AICS.
 */
#define GA_MICP_INVALID_SERVICE_TYPE                     (GA_MICP_ERR_ID | 0x0018U)

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_csip_module
 * \{
 */

/**
 * \addtogroup ga_csip_defines
 * \{
 */

/**
 * \addtogroup ga_csip_error_codes
 * \{
 */

/**
 * \name Implementation Error Codes: CSIP
 * \{
 * \brief This section lists Implementation Error Codes for CSIP.
 */

#define CSIP_MUTEX_INIT_FAILED                    \
        (GA_CSIP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define CSIP_COND_INIT_FAILED                     \
        (GA_CSIP_ERR_ID | GA_COND_INIT_FAILED)
#define CSIP_MUTEX_LOCK_FAILED                    \
        (GA_CSIP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define CSIP_MUTEX_UNLOCK_FAILED                  \
        (GA_CSIP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define CSIP_MEMORY_ALLOCATION_FAILED             \
        (GA_CSIP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/**
 * Coordinated Set Identification Profile Error Code ID - Invalid State to
 * perform the operation.
 */
#define GA_CSIP_INVALID_STATE                            (GA_CSIP_ERR_ID | 0x0011U)
/** Coordinated Set Identification Profile Error Code ID - Invalid Parameters provided */
#define GA_CSIP_INVALID_PARAMS                           (GA_CSIP_ERR_ID | 0x0012U)
/** Coordinated Set Identification Profile Error Code ID - New Context allocation failed */
#define GA_CSIP_CNTX_ALLOC_FAILED                        (GA_CSIP_ERR_ID | 0x0013U)
/**
 * Coordinated Set Identification Profile Error Code ID - Another operation
 * already in progress.
 */
#define GA_CSIP_IN_BUSY_STATE                            (GA_CSIP_ERR_ID | 0x0014U)
/**
 * Coordinated Set Identification Profile Error Code ID - Invalid Handle to
 * perform the operation.
 */
#define GA_CSIP_INVALID_HANDLE                           (GA_CSIP_ERR_ID | 0x0015U)
/**
 * Coordinated Set Identification Profile Error Code ID - Operation Not
 * Supported Property not set for this Char by the remote.
 */
#define GA_CSIP_OPERATION_NOT_SUPPORTED_BY_PEER_DEVICE   (GA_CSIP_ERR_ID | 0x0016U)
/**
 * Coordinated Set Identification Profile Error Code ID - Service not
 * registered by Local Server.
 */
#define GA_CSIP_SERVICE_ID_NOT_FOUND                     (GA_CSIP_ERR_ID | 0x0017U)

/** \} */
/** \} */
/** \} */
/** \} */

/** \} */
/** \} */

/**
 * \addtogroup bt_ga_profiles
 * \{
 */

/**
 * \addtogroup bt_ga_content_control
 * \{
 */

/**
 * \addtogroup bt_ga_ccp
 * \{
 */

/**
 * \addtogroup ga_ccp_defines
 * \{
 */

/**
 * \defgroup ga_ccp_common CCP Common
 * \{
 * \brief Describes common macros for the module.
 */

/**
 * \addtogroup ga_ccp_error_code Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of CCP \ref ga_ccp_ce_module_api,
 * \ref ga_ccp_se_module_api.
 */

/**
 * \name Implementation Error Codes: CCP
 * \{
 * \brief This section lists Implementation Error Codes for CCP.
 */

#define CCP_MUTEX_INIT_FAILED                    \
        (GA_CCP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define CCP_COND_INIT_FAILED                     \
        (GA_CCP_ERR_ID | GA_COND_INIT_FAILED)
#define CCP_MUTEX_LOCK_FAILED                    \
        (GA_CCP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define CCP_MUTEX_UNLOCK_FAILED                  \
        (GA_CCP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define CCP_MEMORY_ALLOCATION_FAILED             \
        (GA_CCP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/**
 * Call Control Profile Error Code ID - Invalid State to perform the
 * operation.
 */
#define GA_CCP_INVALID_STATE                             (GA_CCP_ERR_ID | 0x0011U)
/** Call Control Profile Error Code ID - Invalid Parameters provided */
#define GA_CCP_INVALID_PARAMS                            (GA_CCP_ERR_ID | 0x0012U)
/** Call Control Profile Error Code ID - New Context allocation failed GTBS */
#define GA_CCP_CNTX_ALLOC_FAILED                         (GA_CCP_ERR_ID | 0x0013U)
/** Call Control Profile Error Code ID - New Context allocation failed TBS */
#define GA_CCP_TBS_CNTX_ALLOC_FAILED                     (GA_CCP_ERR_ID | 0x0014U)
/**
 * Call Control Profile Error Code ID - Another operation already in
 * progress.
 */
#define GA_CCP_IN_BUSY_STATE                             (GA_CCP_ERR_ID | 0x0015U)
/**
 * Call Control Profile Error Code ID - Context not found for this
 * request/event.
 */
#define GA_CCP_CNTX_NOT_FOUND                            (GA_CCP_ERR_ID | 0x0016U)
/**
 * Call Control Profile Error Code ID - Invalid Handle to perform the
 * operation.
 */
#define GA_CCP_INVALID_HANDLE                            (GA_CCP_ERR_ID | 0x0017U)

/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

/**
 * \addtogroup bt_ga_mcp Media Control
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Generic Audio (GA) Profile Media Control module to the Application.
 */

/**
 * \defgroup ga_mcp_defines Defines
 * \{
 * \brief Describes defines for the module.
 */

/**
 * \defgroup ga_mcp_common MCP Common
 * \{
 * \brief Describes common macros for the module.
 */

/**
 * \defgroup ga_mcp_and_otp_error_code Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of MCP and OTP
 */

/**
 * \name Implementation Error Codes: MCP
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of MCP \ref ga_mcp_ce_api_defs,
 * \ref ga_mcp_se_api_defs
 */

#define MCP_MUTEX_INIT_FAILED                    \
        (GA_MCP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define MCP_COND_INIT_FAILED                     \
        (GA_MCP_ERR_ID | GA_COND_INIT_FAILED)
#define MCP_MUTEX_LOCK_FAILED                    \
        (GA_MCP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define MCP_MUTEX_UNLOCK_FAILED                  \
        (GA_MCP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define MCP_MEMORY_ALLOCATION_FAILED             \
        (GA_MCP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/**
 * Media Control Profile Error Code ID - Invalid State to perform the
 * operation.
 */
#define GA_MCP_INVALID_STATE                             (GA_MCP_ERR_ID | 0x0011U)
/** Media Control Profile Error Code ID - Invalid Parameters provided */
#define GA_MCP_INVALID_PARAMS                            (GA_MCP_ERR_ID | 0x0012U)
/** Media Control Profile Error Code ID - New Context allocation failed */
#define GA_MCP_CNTX_ALLOC_FAILED                         (GA_MCP_ERR_ID | 0x0013U)
/**
 * Media Control Profile Error Code ID - Another operation already in
 * progress.
 */
#define GA_MCP_IN_BUSY_STATE                             (GA_MCP_ERR_ID | 0x0014U)
/**
 * Media Control Profile Error Code ID - Context not found for this
 * request/event.
 */
#define GA_MCP_CNTX_NOT_FOUND                            (GA_MCP_ERR_ID | 0x0015U)
/**
 * Media Control Profile Error Code ID - Invalid Handle to perform the
 * operation.
 */
#define GA_MCP_INVALID_HANDLE                            (GA_MCP_ERR_ID | 0x0016U)

/** \} */

/**
 * \name Implementation Error Codes: OTP
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of OTP \ref ga_mcp_ce_api_defs,
 * \ref ga_mcp_se_api_defs
 */

/**
 * Object Transfer Profile Error Code ID - Invalid State to perform the
 * operation.
 */
#define GA_OTP_INVALID_STATE                             (GA_OTP_ERR_ID | 0x0011U)
/** Object Transfer Profile Error Code ID - Invalid Parameters provided */
#define GA_OTP_INVALID_PARAMS                            (GA_OTP_ERR_ID | 0x0012U)
/** Object Transfer Profile Error Code ID - New Context allocation failed */
#define GA_OTP_CNTX_ALLOC_FAILED                         (GA_OTP_ERR_ID | 0x0013U)
/**
 * Object Transfer Profile Error Code ID - Another operation already in
 * progress.
 */
#define GA_OTP_IN_BUSY_STATE                             (GA_OTP_ERR_ID | 0x0014U)
/**
 * Object Transfer Profile Error Code ID - Context not found for this
 * request/event.
 */
#define GA_OTP_CNTX_NOT_FOUND                            (GA_OTP_ERR_ID | 0x0015U)
/**
 * Object Transfer Profile Error Code ID - OTP Session already exists for
 * this handle.
 */
#define GA_OTP_SESSION_EXISTS                            (GA_OTP_ERR_ID | 0x0016U)

/** \} */

/** \} */
/** \} */
/** \} */
/** \} */

/** \} */

/**
 * \addtogroup bt_ga_appl_profiles
 * \{
 */

/**
 * \addtogroup hap_module
 * \{
 */

/**
 * \addtogroup hap_defines
 * \{
 */

/**
 * \defgroup hap_error_codes Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of HAP.
 */

/**
 * \name Implementation Error Codes: HAP
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of HAP \ref ga_ha_api_defs
 */

#define HAP_MUTEX_INIT_FAILED                    \
        (GA_HAP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define HAP_COND_INIT_FAILED                     \
        (GA_HAP_ERR_ID | GA_COND_INIT_FAILED)
#define HAP_MUTEX_LOCK_FAILED                    \
        (GA_HAP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define HAP_MUTEX_UNLOCK_FAILED                  \
        (GA_HAP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define HAP_MEMORY_ALLOCATION_FAILED             \
        (GA_HAP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/**
 * Hearing Access Profile Error Code ID - Invalid State to perform the
 * operation.
 */
#define GA_HAP_INVALID_STATE                             (GA_HAP_ERR_ID | 0x0011U)
/**
 * Hearing Access Profile Error Code ID - Invalid Parameters provided. */
#define GA_HAP_INVALID_PARAMS                            (GA_HAP_ERR_ID | 0x0012U)
/**
 * Hearing Access Profile Error Code ID - New Context allocation failed. */
#define GA_HAP_CNTX_ALLOC_FAILED                         (GA_HAP_ERR_ID | 0x0013U)
/**
 * Hearing Access Profile Error Code ID - Another operation already in
 * progress.
 */
#define GA_HAP_IN_BUSY_STATE                             (GA_HAP_ERR_ID | 0x0014U)
/**
 * Hearing Access Profile Error Code ID - Context not found for this
 * request/event.
 */
#define GA_HAP_CNTX_NOT_FOUND                            (GA_HAP_ERR_ID | 0x0015U)
/**
 * Hearing Access Profile Error Code ID - Invalid Handle to perform
 * the operation.
 */
#define GA_HAP_INVALID_HANDLE                            (GA_HAP_ERR_ID | 0x0016U)

/** \} */
/** \} */
/** \} */
/** \} */

/**
 * \addtogroup tmap_module
 * \{
 */

/**
 * \addtogroup tmap_defines
 * \{
 */

/**
 * \defgroup tmap_error_code Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of TMAP
 */

/**
 * \name Implementation Error Codes: TMAP
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of TMAP \ref tmap_api_defs
 */

#define TMAP_MUTEX_INIT_FAILED                    \
        (GA_TMAP_ERR_ID | GA_MUTEX_INIT_FAILED)
#define TMAP_COND_INIT_FAILED                     \
        (GA_TMAP_ERR_ID | GA_COND_INIT_FAILED)
#define TMAP_MUTEX_LOCK_FAILED                    \
        (GA_TMAP_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define TMAP_MUTEX_UNLOCK_FAILED                  \
        (GA_TMAP_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define TMAP_MEMORY_ALLOCATION_FAILED             \
        (GA_TMAP_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/**
 * Telephony and Media Audio Profile Error Code ID - Invalid State to perform
 * the operation.
 */
#define GA_TMAP_INVALID_STATE                            (GA_TMAP_ERR_ID | 0x0011U)
/**
 * Telephony and Media Audio Profile Error Code ID - Invalid Parameters
 * provided.
 */
#define GA_TMAP_INVALID_PARAMS                           (GA_TMAP_ERR_ID | 0x0012U)
/**
 * Telephony and Media Audio Profile Error Code ID - New Context allocation
 * failed.
 */
#define GA_TMAP_CNTX_ALLOC_FAILED                        (GA_TMAP_ERR_ID | 0x0013U)
/**
 * Telephony and Media Audio Profile Error Code ID - Another operation already
 * in progress.
 */
#define GA_TMAP_IN_BUSY_STATE                            (GA_TMAP_ERR_ID | 0x0014U)
/**
 * Telephony and Media Audio Profile Error Code ID - Context not found for this
 * request/event.
 */
#define GA_TMAP_CNTX_NOT_FOUND                           (GA_TMAP_ERR_ID | 0x0015U)
/**
 * Telephony and Media Audio Profile Error Code ID - Invalid Handle to perform
 * the operation.
 */
#define GA_TMAP_INVALID_HANDLE                           (GA_TMAP_ERR_ID | 0x0016U)

/** \} */

/** \} */
/** \} */
/** \} */
/** \} */
/** \} */

/**
 * \addtogroup bt_ga_core
 * \{
 */

/**
 * \addtogroup ga_cap_module
 * \{
 */

/**
 * \addtogroup ga_bass_module
 * \{
 */

/**
 * \addtogroup ga_bass_defines
 * \{
 */

/**
 * \addtogroup ga_bass_common
 * \{
 */

/**
 * \addtogroup ga_bass_error_codes Error Codes
 * \{
 */

/**
 * \name Implementation Error Codes: BASS
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of BASS \ref ga_bass_ce_api_defs,
 * \ref ga_bass_se_api_defs
 */

#define BASS_MUTEX_INIT_FAILED                    \
        (GA_BASS_ERR_ID | GA_MUTEX_INIT_FAILED)
#define BASS_COND_INIT_FAILED                     \
        (GA_BASS_ERR_ID | GA_COND_INIT_FAILED)
#define BASS_MUTEX_LOCK_FAILED                    \
        (GA_BASS_ERR_ID | GA_MUTEX_LOCK_FAILED)
#define BASS_MUTEX_UNLOCK_FAILED                  \
        (GA_BASS_ERR_ID | GA_MUTEX_UNLOCK_FAILED)
#define BASS_MEMORY_ALLOCATION_FAILED             \
        (GA_BASS_ERR_ID | GA_MEMORY_ALLOCATION_FAILED)

/**
 * Broadcast Audio Scan Service Error Code ID - Invalid State to perform
 * the operation.
 */
#define GA_BASS_INVALID_STATE                            (GA_BASS_ERR_ID | 0x0011U)

/**
 * Broadcast Audio Scan Service Error Code ID - Invalid Parameters
 * provided.
 */
#define GA_BASS_INVALID_PARAMS                           (GA_BASS_ERR_ID | 0x0012U)

/**
 * Broadcast Audio Scan Service Error Code ID - New Context allocation
 * failed.
 */
#define GA_BASS_CNTX_ALLOC_FAILED                        (GA_BASS_ERR_ID | 0x0013U)

/**
 * Broadcast Audio Scan Service Error Code ID - Another operation already
 * in progress.
 */
#define GA_BASS_IN_BUSY_STATE                            (GA_BASS_ERR_ID | 0x0014U)

/**
 * Broadcast Audio Scan Service Error Code ID - Context not found for this
 * request/event.
 */
#define GA_BASS_CNTX_NOT_FOUND                           (GA_BASS_ERR_ID | 0x0015U)

/**
 * Broadcast Audio Scan Service Error Code ID - Invalid Handle to perform
 * the operation.
 */
#define GA_BASS_INVALID_HANDLE                           (GA_BASS_ERR_ID | 0x0016U)

/** \} */
/** \} */
/** \} */

/** \} */
/** \} */
/** \} */
/** \} */

#endif /* _H_GA_ERROR_ */
