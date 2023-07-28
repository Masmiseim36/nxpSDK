/*!
 * \file
 *
 * Software implementation of AES and CCM* functionality. The functions are avaliable to
 * all higher layers. The AES module is not reentrant.
 *
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2022 NXP
 *
 * NXP Confidential Proprietary
 *
 * No part of this document must be reproduced in any form - including copied,
 * transcribed, printed or by any electronic means - without specific written
 * permission from NXP.
 *
 */
#include "EmbeddedTypes.h"
#include "FunctionLib.h"
#include "SecLib.h"
#include "CryptoLibSW.h"

/*
 * AES encryption engine - Without Hardware Support
 *
 * Input: 128bit key, and 128bit data, d0,d1,d2,...,d13,d14,d15
 * Output 128bit encrypted data cipher
 */

/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
******************************************************************************/

// Block size in AES128 (128 bit, 16 bytes)
#define gCcmBlockSize_c 16u

#define CalcGf2(input) (input & 0x80) ? ((input << 1) ^ 0x1B) : (input << 1)
#define CalcGf3(input) (input & 0x80) ? ((input << 1) ^ input ^ 0x1B) : ((input << 1) ^ input)

/*
 * Select xtime implementation. Table lookup is recommended!
 */
#define xtime(x)   gpXTimeTable[x]
#define xtime4(x)  (xtime(xtime(x)))
#define xtime8(x)  (xtime(xtime4(x)))
#define xtime9(x)  (xtime8(x) ^ (x))
#define xtime11(x) (xtime8(x) ^ xtime(x) ^ (x))
#define xtime13(x) (xtime8(x) ^ xtime4(x) ^ (x))
#define xtime14(x) (xtime8(x) ^ xtime4(x) ^ xtime(x))
//#define xtime(x) CalcGf2(x)
//#define xtime(x) FuncAsmCalcGf2(x)

#define CCM_AesCipherInplace(pKey, pData) AES_128_Encrypt(pData, (uint8_t *)pKey, pData)

/******************************************************************************
*******************************************************************************
* Private memory definitions
*******************************************************************************
******************************************************************************/

/* ROM DATA */
static const uint8_t gpXTimeTable[256] = {
    0,   2,   4,   6,   8,   10,  12,  14,  16,  18,  20,  22,  24,  26,  28,  30,  32,  34,  36,  38,  40,  42,
    44,  46,  48,  50,  52,  54,  56,  58,  60,  62,  64,  66,  68,  70,  72,  74,  76,  78,  80,  82,  84,  86,
    88,  90,  92,  94,  96,  98,  100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130,
    132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174,
    176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218,
    220, 222, 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254, 27,  25,  31,  29,
    19,  17,  23,  21,  11,  9,   15,  13,  3,   1,   7,   5,   59,  57,  63,  61,  51,  49,  55,  53,  43,  41,
    47,  45,  35,  33,  39,  37,  91,  89,  95,  93,  83,  81,  87,  85,  75,  73,  79,  77,  67,  65,  71,  69,
    123, 121, 127, 125, 115, 113, 119, 117, 107, 105, 111, 109, 99,  97,  103, 101, 155, 153, 159, 157, 147, 145,
    151, 149, 139, 137, 143, 141, 131, 129, 135, 133, 187, 185, 191, 189, 179, 177, 183, 181, 171, 169, 175, 173,
    163, 161, 167, 165, 219, 217, 223, 221, 211, 209, 215, 213, 203, 201, 207, 205, 195, 193, 199, 197, 251, 249,
    255, 253, 243, 241, 247, 245, 235, 233, 239, 237, 227, 225, 231, 229};

