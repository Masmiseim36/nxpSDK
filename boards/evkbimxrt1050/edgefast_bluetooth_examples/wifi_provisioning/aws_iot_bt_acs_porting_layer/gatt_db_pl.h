
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

/* ----------------------------------------------- Function Declarations */
API_RESULT gatt_db_service_change_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );
API_RESULT gatt_db_battery_level_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );
#endif /* _H_GATT_DB_PL_ */

