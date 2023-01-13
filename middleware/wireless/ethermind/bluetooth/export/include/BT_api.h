
/**
 *  \file BT_api.h
 *
 *  This file contains the API Header Files of the EtherMind Protocols
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_API_
#define _H_BT_API_

/* ---------------------------------------------- Global Definitions */
#include "BT_common.h"
#include "BT_hci_api.h"

#ifdef CLASSIC_SEC_MANAGER
#include "BT_sm_api.h"
#endif /* CLASSIC_SEC_MANAGER */

#ifdef SDP
    #include "BT_sdp_api.h"
    #include "BT_dbase_api.h"
#endif /* SDP */

#ifdef RFCOMM
    #include "BT_rfcomm_api.h"
#endif /* RFCOMM */

#ifdef BNEP
    #include "BT_bnep_api.h"
#endif /* BNEP */

#ifdef AVDTP
    #include "BT_avdtp_api.h"
#endif /* AVDTP */

#ifdef AVCTP
    #include "BT_avctp_api.h"
#endif /* AVCTP */

#ifdef OBEX
    #include "BT_obex_api.h"
#endif /* OBEX */

#ifdef SMP
#include "BT_smp_api.h"
#endif /* SMP */

#ifdef ATT
#include "BT_att_api.h"
#endif /* ATT */

/* ---------------------------------------------- Global Definitions */


/* ---------------------------------------------- Structures/Data Types */


/* ---------------------------------------------- API Declarations */

#endif /* _H_BT_API_ */

