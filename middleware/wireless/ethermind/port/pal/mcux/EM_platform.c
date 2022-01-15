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

int EM_str_to_num_pl
    (
        /* IN */  char * str,
        /* IN */  int    len
    )
{
    int num = 0;
    int index;

    for(index = 0; index < len; index++)
    {
        if(str[index] >= '0' && str[index] <= '9')
        {
            num = (num * 10) + (str[index] - '0');
        }
        else
        {
            /* : What error value is to be returned? */
        }
    }

    return num;
}

int EM_str_to_hexnum_pl
    (
        /* IN */ char * str,
        /* IN */ int    len
    )
{
    int num = 0;
    int index;
    unsigned char val;

    if (len > 8)
    {
        printf ("Invalid string length for str to hex num conversion \n");
        return 0;
    }

    val = 0U;
    for (index = 0; index < len; index++)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            val = (str[index] - '0');

        }
        else if ((str[index] == 'A') ||  (str[index] == 'a'))
        {
            val = 0xAU;
        }
        else if ((str[index] == 'B') || (str[index] == 'b'))
        {
            val = 0xBU;
        }
        else if ((str[index] == 'C') || (str[index] == 'c'))
        {
            val = 0xCU;
        }
        else if ((str[index] == 'D') || (str[index] == 'd'))
        {
            val = 0xDU;
        }
        else if ((str[index] == 'E') || (str[index] == 'e'))
        {
            val = 0xEU;
        }
        else if ((str[index] == 'F') || (str[index] == 'f'))
        {
            val = 0xFU;
        }
        else
        {
            /* Should not reach here */
            printf ("Invalid char in string for str to hex num conversion \n");
        }

        num = (num  << 4U) | val;
    }

    return num;
}

void EM_num_to_str_pl
     (
         /* IN  */ int    num,
         /* OUT */ char * str,
         /* OUT */ int  * len
     )
{
    int length, index;
    char ch;

    length = 0;

    if (NULL == str)
    {
        return;
    }

    do
    {
        ch = num % 10;
        ch = ch + '0';
        num = num / 10;

        str[length] = ch;
        length++;

    }while(num > 0);

    for (index = 0; index < (length >> 1); index++)
    {
        ch = str[index];
        str[index] = str[length - 1 - index];
        str[length - 1 - index] = ch;
    }

    str[length] = '\0';
    *len = length;

    return;
}

void __assertion_failed(char * p)
{
}

int _printf(const char *fmt, ...)
{
    /* fix build warning: missing return statement at end of non-void function. */
    return 0U;
}

