/**
 *  \file GA_bass_db.h
 *
 *  \brief This file defines the BASS DB methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_BASS_DB_
#define _H_GA_BASS_DB_

/* --------------------------------------------- Header File Inclusion */
#include "GA_bass_api.h"

/* --------------------------------------------- Global Definitions */
/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- APIs */

#ifdef BASS_DB

void BASS_DB_init(void);

void BASS_DB_deinit(void);

GA_RESULT BASS_DB_get_free_index(UCHAR* index);

GA_RESULT BASS_DB_add_source(GA_BASS_BC_RX_STATE * add_src, UINT8 * db_index);

GA_RESULT BASS_DB_modify_source
          (
              UINT8                 db_index,
              GA_BASS_BC_RX_STATE * modify_src
          );

GA_RESULT BASS_DB_remove_source(UINT8 db_index);

GA_RESULT BASS_DB_fetch_rx_state_instance
          (
              UINT8                 db_index,
              GA_BASS_BC_RX_STATE * rx_state
          );

#endif /* BASS_DB */

#endif /* _H_GA_BASS_DB_ */