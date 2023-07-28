/**
 *  \file appl_mcp_se.c
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of MCP - Server role interfaces exposed by MCP Server library.
 *  1 instance of GMCS and 2 instances of MCS are added into the DB and based
 *  on the Client performing procedures on the respective GMCS/MCS instance,
 *  action/responses are taken here.
 *  Note: Complete values of the Char needs to be filled and maintained here
 *  in the app.
 */

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/

/* --------------------------------------------- Header File Inclusion */
#include "appl_mcp_se.h"
#include "appl_mcp_utils.h"
#include "otp_se_utils.h"

/* #include "db_gen.h" */

#ifdef MCP_SERVER
/* --------------------------------------------- Global Definitions */

static const UCHAR ga_mcp_se_appl_menu[] =
"\n\
================ GA MCP Server MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Service Management ~ Init ============ \n\
    5. GA MCS Initialize \n\
\n\
================ Service Management ~ Deinit ============ \n\
    6. GA MCS Shutdown \n\
\n\
================ Service Management ~ Optional Service ============ \n\
    7. Add MCS Instance: VLC/MPC Player \n\
\n\
================ Device Management ~ Setting ============ \n\
    9. Set Remote Dev addr \n\
\n\
================ Misc Management ~ Get/Set ============ \n\
   10. Set the MCS Handle for sending notification \n\
   12. Set GMCS to an active Media Player MCS Instance \n\
\n\
================ Service Management ~ Operations(Notify) ============ \n\
   15. Notify Media Player Name \n\
   16. Notify Track Title \n\
   17. Notify Track Duration \n\
   18. Notify Track Pos \n\
   19. Notify Playback Speed \n\
   20. Notify Seek Speed \n\
   21. Notify Curr Track Obj Id \n\
   22. Notify Next Track Obj Id \n\
   23. Notify Parent Group Obj Id \n\
   24. Notify Curr Group Obj Id \n\
   25. Notify Playing Order \n\
   26. Notify Media State \n\
   27. Notify MCP \n\
   28. Notify MCP Opcode Supp \n\
   29. Notify SCP \n\
   30. Notify Search Result Obj ID \n\
   31. Notify Track Changed \n\
\n\
   32. Set Search Result Obj ID as NIL \n\
   33. Set Media Player State \n\
   34. Set Current Track to a Valid Track \n\
   35. Set Segment of the Current Track \n\
   36. Set Current Track \n\
   37. Set Current Group \n\
   38. Update Media Player name \n\
   39. Update Track Title \n\
\n\
================ Service Management ~ SDP Record ============ \n\
   40. Activate SDP Record \n\
\n\
Your Option? ";

DECL_STATIC GA_ENDPOINT           appl_ga_mcs_device;
/* --------------------------------------------- Data types /Structures */

/* Trigger Notifications based on MCP Operations */
#define APPL_MCP_SE_NTF_MEDIA_STATE       0x01U
#define APPL_MCP_SE_NTF_SEEKING_SPEED     0x02U
#define APPL_MCP_SE_NTF_TRACK_POS         0x04U
#define APPL_MCP_SE_NTF_NEXT_TRACK_OBJ_ID 0x08U
#define APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID 0x10U
#define APPL_MCP_SE_NTF_TRACK_DURATION    0x20U
#define APPL_MCP_SE_NTF_TRACK_TITLE       0x40U
#define APPL_MCP_SE_NTF_TRACK_CHANGED     0x80U
#define APPL_MCP_SE_NTF_CURR_GRP_OBJ_ID   0x100U

#define APPL_MCP_MAX_STR_LEN              10U
#define APPL_MCP_MAX_STR_LONG_LEN         30U
#define APPL_MCP_MAX_MCS_SESSIONS         2U /* 2 MCS */

#define APPL_MCP_VLC_PLAYER               1U
#define APPL_MCP_MPC_PLAYER               2U

#define APPL_OBJ_ID_LEN                   6

#define APPL_OBJ_DATA_MAX_LEN             30U

#define APPL_MCP_SE_TRACK_POS_MIN         0
#define APPL_MCP_SE_TRACK_POS_MAX         0x259

/* Segment Positions for each segment with a pos difference of 30 */
#define APPL_MCP_SE_SEG_POS_FIRST                  0x00000000
#define APPL_MCP_SE_SEG_POS_SECOND                 0x0000001E
#define APPL_MCP_SE_SEG_POS_THIRD                  0x0000003C
#define APPL_MCP_SE_SEG_POS_FOURTH                 0x0000005A
#define APPL_MCP_SE_SEG_POS_BEYOND_LAST            0x00000094

#define MCP_SE_MCP_RESULT_SUCCESS                  0x01
#define MCP_SE_MCP_RESULT_OPCODE_NOT_SUPP          0x02
#define MCP_SE_MCP_RESULT_MEDIA_PLAYER_INACTIVE    0x03
#define MCP_SE_MCP_RESULT_CMD_CANNOT_BE_COMP       0x04

/* Data buffer to be used to send the CP Responses to the client */
static UCHAR    cp_rsp[10U];

#ifdef MCP_SUPPORT_OBJECT_TRANSFER

/* Data buffer to be used to send the GATT Ind responses to the client */
static UCHAR    otp_cp_rsp[10U];

/** Set the default Success response for Control Point Procedures */
#define SET_DEFAULT_OLCP_CNTRL_PNT_RSP()       \
        otp_cp_rsp[0U] = MCP_OTS_OLCP_RSP_CODE_OPCODE;   \
        otp_cp_rsp[2U] = MCP_OTS_OLCP_SUCCESS;

/** Set the default Success response for Control Point Procedures */
#define SET_DEFAULT_OACP_CNTRL_PNT_RSP()       \
        otp_cp_rsp[0U] = MCP_OTS_OACP_RSP_CODE_OPCODE;   \
        otp_cp_rsp[2U] = MCP_OTS_OACP_SUCCESS;

/** Set the Control Point Response Event Type */
#define SET_CNTRL_PNT_RSP_EVT_TYPE(opcode)     \
        otp_cp_rsp[1U] = (opcode);

/** Set the Control Point Result Code */
#define SET_CNTRL_PNT_RSP_RESULT_CODE(result)  \
        otp_cp_rsp[2U] = (result);

/** Validate if this Opcode is supported by the App */
#define IS_OACP_READ_OP_SUPP(appl_mcp_curr_obj_prop)         \
        (0x10U == ((appl_mcp_curr_obj_prop) & 0x10U))

/* Timer Handle to send OACP/OLCP Control Point Response to client post expiry */
static BT_timer_handle ots_OACP_send_ctrl_pnt_rsp_timer_handle;
static BT_timer_handle ots_OLCP_send_ctrl_pnt_rsp_timer_handle;

void ots_OACP_send_ctrl_pnt_rsp_on_timer_expiry_handler
     (
         void   * cntrl_point_rsp,
         UINT16   datalen
     );

void ots_OLCP_send_ctrl_pnt_rsp_on_timer_expiry_handler
     (
         void   * cntrl_point_rsp,
         UINT16   datalen
     );

void appl_mcp_se_otp_se_parse_oacp_req(GA_ENDPOINT * device, MCP_SE_EVT * evt);

void appl_mcp_se_otp_se_set_curr_obj(MCP_SE_EVT * evt);

API_RESULT appl_mcp_se_otp_se_OACP_read_proc_handler
           (
               UINT8  * param,
               UINT16   param_len
           );

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/* GMCS Player pointing to a MCS player */
typedef UINT8 MCS_GMCS_PLAYER_ID;

