
/**
 *  \file ga_brr_extern.h
 *
 *  \brief This file defines the Generic Audio Beater Extern Defines.
 */

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/

#ifndef _H_GA_BRR_EXTERN_
#define _H_GA_BRR_EXTERN_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */
/**
 * Common Bearer Callback to GA Core.
 */
extern GA_BRR_CB          ga_brr_cb;

/**
 * GA Bearer Profile Contexts
 */
extern GA_BRR_PROFILE_CNTX     ga_brr_profile_cntx[GA_BRR_MAX_PROFILE_CNTX];

/**
 * GA Bearer Service Contexts
 */
extern GA_BRR_SERVICE_CNTX     ga_brr_srvc_cntx[GA_BRR_MAX_SERVICE_CNTX];

/**
 * GA Bearer Include Service Contexts
 */
extern GA_BRR_INCLUDE_SERVICE_CNTX ga_brr_in_srvc_cntx[GA_BRR_MAX_INCLUDE_SERVICE_CNTX];

/**
 * GA Bearer Service UUID Contexts
 */
extern GA_SERVICE_UUID_CNTX ga_brr_srvc_uuid_cntx[GA_BRR_MAX_SERVICE_UUID_CNTX];

/**
 * GA Bearer Characteristic UUID Contexts
 */
extern GA_CHAR_UUID_CNTX ga_brr_char_uuid_cntx[GA_BRR_MAX_CHAR_UUID_CNTX];

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */


#endif /* _H_GA_BRR_EXTERN_ */
