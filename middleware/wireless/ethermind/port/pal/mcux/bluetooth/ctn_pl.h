
/**
 *  \file ctn_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_CTN_PL_
#define _H_CTN_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_ctn_api.h"
#include "BT_fops.h"

/* --------------------------------------------- Global Definitions */
/* Max Folder Name length */
#define     MAX_CTN_FOLDER_NAME_LEN                     128U

/* Max CTN object name including path */
#define     MAX_CTN_OBJECT_NAME_LEN                     256U


/* Max CTN handle length */
#define     MAX_CTN_OBJ_HANDLE_LEN                      34U

/* Max. CTN object name length */
#define     MAX_CTN_OBJ_NAME_LEN                        40U

/* Max. no. of objects in given folder */
#define     MAX_CTN_OBJ_COUNT                           10U

/* Reference folder for CTN objects */
#define     APPL_CTN_ROOT_FOLDER_REFERENCE \
    BT_FOPS_PATH_JOIN(BT_FOPS_BASE,"data" BT_FOPS_PATH_SEP "ctn")

/* Reference folder for CTN Calender objects */
#define     APPL_CTN_CALENDAR_FOLDER_REFERENCE \
            BT_FOPS_PATH_JOIN(BT_FOPS_BASE,"data" BT_FOPS_PATH_SEP "ctn" BT_FOPS_PATH_SEP "root" BT_FOPS_PATH_SEP "telecom" BT_FOPS_PATH_SEP "CTN" BT_FOPS_PATH_SEP "calendar")

/* Folder path for ctn objects */
#define     APPL_CTN_CALENDAR_FOLDER_PATH \
    BT_FOPS_PATH_SEP "root" BT_FOPS_PATH_SEP "telecom" BT_FOPS_PATH_SEP "CTN" BT_FOPS_PATH_SEP "calendar"
#define     APPL_CTN_NOTES_FOLDER_PATH \
    BT_FOPS_PATH_SEP "root" BT_FOPS_PATH_SEP "telecom" BT_FOPS_PATH_SEP "CTN" BT_FOPS_PATH_SEP "notes"
#define     APPL_CTN_TASKS_FOLDER_PATH \
    BT_FOPS_PATH_SEP "root" BT_FOPS_PATH_SEP "telecom" BT_FOPS_PATH_SEP "CTN" BT_FOPS_PATH_SEP "tasks"

/* CTN-listing file name */
#define     APPL_LISTING_FILE                           "CTN-listing.xml"

#define     APPL_LARGE_LISTING_FILE                     "CTN-listing_large.xml"

#define     APPL_NEW_CALENDER_FILE                      "new_calendar.ics"

/* CTN-get file */
#define     APPL_GET_FILE_NAME                          "CTN-pull-object.ics"

/* CTN update time */
#define     APPL_CTN_UPDATE_TIME                        "20160531T195810"

/**
 * CTN Put file name
 */
#define     APPL_PUSH_FILE_NAME                         "CTN-push.ics"

/* CTN Event file name */
#define     APPL_CTN_EVENT_FILE_NAME                    "CTN-event.ics"

/* CTN as CCE Event file name */
#define     APPL_CTN_CCE_EVENT_FILE_NAME                "CTN_CCE-event.ics"

/* CTN Account info file */
#define     APPL_CTN_ACCOUNT_INFO_FILE                  "CTN-account-info.ics"

/* CTN as CCE Account info file */
#define     APPL_CTN_CCE_ACCOUNT_INFO_FILE              "CTN_CCE-account-info.ics"

#define     APPL_CTN_CSE_EMAIL_URL_NAME                 "mindtree.com"

/* CTN object type constants */
#define     APPL_CTN_OBJ_TYPE_VEVENT                    0x00U
#define     APPL_CTN_OBJ_TYPE_VTODO                     0x01U
#define     APPL_CTN_OBJ_TYPE_VNOTE                     0x02U

#define     APPL_CTN_OBJ_TYPE_INVALIDE                  0xFFU

/* Bit mask for parameters for filtering */
#define     CTN_OBJ_PROP_MASK_ATTACHMENT                0x00000001U
#define     CTN_OBJ_PROP_MASK_SUMMARY                   0x00000002U
#define     CTN_OBJ_PROP_MASK_ENDTIME                   0x00000004U
#define     CTN_OBJ_PROP_MASK_ORIGINATOR_NAME           0x00000008U
#define     CTN_OBJ_PROP_MASK_ORIGINATOR_ADDRS          0x00000010U
#define     CTN_OBJ_PROP_MASK_PRIORITY                  0x00000020U
#define     CTN_OBJ_PROP_MASK_PSTATUS                   0x00000040U
#define     CTN_OBJ_PROP_MASK_ALARMSTATUS               0x00000080U
#define     CTN_OBJ_PROP_MASK_SENDSTATUS                0x00000100U
#define     CTN_OBJ_PROP_MASK_RECURRENT                 0x00000200U

