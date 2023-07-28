
/**
 *  \file GA_ccp.h
 *
 *  \brief This file defines the GA Call Control Profile(CCP) Server Entity(SE)
 *   Data Structures
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPl_CCP_
#define _H_APPl_CCP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_ccp_se_api.h"
#include "GA_ccp_ce_api.h"

#include "GA_bearer_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/**
 * \addtogroup bt_ga_profiles GA_Profiles
 * \{
 */

 /**
 * TBS characteristics value maximum size.
 * Update below defines based on the application requirements.
 */
#define MAX_TBS_BRR_PROVIDER_NAME_SIZE              64
#define MAX_TBS_BRR_UCI_SIZE                        64
#define MAX_TBS_BRR_URI_SUPPORTED_LIST_SIZE         64
#define MAX_TBS_URI_LEN                             64
#define MAX_TBS_CALLER_NAME_LEN                     64


/* --------------------------------------------- Data types /Structures */
/**
 * CCP Call URI
 */
typedef struct _CCP_URI
{
    /**
     * URI Value.
     */
    UCHAR    uri[MAX_TBS_URI_LEN];
}CCP_URI;

/**
 * TBS Bearer Current Call Details
 */
typedef struct _CCP_BRR_LIST_CUR_CALL
{
    /**
     * List Item Length
     */
    UCHAR   list_item_length;

    /**
     * Call Index
     */
    UCHAR   call_index;

    /**
     * Call State
     */
    UCHAR   call_state;

    /**
     * Call Falgs
     */
    UCHAR   call_flags;

    /**
     * URI
     */
    UCHAR    uri[MAX_TBS_URI_LEN];

    /**
     * URI Length
     */
    UCHAR    uri_len;
}CCP_BRR_LIST_CUR_CALL;

/**
 * TBS Call State
 */
typedef CCP_BRR_LIST_CUR_CALL CCP_CALL_STATE;

/**
 * TBS Caller Id
 */
typedef struct _CCP_CALLER_ID
{
    /**
     * Call index
     */
    UCHAR   call_indx;

    /**
     * URI
     */
    UCHAR    uri[MAX_TBS_URI_LEN];

    /**
     * URI Length
     */
    UCHAR    uri_len;

}CCP_CALLER_ID;

/**
 * TBS Termination Reason
 */
typedef struct _CCP_TERMIN_REASON
{
    /**
     * Call index
     */
    UCHAR   call_indx;

    /**
     * Reason
     */
    UCHAR   reason;

}CCP_TERMIN_REASON;

/**
 * TBS Friendly Name
 */
typedef struct _CCP_CALLER_FRIENDLY_NAME
{
    /**
     * Call index
     */
    UCHAR   call_indx;

    /**
     * Friendly Name
     */
    UCHAR    name[MAX_TBS_CALLER_NAME_LEN];

    /**
     * Friendly Name length
     */
    UCHAR    name_len;

}CCP_CALLER_FRIENDLY_NAME;

/**
 * GTBS/TBS Characteristics Values
 */
typedef struct _CCP_PARAMS
{
    /* Bearer Provider Name */
    UCHAR bearer_pname[MAX_TBS_BRR_PROVIDER_NAME_SIZE];

    UCHAR bearer_pname_len;

    /* Bearer UCI */
    UCHAR bearer_uci[MAX_TBS_BRR_UCI_SIZE];

    UCHAR bearer_uci_len;

    /* Bearer Technology */
    UCHAR brr_tech;

    /* Bearer URI Prefixes Supported List */
    UCHAR brr_uri_pfix_supp_list[MAX_TBS_BRR_URI_SUPPORTED_LIST_SIZE];

    /* Bearer URI Prefixes Supported List length */
    UCHAR brr_uri_pfix_supp_list_len;

    /* Bearer Signal Strength */
    UCHAR brr_sig_strength;

    /* Bearer Signal Strength Reporting Interval */
    UCHAR brr_sig_strength_reptg_intval;

    /* Bearer List Current Calls */
    CCP_BRR_LIST_CUR_CALL *brr_list_curr_list;

    /* Content Control ID (CCID) */
    UINT16 content_control_id;

    /* Supported Features */
    UINT16 status_flags;

    /* Incoming Call Target Caller ID */
    CCP_CALLER_ID *incmg_caller_trgt_id;

    /* Call State */
    CCP_CALL_STATE *call_state;

    /**
     * Optional Control Point Opcodes
     */
    UINT16 opt_cp_opcodes;

    /* Termination Reason */
    CCP_TERMIN_REASON *term_reason;

    /* Incoming Caller Info */
    CCP_CALLER_ID *incmg_caller_info;

    /**
     * Caller Friendly Name
     */
    CCP_CALLER_FRIENDLY_NAME  *frndly_name;
}CCP_PARAMS;

