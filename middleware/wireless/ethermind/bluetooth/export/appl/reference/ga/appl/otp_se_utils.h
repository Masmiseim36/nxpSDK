
/**
 *  \file otp_se_utils.h
 *
 *  \brief This file implements utilities that a OTP requires as a OTP Server
 *         role.
 */

 /**
  *  Copyright (C) 2020. Mindtree Ltd.
  *  All rights reserved.
  */

#ifndef _H_OTP_SE_UTILS_
#define _H_OTP_SE_UTILS_
/* --------------------------------------------- Header File Inclusion */
#include "appl_utils.h"
/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */
#define OTP_OBJ_FILTER_SETTING_LEN                 0x01
#define OTP_OBJ_TYPE_LEN                           0X02
#define OTP_OBJ_ID_LEN                             0x06
#define OTP_OBJ_SIZE_LEN                           0x04
#define OTP_OBJ_DATE_LEN                           0x07
#define OTP_OBJ_FILE_NAME_LEN                      0x1E
#define OTP_MAX_OBJ_NAME_LEN                       25

/* --------------------------------------------- APIs */
typedef struct _OTP_SE_OBJ_METADATA_
{
    /* Object Type */
    UINT16  obj_type;

    /* Object Size */
    UCHAR   obj_curr_size[OTP_OBJ_SIZE_LEN];

    /* Object Size */
    UCHAR   obj_alloc_size[OTP_OBJ_SIZE_LEN];

    /* Object First Created */
    UCHAR   obj_frst_crtd[OTP_OBJ_DATE_LEN];

    /* Object Last Modified */
    UCHAR   obj_lst_mod[OTP_OBJ_DATE_LEN];

    /* Object Properties */
    UINT32  obj_prpty;

    /* Object ID */
    UINT8   obj_id[OTP_OBJ_ID_LEN];

    /* Object Name Length */
    UCHAR   obj_name_len;

    /* Object Name Pointer */
    DECL_CONST CHAR * obj_name;

    /* Object Data Length */
    UINT16  obj_datalen;

    /* Object Data Pointer */
    UCHAR * obj_data;
} OTP_SE_OBJ_METADATA;

#endif /* _H_OTP_SE_UTILS_ */