typedef struct _MCS_CURR_TRACK
{
    UCHAR                        track_title_len;
    MCP_CHAR_PARAM_TYPE_UTF8_STR   track_title[APPL_MCP_MAX_STR_LONG_LEN];
    MCP_CHAR_PARAM_TYPE_UTF8_STR   artist_name[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_UTF8_STR   album_name[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_UTF8_STR   group_name[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_UTF8_STR   earliest_year[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_UTF8_STR   latest_year[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_UTF8_STR   genre[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_UTF8_STR   only_tracks[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_UTF8_STR   only_groups[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_INT32      track_dur;
    MCP_CHAR_PARAM_TYPE_INT32      track_pos;
    MCP_CHAR_PARAM_TYPE_INT8       playback_speed;
    MCP_CHAR_PARAM_TYPE_INT8       seeking_speed;
    MCP_PARAM_OBJ_ID             curr_track_seg_obj_id;
    MCP_PARAM_OBJ_ID             curr_track_obj_id;
    MCP_PARAM_OBJ_ID             next_track_obj_id;
    MCP_PARAM_OBJ_ID             parent_grp_obj_id;
    MCP_PARAM_OBJ_ID             curr_grp_obj_id;
    MCP_CHAR_PARAM_TYPE_UINT32     mcp_opc_supp;
    MCP_PARAM_OBJ_ID             search_res_obj_id;
    UINT8                        search_res_obj_id_len;
    UINT8                        ccid;
}MCS_CURR_TRACK;

typedef struct _MCS_MP_PARAMS
{
    UCHAR                 mp_name_len;
    MCP_CHAR_PARAM_TYPE_UTF8_STR    mp_name[APPL_MCP_MAX_STR_LONG_LEN];
    MCP_PARAM_OBJ_ID      mp_icon;
    UINT16                mp_url_len;
    MCP_CHAR_PARAM_TYPE_UTF8_STR    mp_url[APPL_MCP_MAX_STR_LEN];
    MCP_CHAR_PARAM_TYPE_UINT8       media_state;
    MCP_CHAR_PARAM_TYPE_UINT8       playing_order;
    MCP_CHAR_PARAM_TYPE_UINT16      playing_order_supp;
}MCS_MP_PARAMS;

typedef struct _MCS_PLAYER
{
    /* Char Values of this Media Player */
    MCS_MP_PARAMS    mcs_mp_params;

    /* Current Media Player - Active */
    UINT8             is_mp_active;

    /* Current Track Details */
    MCS_CURR_TRACK   mcs_curr_track;

}MCS_PLAYER;

typedef struct _APPL_CNTRL_POINT_RSP_PARAM
{
    GA_ENDPOINT     dev;
    MCP_SE_HANDLE   mcp_se_handle;
    UCHAR         * char_info;
    UINT16          char_info_len;
}APPL_CNTRL_POINT_RSP_PARAM;

typedef struct _APPL_MCP_NTF_INFO
{
    GA_ENDPOINT     device;
    UINT8           mcs_sess_id;
    MCP_SE_HANDLE   mcp_se_handle;
    UINT16          mcp_ntf_type;
}APPL_MCP_NTF_INFO;

MCS_PLAYER appl_mcs_players[APPL_MCP_MAX_MCS_SESSIONS];
MCS_GMCS_PLAYER_ID appl_gmcs_player_id;

MCP_SE_HANDLE mcs_sess_inst_id;

UCHAR appl_mcp_no_obj_set[APPL_OBJ_ID_LEN] = { 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU };

UCHAR appl_mcp_se_otp_se_feature[8U] = { 0x77U, 0x01U, 0x00U, 0x00U, 0x05U, 0x00U, 0x00U, 0x00U };

UCHAR appl_mcp_grp_obj_1_data[APPL_OBJ_DATA_MAX_LEN];
UCHAR appl_mcp_grp_obj_2_data[APPL_OBJ_DATA_MAX_LEN];
UCHAR appl_mcp_grp_obj_3_data[APPL_OBJ_DATA_MAX_LEN];
UCHAR appl_mcp_parent_grp_obj_data[APPL_OBJ_DATA_MAX_LEN];
UCHAR appl_mcp_track_obj_data[APPL_OBJ_DATA_MAX_LEN];
UCHAR appl_mcp_icon_obj_data[APPL_OBJ_DATA_MAX_LEN];
UCHAR appl_mcp_seg_obj_data[APPL_OBJ_DATA_MAX_LEN];
UCHAR appl_mcp_search_res_obj_data[APPL_OBJ_DATA_MAX_LEN];

UINT16 lcid;

MCP_SE_HANDLE appl_mcp_se_vlc_handle;
MCP_SE_HANDLE appl_mcp_se_mpc_handle;

/* Timer Handle to send MCP Control Point Response to client post expiry */
static BT_timer_handle appl_mcp_se_send_mcp_cp_rsp_timer_handle;
static BT_timer_handle appl_mcp_se_send_ntf_timer_handle;

/* Timer Handle to notify Track Position to client */
static BT_timer_handle appl_mcp_se_send_ntf_curr_track_pos_timer_handle;

/* Timer Handle to notify Playback Speed to client */
static BT_timer_handle appl_mcp_se_send_ntf_curr_playback_speed_timer_handle;

/* Timer Handle to notify Current Track Object ID to client */
static BT_timer_handle appl_mcp_se_send_ntf_curr_track_obj_id_timer_handle;

/* Timer Handle to notify Playing Order to client */
static BT_timer_handle appl_mcp_se_send_ntf_playing_order_timer_handle;

/* Timer Handle to notify Search Result Object ID to client */
static BT_timer_handle appl_mcp_se_send_ntf_search_res_obj_id_timer_handle;

UCHAR track_obj_data[APPL_OBJ_DATA_MAX_LEN] = "ID3v2 Data";
UCHAR icon_obj_data[APPL_OBJ_DATA_MAX_LEN] = ".png Format Data";
UCHAR seg_name_first[APPL_OBJ_DATA_MAX_LEN] = "Segment 1";
UCHAR seg_name_second[APPL_OBJ_DATA_MAX_LEN] = "Segment 2";
UCHAR seg_name_third[APPL_OBJ_DATA_MAX_LEN] = "Segment 3";
UCHAR seg_name_fourth[APPL_OBJ_DATA_MAX_LEN] = "Segment 4";

OTP_SE_OBJ_METADATA appl_mcp_se_track_obj_1 =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_TRACK,

    /* Object Size */
    /* Current Size */
    { 0x0AU, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x0AU, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x01U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    15U,

    /* Object Name */
    "Track Object 1",

    /* Object Length */
    0U,

    /* Object Data - Current Track */
    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_track_obj_2 =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_TRACK,

    /* Object Size */
    /* Current Size */
    { 0x0AU, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x0AU, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x02U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    15U,

    /* Object Name */
    "Track Object 2",

    /* Object Length */
    0U,

    /* Object Data - Current Track */
    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_track_obj_3 =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_TRACK,

    /* Object Size */
    /* Current Size */
    { 0x0AU, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x0AU, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x08U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    15U,

    /* Object Name */
    "Track Object 3",

    /* Object Length */
    0U,

    /* Object Data - Current Track */
    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_icon_obj_vlc =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_MPLAYER_ICON,

    /* Object Size */
    /* Current Size */
    { 0x10U, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x10U, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x03U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    8U,

    /* Object Name */
    "VLC Icon",

    /* Object Length */
    0U,

    /* Object Data - Icon in .png format */
    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_icon_obj_mpc =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_MPLAYER_ICON,

    /* Object Size */
    /* Current Size */
    { 0x10U, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x10U, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x04U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    8U,

    /* Object Name */
    "MPC Icon",

    /* Object Length */
    0U,

    /* Object Data - Icon in .png format */
    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_search_res_obj =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_GROUP,

    /* Object Size */
    /* Current Size */
    { 0x07U, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x07U, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x05U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    19U,

    /* Object Name */
    "Search Result Object",

    /* Object Length: Search Result is a Obj ID */
    0U,

    /* Object Data - Search Result Obj ID : Track Obj 1 */
    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_track_segment_obj =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_TRACK_SEGMENTS,

    /* Object Size */
    /* Current Size */
    { 0x1CU, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x1CU, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x06U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    14U,

    /* Object Name */
    "Segment Object",

    /* Object Length: Segment Name Length + Segment Name + Segment Position */
    0U,

    /* Object Data - Search Result Obj ID : Track Obj 1 */
    NULL
};

/* Parent Group and Current Group having same Object ID */

OTP_SE_OBJ_METADATA appl_mcp_se_parent_grp_obj =
{
    /* Object Type - Group */
    GA_OBJTYPE_MCS_GROUP,

    /* Object Size */
    /* Current Size */
    { 0x15U, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x15U, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x07U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    17U,

    /* Object Name */
    "Parent Grp Object",

    0U,

    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_grp_obj_1 =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_GROUP,

    /* Object Size */
    /* Current Size */
    { 0x07U, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x07U, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x09U, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    14U,

    /* Object Name */
    "Group Object 1",

    0U,

    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_grp_obj_2 =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_GROUP,

    /* Object Size */
    /* Current Size */
    { 0x07U, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x07U, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x0AU, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    14U,

    /* Object Name */
    "Group Object 2",

    0U,

    NULL
};

OTP_SE_OBJ_METADATA appl_mcp_se_grp_obj_3 =
{
    /* Object Type - Track */
    GA_OBJTYPE_MCS_GROUP,

    /* Object Size */
    /* Current Size */
    { 0x15U, 0x00U, 0x00U, 0x00U },

    /* Allocated Size */
    { 0x15U, 0x00U, 0x00U, 0x00U },

    /* Object First Created - 2013/11/01 10:31:17 */
    { 0xDDU, 0x07U, 0x0BU, 0x01U, 0x0AU, 0x1FU, 0x11U },

    /* Object Last Modified - 2013/11/08 16:25:00 */
    { 0xDDU, 0x07U, 0x0BU, 0x08U, 0x10U, 0x19U, 0x00U },

    /* Object Properties */
    0x000000FDU, /* Patch|Truncate|Append|Write|Read|Delete*/

    /* Object ID */
    { 0x00U, 0x0BU, 0x00U, 0x00U, 0x00U, 0x00U },

    /* Object Name Length */
    14U,

    /* Object Name */
    "Group Object 3",

    0U,

    NULL
};

OTP_SE_OBJ_METADATA * appl_otp_se_curr_obj;

UCHAR mpNameValueChangedDuringRead;
UCHAR mpTrackTitleValueChangedDuringRead;

/* --------------------------------------------- External Global Variables */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */


/* --------------------------------------------- Functions */

GA_RESULT appl_mcp_se_send_ntf_curr_track_pos_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          );
GA_RESULT appl_mcp_se_send_ntf_curr_playback_speed_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          );
GA_RESULT appl_mcp_se_send_ntf_curr_track_obj_id_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          );
GA_RESULT appl_mcp_se_send_ntf_next_track_obj_id
          (
              GA_ENDPOINT   * dev,
              UINT8           mcs_sess_id,
              MCP_SE_HANDLE   mcp_se_handle
          );
GA_RESULT appl_mcp_se_handle_write_curr_grp_id
          (
              GA_ENDPOINT   * dev,
              UINT8           mcs_sess_id,
              MCP_SE_HANDLE   mcp_se_handle,
              UINT8         * new_grp_obj_id
          );
GA_RESULT appl_mcp_se_send_ntf_playing_order_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          );
GA_RESULT appl_mcp_se_send_ntf_search_res_obj_id_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          );
GA_RESULT appl_mcp_handle_mcp_ind
          (
              GA_ENDPOINT * dev,
              MCP_SE_EVT  * evt,
              UINT8         mcs_sess_id
          );
GA_RESULT appl_mcp_handle_scp_ind
          (
              GA_ENDPOINT * dev,
              MCP_SE_EVT  * evt,
              UINT8         mcs_sess_id
          );
void appl_mcp_se_send_mcp_cp_rsp_on_timer_expiry_handler
     (
         void   * cntrl_point_rsp,
         UINT16   datalen
     );
void appl_mcp_se_send_ntf_timer_expiry_handler
     (
         void   * data,
         UINT16   datalen
     );
void appl_set_mcs_sdp_record_status(UCHAR flag);

void main_mcp_se_menu_operations(void)
{
    int       choice, input, menu_choice;
    GA_RESULT retval;
    UCHAR ntf_data[20U];
    MCP_SE_HANDLE appl_mcp_se_handle;
    UINT8      conn_dev_index;

    GA_LOOP_FOREVER()
    {
        CONSOLE_OUT("%s", ga_mcp_se_appl_menu);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 5:
            /* By default, Set VLC Player as GMCS Instance */
            appl_update_default_vlc_char_values();

            APPL_TRC("GA_mcp_se_init...\n");
            retval = GA_mcp_se_init(appl_mcp_se_callback);
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            APPL_TRC("By default, GMCS is set to VLC Player Instance\n");

            /* MCP SE Init Objects Data and len */
            appl_mcp_se_otp_se_init_obj();
            break;

        case 6:
            APPL_TRC("GA_mcp_se_shutdown...\n");
            retval = GA_mcp_se_shutdown();
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 7:
            CONSOLE_OUT("Enter Media Player to be added: [1: VLC] [2:MPC] ");
            CONSOLE_IN("%d", &input);
            APPL_TRC("GA_mcp_se_add_mcs_inst...\n");
            retval = GA_mcp_se_add_mcs_inst(&appl_mcp_se_handle);
            APPL_TRC("Session ID: %x, Retval - ",
                appl_mcp_se_handle);
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_MCS
            );
            if (GA_SUCCESS == retval)
            {
                if (APPL_MCP_VLC_PLAYER == input)
                {
                    appl_mcp_se_vlc_handle = appl_mcp_se_handle;
                    appl_update_default_vlc_char_values();
                }
                else if (APPL_MCP_MPC_PLAYER == input)
                {
                    appl_mcp_se_mpc_handle = appl_mcp_se_handle;
                    appl_update_default_mpc_char_values();
                }
            }
            break;

        case 9:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &appl_ga_mcs_device
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter endpoint address: ");
                appl_get_bd_addr(appl_ga_mcs_device.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &input);
                appl_ga_mcs_device.bd_type = (UINT8)input;
            }
            break;

        case 10:
            CONSOLE_OUT("Session Instance ID: \n");
            CONSOLE_IN("%d", &input);
            mcs_sess_inst_id = (UINT8)input;
            break;

        case 12:
            CONSOLE_OUT
            (
                "Set GMCS to an active Media Player MCS Instance: [1: VLC] [2:MPC]\n"
            );
            CONSOLE_IN("%d", &input);
            appl_mcp_se_handle = (UINT8)input;

            appl_mcs_players[appl_mcp_se_handle - 1U].is_mp_active = GA_TRUE;
            if (APPL_MCP_VLC_PLAYER == input)
            {
                appl_gmcs_player_id = appl_mcp_se_vlc_handle - 1U;
            }
            else if (APPL_MCP_MPC_PLAYER == input)
            {
                appl_gmcs_player_id = appl_mcp_se_mpc_handle - 1U;
            }
            break;

        case 15:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name,
                (appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name_len * sizeof(UCHAR))
            );
            APPL_TRC("GA_mcp_se_notify_media_player_name...\n");
            retval = GA_mcp_se_notify_media_player_name
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name_len
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 31:
            APPL_TRC("GA_mcp_se_notify_track_changed...\n");
            retval = GA_mcp_se_notify_track_changed
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         NULL,
                         0U
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 16:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title,
                (appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title_len * sizeof(UCHAR))
            );
            APPL_TRC("GA_mcp_se_notify_track_title...\n");
            retval = GA_mcp_se_notify_track_title
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title_len
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 17:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_dur,
                sizeof(MCP_CHAR_PARAM_TYPE_INT32)
            );
            APPL_TRC("GA_mcp_se_notify_track_duration...\n");
            retval = GA_mcp_se_notify_track_duration
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         sizeof(MCP_CHAR_PARAM_TYPE_INT32)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 18:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_pos,
                sizeof(MCP_CHAR_PARAM_TYPE_INT32)
            );
            APPL_TRC("GA_mcp_se_notify_track_pos...\n");
            retval = GA_mcp_se_notify_track_pos
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         sizeof(MCP_CHAR_PARAM_TYPE_INT32)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 19:
            ntf_data[0U] = appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.playback_speed;
            APPL_TRC("GA_mcp_se_notify_playback_speed...\n");
            retval = GA_mcp_se_notify_playback_speed
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         sizeof(MCP_CHAR_PARAM_TYPE_INT8)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 20:
            ntf_data[0U] = appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.seeking_speed;
            APPL_TRC("GA_mcp_se_notify_seeking_speed...\n");
            retval = GA_mcp_se_notify_seeking_speed
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         sizeof(MCP_CHAR_PARAM_TYPE_INT8)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 21:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_track_obj_id,
                (MCP_OBJ_ID_LEN * sizeof(UCHAR))
            );
            APPL_TRC("GA_mcp_se_notify_curr_track_obj_id...\n");
            retval = GA_mcp_se_notify_curr_track_obj_id
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         MCP_OBJ_ID_LEN
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 22:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.next_track_obj_id,
                (MCP_OBJ_ID_LEN * sizeof(UCHAR))
            );
            APPL_TRC("GA_mcp_se_notify_next_track_obj_id...\n");
            retval = GA_mcp_se_notify_next_track_obj_id
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         MCP_OBJ_ID_LEN
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 23:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.parent_grp_obj_id,
                (MCP_OBJ_ID_LEN * sizeof(UCHAR))
            );
            APPL_TRC("GA_mcp_se_notify_parent_grp_obj_id...\n");
            retval = GA_mcp_se_notify_parent_grp_obj_id
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         MCP_OBJ_ID_LEN
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 24:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_grp_obj_id,
                (MCP_OBJ_ID_LEN * sizeof(UCHAR))
            );
            APPL_TRC("GA_mcp_se_notify_curr_grp_obj_id...\n");
            retval = GA_mcp_se_notify_curr_grp_obj_id
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         MCP_OBJ_ID_LEN
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 25:
            ntf_data[0U] = appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.playing_order;
            APPL_TRC("GA_mcp_se_notify_playing_order...\n");
            retval = GA_mcp_se_notify_playing_order
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         sizeof(MCP_CHAR_PARAM_TYPE_UINT8)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 26:
            ntf_data[0U] = appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.media_state;
            APPL_TRC("GA_mcp_se_notify_media_state...\n");
            retval = GA_mcp_se_notify_media_state
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         sizeof(MCP_CHAR_PARAM_TYPE_UINT8)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 27:
        {
            MCP_PARAM_MCP_RSP mcp_rsp;

            CONSOLE_OUT("Input the Requested Opcode in Hex\n");
            CONSOLE_IN("%x", &input);
            mcp_rsp.req_opc = (UINT8 )input;
            CONSOLE_OUT("Input the Result Code in Hex\n");
            CONSOLE_OUT("[0x01]: Success, [0x02]: Opcode Not Supported\n");
            CONSOLE_OUT("[0x03]: Media Player Inactive, [0x04]: Command cannot be Completed\n");
            CONSOLE_IN("%x", &input);
            mcp_rsp.result = (UINT8)input;
            APPL_TRC("GA_mcp_se_notify_mcp...\n");
            retval = GA_mcp_se_notify_mcp
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &mcp_rsp,
                         sizeof(MCP_PARAM_MCP_RSP)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
            break;

        case 28:
        {
            MCP_CHAR_PARAM_TYPE_UINT32 mcp_cp_opc_supp_param;
            mcp_cp_opc_supp_param = 0x0002U;
            APPL_TRC("GA_mcp_se_notify_mcp_opc_supp...\n");
            retval = GA_mcp_se_notify_mcp_opc_supp
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &mcp_cp_opc_supp_param,
                         sizeof(MCP_CHAR_PARAM_TYPE_UINT32)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
            break;

        case 29:
        {
            MCP_SCP_RES_CODE scp_rsp;
            CONSOLE_OUT("Input the Result Code in Hex\n");
            CONSOLE_OUT("[0x01]: Success, [0x02]: Failure\n");
            CONSOLE_IN("%x", &input);
            scp_rsp = (UINT8 )input;
            APPL_TRC("GA_mcp_se_notify_scp...\n");
            retval = GA_mcp_se_notify_scp
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &scp_rsp,
                         sizeof(MCP_SCP_RES_CODE)
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
            break;

        case 30:
            GA_mem_copy
            (
                &ntf_data[0U],
                &appl_mcp_se_search_res_obj.obj_id[0U],
                (MCP_OBJ_ID_LEN * sizeof(UCHAR))
            );
            APPL_TRC("GA_mcp_se_notify_search_res_obj_id...\n");
            retval = GA_mcp_se_notify_search_res_obj_id
                     (
                         &appl_ga_mcs_device,
                         mcs_sess_inst_id,
                         &ntf_data[0U],
                         MCP_OBJ_ID_LEN
                     );
            APPL_TRC("Retval - ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
            break;

        case 32:
            CONSOLE_OUT("Set the Obj ID Len: Search Results\n");
            CONSOLE_IN("%d", &input);
            appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.search_res_obj_id_len = (UINT8 )input;
            break;

        case 33:
            CONSOLE_OUT("Set Media Player State:\n");
            CONSOLE_OUT
            (
                "[0x00]: Inactive, [0x01]: Playing, [0x02]: Paused, [0x03]: Seeking\n"
            );
            CONSOLE_IN("%d", &input);
            appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.media_state = (UINT8)input;
            if (MCP_MEDIA_STATE_INACTIVE ==
                appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.media_state)
            {
                appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_pos =
                    MCP_DEFAULT_TRACK_VALUE;
                appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_dur =
                    MCP_DEFAULT_TRACK_VALUE;
                /* Set Current Track Obj as Invalid */
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_no_obj_set[0U],
                    MCP_OBJ_ID_LEN
                );
            }
            break;

        case 34:
            APPL_TRC("Set the Current Track to a Valid Track\n");
            /* Set Current Track Obj as Valid by moving state from Inactive to Paused */
            GA_mem_copy
            (
                &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_track_obj_id,
                &appl_mcp_se_track_obj_1.obj_id[0U],
                MCP_OBJ_ID_LEN
            );
            appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.media_state =
                MCP_MEDIA_STATE_PAUSED;
            appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_pos =
                APPL_MCP_SE_TRACK_POS_MIN;
            appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_dur =
                APPL_MCP_SE_TRACK_POS_MAX;
            break;

        case 35:
            CONSOLE_OUT("[0x01]: First, [0x02]: Second\n");
            CONSOLE_IN("%d", &input);
            appl_mcp_se_generate_segment((UINT8) input);
            break;

        case 36:
            CONSOLE_OUT("Set Current Track\n");
            CONSOLE_OUT("[0x01]: Track 1\n");
            CONSOLE_OUT("[0x02]: Track 2\n");
            CONSOLE_OUT("[0x03]: Track 3\n");
            CONSOLE_IN("%d", &input);
            if (0x01 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_1.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.next_track_obj_id,
                    &appl_mcp_se_track_obj_2.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
            }
            else if (0x02 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_2.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.next_track_obj_id,
                    &appl_mcp_se_track_obj_3.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
            }
            else if (0x03 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_3.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.next_track_obj_id,
                    &appl_mcp_se_track_obj_1.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
            }
            break;

        case 37:
            CONSOLE_OUT("Set Current Group\n");
            CONSOLE_OUT("[0x01]: Group 1\n");
            CONSOLE_OUT("[0x02]: Group 2\n");
            CONSOLE_OUT("[0x03]: Group 3\n");
            CONSOLE_OUT("[0x04]: Parent Group\n");
            CONSOLE_IN("%d", &input);
            if (0x01 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_grp_obj_id,
                    &appl_mcp_se_grp_obj_1.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.parent_grp_obj_id,
                    &appl_mcp_se_parent_grp_obj.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
            }
            else if (0x02 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_grp_obj_id,
                    &appl_mcp_se_grp_obj_2.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.parent_grp_obj_id,
                    &appl_mcp_se_parent_grp_obj.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
            }
            else if (0x03 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_grp_obj_id,
                    &appl_mcp_se_grp_obj_3.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.parent_grp_obj_id,
                    &appl_mcp_se_parent_grp_obj.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
            }
            else if (0x04 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.curr_grp_obj_id,
                    &appl_mcp_se_parent_grp_obj.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.parent_grp_obj_id,
                    &appl_mcp_se_parent_grp_obj.obj_id[0U],
                    MCP_OBJ_ID_LEN
                );
            }
            break;

        case 38:
        {
            CONSOLE_OUT("Set Media Player Name\n");
            CONSOLE_OUT("[0x01]: VLC Media Player\n");
            CONSOLE_OUT("[0x02]: VLC Media Player Long\n");
            CONSOLE_OUT("[0x03]: VLC Media Player Long Chg\n");
            CONSOLE_IN("%d", &input);

            if (1 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name[0U],
                    "VLC Media Player",
                    16U
                );
                /* Update the media player name length */
                appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name_len = 16U;
            }
            else if (2 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name[0U],
                    "VLC Media Player Long",
                    21U
                );
                /* Update the media player name length */
                appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name_len = 21U;
            }
            else if (3 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name[0U],
                    "VLC Media Player Long Chg",
                    25U
                );
                /* Update the media player name length */
                appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name_len = 25U;
                mpNameValueChangedDuringRead = GA_TRUE;

                /* Notify change */
                APPL_TRC("GA_mcp_se_notify_media_player_name...\n");
                retval = GA_mcp_se_notify_media_player_name
                         (
                             &appl_ga_mcs_device,
                             mcs_sess_inst_id,
                             &appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name[0U],
                             appl_mcs_players[appl_gmcs_player_id].mcs_mp_params.mp_name_len
                         );
                APPL_TRC("Retval - ");
                appl_mcp_utils_display_ga_result
                (
                    retval,
                    GA_SERVICE_GMCS
                );
            }
        }
            break;

        case 39:
        {
            CONSOLE_OUT("Set Track Title Name\n");
            CONSOLE_OUT("[0x01]: AtoBtoCto\n");
            CONSOLE_OUT("[0x02]: AtoBtoCto Long Long Long\n");
            CONSOLE_OUT("[0x03]: AtoBtoCto Long Long Chg\n");
            CONSOLE_IN("%d", &input);

            if (1 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title[0U],
                    "AtoBtoCto",
                    10U
                );
                appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title_len = 10U;
            }
            else if (2 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title[0U],
                    "AtoBtoCto Long Long Long",
                    24U
                );
                appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title_len = 24U;
            }
            else if (3 == input)
            {
                GA_mem_copy
                (
                    &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title[0U],
                    "AtoBtoCto Long Long Chg",
                    23U
                );
                appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title_len = 23U;
                mpTrackTitleValueChangedDuringRead = GA_TRUE;

                /* Notify change */
                APPL_TRC("GA_mcp_se_notify_track_title...\n");
                retval = GA_mcp_se_notify_track_title
                         (
                             &appl_ga_mcs_device,
                             mcs_sess_inst_id,
                             &appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title[0U],
                             appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_title_len
                         );
                APPL_TRC("Retval - ");
                appl_mcp_utils_display_ga_result
                (
                    retval,
                    GA_SERVICE_GMCS
                );
            }
        }
        break;

        case 40:
        {
            CONSOLE_OUT("Set SDP Record for MCS: 0[Disable], 1[Enable]\n");
            CONSOLE_IN("%d", &input);
            appl_set_mcs_sdp_record_status((UCHAR )input);
        }
            break;

        default:
            APPL_INF("Invalid Choice\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

GA_RESULT appl_mcp_se_callback
          (
              /* IN */ GA_ENDPOINT           * device,
              /* IN */ GA_MCP_SE_RSP_CONTEXT * ctx,
              /* IN */ MCP_SE_EVT_ID           evt_id,
              /* IN */ UINT16                  evt_status,
              /* IN */ MCP_SE_EVT            * evt_data
          )
{
    UINT8       mcs_sess_id;
    UINT8       send_rsp;
    UINT16      offset;
    UINT16      rsp_len;
    GA_RESULT   retval;
    void      * rsp;
    UCHAR       index;

    APPL_MCP_NTF_INFO  mcs_ntf_info;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    APPL_CNTRL_POINT_RSP_PARAM  ots_cntrl_rsp;
    UINT32                      oacp_feature;
    UINT32                      olcp_feature;
    UINT32                      curr_size;
    UINT32                      alloc_size;
    UCHAR                       obj_size[8U];
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    /* Initialize */
    send_rsp = GA_TRUE;
    rsp = NULL;
    rsp_len = 0U;
    offset = 0U;
    retval = GA_SUCCESS;
    index = 0U;
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    oacp_feature = 0U;
    olcp_feature = 0U;
    curr_size = 0U;
    alloc_size = 0U;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    APPL_TRC("[MCS_APPL] >> appl_mcs_callback\n");

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("MCP Server");

    if (MCP_SE_GMCS_HANDLE == evt_data->mcp_se_handle)
    {
        APPL_TRC("[Profile]     : MCP - GMCS (0x%04X)\n", GA_SERVICE_GMCS);
        mcs_sess_id = appl_gmcs_player_id;
    }
    else
    {
        APPL_TRC("[Profile]     : MCP - MCS (0x%04X)\n", GA_SERVICE_MCS);
        mcs_sess_id = evt_data->mcp_se_handle - 1;
    }
    APPL_TRC("[SECTION]     : MCP SERVER EVENTS\n");

    switch (evt_id)
    {
    case MCP_SE_READ_MEDIA_PLAYER_NAME_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        if (GA_TRUE == ctx->is_blob)
        {
            APPL_TRC("[TYPE]        : READ BLOB REQUEST\n");
        }
        else
        {
            APPL_TRC("[TYPE]        : READ REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_MEDIA_PLAYER_NAME_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Player Name (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_PLAYER_NAME
        );
        GA_FORMAT_DISPLAY();
        if (!mpNameValueChangedDuringRead)
        {
            if ((NULL != evt_data->data) && (0U != evt_data->len))
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("RECEIVED DATA\n");
                GA_FORMAT_DISPLAY();
                if (2U == evt_data->len)
                {
                    GA_UNPACK_LE_2_BYTE(&offset, (UCHAR*)evt_data->data);
                    rsp_len = appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_name_len - offset;
                    APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
                    APPL_TRC("Data:\n");
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Offset: 0x%04X\n",
                        (UCHAR)sizeof(UINT16),
                        offset
                    );
                }
                else
                {
                    /* Event Length Incorrect - Skip */
                    APPL_ERR("Incorrect Length For Offset Value: %d !", evt_data->len);

                    /* TODO: Need a way to send error response when Offset Value parameter is incorrect */
                }
            }
            else
            {
                offset = 0U;
                rsp_len = appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_name_len;
            }
            rsp = &appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_name[offset];
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%04X\n", rsp_len);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%04X,  Media Player Name: ", rsp_len);
            for (index = (UINT8 )offset; index < appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_name_len; index++)
            {
                APPL_TRC("%c", appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_name[index]);
            }
            APPL_TRC("\n");
        }
        else
        {
            retval = MCP_ERR_CODE_VALUE_CHANGED_DURING_READ_LONG;
            mpNameValueChangedDuringRead = GA_FALSE;
        }
        break;

    case MCP_SE_READ_MP_ICON_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_MP_ICON_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Player Icon Object ID (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_PLAYER_ICON_OBJ_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_icon;
        rsp_len = sizeof(MCP_PARAM_OBJ_ID);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == rsp_len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Media Player Icon Object ID: Media player has no icon (NULL)\n",
                rsp_len
            );
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Media Player Icon Object ID: 0x", rsp_len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_icon.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_SE_READ_MP_ICON_URL_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_MP_ICON_URL_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Player Icon URL (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_PLAYER_ICON_URL
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_url;
        rsp_len = appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_url_len;
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Media Player Icon URL: ", rsp_len);
        for (index = 0U; index < rsp_len; index++)
        {
            APPL_TRC("%c", appl_mcs_players[mcs_sess_id].mcs_mp_params.mp_url[index]);
        }
        APPL_TRC("\n");
        break;

    case MCP_SE_READ_TRACK_TITLE_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        if (GA_TRUE == ctx->is_blob)
        {
            APPL_TRC("[TYPE]        : READ BLOB REQUEST\n");
        }
        else
        {
            APPL_TRC("[TYPE]        : READ REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_TRACK_TITLE_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Title (0x%04X)\n",
            GA_CHAR_MCS_TRACK_TITLE
        );
        GA_FORMAT_DISPLAY();
        if (!mpTrackTitleValueChangedDuringRead)
        {
            if ((NULL != evt_data->data) && (0U != evt_data->len))
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("RECEIVED DATA\n");
                GA_FORMAT_DISPLAY();
                if (2U == evt_data->len)
                {
                    GA_UNPACK_LE_2_BYTE(&offset, (UCHAR*)evt_data->data);
                    rsp_len = appl_mcs_players[mcs_sess_id].mcs_curr_track.track_title_len - offset;
                    APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
                    APPL_TRC("Data:\n");
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Offset: 0x%04X\n",
                        (UCHAR)sizeof(UINT16),
                        offset
                    );
                }
                else
                {
                    /* Event Length Incorrect - Skip */
                    APPL_ERR("Incorrect Length For Offset Value: %d !", evt_data->len);

                    /* TODO: Need a way to send error response when Offset Value parameter is incorrect */
                }
            }
            else
            {
                offset = 0U;
                rsp_len = appl_mcs_players[mcs_sess_id].mcs_curr_track.track_title_len;
            }
            rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.track_title[offset];
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%04X\n", rsp_len);
            if (MCP_DEFAULT_UTF8_STR_LEN == rsp_len)
            {
                APPL_TRC
                (
                    "Data: NULL (No current track or Track title unavailable)\n"
                );
            }
            else
            {
                APPL_TRC("Data:\n");
                APPL_TRC("Len: 0x%02X,  Track Title: ", rsp_len);
                for (index = (UINT8 )offset; index < appl_mcs_players[mcs_sess_id].mcs_curr_track.track_title_len; index++)
                {
                    APPL_TRC("%c", appl_mcs_players[mcs_sess_id].mcs_curr_track.track_title[index]);
                }
                APPL_TRC("\n");
            }
        }
        else
        {
            retval = MCP_ERR_CODE_VALUE_CHANGED_DURING_READ_LONG;
            mpTrackTitleValueChangedDuringRead = GA_FALSE;
        }
        break;

    case MCP_SE_READ_TRACK_DUR_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_TRACK_DUR_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Duration (0x%04X)\n",
            GA_CHAR_MCS_TRACK_DURATION
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.track_dur;
        rsp_len = sizeof(MCP_CHAR_PARAM_TYPE_INT32);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_TRACK_VALUE == *((INT32*)rsp))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Track Duration: Media player has no current track or the duration of the current track is unknown (0x%08X)\n",
                rsp_len,
                *((INT32*)rsp)
            );
        }
        else
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Track Duration: 0x%08X\n",
                rsp_len,
                *((INT32*)rsp)
            );
        }
        break;

    case MCP_SE_READ_TRACK_POS_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_TRACK_POS_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Position (0x%04X)\n",
            GA_CHAR_MCS_TRACK_POS
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos;
        rsp_len = sizeof(MCP_CHAR_PARAM_TYPE_INT32);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_TRACK_VALUE == *((INT32*)rsp))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Track Position: Unavailable (0x%08X)\n",
                rsp_len,
                *((INT32*)rsp)
            );
        }
        else
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Track Position: 0x%08X\n",
                rsp_len,
                *((INT32*)rsp)
            );
        }
        break;

    case MCP_SE_READ_PLAYBACK_SPEED_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_PLAYBACK_SPEED_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playback Speed (0x%04X)\n",
            GA_CHAR_MCS_PLAYBACK_SPEED
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed;
        rsp_len = sizeof(MCP_CHAR_PARAM_TYPE_INT8);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  Playback Speed: 0x%02X\n",
            rsp_len,
            *((INT8*)rsp)
        );
        break;

    case MCP_SE_READ_SEEKING_SPEED_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_SEEKING_SPEED_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Seeking Speed (0x%04X)\n",
            GA_CHAR_MCS_SEEKING_SPEED
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.seeking_speed;
        rsp_len = sizeof(MCP_CHAR_PARAM_TYPE_INT8);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_SEEKING_SPEED == *((INT8*)rsp))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Seeking Speed: Media player is not seeking (0x%02X)\n",
                rsp_len,
                *((INT8*)rsp)
            );
        }
        else
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Seeking Speed: 0x%02X\n",
                rsp_len,
                *((INT8*)rsp)
            );
        }
        break;

    case MCP_SE_READ_CURR_TRACK_SEG_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_CURR_TRACK_SEG_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Track Segments Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_TRACK_SEG_OBJ_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_seg_obj_id;
        rsp_len = sizeof(MCP_PARAM_OBJ_ID);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == rsp_len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Current Track Segments Object ID: Media player has no current track (NULL)\n",
                rsp_len
            );
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Current Track Segments Object ID: 0x", rsp_len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_seg_obj_id.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_SE_READ_CURR_TRACK_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_CURR_TRACK_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id;
        rsp_len = sizeof(MCP_PARAM_OBJ_ID);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == rsp_len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Current Track Object ID: Media player has no current track (NULL)\n",
                rsp_len
            );
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Current Track Object ID: 0x", rsp_len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_SE_READ_NEXT_TRACK_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_NEXT_TRACK_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Next Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_NEXT_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id;
        rsp_len = sizeof(MCP_PARAM_OBJ_ID);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == rsp_len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Next Track Object ID: Media player has no next track (NULL)\n",
                rsp_len
            );
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Next Track Object ID: 0x", rsp_len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_SE_READ_PARENT_GROUP_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_PARENT_GROUP_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Parent Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_PARENT_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.parent_grp_obj_id;
        rsp_len = sizeof(MCP_PARAM_OBJ_ID);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == rsp_len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Parent Group Object ID: Media player has not created a parent group or a current group (NULL)\n",
                rsp_len
            );
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Parent Group Object ID: 0x", rsp_len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.parent_grp_obj_id.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_SE_READ_CURR_GROUP_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_CURR_GROUP_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id;
        rsp_len = sizeof(MCP_PARAM_OBJ_ID);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == rsp_len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Current Group Object ID: Media player has not created a current group (NULL)\n",
                rsp_len
            );
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Current Group Object ID: 0x", rsp_len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_SE_READ_PLAYING_ORDER_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_PLAYING_ORDER_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playing Order (0x%04X)\n",
            GA_CHAR_MCS_PLAYING_ORDER
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order;
        rsp_len = sizeof(MCP_CHAR_PARAM_TYPE_UINT8);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Playing Order: ", rsp_len);
        appl_mcp_utils_parse_and_display_playing_order(*((MCP_CHAR_PARAM_TYPE_UINT8*)rsp));
        break;

    case MCP_SE_READ_PLAYING_ORDER_SUPP_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_PLAYING_ORDER_SUPP_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playing Orders Supported (0x%04X)\n",
            GA_CHAR_MCS_PLAYING_ORDERS_SUPP
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp;
        rsp_len = sizeof(MCP_CHAR_PARAM_TYPE_UINT16);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  Playing Orders Supported: 0x%04X\n",
            rsp_len,
            *((MCP_CHAR_PARAM_TYPE_UINT16*)rsp)
        );
        appl_mcp_utils_parse_and_display_playing_orders_supp(*((MCP_CHAR_PARAM_TYPE_UINT16*)rsp));
        break;

    case MCP_SE_READ_MEDIA_STATE_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_MEDIA_STATE_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media State (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_STATE
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state;
        rsp_len = sizeof(MCP_CHAR_PARAM_TYPE_UINT8);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Media State: ", rsp_len);
        appl_mcp_utils_parse_and_display_media_state(*((MCP_CHAR_PARAM_TYPE_UINT8*)rsp));
        break;

    case MCP_SE_READ_MCP_OPC_SUPP_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_MCP_OPC_SUPP_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Control Point Opcodes Supported (0x%04X)\n",
            GA_CHAR_MCS_CP_OPC_SUPP
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.mcp_opc_supp;
        rsp_len = sizeof(MCP_CHAR_PARAM_TYPE_UINT32);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  Media Control Point Opcodes Supported: 0x%08X\n",
            rsp_len,
            *((MCP_CHAR_PARAM_TYPE_UINT32*)rsp)
        );
        appl_mcp_utils_parse_and_display_mcp_opc_supp(*((MCP_CHAR_PARAM_TYPE_UINT32*)rsp));
        break;

    case MCP_SE_READ_SEARCH_RES_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_SEARCH_RES_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Search Results Object ID (0x%04X)\n",
            GA_CHAR_MCS_SEARCH_RES_OBJ_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.search_res_obj_id;
        rsp_len = appl_mcs_players[mcs_sess_id].mcs_curr_track.search_res_obj_id_len;
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        if (MCP_DEFAULT_OBJ_ID_LEN == rsp_len)
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Search Results Object ID: There are no search results (NULL)\n",
                rsp_len
            );
        }
        else
        {
            APPL_TRC("Len: 0x%02X,  Search Results Object ID: 0x", rsp_len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.search_res_obj_id.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");
        }
        break;

    case MCP_SE_READ_CCID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_READ_CCID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Content Control ID (0x%04X)\n",
            GA_CHAR_CONTENT_CONTROL_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcs_players[mcs_sess_id].mcs_curr_track.ccid;
        rsp_len = sizeof(UINT8);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  Content Control ID: 0x%02X\n",
            rsp_len,
            appl_mcs_players[mcs_sess_id].mcs_curr_track.ccid
        );
        break;

    case MCP_SE_WRITE_TRACK_POS_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-WRITE\n");
        /* It is a Write Command, Do not send Response */
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_WRITE_TRACK_POS_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Track Position (0x%04X)\n",
            GA_CHAR_MCS_TRACK_POS
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if (sizeof(MCP_CHAR_PARAM_TYPE_INT32) == evt_data->len)
        {
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            if (MCP_DEFAULT_TRACK_VALUE == *(MCP_CHAR_PARAM_TYPE_INT32 *)evt_data->data)
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  Track Position: Unavailable (0x%08X)\n",
                    evt_data->len,
                    *(MCP_CHAR_PARAM_TYPE_INT32 *)evt_data->data
                );
            }
            else
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  Track Position: 0x%08X\n",
                    evt_data->len,
                    *(MCP_CHAR_PARAM_TYPE_INT32 *)evt_data->data
                );
            }

            if (0 <= *(MCP_CHAR_PARAM_TYPE_INT32 *)evt_data->data)
            {
                /* New Current Position is positive: Just move to the position */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos = *(MCP_CHAR_PARAM_TYPE_INT32 *)evt_data->data;
            }
            else
            {
                /*
                 * New Current Position is negative:
                 *  1. Move current playing position from end of track wrt offset.
                 *  Eg: track_dur: 100, track_pos: 10
                 *      Requested track_pos: -30
                 *      100 - 30 = 70
                 *      New track_pos = 70
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.track_dur +
                    *(MCP_CHAR_PARAM_TYPE_INT32 *)evt_data->data;
            }
            if ((0 <= appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos)
                &&
                (appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos <=
                 appl_mcs_players[mcs_sess_id].mcs_curr_track.track_dur))
            {
                /* APPL_TRC("Set to a valid track position, Requested Offset: %d\n",
                    *(MCP_CHAR_PARAM_TYPE_INT32 *)evt_data->data); */
            }
            else
            {
                APPL_ERR
                (
                    "Does not correspond to a valid track position, Requested Offset: %d !\n",
                    *(MCP_CHAR_PARAM_TYPE_INT32*)evt_data->data
                );
                /* Set it to a valid value, moving it back to 0 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos = 0;
            }
            /* APPL_TRC("Current Track Position: %d\n", appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos); */

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }

            /* Copy the info to send notification */
            GA_COPY_BRR_DEVICE(&mcs_ntf_info.device, device);
            mcs_ntf_info.mcs_sess_id = mcs_sess_id;
            mcs_ntf_info.mcp_se_handle = evt_data->mcp_se_handle;
            /* Schedule a timer to send Notification for Current Track Position */
            BT_start_timer
            (
                &appl_mcp_se_send_ntf_curr_track_pos_timer_handle,
                1U,
                appl_mcp_se_send_ntf_curr_track_pos_timer_expiry_handler,
                &mcs_ntf_info,
                sizeof(mcs_ntf_info)
            );
        }
        else
        {
            APPL_ERR("Parameter Length of Write Request Invalid !\n");
        }
        break;

    case MCP_SE_WRITE_TRACK_PLAYBACK_SPEED_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-WRITE\n");
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_WRITE_TRACK_PLAYBACK_SPEED_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playback Speed (0x%04X)\n",
            GA_CHAR_MCS_PLAYBACK_SPEED
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if (sizeof(MCP_CHAR_PARAM_TYPE_INT8) == evt_data->len)
        {
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Playback Speed: 0x%02X\n",
                evt_data->len,
                *(MCP_CHAR_PARAM_TYPE_INT8 *)evt_data->data
            );
            if ((*(MCP_CHAR_PARAM_TYPE_INT8 *)evt_data->data == -128)
                ||
                (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data == -64)
                ||
                (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data == 0)
                ||
                (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data == 64)
                ||
                (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data == 127))
            {
                /* It is a Valid Playback speed, set it */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed =
                    (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data);
            }
            else
            {
                switch(appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed)
                {
                case -128:
                    if (-128 < (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data))
                    {
                        /* Value is greater, Set to the next Highest */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = -64;
                    }
                    else
                    {
                        /* Value is lesser, not defined in spec in this case */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = -128;
                    }
                    break;
                case -64:
                    if (-64 < (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data))
                    {
                        /* Value is greater, Set to the next Highest */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = 0;
                    }
                    else
                    {
                        /* Value is lesser, Set to the next Lowest */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = -128;
                    }
                    break;
                case 0:
                    if (0 < (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data))
                    {
                        /* Value is greater, Set to the next Highest */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = 64;
                    }
                    else
                    {
                        /* Value is lesser, Set to the next Lowest */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = -64;
                    }
                    break;
                case 64:
                    if (64 < (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data))
                    {
                        /* Value is greater, Set to the next Highest */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = 127;
                    }
                    else
                    {
                        /* Value is lesser, Set to the next Lowest */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = 0;
                    }
                    break;
                case 127:
                    if (127 < (*(MCP_CHAR_PARAM_TYPE_INT8*)evt_data->data))
                    {
                        /* Value is greater, not defined in spec in this case, Retain */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = 127;
                    }
                    else
                    {
                        /* Value is lesser, Set to the next Lowest */
                        appl_mcs_players[mcs_sess_id].mcs_curr_track.playback_speed
                            = 64;
                    }
                    break;
                default:
                    /* This case should never be hit */
                    APPL_ERR("Unknown Current Playback Speed !\n");
                    break;
                }
            }

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }

            /* Copy the info to send notification */
            GA_COPY_BRR_DEVICE(&mcs_ntf_info.device, device);
            mcs_ntf_info.mcs_sess_id = mcs_sess_id;
            mcs_ntf_info.mcp_se_handle = evt_data->mcp_se_handle;
            /* Schedule a timer to send Notification for Current Track Position */
            BT_start_timer
            (
                &appl_mcp_se_send_ntf_curr_playback_speed_timer_handle,
                1U,
                appl_mcp_se_send_ntf_curr_playback_speed_timer_expiry_handler,
                &mcs_ntf_info,
                sizeof(mcs_ntf_info)
            );
        }
        else
        {
            APPL_ERR
            (
                "Parameter Length of Write Request (%d) Invalid !\n",
                evt_data->len
            );
        }
        break;

    case MCP_SE_WRITE_CURR_TRACK_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-WRITE\n");
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_WRITE_CURR_TRACK_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if ((APPL_OBJ_ID_LEN == evt_data->len) && (NULL != evt_data->data))
        {
            GA_mem_copy
            (
                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id.mcp_obj_id[0U],
                evt_data->data,
                APPL_OBJ_ID_LEN
            );
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Current Track Object ID: 0x", evt_data->len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }
        }
        else
        {
            APPL_ERR
            (
                "Object ID Length expected 6, but received %d or Object ID is NULL !\n",
                evt_data->len
            );
        }
        /* Do not send Notification in case where Client has changed the value */
        break;

    case MCP_SE_WRITE_NEXT_TRACK_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-WRITE\n");
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_WRITE_NEXT_TRACK_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Next Track Object ID (0x%04X)\n",
            GA_CHAR_MCS_NEXT_TRACK_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if ((APPL_OBJ_ID_LEN == evt_data->len) && (NULL != evt_data->data))
        {
            GA_mem_copy
            (
                &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id.mcp_obj_id[0U],
                evt_data->data,
                APPL_OBJ_ID_LEN
            );
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Next Track Object ID: 0x", evt_data->len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id.mcp_obj_id[index]
                );
            }
            APPL_TRC("\n");

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }
        }
        else
        {
            APPL_ERR
            (
                "Object ID Length expected 6, but received %d or Object ID is NULL !\n",
                evt_data->len
            );
        }
        /* Do not send Notification in case where Client has changed the value */
        break;

    case MCP_SE_WRITE_CURR_GRP_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-WRITE\n");
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_WRITE_CURR_GRP_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Current Group Object ID (0x%04X)\n",
            GA_CHAR_MCS_CURR_GROUP_OBJ_ID
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if ((APPL_OBJ_ID_LEN == evt_data->len) && (NULL != evt_data->data))
        {
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Current Group Object ID: 0x", evt_data->len);
            for (index = 0U; index < MCP_OBJ_ID_LEN; index++)
            {
                APPL_TRC
                (
                    "%02X",
                    ((UINT8*)evt_data->data)[index]
                );
            }
            APPL_TRC("\n");

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }

            appl_mcp_se_handle_write_curr_grp_id
            (
                device,
                mcs_sess_id,
                evt_data->mcp_se_handle,
                evt_data->data
            );
        }
        else
        {
            APPL_ERR
            (
                "Object ID Length expected 6, but received %d or Object ID is NULL !\n",
                evt_data->len
            );
        }
        /* Do not send Notification in case where Client has changed the value */
        break;

    case MCP_SE_WRITE_PLAYING_ORDER_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-WRITE\n");
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_WRITE_PLAYING_ORDER_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Playing Order (0x%04X)\n",
            GA_CHAR_MCS_PLAYING_ORDER
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if ((sizeof(MCP_CHAR_PARAM_TYPE_UINT8) == evt_data->len) && (NULL != evt_data))
        {
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Playing Order: ", evt_data->len);
            appl_mcp_utils_parse_and_display_playing_order
            (
                *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data
            );

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }

            /*
             * Validate if in our Supp features, we have this and
             * the Value received is a Valid Playing Order.
             */
            if (((MCP_PLAYING_ORDER_SINGLE_ONCE == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_SINGLE_ONCE ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_SINGLE_ONCE)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_REPEAT == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_SINGLE_REPEAT ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_SINGLE_REPEAT)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_IN_ORDER_ONCE == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_IN_ORDER_ONCE ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_IN_ORDER_ONCE)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_IN_ORDER_REPEAT == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_IN_ORDER_REPEAT ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_IN_ORDER_REPEAT)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_OLDEST_ONCE == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_OLDEST_ONCE ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_OLDEST_ONCE)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_OLDEST_REPEAT == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_OLDEST_REPEAT ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_OLDEST_REPEAT)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_NEWEST_ONCE == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_NEWEST_ONCE ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_NEWEST_ONCE)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_NEWEST_REPEAT == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_NEWEST_REPEAT ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_NEWEST_REPEAT)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_SHUFFLE_ONCE == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_SHUFFLE_ONCE ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_SHUFFLE_ONCE)))
                ||
                ((MCP_PLAYING_ORDER_SINGLE_SHUFFLE_REPEAT == *(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data) &&
                 (MCP_PLAYING_ORDER_SUPP_SHUFFLE_REPEAT ==
                  (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order_supp & MCP_PLAYING_ORDER_SUPP_SHUFFLE_REPEAT))))
            {
                /* It is a valid Playing Order, Set it */
                /*
                 * Check if the current value is the same as receive value,
                 * If Yes, Do not send Ntf.
                 */
                if (appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order !=
                        (*(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data))
                {
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.playing_order =
                        (*(MCP_CHAR_PARAM_TYPE_UINT8*)evt_data->data);
                    /*
                     * If the Playing Order characteristic supports notification,
                     * a server device shall notify this characteristic to the
                     * client device when the Playing Order characteristic value
                     * changes.
                     * As per MCS Spec V1.0, Page 23
                     */
                    /* Copy the info to send notification */
                    GA_COPY_BRR_DEVICE(&mcs_ntf_info.device, device);
                    mcs_ntf_info.mcs_sess_id = mcs_sess_id;
                    mcs_ntf_info.mcp_se_handle = evt_data->mcp_se_handle;
                    /* Schedule a timer to send Notification for Playing Order */
                    BT_start_timer
                    (
                        &appl_mcp_se_send_ntf_playing_order_timer_handle,
                        1U,
                        appl_mcp_se_send_ntf_playing_order_timer_expiry_handler,
                        &mcs_ntf_info,
                        sizeof(mcs_ntf_info)
                    );
                }
            }
            else
            {
                APPL_ERR("Playing Order is not supported !\n");
            }
        }
        break;

    case MCP_SE_WRITE_MCP_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-WRITE\n");
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_WRITE_MCP_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Media Control Point (0x%04X)\n",
            GA_CHAR_MCS_MEDIA_CONTROL_POINT
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();

        appl_mcp_handle_mcp_ind(device, evt_data, mcs_sess_id);

        if (ctx->to_rsp)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x00\n");
            APPL_TRC("Data: NULL\n");
        }
        break;

    case MCP_SE_WRITE_SCP_IND:
        APPL_TRC("[SUB-SECTION] : MCP SERVER-WRITE\n");
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_GMCS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_WRITE_SCP_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Search Control Point (0x%04X)\n",
            GA_CHAR_MCS_SEARCH_CONTROL_POINT
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();

        appl_mcp_handle_scp_ind(device, evt_data, mcs_sess_id);

        if (ctx->to_rsp)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x00\n");
            APPL_TRC("Data: NULL\n");
        }
        break;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    case MCP_SE_OTP_SE_READ_OTS_FEATURE_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OTS_FEATURE_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: OTS Feature (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OTS_FEATURE
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_mcp_se_otp_se_feature[0U];
        rsp_len = 8U;
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        GA_UNPACK_LE_4_BYTE(&oacp_feature, &((UINT8*)rsp)[0U]);
        APPL_TRC
        (
            "Len: 0x%02X,  OACP Features: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            oacp_feature
        );
        appl_mcp_par_and_display_oacp_feature(oacp_feature);
        GA_UNPACK_LE_4_BYTE(&olcp_feature, &((UINT8*)rsp)[4U]);
        APPL_TRC
        (
            "Len: 0x%02X,  OLCP Features: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            olcp_feature
        );
        appl_mcp_par_and_display_olcp_feature(olcp_feature);
        break;

    case MCP_SE_OTP_SE_READ_OBJ_NAME_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OBJ_NAME_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object Name (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OBJ_NAME
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_otp_se_curr_obj->obj_name[0U];
        rsp_len = appl_otp_se_curr_obj->obj_name_len;
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Object Name: ", rsp_len);
        for (index = 0U; index < rsp_len; index++)
        {
            APPL_TRC("%c", appl_otp_se_curr_obj->obj_name[index]);
        }
        APPL_TRC("\n");
        break;

    case MCP_SE_OTP_SE_READ_OBJ_TYPE_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OBJ_TYPE_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object Type (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OBJ_TYPE
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_otp_se_curr_obj->obj_type;
        rsp_len = sizeof(UINT16);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Object Type: ", rsp_len);
        appl_mcp_utils_parse_and_display_object_type(rsp);
        break;

    case MCP_SE_OTP_SE_READ_OBJ_SIZE_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OBJ_SIZE_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object Size (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OBJ_SIZE
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        GA_mem_copy
        (
            &obj_size[0U],
            &appl_otp_se_curr_obj->obj_curr_size[0U],
            OTP_OBJ_SIZE_LEN
        );
        GA_mem_copy
        (
            &obj_size[4U],
            &appl_otp_se_curr_obj->obj_alloc_size[0U],
            OTP_OBJ_SIZE_LEN
        );
        rsp = &obj_size[0U];
        rsp_len = 8U;
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        GA_UNPACK_LE_4_BYTE(&curr_size, &((UINT8*)rsp)[0U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Current Size: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            curr_size
        );
        GA_UNPACK_LE_4_BYTE(&alloc_size, &((UINT8*)rsp)[4U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Allocated Size: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            alloc_size
        );
        break;

    case MCP_SE_OTP_SE_READ_OBJ_FIRST_CREATED_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OBJ_FIRST_CREATED_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object First-Created (0x%04X)\n",
            GA_CHAR_OTS_CHAR_FIRST_CREATED
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_otp_se_curr_obj->obj_frst_crtd;
        rsp_len = OTP_OBJ_DATE_LEN;
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Object First-Created:\n", rsp_len);
        GA_UNPACK_LE_2_BYTE(&offset, &((UINT8*)rsp)[0U]);
        APPL_TRC("\t-Year: %d (0x%04X)\n", offset, offset);
        APPL_TRC("\t-Month: %d (0x%02X)\n", ((UINT8*)rsp)[2U], ((UINT8*)rsp)[2U]);
        APPL_TRC("\t-Day: %d (0x%02X)\n", ((UINT8*)rsp)[3U], ((UINT8*)rsp)[3U]);
        APPL_TRC("\t-Hours: %d (0x%02X)\n", ((UINT8*)rsp)[4U], ((UINT8*)rsp)[4U]);
        APPL_TRC("\t-Minutes: %d (0x%02X)\n", ((UINT8*)rsp)[5U], ((UINT8*)rsp)[5U]);
        APPL_TRC("\t-Seconds: %d (0x%02X)\n", ((UINT8*)rsp)[6U], ((UINT8*)rsp)[6U]);
        break;

    case MCP_SE_OTP_SE_READ_OBJ_LAST_MOD_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OBJ_LAST_MOD_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object Last-Modified (0x%04X)\n",
            GA_CHAR_OTS_CHAR_LAST_MOD
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_otp_se_curr_obj->obj_lst_mod;
        rsp_len = OTP_OBJ_DATE_LEN;
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Object First-Created:\n", rsp_len);
        GA_UNPACK_LE_2_BYTE(&offset, &((UINT8*)rsp)[0U]);
        APPL_TRC("\t-Year: %d (0x%04X)\n", offset, offset);
        APPL_TRC("\t-Month: %d (0x%02X)\n", ((UINT8*)rsp)[2U], ((UINT8*)rsp)[2U]);
        APPL_TRC("\t-Day: %d (0x%02X)\n", ((UINT8*)rsp)[3U], ((UINT8*)rsp)[3U]);
        APPL_TRC("\t-Hours: %d (0x%02X)\n", ((UINT8*)rsp)[4U], ((UINT8*)rsp)[4U]);
        APPL_TRC("\t-Minutes: %d (0x%02X)\n", ((UINT8*)rsp)[5U], ((UINT8*)rsp)[5U]);
        APPL_TRC("\t-Seconds: %d (0x%02X)\n", ((UINT8*)rsp)[6U], ((UINT8*)rsp)[6U]);
        break;

    case MCP_SE_OTP_SE_READ_OBJ_ID_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OBJ_ID_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object ID (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OBJ_ID
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_otp_se_curr_obj->obj_id[0U],
        rsp_len = OTP_OBJ_ID_LEN;
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Object ID: 0x", rsp_len);
        for (index = 0U; index < rsp_len; index++)
        {
            APPL_TRC("%02X", appl_otp_se_curr_obj->obj_id[index]);
        }
        APPL_TRC("\n");
        break;

    case MCP_SE_OTP_SE_READ_OBJ_PROP_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OBJ_PROP_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object Properties (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OBJ_PROP
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        rsp = &appl_otp_se_curr_obj->obj_prpty;
        rsp_len = sizeof(UINT32);
        APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        appl_mcp_utils_parse_and_display_obj_prop(rsp, rsp_len);
        break;

    case MCP_SE_OTP_SE_READ_OBJ_FILTER_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-READ\n");
        APPL_TRC("[TYPE]        : READ REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_READ_OBJ_FILTER_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object List Filter (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OBJ_LIST_FILTER
        );
        GA_FORMAT_DISPLAY();

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        /* TODO: */
        /* APPL_TRC("Data Length: 0x%04X\n", rsp_len);
        APPL_TRC("Data:\n");
        appl_mcp_utils_parse_and_display_list_filter(rsp, rsp_len); */
        break;

    case MCP_SE_OTP_SE_WRITE_OBJ_FILTER_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-WRITE\n");
        /* It is a Write Command, Do not send Response */
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_WRITE_OBJ_FILTER_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object List Filter (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OBJ_LIST_FILTER
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if ((0 == evt_data->len) || (NULL == evt_data->data))
        {
            APPL_ERR
            (
                "Filter Param Len is %d or Param is NULL !\n",
                evt_data->len
            );
        }
        else
        {
            /* TODO: */
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            appl_mcp_utils_parse_and_display_list_filter(evt_data->data, evt_data->len);

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }
        }
        break;

    case MCP_SE_OTP_SE_WRITE_OACP_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-WRITE\n");
        /* It is a Write Command, Do not send Response */
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_WRITE_OACP_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object Action Control Point (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OACP
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if ((1U <= evt_data->len) || (NULL != evt_data->data))
        {
            APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
            APPL_TRC("Data:\n");
            appl_mcp_utils_parse_and_display_oacp(evt_data->data);
            /* Parse the OACP request */
            appl_mcp_se_otp_se_parse_oacp_req(device, evt_data);

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }
        }
        else
        {
            APPL_ERR
            (
                "OACP Command Length is %d, or Param is NULL !\n",
                evt_data->len
            );
        }
        break;

    case MCP_SE_OTP_SE_WRITE_OLCP_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-WRITE\n");
        /* It is a Write Command, Do not send Response */
        if (!ctx->to_rsp)
        {
            APPL_TRC("[TYPE]        : WRITE COMMAND\n");
            send_rsp = GA_FALSE;
        }
        else
        {
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
        }
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_OTP_SE_WRITE_OLCP_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Object List Control Point (0x%04X)\n",
            GA_CHAR_OTS_CHAR_OLCP
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        if ((1U <= evt_data->len) || (NULL != evt_data->data))
        {
            appl_mcp_utils_parse_and_display_olcp(evt_data->data, evt_data->len);

            /* Parse the OACP request */
            appl_mcp_se_otp_se_set_curr_obj(evt_data);

            /* Post this return, trigger a call to send OLCP Rsp via Ind */
            SET_DEFAULT_OLCP_CNTRL_PNT_RSP();

            /* TODO:Below shd be based on req */
            SET_CNTRL_PNT_RSP_EVT_TYPE(MCP_OTS_OLCP_GO_TO_OPCODE);

            GA_COPY_BRR_DEVICE(&ots_cntrl_rsp.dev, device);
            ots_cntrl_rsp.mcp_se_handle = evt_data->mcp_se_handle;
            ots_cntrl_rsp.char_info = &otp_cp_rsp[0U];
            ots_cntrl_rsp.char_info_len = 3U;

            /* Schedule a timer to send the Control Point response */
            BT_start_timer
            (
                &ots_OLCP_send_ctrl_pnt_rsp_timer_handle,
                1U,
                ots_OLCP_send_ctrl_pnt_rsp_on_timer_expiry_handler,
                &ots_cntrl_rsp,
                sizeof(ots_cntrl_rsp)
            );

            if (ctx->to_rsp)
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");
            }
        }
        break;

    case MCP_SE_DATA_CHANNEL_UP_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_DATA_CHANNEL_UP_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        /* Save the lcid connected */
        lcid = *((UINT16*)evt_data->data);
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Channel ID: 0x%04X\n", evt_data->len, lcid);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x00\n");
        APPL_TRC("Data: NULL\n");
        break;

    case MCP_SE_DATA_CHANNEL_DOWN_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-RELEASE\n");
        APPL_TRC("[TYPE]        : RELEASE REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_DATA_CHANNEL_DOWN_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        lcid = *((UINT16*)evt_data->data);
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Channel ID: 0x%04X\n", evt_data->len, lcid);

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x00\n");
        APPL_TRC("Data: NULL\n");
        break;

    case MCP_SE_DATA_CHANNEL_WRITE_IND:
        APPL_TRC("[SUB-SECTION] : MCP - OTS SERVER-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP REQUEST\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_mcp_utils_display_ga_result
        (
            evt_status,
            GA_SERVICE_UUID_OTS
        );
        APPL_TRC
        (
            "[Event]       : MCP_SE_DATA_CHANNEL_WRITE_IND (0x%02X)\n",
            evt_id
        );
        APPL_TRC
        (
            "[Service Inst]: 0x%02X\n",
            evt_data->mcp_se_handle
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%04X\n", evt_data->len);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  No of Data Bytes Written: 0x%04X\n",
            evt_data->len,
            *((UINT16 *)evt_data->data)
        );

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("FORMING RESPONSE DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x00\n");
        APPL_TRC("Data: NULL\n");
        break;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    default:
        APPL_ERR ("Unknow Event: 0x%02X !\n", evt_id);
        break;
    }

    if (GA_TRUE == send_rsp)
    {
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MCP RESPONSE\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_mcp_se_send_rsp\n");
        retval = GA_mcp_se_send_rsp
                 (
                     device,
                     ctx,
                     evt_data->mcp_se_handle,
                     evt_id,
                     retval,
                     rsp,
                     rsp_len
                 );
        APPL_TRC("Retval: ");
        appl_mcp_utils_display_ga_result
        (
            retval,
            GA_SERVICE_GMCS
        );
    }

    GA_CB_EXIT();

    return GA_SUCCESS;
}

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
void ots_OACP_send_ctrl_pnt_rsp_on_timer_expiry_handler
     (
         void   * cntrl_point_rsp,
         UINT16   datalen
     )
{
    GA_IGNORE_UNUSED_PARAM(datalen);

    GA_RESULT retval;

    APPL_CNTRL_POINT_RSP_PARAM * ots_cntrl_point_rsp_param;

    ots_cntrl_point_rsp_param = (APPL_CNTRL_POINT_RSP_PARAM *)cntrl_point_rsp;

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC("SENDING GA MCP OTP INDICATION\n");
    GA_FORMAT_DISPLAY();

    APPL_TRC("GA_mcp_se_OTS_indicate_OACP_rsp\n");
    retval = GA_mcp_se_OTS_indicate_OACP_rsp
             (
                 &ots_cntrl_point_rsp_param->dev,
                 ots_cntrl_point_rsp_param->mcp_se_handle,
                 (void *)ots_cntrl_point_rsp_param->char_info,
                 ots_cntrl_point_rsp_param->char_info_len
             );

    APPL_TRC("Retval: ");
    appl_mcp_utils_display_ga_result
    (
        retval,
        GA_SERVICE_UUID_OTS
    );

    if (MCP_OTS_OACP_READ_OPCODE == ots_cntrl_point_rsp_param->char_info[1U])
    {
        /* Trigger sending of data here */
        retval = GA_mcp_se_data_channel_write
                 (
                     &ots_cntrl_point_rsp_param->dev,
                     lcid,
                     appl_otp_se_curr_obj->obj_data,
                     appl_otp_se_curr_obj->obj_datalen
                 );
    }
}

void ots_OLCP_send_ctrl_pnt_rsp_on_timer_expiry_handler
     (
         void   * cntrl_point_rsp,
         UINT16   datalen
     )
{
    GA_IGNORE_UNUSED_PARAM(datalen);

    GA_RESULT retval;

    APPL_CNTRL_POINT_RSP_PARAM * ots_cntrl_point_rsp_param;

    ots_cntrl_point_rsp_param = (APPL_CNTRL_POINT_RSP_PARAM *)cntrl_point_rsp;

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC("SENDING GA MCP OTP INDICATION\n");
    GA_FORMAT_DISPLAY();

    APPL_TRC("GA_mcp_se_OTS_indicate_OLCP_rsp\n");
    retval = GA_mcp_se_OTS_indicate_OLCP_rsp
             (
                 &ots_cntrl_point_rsp_param->dev,
                 ots_cntrl_point_rsp_param->mcp_se_handle,
                 (void *)ots_cntrl_point_rsp_param->char_info,
                 ots_cntrl_point_rsp_param->char_info_len
             );

    APPL_TRC("Retval: ");
    appl_mcp_utils_display_ga_result
    (
        retval,
        GA_SERVICE_UUID_OTS
    );
}

void appl_mcp_se_otp_se_parse_oacp_req(GA_ENDPOINT * device, MCP_SE_EVT * evt)
{
    UCHAR * param = evt->data;
    APPL_CNTRL_POINT_RSP_PARAM ots_oacp_cntrl_rsp;
    UINT32 checksum = 0U;

    GA_COPY_BRR_DEVICE(&ots_oacp_cntrl_rsp.dev, device);
    ots_oacp_cntrl_rsp.mcp_se_handle = evt->mcp_se_handle;
    ots_oacp_cntrl_rsp.char_info = &otp_cp_rsp[0U];

    switch (param[0U])
    {
    case MCP_OTS_OACP_CAL_CHECK_SUM_OPCODE:
        SET_CNTRL_PNT_RSP_EVT_TYPE(MCP_OTS_OACP_CAL_CHECK_SUM_OPCODE);
        cry_perform_crc32_op
        (
            &appl_otp_se_curr_obj->obj_data[0U],
            appl_otp_se_curr_obj->obj_datalen,
            &checksum
        );
        /* Increment the length parameter and copy the checksum */
        GA_PACK_LE_4_BYTE_VAL(&otp_cp_rsp[3U], checksum);
        ots_oacp_cntrl_rsp.char_info_len = 3U;
        ots_oacp_cntrl_rsp.char_info_len += sizeof(UINT32);

#if 0
        appl_mcp_se_otp_se_calc_checksum_proc_handler
        (
            &param[1],
            evt->len - 1U,
            &ots_oacp_cntrl_rsp
        );
#endif
        break;
    case MCP_OTS_OACP_READ_OPCODE:
#if 0
        appl_mcp_se_otp_se_OACP_read_proc_handler
        (
            &param[1],
            evt->len - 1U
        );
#endif
        /* TODO:Below shd be based on req */
        SET_CNTRL_PNT_RSP_EVT_TYPE(MCP_OTS_OACP_READ_OPCODE);
        ots_oacp_cntrl_rsp.char_info_len = 3U;
        break;
    default:
        /* Unknown Opcode */
        otp_cp_rsp[2U] = MCP_OTS_OACP_OPCODE_NOT_SUPPORTED;
        break;
    }

    /* Post this return, trigger a call to send OACP Rsp via Ind */
    SET_DEFAULT_OACP_CNTRL_PNT_RSP();

    /* Schedule a timer to send the Control Point response */
    BT_start_timer
    (
        &ots_OACP_send_ctrl_pnt_rsp_timer_handle,
        1U,
        ots_OACP_send_ctrl_pnt_rsp_on_timer_expiry_handler,
        &ots_oacp_cntrl_rsp,
        sizeof(ots_oacp_cntrl_rsp)
    );
}

API_RESULT appl_mcp_se_otp_se_OACP_read_proc_handler
           (
               UINT8  * param,
               UINT16   param_len
           )
{
    UINT32 offset = 0U;
    UINT32 length = 0U;
    UINT32 obj_curr_size;

    GA_IGNORE_UNUSED_PARAM(param_len);

    /* Is this Opcode supported by the App */
    if (!IS_OACP_READ_OP_SUPP(appl_otp_se_curr_obj->obj_prpty))
    {
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_OPCODE_NOT_SUPPORTED);
        return API_SUCCESS;
    }

    /* Extract offset */
    GA_UNPACK_LE_4_BYTE(&offset, param);

    param = param + 4U;

    /* Extract Length */
    GA_UNPACK_LE_4_BYTE(&length, param);

    /* If there is no valid current object, return below error */
    /* GET_CURR_OBJ_REG_ID(ots_app_index, curr_obj_reg_id); */

    /* If there is no valid current object, return below error */
    obj_curr_size = MCP_OBJ_ID_LEN;
#if 0
    if (!IS_CURR_OBJ_VALID(ots_app_index))
    {
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_INVALID_OBJECT);
    }
    else if (!IS_OBJ_PROP_READ_SET(ots_app_index, curr_obj_index))
    {
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_PROCEDURE_NOT_PERMITTED);
    }
    else if (!IS_OTS_TRANSPORT_CONN(ots_app_index))
    {
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_CHANNEL_UNAVAILABLE);
    }
    else if (offset > obj_curr_size)
    {
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_INVALID_PARAMETER);
    }
    else if ((offset + length) > obj_curr_size)
    {
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_INVALID_PARAMETER);
    }
    else if (0U)
    {
        //TODO length CBFC size
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_INSUFF_RESOURCES);
    }
    else if ((IS_OBJ_LOCKED(ots_app_index))
             ||
             (IS_OBJ_TRANSFER_ONGOING(ots_app_index)))
    {
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_OBJECT_LOCKED);
    }
    else if (8U != param_len)
    {
        SET_CNTRL_PNT_RSP_RESULT_CODE(MCP_OTS_OACP_OPERATION_FAILED);
    }
    else
    {

    }
