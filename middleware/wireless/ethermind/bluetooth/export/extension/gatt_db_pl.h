
/**
 *  \file gatt_db_pl.h
 *
 *  Platform specific Header File for Device Queue module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_PL_
#define _H_GATT_DB_PL_

/* ----------------------------------------------- Header File Inclusion */

/* ----------------------------------------------- Global Definitions */

/* ----------------------------------------------- Structures/Data Types */
/**
 * GATT DB PL extension, GATT DB Handler Callback.
 * The GATT DB PL extension calls the registered callback
 * to indicate any ongoing GATT DB Operation.
 * The Upper Layer can implement specific handling for any Characteristic
 * or Characteristic Descriptor depending on its requirement.
 *
 * \param [in] handle Pointer to GATT DB identifier \ref GATT_DB_HANDLE
 * \param [in] param  Pointer to GATT DB Parameters \ref GATT_DB_PARAMS
 *
 * \return API_SUCCESS if ATT/GATT response for the incoming request to be sent
 *         from the below layer
 *         GATT_DB_DELAYED_RESPONSE if ATT/GATT response will be sent at a
 *         later point by the application
 *         GATT_DB_DONOT_RESPOND if ATT/GATT response will be sent at a
 *         later point by the application
 *         GATT_DB_ALREADY_RESPONDED if ATT/GATT response is already sent
 *         the application
 *         else and Error code describing cause of failure.
 */
typedef API_RESULT (* GATT_DB_PL_EXT_HANDLER_CB)
                   (
                       /* IN */ GATT_DB_HANDLE * handle,
                       /* IN */ GATT_DB_PARAMS * param
                   ) DECL_REENTRANT;

/* ----------------------------------------------- Function Declarations */
API_RESULT gatt_db_init_pl(GATT_DB_PL_EXT_HANDLER_CB hndlr_cb);

#endif /* _H_GATT_DB_PL_ */

