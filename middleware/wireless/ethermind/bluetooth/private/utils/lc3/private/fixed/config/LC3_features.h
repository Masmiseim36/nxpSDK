
/**
 *  \file lc3_features.h
 *
 *  \brief LC3 Compilation Switches Configuration header file.
 *
 *  This file lists all the Compilation Flags available in LC3 module.
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_LC3_FEATURES_
#define _H_LC3_FEATURES_


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
 *  Note : Disable for LPDSP32
 */
//#define LC3_ENABLE_C4_SMLAD



#define LC3_HAVE_NB
/*
 *  LC3_HAVE_NB
 *
 *  This flag is used to enable support for sampling rate 8 kHz.
 *
 *  Dependency: None
 */

#define LC3_HAVE_WB
/*
 *  LC3_HAVE_WB
 *
 *  This flag is used to enable support for sampling rate 16 kHz.
 *
 *  Dependency: None
 */

#define LC3_HAVE_SSWB
/*
 *  LC3_HAVE_SSWB
 *
 *  This flag is used to enable support for sampling rate 24 kHz.
 *
 *  Dependency: None
 */

#define LC3_HAVE_SWB
/*
 *  LC3_HAVE_SWB
 *
 *  This flag is used to enable support for sampling rate 32 kHz.
 *
 *  Dependency: None
 */

#define LC3_HAVE_FB
/*
 *  LC3_HAVE_FB
 *
 *  This flag is used to enable support for sampling rate 44.1 or 48 kHz.
 *
 *  Dependency: None
 */


/* Check atleast one among samplerate config is defined */
#if !(defined(LC3_HAVE_NB) || defined(LC3_HAVE_WB) || defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_SWB) || defined(LC3_HAVE_FB))
#error "Atleast one of LC3_HAVE_NB, LC3_HAVE_WB, LC3_HAVE_SSWB, LC3_HAVE_SWB, LC3_HAVE_FB shall be defined"
#endif


#endif /* _H_LC3_FEATURES_ */

