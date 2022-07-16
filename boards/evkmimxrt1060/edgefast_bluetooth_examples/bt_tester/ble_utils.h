/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019, 2021-2022 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
/* Shift definitions */
#define SHIFT0      (0U)
#define SHIFT1      (1U)
#define SHIFT2      (2U)
#define SHIFT3      (3U)
#define SHIFT4      (4U)
#define SHIFT5      (5U)
#define SHIFT6      (6U)
#define SHIFT7      (7U)
#define SHIFT8      (8U)
#define SHIFT9      (9U)
#define SHIFT10     (10U)
#define SHIFT11     (11U)
#define SHIFT12     (12U)
#define SHIFT13     (13U)
#define SHIFT14     (14U)
#define SHIFT15     (15U)
#define SHIFT16     (16U)
#define SHIFT17     (17U)
#define SHIFT18     (18U)
#define SHIFT19     (19U)
#define SHIFT20     (20U)
#define SHIFT21     (21U)
#define SHIFT22     (22U)
#define SHIFT23     (23U)
#define SHIFT24     (24U)
#define SHIFT25     (25U)
#define SHIFT26     (26U)
#define SHIFT27     (27U)
#define SHIFT28     (28U)
#define SHIFT29     (29U)
#define SHIFT30     (30U)
#define SHIFT31     (31U)

#define SHIFT32     (32U)
#define SHIFT33     (33U)
#define SHIFT34     (34U)
#define SHIFT35     (35U)
#define SHIFT36     (36U)
#define SHIFT37     (37U)
#define SHIFT38     (38U)
#define SHIFT39     (39U)
#define SHIFT40     (40U)
#define SHIFT41     (41U)
#define SHIFT42     (42U)
#define SHIFT43     (43U)
#define SHIFT44     (44U)
#define SHIFT45     (45U)
#define SHIFT46     (46U)
#define SHIFT47     (47U)
#define SHIFT48     (48U)
#define SHIFT49     (49U)
#define SHIFT50     (50U)
#define SHIFT51     (51U)
#define SHIFT52     (52U)
#define SHIFT53     (53U)
#define SHIFT54     (54U)
#define SHIFT55     (55U)
#define SHIFT56     (56U)
#define SHIFT57     (57U)
#define SHIFT58     (58U)
#define SHIFT59     (59U)
#define SHIFT60     (60U)
#define SHIFT61     (61U)
#define SHIFT62     (62U)
#define SHIFT63     (63U)

/*! Returns a uint16_t from a buffer, little-endian */
#define Utils_ExtractTwoByteValue(buf) \
    (((uint16_t)(*(buf))) | ( ((uint16_t)(buf)[1]) << 8U) )

/*! Returns a 3-byte value from a buffer, little-endian */
#define Utils_ExtractThreeByteValue(buf) \
( \
    (*(buf)) \
    | ( (*((buf) + 1U)) << 8U) \
    | ( (*((buf) + 2U)) << 16U) \
)

/*! Returns a uint32_t from a buffer, little-endian */
#define Utils_ExtractFourByteValue(buf) \
( \
    (*(buf)) \
    | ( ((uint32_t)(buf)[1]) << 8U) \
    | ( ((uint32_t)(buf)[2]) << 16U) \
    | ( ((uint32_t)(buf)[3]) << 24U) \
)

/*! Writes a uint16_t into a buffer, little-endian */
#define Utils_PackTwoByteValue(value, buf) \
{ \
    (buf)[0] = (uint8_t) ((value) & 0xFFU); \
    (buf)[1] = (uint8_t) (((uint16_t)(value) >> 8U) & 0xFFU); \
}

/*! Writes a 3-byte value into a buffer, little-endian */
#define Utils_PackThreeByteValue(value, buf) \
{ \
    (buf)[0] = (uint8_t) ((value) & 0xFFU); \
    (buf)[1] = (uint8_t) (((value) >> 8U) & 0xFFU); \
    (buf)[2] = (uint8_t) (((value) >> 16U) & 0xFFU); \
}

/*! Writes a uint32_t into a buffer, little-endian */
#define Utils_PackFourByteValue(value, buf) \
{ \
    (buf)[0] = (uint8_t) ((value) & 0xFFU); \
    (buf)[1] = (uint8_t) (((value) >> 8U) & 0xFFU); \
    (buf)[2] = (uint8_t) (((value) >> 16U) & 0xFFU); \
    (buf)[3] = (uint8_t) (((value) >> 24U) & 0xFFU); \
}

/*! Writes a uint8_t into a buffer, little-endian, and increments the pointer.*/
#define Utils_Copy8(ptr, val8) \
        {   *(ptr) = (uint8_t)(val8); \
             (ptr)++; }

/*! Writes a uint16_t into a buffer, little-endian, and increments the pointer. */
#define Utils_Copy16(ptr, val16) \
        {   *(ptr) = (uint8_t)(val16);\
             (ptr)++; \
            *(ptr) = (uint8_t)(((uint16_t)(val16)) >> SHIFT8);\
             (ptr)++; }

/*! Writes a uint32_t into a buffer, little-endian, and increments the pointer. */
#define Utils_Copy32(ptr, val32) \
        {   *(ptr) = (uint8_t)(val32);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val32) >> SHIFT8);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val32) >> SHIFT16);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val32) >> SHIFT24);\
             (ptr)++; }

/*! Writes a uint64_t into a buffer, little-endian, and increments the pointer. */
#define Utils_Copy64(ptr, val64) \
        {   *(ptr) = (uint8_t)(val64);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val64) >> SHIFT8);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val64) >> SHIFT16);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val64) >> SHIFT24);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val64) >> SHIFT32);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val64) >> SHIFT40);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val64) >> SHIFT48);\
             (ptr)++; \
            *(ptr) = (uint8_t)((val64) >> SHIFT56);\
             (ptr)++; }

/*! Reverts the order of bytes in an array - useful for changing the endianness */
#define Utils_RevertByteArray(array, size) \
{ \
    uint8_t temp; \
    for (uint32_t j = 0U; j < (uint32_t)(size)/2U; j++) \
    { \
        temp = (array)[j]; \
        (array)[j] = (array)[(size) - 1U - j]; \
        (array)[(size) - 1U - j] = temp; \
    } \
}

/*! Value of x rounded up to the next multiple of align, which must be a power of 2. */
#define ROUND_UP(x, align)                                   \
    (((unsigned long)(x) + ((unsigned long)(align) - 1)) & \
     ~((unsigned long)(align) - 1))

/*! *********************************************************************************
* @}
********************************************************************************** */
