/**
 *  \file pbap_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_PBAP_PL_
#define _H_PBAP_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_pbap_api.h"
#include "BT_fops.h"

/* --------------------------------------------- Global Definitions */
/* Max Folder Name length */
#define PBAP_FOLDER_NAME_LEN                100

/* Reference folder for PBAP Client objects */
#define   APPL_PBAP_FOLDER_REFERENCE \
          BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "pbap")

/* Base folder for PBAP objects */
#define PBAP_ROOT_FOLDER_BASE \
    BT_FOPS_PATH_JOIN(BT_FOPS_BASE,"data" BT_FOPS_PATH_SEP "pbap" BT_FOPS_PATH_SEP "phonebook")

#define PBAP_PB_LISTING_FILE_NAME           "vcard_listing.xml"
#define PBAP_PB_NAME                        "pb.vcf"
#define PBAP_VCARD_NAME                     "vcard.obj"
#define PBAP_ROOT_FOLDER                    "root"

#define MAX_VCARDS                          20
#define MAX_VCARD_PROPERTIES                20
#define MAX_VCARD_PROP_PARAMS               4

#define MAX_VCARD_NAME_LEN                  64
#define MAX_VCARD_TEL_LEN                   20
#define MAX_VCARD_UID_LEN                   32

#define MAX_PBAP_FILE_OBJ_NAME_LEN          128

/* --------------------------------------------- Structures/Data Types */

/* vCard property parameters information */
typedef struct _PBAP_VCARD_PROP_PARAM_INFO
{
    /* vCard property parameter name */
    UCHAR    * param_name;

    /* vCard property parameter name length */
    UINT16     param_name_len;

    /* vCard property parameter value */
    UCHAR    * param_value;

    /* vCard property parameter value length */
    UINT16     param_value_len;

}PBAP_VCARD_PROP_PARAM_INFO;

/**
 * vCard Propterty Information: Property Parameters, Property Value
 */
typedef struct _PBAP_VCARD_INFO
{
    UCHAR   * group_name;

    UCHAR     group_name_len;

    UINT16    prop_name_id;

    UCHAR   * prop_name;

    UINT16    prop_name_len;

    PBAP_VCARD_PROP_PARAM_INFO parm_info[MAX_VCARD_PROP_PARAMS];

    UCHAR     num_params;

    UCHAR   * prop_val;

    UINT16    prop_val_len;

    UINT32    prop_bit_mask;

}PBAP_VCARD_INFO;


typedef struct _PBAP_VCARD_OBJECT
{
    /* vCard Name */
    UCHAR   vcard_file_name[MAX_VCARD_NAME_LEN];

    /* No. of properties in the vCard */
    UCHAR   num_properties;

    /* Property bit mask for filtering vcard_listing request */
    UINT32  prop_bit_mask;

    /* Flag FALSE/TRUE after filtering */
    UCHAR   status;

    /* vCard Details */
    PBAP_VCARD_INFO  info[MAX_VCARD_PROPERTIES];

}PBAP_VCARD_OBJECT;


typedef struct _PBAP_VCARD_LIST
{
    /* vCard Name */
    UCHAR   vcard_file_name[MAX_VCARD_NAME_LEN];

    UCHAR   vcard_file_name_len;

    /* vCard Name property */
    UCHAR   n[MAX_VCARD_NAME_LEN];

    UCHAR   n_len;

    /* vCard tel property */
    UCHAR   tel[MAX_VCARD_TEL_LEN];

    UCHAR   tel_len;

    /* vCard x-bt-uid property */
    UCHAR   vcard_bt_uid[MAX_VCARD_UID_LEN+1];

    UCHAR   vcard_bt_uid_len;

    /* Property bit mask for filtering vcard_listing request */
    UINT32  prop_bit_mask;

    /* Flag FALSE/TRUE after filtering */
    UCHAR   listing_status;

}PBAP_VCARD_LIST;

/* --------------------------------------------- Macros */
#ifdef PBAP_DEBUG
#define PBAP_PL_TRC(...)                 BT_debug_trace(BT_MODULE_ID_PBAP, __VA_ARGS__)
#else /* PBAP_DEBUG */
#define PBAP_PL_TRC                      BT_debug_null
#endif /* PBAP_DEBUG */

