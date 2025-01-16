
/**
 *  \file lc3_ndsp_features.h
 *
 *  \brief LC3 Compilation Switches Configuration header file.
 *
 *  This file lists all the Compilation Flags available in LC3 module.
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */

#if defined(LC3_DSP) && (LC3_DSP == 0)
#ifndef _H_LC3_NDSP_FEATURES_
#define _H_LC3_NDSP_FEATURES_


/* ----------------------------------------------------------------------- */
/* ======== LC3 Specification Flags ====================================== */
/* ----------------------------------------------------------------------- */

/*
 *  LC3_ENCODER_SUPPORT
 *
 *  This flag is used to enable LC3 encoder funcationality.
 *
 *  Dependency: None
 */
#define LC3_ENCODER_SUPPORT

/*
 *  LC3_DECODER_SUPPORT
 *
 *  This flag is used to enable LC3 decoder funcationality.
 *
 *  Dependency: None
 */
#define LC3_DECODER_SUPPORT

/* Check atleast one among Encoder and Decoder flag is defined */
#if (!((defined LC3_ENCODER_SUPPORT) || (defined LC3_DECODER_SUPPORT)))
#error "Atleast one of LC3_ENCODER_SUPPORT, LC3_DECODER_SUPPORT shall be defined"
#endif /* (!((defined LC3_ENCODER_SUPPORT) || (defined LC3_DECODER_SUPPORT))) */

/*
 *  LC3_ENABLE_7_5MS_MODE
 *
 *  This flag is used to enable support for 7.5ms frame duration.
 *
 *  Dependency: None
 */
#define LC3_ENABLE_7_5MS_MODE

/*
 *  LC3_HAVE_24_BPS_SUPPORT
 *
 *  This flag is used to enable support for 24 bits per audio sample.
 *
 *  Dependency: None
 */
#define LC3_HAVE_24_BPS_SUPPORT

/*
 *  LC3_HAVE_32_BPS_SUPPORT
 *
 *  This flag is used to enable support for 32 bits per audio sample.
 *
 *  Dependency: None
 */
#define LC3_HAVE_32_BPS_SUPPORT

/*
 *  LC3_ENABLE_STANDARD_PLC
 *
 *  This flag is used to enable support for LC3 specification defined
 *  standard Packet Loss Concealment algorithm.
 *
 *  Dependency: None
 */
#define LC3_ENABLE_STANDARD_PLC

/*
 *  LC3_ENABLE_C4_SMLAD
 *
 *  This flag is used to enable ARM Cortex-M4 SMLAD [dual 16x16 mac SIMD] instruction usage.
 *
 *  Dependency: None
 */
#define LC3_ENABLE_C4_SMLAD


/*
 *  LC3_CHANNELS_MAX
 *
 *  Define the max channles of a CIG/BIG
 *
 *  Dependency: None
 */
#define  LC3_CHANNELS_MAX 		2


#endif /* _H_LC3_NDSP_FEATURES_ */
#endif /*defined(LC3_DSP) && (LC3_DSP == 0)*/
