
/**
 *  \file cap.h
 *
 *  \brief This file defines the internal profile interfaces for CAP.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_CAP_
#define _H_CAP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"

/* --------------------------------------------- Global Definitions */
/* BAP Event Definitions */

/* CSIP Event Definitions */

/* MICP Event Definitions */

/* VCP Event Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */
GA_RESULT cap_iface_process_bap_event
          (
              GA_ENDPOINT * device,
              UINT8 event_type,
              UINT16 status,
              void * data,
              UINT16 datalen
          );

GA_RESULT cap_iface_process_vcp_event
          (
              VCP_HANDLE *vctx,
              UINT8 event_type,
              UINT16 status,
              void * data,
              UINT16 datalen
          );

GA_RESULT cap_iface_process_vcs_event
          (
              GA_ENDPOINT * device,
              UINT8 event_type,
              UINT16 status,
              void * data,
              UINT16 datalen
          );

GA_RESULT cap_iface_process_micp_event
          (
              MICP_HANDLE * mctx,
              UINT8 event_type,
              UINT16 status,
              void   *data,
              UINT16 datalen
          );

GA_RESULT cap_iface_process_mics_event
          (
              GA_ENDPOINT * device,
              UINT8         event_type,
              UINT16        status,
              void        * data,
              UINT16        datalen
          );

GA_RESULT cap_iface_process_csip_event
          (
              CSIP_HANDLE * handle,
              UINT8 event_type,
              UINT16 status,
              void * data,
              UINT16 datalen
          );

GA_RESULT cap_iface_process_csis_event
          (
              GA_ENDPOINT * device,
              UINT8         event_type,
              UINT16        status,
              void        * data,
              UINT16        datalen
          );

#endif /* _H_CAP_ */




