/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nand_ecc.h"

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

typedef struct ecc_code_byte
{
    union
    {
        uint8_t rpeven;
        struct
        {
          uint8_t rp0:1;
          uint8_t rp1:1;
          uint8_t rp2:1;
          uint8_t rp3:1;
          uint8_t rp4:1;
          uint8_t rp5:1;
          uint8_t rp6:1;
          uint8_t rp7:1;
        } byte0_rpl;
        struct
        {
          uint8_t rp0:1;
          uint8_t rp2:1;
          uint8_t rp4:1;
          uint8_t rp6:1;
          uint8_t rp8:1;
          uint8_t rp10:1;
          uint8_t rp12:1;
          uint8_t rp14:1;
        } byte0_rpeven;
    };
    union
    {
        uint8_t rpodd;
        struct
        {
          uint8_t rp8:1;
          uint8_t rp9:1;
          uint8_t rp10:1;
          uint8_t rp11:1;
          uint8_t rp12:1;
          uint8_t rp13:1;
          uint8_t rp14:1;
          uint8_t rp15:1;
        } byte1_rpm;
        struct
        {
          uint8_t rp1:1;
          uint8_t rp3:1;
          uint8_t rp5:1;
          uint8_t rp7:1;
          uint8_t rp9:1;
          uint8_t rp11:1;
          uint8_t rp13:1;
          uint8_t rp15:1;
        } byte1_rpodd;
    };
    union
    {
        uint8_t cp;
        struct
        {
          uint8_t rp16:1;
          uint8_t rp17:1;
          uint8_t cp0:1;
          uint8_t cp1:1;
          uint8_t cp2:1;
          uint8_t cp3:1;
          uint8_t cp4:1;
          uint8_t cp5:1;
        } byte2_cp;
    };
} ecc_code_byte_t;

////////////////////////////////////////////////////////////////////////////////
// Local prototypes
////////////////////////////////////////////////////////////////////////////////

uint32_t nand_ecc_count_bit1(uint8_t *byteData, uint32_t byteSize);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
/*
 * Pre-calculated byte column parity
 *    |-----------------------------------------------------------------------|
 *    |  bit7  |  bit6  |  bit5  |  bit4  |  bit3  |  bit2  |  bit1  |  bit0  |
 *    |   0    |   RP   |  CP5   |  CP4   |  CP3   |  CP2   |  CP1   |  CP0   |
 *    |-----------------------------------------------------------------------|
 */

#define ECC_BYTE_PARITY_TABLE_CP_BIT_SHIFT (0u)
#define ECC_BYTE_PARITY_TABLE_CP_BIT_MASK  (0x3Fu)

#define ECC_BYTE_PARITY_TABLE_RP_BIT_SHIFT (6u)
#define ECC_BYTE_PARITY_TABLE_RP_BIT_MASK  (0x40u)

