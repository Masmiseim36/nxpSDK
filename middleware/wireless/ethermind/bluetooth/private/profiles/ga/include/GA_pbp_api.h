
/**
 *  \file GA_pbp_api.h
 *
 *  \brief This file defines the GA Public Broadcast Profile defines,
 *  methods and interfaces
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_PBP_API_
#define _H_GA_PBP_API_

/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"


/**
 * \addtogroup bt_ga_appl_profiles Application Profiles
 * \ingroup bt_ga_profiles
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * GA Application Profiles.
 */

/**
 * \defgroup pbp_module Public Broadcast Profile (PBP)
 * \ingroup bt_ga_appl_profiles
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * GA PBP (PBP) module to the Application and other upper
 * layers of the stack.
 */

/* --------------------------------------------- Global Definitions */
/**
 * \defgroup pbp_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind GA
 * PBP Layer.
 */

/**
 * \defgroup pbp_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind GA
 * PBP Layer.
 */

/**
 * \name PBP Broadcast features
 * \{
 */
/** Encryption feature */
#define PBP_BCAST_ENCRYPTION_FEATURE_BIT      0U
/** Standard Quality Audio feature */
#define PBP_BCAST_SQ_FEATURE_BIT              1U
/** High Quality Audio feature */
#define PBP_BCAST_HQ_FEATURE_BIT              2U

/** \} */

/**
 * \name PBP Encryption supported values
 * \{
 */

/** Profile Encryption - OFF */
#define PBP_BCAST_ENCRYPTION_OFF              0x00U
/** Profile Encryption - ON */
#define PBP_BCAST_ENCRYPTION_ON               0x01U

/** \} */

/**
 * \name PBP Standard Quality broadcast supported values
 * \{
 */

/** Profile Standard Quality Broadcast - OFF */
#define PBP_BCAST_SQ_OFF                      0x00U
/** Profile Standard Quality Broadcast - ON */
#define PBP_BCAST_SQ_ON                       0x01U

/** \} */

/**
 * \name PBP High Quality broadcast supported values
 * \{
 */

/** Profile High Quality Broadcast - OFF */
#define PBP_BCAST_HQ_OFF                      0x00U
/** Profile High Quality Broadcast - OFF */
#define PBP_BCAST_HQ_ON                       0x01U

/** \} */

/** \} */

/* --------------------------------------------- Macros */
/**
 * \defgroup pbp_macros Macros
 * \{
 * \brief This section describes the various Utility and functional Macros in
 * EtherMind GA PBP Layer.
 */

/** Interface to prepare additional profile service data */
#define PBP_ENCODE_BCAST_ADV_SERVICE_DATA(buf, id, en, sq, hq) \
        { \
            UINT16 uuid; \
            uuid = GA_SERVICE_PBAS; \
 \
            (id) = 0U; \
            (buf)[(id)] = 1U + 3U + 1U; \
            (id)++; \
            (buf)[(id)] = GA_ADTYPE_SERVICE_DATA; \
            (id)++; \
            GA_PACK_LE_2_BYTE_VAL(&(buf)[(id)], uuid); \
            (id) += 2U; \
            (buf)[(id)]  = ((en) << PBP_BCAST_ENCRYPTION_FEATURE_BIT); \
            (buf)[(id)] |= ((sq) << PBP_BCAST_SQ_FEATURE_BIT); \
            (buf)[(id)] |= ((hq) << PBP_BCAST_HQ_FEATURE_BIT); \
            (id)++; \
            (buf)[(id)] = 0x00U; \
            (id)++; \
        }

/** Interface to prepare additional profile service data */
#define PBP_ENCODE_BCAST_ADV_SERVICE_DATA_WITH_METADATA(buf, id, en, sq, hq, ml, m) \
        { \
            UINT16 uuid; \
            uuid = GA_SERVICE_PBAS; \
 \
            (id) = 0U; \
            (buf)[(id)] = 1U + 3U + 1U + ml; \
            (id)++; \
            (buf)[(id)] = GA_ADTYPE_SERVICE_DATA; \
            (id)++; \
            GA_PACK_LE_2_BYTE_VAL(&(buf)[(id)], uuid); \
            (id) += 2U; \
            (buf)[(id)]  = ((en) << PBP_BCAST_ENCRYPTION_FEATURE_BIT); \
            (buf)[(id)] |= ((sq) << PBP_BCAST_SQ_FEATURE_BIT); \
            (buf)[(id)] |= ((hq) << PBP_BCAST_HQ_FEATURE_BIT); \
            (id)++; \
            (buf)[(id)] = (ml); \
            (id)++; \
            GA_mem_copy((&(buf)[(id)]), m, ml); \
            (id) += (ml); \
        }

/**
 * Interface to prepare additional advertising data,
 * As per PBP Spec, Name Length: Min 4, Max 32.
 */
#define PBP_ENCODE_BCAST_ADV_BROADCAST_NAME(buf, id, name, len) \
        (id) = 0U; \
        (buf)[(id)] = 1U + len; \
        (id)++; \
        (buf)[(id)] = GA_ADTYPE_BROADCAST_NAME; \
        (id)++; \
        GA_mem_copy(&(buf)[(id)], (name), (len)); \
        (id) += len;

/**
 * When populating AdvData, a PBS should include the Appearance Value AD Type
 * with a value that identifies the audio source as a type of device or venue
 * (defined in Bluetooth Assigned Numbers).
 *
 * Refer below for values:
 * https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf
 */
#define PBP_ENCODE_BCAST_ADV_APPEARANCE(buf, id, value) \
        (id) = 0U; \
        (buf)[(id)] = 1U + 2U; \
        (id)++; \
        (buf)[(id)] = GA_ADTYPE_APPEARANCE; \
        (id)++; \
        GA_PACK_LE_2_BYTE_VAL(&(buf)[(id)], (value)); \
        (id) += 2U;

/** \} */

/** \} */

/* --------------------------------------------- APIs */

/** \} */
/** \} */

#endif /* _H_GA_PBP_API_ */
