
/**
*  \file appl_le.h
*
*  Main Application Header File
*/

/*
*  Copyright (C) 2013. Mindtree Ltd.
*  All rights reserved.
*/

#ifndef _H_APPL_LE_
#define _H_APPL_LE_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_device_queue.h"
#include "BT_hci_api.h"
#include "l2cap.h"

#include "BT_smp_api.h"
#include "BT_att_api.h"
#include "BT_gatt_db_api.h"

#include "fsm_defines.h"
#include "gatt_defines.h"

#ifdef BT_DUAL_MODE
#include "db_gen.h"
#endif /* BT_DUAL_MODE */

/* ----------------------------------------- Global Definitions */
/** Application Error ID */
#define APPL_ERR_ID                          0xEE00

/*
 * Device Advertising data and length
 *
 *  Flags:
 *      0x02: LE General Discoverable Mode
 *      0x08: Simultaneous LE and BR/EDR to Same Device
 *            Capable (Controller)
 *      0x10: Simultaneous LE and BR/EDR to Same Device
 *            Capable (Host)
 *
 *  Service UUID List:
 *      Battery Service (0x180F)
 *      DeviceInformation Service (0x180A)
 *
 *  Shortened Device Name: EtherMind-MT-LE
 */
#define APPL_LE_GAP_ADV_DATALEN             26
#define APPL_LE_GAP_ADV_DATA                0x02, 0x01, 0x1a, \
                                            0x05, 0x02, 0x0f, 0x18, 0x0a, 0x18, \
                                            0x09, 0x10, 0x45, 0x74, 0x68, 0x65, 0x72, 0x4d, 0x69, 0x6e, \
                                            0x64, 0x2d, 0x4D, 0x54, 0x2d, 0x4c, 0x45

/* Initial Battery level 100% */
#define APPL_BATTERY_PERCENT                0x64

/* Battery notification interval in seconds*/
#define APPL_BATT_NTF_TIMEOUT               30

/* Num Application events */
#define APPL_MAX_NUM_EVENTS                 17

/* ----------------------------------------- Structures/Data Types */
/* Abstrats Application Instance Identifier */
typedef UCHAR APPL_HANDLE;

/* Abstrats Application Instance Identifier */
typedef UINT16 APPL_STATE_T;

/* This is the table for active on-going connection only */
typedef struct appl_peer_instance_t
{
    UINT16          cxn_handle;

    ATT_HANDLE      att_handle;

    APPL_STATE_T    state;

} APPL_PEER_INSTANCE;

typedef struct appl_event_param_t
{
    /* Event Data  */
    UCHAR                   * data_param;

    /* Event Data Length */
    UINT16                 data_len;

    /* Application instance Handle */
    APPL_HANDLE            handle;

    /* Indicated whether the event is peer or application initiated */
    UCHAR                  direction;

}APPL_EVENT_PARAM;

/* --------------------------------------------- External Global Variables */
extern API_RESULT appl_smp_assert_security
                  (
                      SMP_BD_HANDLE * handle,
                      UCHAR           security,
                      UCHAR           ekey_size
                  );

extern APPL_PEER_INSTANCE appl_peer_device_list[BT_MAX_REMOTE_DEVICES];
extern UCHAR              appl_active_connections;
extern BT_DEVICE_ADDR     peer_bd_addr;

extern DECL_CONST STATE_EVENT_TABLE appl_state_event_table[];
extern DECL_CONST EVENT_HANDLER_TABLE appl_event_handler_table[];

/* ----------------------------------------- Macros */
#define SET_ALL_APPL_STATE(state)           appl_set_all_state(state)
#define SET_APPL_STATE(inst,state)          appl_set_state(inst,state)
#define GET_APPL_STATE(inst)                appl_peer_device_list[(inst)].state
#define APPL_CONFIG_PEER_DEVICE_ADDR()      (&peer_bd_addr.addr[0])
#define APPL_CONFIG_PEER_DEVICE_ADDR_TYPE() (peer_bd_addr.type)

/* Encoding of application PDU parameters */
#define appl_pack_1_byte_param(dest, src) \
    *((dest)+0) = (UCHAR)(*((UCHAR *)(src)));

#define appl_pack_2_byte_param(dest, src) \
    *((dest)+0) = (UCHAR)(*((UINT16 *)(src))); \
    *((dest)+1) = (UCHAR)(*((UINT16 *)(src)) >> 8);

#define appl_pack_3_byte_param(dest, src) \
    *((dest)+0) = (UCHAR)(*((UINT32 *)(src))); \
    *((dest)+1) = (UCHAR)(*((UINT32 *)(src)) >> 8); \
    *((dest)+2) = (UCHAR)(*((UINT32 *)(src)) >> 16);

#define appl_pack_4_byte_param(dest, src) \
    *((dest)+0) = (UCHAR)(*((UINT32 *)(src))); \
    *((dest)+1) = (UCHAR)(*((UINT32 *)(src)) >> 8); \
    *((dest)+2) = (UCHAR)(*((UINT32 *)(src)) >> 16); \
    *((dest)+3) = (UCHAR)(*((UINT32 *)(src)) >> 24);