static const uint8_t s_eccByteParityTable[] = {
   0x00, 0x55, 0x56, 0x03, 0x59, 0x0c, 0x0f, 0x5a, 0x5a, 0x0f, 0x0c, 0x59, 0x03, 0x56, 0x55, 0x00,
   0x65, 0x30, 0x33, 0x66, 0x3c, 0x69, 0x6a, 0x3f, 0x3f, 0x6a, 0x69, 0x3c, 0x66, 0x33, 0x30, 0x65,
   0x66, 0x33, 0x30, 0x65, 0x3f, 0x6a, 0x69, 0x3c, 0x3c, 0x69, 0x6a, 0x3f, 0x65, 0x30, 0x33, 0x66,
   0x03, 0x56, 0x55, 0x00, 0x5a, 0x0f, 0x0c, 0x59, 0x59, 0x0c, 0x0f, 0x5a, 0x00, 0x55, 0x56, 0x03,
   0x69, 0x3c, 0x3f, 0x6a, 0x30, 0x65, 0x66, 0x33, 0x33, 0x66, 0x65, 0x30, 0x6a, 0x3f, 0x3c, 0x69,
   0x0c, 0x59, 0x5a, 0x0f, 0x55, 0x00, 0x03, 0x56, 0x56, 0x03, 0x00, 0x55, 0x0f, 0x5a, 0x59, 0x0c,
   0x0f, 0x5a, 0x59, 0x0c, 0x56, 0x03, 0x00, 0x55, 0x55, 0x00, 0x03, 0x56, 0x0c, 0x59, 0x5a, 0x0f,
   0x6a, 0x3f, 0x3c, 0x69, 0x33, 0x66, 0x65, 0x30, 0x30, 0x65, 0x66, 0x33, 0x69, 0x3c, 0x3f, 0x6a,
   0x6a, 0x3f, 0x3c, 0x69, 0x33, 0x66, 0x65, 0x30, 0x30, 0x65, 0x66, 0x33, 0x69, 0x3c, 0x3f, 0x6a,
   0x0f, 0x5a, 0x59, 0x0c, 0x56, 0x03, 0x00, 0x55, 0x55, 0x00, 0x03, 0x56, 0x0c, 0x59, 0x5a, 0x0f,
   0x0c, 0x59, 0x5a, 0x0f, 0x55, 0x00, 0x03, 0x56, 0x56, 0x03, 0x00, 0x55, 0x0f, 0x5a, 0x59, 0x0c,
   0x69, 0x3c, 0x3f, 0x6a, 0x30, 0x65, 0x66, 0x33, 0x33, 0x66, 0x65, 0x30, 0x6a, 0x3f, 0x3c, 0x69,
   0x03, 0x56, 0x55, 0x00, 0x5a, 0x0f, 0x0c, 0x59, 0x59, 0x0c, 0x0f, 0x5a, 0x00, 0x55, 0x56, 0x03,
   0x66, 0x33, 0x30, 0x65, 0x3f, 0x6a, 0x69, 0x3c, 0x3c, 0x69, 0x6a, 0x3f, 0x65, 0x30, 0x33, 0x66,
   0x65, 0x30, 0x33, 0x66, 0x3c, 0x69, 0x6a, 0x3f, 0x3f, 0x6a, 0x69, 0x3c, 0x66, 0x33, 0x30, 0x65,
   0x00, 0x55, 0x56, 0x03, 0x59, 0x0c, 0x0f, 0x5a, 0x5a, 0x0f, 0x0c, 0x59, 0x03, 0x56, 0x55, 0x00
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
/*
 * Calculate 3-byte ECC for 256-byte (22bit)/512-byte(24bit) block
 *    |------------------------------------------------------------------------------------|
 *    | ECC result |  bit7  |  bit6  |  bit5  |  bit4  |  bit3  |  bit2  |  bit1  |  bit0  |
 *    |   byte0    |  RP7   |  RP6   |  RP5   |  RP4   |  RP3   |  RP2   |  RP1   |  RP0   |
 *    |   byte1    |  RP15  |  RP14  |  RP13  |  RP12  |  RP11  |  RP10  |  RP9   |  RP8   |
 *    |   byte2    |  CP5   |  CP4   |  CP3   |  CP2   |  CP1   |  CP0   |  RP17  |  RP16  |
 *    |------------------------------------------------------------------------------------|
 */

#define ECC_CODE_PARITY_BYTE_CP_BIT_SHIFT  (2u)
#define ECC_CODE_PARITY_BYTE_CP_BIT_MASK   (0xFCu)

void nand_ecc_sec_calculate_encode(nand_ecc_block_size_t blockSize, uint8_t *nandData, uint8_t *calculatedEccCode)
{
    uint32_t rpIndex;
    uint8_t cpByteParity = 0;
    ecc_code_byte_t eccCodeByte;
    ecc_code_byte_t *calculatedEccCodeByte = (ecc_code_byte_t *)calculatedEccCode;

    // Cleanup Ecc result byte
    memset((uint8_t *)&eccCodeByte, 0x0u, sizeof(ecc_code_byte_t));

    for (rpIndex = 0; rpIndex < blockSize; rpIndex++)
    {
        // Got ecc parity vaule for current byte
        uint8_t tableIndex = *nandData++;
        uint8_t eccByteParity = s_eccByteParityTable[tableIndex];

        // Calculate CPx
        // Note: we just directly do XOR operation for all payload bits
        cpByteParity ^= (eccByteParity & ECC_BYTE_PARITY_TABLE_CP_BIT_MASK);

        // Calculate RPx
        // Note: if the number of 1'b1 in byte/data is Odd, then the parity is 1'b1,
        //    so we just need to count the number of 1'b1 for all payload bits
        if (eccByteParity & ECC_BYTE_PARITY_TABLE_RP_BIT_MASK)
        {
            eccCodeByte.rpodd ^= (uint8_t)rpIndex;
            eccCodeByte.rpeven ^= ~(uint8_t)rpIndex;
            if (blockSize == kNandEccBlockSize_512B)
            {
                eccCodeByte.byte2_cp.rp17 ^= (uint8_t)(rpIndex >> 8);
                eccCodeByte.byte2_cp.rp16 ^= ~(uint8_t)(rpIndex >> 8);
            }
        }
    }

    // Calculate final ECC code (CP)
    if (blockSize == kNandEccBlockSize_256B)
    {
        eccCodeByte.cp |= ~ECC_CODE_PARITY_BYTE_CP_BIT_MASK;
    }
    cpByteParity = ((cpByteParity & ECC_BYTE_PARITY_TABLE_CP_BIT_MASK) >> ECC_BYTE_PARITY_TABLE_CP_BIT_SHIFT) << ECC_CODE_PARITY_BYTE_CP_BIT_SHIFT;
    eccCodeByte.cp |= cpByteParity;

    // Set ECC result
    calculatedEccCodeByte->byte0_rpl.rp0 = eccCodeByte.byte0_rpeven.rp0;
    calculatedEccCodeByte->byte0_rpl.rp1 = eccCodeByte.byte1_rpodd.rp1;
    calculatedEccCodeByte->byte0_rpl.rp2 = eccCodeByte.byte0_rpeven.rp2;
    calculatedEccCodeByte->byte0_rpl.rp3 = eccCodeByte.byte1_rpodd.rp3;
    calculatedEccCodeByte->byte0_rpl.rp4 = eccCodeByte.byte0_rpeven.rp4;
    calculatedEccCodeByte->byte0_rpl.rp5 = eccCodeByte.byte1_rpodd.rp5;
    calculatedEccCodeByte->byte0_rpl.rp6 = eccCodeByte.byte0_rpeven.rp6;
    calculatedEccCodeByte->byte0_rpl.rp7 = eccCodeByte.byte1_rpodd.rp7;
    calculatedEccCodeByte->byte1_rpm.rp8 = eccCodeByte.byte0_rpeven.rp8;
    calculatedEccCodeByte->byte1_rpm.rp9 = eccCodeByte.byte1_rpodd.rp9;
    calculatedEccCodeByte->byte1_rpm.rp10 = eccCodeByte.byte0_rpeven.rp10;
    calculatedEccCodeByte->byte1_rpm.rp11 = eccCodeByte.byte1_rpodd.rp11;
    calculatedEccCodeByte->byte1_rpm.rp12 = eccCodeByte.byte0_rpeven.rp12;
    calculatedEccCodeByte->byte1_rpm.rp13 = eccCodeByte.byte1_rpodd.rp13;
    calculatedEccCodeByte->byte1_rpm.rp14 = eccCodeByte.byte0_rpeven.rp14;
    calculatedEccCodeByte->byte1_rpm.rp15 = eccCodeByte.byte1_rpodd.rp15;
    calculatedEccCodeByte->cp = eccCodeByte.cp;
}

uint32_t nand_ecc_count_bit1(uint8_t *byteData, uint32_t byteSize)
{
    uint32_t bits = 0;
    while(byteSize--)
    {
        uint8_t data = *byteData++;
        for (; data > 0; data >>= 1u)
        {
            bits += data & 0x1u;
        }
    }

    return bits;
}

/*
 *Detect and correct a 1 bit error for 256/512 byte block
 */
status_t nand_ecc_sec_verify_correct(nand_ecc_block_size_t blockSize, uint8_t *nandData, uint8_t *readoutEccCode)
{
    uint8_t calculatedEccCode[3] = {0, 0 ,0};
    uint8_t eccXorResults[3];
    uint32_t eccXorBit1Count, singleErrorBit1Count;

    // Calculate ECC of nand data
    nand_ecc_sec_calculate_encode(blockSize, nandData, calculatedEccCode);

    // XOR readout ECC with calculated ECC
    for (uint8_t i = 0; i < sizeof(eccXorResults); i++)
    {
        eccXorResults[i] = calculatedEccCode[i] ^ readoutEccCode[i];
    }

    // Calculate the count of bit1 in ECC XOR result
    eccXorBit1Count = nand_ecc_count_bit1(eccXorResults, sizeof(eccXorResults));
    if (blockSize == kNandEccBlockSize_256B)
    {
        singleErrorBit1Count = kNandEccSecResultBit1Count_SingleError256B;
    }
    else if (blockSize == kNandEccBlockSize_512B)
    {
        singleErrorBit1Count = kNandEccSecResultBit1Count_SingleError512B;
    }

    // Check ECC result:
    // No error: The results of XOR are all 0.
    if (eccXorBit1Count == kNandEccSecResultBit1Count_NoError)
    {
        return kStatus_NandECC_NoBitErrorDetected;
    }
    // Correctable error(1-bit): The results of XOR contain n bits at 1.
    //  For 256-byte inputs, the generated ECC has 11 bits at 1.
    //  For 512-byte inputs, the generated ECC has 12 bits at 1.
    else if (eccXorBit1Count == singleErrorBit1Count)
    {
        uint32_t dataIndex, bitIndex;
        // Find bit address of error byte
        bitIndex = ((eccXorResults[2] >> 3) & 0x01) |
                   ((eccXorResults[2] >> 4) & 0x02) |
                   ((eccXorResults[2] >> 5) & 0x04);
        // Find byte address of error byte
        dataIndex = ((eccXorResults[0] >> 1) & 0x01) |
                    ((eccXorResults[0] >> 2) & 0x02) |
                    ((eccXorResults[0] >> 3) & 0x04) |
                    ((eccXorResults[0] >> 4) & 0x08) |
                    ((eccXorResults[1] << 3) & 0x10) |
                    ((eccXorResults[1] << 2) & 0x20) |
                    ((eccXorResults[1] << 1) & 0x40) |
                    ((eccXorResults[1] << 0) & 0x80);
        if (blockSize == kNandEccBlockSize_512B)
        {
            dataIndex |= ((uint16_t)eccXorResults[2] << 7) & 0x100;
        }
        // Correct discovered error bit
        nandData[dataIndex] ^= (1 << bitIndex);

        return kStatus_NandECC_SingleBitErrorCorrected;
    }
    // ECC error: The result of XOR has only 1 bit at 1. This means that the error is in the ECC area.
    else if (eccXorBit1Count == kNandEccSecResultBit1Count_OobError)
    {
        return kStatus_NandECC_EccDataErrorInOOB;
    }
    // Non-correctable error: The result of XOR provides all other results.
    else
    {
        return kStatus_NandECC_NoLessThanDoubleBitError;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
