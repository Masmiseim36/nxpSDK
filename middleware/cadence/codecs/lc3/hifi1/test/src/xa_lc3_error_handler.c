/*
 * Copyright (c) 2019-2023 Cadence Design Systems, Inc.
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

/*******************************************************************************
 Function: Error related functions of lc3 codec
 ******************************************************************************/
#include <stdio.h>
#include <xa_lc3_codec_api.h>
#include <xa_error_handler.h>

/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Nonfatal Errors */

/* Fatal Errors */
const char *ppb_xa_lc3_api_fatal[] = 
{
    "NULL Pointer: Memory Allocation Error", 
    "Memory Allocation Error: Alignment requirement not met",
    "Invalid Command",
    "Invalid Command Type/Index"
};

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
const char *ppb_xa_lc3_config_non_fatal[] = 
{
    "Invalid param",
    "Invalid PLC method"
};

/* Fatal Errors */
const char *ppb_xa_lc3_config_fatal[] =
{
    "Invalid sampling rate",
    "Invalid number of channels",
    "Invalid bitrate",
    "Invalid bitdepth",    
    "Invalid Frame Size"
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
const char *ppb_xa_lc3_execute_non_fatal[] =
{
    "Frame failed to decode and was concealed"
};

/* Fatal Errors */
const char *ppb_xa_lc3_execute_fatal[] =
{
    "Invalid number of bytes",
    "LC3 not initialized"
};

/*****************************************************************************/
/* Class 0: Memory & File Manager Errors                                     */
/*****************************************************************************/
/* Non Fatal Errors */

/* Fatal Errors */
const char *ppb_xa_testbench_mem_file_man_fatal[] = 
{
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

void xa_testbench_error_handler_init()
{
	/* The Message Pointers	*/
	xa_testbench_error_info.ppppb_error_msg_pointers[1][ 0] = 
		ppb_xa_testbench_mem_file_man_fatal;
}


/*****************************************************************************/
/* error info structure                                                      */
/*****************************************************************************/
/* The Module's Error Info Structure */
xa_error_info_struct xa_lc3_error_info = 
{
	/* The Module Name	*/
	"Cadence LC3 Codec",
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
  	
  	
void xa_lc3_error_handler_init()
{
	/* The Message Pointers	*/
    xa_lc3_error_info.ppppb_error_msg_pointers[1][0] =
        ppb_xa_lc3_api_fatal;
    xa_lc3_error_info.ppppb_error_msg_pointers[0][1] =
		ppb_xa_lc3_config_non_fatal;
    xa_lc3_error_info.ppppb_error_msg_pointers[1][1] =
		ppb_xa_lc3_config_fatal;
    xa_lc3_error_info.ppppb_error_msg_pointers[0][2] =
		ppb_xa_lc3_execute_non_fatal;
    xa_lc3_error_info.ppppb_error_msg_pointers[1][2] =
        ppb_xa_lc3_execute_fatal;
}  	


XA_ERRORCODE
xa_error_handler(xa_error_info_struct *p_mod_err_info, 
		 const char *pb_context,
		 XA_ERRORCODE code) 
{
    if (code == XA_NO_ERROR) {
        return XA_NO_ERROR;
    }
    
    {
        WORD is_fatal	  = XA_ERROR_SEVERITY(code);
        WORD err_class    = XA_ERROR_CLASS(code);
        /* not required here 
        WORD err_codec    = XA_ERROR_CODEC(code); */
        WORD err_sub_code = XA_ERROR_SUBCODE(code); 

        printf("\n");
        if (!is_fatal) 
        {
            printf("non ");
        }
        printf("fatal error: ");

        if ((p_mod_err_info->pb_module_name != NULL) &&
        (p_mod_err_info->pb_module_name[0] != '\0')) 
        {
            printf("%s: ", p_mod_err_info->pb_module_name);
        }
        if (p_mod_err_info->ppb_class_names[err_class] != NULL) 
        {
            printf("%s: ", p_mod_err_info->ppb_class_names[err_class]);
        }
        if (pb_context != NULL) 
        {
            printf("%s: ", pb_context);
        }
        if (p_mod_err_info->ppppb_error_msg_pointers[is_fatal][err_class] != NULL)
        {
            printf("%s\n", p_mod_err_info->ppppb_error_msg_pointers[is_fatal][err_class][err_sub_code]);
        }
    }
    return XA_NO_ERROR;
}
