
/**
 *  \file ga_brr_utils.h
 *
 *  \brief This file defines the Generic Audio Bearer Utility Interface - includes
 *  Data Structures and Methods.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_BRR_UTILS_
#define _H_GA_BRR_UTILS_

/* --------------------------------------------- Header File Inclusion */
#include "GA_bearer_api.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

/**
 *  \brief To Allocate a context for GA Profile.
 *
 *  \par Description:
 *       This function enables to allocate an context for GA Profile.
 *
 *  \param [out] pid
 *         Profile context ID.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT ga_brr_allocate_profile_cntx
          (
              /* OUT */ GA_BRR_PRF_HANDLE * pid
          );

/**
 *  \brief To Free a context of a GA Profile.
 *
 *  \param [in] pid
 *         Profile context ID.
 *
 *  \par Description:
 *       This function enables to free a context for GA Profile.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT ga_brr_free_profile_cntx
          (
              /* IN */ GA_BRR_PRF_HANDLE pid
          );

GA_RESULT ga_brr_search_service_cntx
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ UCHAR           only_ps,
              /* OUT */ UCHAR           * id
          );
GA_RESULT ga_brr_allocate_service_cntx
          (
              /* OUT */ UCHAR           * id
          );

GA_RESULT ga_brr_free_service_cntx
          (
              /* IN */ UCHAR id
          );

GA_RESULT ga_brr_fetch_sid_of_cid
          (
              /* IN  */ GA_BRR_CHR_INST  cid,
              /* OUT */ GA_BRR_SVC_INST  * sid
          );

GA_RESULT ga_brr_allocate_include_service_cntx
          (
              /* OUT */ UCHAR * id
          );
GA_RESULT ga_brr_search_include_service_cntx
          (
              /* IN  */ GA_BRR_SVC_INST isid,
              /* OUT */ UCHAR           * id
          );
GA_RESULT ga_brr_free_include_service_cntx
          (
              /* IN */ GA_BRR_SVC_INST sid
          );

GA_RESULT ga_brr_search_serv_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID s_uuid,
              /* OUT */ UCHAR       * id
          );

GA_RESULT ga_brr_allocate_serv_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID s_uuid,
              /* OUT */ UCHAR       * id
          );

GA_RESULT ga_brr_free_serv_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID s_uuid
          );

GA_RESULT ga_brr_search_char_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID c_uuid,
              /* OUT */ UCHAR       * id
          );

GA_RESULT ga_brr_allocate_char_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID c_uuid,
              /* OUT */ UCHAR       * id
          );

GA_RESULT ga_brr_free_char_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID c_uuid
          );
#endif /* _H_GA_BRR_UTILS_ */

