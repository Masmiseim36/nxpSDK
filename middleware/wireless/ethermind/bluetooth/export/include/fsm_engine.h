
/**
 *  \file fsm_engine.h
 *
 *  This file defines interface offered by the FSM module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_FSM_ENGINE_
#define _H_FSM_ENGINE_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "fsm_defines.h"


#ifndef FSM_NO_DEBUG
#ifdef VAR_ARG_IN_MACRO_NOT_SUPPORTED
#define FSM_ERR
#else
#define FSM_ERR(...)       BT_debug_error(BT_MODULE_ID_FSM,__VA_ARGS__)
#endif /* VAR_ARG_IN_MACRO_NOT_SUPPORTED */
#else  /* FSM_NO_DEBUG */
#define FSM_ERR            BT_debug_null
#endif /* FSM_NO_DEBUG */

#ifdef FSM_DEBUG

#define FSM_TRC(...)       BT_debug_trace(BT_MODULE_ID_FSM,__VA_ARGS__)
#define FSM_INF(...)       BT_debug_info(BT_MODULE_ID_FSM,__VA_ARGS__)

#else /* FSM_DEBUG */
#ifdef VAR_ARG_IN_MACRO_NOT_SUPPORTED
#define FSM_TRC
#define FSM_INF
#else
#define FSM_TRC            BT_debug_null
#define FSM_INF            BT_debug_null
#endif /* VAR_ARG_IN_MACRO_NOT_SUPPORTED */

#endif /* FSM_DEBUG */

/* --------------------------------------------- Functions */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \addtogroup fsm_module FSM (Finite State Machine)
 * \{
 */

/**
 * \defgroup fsm_api FSM API definitions
 * \{
 * This section describes the definitions of FSM APIs
 */
/**
 *  \brief To do power on initialization of FSM module
 *
 *  \par Description:
 *       This function is the EtherMind-Init handler for the FSM module
 *       and performs power-on initialization.
 *
 *  \note This function must be called only once.
 */
void em_fsm_init (void);

#ifdef BT_HAVE_SHUTDOWN
/**
 *  \brief To de-initialize FSM module
 *
 *  \par Description:
 *       This function is the EtherMind-de-init handler for the FSM module.
 */
void em_fsm_shutdown (void);
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \brief To perform Bluetooth specific initializations for FSM module
 *
 *  \par Description:
 *       This function is the Bluetooth-ON handler for FSM module, and it
 *       performs bluetooth specific initializations for the FSM module.
 */
void fsm_bt_init (void);

/**
 *  \brief To perform Bluetooth specific shutdown for FSM module
 *
 *  \par Description:
 *       This function is the Bluetooth-OFF handler for FSM module, and it
 *       performs bluetooth specific shutdown for the FSM module.
 */
void fsm_bt_shutdown (void);

/**
 *  \brief Register with FSM
 *
 *  \par Description:
 *       This routine registers state-event table with the FSM module
 *
 *  \param [in] module_fsm
 *         State-event table to be registered with FSM module
 *
 *  \param [out] fsm_id
 *         Pointer to FSM Identifier, populated by the FSM module
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT fsm_register_module
           (
               /* IN */  DECL_CONST FSM_MODULE_TABLE_T * module_fsm,
               /* OUT */ UCHAR * fsm_id
           );

/**
 *  \brief To post event to FSM
 *
 *  \par Description
 *  This function posts event with associated parameters to FSM for processing.
 *
 *  \param [in] fsm_id     Module identifier of the recepient of the event
 *
 *  \param [in] fsm_event  Event type
 *
 *  \param [in] param      Parameter associated with the event
 *
 *  \return API_RESULT
 */
API_RESULT fsm_post_event
           (
               UCHAR     fsm_id,
               EVENT_T   fsm_event,
               void      * param
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
#endif /* _H_FSM_ENGINE_ */