#endif

    return API_SUCCESS;
}

void appl_mcp_se_otp_se_set_curr_obj(MCP_SE_EVT * evt)
{
    UCHAR * obj_id = (UCHAR*)evt->data;

    /* Obj Id + Opc : Length */
    if (((MCP_OBJ_ID_LEN + 1U) != evt->len) || (NULL == obj_id))
    {
        return;
    }

    if (0U == GA_mem_cmp
              (
                  &appl_mcp_se_icon_obj_mpc.obj_id[0U],
                  &obj_id[1U],
                  MCP_OBJ_ID_LEN
              ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_icon_obj_mpc;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_icon_obj_vlc.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_icon_obj_vlc;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_track_obj_1.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_track_obj_1;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_track_obj_2.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_track_obj_2;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_track_obj_3.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_track_obj_3;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_search_res_obj.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_search_res_obj;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_track_segment_obj.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_track_segment_obj;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_parent_grp_obj.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_parent_grp_obj;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_grp_obj_1.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_grp_obj_1;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_grp_obj_2.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_grp_obj_2;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcp_se_grp_obj_3.obj_id[0U],
                       &obj_id[1U],
                       MCP_OBJ_ID_LEN
                   ))
    {
        appl_otp_se_curr_obj = &appl_mcp_se_grp_obj_3;
    }
    else
    {
        /* Unknown Object ID */
        return;
    }
}

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

