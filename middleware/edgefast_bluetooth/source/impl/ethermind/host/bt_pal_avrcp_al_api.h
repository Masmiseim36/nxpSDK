
/**
 *  \file bt_pal_avrcp_al_api.h
 *
 *  This Header File contains the high level API declarations
 *  built on top of AVRCP APIs.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_AVRCP_AL_API_
#define _H_BT_AVRCP_AL_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_avrcp_api.h"


/* --------------------------------------------- Global Definitions */
#define     AVRCP_AL_FIXED_CMD_LEN_UNIT_INFO           (AVRCP_COMMAND_HDR_LEN + 5)
#define     AVRCP_AL_FIXED_CMD_LEN_PASSTHROUGH         (AVRCP_COMMAND_HDR_LEN + 2)
#define     AVRCP_AL_FIXED_CMD_LEN_VU_PASSTHROUGH      (AVRCP_COMMAND_HDR_LEN + 7)

#define     AVRCP_AL_MAX_AVC_PACKET_LEN                512


/* --------------------------------------------- Structres/Data Types */

/* AVRCP Control Command information structure */
typedef struct _AVRCP_AL_CT_CMD_INFO
{
    /* OP_CODE of Command */
    UCHAR   cmd_opcode;

    /* Transaction Label */
    UCHAR   cmd_tl;

    /* Commamd data */
    void    *cmd_info;

    /* Command data length */
    UINT16  cmd_info_len;

}AVRCP_AL_CT_CMD_INFO;

/* AVRCP Control Command response information structure */
typedef struct _AVRCP_AL_CT_RSP_INFO
{
    /* OP_CODE of commnad in the response */
    UCHAR   cmd_opcode;

    /**
     * Response Type:
     * 0x08 -> Not Implemented
     * 0x09 -> Accepted
     * 0x0A -> Rejected
     * 0x0F -> Interim response
     * 0x0C -> Stable
     * 0x0D -> Changed
     */
    UCHAR   rsp_type;

    /* Response data */
    void    *rsp_info;

    /* Response data length */
    UINT16  rsp_info_len;

}AVRCP_AL_CT_RSP_INFO;


/**
 * AVRCP Vendor Dependent Message(Command/Response)
 * information structure 
 */
typedef struct _AVRCP_AL_VD_CMD_INFO
{
    /* PDU id */
    UCHAR   pdu_id;

    /* Transaction Label */
    UCHAR   tl;

    /**
     * For Command message cmd_type correspond to command type:
     *  0x00 -> Control
     *  0x01 -> Status
     *  0x03 -> Notify
     *
     * For Response Message cmd_type correspond to response type:
     *  0x08 -> Not Implemented
     *  0x09 -> Accepted
     *  0x0A -> Rejected
     *  0x0F -> Interim response
     *  0x0C -> Stable
     *  0x0D -> Changed
     */
    UCHAR   cmd_type;

    /**
     * Command packet type:
     *  0 -> Single packet
     *  1 -> Start packet
     *  2 -> Continue packet
     *  3 -> End packet
     */
    UCHAR   packet_type;

    /* Command/Response parameters length */
    UCHAR   param_len;

    /* Command/Response data */
    void  * vd_cmd_data;

    /* Command/Response data length */
    UINT16  vd_cmd_datalen;

}AVRCP_AL_VD_CMD_INFO;

/* GetFolder Items Command information */
typedef struct _AVRCP_AL_GET_FOLDER_ITEMS_PARAM
{
    /**
     * Media Content scope:
     *  0x00 -> Media Player List
     *  0x01 -> Media Player Virtual Filesystem
     *  0x02 -> Search
     *  0x03 -> Now Playing
     */
    UCHAR   scope;

    /**
     *  First item index in the list
     */
    UINT32  start_item;

    /**
     *  Last item index in the list
     */
    UINT32  end_item;

    /* Attribute Count of the Media Item */
    UCHAR   attr_count;

    /* Attribute ID's of the Media Item */
    UINT32  attr_list[8];

}AVRCP_AL_GET_FOLDER_ITEMS_PARAM;

/* GetItemsAttributes Command information */
typedef struct _AVRCP_AL_GET_ITEM_ATTRIBUTE_PARAMS
{
    /**
     * Media Content scope:
     *  0x00 -> Media Player List
     *  0x01 -> Media Player Virtual Filesystem
     *  0x02 -> Search
     *  0x03 -> Now Playing
     */
    UCHAR   scope;

    /* Item UID most significant part */
    UINT32  uid_msb;

    /* Item UID least significant part */
    UINT32  uid_lsb;

    /* Item UID counter */
    UINT16  uid_counter;

    /* Attribute Count of Media Item */
    UCHAR   attr_count;

    /* Attribute ID's of Media Item */
    UINT32  attr_list[8];

}AVRCP_AL_GET_ITEM_ATTRIBUTE_PARAMS;