static const uint8_t gSBox_c[256] = {
    99,  124, 119, 123, 242, 107, 111, 197, 48,  1,   103, 43,  254, 215, 171, 118, 202, 130, 201, 125, 250, 89,
    71,  240, 173, 212, 162, 175, 156, 164, 114, 192, 183, 253, 147, 38,  54,  63,  247, 204, 52,  165, 229, 241,
    113, 216, 49,  21,  4,   199, 35,  195, 24,  150, 5,   154, 7,   18,  128, 226, 235, 39,  178, 117, 9,   131,
    44,  26,  27,  110, 90,  160, 82,  59,  214, 179, 41,  227, 47,  132, 83,  209, 0,   237, 32,  252, 177, 91,
    106, 203, 190, 57,  74,  76,  88,  207, 208, 239, 170, 251, 67,  77,  51,  133, 69,  249, 2,   127, 80,  60,
    159, 168, 81,  163, 64,  143, 146, 157, 56,  245, 188, 182, 218, 33,  16,  255, 243, 210, 205, 12,  19,  236,
    95,  151, 68,  23,  196, 167, 126, 61,  100, 93,  25,  115, 96,  129, 79,  220, 34,  42,  144, 136, 70,  238,
    184, 20,  222, 94,  11,  219, 224, 50,  58,  10,  73,  6,   36,  92,  194, 211, 172, 98,  145, 149, 228, 121,
    231, 200, 55,  109, 141, 213, 78,  169, 108, 86,  244, 234, 101, 122, 174, 8,   186, 120, 37,  46,  28,  166,
    180, 198, 232, 221, 116, 31,  75,  189, 139, 138, 112, 62,  181, 102, 72,  3,   246, 14,  97,  53,  87,  185,
    134, 193, 29,  158, 225, 248, 152, 17,  105, 217, 142, 148, 155, 30,  135, 233, 206, 85,  40,  223, 140, 161,
    137, 13,  191, 230, 66,  104, 65,  153, 45,  15,  176, 84,  187, 22

};
static const uint8_t gInvSbox_s[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB, 0x7C, 0xE3, 0x39,
    0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB, 0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2,
    0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E, 0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76,
    0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25, 0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC,
    0x5D, 0x65, 0xB6, 0x92, 0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D,
    0x84, 0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06, 0xD0, 0x2C,
    0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B, 0x3A, 0x91, 0x11, 0x41, 0x4F,
    0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73, 0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85,
    0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E, 0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62,
    0x0E, 0xAA, 0x18, 0xBE, 0x1B, 0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD,
    0x5A, 0xF4, 0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F, 0x60,
    0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF, 0xA0, 0xE0, 0x3B, 0x4D,
    0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6,
    0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};

static uint8_t gsState[gCcmBlockSize_c];
static uint8_t gsKey[gCcmBlockSize_c];
static uint8_t gsExpandedKey[10][gCcmBlockSize_c];

/******************************************************************************
*******************************************************************************
* Public function prototypes
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private function prototypes
*******************************************************************************
******************************************************************************/
static void AES_AddKeyByteSubShiftKeyExpand(uint8_t round);
static void AES_MixColumn(void);
static void AES_ExpandKey(uint8_t *prevRound, uint8_t *currentRound, uint8_t round);
static void AES_ExpandForDecryption(void);
static void AES_InverseMixColumn(void);
static void AES_InverseShiftSubBytes(void);
/******************************************************************************
*******************************************************************************
* Private functions
*******************************************************************************
******************************************************************************/
static void AES_ExpandKey(uint8_t *prevRound, uint8_t *currentRound, uint8_t round)
{
    currentRound[0]  = prevRound[0] ^ gSBox_c[prevRound[7]] ^ round;
    currentRound[1]  = prevRound[1] ^ currentRound[0];
    currentRound[2]  = prevRound[2] ^ currentRound[1];
    currentRound[12] = prevRound[12] ^ gSBox_c[prevRound[3]];
    currentRound[3]  = prevRound[3] ^ currentRound[2];
    currentRound[4]  = prevRound[4] ^ gSBox_c[prevRound[11]];
    currentRound[5]  = prevRound[5] ^ currentRound[4];
    currentRound[6]  = prevRound[6] ^ currentRound[5];
    currentRound[7]  = prevRound[7] ^ currentRound[6];
    currentRound[8]  = prevRound[8] ^ gSBox_c[prevRound[15]];
    currentRound[9]  = prevRound[9] ^ currentRound[8];
    currentRound[10] = prevRound[10] ^ currentRound[9];
    currentRound[11] = prevRound[11] ^ currentRound[10];
    currentRound[13] = prevRound[13] ^ currentRound[12];
    currentRound[14] = prevRound[14] ^ currentRound[13];
    currentRound[15] = prevRound[15] ^ currentRound[14];
}
static void AES_ExpandForDecryption(void)
{
    uint8_t rounds[] = {1, 2, 4, 8, 16, 32, 64, 128, 27, 54};
    uint8_t index;
    AES_ExpandKey(gsKey, gsExpandedKey[0], rounds[0]);
    for (index = 1u; index < 10u; index++)
    {
        AES_ExpandKey(gsExpandedKey[index - 1u], gsExpandedKey[index], rounds[index]);
    }
}