GA_RESULT appl_mcp_handle_mcp_ind
          (
              GA_ENDPOINT * dev,
              MCP_SE_EVT  * evt,
              UINT8         mcs_sess_id
          )
{
    MCP_CP_REQ_OPC * opc = (MCP_CP_REQ_OPC *)evt->data;
    INT32 offset;
    UINT8 currTrack;
    UINT8 currGrp;
    APPL_CNTRL_POINT_RSP_PARAM mcp_cntrl_rsp;
    APPL_MCP_NTF_INFO mcs_ntf_info;
    UINT16 mcp_ntf_flag;

    offset = 0U;
    currTrack = 0U;
    currGrp = 0U;

    /* By default, MCP Response Code is set to Success */
    cp_rsp[0] = opc[0U];
    cp_rsp[1] = MCP_CP_RES_CODE_SUCCESS;

    /* By default, Set to send Ntf - CP Response */
    GA_COPY_BRR_DEVICE(&mcp_cntrl_rsp.dev, dev);
    mcp_cntrl_rsp.mcp_se_handle = evt->mcp_se_handle;
    mcp_cntrl_rsp.char_info = &cp_rsp[0U];
    mcp_cntrl_rsp.char_info_len = 2U;

    /* Set to Send Ntf - Others, if any */
    GA_COPY_BRR_DEVICE(&mcs_ntf_info.device, dev);
    mcs_ntf_info.mcs_sess_id = mcs_sess_id;
    mcs_ntf_info.mcp_se_handle = evt->mcp_se_handle;
    mcp_ntf_flag = 0xFFFFU;

    if (0U == GA_mem_cmp
             (
                 &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                 &appl_mcp_se_track_obj_1.obj_id[0U],
                 APPL_OBJ_ID_LEN
             ))
    {
        currTrack = 1U;
    }
    else if (0 == GA_mem_cmp
                  (
                      &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                      &appl_mcp_se_track_obj_2.obj_id[0U],
                      APPL_OBJ_ID_LEN
                  ))
    {
        currTrack = 2;
    }
    else if (0U == GA_mem_cmp
                  (
                      &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                      &appl_mcp_se_track_obj_2.obj_id[0U],
                      APPL_OBJ_ID_LEN
                  ))
    {
        currTrack = 3U;
    }
    else
    {
        currTrack = 0U;
    }

    /* Current Group */
    if (0U == GA_mem_cmp
             (
                 &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                 &appl_mcp_se_grp_obj_1.obj_id[0U],
                 APPL_OBJ_ID_LEN
             ))
    {
        currGrp = 1;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                       &appl_mcp_se_grp_obj_2.obj_id[0U],
                       APPL_OBJ_ID_LEN
                   ))
    {
        currGrp = 2U;
    }
    else if (0U == GA_mem_cmp
                   (
                       &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                       &appl_mcp_se_grp_obj_3.obj_id[0U],
                       APPL_OBJ_ID_LEN
                   ))
    {
        currGrp = 3U;
    }
    else
    {
        currGrp = 0U;
    }

    if (MCP_MEDIA_STATE_INACTIVE == appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
    {
        cp_rsp[1U] = MCP_CP_RES_CODE_MP_INACTIVE;
        APPL_ERR("Media State is Inactive !\n");
    }
    else
    {
        APPL_TRC("Data Length: 0x%04X\n", evt->len);
        APPL_TRC("Data:\n");
        APPL_TRC("Len: 0x%02X,  Opcode: ", (UCHAR)sizeof(opc[0U]));
        switch (opc[0U])
        {
        case MCP_CP_OPCODE_PLAY:
            APPL_TRC("Play (0x%02X)\n", opc[0U]);
            if ((appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state ==
                MCP_MEDIA_STATE_PAUSED)
                ||
                (appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state ==
                    MCP_MEDIA_STATE_SEEKING))
            {
                appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                    MCP_MEDIA_STATE_PLAYING;

                mcp_ntf_flag = APPL_MCP_SE_NTF_MEDIA_STATE;
            }
            break;
        case MCP_CP_OPCODE_PAUSE:
            APPL_TRC("Pause (0x%02X)\n", opc[0U]);
            if ((appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state ==
                MCP_MEDIA_STATE_PLAYING)
                ||
                (appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state ==
                    MCP_MEDIA_STATE_SEEKING))
            {
                appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                    MCP_MEDIA_STATE_PAUSED;
                /*
                 * TODO: If the Media State characteristic value is Seeking,
                 * the media player shall stop seeking, set the current track
                 * and track position as a result of seeking.
                 * So, need to notify Current Track Object ID and
                 * Track Position.
                 */

                mcp_ntf_flag = APPL_MCP_SE_NTF_MEDIA_STATE;
            }
            break;
        case MCP_CP_OPCODE_STOP:
            APPL_TRC("Stop (0x%02X)\n", opc[0U]);
            {
                /* Set the Track Position to beginning of track - 0 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos = 0;
                /* Track Position is getting updated, send notification */
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;

                /*
                 * Validate current Media State. For Paused Media state,
                 * there is no updation of media state. So, no need to notify
                 * media state.
                 */
                if (appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state !=
                    MCP_MEDIA_STATE_PAUSED)
                {
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                        MCP_MEDIA_STATE_PAUSED;
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_MEDIA_STATE;
                }
            }
            break;
        case MCP_CP_OPCODE_FAST_RWD: /* Fall - through */
        case MCP_CP_OPCODE_FAST_FWD:
            if (appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state !=
                MCP_MEDIA_STATE_SEEKING)
            {
                appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                    MCP_MEDIA_STATE_SEEKING;
                mcp_ntf_flag = APPL_MCP_SE_NTF_MEDIA_STATE;
            }
            if (MCP_CP_OPCODE_FAST_RWD == opc[0U])
            {
                APPL_TRC("Fast Rewind (0x%02X)\n", opc[0U]);
                appl_mcs_players[mcs_sess_id].mcs_curr_track.seeking_speed = -1;
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos--;
            }
            else
            {
                APPL_TRC("Fast Forward (0x%02X)\n", opc[0U]);
                appl_mcs_players[mcs_sess_id].mcs_curr_track.seeking_speed = 1;
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos++;
            }
            /*
             * TODO: Seeking speed should get updated when media state is seeking.
             * So, need to notify seeking speed when media state is seeking.
             */
            if (0xFFFFU != mcp_ntf_flag)
            {
                mcp_ntf_flag |= APPL_MCP_SE_NTF_SEEKING_SPEED;
            }
            else
            {
                mcp_ntf_flag = APPL_MCP_SE_NTF_SEEKING_SPEED;
            }
            break;
        case MCP_CP_OPCODE_MOVE_RELATIVE:
            APPL_TRC("Move Relative (0x%02X)\n", opc[0U]);
            if ((appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state ==
                MCP_MEDIA_STATE_PLAYING)
                ||
                (appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state ==
                    MCP_MEDIA_STATE_PAUSED)
                ||
                (appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state ==
                    MCP_MEDIA_STATE_SEEKING)
                )
            {
                GA_UNPACK_LE_4_BYTE(&offset, &opc[1U]);
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos += offset;
                if (appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos < APPL_MCP_SE_TRACK_POS_MIN)
                {
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos = APPL_MCP_SE_TRACK_POS_MIN;
                }
                else if (appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos > APPL_MCP_SE_TRACK_POS_MAX)
                {
                    appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos = APPL_MCP_SE_TRACK_POS_MAX;
                }
                else
                {
                    /* Do nothing */
                }

                APPL_TRC
                (
                    "Len: 0x%02X,  offset: 0x%08X\n",
                    (UCHAR)sizeof(MCP_CHAR_PARAM_TYPE_INT32),
                    offset
                );
                /* Track position is updated, send notification */
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;
            }
            else
            {
                /* No action */
            }
            break;
        case MCP_CP_OPCODE_FIRST_SEG:
            APPL_TRC("First Segment (0x%02X)\n", opc[0U]);
            /*
             * Current track pos shall be set to the starting position of the
             * first segment of this track.
             */
            appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                APPL_MCP_SE_SEG_POS_FIRST;

            mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;

            /* State Management in this case is left to implementation */
            /*
             * Sending notification for Media State is mentioned as
             * optional("may send") in the MCS Test Spec.
             * So, media state is not notified to the client for now.
             */
            break;
        case MCP_CP_OPCODE_LAST_SEG:
            APPL_TRC("Last Segment (0x%02X)\n", opc[0U]);
            /*
             * Current track pos shall be set to the starting position of the
             * last segment of this track.
             */
             appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                 APPL_MCP_SE_SEG_POS_FOURTH;
            mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;

            /* State Management in this case is left to implementation */
            /*
             * Sending notification for Media State is mentioned as
             * optional("may send") in the MCS Test Spec.
             * So, media state is not notified to the client for now.
             */
            break;
        case MCP_CP_OPCODE_PREV_SEG:
            APPL_TRC("Previous Segment (0x%02X)\n", opc[0U]);
            /*
             * Current track pos shall be set to the starting position of the
             * previous segment of this track.
             */
            appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                APPL_MCP_SE_SEG_POS_FIRST;
            mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;

            /* State Management in this case is left to implementation */
            /*
             * Sending notification for Media State is mentioned as
             * optional("may send") in the MCS Test Spec.
             * So, media state is not notified to the client for now.
             */
            break;
        case MCP_CP_OPCODE_NEXT_SEG:
            APPL_TRC("Next Segment (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * next segment of this track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    APPL_MCP_SE_SEG_POS_SECOND;

                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;

                /* State Management in this case is left to implementation */
                /*
                 * Sending notification for Media State is mentioned as
                 * optional("may send") in the MCS Test Spec.
                 * So, media state is not notified to the client for now.
                 */
            }
            break;
        case MCP_CP_OPCODE_GOTO_SEG:
            APPL_TRC("Goto Segment (0x%02X)\n", opc[0U]);
            {
                GA_UNPACK_LE_4_BYTE(&offset, &opc[1U]);
                APPL_TRC
                (
                    "Len: 0x%02X,  n: 0x%08X\n",
                    (UCHAR)sizeof(MCP_CHAR_PARAM_TYPE_INT32),
                    offset
                );
                /*
                 * Current track pos shall be set to the starting position of the
                 * nth segment.
                 */

                 /* Positive */
                if (offset > 0)
                {
                    /*
                     * If the value n is a positive number, setting the current
                     * track position is equivalent to sending the
                     * First Segment opcode once, followed by the Next Segment
                     * opcode n-1 times.
                     */
                    switch (offset)
                    {
                        case 1:
                            appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                                APPL_MCP_SE_SEG_POS_FIRST;
                            break;
                        case 2:
                            appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                                APPL_MCP_SE_SEG_POS_SECOND;
                            break;
                        case 3:
                            appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                                APPL_MCP_SE_SEG_POS_THIRD;
                            break;
                        case 4:
                            appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                                APPL_MCP_SE_SEG_POS_FOURTH;
                            break;
                        default:
                            /* Not a valid Segment. No action */
                            break;
                    }
                }
                else if (offset < 0)
                {
                    /*
                     * If the value n is a negative number setting, the current
                     * track position is equivalent to sending the Last Segment
                     * opcode once, followed by the Previous Segment opcode
                     * n-1 times.
                     */
                    if (-1 == offset)
                    {
                           appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                               APPL_MCP_SE_SEG_POS_FOURTH;
                    }
                    else if (-2 == offset)
                    {
                           appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                               APPL_MCP_SE_SEG_POS_THIRD;
                    }
                    else if (-3 == offset)
                    {
                           appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                               APPL_MCP_SE_SEG_POS_SECOND;
                    }
                    else if (-4 == offset)
                    {
                           appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                               APPL_MCP_SE_SEG_POS_FIRST;
                    }
                    else
                    {
                        /* Not a valid Segment. No action */
                    }
                }
                else
                {
                    /*
                     * If the value of n is zero, the current track
                     * position shall not change
                     */
                }

                if (MCP_MEDIA_STATE_SEEKING ==
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
                {
                    /* State Management in this case is left to implementation */
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                        MCP_MEDIA_STATE_PAUSED;
                    /*
                     * Sending notification for Media State is not mentioned as
                     * mandatory/optional in the MCS Test Spec.
                     * So, after media state value update, its not notified to
                     * the client for now.
                     */
                }

                /*
                 * The IUT does not send a notification of the
                 * Track Position characteristic.
                 * As per MCS.TS.p2, Page: 57, Section: 4.8.3(Goto Segment Zero)
                 */
                if (0 != offset)
                {
                    mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;
                }
            }
            break;

        case MCP_CP_OPCODE_PREV_TRACK:
            APPL_TRC("Previous Track (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * previous track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                if (currTrack == 3U)
                {
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                        &appl_mcp_se_track_obj_2.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                        &appl_mcp_se_track_obj_3.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                }
                else if (currTrack == 2U)
                {
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                        &appl_mcp_se_track_obj_1.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                        &appl_mcp_se_track_obj_2.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                }
                else if (currTrack == 1U)
                {
                    /* Just remain in the current track */
                }
                else
                {
                    /* No Track */
                }
                if (MCP_MEDIA_STATE_SEEKING ==
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
                {
                    /* State Management in this case is left to implementation */
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                        MCP_MEDIA_STATE_PAUSED;
                    /*
                     * Sending notification for Media State is mentioned as
                     * optional("may send") in the MCS Test Spec.
                     * So, after media state value update, its not notified to
                     * the client for now.
                     */
                }
                /*
                 * Sending notification for Track Changed is mentioned as
                 * optional("may send") in the MCS Test Spec.
                 * So, after current and next track value update, track changed
                 * char is not notified to the client for now.
                 */
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_TITLE;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_NEXT_TRACK_OBJ_ID;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_POS;
            }
            break;
        case MCP_CP_OPCODE_NEXT_TRACK:
            APPL_TRC("Next Track (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * next track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                if (currTrack == 3U)
                {
                    /* Just remain in the current track */
                }
                else if (currTrack == 2U)
                {
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                        &appl_mcp_se_track_obj_3.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                        &appl_mcp_se_track_obj_1.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                }
                else if (currTrack == 1U)
                {
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                        &appl_mcp_se_track_obj_2.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                        &appl_mcp_se_track_obj_3.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                }
                else
                {
                    /* No Track */
                }

                if (MCP_MEDIA_STATE_SEEKING ==
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
                {
                    /* State Management in this case is left to implementation */
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                        MCP_MEDIA_STATE_PAUSED;
                    /*
                     * Sending notification for Media State is mentioned as
                     * optional("may send") in the MCS Test Spec.
                     * So, after media state value update, its not notified to
                     * the client for now.
                     */
                }
                /*
                 * Sending notification for Track Changed is mentioned as
                 * optional("may send") in the MCS Test Spec.
                 * So, after current and next track value update, track changed
                 * char is not notified to the client for now.
                 */
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_TITLE;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_NEXT_TRACK_OBJ_ID;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_POS;
            }
            break;
        case MCP_CP_OPCODE_LAST_TRACK:
            APPL_TRC("Last Track (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * last track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_3.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                    &appl_mcp_se_track_obj_1.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );
                /*
                 * Sending notification for Media State is mentioned as
                 * optional("may send") in the MCS Test Spec.
                 * So, media state is not notified to the client for now.
                 */
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_CHANGED;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_TITLE;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_POS;
            }
            break;

        case MCP_CP_OPCODE_FIRST_TRACK:
            APPL_TRC("First Track (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * first track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_1.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                    &appl_mcp_se_track_obj_2.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );
            }
            /*
             * Sending notification for Media State is mentioned as
             * optional("may send") in the MCS Test Spec.
             * So, media state is not notified to the client for now.
             */
            mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_CHANGED;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_TITLE;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_POS;
            break;

        case MCP_CP_OPCODE_GOTO_TRACK:
            APPL_TRC("Goto Track (0x%02X)\n", opc[0U]);
            {
                GA_UNPACK_LE_4_BYTE(&offset, &opc[1U]);
                APPL_TRC
                (
                    "Len: 0x%02X,  n: 0x%08X\n",
                    (UCHAR)sizeof(MCP_CHAR_PARAM_TYPE_INT32),
                    offset
                );
                /*
                 * Current track pos shall be set to the starting position of the
                 * nth track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                /* Positive */
                if (offset > 0)
                {
                    /*
                     * If the value n is a positive number, setting the current
                     * track is equivalent to sending the First Track opcode
                     * once, followed by the Next Track opcode n-1 times.
                     */
                    switch (offset)
                    {
                        case 1:
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                                &appl_mcp_se_track_obj_1.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                                &appl_mcp_se_track_obj_2.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            break;
                        case 2:
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                                &appl_mcp_se_track_obj_2.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                                &appl_mcp_se_track_obj_3.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            break;
                        case 3:
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                                &appl_mcp_se_track_obj_3.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                                &appl_mcp_se_track_obj_1.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            break;
                        default:
                            /* Not a valid Track. No action */
                            break;
                    }
                }
                else if (offset < 0)
                {
                    /*
                     * If the value n is a negative number, then setting the
                     * current track is equivalent to sending the Last Track
                     * opcode once, followed by the Previous Track opcode
                     * n-1 times.
                     */
                    if (-1 == offset)
                    {
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                            &appl_mcp_se_track_obj_3.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                            &appl_mcp_se_track_obj_1.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                    }
                    else if (-2 == offset)
                    {
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                            &appl_mcp_se_track_obj_2.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                            &appl_mcp_se_track_obj_3.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                    }
                    else if (-3 == offset)
                    {
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                            &appl_mcp_se_track_obj_1.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                            &appl_mcp_se_track_obj_2.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                    }
                    else
                    {
                        /* Not a valid Track. No action */
                    }
                }
                else
                {
                    /*
                     * If the value n is zero, the current track
                     * shall not be changed
                     */
                }

                if (MCP_MEDIA_STATE_SEEKING ==
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
                {
                    /* State Management in this case is left to implementation */
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                        MCP_MEDIA_STATE_PAUSED;
                    /*
                     * Sending notification for Media State is mentioned as
                     * optional("may send") in the MCS Test Spec.
                     * So, after media state value update, its not notified to
                     * the client for now.
                     */
                }
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;
                /*
                 * The IUT does not send a notification of the
                 * Track Changed characteristic.
                 * As per MCS.TS.p2, Page: 58, Section: 4.8.4(Goto Track Zero)
                 */
                if (0 != offset)
                {
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_CHANGED;
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_TITLE;
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
                }
            }
            break;

        case MCP_CP_OPCODE_PREV_GROUP:
            APPL_TRC("Previous Group (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * first track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_1.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                    &appl_mcp_se_track_obj_2.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.parent_grp_obj_id,
                    &appl_mcp_se_parent_grp_obj.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );

                if (currGrp == 3U)
                {
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                        &appl_mcp_se_grp_obj_2.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                }
                else if (currGrp == 2U)
                {
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                        &appl_mcp_se_grp_obj_1.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                }
                else if (currGrp == 1U)
                {
                    /* Just remain in the current Group */
                }
                else
                {
                    /* No Group */
                }

                if (MCP_MEDIA_STATE_SEEKING ==
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
                {
                    /* State Management in this case is left to implementation */
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                        MCP_MEDIA_STATE_PAUSED;
                    /*
                     * Sending notification for Media State is mentioned as
                     * optional("may send") in the MCS Test Spec.
                     * So, after media state value update, its not notified to
                     * the client for now.
                     */
                }
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_CHANGED;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_TITLE;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_POS;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_GRP_OBJ_ID;
            }
            break;

        case MCP_CP_OPCODE_NEXT_GROUP:
            APPL_TRC("Next Group (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * first track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_1.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );
                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id,
                    &appl_mcp_se_track_obj_2.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.parent_grp_obj_id,
                    &appl_mcp_se_parent_grp_obj.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );

                if (currGrp == 1U)
                {
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                        &appl_mcp_se_grp_obj_2.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                }
                else if (currGrp == 2U)
                {
                    GA_mem_copy
                    (
                        &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                        &appl_mcp_se_grp_obj_3.obj_id[0U],
                        APPL_OBJ_ID_LEN
                    );
                }
                else if (currGrp == 3U)
                {
                    /* Just remain in the current Group */
                }
                else
                {
                    /* No Group */
                }

                if (MCP_MEDIA_STATE_SEEKING ==
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
                {
                    /* State Management in this case is left to implementation */
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                        MCP_MEDIA_STATE_PAUSED;
                    /*
                     * Sending notification for Media State is mentioned as
                     * optional("may send") in the MCS Test Spec.
                     * So, after media state value update, its not notified to
                     * the client for now.
                     */
                }
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_CHANGED;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_TITLE;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_POS;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
                mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_GRP_OBJ_ID;
            }
            break;

        case MCP_CP_OPCODE_FIRST_GROUP:
            APPL_TRC("First Group (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * first track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_1.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                    &appl_mcp_se_grp_obj_1.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );
            }
            if (MCP_MEDIA_STATE_SEEKING ==
                appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
            {
                /* State Management in this case is left to implementation */
                appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                    MCP_MEDIA_STATE_PAUSED;
                /*
                 * Sending notification for Media State is mentioned as
                 * optional("may send") in the MCS Test Spec.
                 * So, after media state value update, its not notified to
                 * the client for now.
                 */
            }
            /*
             * TODO: Parent Group Object ID will get updated?
             * Next Track Object ID will get updated?
             */
            mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_CHANGED;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_TITLE;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_POS;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_GRP_OBJ_ID;
            break;

        case MCP_CP_OPCODE_LAST_GROUP:
            APPL_TRC("Last Group (0x%02X)\n", opc[0U]);
            {
                /*
                 * Current track pos shall be set to the starting position of the
                 * first track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                    &appl_mcp_se_track_obj_1.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );

                GA_mem_copy
                (
                    &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                    &appl_mcp_se_grp_obj_3.obj_id[0U],
                    APPL_OBJ_ID_LEN
                );
            }
            if (MCP_MEDIA_STATE_SEEKING ==
                appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
            {
                /* State Management in this case is left to implementation */
                appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                    MCP_MEDIA_STATE_PAUSED;
                /*
                 * Sending notification for Media State is mentioned as
                 * optional("may send") in the MCS Test Spec.
                 * So, after media state value update, its not notified to
                 * the client for now.
                 */
            }
            /*
             * TODO: Parent Group Object ID will get updated?
             * Next Track Object ID will get updated?
             */
            mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_CHANGED;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_TITLE;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_POS;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
            mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_GRP_OBJ_ID;
            break;

        case MCP_CP_OPCODE_GOTO_GROUP:
            APPL_TRC("Goto Group (0x%02X)\n", opc[0U]);
            {
                GA_UNPACK_LE_4_BYTE(&offset, &opc[1U]);
                APPL_TRC
                (
                    "Len: 0x%02X,  n: 0x%08X\n",
                    (UCHAR)sizeof(MCP_CHAR_PARAM_TYPE_INT32),
                    offset
                );
                /*
                 * Current track pos shall be set to the starting position of the
                 * first track.
                 */
                appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos =
                    0;

                /* Positive */
                if (offset > 0)
                {
                    /*
                     * If the value n is a positive number, setting the
                     * current group is equivalent to sending the First Group
                     * opcode once, followed by the Next Group opcode
                     * n-1 times.
                     */
                    switch (offset)
                    {
                        case 1:
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                                &appl_mcp_se_grp_obj_1.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            /*
                             * The current track shall be set to the first track of the
                             * current group resulting from the Goto Group command.
                             */
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                                &appl_mcp_se_track_obj_1.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            break;
                        case 2:
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                                &appl_mcp_se_grp_obj_2.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            /*
                             * The current track shall be set to the first track of the
                             * current group resulting from the Goto Group command.
                             */
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                                &appl_mcp_se_track_obj_1.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            break;
                        case 3:
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                                &appl_mcp_se_grp_obj_3.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            /*
                             * The current track shall be set to the first track of the
                             * current group resulting from the Goto Group command.
                             */
                            GA_mem_copy
                            (
                                &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                                &appl_mcp_se_track_obj_1.obj_id[0U],
                                APPL_OBJ_ID_LEN
                            );
                            break;
                        default:
                            /* Not a valid Group. No action */
                            break;
                    }
                }
                else if (offset < 0)
                {
                    /*
                     * If the value n is a negative number, setting the current
                     * group is equivalent to sending the Last Group opcode
                     * once, followed by the Previous Group opcode n-1 times.
                     */
                    if (-1 == offset)
                    {
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                            &appl_mcp_se_grp_obj_3.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                        /*
                         * The current track shall be set to the first track of the
                         * current group resulting from the Goto Group command.
                         */
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                            &appl_mcp_se_track_obj_1.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                    }
                    else if (-2 == offset)
                    {
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                            &appl_mcp_se_grp_obj_2.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                        /*
                         * The current track shall be set to the first track of the
                         * current group resulting from the Goto Group command.
                         */
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                            &appl_mcp_se_track_obj_1.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                    }
                    else if (-3 == offset)
                    {
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                            &appl_mcp_se_grp_obj_1.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                        /*
                         * The current track shall be set to the first track of the
                         * current group resulting from the Goto Group command.
                         */
                        GA_mem_copy
                        (
                            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                            &appl_mcp_se_track_obj_1.obj_id[0U],
                            APPL_OBJ_ID_LEN
                        );
                    }
                    else
                    {
                        /* Not a valid Group. No action */
                    }
                }
                else
                {
                    /*
                     * If the value of n is zero, the current group
                     * shall not be changed
                     */
                }

                if (MCP_MEDIA_STATE_SEEKING ==
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state)
                {
                    /* State Management in this case is left to implementation */
                    appl_mcs_players[mcs_sess_id].mcs_mp_params.media_state =
                        MCP_MEDIA_STATE_PAUSED;
                    /*
                     * Sending notification for Media State is mentioned as
                     * optional("may send") in the MCS Test Spec.
                     * So, after media state value update, its not notified to
                     * the client for now.
                     */
                }
                /*
                 * TODO: Parent Group Object ID will get updated?
                 * Next Track Object ID will get updated?
                 */
                mcp_ntf_flag = APPL_MCP_SE_NTF_TRACK_POS;
                /*
                 * If Current Group Object ID notification is supported,
                 * the IUT does not send a notification of the
                 * Current Group Object ID.
                 * As per MCS.TS.p2, Page: 59, Section: 4.8.5(Goto Group Zero)
                 */
                if (0 != offset)
                {
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_CHANGED;
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_TITLE;
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_TRACK_DURATION;
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID;
                    mcp_ntf_flag |= APPL_MCP_SE_NTF_CURR_GRP_OBJ_ID;
                }
            }
            break;

        default:
            /* Opcode Unknown */
            APPL_ERR("Opcode Unknown (0x%02X) !\n", opc[0U]);
            cp_rsp[1U] = MCP_SE_MCP_RESULT_OPCODE_NOT_SUPP;
            break;
        }
    }

    if ((0xFFFFU != mcp_ntf_flag))
    {
        mcs_ntf_info.mcp_ntf_type = mcp_ntf_flag;

        /* Schedule a timer to send Notifications */
        BT_start_timer
        (
            &appl_mcp_se_send_ntf_timer_handle,
            1U,
            appl_mcp_se_send_ntf_timer_expiry_handler,
            &mcs_ntf_info,
            sizeof(mcs_ntf_info)
        );
    }

    /* Schedule a timer to send the Control Point response */
    BT_start_timer
    (
        &appl_mcp_se_send_mcp_cp_rsp_timer_handle,
        3U,
        appl_mcp_se_send_mcp_cp_rsp_on_timer_expiry_handler,
        &mcp_cntrl_rsp,
        sizeof(mcp_cntrl_rsp)
    );

    return GA_SUCCESS;
}

GA_RESULT appl_mcp_handle_scp_ind
          (
              GA_ENDPOINT * dev,
              MCP_SE_EVT  * evt,
              UINT8         mcs_sess_id
          )
{
    APPL_CNTRL_POINT_RSP_PARAM mcp_cntrl_rsp;
    APPL_MCP_NTF_INFO  mcs_ntf_info;
    UCHAR index;
    UCHAR* scp_req = (UCHAR *)evt->data;

    /* By default, SCP Response Code is set to Success */
    cp_rsp[0U] = MCP_SCP_RES_CODE_SUCCESS;

    /* By default, Set to send Ntf - CP Response */
    GA_COPY_BRR_DEVICE(&mcp_cntrl_rsp.dev, dev);
    mcp_cntrl_rsp.mcp_se_handle = evt->mcp_se_handle;
    mcp_cntrl_rsp.char_info = &cp_rsp[0U];
    mcp_cntrl_rsp.char_info_len = 1U;

    /*
     * TODO:
     * Currently for all MCS session search result object is set to
     * appl_mcp_se_search_res_obj. All the search Control Point Type values
     * for all MCS session are getting set with same default values. Based on
     * the default values make change in IXIT values to check during
     * Search control point.
     * Implement to search results based on received SCP Type values.
     */
    if (evt->len < 2U)
    {
        cp_rsp[0] = MCP_SCP_RES_CODE_FAILURE;
        APPL_ERR("\n==== Invalid SCP Write Length ======== !\n");
    }
    else
    {
        APPL_TRC("Data Length: 0x%04X\n", evt->len);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  Length: 0x%02X\n",
            (UCHAR)sizeof(scp_req[0U]),
            scp_req[0U]
        );
        APPL_TRC("Len: 0x%02X,  Type: ", (UCHAR)sizeof(scp_req[1U]));
        switch (scp_req[1U])
        {
        case MCP_SCP_TYPE_TRACK_NAME:
            APPL_TRC("Track Name (0x%02X)\n", scp_req[1U]);
            APPL_TRC("Len: 0x%02X,  Parameter: ", (scp_req[1U]-1U));
            for (index = 0U; index < (scp_req[1U]-1U); index++)
            {
                APPL_TRC("%c", scp_req[index+2U]);
            }
            APPL_TRC("\n");
            break;
        case MCP_SCP_TYPE_ARTIST_NAME:
            APPL_TRC("Artist Name (0x%02X)\n", scp_req[1U]);
            APPL_TRC("Len: 0x%02X,  Parameter: ", (scp_req[1U]-1U));
            for (index = 0U; index < (scp_req[1U]-1U); index++)
            {
                APPL_TRC("%c", scp_req[index+2U]);
            }
            APPL_TRC("\n");
            break;
        case MCP_SCP_TYPE_ALBUM_NAME:
            APPL_TRC("Album Name (0x%02X)\n", scp_req[1U]);
            APPL_TRC("Len: 0x%02X,  Parameter: ", (scp_req[1U]-1U));
            for (index = 0U; index < (scp_req[1U]-1U); index++)
            {
                APPL_TRC("%c", scp_req[index+2U]);
            }
            APPL_TRC("\n");
            break;
        case MCP_SCP_TYPE_GROUP_NAME:
            APPL_TRC("Group Name (0x%02X)\n", scp_req[1U]);
            APPL_TRC("Len: 0x%02X,  Parameter: ", (scp_req[1U]-1U));
            for (index = 0U; index < (scp_req[1U]-1U); index++)
            {
                APPL_TRC("%c", scp_req[index+2U]);
            }
            APPL_TRC("\n");
            break;
        case MCP_SCP_TYPE_EARLIEST_YEAR:
            APPL_TRC("Earliest Year (0x%02X)\n", scp_req[1U]);
            APPL_TRC("Len: 0x%02X,  Parameter: ", (scp_req[1U]-1U));
            for (index = 0U; index < (scp_req[1U]-1U); index++)
            {
                APPL_TRC("%c", scp_req[index+2U]);
            }
            APPL_TRC("\n");
            break;
        case MCP_SCP_TYPE_LATEST_YEAR:
            APPL_TRC("Latest Year (0x%02X)\n", scp_req[1U]);
            APPL_TRC("Len: 0x%02X,  Parameter: ", (scp_req[1U]-1U));
            for (index = 0U; index < (scp_req[1U]-1U); index++)
            {
                APPL_TRC("%c", scp_req[index+2U]);
            }
            APPL_TRC("\n");
            break;
        case MCP_SCP_TYPE_GENRE:
            APPL_TRC("Genre (0x%02X)\n", scp_req[1U]);
            APPL_TRC("Len: 0x%02X,  Parameter: ", (scp_req[1U]-1U));
            for (index = 0U; index < (scp_req[1U]-1U); index++)
            {
                APPL_TRC("%c", scp_req[index+2U]);
            }
            APPL_TRC("\n");
            break;
        case MCP_SCP_TYPE_ONLY_TRACKS:
            APPL_TRC("Only Tracks (0x%02X)\n", scp_req[1U]);
            break;
        case MCP_SCP_TYPE_ONLY_GROUPS:
            APPL_TRC("Only Groups (0x%02X)\n", scp_req[1U]);
            break;
        default:
            APPL_ERR("Type Unknown (0x%02X) !\n", scp_req[1U]);
            cp_rsp[0U] = MCP_SCP_RES_CODE_FAILURE;
            break;
        }
    }

    if (MCP_SCP_RES_CODE_FAILURE == cp_rsp[0U])
    {
        appl_mcs_players[mcs_sess_id].mcs_curr_track.search_res_obj_id_len = 0U;
    }
    else
    {
        /* Copy the info to send notification */
        GA_COPY_BRR_DEVICE(&mcs_ntf_info.device, dev);
        mcs_ntf_info.mcs_sess_id = mcs_sess_id;
        mcs_ntf_info.mcp_se_handle = evt->mcp_se_handle;
        /* Schedule a timer to send Notification for Search Result Object ID */
        BT_start_timer
        (
            &appl_mcp_se_send_ntf_search_res_obj_id_timer_handle,
            1U,
            appl_mcp_se_send_ntf_search_res_obj_id_timer_expiry_handler,
            &mcs_ntf_info,
            sizeof(mcs_ntf_info)
        );
    }

    /* Schedule a timer to send the Control Point response */
    BT_start_timer
    (
        &appl_mcp_se_send_mcp_cp_rsp_timer_handle,
        3U,
        appl_mcp_se_send_mcp_cp_rsp_on_timer_expiry_handler,
        &mcp_cntrl_rsp,
        sizeof(mcp_cntrl_rsp)
    );

    return GA_SUCCESS;
}

void appl_update_default_vlc_char_values(void )
{
    UINT8 index;

    /* Initialize */
    index = APPL_MCP_VLC_PLAYER - 1U;

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_mp_params.mp_name[0U],
        "VLC Media Player",
        16U
    );

    appl_mcs_players[index].mcs_mp_params.mp_name_len = 16U;

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_mp_params.mp_url[0U],
        "https://vlc_url.com",
        APPL_MCP_MAX_STR_LEN
    );

    appl_mcs_players[index].mcs_mp_params.mp_url_len = APPL_MCP_MAX_STR_LEN;

    /* Object ID - Icon VLC */
    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_mp_params.mp_icon.mcp_obj_id[0U],
        &appl_mcp_se_icon_obj_vlc.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    /* By default, set MP as Active and Playing */
    appl_mcs_players[index].is_mp_active = GA_TRUE;

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.track_title[0U],
        "AtoBtoCto",
        (9U+1U)
    );
    appl_mcs_players[index].mcs_curr_track.track_title_len = (9U + 1U);

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.artist_name[0U],
        "XYZ",
        (3U+1U)
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.album_name[0U],
        "ABC",
        (3U+1U)
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.group_name[0U],
        "Live",
        (4U+1U)
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.earliest_year[0U],
        "2021",
        (4U+1U)
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.latest_year[0U],
        "2021",
        (4U+1U)
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.genre[0U],
        "Happy",
        (5U+1U)
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.only_tracks[0U],
        "AtoBtoCto",
        (9U+1U)
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.only_groups[0U],
        "AtoBtoCto",
        (9U+1U)
    );

    /* Set Track Dur, Pos, Playback/Seeking Speed */
    appl_mcs_players[index].mcs_curr_track.track_dur = 0x05;
    appl_mcs_players[index].mcs_curr_track.track_pos = 0x0000000F;

    appl_mcs_players[index].mcs_curr_track.playback_speed = MCP_DEFAULT_PLAYBACK_SPEED;
    appl_mcs_players[index].mcs_curr_track.seeking_speed = MCP_DEFAULT_SEEKING_SPEED;

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.curr_track_seg_obj_id,
        &appl_mcp_se_track_segment_obj.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.curr_track_obj_id,
        &appl_mcp_se_track_obj_1.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.next_track_obj_id,
        &appl_mcp_se_track_obj_2.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.parent_grp_obj_id,
        &appl_mcp_se_parent_grp_obj.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.curr_grp_obj_id,
        &appl_mcp_se_grp_obj_1.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.search_res_obj_id,
        &appl_mcp_se_search_res_obj.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    appl_mcs_players[index].mcs_curr_track.search_res_obj_id_len = APPL_OBJ_ID_LEN;

    appl_mcs_players[index].mcs_curr_track.ccid = 0x00U;

    appl_mcs_players[index].mcs_mp_params.playing_order = MCP_PLAYING_ORDER_SINGLE_ONCE;
    appl_mcs_players[index].mcs_mp_params.playing_order_supp = MCP_PLAYING_ORDER_SUPP_SINGLE_ONCE;
    appl_mcs_players[index].mcs_mp_params.media_state = MCP_MEDIA_STATE_PAUSED;
    appl_mcs_players[index].mcs_curr_track.mcp_opc_supp =
                                                        MCP_CP_OPC_SUPP_PLAY |
                                                        MCP_CP_OPC_SUPP_PAUSE |
                                                        MCP_CP_OPC_SUPP_FAST_REW |
                                                        MCP_CP_OPC_SUPP_FAST_FWD |
                                                        MCP_CP_OPC_SUPP_STOP |
                                                        MCP_CP_OPC_SUPP_MOVE_RELATIVE |
                                                        MCP_CP_OPC_SUPP_PREV_SEG |
                                                        MCP_CP_OPC_SUPP_NEXT_SEG |
                                                        MCP_CP_OPC_SUPP_FIRST_SEG |
                                                        MCP_CP_OPC_SUPP_LAST_SEG |
                                                        MCP_CP_OPC_SUPP_GOTO_SEG |
                                                        MCP_CP_OPC_SUPP_PREV_TRACK |
                                                        MCP_CP_OPC_SUPP_NEXT_TRACK |
                                                        MCP_CP_OPC_SUPP_FIRST_TRACK |
                                                        MCP_CP_OPC_SUPP_LAST_TRACK |
                                                        MCP_CP_OPC_SUPP_GOTO_TRACK |
                                                        MCP_CP_OPC_SUPP_PREV_GRP |
                                                        MCP_CP_OPC_SUPP_NEXT_GRP |
                                                        MCP_CP_OPC_SUPP_FIRST_GRP |
                                                        MCP_CP_OPC_SUPP_LAST_GRP |
                                                        MCP_CP_OPC_SUPP_GOTO_GRP;

    /* Timer Handles for sending Notifications */
    appl_mcp_se_send_mcp_cp_rsp_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_curr_track_pos_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_curr_playback_speed_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_curr_track_obj_id_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_playing_order_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_search_res_obj_id_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
}