/* vCard Property Bit Mask Defines */
#define     VCARD_PROP_BIT_MASK_VERSION                     0x00000001
#define     VCARD_PROP_BIT_MASK_FN                          0x00000002
#define     VCARD_PROP_BIT_MASK_N                           0x00000004
#define     VCARD_PROP_BIT_MASK_PHOTO                       0x00000008
#define     VCARD_PROP_BIT_MASK_BDAY                        0x00000010
#define     VCARD_PROP_BIT_MASK_ADR                         0x00000020
#define     VCARD_PROP_BIT_MASK_LABEL                       0x00000040
#define     VCARD_PROP_BIT_MASK_TEL                         0x00000080
#define     VCARD_PROP_BIT_MASK_EMAIL                       0x00000100
#define     VCARD_PROP_BIT_MASK_MAILER                      0x00000200
#define     VCARD_PROP_BIT_MASK_TZ                          0x00000400
#define     VCARD_PROP_BIT_MASK_GEO                         0x00000800
#define     VCARD_PROP_BIT_MASK_TITLE                       0x00001000
#define     VCARD_PROP_BIT_MASK_ROLE                        0x00002000
#define     VCARD_PROP_BIT_MASK_LOGO                        0x00004000
#define     VCARD_PROP_BIT_MASK_AGENT                       0x00008000
#define     VCARD_PROP_BIT_MASK_ORG                         0x00010000
#define     VCARD_PROP_BIT_MASK_NOTE                        0x00020000
#define     VCARD_PROP_BIT_MASK_REV                         0x00040000
#define     VCARD_PROP_BIT_MASK_SOUND                       0x00080000
#define     VCARD_PROP_BIT_MASK_URL                         0x00100000
#define     VCARD_PROP_BIT_MASK_UID                         0x00200000
#define     VCARD_PROP_BIT_MASK_KEY                         0x00400000
#define     VCARD_PROP_BIT_MASK_NICKNAME                    0x00800000
#define     VCARD_PROP_BIT_MASK_CATEGORIES                  0x01000000
#define     VCARD_PROP_BIT_MASK_PROID                       0x02000000
#define     VCARD_PROP_BIT_MASK_CLASS                       0x04000000
#define     VCARD_PROP_BIT_MASK_SORT_STRING                 0x08000000
#define     VCARD_PROP_BIT_MASK_X_IRMC_CALL_DATETIME        0x10000000
#define     VCARD_PROP_BIT_MASK_X_BT_SPEEDDIALKEY           0x20000000
#define     VCARD_PROP_BIT_MASK_X_BT_UCI                    0x40000000
#define     VCARD_PROP_BIT_MASK_X_BT_UID                    0x80000000

/* --------------------------------------------- Internal Functions */

void get_vcard_handle(UCHAR *vcard, UINT16 *handle);

void pbap_sort_pb_list
        (
            PBAP_VCARD_LIST   * vcards,
            UINT16              vcard_count
        );

/* --------------------------------------------- API Declarations */

API_RESULT BT_pbap_build_xml_vcard_listing_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *listingfile,
               /* IN */  PBAP_VCARD_LIST   * vcards,
               /* IN */  UINT16  num_vcards,
               /* IN */  PBAP_APPL_PARAMS *appl_params
           );

API_RESULT BT_pbap_get_vcard_list_pl
           (
               /* IN  */  UCHAR             * dir_entry,
               /* OUT */  PBAP_VCARD_LIST   * vcards,
               /* OUT */  UINT16            * num_vcards
           );

API_RESULT BT_pbap_build_vcard_pl
           (
                /* IN */    PBAP_VCARD_OBJECT   * vcard_info,
                /* IN */    UINT32 vcard_prop_filter,
                /* IN */    UCHAR  * in_vcard_data,
                /* IN */    UINT16   in_vcard_data_len,
                /* OUT */   UCHAR  * out_vcard_data,
                /* OUT */   UINT16 * out_vcard_data_len
           );

#endif /* _H_PBAP_PL_ */