/**
 *  \defgroup ccp_structures Data Structures
 *  \{
 *  This section lists the various data structures for use
 *  by the module.
 */

/**
 * TBS Call URI
 */
typedef struct _TBS_URI
{
    /**
     * URI Value.
     */
    UCHAR    uri[MAX_TBS_URI_LEN];
}TBS_URI;

/**
 * TBS Bearer Current Call Details
 */
typedef struct _TBS_BRR_LIST_CUR_CALL_
{
    /**
     * List Item Length
     */
    UCHAR   list_item_length;

    /**
     * Call Index
     */
    UCHAR   call_index;

    /**
     * Call State
     */
    UCHAR   call_state;

    /**
     * Call Flags
     */
    UCHAR   call_flags;

    /**
     * URI
     */
    UCHAR    uri[MAX_TBS_URI_LEN];

    /**
     * URI Length
     */
    UCHAR    uri_len;
}TBS_BRR_LIST_CUR_CALL;

/**
 * TBS Call State
 */
typedef TBS_BRR_LIST_CUR_CALL TBS_CALL_STATE;

/**
 * TBS Caller Id
 */
typedef struct _TBS_CALLER_ID_
{
    /**
     * Call index
     */
    UCHAR   call_indx;

    /**
     * URI
     */
    UCHAR    uri[MAX_TBS_URI_LEN];

    /**
     * URI Length
     */
    UCHAR    uri_len;

}TBS_CALLER_ID;

/**
 * TBS Termination Reason
 */
typedef struct _TBS_TERMIN_REASON
{
    /**
     * Call index
     */
    UCHAR   call_indx;

    /**
     * Reason
     */
    UCHAR   reason;

}TBS_TERMIN_REASON;

/**
 * TBS Friendly Name
 */
typedef struct _TBS_CALLER_FRIENDLY_NAME
{
    /**
     * Call index
     */
    UCHAR   call_indx;

    /**
     * Friendly Name
     */
    UCHAR    name[MAX_TBS_CALLER_NAME_LEN];

    /**
     * Friendly Name length
     */
    UCHAR    name_len;

}TBS_CALLER_FRIENDLY_NAME;

/**
 * GTBS/TBS Characteristics Values
 */
typedef struct _TBS_PARAMS
{
    /* Bearer Provider Name */
    UCHAR bearer_pname[MAX_TBS_BRR_PROVIDER_NAME_SIZE];

    UCHAR bearer_pname_len;

    /* Bearer UCI */
    UCHAR bearer_uci[MAX_TBS_BRR_UCI_SIZE];

    UCHAR bearer_uci_len;

    /* Bearer Technology */
    UCHAR brr_tech;

    /* Bearer URI Prefixes Supported List */
    UCHAR brr_uri_pfix_supp_list[MAX_TBS_BRR_URI_SUPPORTED_LIST_SIZE];

    /* Bearer URI Prefixes Supported List length */
    UCHAR brr_uri_pfix_supp_list_len;

    /* Bearer Signal Strength */
    UCHAR brr_sig_strength;

    /* Bearer Signal Strength Reporting Interval */
    UCHAR brr_sig_strength_reptg_intval;

    /* Bearer List Current Calls */
    TBS_BRR_LIST_CUR_CALL *brr_list_curr_list;

    /* Content Control ID (CCID) */
    UCHAR content_control_id;

    /* Supported Features */
    UINT16 status_flags;

    /* Incoming Call Target Caller ID */
    TBS_CALLER_ID *incmg_caller_trgt_id;

    /* Call State */
    TBS_CALL_STATE *call_state;

    /**
     * Optional Control Point Opcodes
     */
    UINT16 opt_cp_opcodes;

    /* Termination Reason */
    TBS_TERMIN_REASON *term_reason;

    /* Incoming Caller Info */
    TBS_CALLER_ID *incmg_caller_info;

    /**
     * Caller Friendly Name
     */
    TBS_CALLER_FRIENDLY_NAME  *frndly_name;
}TBS_PARAMS;

/** \} */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

/** \} */
/** \} */

#endif /* _H_APPl_CCP_ */

