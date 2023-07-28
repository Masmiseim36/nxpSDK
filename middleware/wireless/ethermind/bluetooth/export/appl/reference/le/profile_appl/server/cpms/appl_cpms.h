
/**
 *  \file appl_cpms.h
 *
 *  Sample Profile Application Header.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_CPMS_
#define _H_APPL_CPMS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"

/**
 * Needed if CPMS supports Broadcast Feature.
 */
#define APPL_GAP_BROADCASTER_SUPPORT

/* --------------------------------------------- Global Definitions */
typedef struct cpm_cntrl_point_param
{
    APPL_HANDLE    appl_handle;

    ATT_VALUE      rsp_param;

}APPL_CPM_CNTRL_POINT_RSP_PARAM;

#define APPL_NUM_OF_CPM_MSRMT_MASK_FIELD        9U
#define APPL_CPM_MSRMNT_LENGTH                  30U
#define APPL_CPM_MSRMT_INTERVAL                 5U
#define APPL_CPM_VECTOR_INTERVAL                5U
#define APPL_CPM_VECTOR_DATA_LENGTH             11U

/* CPM Control Point Opcodes */
#define APPL_CPM_SET_CUMMULATIVE_VALUE_OPCODE 0x01U
#define APPL_CPM_UPDATE_SENSOR_LOCATION_OPCODE 0x02U
#define APPL_CPM_REQUEST_SUPPORTED_SENSOR_LOCATION_OPCODE 0x03U
#define APPL_CPM_SET_CRANK_LENGTH_OPCODE     0x04U
#define APPL_CPM_REQ_CRANK_LENGTH_OPCODE     0x05U
#define APPL_CPM_SET_CHAIN_LENGTH_OPCODE     0x06U
#define APPL_CPM_REQ_CHAIN_LENGTH_OPCODE     0x07U
#define APPL_CPM_SET_CHAIN_WEIGHT_OPCODE     0x08U
#define APPL_CPM_REQ_CHAIN_WEIGHT_OPCODE     0x09U
#define APPL_CPM_SET_SPAN_LENGTH_OPCODE      0x0AU
#define APPL_CPM_REQ_SPAN_LENGTH_OPCODE      0x0BU
#define APPL_CPM_STRT_OFFSET_COMPNSTN_OPCODE 0x0CU
#define APPL_MASK_CPM_CHAR_CONTENT_OPCODE    0x0DU
#define APPL_CPM_REQ_SAMPLING_RATE_OPCODE    0x0EU
#define APPL_CPM_REQ_FACTRY_CALIB_DATE_OPCODE 0x0FU
#define APPL_CPM_STRT_ENHCED_OFFSET_COMPNSTN_OPCODE 0X10U
#define APPL_CPM_RESPONSE_CODE               0x20U


#define APPL_CPM_CUMMULATIVE_VALUE_LENGTH    4U

/* Supported Sensor Location Count */
#define APPL_CPM_SUPPORTED_SENSOR_LOC_COUNT  3U

/* CPM Control Point Response Values */
#define APPL_CPM_SUCCESS                     0x01U
#define APPL_CPM_OPCODE_NOT_SUPPORTED        0x02U
#define APPL_CPM_INVALID_PARAMETER           0x03U
#define APPL_CPM_OPERATION_FAILED            0x04U

/* CPM Control point Response parameter */
#define APPL_CPM_INCORRECT_CALBN_POSITION   0x01U

/* Valid Crank Length Param */
#define APPL_MAX_CRANK_LENGTH                30U
#define APPL_MIN_CRANK_LENGTH                10U
#define APPL_VALID_CRANK_LENGTH_PARAM_LEN    2U

/* Valid Chain Length Param */
#define APPL_MAX_CHAIN_LENGTH                50U
#define APPL_MIN_CHAIN_LENGTH                10U
#define APPL_VALID_CHAIN_LENGTH_PARAM_LEN    2U

/* Valid Chain Weight Param */
#define APPL_MAX_CHAIN_WEIGHT                50U
#define APPL_MIN_CHAIN_WEIGHT                10U
#define APPL_VALID_CHAIN_WEIGHT_PARAM_LEN    2U

/* Valid Span Length Param */
#define APPL_MAX_SPAN_LENGTH                20U
#define APPL_MIN_SPAN_LENGTH                10U
#define APPL_VALID_SPAN_LENGTH_PARAM_LEN    2U

/* Vector Control Values */
#define APPL_START_CPM_VECTOR_TRANSFER      0x01U
#define APPL_STOP_CPM_VECTOR_TRANSFER       0x00U

#define APPL_CPM_INAPPROPRIATE_CONNECTION_PARAMETERS  0x80U
#define APPL_CPM_BROADCAST_DATA_LENGTH                15U

