/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LIN_TYPES_H
#define LIN_TYPES_H

#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Unsigned 8 bit integer
 * Implements : l_u8_Class
 */
typedef unsigned char l_u8;

/*!
 * @brief Unsigned 16 bit integer
 * Implements : l_u16_Class
 */
typedef unsigned short int l_u16;

/*!
 * @brief Unsigned 32 bit integer
 * Implements : l_u32_Class
 */
typedef unsigned long l_u32;

/*!
 * @brief 0 is false, and non-zero (>0) is true
 * Implements : l_bool_Class
 */
typedef bool l_bool;

/*******************************************************************************
 * General
 ******************************************************************************/
/*!
 * @brief Product id structure
 * Implements : lin_product_id_t_Class
 */
typedef struct
{
    l_u16 supplier_id; /*!< Supplier ID */
    l_u16 function_id; /*!< Function ID */
    l_u8 variant;      /*!< Variant value */
} lin_product_id_t;

#endif /* LIN_TYPES_H */

/******************************************************************************/
/* EOF */
/******************************************************************************/
