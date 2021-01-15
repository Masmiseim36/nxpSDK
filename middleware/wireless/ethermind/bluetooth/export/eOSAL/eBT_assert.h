
/**
 *  \file eBT_assert.h
 *
 *  This file contains the implementation of BT_assert() macro,
 *  used to evaluate an expression and take action based on whether
 *  the expression is true or false.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EBT_ASSERT_
#define _H_EBT_ASSERT_

/* --------------------------------------------- Header File Inclusion */
#include "EM_assert.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#ifdef BT_DEBUG_ASSERT
#define BT_assert(exp)                      EM_assert(exp)

#else  /* BT_DEBUG_ASSERT */
#define BT_assert(exp)

#endif /* BT_DEBUG_ASSERT */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

#endif /* _H_EBT_ASSERT_ */