static void AES_AddKeyByteSubShiftKeyExpand(uint8_t round)
{
    uint8_t gsTmp;
    // Add round key
    // S-Box lookup
    // Transformation
    // Expand key
    gsState[0] = gSBox_c[gsState[0] ^ gsKey[0]];
    gsState[1] = gSBox_c[gsState[1] ^ gsKey[1]];
    gsState[2] = gSBox_c[gsState[2] ^ gsKey[2]];
    gsState[3] = gSBox_c[gsState[3] ^ gsKey[3]];

    gsTmp      = gsState[4] ^ gsKey[4];
    gsState[4] = gSBox_c[gsState[5] ^ gsKey[5]];
    gsState[5] = gSBox_c[gsState[6] ^ gsKey[6]];
    gsState[6] = gSBox_c[gsState[7] ^ gsKey[7]];
    gsState[7] = gSBox_c[gsTmp];

    gsTmp       = gsState[8] ^ gsKey[8];
    gsState[8]  = gSBox_c[gsState[10] ^ gsKey[10]];
    gsState[10] = gSBox_c[gsTmp];
    gsTmp       = gsState[9] ^ gsKey[9];
    gsState[9]  = gSBox_c[gsState[11] ^ gsKey[11]];
    gsState[11] = gSBox_c[gsTmp];

    gsTmp       = gsState[15] ^ gsKey[15];
    gsState[15] = gSBox_c[gsState[14] ^ gsKey[14]];
    gsState[14] = gSBox_c[gsState[13] ^ gsKey[13]];
    gsState[13] = gSBox_c[gsState[12] ^ gsKey[12]];
    gsState[12] = gSBox_c[gsTmp];

    // Expand key
    AES_ExpandKey(gsKey, gsKey, round);
}

/*****************************************************************************/

static void AES_MixColumn(void)
{
    uint8_t gsTmp, gsTmp2;
    // Mix
    gsTmp2 = gsState[0] ^ gsState[4] ^ gsState[8] ^ gsState[12];
    gsTmp  = gsState[0];
    gsState[0] ^= xtime((gsState[0] ^ gsState[4])) ^ gsTmp2;
    gsState[4] ^= xtime(gsState[4] ^ gsState[8]) ^ gsTmp2;
    gsState[8] ^= xtime(gsState[8] ^ gsState[12]) ^ gsTmp2;
    gsState[12] ^= xtime(gsState[12] ^ gsTmp) ^ gsTmp2;
    // Mix
    gsTmp  = gsState[0 + 1];
    gsTmp2 = gsState[0 + 1] ^ gsState[4 + 1] ^ gsState[8 + 1] ^ gsState[12 + 1];
    gsState[0 + 1] ^= xtime(gsState[0 + 1] ^ gsState[4 + 1]) ^ gsTmp2;
    gsState[4 + 1] ^= xtime(gsState[4 + 1] ^ gsState[8 + 1]) ^ gsTmp2;
    gsState[8 + 1] ^= xtime(gsState[8 + 1] ^ gsState[12 + 1]) ^ gsTmp2;
    gsState[12 + 1] ^= xtime(gsState[12 + 1] ^ gsTmp) ^ gsTmp2;
    // Mix
    gsTmp  = gsState[0 + 2];
    gsTmp2 = gsState[0 + 2] ^ gsState[4 + 2] ^ gsState[8 + 2] ^ gsState[12 + 2];
    gsState[0 + 2] ^= xtime(gsState[0 + 2] ^ gsState[4 + 2]) ^ gsTmp2;
    gsState[4 + 2] ^= xtime(gsState[4 + 2] ^ gsState[8 + 2]) ^ gsTmp2;
    gsState[8 + 2] ^= xtime(gsState[8 + 2] ^ gsState[12 + 2]) ^ gsTmp2;
    gsState[12 + 2] ^= xtime(gsState[12 + 2] ^ gsTmp) ^ gsTmp2;
    // Mix
    gsTmp  = gsState[0 + 3];
    gsTmp2 = gsState[0 + 3] ^ gsState[4 + 3] ^ gsState[8 + 3] ^ gsState[12 + 3];
    gsState[0 + 3] ^= xtime(gsState[0 + 3] ^ gsState[4 + 3]) ^ gsTmp2;
    gsState[4 + 3] ^= xtime(gsState[4 + 3] ^ gsState[8 + 3]) ^ gsTmp2;
    gsState[8 + 3] ^= xtime(gsState[8 + 3] ^ gsState[12 + 3]) ^ gsTmp2;
    gsState[12 + 3] ^= xtime(gsState[12 + 3] ^ gsTmp) ^ gsTmp2;
}

