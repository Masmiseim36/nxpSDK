/*
 * Copyright (c) 2009-2020 Cadence Design Systems, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/*****************************************************************************/
/*                                                                           */
/*  File Name        : xa_sbc_dec_error_handler.c                            */
/*                                                                           */
/*  Description      : Error related functions of sbc decoder                */
/*                                                                           */
/*  List of Functions: 1. xa_main_process                                    */
/*                                                                           */
/*  Issues / Problems: None                                                  */
/*                                                                           */
/*  Revision History :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*        29 07 2005       Tejaswi/Vishal        Created                     */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* File includes                                                             */
/*****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"

/*****************************************************************************/
/* Process select hash defines                                               */
/*****************************************************************************/
#define DISPLAY_MESSAGE

/*****************************************************************************/
/* Constant hash defines                                                     */
/*****************************************************************************/
#define MAX_STACK_PROC 10
#define MAX_MEM_ALLOCS 100
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define XA_SCREEN_WIDTH 80

/*****************************************************************************/
/* Global memory constants                                                   */
/*****************************************************************************/
/*****************************************************************************/
/* sbc_dec ErrorCode Definitions                                             */
/*****************************************************************************/
/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Nonfatal Errors (none for SBC decode) */
/* Fatal Errors */
const char *ppb_xa_sbc_dec_api_fatal[] = {
  "NULL Pointer: Memory Allocation Error", 
  "Memory Allocation Error: Alignment requirement not met",
  "Invalid Command",
  "Invalid Command Type/Index"
};

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
const char *ppb_xa_sbc_dec_config_non_fatal[] = {
  "", 
  ""
};