void appl_update_default_mpc_char_values(void )
{
    UINT8 index;

    /* Initialize */
    index = APPL_MCP_MPC_PLAYER - 1U;

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_mp_params.mp_name,
        "MPClassic",
        APPL_MCP_MAX_STR_LEN
    );

    appl_mcs_players[index].mcs_mp_params.mp_name_len = APPL_MCP_MAX_STR_LEN;

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_mp_params.mp_url,
        "https://mpc_url.com",
        APPL_MCP_MAX_STR_LEN
    );

    appl_mcs_players[index].mcs_mp_params.mp_url_len = APPL_MCP_MAX_STR_LEN;

    /* Object ID - Icon MPC */
    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_mp_params.mp_icon.mcp_obj_id,
        &appl_mcp_se_icon_obj_mpc.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    /* By default, set MP as Inactive */
    appl_mcs_players[index].is_mp_active = GA_FALSE;

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.track_title[0U],
        "",
        (0U+1U)
    );
    appl_mcs_players[index].mcs_curr_track.track_title_len = (0U+1U);

    /* Set Track Dur, Pos, Playback/Seeking Speed */
    appl_mcs_players[index].mcs_curr_track.track_dur = 0x05;
    appl_mcs_players[index].mcs_curr_track.track_pos = 0x0000000F;

    appl_mcs_players[index].mcs_curr_track.playback_speed = MCP_DEFAULT_PLAYBACK_SPEED;
    appl_mcs_players[index].mcs_curr_track.seeking_speed = MCP_DEFAULT_SEEKING_SPEED;

    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.curr_track_seg_obj_id,
        &appl_mcp_no_obj_set[0U],
        APPL_OBJ_ID_LEN
    );
    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.curr_track_obj_id,
        &appl_mcp_no_obj_set[0U],
        APPL_OBJ_ID_LEN
    );
    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.next_track_obj_id,
        &appl_mcp_no_obj_set[0U],
        APPL_OBJ_ID_LEN
    );
    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.parent_grp_obj_id,
        &appl_mcp_no_obj_set[0U],
        APPL_OBJ_ID_LEN
    );
    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.curr_grp_obj_id,
        &appl_mcp_no_obj_set[0U],
        APPL_OBJ_ID_LEN
    );

    appl_mcs_players[index].mcs_mp_params.playing_order = MCP_PLAYING_ORDER_SINGLE_ONCE;
    appl_mcs_players[index].mcs_mp_params.playing_order_supp = MCP_PLAYING_ORDER_SUPP_SINGLE_ONCE;
    appl_mcs_players[index].mcs_mp_params.media_state = MCP_MEDIA_STATE_PAUSED;
    appl_mcs_players[index].mcs_curr_track.mcp_opc_supp =
        MCP_CP_OPC_SUPP_PLAY |
        MCP_CP_OPC_SUPP_PAUSE |
        MCP_CP_OPC_SUPP_FAST_REW |
        MCP_CP_OPC_SUPP_FAST_FWD |
        MCP_CP_OPC_SUPP_STOP |
        MCP_CP_OPC_SUPP_MOVE_RELATIVE |
        MCP_CP_OPC_SUPP_PREV_SEG |
        MCP_CP_OPC_SUPP_NEXT_SEG |
        MCP_CP_OPC_SUPP_FIRST_SEG |
        MCP_CP_OPC_SUPP_LAST_SEG |
        MCP_CP_OPC_SUPP_GOTO_SEG |
        MCP_CP_OPC_SUPP_PREV_TRACK |
        MCP_CP_OPC_SUPP_NEXT_TRACK |
        MCP_CP_OPC_SUPP_FIRST_TRACK |
        MCP_CP_OPC_SUPP_LAST_TRACK |
        MCP_CP_OPC_SUPP_GOTO_TRACK |
        MCP_CP_OPC_SUPP_PREV_GRP |
        MCP_CP_OPC_SUPP_NEXT_GRP |
        MCP_CP_OPC_SUPP_FIRST_GRP |
        MCP_CP_OPC_SUPP_LAST_GRP |
        MCP_CP_OPC_SUPP_GOTO_GRP;
    GA_mem_copy
    (
        &appl_mcs_players[index].mcs_curr_track.search_res_obj_id,
        &appl_mcp_se_search_res_obj.obj_id[0U],
        APPL_OBJ_ID_LEN
    );

    appl_mcs_players[index].mcs_curr_track.ccid = 0x00U;

    /* Timer Handles for sending Notifications */
    appl_mcp_se_send_mcp_cp_rsp_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_curr_track_pos_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_curr_playback_speed_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_curr_track_obj_id_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_playing_order_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    appl_mcp_se_send_ntf_search_res_obj_id_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
}