/*****************************************************************************/
static void AES_InverseMixColumn(void)
{
    uint8_t gsTmp, gsTmp1, gsTmp2, gsTmp3;
    uint8_t idx;
    for (idx = 0; idx < 4u; idx++)
    {
        gsTmp = xtime14(gsState[0u + idx]) ^ xtime11(gsState[4u + idx]) ^ xtime13(gsState[8u + idx]) ^
                xtime9(gsState[12u + idx]);
        gsTmp1 = xtime14(gsState[4u + idx]) ^ xtime11(gsState[8u + idx]) ^ xtime13(gsState[12u + idx]) ^
                 xtime9(gsState[0u + idx]);
        gsTmp2 = xtime14(gsState[8u + idx]) ^ xtime11(gsState[12u + idx]) ^ xtime13(gsState[0u + idx]) ^
                 xtime9(gsState[4u + idx]);
        gsTmp3 = xtime14(gsState[12u + idx]) ^ xtime11(gsState[0u + idx]) ^ xtime13(gsState[4u + idx]) ^
                 xtime9(gsState[8u + idx]);
        gsState[0u + idx]  = gsTmp;
        gsState[4u + idx]  = gsTmp1;
        gsState[8u + idx]  = gsTmp2;
        gsState[12u + idx] = gsTmp3;
    }
}
static void AES_InverseShiftSubBytes(void)
{
    uint8_t gsTmp;
    gsState[0]  = gInvSbox_s[gsState[0]];
    gsState[1]  = gInvSbox_s[gsState[1]];
    gsState[2]  = gInvSbox_s[gsState[2]];
    gsState[3]  = gInvSbox_s[gsState[3]];
    gsTmp       = gsState[7];
    gsState[7]  = gInvSbox_s[gsState[6]];
    gsState[6]  = gInvSbox_s[gsState[5]];
    gsState[5]  = gInvSbox_s[gsState[4]];
    gsState[4]  = gInvSbox_s[gsTmp];
    gsTmp       = gsState[11];
    gsState[11] = gInvSbox_s[gsState[9]];
    gsState[9]  = gInvSbox_s[gsTmp];
    gsTmp       = gsState[10];
    gsState[10] = gInvSbox_s[gsState[8]];
    gsState[8]  = gInvSbox_s[gsTmp];
    gsTmp       = gsState[12];
    gsState[12] = gInvSbox_s[gsState[13]];
    gsState[13] = gInvSbox_s[gsState[14]];
    gsState[14] = gInvSbox_s[gsState[15]];
    gsState[15] = gInvSbox_s[gsTmp];
}