/* Fatal Errors */
const char *ppb_xa_sbc_dec_config_fatal[] = {
  "",
  ""
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
const char *ppb_xa_sbc_dec_execute_non_fatal[] = {
  "Not a SBC frame", // bad syncword
  "CRC Error",		// bad crc
  "Error in unpacked SBC block data", // bad frame configuration params
  "Not enough input data to decode a complete frame",
  "",
  ""
};

/* Fatal Errors */
const char *ppb_xa_sbc_dec_execute_fatal[] = {
  "Not a SBC audio file!!", 
  ""
};

/*****************************************************************************/
/* xa_testbench ErrorCode Definitions                                        */
/*****************************************************************************/
/*****************************************************************************/
/* Class 0: Memory & File Manager Errors                                     */
/*****************************************************************************/
/* Non Fatal Errors */
/* Fatal Errors */
const char *ppb_xa_testbench_mem_file_man_fatal[] = {
  "Memory Allocation Error", 
  "File Open Failed"
};

/*****************************************************************************/
/* error info structure                                                      */
/*****************************************************************************/
/* The Module's Error Info Structure */
xa_error_info_struct xa_testbench_error_info = 
{
	/* The Module Name	*/
	"xa_testbench",
	{
		/* The Class Names	*/
		"Memory & File Manager",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	},
	{
		/* The Message Pointers	*/
		{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
		  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
		{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
		  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
	}
};

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_testbench_error_handler_init                          */
/*                                                                           */
/*  Description   : Initialize the error struct with string pointers         */
/*                                                                           */
/*  Inputs        : none                                                     */
/*                                                                           */
/*  Globals       : xa_error_info_struct xa_testbench_error_info             */
/*                  const char *ppb_xa_testbench_mem_file_man_fatal          */
/*                                                                           */
/*  Processing    : Init the struct with error string pointers               */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : none                                                     */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/

VOID xa_testbench_error_handler_init()
{
	/* The Message Pointers	*/
	xa_testbench_error_info.ppppb_error_msg_pointers[1][ 0] = 
		ppb_xa_testbench_mem_file_man_fatal;
}

/*****************************************************************************/
/* error info structure                                                      */
/*****************************************************************************/
/* The Module's Error Info Structure */
xa_error_info_struct xa_sbc_dec_error_info = 
{
	/* The Module Name	*/
	"Tensilica SBC Decoder",
	{
		/* The Class Names	*/
		"API",
		"Configuration",
		"Execution",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	},
	{
	  /* The Message Pointers	*/
	  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
	    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
	  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
	    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
	}
};

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_sbc_dec_error_handler_init                            */
/*                                                                           */
/*  Description   : Initialize the error struct with string pointers         */
/*                                                                           */
/*  Inputs        : none                                                     */
/*                                                                           */
/*  Globals       : xa_error_info_struct xa_sbc_dec_error_info               */
/*                  const char *ppb_xa_sbc_dec_api_fatal                     */
/*                  const char *ppb_xa_sbc_dec_config_non_fatal              */
/*                  const char *ppb_xa_sbc_dec_execute_non_fatal             */
/*                  const char *ppb_xa_sbc_dec_execute_fatal                 */
/*                                                                           */
/*  Processing    : Init the struct with error string pointers               */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : none                                                     */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/

VOID xa_sbc_dec_error_handler_init()
{
	/* The Message Pointers	*/
	xa_sbc_dec_error_info.ppppb_error_msg_pointers[1][ 0] = 
		ppb_xa_sbc_dec_api_fatal;
	xa_sbc_dec_error_info.ppppb_error_msg_pointers[0][ 1] = 
		ppb_xa_sbc_dec_config_non_fatal;
	xa_sbc_dec_error_info.ppppb_error_msg_pointers[1][ 1] = 
		ppb_xa_sbc_dec_config_fatal;
	xa_sbc_dec_error_info.ppppb_error_msg_pointers[0][ 2] = 
		ppb_xa_sbc_dec_execute_non_fatal;
	xa_sbc_dec_error_info.ppppb_error_msg_pointers[1][ 2] = 
		ppb_xa_sbc_dec_execute_fatal;
}

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_error_handler                                         */
/*                                                                           */
/*  Description   : Called Prints the status error code from the err_info    */
/*                                                                           */
/*  Inputs        : xa_error_info_struct *p_mod_err_info (Error info struct) */
/*                  const char *pb_context (Context of error)                */
/*                  XA_ERRORCODE code (Error code)                           */
/*                                                                           */
/*  Globals       : none                                                     */
/*                                                                           */
/*  Processing    : whenever any module calls the errorhandler,  it  informs */
/*                  it about the module for which it is called and a context */
/*                  in which it was  called  in addition to  the  error_code */
/*                  the message is displayed  based  on the  module's  error */
/*                  message  array  that maps to  the error_code the context */
/*                  gives specific info in which the error occured  e.g. for */
/*                  testbench   module,  memory  allocator   can   call  the */
/*                  errorhandler   for  memory  inavailability  in   various */
/*                  contexts like input_buf or output_buf e.g.  for  sbc_enc */
/*                  module, there can be various instances running.  context */
/*                  can be used to  identify  the  particular  instance  the */
/*                  error handler is being called for                        */
/*                                                                           */
/*  Outputs       : None                                                     */
/*                                                                           */
/*  Returns       : XA_ERRORCODE error_value  (Error value)                  */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*        29 07 2005       Tejaswi/Vishal        Created                     */
/*                                                                           */
/*****************************************************************************/

XA_ERRORCODE
xa_error_handler(xa_error_info_struct *p_mod_err_info, 
		 const char *pb_context,
		 XA_ERRORCODE code) {
  if (code == XA_NO_ERROR) {
    return XA_NO_ERROR;
  }
  {
    WORD is_fatal     = XA_ERROR_SEVERITY(code) != 0;
    WORD err_class    = XA_ERROR_CLASS(code);
    WORD err_sub_code = XA_ERROR_SUBCODE(code);
	
    printf("\n");
    if (!is_fatal) {
      printf("non ");
    }
    printf("fatal error: ");

    if ((p_mod_err_info->pb_module_name != NULL) &&
	(p_mod_err_info->pb_module_name[0] != '\0')) {
      printf("%s: ", p_mod_err_info->pb_module_name);
    }
    if (p_mod_err_info->ppb_class_names[err_class] != NULL) {
      printf("%s: ", p_mod_err_info->ppb_class_names[err_class]);
    }
    if (pb_context != NULL) {
      printf("%s: ", pb_context);
    }
    printf("%s\n", p_mod_err_info->ppppb_error_msg_pointers[is_fatal][err_class][err_sub_code]);
  }
  return XA_NO_ERROR;
}
