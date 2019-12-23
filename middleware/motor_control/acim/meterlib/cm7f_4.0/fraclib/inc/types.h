/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      types.h
 * @version   1.0.0.0
 * @date      Jan-27-2015
 * @brief     Header file.
 ******************************************************************************/
#ifndef __TYPES_H
#define __TYPES_H

/******************************************************************************
 * Common macros and defines                                                  *
 ******************************************************************************/
#ifndef NULL
  #define NULL    (void*)0
#endif
#define TRUE    1
#define FALSE   0

/******************************************************************************
 * List of the basic data types
 *
 *//*! @addtogroup basic_data_types
 * @{
 ******************************************************************************/
typedef unsigned char       uint8;    ///< 8-bit unsigned integer
typedef unsigned short int  uint16;   ///< 16-bit unsigned integer
typedef unsigned long int   uint32;   ///< 32-bit unsigned integer
typedef unsigned long long  uint64;   ///< 64-bit unsigned integer

typedef signed char         int8;     ///< 8-bit signed integer
typedef short int           int16;    ///< 16-bit signed integer
typedef int                 int32;    ///< 32-bit signed integer
typedef long long           int64;    ///< 64-bit signed integer

typedef short int           frac16;   ///< Q0.15  fractional
typedef long                frac24;   ///< Q8.23  fractional
typedef long                frac32;   ///< Q0.31  fractional
typedef long long           frac48;   ///< Q16.47 fractional
typedef long long           frac64;   ///< Q0.63  fractional
/*! @} End of basic_data_types                                                */

/******************************************************************************
 * List of basic macros for conversions from fractional data type to double and
 * vice versa.
 *
 *//*! @addtogroup conversion_macros
 * @{
 ******************************************************************************/
/******************************************************************************
 * common fractional conversion macro defintions                              *
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Converts double precision 64-bit floating point value to Qn.15
 *          fractional value.
 * @details This macro converts double precision 64-bit floating point value
 *          to Qn.15 fractional value.
 * @param   x     Double precision 64-bit floating point value.
 * @note    Implemented as an inline macro.
 * @see     @ref F16TODBL
 ******************************************************************************/
#define FRAC16(x)   (frac64)((x)*(((x)>0)?0x7fff:0x8000))

/***************************************************************************//*!
 * @brief   Converts Qn.15 fractional value to double precision 64-bit floating
 *          point value.
 * @details This macro converts Qn.15 fractional value to double precision
 *          64-bit floating point value.
 * @param   x     Qn.15 fractional value.
 * @note    Implemented as an inline macro.
 * @see     @ref FRAC16
 ******************************************************************************/
#define F16TODBL(x) (double)(((double)(x))/(double)0x8000)

/***************************************************************************//*!
 * @brief   Converts double precision 64-bit floating point value to Qn.23
 *          fractional value.
 * @details This macro converts double precision 64-bit floating point value
 *          to Qn.23 fractional value.
 * @param   x     Double precision 64-bit floating point value.
 * @note    Implemented as an inline macro.
 * @see     @ref F24TODBL
 ******************************************************************************/
#define FRAC24(x)   (frac64)((x)*(((x)>0)?0x7fffff:0x800000))

/***************************************************************************//*!
 * @brief   Converts Qn.23 fractional value to double precision 64-bit floating
 *          point value.
 * @details This macro converts Qn.23 fractional value to double precision
 *          64-bit floating point value.
 * @param   x     Qn.23 fractional value.
 * @note    Implemented as an inline macro.
 * @see     @ref FRAC24
 ******************************************************************************/
#define F24TODBL(x) (double)(((double)(x))/(double)0x800000)

/***************************************************************************//*!
 * @brief   Converts double precision 64-bit floating point value to Qn.31
 *          fractional value.
 * @details This macro converts double precision 64-bit floating point value
 *          to Qn.31 fractional value.
 * @param   x     Double precision 64-bit floating point value.
 * @note    Implemented as an inline macro.
 * @see     @ref F32TODBL
 ******************************************************************************/
#define FRAC32(x)   (frac64)((x)*(((x)>0)?0x7fffffff:0x80000000))

/***************************************************************************//*!
 * @brief   Converts Qn.31 fractional value to double precision 64-bit floating
 *          point value.
 * @details This macro converts Qn.31 fractional value to double precision
 *          64-bit floating point value.
 * @param   x     Qn.31 fractional value.
 * @see     @ref FRAC32
 ******************************************************************************/
#define F32TODBL(x) (double)(((double)(x))/(double)0x80000000)

/***************************************************************************//*!
 * @brief   Converts double precision 64-bit floating point value to Qn.47
 *          fractional value.
 * @details This macro converts double precision 64-bit floating point value
 *          to Qn.47 fractional value.
 * @param   x     Double precision 64-bit floating point value.
 * @note    Implemented as an inline macro.
 * @see     @ref F24TODBL
 ******************************************************************************/
#define FRAC48(x)   (frac64)((x)*(((x)>0)?0x7fffffffffff:0x800000000000))

/***************************************************************************//*!
 * @brief   Converts Qn.47 fractional value to double precision 64-bit floating
 *          point value.
 * @details This macro converts Qn.47 fractional value to double precision
 *          64-bit floating point value.
 * @param   x     Qn.47 fractional value.
 * @note    Implemented as an inline macro.
 * @see     @ref FRAC48
 ******************************************************************************/
#define F48TODBL(x) (double)(((double)(x))/(double)0x800000000000)

/***************************************************************************//*!
 * @brief   Converts double precision 64-bit floating point value to Q0.63
 *          fractional value.
 * @details This macro converts double precision 64-bit floating point value
 *          to the Q0.63 fractional value.
 * @param   x     Double precision 64-bit floating point value.
 * @note    Note that this macro doesn't work correctly for floating point
 *          values close to 1.0 and -1.0. Implemented as an inline macro.
 * @see     @ref F64TODBL
 ******************************************************************************/
#define FRAC64(x)   (frac64)((x)*(((x)>0)?0x7fffffffffffffff:0x8000000000000000))

/***************************************************************************//*!
 * @brief   Converts Q0.63 fractional value to double precision 64-bit floating
 *          point value.
 * @details This macro converts Q0.63 fractional value to double precision
 *          64-bit floating point value.
 * @param   x     Q0.63 fractional value.
 * @note    Implemented as an inline macro.
 * @see     @ref FRAC64
 ******************************************************************************/
#define F64TODBL(x) (double)(((double)(x))/(double)0x8000000000000000)

/*! @} End of conversion_macros                                               */

#endif /* __TYPES_H */