/* Decoding of application PDU parameters */
#define appl_unpack_1_byte_param(dest, src) \
    (dest) = (src)[0];

#define appl_unpack_2_byte_param(dest, src) \
    (dest) = (src)[1]; \
    (dest) = ((dest) << 8); \
    (dest) |= (src)[0];

#define appl_unpack_4_byte_param(dest, src) \
    (dest) = (src)[3]; \
    (dest) = ((dest) << 8); \
    (dest) |= (src)[2]; \
    (dest) = ((dest) << 8); \
    (dest) |= (src)[1]; \
    (dest) = ((dest) << 8); \
    (dest) |= (src)[0];

#define APPL_DEVICE_INIT(x)\
    ATT_INIT_HANDLE(appl_peer_device_list[(x)].att_handle); \
    appl_peer_device_list[(x)].cxn_handle = HCI_INVALID_CONNECTION_HANDLE

#define APPL_GET_CONNECTION_HANDLE(x)\
    appl_peer_device_list[(x)].cxn_handle

/**
*  Please note that device queue handle actually is independent of ATT
*  instance handle. But since device queue is a part of ATT Handle, its
*  being accessed through the ATT handle for avoiding redudant copies.
*/
#define APPL_GET_DEVICE_HANDLE(x)\
    appl_peer_device_list[(x)].att_handle.device_id

#define APPL_GET_ATT_INSTANCE(x)\
    appl_peer_device_list[(x)].att_handle

#define APPL_SET_ATT_INSTANCE(hndl,att_inst)\
    appl_peer_device_list[(hndl)].att_handle = (att_inst)

#define APPL_HANDLE_INIT(x)\
    (x) = BT_MAX_REMOTE_DEVICES

#define APPL_EVENT_PARAM_INIT(param)\
    APPL_HANDLE_INIT((param)->handle); \
    (param)->data_param = NULL; \
    (param)->data_len = 0; \

#ifdef ATT_ON_BR_EDR_SUPPORT
#define APPL_UPDATE_AND_ACTIVATE_SDP_RECORD(service, inst) \
    appl_update_and_activate_sdp_record \
    (\
    GATT_SER_ ## service ## _INST, \
    DB_RECORD_ ## service, \
    inst \
    )
#else
#define APPL_UPDATE_AND_ACTIVATE_SDP_RECORD(service, inst)
#endif /* ATT_ON_BR_EDR_SUPPORT */

#ifndef SMP
#define appl_smp_assert_security(a, b, c) API_SUCCESS
#endif /* SMP */

#ifndef BT_DUAL_MODE
#define appl_update_and_activate_sdp_record(x, y, z)
#endif /* BT_DUAL_MODE */

/* ----------------------------------------- Function Declarations */
int appl_init(void);
void appl_init_complete(void);

API_RESULT appl_fsm_post_event(EVENT_T fsm_event, void * param);

API_RESULT appl_battery_level_handler(GATT_DB_HANDLE * handle, UINT16 config);
void appl_update_battery_lvl(DEVICE_HANDLE  * dq_handle);
void appl_batt_timer_expiry_handler(void *data, UINT16 datalen);
void appl_set_target_address_in_adv_data(UCHAR offset);

void appl_set_all_state(APPL_STATE_T state);
void appl_set_state(APPL_HANDLE  handle, APPL_STATE_T state);

API_RESULT appl_add_device
           (
               /* IN */ ATT_HANDLE     * att_handle,
               /* IN */ APPL_HANDLE    * handle
           );

API_RESULT appl_get_handle_from_cxn_handle
           (
               /* IN */  UINT16         cxn_handle,
               /* OUT */ APPL_HANDLE    * handle
           );

API_RESULT appl_set_cxn_handle
           (
               /* IN */ UINT16            cxn_handle,
               /* IN */ DEVICE_HANDLE     dq_handle,
               /* OUT */ APPL_HANDLE      * handle
           );

API_RESULT appl_get_handle_from_device_handle
           (
               /* IN */  DEVICE_HANDLE     dq_handle,
               /* OUT */ APPL_HANDLE       * handle
           );

API_RESULT appl_get_cxn_handle_from_device_handle
           (
                /* IN */  DEVICE_HANDLE    dq_handle,
                /* OUT */ UINT16      * handle
           );

API_RESULT appl_delete_device
           (
               /* IN */ UINT16 cxn_handle
           );

API_RESULT appl_set_att_instance
           (
               /* IN */  ATT_HANDLE     * att_instace,
               /* OUT */ APPL_HANDLE    * handle
           );

#ifdef BT_DUAL_MODE
API_RESULT appl_update_and_activate_sdp_record
           (
               /* IN */ UCHAR service_type,
               /* IN */ ATT_ATTR_HANDLE start_handle,
               /* IN */ ATT_ATTR_HANDLE end_handle
           );
#endif /* BT_DUAL_MODE */

API_RESULT appl_access_state_handler(void * param, STATE_T * state);

void appl_display_all_connected_devices(void);

#endif /* _H_APPL_LE_ */

