
/**
 *  \file tokenizer.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_TOKENIZER_
#define _H_TOKENIZER_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */
typedef struct
{
    UCHAR input;
    UINT16 next_state;
} TRANSITION_TABLE;

typedef struct
{
    UINT16 table_index;
    UINT16 count;
} LOOKUP_TABLE;

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
/**
 *  \fn token_match
 *
 *  \brief This is the main and the only tokenizer function.
 *
 *  \Description This functino tokenizes the input stream and reports if any
 *  matching token/keyword is found, this function is totally stream based and
 *  re-entrant, the user of the API is responsible for setting the input
 *  parameter prev_state to the correct value based on the return value of
 *  the state from the previous call to the API. The very first time the
 *  prev_state parameter should be set to start_state.
 *
 *
 *  \param stream
 *  \param length
 *  \param prev_state
 *  \param toked_len
 *
 *  \return
 */

UINT16 token_match
       (
           UCHAR *stream,
           UINT16 length,
           UINT16 * toked_len
       );

#endif /* _H_TOKENIZER_ */
