/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _VECTORS_MCUX_H_
#define _VECTORS_MCUX_H_

#include "hardware_cfg.h"


/*******************************************************************************
* Definitions
******************************************************************************/
/*  Flash configuration and the access control table. */
#define FLASH_CONFIG_1  (0xFFFFFFFFUL) /* Back-door comparison key B0-B3. */
#define FLASH_CONFIG_2  (0xFFFFFFFFUL) /* Back-door comparison key B4-B7. */
#define FLASH_CONFIG_3  (0xFFFFFFFFUL) /* Program flash protection bytes. */
#define FLASH_CONFIG_4  (0xFFFFFFFEUL) /* Flash non-volatile option and flash security byte. */


/* The interrupt vector handler assignment table. */
#ifndef   VECTOR_001
#define   VECTOR_001  IRQHNDLR_fsDefault    /* Initial Program Counter from ARM core. */
#endif /* VECTOR_001 */

#ifndef   VECTOR_002
  #define VECTOR_002  IRQHNDLR_fsDefault    /* Non-maskable Interrupt (NMI) from ARM core. */
#endif /* VECTOR_002 */

#ifndef   VECTOR_003
  #define VECTOR_003  IRQHNDLR_fsDefault    /* Hard-fault from ARM core. */
#endif /* VECTOR_003 */

#ifndef   VECTOR_004
  #define VECTOR_004  IRQHNDLR_fsDefault
#endif /* VECTOR_004 */


#ifndef   VECTOR_005
  #define VECTOR_005  IRQHNDLR_fsDefault
#endif /* VECTOR_005 */


#ifndef   VECTOR_006
  #define VECTOR_006  IRQHNDLR_fsDefault
#endif /* VECTOR_006 */


#ifndef   VECTOR_007
  #define VECTOR_007  IRQHNDLR_fsDefault
#endif /* VECTOR_007 */


#ifndef   VECTOR_008
  #define VECTOR_008  IRQHNDLR_fsDefault
#endif /* VECTOR_008 */


#ifndef   VECTOR_009
  #define VECTOR_009  IRQHNDLR_fsDefault
#endif /* VECTOR_009 */


#ifndef   VECTOR_010
  #define VECTOR_010  IRQHNDLR_fsDefault
#endif /* VECTOR_010 */


#ifndef   VECTOR_011
  #define VECTOR_011  IRQHNDLR_fsDefault
#endif /* VECTOR_011 */


#ifndef   VECTOR_012
  #define VECTOR_012  IRQHNDLR_fsDefault
#endif /* VECTOR_012 */


#ifndef   VECTOR_013
  #define VECTOR_013  IRQHNDLR_fsDefault
#endif /* VECTOR_013 */


#ifndef   VECTOR_014
  #define VECTOR_014  IRQHNDLR_fsDefault
#endif /* VECTOR_014 */


#ifndef   VECTOR_015
  #define VECTOR_015  IRQHNDLR_fsDefault
#endif /* VECTOR_015 */


#ifndef   VECTOR_016
  #define VECTOR_016  IRQHNDLR_fsDefault
#endif /* VECTOR_016 */


#ifndef   VECTOR_017
  #define VECTOR_017  IRQHNDLR_fsDefault
#endif /* VECTOR_017 */


#ifndef   VECTOR_018
  #define VECTOR_018  IRQHNDLR_fsDefault
#endif /* VECTOR_018 */


#ifndef   VECTOR_019
  #define VECTOR_019  IRQHNDLR_fsDefault
#endif /* VECTOR_019 */


#ifndef   VECTOR_020
  #define VECTOR_020  IRQHNDLR_fsDefault
#endif /* VECTOR_020 */


#ifndef   VECTOR_021
  #define VECTOR_021  IRQHNDLR_fsDefault
#endif /* VECTOR_021 */


#ifndef   VECTOR_022
  #define VECTOR_022  IRQHNDLR_fsDefault
#endif /* VECTOR_022 */


#ifndef   VECTOR_023
  #define VECTOR_023  IRQHNDLR_fsDefault
#endif /* VECTOR_023 */


#ifndef   VECTOR_024
  #define VECTOR_024  IRQHNDLR_fsDefault
#endif /* VECTOR_024 */


#ifndef   VECTOR_025
  #define VECTOR_025  IRQHNDLR_fsDefault
#endif /* VECTOR_025 */


#ifndef   VECTOR_026
  #define VECTOR_026  IRQHNDLR_fsDefault