#define     CTN_OBJ_PROP_MASK_UPDATE                    0x00000400U
#define     CTN_OBJ_PROP_MASK_CALTYPE                   0x00000800U
#define     CTN_OBJ_PROP_MASK_STARTTIME                 0x00001000U
#define     CTN_OBJ_PROP_MASK_SIZE                      0x00002000U
#define     CTN_OBJ_PROP_MASK_HANDLE                    0x00004000U

#define     MAX_CTN_ATTR_SIZE                           1024U

/* ---- CTN object attribute value length defines -------  */

/**
 * Max CTN update time attribute length.
 */
#define    MAX_CTN_ATTR_UPDATE_TIME_LEN                      24U

/* Max CTN calender type length */
#define    MAX_CTN_ATTR_CAL_TYPE_LEN                         8U

/**
 * Max. CTN originator name length.
 *
 * Note:
 * From the CTN spec. this attrbute can be 256 bytes.
 */
#define    MAX_CTN_ATTR_ORIGIN_NAME_LEN                      64U

/**
 * Max. CTN originator address length.
 *
 * Note:
 * From the CTN spec. this attrbute can be 256 bytes.
 */
#define    MAX_CTN_ATTR_ORIGIN_ADDR_LEN                      64U

/**
 * Max. CTN summary length
 *
 * Note:
 * From the CTN spec. this attrbute can be 256 bytes.
 */
#define    MAX_CTN_ATTR_SUMMARY_LEN                          256U

/* Max. CTN calender entry propert length */
#define    MAX_CTN_ATTR_CAL_ENTRY_PRIORITY_LEN               8U

/* Max. CTN calender entry status length */
#define    MAX_CTN_ATTR_CAL_ENTRY_STATUS_LEN                 24U

/* Max. CTN calender entry alarm status length */
#define    MAX_CTN_ATTR_CAL_ENTRY_ALARM_STATUS               10U

/* Max. CTN recurrent property length */
#define    MAX_CTN_ATTR_RECURRENT_PROPRTY_LEN                4U

/* Max CTN send status property length */
#define    MAX_CTN_ATTR_SEND_STATUS_PROPRTY_LEN              4U

/* Max CTN server time offset length */
#define    MAX_CTN_ATTR_CSE_TIME_OFFSET                      24U


/* --------------------------------------------- Structures/Data Types */
typedef struct _CTN_OBJ_ATTR_PL
{
    /* objec type */
    UCHAR   obj_type;

    /**
     * object handle
     */
    CHAR    handle[MAX_CTN_OBJ_HANDLE_LEN];

    /**
     * latest update timestamp
     */
    CHAR    update[MAX_CTN_ATTR_UPDATE_TIME_LEN];

    /**
     * calendar type
     */
    CHAR    cal_type[MAX_CTN_ATTR_CAL_TYPE_LEN];

    /**
     * Summary
     */
    CHAR    summary[MAX_CTN_ATTR_SUMMARY_LEN];

    /* start time */
    CHAR    start_time[MAX_CTN_ATTR_UPDATE_TIME_LEN];

    /* end time */
    CHAR    end_time[MAX_CTN_ATTR_UPDATE_TIME_LEN];

    /* originator name */
    CHAR    org_name[MAX_CTN_ATTR_ORIGIN_NAME_LEN];

    /* originator address */
    CHAR    org_addr[MAX_CTN_ATTR_ORIGIN_ADDR_LEN];

    /* priority */
    CHAR   priority[MAX_CTN_ATTR_CAL_ENTRY_PRIORITY_LEN];

    /* pstatus property */
    CHAR   pstaus[MAX_CTN_ATTR_CAL_ENTRY_STATUS_LEN];

    /* alarm property */
    CHAR   alarm_status[MAX_CTN_ATTR_CAL_ENTRY_ALARM_STATUS];

    /* recurrent property */
    CHAR   recurrent[MAX_CTN_ATTR_RECURRENT_PROPRTY_LEN];

    /* send status property */
    CHAR   sentstatus[MAX_CTN_ATTR_SEND_STATUS_PROPRTY_LEN];

    /* object size in bytes */
    UINT16  size;

    /*  object parameter mask */
    UINT32   obj_param_mask;
} CTN_OBJ_ATTR_PL;

/**
 * Structure to hold ctn_get_listing response info.
 */
typedef struct _CTN_OBJ_LISTING_INFO_
{
    /* No. of objects in listing object */
    UINT16 listing_count;

    /* CSE time */
    UCHAR   cse_time_offset[MAX_CTN_ATTR_CSE_TIME_OFFSET];
}CTN_OBJ_LISTING_INFO;

typedef struct _CTN_OBJ_INFO_
{
    /* name */
    CHAR       name[MAX_CTN_OBJ_NAME_LEN];

    /* name len */
    UCHAR       name_len;

    /* handle */
    CHAR       handle[MAX_CTN_OBJ_HANDLE_LEN];

    /* hanlde len */
    UCHAR       handle_len;
}CTN_OBJ_INFO;

