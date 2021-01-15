/**
 *  \file EM_platform.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "EM_platform.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Function */
void EM_enter_sleep_pl(void)
{
}

void EM_exit_sleep_pl(void)
{
}

void __assertion_failed(char * p)
{
}

int _printf(const char *fmt, ...)
{
    /* fix build warning: missing return statement at end of non-void function. */
    return 0;
}