/*! *********************************************************************************
 * \brief  This function createa the A0 block used by the CCM.
 *
 * \param[out]  pData Pointer to a location where the A0 block will be stored
 *
 * \param[in]  nonce  Pointer to the Nonce
 *
 * \param[in]  size   The size of the nonce in bytes
 *
 ********************************************************************************** */
void CCM_CreateA0(uint8_t *pData, const uint8_t *nonce, uint8_t size)
{
    *pData++ = (15u - size) - 1u; /* L-1 */
    FLib_MemCpy(pData, (const void *)nonce, size);
    pData += size;
    FLib_MemSet(pData, 0, (uint32_t)gCcmBlockSize_c - (uint32_t)size - 1u);
}
/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
******************************************************************************/

/*! *********************************************************************************
 * \brief  This function performs SW AES-128 encryption on a 16-byte block.
 *
 * \param[in]  pInput Pointer to the location of the 16-byte plain text block.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
 *
 ********************************************************************************** */
void sw_Aes128(const uint8_t *pData, const uint8_t *pKey, uint8_t enc, uint8_t *pReturnData)
{
#if defined(TestTimingOnPins) && (TestTimingOnPins == 1) // Timing test...
    SEC_SETTIMINGPIN_2;                                  // For timing measurements. Default empty. Check target.h.
#endif

    gsState[0]  = pData[0];
    gsState[1]  = pData[4];
    gsState[2]  = pData[8];
    gsState[3]  = pData[12];
    gsState[4]  = pData[1];
    gsState[5]  = pData[5];
    gsState[6]  = pData[9];
    gsState[7]  = pData[13];
    gsState[8]  = pData[2];
    gsState[9]  = pData[6];
    gsState[10] = pData[10];
    gsState[11] = pData[14];
    gsState[12] = pData[3];
    gsState[13] = pData[7];
    gsState[14] = pData[11];
    gsState[15] = pData[15];
    gsKey[0]    = pKey[0];
    gsKey[1]    = pKey[4];
    gsKey[2]    = pKey[8];
    gsKey[3]    = pKey[12];
    gsKey[4]    = pKey[1];
    gsKey[5]    = pKey[5];
    gsKey[6]    = pKey[9];
    gsKey[7]    = pKey[13];
    gsKey[8]    = pKey[2];
    gsKey[9]    = pKey[6];
    gsKey[10]   = pKey[10];
    gsKey[11]   = pKey[14];
    gsKey[12]   = pKey[3];
    gsKey[13]   = pKey[7];
    gsKey[14]   = pKey[11];
    gsKey[15]   = pKey[15];
    if (enc != 0u)
    {
        AES_AddKeyByteSubShiftKeyExpand(1);
        AES_MixColumn();
        AES_AddKeyByteSubShiftKeyExpand(2);
        AES_MixColumn();
        AES_AddKeyByteSubShiftKeyExpand(4);
        AES_MixColumn();
        AES_AddKeyByteSubShiftKeyExpand(8);
        AES_MixColumn();
        AES_AddKeyByteSubShiftKeyExpand(16);
        AES_MixColumn();
        AES_AddKeyByteSubShiftKeyExpand(32);
        AES_MixColumn();
        AES_AddKeyByteSubShiftKeyExpand(64);
        AES_MixColumn();
        AES_AddKeyByteSubShiftKeyExpand(128);
        AES_MixColumn();
        AES_AddKeyByteSubShiftKeyExpand(27);
        AES_MixColumn();
        // Last
        AES_AddKeyByteSubShiftKeyExpand(54);
    }
    else
    {
        AES_ExpandForDecryption();
        uint8_t index = 0;
        for (index = 0; index < 16u; index++)
        {
            gsState[index] ^= gsExpandedKey[9][index];
        }
        AES_InverseShiftSubBytes();
        uint8_t index2 = 0;
        for (index = 9u; index >= 1u; index--)
        {
            for (index2 = 0; index2 < 16u; index2++)
            {
                gsState[index2] ^= gsExpandedKey[index - 1u][index2];
            }
            AES_InverseMixColumn();
            AES_InverseShiftSubBytes();
        }
    }
    // Returns data in 'byte-order', d0,d1,d2,...,d13,d14,d15
    pReturnData[0]  = gsState[0] ^ gsKey[0];
    pReturnData[4]  = gsState[1] ^ gsKey[1];
    pReturnData[8]  = gsState[2] ^ gsKey[2];
    pReturnData[12] = gsState[3] ^ gsKey[3];
    pReturnData[1]  = gsState[4] ^ gsKey[4];
    pReturnData[5]  = gsState[5] ^ gsKey[5];
    pReturnData[9]  = gsState[6] ^ gsKey[6];
    pReturnData[13] = gsState[7] ^ gsKey[7];
    pReturnData[2]  = gsState[8] ^ gsKey[8];
    pReturnData[6]  = gsState[9] ^ gsKey[9];
    pReturnData[10] = gsState[10] ^ gsKey[10];
    pReturnData[14] = gsState[11] ^ gsKey[11];
    pReturnData[3]  = gsState[12] ^ gsKey[12];
    pReturnData[7]  = gsState[13] ^ gsKey[13];
    pReturnData[11] = gsState[14] ^ gsKey[14];
    pReturnData[15] = gsState[15] ^ gsKey[15];

#if (defined(TestTimingOnPins) && (TestTimingOnPins == 1)) // Timing test...
    SEC_CLRTIMINGPIN_2;                                    // For timing measurements. Default empty. Check target.h.
#endif
}