/**
 * Structure to hold ctn object name, handle in calendar, notes, tasks
 */
typedef struct _CTN_OBJ_LIST_INFO_
{
    /* object name */
    CTN_OBJ_INFO   obj_details[MAX_CTN_OBJ_COUNT];

    /* num of objects */
    UCHAR   num_objects;
}CTN_OBJ_LIST_INFO;

/* --------------------------------------------- Macros */
#ifndef CTN_PL_NO_DEBUG
#define CTN_PL_ERR(...)         BT_debug_error(BT_MODULE_ID_CTN_PL, __VA_ARGS__)
#else  /* CTN_PL_NO_DEBUG */
#define CTN_PL_ERR              BT_debug_null
#endif /* CTN_PL_NO_DEBUG */

#ifdef CTN_PL_DEBUG

#define CTN_PL_TRC(...)         BT_debug_trace(BT_MODULE_ID_CTN_PL, __VA_ARGS__)
#define CTN_PL_INF(...)         BT_debug_info(BT_MODULE_ID_CTN_PL, __VA_ARGS__)

#define CTN_PL_debug_dump_bytes(data, datalen) BT_debug_dump_bytes(BT_MODULE_ID_CTN_PL, (data), (datalen))

#else /* CTN_PL_DEBUG */

#define CTN_PL_TRC              BT_debug_null
#define CTN_PL_INF              BT_debug_null

#define CTN_PL_debug_dump_bytes(data, datalen)

#endif /* CTN_PL_DEBUG */

/* --------------------------------------------- Internal Functions */
/* This function read attribues of an CTN object */
API_RESULT ctn_get_object_attributes_pl
           (
               /* IN */  UCHAR * idir,
               /* IN */  CHAR * object_name,
               /* OUT */ CTN_OBJ_ATTR_PL * attr
           );

/* compares timestamp values */
INT16 ctn_compare_timestamp_pl
      (
          /* IN */  UCHAR *t1,
          /* IN */  UCHAR  t1_len,
          /* IN */  UCHAR *t2,
          /* IN */  UCHAR  t2_len
      );

CHAR  ctn_nibble_to_char(UCHAR nibble);

/* --------------------------------------------- API Declarations */

/**
 *  \fn BT_ctn_build_object_listing_pl
 *
 *  \brief To build CTN object listing object with filters
 *
 *  \Description
 *
 *
 *  \param [in] dir_entry
 *  \param [in] listingfile
 *  \param [in] appl_param
 *  \param [out] listing_obj_info
 *
 *  \return None
 */
API_RESULT BT_ctn_build_object_listing_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *listingfile,
               /* IN */  CTN_APPL_PARAMS      *appl_param,
               /* OUT */ CTN_OBJ_LISTING_INFO *listing_obj_info
           );

/**
 *  \fn BT_ctn_get_object_list
 *
 *  \brief To get ctn objects details in given directory
 *
 *  \Description
 *
 *  \param [in] dir_entry
 *  \param [in] obj_list_info
 *
 *  \return None
 */
API_RESULT BT_ctn_get_object_list
           (
               /* IN */  UCHAR   *dir_entry,
               /* OUT */ CTN_OBJ_LIST_INFO    *obj_list_info
           );

/**
 *  \fn BT_ctn_convert_bytes_to_chars
 *
 *  \brief To convert byte stream to character format
 *
 *  \Description
 *
 *
 *  \param [in] byte_stream
 *  \param [in] byte_stream_len
 *  \param [out] chars
 *  \param [out] chars_len
 *
 *  \return None
 */
API_RESULT BT_ctn_convert_bytes_to_chars
           (
               /* IN */  UCHAR    *byte_stream,
               /* IN */  UCHAR     byte_stream_len,
               /* OUT */ UCHAR    *chrs,
               /* OUT */ UCHAR    *chrs_len
           );

/**
 *  \fn BT_ctn_update_object_handle_pl
 *
 *  \brief To add CTN handle property to the object
 *
 *  \Description
 *
 *
 *  \param dir_entry
 *  \param src_file_name
 *  \param ctn_handle
 *  \param ctn_handle_len
 *
 *  \return None
 */
API_RESULT BT_ctn_update_object_handle_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *src_file_name,
               /* IN */  UCHAR   *ctn_handle,
               /* IN */  UCHAR    ctn_handle_len
           );

/**
 *  \fn BT_ctn_build_event_file_pl
 *
 *  \brief
 *      This is function will build CTN event object
 *
 *  \Description
 *
 *
 *  \param [in] dir_entry
 *  \param [in] event_file
 *  \param [in] event_type
 *  \param [in] handle
 *  \param [in] cal_type
 *
 *  \return None
 */
API_RESULT BT_ctn_build_event_file_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *event_file,
               /* IN */  UCHAR   *event_type,
               /* IN */  UCHAR   *handle,
               /* IN */  UCHAR   *cal_type
           );

#endif /* _H_CTN_PL_ */