/* Browsing Command information */
typedef struct _AVRCP_AL_BROW_CMD_INFO
{
    /* PDU id */
    UCHAR   pdu_id;

    /* Transaction label */
    UCHAR   tl;

    /* Browsing command data */
    void  * brow_cmd_info;

    /* Browsing command data length */
    UINT16  brow_cmd_info_len;

}AVRCP_AL_BROW_CMD_INFO;

/* Browsing Command information */
typedef struct _AVRCP_AL_BROW_RSP_INFO
{
    /* PDU id */
    UCHAR   pdu_id;

    /* Transaction label */
    UCHAR   tl;

    /* Browsing command response data */
    void  * param_info;

    /* Browsing command response data length */
    UINT16  param_info_len;

}AVRCP_AL_BROW_RSP_INFO;

/* AVRCP Abstractin Layer Event Notification callback */
typedef API_RESULT (* AVRCP_AL_EVENT_NTF_CB)
                   (
                        AVRCP_HANDLE * avrcp_handle,
                        UCHAR          event_type,
                        UINT16         event_result,
                        void         * event_data,
                        UINT16         event_datalen
                   );

/* --------------------------------------------- Macros */
/**
 * Mapping of AVRCP Connect/Disconnect APIs
 */
#define BT_avrcp_al_connect_req               BT_avrcp_connect_req
#define BT_avrcp_al_disconnect_req            BT_avrcp_disconnect_req

#define BT_avrcp_al_channel_connect_req       BT_avrcp_channel_connect_req
#define BT_avrcp_al_channel_disconnect_req    BT_avrcp_channel_disconnect_req
/**
 *  \fn BT_avrcp_al_send_unit_info
 *
 *  \par brief:
 *  This function is used to retrieve TG AV/C unit information.
 *
 *  \param [in] handle:
 *         AVRCP Handle.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
#define BT_avrcp_al_send_unit_info(handle)                             \
        BT_avrcp_al_send_info_cmd ((handle), AVRCP_OPCODE_UNIT_INFO)

/**
 *  \fn BT_avrcp_al_send_subunit_info
 *
 *  \par brief:
 *  This function is used to retrieve TG AV/C sub-unit information.
 *
 *  \param [in] handle:
 *         AVRCP Handle.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
#define BT_avrcp_al_send_subunit_info(handle)                          \
        BT_avrcp_al_send_info_cmd ((handle), AVRCP_OPCODE_SUBUNIT_INFO)

/**
 *  \fn BT_avrcp_al_get_companyid_supported
 *
 *  \par brief:
 *  This function is used to retrieve TG company id information.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
#define BT_avrcp_al_get_companyid_supported(handle)                          \
        BT_avrcp_al_get_capabilities ((handle), AVRCP_CAP_ID_COMPANY_ID)

/**
 *  \fn BT_avrcp_al_get_event_supported
 *
 *  \par brief:
 *  This function is used to retrieve TG supported events for notification.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
#define BT_avrcp_al_get_event_supported(handle)                    \
        BT_avrcp_al_get_capabilities ((handle), AVRCP_CAP_ID_EVENTS_SUPPORTED)

/**
 *  \fn BT_avrcp_al_send_request_continuing_rsp
 *
 *  \par brief:
 *  This function is used to retrieve continue response.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] pdu_id:
 *         pdu id for continue response.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
#define BT_avrcp_al_send_request_continuing_rsp(handle, pdu_id)        \
        BT_avrcp_al_send_continuation_pdu((handle), AVRCP_METADATA_PDU_ID_REQUEST_CONTINUING_RESPONSE, (pdu_id))

/**
 *  \fn BT_avrcp_al_send_abort_continuing_rsp
 *
 *  \par brief:
 *  This function is used to abort continue response.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] pdu_id:
 *         pdu id for abort response.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
#define BT_avrcp_al_send_abort_continuing_rsp(handle, pdu_id)        \
        BT_avrcp_al_send_continuation_pdu((handle), AVRCP_METADATA_PDU_ID_ABORT_CONTINUING_RESPONSE, (pdu_id))

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

/**
 *  \fn BT_avrcp_al_init
 *
 *  \brief:
 *  This function is used for AVRCP Abstraction layer Initialization.
 *
 *  \param None
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_init(void);

/**
 *  \fn BT_avrcp_al_shutdown
 *
 *  \brief:
 *  This function is used for AVRCP Abstraction layer shutdown.
 *
 *  \param None
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_shutdown(void);

/**
 *  \fn BT_avrcp_al_start
 *
 *  \brief:
 *  This function is used for AVRCP Abstraction layer start.
 *
 *  \param [out] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] al_nty_cb:
 *         Application callback registing with Abstraction layer.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_start
           (
               /* OUT */ AVRCP_HANDLE *handle,
               /* IN  */ AVRCP_AL_EVENT_NTF_CB al_nty_cb
           );

