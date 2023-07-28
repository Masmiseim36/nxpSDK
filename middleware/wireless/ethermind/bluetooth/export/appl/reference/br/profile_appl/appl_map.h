
/**
 *  \file appl_map.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_MAP_
#define _H_APPL_MAP_

/* --------------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_map_api.h"
#include "BT_fops.h"
#include "object_parser_api.h"
#include "xml_parser.h"

#ifdef MAP_MSE
#include "map_pl.h"
#endif /* MAP_MSE */


/* --------------------------------------------- Global Definitions */
/* #define HAVE_OBJECT_PARSER */
/* #define HAVE_OBJECT_PARSER_DUMP */

#define MAP_CLIENT_NUM_INSTANCES         2U /* 1 */
#define MAP_SERVER_NUM_INSTANCES         2U /* 1 */

#define APPL_MAP_MAX_TAG_LEN             32U
#define APPL_PARAM_VALUE_MAX_LEN         64U

#define MAP_FOLDERLISTING_FILE           "folder_listing.xml"
#define MESSAGESLISTING_FILE             "messages_listing.xml"
#define MESSAGE_FILE                     "message.vmg"
#define EVENT_FILE                       "map_event.xml"
#define CONVLIST_DATA_FILE               "map_mse_conversation_list.txt"

#define MAP_ROOT_FOLDER                  "root"

#define MAP_FOLDER_DEPTH                 8U
#define MAP_FOLDER_NAME_LEN              64U
#define MAP_MESSAGEFILE_NAME_LEN         64U
#define MAP_CONVSN_OBJ_FILE_LEN          64U

/* Maximum length of property name */
#define M_VCARD_MAX_PROPERTY_NAME_LEN    20U

/* Other constants */
#define M_VCARD_MAX_PROPERTIES           10U
#define M_VCARD_MAX_PROPERTY_VALUE_LEN   255U
#define M_VCARD_MAX_PROPERTY_PARAMS      5U
#define M_VCARD_MAX_PARAM_NAME_LEN       10U
#define M_VCARD_MAX_GROUP_NAME_LEN       10U
#define M_VCARD_MAX_PARAM_VALUE_LEN      20U

#define BMSG_CONTENT_MARKER_LENGTH       20U

#define MAP_SDP_RECORD_DATA_SIZE         500U
#define MAX_MAP_REPOSITORY_DEPTH         3U

#define APPL_MAP_MCE_SUPPORTED_FEATURES  0x00FFFFF

/* Max. Conversations */
#define MAX_APPL_IM_CONVERSATIONS        5U

#define APPL_MAP_CONV_LISTING_VER_CNTR_LEN    0x10U
#define APPL_MAP_DATABASE_IDENTIFIER_LEN      0x10U
#define APPL_MAP_FOLDER_VER_CNTR_LEN          0x10U

/* --------------------------------------------- Structures/Data Types */

typedef struct _MAP_SDP_RECORDS
{
    UCHAR map_attrib_data[MAP_SDP_RECORD_DATA_SIZE];

    UINT16 map_len_attrib_data;

    SDP_HANDLE    map_sdp_handle;

}MAP_SDP_RECORD;

typedef struct _MAP_INSTANCE
{
    /* MAP Handle */
    MAP_HANDLE    handle;

    /* MAP SDP record */
    MAP_SDP_RECORD map_sdp_record;

    /* Peer BD Address */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

} MAP_INSTANCE;

typedef struct _MAP_MAS_INSTANCE
{
    /* MAP Instance */
    MAP_INSTANCE instance;

    /* Instance current folder path */
    UCHAR path[MAP_FOLDER_NAME_LEN];

    /* Root depth of the instance */
    INT8 root_depth;

    /* MNS Notify Status */
    UCHAR       ntf_reg_status;

    /* MNS Instance ID reference */
    UCHAR mns_id;

} MAP_MAS_INSTANCE;

/**
 * MAP IM owner status info.
 */