#endif /* VECTOR_026 */


#ifndef   VECTOR_027
  #define VECTOR_027  IRQHNDLR_fsDefault
#endif /* VECTOR_027 */


#ifndef   VECTOR_028
  #define VECTOR_028  IRQHNDLR_fsDefault
#endif /* VECTOR_028 */


#ifndef   VECTOR_029
  #define VECTOR_029  IRQHNDLR_fsDefault
#endif /* VECTOR_029 */


#ifndef   VECTOR_030
  #define VECTOR_030  IRQHNDLR_fsDefault
#endif /* VECTOR_030 */


#ifndef   VECTOR_031
  #define VECTOR_031  IRQHNDLR_fsDefault
#endif /* VECTOR_031 */


#ifndef   VECTOR_032
  #define VECTOR_032  IRQHNDLR_fsDefault
#endif /* VECTOR_032 */


#ifndef   VECTOR_033
  #define VECTOR_033  IRQHNDLR_fsDefault
#endif /* VECTOR_033 */


#ifndef   VECTOR_034
  #define VECTOR_034  IRQHNDLR_fsDefault
#endif /* VECTOR_034 */


#ifndef   VECTOR_035
  #define VECTOR_035  IRQHNDLR_fsDefault
#endif /* VECTOR_035 */


#ifndef   VECTOR_036
  #define VECTOR_036  IRQHNDLR_fsDefault
#endif /* VECTOR_036 */


#ifndef   VECTOR_037
  #define VECTOR_037  IRQHNDLR_fsDefault
#endif /* VECTOR_037 */


#ifndef   VECTOR_038
  #define VECTOR_038  IRQHNDLR_fsDefault
#endif /* VECTOR_038 */


#ifndef   VECTOR_039
  #define VECTOR_039  IRQHNDLR_fsDefault
#endif /* VECTOR_039 */


#ifndef   VECTOR_040
  #define VECTOR_040  IRQHNDLR_fsDefault
#endif /* VECTOR_040 */


#ifndef   VECTOR_041
  #define VECTOR_041  IRQHNDLR_fsDefault
#endif /* VECTOR_041 */


#ifndef   VECTOR_042
  #define VECTOR_042  IRQHNDLR_fsDefault
#endif /* VECTOR_042 */


#ifndef   VECTOR_043
  #define VECTOR_043  IRQHNDLR_fsDefault
#endif /* VECTOR_043 */


#ifndef   VECTOR_044
  #define VECTOR_044  IRQHNDLR_fsDefault
#endif /* VECTOR_044 */


#ifndef   VECTOR_045
  #define VECTOR_045  IRQHNDLR_fsDefault
#endif /* VECTOR_045 */


#ifndef   VECTOR_046
  #define VECTOR_046  IRQHNDLR_fsDefault
#endif /* VECTOR_046 */


#ifndef   VECTOR_047
  #define VECTOR_047  IRQHNDLR_fsDefault
#endif /* VECTOR_047 */


#ifndef   VECTOR_048
  #define VECTOR_048  IRQHNDLR_fsDefault
#endif /* VECTOR_048 */


#ifndef   VECTOR_049
  #define VECTOR_049  IRQHNDLR_fsDefault
#endif /* VECTOR_049 */


#ifndef   VECTOR_050
  #define VECTOR_050  IRQHNDLR_fsDefault
#endif /* VECTOR_050 */


#ifndef   VECTOR_051
  #define VECTOR_051  IRQHNDLR_fsDefault
#endif /* VECTOR_051 */


#ifndef   VECTOR_052
  #define VECTOR_052  IRQHNDLR_fsDefault
#endif /* VECTOR_052 */


#ifndef   VECTOR_053
  #define VECTOR_053  IRQHNDLR_fsDefault
#endif /* VECTOR_053 */


#ifndef   VECTOR_054
  #define VECTOR_054  IRQHNDLR_fsDefault
#endif /* VECTOR_054 */


#ifndef   VECTOR_055
  #define VECTOR_055  IRQHNDLR_fsDefault
#endif /* VECTOR_055 */


#ifndef   VECTOR_056
  #define VECTOR_056  IRQHNDLR_fsDefault
#endif /* VECTOR_056 */


#ifndef   VECTOR_057
  #define VECTOR_057  IRQHNDLR_fsDefault
#endif /* VECTOR_057 */


#ifndef   VECTOR_058
  #define VECTOR_058  IRQHNDLR_fsDefault