/**
 *  \fn BT_avrcp_al_stop
 *
 *  \brief:
 *  This function is used for AVRCP Abstraction layer stop.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] al_nty_cb:
 *         Application callback registing with Abstraction layer.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_stop
           (
               /* IN */ AVRCP_HANDLE *handle
           );

/**
 *  \fn BT_avrcp_al_send_info_cmd
 *
 *  \par brief:
 *  This function is used to send AV/C unit commands.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] info_opcode:
 *         opcode for UNIT or SUBUNIT
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_send_info_cmd
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          info_opcode
           );

/**
 *  \fn BT_avrcp_al_send_info_rsp
 *
 *  \par brief:
 *  This function is used to send AV/C unit command response.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] avctp_tl:
 *         Transaction label is tag to identify command/response.
 *
 *  \param [in] info_opcode:
 *         opcode for UNIT or SUBUNIT
 *
 *  \param [in] info_type:
 *         Unit Type
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */

API_RESULT BT_avrcp_al_send_info_rsp
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          avctp_tl,
               /* IN */ UCHAR          info_opcode,
               /* IN */ UCHAR          info_type
           );

/**
 *  \fn BT_avrcp_al_send_passthrough_cmd_rsp
 *
 *  \par brief:
 *  This function is used to send PASSTHROUGH command/response.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] avctp_tl:
 *         Transaction label tag to identify response to a command.
 *
 *  \param [in] cmd_rsp_type:
 *         It represents command or response type.
 *
 *  \param [in] operation_id:
 *         passthrough operation id
 *
 *  \param [in] state:
 *         passthrough command state -> pressed or released
 *
 *  \param [in] vu_ip_id:
 *         Vendor Unique operation ID
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_send_passthrough_cmd_rsp
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          avctp_tl,
               /* IN */ UCHAR          cmd_rsp_type,
               /* IN */ UCHAR          operation_id,
               /* IN */ UCHAR          state,
               /* IN */ UINT16         vu_op_id
           );

/**
 *  \fn BT_avrcp_al_send_metadata_pdu
 *
 *  \par brief:
 *  This is a common function to send AVRCP Vendor dependent command/response PDUs.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] pdu_info:
 *         PDU information
 *
 *  \param [in] dir:
 *         Direction -> Command or Response
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_send_metadata_pdu
           (
               /* IN */ AVRCP_HANDLE         * avrcp_handle,
               /* IN */ AVRCP_AL_VD_CMD_INFO * pdu_info,
               /* IN */ UCHAR                  dir
           );

/**
 *  \fn BT_avrcp_al_set_absolute_volume
 *
 *  \par brief:
 *  Function to absolute volume.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] volume:
 *         Absolute Volume value (range : 0 to 255)
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_set_absolute_volume
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          volume
           );
/**
 *  \fn BT_avrcp_al_get_capabilities
 *
 *  \par brief:
 *  Function to get capabilities supported by remote device.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] cap_id:
 *         Capability ID
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_get_capabilities
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          cap_id
           );

/**
 *  \fn BT_avrcp_al_send_set_addressed_player_cmd
 *
 *  \par brief:
 *  This function is used to inform the TG about media player that CT wishes to control.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] player_id:
 *         Unique Media Player ID
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_send_set_addressed_player_cmd
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UINT16         player_id
           );

/**
 *  \fn BT_avrcp_al_send_set_addressed_player_rsp
 *
 *  \par brief:
 *  This function is used to send response for set address palyer command.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] tl:
 *         AVCTP transaction label.
 *
 *  \param [in] rsp_type:
 *         Response type -> Accepted or Rejected.
 *
 *  \param [in] status:
 *         Response succes or error code
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_send_set_addressed_player_rsp
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          tl,
               /* IN */ UCHAR          rsp_type,
               /* IN */ UCHAR          status
           );

/**
 *  \fn BT_avrcp_al_get_play_status
 *
 *  \par brief:
 *  This function is used to get status of currently playing media item.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_get_play_status
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle
           );

/**
 *  \fn BT_avrcp_al_register_notification
 *
 *  \par brief:
 *  This function is used to register with TG to receive notifications
 *  asynchronously based on specific event occuring.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] event_id:
 *         Event ID for which CT requires notification.
 *
 *  \param [in] interval:
 *         Time interval at which the change in playback position will be notified.
 *         Application for only EVENT_ID = EVETN_PLAYBACK_POS_CHANGED
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_register_notification
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          event_id,
               /* IN */ UINT32         interval
           );