/*! *********************************************************************************
* \brief  This function performs SW AES-128-CCM on a message block.
*
* \param[in]  pInput       Pointer to the location of the input message.
*
* \param[in]  inputLen     Length of the input message in bytes.

* \param[in]  pAuthData    Pointer to the aditional authentication data.
*
* \param[in]  authDataLen  Length of aditional authentication data.
*
* \param[in]  pNonce       Pointer to the Nonce.
*
* \param[in]  nonceSize    The size of the nonce (7-13).
*
* \param[in]  pKey         Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput     Pointer to the location to store the 16-byte authentication code.
*
* \param[out]  pCbcMac     Pointer to the location to store the authentication code.
*
* \param[out]  macSize     The size of the MAC.
*
* \param[out]  flags       Select encrypt/decript operations
*
********************************************************************************** */
uint8_t sw_AES128_CCM(const uint8_t *pInput,
                      uint16_t       inputLen,
                      const uint8_t *pAuthData,
                      uint16_t       authDataLen,
                      const uint8_t *pNonce,
                      uint8_t        nonceSize,
                      const uint8_t *pKey,
                      uint8_t *      pOutput,
                      uint8_t *      pCbcMac,
                      uint8_t        macSize,
                      uint32_t       flags)
{
    uint8_t  ccmTemp1[gCcmBlockSize_c];
    uint8_t  ccmTemp2[gCcmBlockSize_c];
    uint8_t *pIn;
    uint16_t length, i = 2u;
    uint8_t  status = 0u;

    while (i != 0u)
    {
        i--;

        /***** Auth transform header block *****/
        if (((i != 0u) && ((flags & 0x01u) == 0u)) || ((i == 0u) && ((flags & 0x01u) != 0u)))
        {
            if ((pCbcMac != (void *)0) && (macSize >= 4u))
            {
                bool_t  firstTime;
                uint8_t done;

                /* Create auth nonce - B(0). */
                CCM_CreateA0(ccmTemp1, pNonce, nonceSize);
                ccmTemp1[0] |= (uint8_t)(((authDataLen != 0u) ? 0x40u : 0x00u) | ((macSize - 2u) << 2));
                ccmTemp1[gCcmBlockSize_c - 1u] = (uint8_t)(inputLen & 0x00FFu);
                ccmTemp1[gCcmBlockSize_c - 2u] = (uint8_t)((inputLen & 0xFF00u) >> 8);

                if (authDataLen != 0u)
                {
                    /* Create L(a) | a | 0 */
                    length = authDataLen < (gCcmBlockSize_c - 2u) ? authDataLen : (gCcmBlockSize_c - 2u);
                    FLib_MemSet(ccmTemp2, 0, gCcmBlockSize_c);
                    FLib_MemCpy(ccmTemp2 + 2u, pAuthData, length);
                    ccmTemp2[1] = (uint8_t)(authDataLen & 0xFFu);
                    ccmTemp2[0] = (uint8_t)((authDataLen >> 8) & 0xFFu);
                    length      = authDataLen + 2u; /* Compensate for length of L(a) */
                    firstTime   = TRUE;
                    done        = 0u;
                    pIn         = ccmTemp2;
                }
                else
                {
                    firstTime = FALSE;
                    pIn       = (uint8_t *)pInput;
                    length    = inputLen;
                    /* Skip header auth, and, if required, message auth. */
                    done = (inputLen != 0u) ? 1u : 2u;
                }

                /* X(1) := E(key, B(0) ^ 0) */
                CCM_AesCipherInplace(pKey, ccmTemp1);

                /* Do both header auth, and message auth. */
                while (done < 2u)
                {
                    done++;
                    do
                    {
                        uint8_t len = length >= (uint16_t)gCcmBlockSize_c ? gCcmBlockSize_c : (uint8_t)(length & 0xFFu);
                        length -= len;

                        /* X(i+1) := E(key, B(i) ^ X(i)) for i=1,...,t */
                        SecLib_XorN(ccmTemp1, pIn, len);
                        CCM_AesCipherInplace(pKey, ccmTemp1);

                        if (firstTime)
                        {
                            firstTime = FALSE;
                            len -= 2u; /* Compensate for length of L(a) */
                            pIn = (uint8_t *)pAuthData;
                        }

                        pIn += len;
                    } while (length != 0u);

                    /* Authenticate message block - NOT always present */
                    if (inputLen != 0u)
                    {
                        length = inputLen;
                        pIn    = (uint8_t *)pInput;
                    }
                    else
                    {
                        break;
                    }
                }

                /* encrypt/decrypt MIC */
                /* Create A(0) */
                CCM_CreateA0(ccmTemp2, pNonce, nonceSize);
                /* Create S(0) := E(key, A(0)) */
                CCM_AesCipherInplace(pKey, ccmTemp2);
                /* U = S(0) ^ T */
                SecLib_XorN(ccmTemp2, ccmTemp1, macSize);

                if ((flags & 0x01u) == 0u)
                {
                    FLib_MemCpy(pCbcMac, ccmTemp2, macSize);
                }
                else
                {
                    if (FLib_MemCmp(pCbcMac, ccmTemp2, macSize) == FALSE)
                    {
                        status = 1u;
                        break;
                    }
                }
            }
        }

        /***** Crypt transform message block *****/
        if ((i != 0u) && (inputLen != 0u))
        {
            uint16_t tempLen  = inputLen;
            uint16_t countLen = 15u - (uint16_t)nonceSize;
            uint64_t count    = 0;

            /* Init crypt nonce - A(0). */
            CCM_CreateA0(ccmTemp1, pNonce, nonceSize);
            pIn = (uint8_t *)pInput;

            while (tempLen != 0u)
            {
                length = (tempLen >= gCcmBlockSize_c) ? gCcmBlockSize_c : tempLen;

                count++;
                FLib_MemCpyReverseOrder(&ccmTemp1[gCcmBlockSize_c - countLen], &count, countLen);
                tempLen -= length;

                /* C(i) := E(key, A(i) ^ M(i)) for i=1,...,t */
                AES_128_Encrypt(ccmTemp1, pKey, ccmTemp2);
                if (pInput != pOutput)
                {
                    SecLib_XorN(ccmTemp2, pIn, (uint8_t)(length & 0xFFu));
                    FLib_MemCpy(pOutput, ccmTemp2, length);
                }
                else
                {
                    SecLib_XorN(pOutput, ccmTemp2, (uint8_t)(length & 0xFFu));
                }

                pOutput += length;
                pIn += length;
            }
            /* Prepare input for authentication process */
            pInput = pOutput - inputLen;
        }
    }

    return status;
}

/*********************************** EOF *************************************/
