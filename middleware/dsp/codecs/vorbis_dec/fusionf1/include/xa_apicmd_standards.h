/*
 * Copyright (c) 2006-2020 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Cadence Design Systems Inc.  They may be adapted and modified by bona fide
 * purchasers for internal use, but neither the original nor any adapted
 * or modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the prior
 * written consent of Cadence Design Systems Inc.  This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */

  
#ifndef __XA_API_CMD_STANDARDS_H__
#define __XA_API_CMD_STANDARDS_H__

/*****************************************************************************/
/* Standard API commands                                                     */
/*****************************************************************************/

enum xa_api_cmd_generic {
  XA_API_CMD_GET_LIB_ID_STRINGS	      = 0x0001,

  XA_API_CMD_GET_API_SIZE             = 0x0002,
  XA_API_CMD_INIT                     = 0x0003,

  XA_API_CMD_SET_CONFIG_PARAM         = 0x0004,
  XA_API_CMD_GET_CONFIG_PARAM         = 0x0005,

  XA_API_CMD_GET_MEMTABS_SIZE         = 0x0006,
  XA_API_CMD_SET_MEMTABS_PTR          = 0x0007,
  XA_API_CMD_GET_N_MEMTABS            = 0x0008,

  XA_API_CMD_EXECUTE                  = 0x0009,

  XA_API_CMD_PUT_INPUT_QUERY          = 0x000A,
  XA_API_CMD_GET_CURIDX_INPUT_BUF     = 0x000B,
  XA_API_CMD_SET_INPUT_BYTES          = 0x000C,
  XA_API_CMD_GET_OUTPUT_BYTES         = 0x000D,
  XA_API_CMD_INPUT_OVER               = 0x000E,

  XA_API_CMD_GET_MEM_INFO_SIZE        = 0x0010,
  XA_API_CMD_GET_MEM_INFO_ALIGNMENT   = 0x0011,
  XA_API_CMD_GET_MEM_INFO_TYPE        = 0x0012,
  XA_API_CMD_GET_MEM_INFO_PLACEMENT   = 0x0013,
  XA_API_CMD_GET_MEM_INFO_PRIORITY    = 0x0014,
  XA_API_CMD_SET_MEM_PTR              = 0x0015,
  XA_API_CMD_SET_MEM_INFO_SIZE        = 0x0016,
  XA_API_CMD_SET_MEM_PLACEMENT        = 0x0017,

  XA_API_CMD_GET_N_TABLES             = 0x0018,
  XA_API_CMD_GET_TABLE_INFO_SIZE      = 0x0019,
  XA_API_CMD_GET_TABLE_INFO_ALIGNMENT = 0x001A,
  XA_API_CMD_GET_TABLE_INFO_PRIORITY  = 0x001B,
  XA_API_CMD_SET_TABLE_PTR            = 0x001C,
  XA_API_CMD_GET_TABLE_PTR            = 0x001D
};

/*****************************************************************************/
/* Standard API command indices                                              */
/*****************************************************************************/

enum xa_cmd_type_generic {
  /* XA_API_CMD_GET_LIB_ID_STRINGS indices */
  XA_CMD_TYPE_LIB_NAME                    = 0x0100,
  XA_CMD_TYPE_LIB_VERSION                 = 0x0200,
  XA_CMD_TYPE_API_VERSION                 = 0x0300,

  /* XA_API_CMD_INIT indices */
  XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS  = 0x0100,
  XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS = 0x0200,
  XA_CMD_TYPE_INIT_PROCESS                = 0x0300,
  XA_CMD_TYPE_INIT_DONE_QUERY             = 0x0400,

  /* XA_API_CMD_EXECUTE indices */
  XA_CMD_TYPE_DO_EXECUTE                  = 0x0100,
  XA_CMD_TYPE_DONE_QUERY                  = 0x0200,
  XA_CMD_TYPE_DO_RUNTIME_INIT             = 0x0300,
};


/*****************************************************************************/
/* Standard API configuration parameters                                     */
/*****************************************************************************/

enum xa_config_param_generic {
  XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS    = 0x0100,
  XA_CONFIG_PARAM_GEN_INPUT_STREAM_POS    = 0x0200,
};

#endif /* __XA_API_CMD_STANDARDS_H__ */