/**
 *  \fn BT_avrcp_al_send_continuation_pdu
 *
 *  \par brief:
 *  This function is used to sent continue packet for given pdu_id.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] pdu_id:
 *         Continue or Abort PDU id.
 *
 *  \param [in] param:
 *         PDU id to continue or abort.

 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_send_continuation_pdu
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          pdu_id,
               /* IN */ UCHAR          param
           );

/***************************** AVRCP BROWSING CHANNEL APIs ******/
/**
 *  \fn BT_avrcp_al_send_browsing_pdu
 *
 *  \par brief:
 *  This function is used to send AVRCP browsing command/response PDUs.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle
 *
 *  \param [in] pdu_id:
 *         Browsing PDU id
 *
 *  \param [in] data:
 *         Browsing command/resonse PDU data
 *
 *  \param [in] data_len:
 *         Browsing command/resonse PDU data length.
 *
 *  \param [in] dir:
 *         Direction -> Command or Response
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_send_browsing_pdu
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          pdu_id,
               /* IN */ UCHAR        * data,
               /* IN */ UINT16         datalen,
               /* IN */ UCHAR          dir
           );
/**
 *  \fn BT_avrcp_al_send_browsing_cmd_rsp
 *
 *  \par brief:
 *  This function is used to send response for a browsing command.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle.
 *
 *  \param [in] brow_rsp_info:
 *         Browsing commnad response data.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_send_browsing_cmd_rsp
           (
               /* IN */ AVRCP_HANDLE         * avrcp_handle,
               /* IN */ AVRCP_AL_BROW_RSP_INFO  * brow_rsp_info
           );

/**
 *  \fn BT_avrcp_al_set_browsed_player
 *
 *  \par brief:
 *  This function is used to set the browsing media player.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle.
 *
 *  \param [in] player_id:
 *         Browsing player id.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_set_browsed_player
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UINT16         player_id
           );

/**
 *  \fn BT_avrcp_al_get_folder_items
 *
 *  \par brief:
 *  This function is used to retrieve a listing of contents of a folder (depending on folder scope).
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle.
 *
 *  \param [in] folder_items_params:
 *         Folder Item information.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_get_folder_items
           (
               /* IN */ AVRCP_HANDLE                     * avrcp_handle,
               /* IN */ AVRCP_AL_GET_FOLDER_ITEMS_PARAM  * folder_items_params
           );

/**
 *  \fn BT_avrcp_al_change_path
 *
 *  \par brief:
 *  This function is used to navigate virtual filesystem.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle.
 *
 *  \param [in] uid_counter:
 *         UID counter.
 *
 *  \param [in] path_dir:
 *         path direction -> Folder up or Folder down.
 *
 *  \param [in] folder_uid:
 *         Unique ID of the folder to navigate.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_change_path
           (
               /* IN */ AVRCP_HANDLE        * avrcp_handle,
               /* IN */ UINT16                uid_counter,
               /* IN */ UCHAR                 path_dir,
               /* IN */ UCHAR               * folder_uid
           );

/**
 *  \fn BT_avrcp_al_get_items_attribute
 *
 *  \par brief:
 *  This function is used to retrieve meta data attributes of a particular media item.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle.
 *
 *  \param [in] item_attr_params:
 *         Item attrribute information.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_get_items_attribute
           (
               /* IN */ AVRCP_HANDLE                       * avrcp_handle,
               /* IN */ AVRCP_AL_GET_ITEM_ATTRIBUTE_PARAMS * item_attr_params
           );


/**
 *  \fn BT_avrcp_al_search
 *
 *  \par brief:
 *  This function is used to do basic search functionality.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle.
 *
 *  \param [in] char_set:
 *         AVRCP Handle.
 *
 *  \param [in] char_set:
 *         Search charater set id.
 *
 *  \param [in] search_str:
 *         Search string.
 *
 *  \param [in] search_str_len:
 *         Search string length.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_search
           (
               /* IN */ AVRCP_HANDLE        * avrcp_handle,
               /* IN */ UINT16                char_set,
               /* IN */ UCHAR               * search_str,
               /* IN */ UINT16                search_str_len
           );

#ifdef AVRCP_GET_NUM_OF_ITEMS
/**
 *  \fn BT_avrcp_al_get_total_number_of_items
 *
 *  \par brief:
 *  This function is used to retrieve the number of items in a folder prior to calling GetFolderItems.
 *
 *  \param [in] avrcp_handle:
 *         AVRCP Handle.
 *
 *  \param [in] scope:
 *         Scope of the browsable item.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
API_RESULT BT_avrcp_al_get_total_number_of_items
           (
               /* IN */ AVRCP_HANDLE        * avrcp_handle,
               /* IN */ UCHAR                 scope
            );

#endif /* AVRCP_GET_NUM_OF_ITEMS */

#endif /* _H_BT_AVRCP_AL_API_ */

