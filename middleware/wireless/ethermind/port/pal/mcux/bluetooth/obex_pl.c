
/**
 *  \file obex_pl.c
 *
 *  This file contains the implementation for all platform specific
 *  extensions for OBEX module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "obex_pl.h"

#ifdef OBEX

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */
/*
 * Function for MD5 algorithm
 *
 * Note:- Duplicated the declaration from obex_internal.h
 * to avoid inclusion of "obex_internal.h"
 */
void obex_md5
     (
         /* OUT */ void *   dest,
         /* IN */  void *   orig,
         /* IN */  UINT32   len
     );

/* ----------------------------------------- Static Global Variables */


/* ----------------------------------------- API Functions */

/*
 *  \fn BT_obex_generate_digest_string ()
 *
 *  \brief To generate a digest string for use in Authentication
 *
 *  \par Description:
 *      This API is called by the application/profile to generate a digest
 *      string for use in Authentication.
 *
 *  \param digest_string (OUT)
 *      The generated digest string
 *
 *  \return API_RESULT
 *      API_SUCCESS on success otherwise an error code describing the
 *      reason for failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *      Get the local time and use it as a nonce value.
 *      Apply a hashing algorithm on the nonce to generate a digest string.
 *      MD5 hashing algorithm is used here.
 *
 *      Return
 *
 *  \endcode
 *  \endif
 *
 *  \note
 *      Using MD5 hashing algorithm the size of the generated digest will
 *      be OBEX_AUTH_BUF_LEN bytes. So, the caller of this function has
 *      to ensure that at least OBEX_AUTH_BUF_LEN(16) bytes of memory
 *      is available for the output.
 */
API_RESULT  BT_obex_generate_digest_string ( /* OUT */ UCHAR * digest_string )
{
    API_RESULT retval;
    const CHAR * pts = "Randomize me";

    if (NULL == digest_string)
    {
        OBEX_PL_ERR(
        "[OBEX_PL] Output Buffer is NULL.\n");

        retval = OBEX_AUTH_FAILED;
    }
    else
    {
        OBEX_PL_INF(
        "[OBEX_PL] Generating the Digest String\n");

        /* Apply an hashing algorithm on the nonce */
        obex_md5 (digest_string, (CHAR *)pts, BT_str_len(pts));

        retval = API_SUCCESS;
    }

    return retval;
}

#endif /* OBEX */

