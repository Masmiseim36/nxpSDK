/**************************************************************************
*
* Copyright 2016-2018 by Andrey Butok. FNET Community.
* Copyright 2018 NXP. Not a Contribution
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#ifndef _FNET_TYPES_H_
#define _FNET_TYPES_H_


/*********************************************************************
*
* The basic data types.
*
*********************************************************************/
typedef unsigned char fnet_uint8_t;         /*  8 bits */
typedef unsigned short int fnet_uint16_t;   /* 16 bits */
typedef unsigned long int fnet_uint32_t;    /* 32 bits */
typedef unsigned long long fnet_uint64_t;   /* 64 bits */

typedef signed char fnet_int8_t;            /*  8 bits */
typedef signed short int fnet_int16_t;      /* 16 bits */
typedef signed long int fnet_int32_t;       /* 32 bits */
typedef signed long long fnet_int64_t;      /* 64 bits */

typedef volatile fnet_uint8_t fnet_vuint8_t;     /*  8 bits */
typedef volatile fnet_uint16_t fnet_vuint16_t;   /* 16 bits */
typedef volatile fnet_uint32_t fnet_vuint32_t;   /* 32 bits */
typedef volatile fnet_uint64_t fnet_vuint64_t;   /* 64 bits */




/**************************************************************************/ /*!
 * @def FNET_NULL
 * @brief NULL pointer.
 * @showinitializer
 ******************************************************************************/
#define FNET_NULL       ((void *)0)

/**************************************************************************/ /*!
 * @def FNET_RAND_MAX
 * @brief The maximum number returned by @ref fnet_rand().
 * @showinitializer
 ******************************************************************************/
#define FNET_RAND_MAX (32767u)

/**************************************************************************/ /*!
 * @brief Unsigned integer type representing the size in bytes.
 ******************************************************************************/
typedef unsigned long fnet_size_t;

/**************************************************************************/ /*!
 * @brief Signed integer type representing the size in bytes.
 ******************************************************************************/
typedef long fnet_ssize_t;

/**************************************************************************/ /*!
 * @brief Unsigned integer type representing the bit flag.
 ******************************************************************************/
typedef unsigned int fnet_flag_t;

/**************************************************************************/ /*!
 * @brief Unsigned integer type representing the index.
 ******************************************************************************/
typedef unsigned int fnet_index_t;

/**************************************************************************/ /*!
 * @brief Type representing the charecter.
 ******************************************************************************/
typedef char fnet_char_t;

/**************************************************************************/ /*!
 * @brief Boolean type.
 ******************************************************************************/
typedef enum
{
    FNET_FALSE = 0, /**< FALSE Boolean value.*/
    FNET_TRUE  = 1  /**< TRUE Boolean value.*/
} fnet_bool_t;

/**************************************************************************/ /*!
 * @brief General return codes, used by most of API functions.
 ******************************************************************************/
typedef enum
{
    FNET_OK  = (0), /**< No error.*/
    FNET_ERR = (-1) /**< There is error.*/
} fnet_return_t;



#endif /* _FNET_TYPES_H_ */

