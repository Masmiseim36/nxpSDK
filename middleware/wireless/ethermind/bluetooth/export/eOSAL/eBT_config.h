
/**
 *  \file eBT_config.h
 *
 *  Header File for EtherMind Configuration File module.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EBT_CONFIG_
#define _H_EBT_CONFIG_

/* --------------------------------------------- Header File Inclusion */
#include "EM_config.h"

/* --------------------------------------------- Global Definitions */
/** Maximum Characters per line in Config File */
#define BT_CONFIG_MAX_CHARS_PER_LINE            EM_CONFIG_MAX_CHARS_PER_LINE

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#define BT_config_read(fn, tf, buf)             EM_config_read((fn), (tf), (buf))

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

#endif /* _H_EBT_CONFIG_ */