/* Utility Macros */
#define APPL_CPM_SUPPORTED_OPCODE(op)\
    (((APPL_CPM_STRT_ENHCED_OFFSET_COMPNSTN_OPCODE >= (op)) && \
        (APPL_CPM_SET_CUMMULATIVE_VALUE_OPCODE <= (op)))? BT_TRUE : BT_FALSE)

#define APPL_CPM_VALIDATE_AND_SET_CRANK_LEN_PARAM(src,dest,len,rsp)\
        if (((APPL_MAX_CRANK_LENGTH >= (UINT16)(*(src))) &&\
            (APPL_MIN_CRANK_LENGTH <= (UINT16)(*(src)))) &&\
            (APPL_VALID_CRANK_LENGTH_PARAM_LEN == (len)))\
        {\
            BT_PACK_LE_2_BYTE(&(dest),(src))\
            (rsp) = APPL_CPM_SUCCESS;\
        }\
        else\
        {\
            (rsp) = APPL_CPM_INVALID_PARAMETER;\
        }

#define APPL_CPM_VALIDATE_AND_SET_CHAIN_LEN_PARAM(src,dest,len,rsp)\
        if (((APPL_MAX_CHAIN_LENGTH >= (UINT16)(*(src))) &&\
            (APPL_MIN_CHAIN_LENGTH <= (UINT16)(*(src)))) &&\
            (APPL_VALID_CHAIN_LENGTH_PARAM_LEN == (len)))\
        {\
            BT_PACK_LE_2_BYTE(&(dest),(src))\
            (rsp) = APPL_CPM_SUCCESS;\
        }\
        else\
        {\
            (rsp) = APPL_CPM_INVALID_PARAMETER;\
        }

#define APPL_CPM_VALIDATE_AND_SET_CHAIN_WEIGHT_PARAM(src,dest,len,rsp)\
        if (((APPL_MAX_CHAIN_WEIGHT >= (UINT16)(*(src))) &&\
            (APPL_MIN_CHAIN_WEIGHT <= (UINT16)(*(src)))) &&\
            (APPL_VALID_CHAIN_WEIGHT_PARAM_LEN == (len)))\
        {\
            BT_PACK_LE_2_BYTE(&(dest),(src))\
            (rsp) = APPL_CPM_SUCCESS;\
        }\
        else\
        {\
            (rsp) = APPL_CPM_INVALID_PARAMETER;\
        }

#define APPL_CPM_VALIDATE_AND_SET_SPAN_LEN_PARAM(src,dest,len,rsp)\
        if (((APPL_MAX_SPAN_LENGTH >= (UINT16)(*(src))) &&\
            (APPL_MIN_SPAN_LENGTH <= (UINT16)(*(src)))) &&\
            (APPL_VALID_SPAN_LENGTH_PARAM_LEN == (len)))\
        {\
            BT_PACK_LE_2_BYTE(&(dest),(src))\
            (rsp) = APPL_CPM_SUCCESS;\
        }\
        else\
        {\
            (rsp) = APPL_CPM_INVALID_PARAMETER;\
        }

/* CPMS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are CPMS UUID(0x1818)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_CPMS_GATT_DB_SIGN                     (0x12340000U | GATT_CPM_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
/* --------------------------------------------- Functions */
void appl_cpms_init(void);
void appl_cpms_bt_init(void);
void appl_cpms_bt_deinit(void);
void appl_timer_expiry_handler (void *data, UINT16 datalen);
void appl_cpm_vector_timer_expiry_handler (void *data, UINT16 datalen);
void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
void appl_cpms_connect(DEVICE_HANDLE  * dq_handle);
void appl_send_cpms_measurement (APPL_HANDLE  * handle);
void appl_cpms_server_reinitialize (void);
void appl_cpm_cntrl_point_cnfg_handler (GATT_DB_HANDLE  *handle,UINT16  config);
void appl_cpm_manage_brodcast(UINT16 config);
void appl_cpms_l2cap_callback
     (
         DEVICE_HANDLE * handle,
         UCHAR           event_type,
         UCHAR         * event_data,
         UINT16          event_datalen
     );
void appl_manage_cpm_vector_msrmnt (DEVICE_HANDLE * handle, UINT16 result);

API_RESULT appl_cpm_control_point_handler
           (
                GATT_DB_HANDLE  * handle,
                ATT_VALUE       * value
           );
void appl_cpms_menu_handler(void);
API_RESULT gatt_db_cpms_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );
void appl_cpms_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_cpms_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_cpms_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_cpms_gatt_db_register(void);
#else
#define appl_cpms_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_cpms_init()
#define APPL_PROFILE_BT_INIT() appl_cpms_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_cpms_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_cpms_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x) appl_send_cpms_measurement(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_cpms_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_cpms_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_cpms_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_cpms_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_cpms_handle_mtu_update_complete((x),(y))

#ifndef USE_BT_PL_TIMER
#define APPL_USE_IDLE_TIMER
#endif /* USE_BT_PL_TIMER */
#define APPL_IDLE_TIMEOUT           30U

#endif /* _H_APPL_CPMS_ */