void appl_mcp_se_otp_se_init_obj(void )
{
    MCP_SE_TRACK_SEG_OBJ seg_obj;
    MCP_SE_TRACK_OBJ     track_obj;
    MCP_SE_MP_ICON_OBJ   icon_obj;
    MCP_SE_GRP_OBJ       curr_grp_obj;
    MCP_SE_GRP_OBJ       parent_grp_obj;

    /* Set the Object Data and Object Data Len here */

    /* Search Result - Track Object 1 */
    appl_mcp_se_search_res_obj.obj_datalen = MCP_OBJ_ID_LEN + 1U;
    appl_mcp_search_res_obj_data[0U] = 0x00U;
    GA_mem_copy
    (
        &appl_mcp_search_res_obj_data[1U],
        &appl_mcp_se_track_obj_1.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    appl_mcp_se_search_res_obj.obj_data = &appl_mcp_search_res_obj_data[0U];

    /* Track Object - 1 */
    track_obj.total_obj_len = 0x0AU;
    track_obj.IDV32_data = &track_obj_data;

    GA_mem_copy
    (
        &appl_mcp_track_obj_data[0U],
        track_obj.IDV32_data,
        track_obj.total_obj_len
    );
    appl_mcp_se_track_obj_1.obj_data = &appl_mcp_track_obj_data[0U];

    /* Track Object - 2 */
    appl_mcp_se_track_obj_2.obj_data = &appl_mcp_track_obj_data[0U];

    /* Track Object - 3 */
    appl_mcp_se_track_obj_3.obj_data = &appl_mcp_track_obj_data[0U];

    /* Icon */
    icon_obj.total_obj_len = 0x10U;
    icon_obj.icon_png_data = &icon_obj_data;

    GA_mem_copy
    (
        &appl_mcp_icon_obj_data[0U],
        icon_obj.icon_png_data,
        icon_obj.total_obj_len
    );

    /* VLC Icon Data */
    appl_mcp_se_icon_obj_vlc.obj_datalen = icon_obj.total_obj_len;
    appl_mcp_se_icon_obj_vlc.obj_data = &appl_mcp_icon_obj_data[0U];

    /* MPC Icon Data */
    appl_mcp_se_icon_obj_mpc.obj_datalen = icon_obj.total_obj_len;
    appl_mcp_se_icon_obj_mpc.obj_data = &appl_mcp_icon_obj_data[0U];

    /* Track Segment: By default, Point it to the First Segment */
    seg_obj.seg_name = seg_name_first;
    seg_obj.seg_name_len = 0x09U;
    seg_obj.seg_pos = 0x0A;
    seg_obj.total_obj_len = sizeof(UINT8) + seg_obj.seg_name_len + sizeof(INT32);
    appl_mcp_seg_obj_data[0U] = seg_obj.seg_name_len;
    GA_mem_copy
    (
        &appl_mcp_seg_obj_data[1U],
        seg_obj.seg_name,
        seg_obj.seg_name_len
    );
    GA_mem_copy(&appl_mcp_seg_obj_data[10U], &seg_obj.seg_pos, sizeof(INT32));

    GA_mem_copy(&appl_mcp_seg_obj_data[0U], &seg_obj, seg_obj.total_obj_len);
    appl_mcp_se_track_segment_obj.obj_data = &appl_mcp_seg_obj_data[0U];
    appl_mcp_se_track_segment_obj.obj_datalen = seg_obj.total_obj_len;

    /* Group Object 1 */
    GA_mem_copy
    (
        &curr_grp_obj.obj[0U],
        &appl_mcp_se_track_obj_1.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    curr_grp_obj.obj_type = MCP_SE_GRP_OBJ_TYPE_TRACK;
    curr_grp_obj.total_obj_len = sizeof(UINT8) + MCP_OBJ_ID_LEN;

    GA_mem_copy
    (
        &appl_mcp_grp_obj_1_data[0U],
        &curr_grp_obj,
        curr_grp_obj.total_obj_len
    );
    appl_mcp_se_grp_obj_1.obj_data = &appl_mcp_grp_obj_1_data[0U];
    appl_mcp_se_grp_obj_1.obj_datalen = curr_grp_obj.total_obj_len;

    /* Group Object 2 */
    GA_mem_copy
    (
        &curr_grp_obj.obj[0U],
        &appl_mcp_se_track_obj_1.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    curr_grp_obj.obj_type = MCP_SE_GRP_OBJ_TYPE_TRACK;
    curr_grp_obj.total_obj_len = sizeof(UINT8) + MCP_OBJ_ID_LEN;

    GA_mem_copy
    (
        &appl_mcp_grp_obj_2_data[0U],
        &curr_grp_obj,
        curr_grp_obj.total_obj_len
    );
    appl_mcp_se_grp_obj_2.obj_data = &appl_mcp_grp_obj_2_data[0U];
    appl_mcp_se_grp_obj_2.obj_datalen = curr_grp_obj.total_obj_len;

    /* Group Object 3 - Track 1, Track 2 and Track 3 */
    curr_grp_obj.obj_type = MCP_SE_GRP_OBJ_TYPE_TRACK;
    GA_mem_copy
    (
        &curr_grp_obj.obj[0U],
        &appl_mcp_se_track_obj_1.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    curr_grp_obj.total_obj_len = sizeof(UINT8) + MCP_OBJ_ID_LEN;

    GA_mem_copy
    (
        &appl_mcp_grp_obj_3_data[0U],
        &curr_grp_obj,
        curr_grp_obj.total_obj_len
    );
    appl_mcp_se_grp_obj_3.obj_datalen = curr_grp_obj.total_obj_len;

    /* Object 2 */
    curr_grp_obj.obj_type = MCP_SE_GRP_OBJ_TYPE_TRACK;
    GA_mem_copy
    (
        &curr_grp_obj.obj[0U],
        &appl_mcp_se_track_obj_2.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    curr_grp_obj.total_obj_len = sizeof(UINT8) + MCP_OBJ_ID_LEN;
    GA_mem_copy
    (
        &appl_mcp_grp_obj_3_data[7U],
        &curr_grp_obj,
        curr_grp_obj.total_obj_len
    );
    appl_mcp_se_grp_obj_3.obj_datalen += curr_grp_obj.total_obj_len;

    /* Object 3 */
    curr_grp_obj.obj_type = MCP_SE_GRP_OBJ_TYPE_TRACK;
    GA_mem_copy
    (
        &curr_grp_obj.obj[0U],
        &appl_mcp_se_track_obj_3.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    curr_grp_obj.total_obj_len = sizeof(UINT8) + MCP_OBJ_ID_LEN;
    GA_mem_copy
    (
        &appl_mcp_grp_obj_3_data[14U],
        &curr_grp_obj,
        curr_grp_obj.total_obj_len
    );
    appl_mcp_se_grp_obj_3.obj_datalen += curr_grp_obj.total_obj_len;

    /* Update the Object Data */
    appl_mcp_se_grp_obj_3.obj_data = &appl_mcp_grp_obj_3_data[0U];

    /* Group Object - Parent */

    /* Object 1 */
    parent_grp_obj.obj_type = MCP_SE_GRP_OBJ_TYPE_GROUP;
    GA_mem_copy
    (
        &parent_grp_obj.obj[0U],
        &appl_mcp_se_grp_obj_1.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    parent_grp_obj.total_obj_len = sizeof(UINT8) + MCP_OBJ_ID_LEN;

    GA_mem_copy
    (
        &appl_mcp_parent_grp_obj_data[0U],
        &parent_grp_obj,
        parent_grp_obj.total_obj_len
    );
    appl_mcp_se_parent_grp_obj.obj_datalen = parent_grp_obj.total_obj_len;

    /* Object 2 */
    parent_grp_obj.obj_type = MCP_SE_GRP_OBJ_TYPE_GROUP;
    GA_mem_copy
    (
        &parent_grp_obj.obj[0U],
        &appl_mcp_se_grp_obj_2.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    parent_grp_obj.total_obj_len = sizeof(UINT8) + MCP_OBJ_ID_LEN;
    GA_mem_copy
    (
        &appl_mcp_parent_grp_obj_data[7U],
        &parent_grp_obj,
        parent_grp_obj.total_obj_len
    );
    appl_mcp_se_parent_grp_obj.obj_datalen += parent_grp_obj.total_obj_len;

    /* Object 3 */
    parent_grp_obj.obj_type = MCP_SE_GRP_OBJ_TYPE_GROUP;
    GA_mem_copy
    (
        &parent_grp_obj.obj[0U],
        &appl_mcp_se_grp_obj_3.obj_id[0U],
        MCP_OBJ_ID_LEN
    );
    parent_grp_obj.total_obj_len = sizeof(UINT8) + MCP_OBJ_ID_LEN;
    GA_mem_copy
    (
        &appl_mcp_parent_grp_obj_data[14U],
        &parent_grp_obj,
        parent_grp_obj.total_obj_len
    );
    appl_mcp_se_parent_grp_obj.obj_datalen += parent_grp_obj.total_obj_len;

    /* Update the Object Data */
    appl_mcp_se_parent_grp_obj.obj_data = &appl_mcp_parent_grp_obj_data[0U];

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    /* Timer Handles for sending Indications */
    ots_OACP_send_ctrl_pnt_rsp_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    ots_OLCP_send_ctrl_pnt_rsp_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
}

void appl_mcp_se_generate_segment(UINT8 seg_type)
{
    MCP_SE_TRACK_SEG_OBJ seg_obj_1, seg_obj_2;

    seg_obj_1.seg_name = seg_name_first;
    seg_obj_1.seg_pos = APPL_MCP_SE_SEG_POS_FIRST;
    seg_obj_2.seg_name = seg_name_second;
    seg_obj_2.seg_pos = APPL_MCP_SE_SEG_POS_SECOND;

    switch (seg_type)
    {
    case 0x01: /* First Segment */
        appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_pos =
            APPL_MCP_SE_SEG_POS_FIRST;
        break;
    case 0x02: /* Second Segment */
        appl_mcs_players[appl_gmcs_player_id].mcs_curr_track.track_pos =
            APPL_MCP_SE_SEG_POS_SECOND;
        break;
    default:
        return;
    }

    seg_obj_1.seg_name_len = 0x09U;
    seg_obj_1.total_obj_len = sizeof(UINT8) + seg_obj_1.seg_name_len + sizeof(INT32);
    appl_mcp_seg_obj_data[0U] = seg_obj_1.seg_name_len;
    GA_mem_copy(&appl_mcp_seg_obj_data[1U], seg_obj_1.seg_name, seg_obj_1.seg_name_len);
    GA_mem_copy(&appl_mcp_seg_obj_data[10U], &seg_obj_1.seg_pos, sizeof(INT32));

    seg_obj_2.seg_name_len = 0x09U;
    seg_obj_2.total_obj_len = sizeof(UINT8) + seg_obj_2.seg_name_len + sizeof(INT32);
    appl_mcp_seg_obj_data[14U] = seg_obj_2.seg_name_len;
    GA_mem_copy(&appl_mcp_seg_obj_data[15U], seg_obj_2.seg_name, seg_obj_2.seg_name_len);
    GA_mem_copy(&appl_mcp_seg_obj_data[24U], &seg_obj_2.seg_pos, sizeof(INT32));

    appl_mcp_se_track_segment_obj.obj_data = (UCHAR *)&appl_mcp_seg_obj_data[0U];
    appl_mcp_se_track_segment_obj.obj_datalen =
        seg_obj_1.total_obj_len + seg_obj_2.total_obj_len;

    return;
}

GA_RESULT appl_mcp_se_send_ntf_curr_track_pos_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          )
{
    GA_RESULT            retval;
    APPL_MCP_NTF_INFO  * mcs_ntf_info;

    retval = GA_FAILURE;

    if ((NULL != data) && (0U != datalen))
    {
        mcs_ntf_info = (APPL_MCP_NTF_INFO *)data;
        if (MCP_MEDIA_STATE_PLAYING == appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_mp_params.media_state)
        {
            /* TODO: How to validate Constant Playback Speed */
            return GA_FAILURE;
        }
        else if (MCP_MEDIA_STATE_SEEKING == appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_mp_params.media_state)
        {
            /*
             * TODO: Keep sending at a Implementation Specific timeout until the
             * playback speed becomes constant and the media state moves back to a
             * different state like Play/Paused.
             */

        }
        else
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("SENDING GA MCP NOTIFICATION\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("GA_mcp_se_notify_track_pos\n");
            retval = GA_mcp_se_notify_track_pos
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.track_pos,
                         sizeof(MCP_CHAR_PARAM_TYPE_INT32)
                     );
            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
    }
    else
    {
        APPL_ERR("Data is NULL !\n");
    }

    return retval;
}

GA_RESULT appl_mcp_se_send_ntf_curr_playback_speed_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          )
{
    GA_RESULT            retval;
    APPL_MCP_NTF_INFO  * mcs_ntf_info;

    retval = GA_FAILURE;

    if ((NULL != data) && (0U != datalen))
    {
        mcs_ntf_info = (APPL_MCP_NTF_INFO *)data;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_mcp_se_notify_playback_speed\n");
        retval = GA_mcp_se_notify_playback_speed
                 (
                     &mcs_ntf_info->device,
                     mcs_ntf_info->mcp_se_handle,
                     &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.playback_speed,
                     sizeof(MCP_CHAR_PARAM_TYPE_INT8)
                 );
        APPL_TRC("Retval: ");
        appl_mcp_utils_display_ga_result
        (
            retval,
            GA_SERVICE_GMCS
        );
    }
    else
    {
        APPL_ERR("Data is NULL !\n");
    }

    return retval;
}

GA_RESULT appl_mcp_se_send_ntf_curr_track_obj_id_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          )
{
    GA_RESULT            retval;
    APPL_MCP_NTF_INFO  * mcs_ntf_info;

    retval = GA_FAILURE;

    if ((NULL != data) && (0U != datalen))
    {
        mcs_ntf_info = (APPL_MCP_NTF_INFO *)data;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_mcp_se_notify_curr_track_obj_id\n");
        retval = GA_mcp_se_notify_curr_track_obj_id
                 (
                     &mcs_ntf_info->device,
                     mcs_ntf_info->mcp_se_handle,
                     &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.curr_track_obj_id.mcp_obj_id[0U],
                     sizeof(MCP_PARAM_OBJ_ID)
                 );
        APPL_TRC("Retval: ");
        appl_mcp_utils_display_ga_result
        (
            retval,
            GA_SERVICE_GMCS
        );
    }
    else
    {
        APPL_ERR("Data is NULL !\n");
    }

    return retval;
}

GA_RESULT appl_mcp_se_send_ntf_next_track_obj_id
          (
              GA_ENDPOINT   * dev,
              UINT8           mcs_sess_id,
              MCP_SE_HANDLE   mcp_se_handle
          )
{
    GA_RESULT retval;

    APPL_TRC("\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC("SENDING GA MCP NOTIFICATION\n");
    GA_FORMAT_DISPLAY();
    APPL_TRC("GA_mcp_se_notify_curr_track_obj_id\n");
    retval = GA_mcp_se_notify_curr_track_obj_id
             (
                 dev,
                 mcp_se_handle,
                 &appl_mcs_players[mcs_sess_id].mcs_curr_track.next_track_obj_id.mcp_obj_id[0U],
                 sizeof(MCP_PARAM_OBJ_ID)
             );
    APPL_TRC("Retval: ");
    appl_mcp_utils_display_ga_result
    (
        retval,
        GA_SERVICE_GMCS
    );

    return retval;
}

GA_RESULT appl_mcp_se_handle_write_curr_grp_id
          (
              GA_ENDPOINT   * dev,
              UINT8           mcs_sess_id,
              MCP_SE_HANDLE   mcp_se_handle,
              UINT8         * new_grp_obj_id
          )
{
    APPL_MCP_NTF_INFO  mcs_ntf_info;

    if ((0U == GA_mem_cmp
               (
                   &appl_mcp_se_grp_obj_1.obj_id[0U],
                   new_grp_obj_id,
                   APPL_OBJ_ID_LEN
               ))
        ||
        (0U == GA_mem_cmp
               (
                   &appl_mcp_se_grp_obj_2.obj_id[0U],
                   new_grp_obj_id,
                   APPL_OBJ_ID_LEN
               ))
        ||
        (0U == GA_mem_cmp
               (
                   &appl_mcp_se_grp_obj_3.obj_id[0U],
                   new_grp_obj_id,
                   APPL_OBJ_ID_LEN
               )))
    {
        /*
         * Valid Group Obj ID:
         * Change the Current Group
         * Set the Current Track to First Track of the new Group
         * Set the Track Pos to 0.
         */
        GA_mem_copy
        (
            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_grp_obj_id.mcp_obj_id[0U],
            new_grp_obj_id,
            APPL_OBJ_ID_LEN
        );

        GA_mem_copy
        (
            &appl_mcs_players[mcs_sess_id].mcs_curr_track.curr_track_obj_id.mcp_obj_id[0U],
            &appl_mcp_se_track_obj_1.obj_id[0U],
            APPL_OBJ_ID_LEN
        );

        /* Copy the info to send notification */
        GA_COPY_BRR_DEVICE(&mcs_ntf_info.device, dev);
        mcs_ntf_info.mcs_sess_id = mcs_sess_id;
        mcs_ntf_info.mcp_se_handle = mcp_se_handle;

        /* Schedule a timer to send Notification for Current Track Object ID */
        BT_start_timer
        (
            &appl_mcp_se_send_ntf_curr_track_obj_id_timer_handle,
            1U,
            appl_mcp_se_send_ntf_curr_track_obj_id_timer_expiry_handler,
            &mcs_ntf_info,
            sizeof(mcs_ntf_info)
        );

        appl_mcs_players[mcs_sess_id].mcs_curr_track.track_pos = 0;
        /* Schedule a timer to send Notification for Current Track Position */
        BT_start_timer
        (
            &appl_mcp_se_send_ntf_curr_track_pos_timer_handle,
            1U,
            appl_mcp_se_send_ntf_curr_track_pos_timer_expiry_handler,
            &mcs_ntf_info,
            sizeof(mcs_ntf_info)
        );
    }

    return GA_SUCCESS;
}

GA_RESULT appl_mcp_se_send_ntf_playing_order_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          )
{
    GA_RESULT            retval;
    APPL_MCP_NTF_INFO  * mcs_ntf_info;

    retval = GA_FAILURE;

    if ((NULL != data) && (0U != datalen))
    {
        mcs_ntf_info = (APPL_MCP_NTF_INFO *)data;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_mcp_se_notify_playing_order\n");
        retval = GA_mcp_se_notify_playing_order
                 (
                     &mcs_ntf_info->device,
                     mcs_ntf_info->mcp_se_handle,
                     &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_mp_params.playing_order,
                     sizeof(MCP_CHAR_PARAM_TYPE_UINT8)
                 );
        APPL_TRC("Retval: ");
        appl_mcp_utils_display_ga_result
        (
            retval,
            GA_SERVICE_GMCS
        );
    }
    else
    {
        APPL_ERR("Data is NULL !\n");
    }

    return retval;
}

GA_RESULT appl_mcp_se_send_ntf_search_res_obj_id_timer_expiry_handler
          (
              void   * data,
              UINT16   datalen
          )
{
    GA_RESULT            retval;
    APPL_MCP_NTF_INFO  * mcs_ntf_info;

    retval = GA_FAILURE;

    if ((NULL != data) && (0U != datalen))
    {
        mcs_ntf_info = (APPL_MCP_NTF_INFO *)data;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("GA_mcp_se_notify_search_res_obj_id\n");
        retval = GA_mcp_se_notify_search_res_obj_id
                 (
                     &mcs_ntf_info->device,
                     mcs_ntf_info->mcp_se_handle,
                     &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.search_res_obj_id.mcp_obj_id[0U],
                     appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.search_res_obj_id_len
                 );
        APPL_TRC("Retval: ");
        appl_mcp_utils_display_ga_result
        (
            retval,
            GA_SERVICE_GMCS
        );
    }
    else
    {
        APPL_ERR("Data is NULL !\n");
    }

    return retval;
}

void appl_mcp_se_send_ntf_timer_expiry_handler
     (
         void   * data,
         UINT16   datalen
     )
{
    GA_RESULT retval;
    APPL_MCP_NTF_INFO * mcs_ntf_info;
    UINT16 mcp_ntf_flag;

    if ((0U != datalen) || (NULL != data))
    {
        mcs_ntf_info = (APPL_MCP_NTF_INFO *)data;
        mcp_ntf_flag = mcs_ntf_info->mcp_ntf_type;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();

        if (mcp_ntf_flag & APPL_MCP_SE_NTF_CURR_GRP_OBJ_ID)
        {
            APPL_TRC("GA_mcp_se_notify_curr_grp_obj_id\n");
            retval = GA_mcp_se_notify_curr_grp_obj_id
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.curr_grp_obj_id,
                         sizeof(MCP_PARAM_OBJ_ID)
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        if (mcp_ntf_flag & APPL_MCP_SE_NTF_TRACK_CHANGED)
        {
            APPL_TRC("GA_mcp_se_notify_track_changed\n");
            retval = GA_mcp_se_notify_track_changed
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         NULL,
                         0U
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        if (mcp_ntf_flag & APPL_MCP_SE_NTF_TRACK_TITLE)
        {
            APPL_TRC("GA_mcp_se_notify_track_title\n");
            retval = GA_mcp_se_notify_track_title
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.track_title,
                         appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.track_title_len
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        if (mcp_ntf_flag & APPL_MCP_SE_NTF_TRACK_DURATION)
        {
            APPL_TRC("GA_mcp_se_notify_track_duration\n");
            retval = GA_mcp_se_notify_track_duration
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.track_dur,
                         sizeof(MCP_CHAR_PARAM_TYPE_INT32)
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        if (mcp_ntf_flag & APPL_MCP_SE_NTF_CURR_TRACK_OBJ_ID)
        {
            APPL_TRC("GA_mcp_se_notify_curr_track_obj_id\n");
            retval = GA_mcp_se_notify_curr_track_obj_id
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.curr_track_obj_id,
                         sizeof(MCP_PARAM_OBJ_ID)
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        if (mcp_ntf_flag & APPL_MCP_SE_NTF_NEXT_TRACK_OBJ_ID)
        {
            APPL_TRC("GA_mcp_se_notify_next_track_obj_id\n");
            retval = GA_mcp_se_notify_next_track_obj_id
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.next_track_obj_id,
                         sizeof(MCP_PARAM_OBJ_ID)
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        if (mcp_ntf_flag & APPL_MCP_SE_NTF_TRACK_POS)
        {
            APPL_TRC("GA_mcp_se_notify_track_pos\n");
            retval = GA_mcp_se_notify_track_pos
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.track_pos,
                         sizeof(MCP_CHAR_PARAM_TYPE_INT32)
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        if (mcp_ntf_flag & APPL_MCP_SE_NTF_SEEKING_SPEED)
        {
            APPL_TRC("GA_mcp_se_notify_seeking_speed\n");
            retval = GA_mcp_se_notify_seeking_speed
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_curr_track.seeking_speed,
                         sizeof(MCP_CHAR_PARAM_TYPE_INT8)
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        if (mcp_ntf_flag & APPL_MCP_SE_NTF_MEDIA_STATE)
        {
            APPL_TRC("GA_mcp_se_notify_media_state\n");
            retval = GA_mcp_se_notify_media_state
                     (
                         &mcs_ntf_info->device,
                         mcs_ntf_info->mcp_se_handle,
                         &appl_mcs_players[mcs_ntf_info->mcs_sess_id].mcs_mp_params.media_state,
                         sizeof(MCP_CHAR_PARAM_TYPE_UINT8)
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
    }
    return;
}

void appl_mcp_se_send_mcp_cp_rsp_on_timer_expiry_handler
     (
         void   * cntrl_point_rsp,
         UINT16   datalen
     )
{
    GA_RESULT retval;
    APPL_CNTRL_POINT_RSP_PARAM * mcp_cntrl_rsp;

    if ((0U != datalen) || (NULL != cntrl_point_rsp))
    {
        mcp_cntrl_rsp = (APPL_CNTRL_POINT_RSP_PARAM*)cntrl_point_rsp;
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("SENDING GA MCP NOTIFICATION\n");
        GA_FORMAT_DISPLAY();

        if (1U == mcp_cntrl_rsp->char_info_len)
        {
            APPL_TRC("GA_mcp_se_notify_scp\n");
            retval = GA_mcp_se_notify_scp
                     (
                         &mcp_cntrl_rsp->dev,
                         mcp_cntrl_rsp->mcp_se_handle,
                         (void*)mcp_cntrl_rsp->char_info,
                         (UCHAR)mcp_cntrl_rsp->char_info_len
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
        else
        {
            APPL_TRC("GA_mcp_se_notify_mcp\n");
            retval = GA_mcp_se_notify_mcp
                     (
                         &mcp_cntrl_rsp->dev,
                         mcp_cntrl_rsp->mcp_se_handle,
                         (void*)mcp_cntrl_rsp->char_info,
                         (UCHAR)mcp_cntrl_rsp->char_info_len
                     );

            APPL_TRC("Retval: ");
            appl_mcp_utils_display_ga_result
            (
                retval,
                GA_SERVICE_GMCS
            );
        }
    }
    return;
}

void appl_set_mcs_sdp_record_status(UCHAR flag)
{
#if 0
    API_RESULT retval;
    UINT32 record_handle;

    /* Get the MCP record handle */
    retval = BT_dbase_get_record_handle(DB_RECORD_MCS, 0U, &record_handle);

    if (0 != flag)
    {
        retval = BT_dbase_activate_record(record_handle);
    }
    else
    {
        retval = BT_dbase_inactivate_record(record_handle);
    }
#else
    GA_IGNORE_UNUSED_PARAM(flag);
#endif /* 0 */

    return;
}
#endif /* MCP_SERVER */

