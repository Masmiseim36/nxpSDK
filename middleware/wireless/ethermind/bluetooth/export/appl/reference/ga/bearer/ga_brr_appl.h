
/**
 *  \file ga_brr_appl.h
 *
 *  \brief This file defines the Generic Audio Bearer Internal Interface - includes
 *  Data Structures and Methods.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */


/* --------------------------------------------- Header File Inclusion */
#include "GA_bearer_api.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */
GA_RESULT appl_ga_brr_cb
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
             /* IN */ UINT16 event_datalen
          );

GA_RESULT appl_ga_profile_cb
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UINT8         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16        event_datalen
          );

GA_RESULT appl_ga_service_cb
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UINT8         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16        event_datalen
         );

void main_ga_brr_operations(void);


