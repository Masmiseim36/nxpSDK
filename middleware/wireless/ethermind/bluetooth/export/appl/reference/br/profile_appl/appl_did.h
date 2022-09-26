
/**
 *  \file appl_did.h
 *
 *  Header File for DID Client/Server Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_DID_
#define _H_APPL_DID_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_dbase_api.h"

#include "db_gen.h"

/* ----------------------------------------- Global Definitions */

/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Macros */
/* DID Server APIs - implemented as macros */
#define BT_did_get_record_handle(instance, record_handle)                     \
{                                                                             \
    (record_handle) = 0xFFFFFFFFU;                                             \
    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_record_handle (DB_RECORD_DID, (instance), &(record_handle)); \
}

#define BT_did_activate(did_record_handle) \
        BT_dbase_activate_record(did_record_handle)

#define BT_did_inactivate(did_record_handle) \
        BT_dbase_inactivate_record(did_record_handle)

#define BT_did_set_specification_id(did_record_handle, specification_id) \
{                                                                        \
        UCHAR attrib_val[3];                                             \
        attrib_val[0] = 0x09; /* UINT16 value */                         \
        attrib_val[1] = (UCHAR)((specification_id) >> 8);                \
        attrib_val[2] = (UCHAR)(specification_id);                       \
        (BT_IGNORE_RETURN_VALUE) BT_dbase_update_attr_value              \
        (                                                                \
            (did_record_handle),                                         \
            0x0200,                                                      \
            attrib_val,                                                  \
            0x03                                                         \
        );                                                               \
}

#define BT_did_set_vendor_id(did_record_handle, vendor_id)               \
{                                                                        \
        UCHAR attrib_val[3];                                             \
        attrib_val[0] = 0x09; /* UINT16 value */                         \
        attrib_val[1] = (UCHAR)((vendor_id) >> 8);                       \
        attrib_val[2] = (UCHAR)(vendor_id);                              \
        (BT_IGNORE_RETURN_VALUE) BT_dbase_update_attr_value              \
        (                                                                \
            (did_record_handle),                                         \
            0x0201,                                                      \
            attrib_val,                                                  \
            0x03                                                         \
        );                                                               \
}

#define BT_did_set_product_id(did_record_handle, product_id)             \
{                                                                        \
        UCHAR attrib_val[3];                                             \
        attrib_val[0] = 0x09; /* UINT16 value */                         \
        attrib_val[1] = (UCHAR)((product_id) >> 8);                      \
        attrib_val[2] = (UCHAR)(product_id);                             \
        (BT_IGNORE_RETURN_VALUE) BT_dbase_update_attr_value              \
        (                                                                \
            (did_record_handle),                                         \
            0x0202,                                                      \
            attrib_val,                                                  \
            0x03                                                         \
        );                                                               \
}

#define BT_did_set_version(did_record_handle, version)                   \
{                                                                        \
        UCHAR attrib_val[3];                                             \
        attrib_val[0] = 0x09; /* UINT16 value */                         \
        attrib_val[1] = (UCHAR)((version) >> 8);                         \
        attrib_val[2] = (UCHAR)(version);                                \
        (BT_IGNORE_RETURN_VALUE) BT_dbase_update_attr_value              \
        (                                                                \
            (did_record_handle),                                         \
            0x0203,                                                      \
            attrib_val,                                                  \
            0x03                                                         \
        );                                                               \
}

#define BT_did_set_primary_record_attr(did_record_handle, is_primary_record) \
{                                                                            \
        UCHAR attrib_val[2];                                                 \
        attrib_val[0] = 0x28; /* BOOLEAN value */                            \
        attrib_val[1] = (UCHAR)(is_primary_record);                          \
        (BT_IGNORE_RETURN_VALUE) BT_dbase_update_attr_value                  \
        (                                                                    \
            (did_record_handle),                                             \
            0x0204,                                                          \
            attrib_val,                                                      \
            0x02                                                             \
        );                                                                   \
}

#define BT_did_set_vendor_id_source(did_record_handle, vendor_id_source) \
{                                                                        \
        UCHAR attrib_val[3];                                             \
        attrib_val[0] = 0x09; /* UINT16 value */                         \
        attrib_val[1] = (UCHAR)((vendor_id_source) >> 8);                \
        attrib_val[2] = (UCHAR)(vendor_id_source);                       \
        (BT_IGNORE_RETURN_VALUE) BT_dbase_update_attr_value              \
        (                                                                \
            (did_record_handle),                                         \
            0x0205,                                                      \
            attrib_val,                                                  \
            0x03                                                         \
        );                                                               \
}

#define BT_did_set_client_executable_url(did_record_handle, url)         \
{                                                                        \
        UCHAR *attrib_val;                                               \
        UCHAR  attrib_len;                                               \
        attrib_len = (UCHAR)(BT_str_len(url) + 3); /* header + len + NULL */      \
        attrib_val = (UCHAR *)BT_alloc_mem(attrib_len);                  \
        if(NULL != attrib_val)                                           \
        {                                                                \
            attrib_val[0] = 0x45; /* URL Type */                         \
            attrib_val[1] = (UCHAR)(attrib_len - 2);                     \
            BT_mem_copy(&attrib_val[2], url, attrib_len - 3);            \
            attrib_val[attrib_len - 1] = 0x00;                           \
            (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value          \
            (                                                            \
                (did_record_handle),                                     \
                0x000B,                                                  \
                attrib_val,                                              \
                attrib_len                                               \
            );                                                           \
       }                                                                 \
}

#define BT_did_set_documentation_url(did_record_handle, url)             \
{                                                                        \
        UCHAR *attrib_val;                                               \
        UCHAR  attrib_len;                                               \
        attrib_len = (UCHAR)(BT_str_len(url) + 3); /* header + len + NULL */      \
        attrib_val = (UCHAR *)BT_alloc_mem(attrib_len);                  \
        if(NULL != attrib_val)                                           \
        {                                                                \
            attrib_val[0] = 0x45; /* URL Type */                         \
            attrib_val[1] = (UCHAR)(attrib_len - 2);                     \
            BT_mem_copy(&attrib_val[2], url, attrib_len - 3);            \
            attrib_val[attrib_len - 1] = 0x00;                           \
            (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value          \
            (                                                            \
                (did_record_handle),                                     \
                0x000A,                                                  \
                attrib_val,                                              \
                attrib_len                                               \
            );                                                           \
       }                                                                 \
}

#define BT_did_set_service_description(did_record_handle, service_desc)  \
{                                                                        \
        UCHAR *attrib_val;                                               \
        UCHAR  attrib_len;                                               \
        attrib_len = (UCHAR)(BT_str_len(service_desc) + 3); /* header + len + NULL */ \
        attrib_val = (UCHAR *)BT_alloc_mem(attrib_len);                  \
        if(NULL != attrib_val)                                           \
        {                                                                \
            attrib_val[0] = 0x25; /* String Type */                      \
            attrib_val[1] = (UCHAR)(attrib_len - 2);                     \
            BT_mem_copy(&attrib_val[2], (service_desc), attrib_len - 3); \
            attrib_val[attrib_len - 1] = 0x00;                           \
            (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value          \
            (                                                            \
                (did_record_handle),                                     \
                0x0101,                                                  \
                attrib_val,                                              \
                attrib_len                                               \
            );                                                           \
       }                                                                 \
}


/* ----------------------------------------- Function Declarations */
void main_did_client_operations(void);
void main_did_server_operations (void);

#endif /* _H_APPL_DID_ */

