
/**
 *  \file ethal_config.h
 *
 *  Header File for EtherMind Configuration File module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_ETHAL_CONFIG_
#define _H_ETHAL_CONFIG_

/* ------------------------------------------ Header File Inclusion */
#include "EM_os.h"

/* ------------------------------------------ Type Definitions */
/** Maximum Characters per line in Config File */
#define EM_CONFIG_MAX_CHARS_PER_LINE            128U


/* ------------------------------------------ API/Function Declarations */
/**
 *  An utility routine to read Configuration Parameter, related to
 *  EtherMind Bluetooth Protocol Stack from a Configuration File.
 *
 *
 *  Rules for writing the Configuration File so that this utility can
 *  access, read & work properly:
 *  (1) Comments start with '#', or, "//" on Column '0'
 *
 *  (2) Every Configuration Parameter must be written in the following
 *      format:
 *               <Token> = <Token Value>
 *
 *      The first character of <Token> should must on Column '0'.
 *
 *  (3) There can be any amount of blank space/tab before & after the '='
 *
 *  (4) Maximum characters per line in the Configuration File must not
 *      exceed 'EM_CONFIG_MAX_CHARS_PER_LINE'.
 *
 *
 *  Return Values:
 *      0  : On success.
 *      -1 : Configuration File could not be opened
 *      -2 : EOF encountered (without getting the config token)
 *      -3 : Config Token ('to_find') located, but failed to retrieve the
 *           Token (Configuration Parameter) value.
 */
INT32 EM_config_read
      (
          /* IN */ CHAR *    filename,
          /* IN */ CHAR *    to_find,
          /* IN */ CHAR *    buffer
      );

#endif /* _H_ETHAL_CONFIG_ */