#endif /* VECTOR_058 */


#ifndef   VECTOR_059
  #define VECTOR_059  IRQHNDLR_fsDefault
#endif /* VECTOR_059 */


#ifndef   VECTOR_060
  #define VECTOR_060  IRQHNDLR_fsDefault
#endif /* VECTOR_060 */


#ifndef   VECTOR_061
  #define VECTOR_061  IRQHNDLR_fsDefault
#endif /* VECTOR_061 */


#ifndef   VECTOR_062
  #define VECTOR_062  IRQHNDLR_fsDefault
#endif /* VECTOR_062 */


#ifndef   VECTOR_063
  #define VECTOR_063  IRQHNDLR_fsDefault
#endif /* VECTOR_063 */


#ifndef   VECTOR_064
  #define VECTOR_064  IRQHNDLR_fsDefault
#endif /* VECTOR_064 */


#ifndef   VECTOR_065
  #define VECTOR_065  IRQHNDLR_fsDefault
#endif /* VECTOR_065 */


#ifndef   VECTOR_066
  #define VECTOR_066  IRQHNDLR_fsDefault
#endif /* VECTOR_066 */


#ifndef   VECTOR_067
  #define VECTOR_067  IRQHNDLR_fsDefault
#endif /* VECTOR_067 */


#ifndef   VECTOR_068
  #define VECTOR_068  IRQHNDLR_fsDefault
#endif /* VECTOR_068 */


#ifndef   VECTOR_069
  #define VECTOR_069  IRQHNDLR_fsDefault
#endif /* VECTOR_069 */


#ifndef   VECTOR_070
  #define VECTOR_070  IRQHNDLR_fsDefault
#endif /* VECTOR_070 */


#ifndef   VECTOR_071
  #define VECTOR_071  IRQHNDLR_fsDefault
#endif /* VECTOR_071 */


#ifndef   VECTOR_072
  #define VECTOR_072  IRQHNDLR_fsDefault
#endif /* VECTOR_072 */


#ifndef   VECTOR_073
  #define VECTOR_073  IRQHNDLR_fsDefault
#endif /* VECTOR_073 */


#ifndef   VECTOR_074
  #define VECTOR_074  IRQHNDLR_fsDefault
#endif /* VECTOR_074 */


#ifndef   VECTOR_075
  #define VECTOR_075  IRQHNDLR_fsDefault
#endif /* VECTOR_075 */


#ifndef   VECTOR_076
  #define VECTOR_076  IRQHNDLR_fsDefault
#endif /* VECTOR_076 */


#ifndef   VECTOR_077
  #define VECTOR_077  IRQHNDLR_fsDefault
#endif /* VECTOR_077 */


#ifndef   VECTOR_078
  #define VECTOR_078  IRQHNDLR_fsDefault
#endif /* VECTOR_078 */


#ifndef   VECTOR_079
  #define VECTOR_079  IRQHNDLR_fsDefault
#endif /* VECTOR_079 */


#ifndef   VECTOR_080
  #define VECTOR_080  IRQHNDLR_fsDefault
#endif /* VECTOR_080 */


#ifndef   VECTOR_081
  #define VECTOR_081  IRQHNDLR_fsDefault
#endif /* VECTOR_081 */


#ifndef   VECTOR_082
  #define VECTOR_082  IRQHNDLR_fsDefault
#endif /* VECTOR_082 */


#ifndef   VECTOR_083
  #define VECTOR_083  IRQHNDLR_fsDefault
#endif /* VECTOR_083 */


#ifndef   VECTOR_084
  #define VECTOR_084  IRQHNDLR_fsDefault
#endif /* VECTOR_084 */


#ifndef   VECTOR_085
  #define VECTOR_085  IRQHNDLR_fsDefault
#endif /* VECTOR_085 */


#ifndef   VECTOR_086
  #define VECTOR_086  IRQHNDLR_fsDefault
#endif /* VECTOR_086 */


#ifndef   VECTOR_087
  #define VECTOR_087  IRQHNDLR_fsDefault
#endif /* VECTOR_087 */


#ifndef   VECTOR_088
  #define VECTOR_088  IRQHNDLR_fsDefault
#endif /* VECTOR_088 */


#ifndef   VECTOR_089
  #define VECTOR_089  IRQHNDLR_fsDefault
#endif /* VECTOR_089 */



#endif /* _VECTORS_MCUX_H_ */
