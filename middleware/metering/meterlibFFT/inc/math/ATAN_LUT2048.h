/************************************************************************************************
*
* Copyright 2013-2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*
****************************************************************************//*!
*
* @file      ATAN_LUT2048.h
*
* @author    B19186
* 
* @version   3.0.0.1
* 
* @date      June-5-2015
* 
* @brief     Header file for arctan computing
*
*******************************************************************************/
#ifndef __ATAN_LUT2048_H__
#define __ATAN_LUT2048_H__

/*******************************************************************************
 * user data type & macro definitions                                          *
 ******************************************************************************/
/*! Arctan LUT - data in this LUT are in 1/2048° (up to 45.000°)              */
#define ATAN_SIZE     2048    ///< arctan LUT size (number of points in the LUT)

extern const unsigned long AtanLUT[];

/*******************************************************************************
 * exported function prototypes                                                *
 ******************************************************************************/
/***************************************************************************//*!
*
* @brief  Two-argument inverse tangent computing
*
* @param   x_re   Real part of the input signed number in the Cartesian form (Q0.24 format)
* @param   x_im   Imaginary part of the input signed number in the Cartesian form (Q0.24 format)
*
* @return  Arctan in thousandth degree <0..360000) or (-180000..+180000> selected by @ref TWO_PI
*
* @remarks Computes the arctangent of x_im/x_re in four quadrants. This computing method is based 
*          on searching in LUT with 24-bit input data range.
*
*******************************************************************************/
extern long Atan2(frac24 x_re, frac24 x_im);

#endif /* __ATAN_LUT2048_H__ */
