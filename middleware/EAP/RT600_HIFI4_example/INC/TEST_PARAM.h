/*
* Copyright 2020 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/

#ifndef TEST_PARAM
#define TEST_PARAM
/*******************************************************************/
/* INCLUDE */
/*******************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LVM.h" // EAP library

/*******************************************************************/
/* DEFINE */
/*******************************************************************/
#ifdef ALGORITHM_DBE
#define ALGORITHM_BASS
#endif /* ALGORITHM_DBE */
#ifdef ALGORITHM_PB
#define ALGORITHM_BASS
#endif /* ALGORITHM_PB */

#ifdef  ALGORITHM_CS
#define ALGORITHM_3DWIDENING                    /* Define when Concert Sound required */
#else
#ifdef  ALGORITHM_CI
#define ALGORITHM_3DWIDENING
#endif  /* ALGORITHM_CI */
#endif  /* ALGORITHM_CS */

#ifndef ALGORITHM_VC
#define  ALGORITHM_VC
#endif

/*******************************************************************/
/* FUNCTION */
/*******************************************************************/
#endif /* TEST_PARAM */
