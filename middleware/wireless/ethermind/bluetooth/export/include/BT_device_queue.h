
/**
 *  \file BT_device_queue.h
 *
 *  Module Header File for Device Queue module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_DEVICE_QUEUE_
#define _H_BT_DEVICE_QUEUE_

/* ----------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup device_queue Device Queue
 * \{
 * Describes Device Queue defined by the module.
 */
/**
 * \defgroup device_queue_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup device_queue_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
#define DEVICE_HANDLE_INIT_VAL     0xFFU

#define DEVICE_HANDLE_INIT(hndl)\
        (hndl) = DEVICE_HANDLE_INIT_VAL

#define DQ_LINK_NONE                    0x00U
#define DQ_BR_LINK                      0x01U
#define DQ_LE_LINK                      0x02U
#define DQ_LINK_ANY                     0xFFU

/** \} */
/**
 * \defgroup device_queue_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/* ----------------------------------------------- Type Definition */
typedef UCHAR DEVICE_HANDLE;

typedef UCHAR DEVICE_LINK_TYPE;
/** \} */
/** \} */

/* ----------------------------------------------- Function Declarations */
/**
 * \defgroup device_queue_api API definitions
 * \{
 * Describes API definitions defined by the module.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Init routine */
/**
 *  \brief To do power on initialization of EtherMind Device Queue module
 *
 *  \par Description:
 *       This function is the EtherMind-Init handler for the Device Queue module
 *       and performs power-on initialization.
 *
 *  \note This function must be called only once.
 */
void em_device_queue_init ( void );

#ifdef BT_HAVE_SHUTDOWN
/** De-init routine */
void em_device_queue_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */

/** Activating routine */
/**
 *  \brief To perform Bluetooth specific initializations for EtherMind Device Queue module
 *
 *  \par Description:
 *       This function is the Bluetooth-ON handler for Device Queue module, and it
 *       performs bluetooth specific initializations for the Device Queue module.
 */
void device_queue_bt_init ( void );

/** Shutdown routine */
/**
 *  \brief To perform Bluetooth specific shutdown for EtherMind Device Queue module
 *
 *  \par Description:
 *       This function is the Bluetooth-OFF handler for Device Queue module, and it
 *       performs bluetooth specific shutdown for the Device Queue module.
 */
void device_queue_bt_shutdown ( void );

/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/** Enqueue routine */
API_RESULT device_queue_alloc
           (
               /* OUT */ DEVICE_HANDLE       * hndl, /* Device Handle */
               /* IN */  BT_DEVICE_ADDR      * addr,  /* Remote Device to be added*/
               /* IN */  DEVICE_LINK_TYPE    link_type
           );


/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/* Fetch Link Type */
API_RESULT device_queue_get_link_type
           (
               /* OUT */ DEVICE_LINK_TYPE    * link_type,
               /* IN */  DEVICE_HANDLE       * hndl
           );

/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/* Delete routine */
API_RESULT device_queue_free
           (
               /* IN */ DEVICE_HANDLE    * hndl /* Device to be removed */
           );

#define device_queue_search_br_edr_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a),DQ_BR_LINK)

#define device_queue_search_le_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a),DQ_LE_LINK)

#define device_queue_search_any_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a),DQ_LINK_ANY)

/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/* Search routine */
API_RESULT device_queue_search_remote_addr
           (
               /* OUT */ DEVICE_HANDLE       * hndl,/* Device Reference */
               /* IN */  BT_DEVICE_ADDR      * addr, /* Remote Device to be searched*/
               /* IN */  DEVICE_LINK_TYPE    link_type
           );

/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/* Fetch Remote Device Address */
API_RESULT device_queue_get_remote_addr
           (
               /* IN */  DEVICE_HANDLE     * hndl, /* Device Reference */
               /* OUT */ BT_DEVICE_ADDR    * addr  /* Remote Device Address */
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/* Fetch Device Address Only */
API_RESULT device_queue_get_remote_addr_only
           (
               /* IN */  DEVICE_HANDLE    * hndl, /* Device Reference */
               /* OUT */ UCHAR            * addr  /* Remote Device Address */
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/* Set remote addr routine */
API_RESULT device_queue_set_remote_addr
           (
               /* IN */ DEVICE_HANDLE     * hndl, /* Device Reference */
               /* IN */ BT_DEVICE_ADDR    * addr  /* Remote Device to be set */
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/* Fetch Local Device Address */
API_RESULT device_queue_get_local_addr
           (
               /* IN */  DEVICE_HANDLE     * hndl, /* Device Reference */
               /* OUT */ BT_DEVICE_ADDR    * addr  /* Device's address */
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
/* Set local addr routine */
API_RESULT device_queue_set_local_addr
           (
               /* IN */ DEVICE_HANDLE     * hndl, /* Device Reference */
               /* IN */ BT_DEVICE_ADDR    * addr  /* Local Device to be set*/
           );
/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *  \param [in]
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT device_queue_set_curr_addr_type
           (
               /* IN */ UCHAR    * addr,/* Local Device to be set*/
               /* IN */ UCHAR    type
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_DEVICE_QUEUE_ */