typedef struct _MAP_IM_OWNER_STATUS
{
    /* Convserstation id */
    UCHAR       conversation_id[MAX_CONVERSATION_ID_SIZE];

    /* Conversation id size */
    UCHAR       conversation_id_size;

    /* Presence Text */
    UCHAR       presence_text[MAX_PRESENCE_TEXT_SIZE];

    /* Presence Text size */
    UCHAR       presence_text_size;

    /* Last Activity */
    UCHAR       last_activity[MAX_MAP_TIMESTAMP_SIZE];

    /* Last Activity size */
    UCHAR       last_activity_size;

    /* Presence Availability */
    UINT8       presence_availability;

    /* Chat state */
    UINT8       chat_state;
}MAP_IM_OWNER_STATUS;

/*************************************************************************/
typedef UINT16 M_VCARD_PROPERTY_NAME;

/*
 * M_VCARD_PROPERTY_PARAM
 *
 * \brief Structure to hold the parameters and parameter values
 * of a property
 */
typedef struct
{
    /* Name of the parameters */
    UCHAR param_name[M_VCARD_MAX_PARAM_NAME_LEN];

    /* Length of parameter name */
    UINT16 param_name_len;

    /* Value of the parameter */
    UCHAR param_value[M_VCARD_MAX_PARAM_VALUE_LEN];

    /* Length of the value */
    UINT16 param_value_len;

}M_VCARD_PROPERTY_PARAM;


/*
 * M_VCARD_PROPERTY
 *
 * \brief Structure to hold a VCard property
 */
typedef struct
{
    /* Name of the group that includes this property */
    UCHAR group_name[M_VCARD_MAX_GROUP_NAME_LEN];

    /* Length of group name */
    UINT16 group_name_len;

    /* Property Type */
    M_VCARD_PROPERTY_NAME name;

    /* Property parameters */
    M_VCARD_PROPERTY_PARAM param[M_VCARD_MAX_PROPERTY_PARAMS];

    /* Number of parameters */
    UINT16 num_params;

    /* Property Value */
    UCHAR value[M_VCARD_MAX_PROPERTY_VALUE_LEN];

    /* Length of Property Value */
    UINT16 value_len;

    /*
     * Flag to indicate whether this property is preferred
     * This flag is present to minimize the time needed to search
     * preferred properties. In many cases application may be interested
     * only in preferred properties.
     */
    UCHAR is_preferred;

    /*
     * Index of the next property of the same name
     * The last property in such linked list will have M_VCARD_INVALID_INDEX
     */
    UCHAR next_index;

}M_VCARD_PROPERTY;


/*
 * M_VCARD
 *
 * Structure to store the VCard
 */
typedef struct
{
    UCHAR tel;

    UCHAR n;

    /* Not-so-frequently-used but required properties */
    M_VCARD_PROPERTY property[M_VCARD_MAX_PROPERTIES];

    /* Number of properties used */
    UINT16 num_properties;

}M_VCARD_OBJECT;

/*************************************************************************/

/* Message Property Structure */
typedef struct _BMSG_PROPERTY
{
    UCHAR     version[8];
    UINT16    status;
    UINT16    type;
    UCHAR     folder[32];
} BMSG_PROPERTY;

/* Message Body Property Structure */
typedef struct _BMSG_BODY_PROPERTY
{
    UINT16    encoding;
    UINT16    charset;
    UCHAR     language[32];
    UINT16    content_length;
} BMSG_BODY_PROPERTY;

/* Message Content Structure */
typedef struct _BMSG_CONTENT
{
    UCHAR                 part_id[8];
    BMSG_BODY_PROPERTY    property;
    UCHAR               * message;
} BMSG_CONTENT;

/* Message Envelope Structure */
typedef struct _BMSG_ENVELOPE
{
    M_VCARD_OBJECT    recepient[3];
    BMSG_CONTENT    content;
} BMSG_ENVELOPE;

/* Message Content Structure */
typedef struct _MESSAGE_OBJECT
{
    BMSG_PROPERTY    property;
    M_VCARD_OBJECT   originator;
    BMSG_ENVELOPE    envelope;
    UCHAR            envelope_level;
    UCHAR            num_envelopes;
} MESSAGE_OBJECT;

/* --------------------------------------------- Macros */
/* To Reset a MAP handle */
#define MAP_RESET_INSTANCE(hdl)    \
        (hdl)->handle = MAP_ENTITY_INVALID;    \
        BT_mem_set((hdl)->bd_addr, 0x00, BT_BD_ADDR_SIZE);

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void main_map_mce_operations (void);
void main_map_mse_operations (void);

#endif /* _H_APPL_MAP_ */

