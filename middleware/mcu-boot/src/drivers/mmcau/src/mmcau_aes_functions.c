/********************************************************************************/
/********************************************************************************/
/*
 * Copyright (c) 2011 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FILE NAME      : mmcau_aes_functions.c
 * VERSION        : $Id: mmcau_aes_functions.c.rca 1.2 Sat Jan  8 01:21:45 2011 rzsx60 Experimental $
 * TYPE           : Source C library code
 * DEPARTMENT     : MSG R&D Core and Platforms
 * AUTHOR         : David Schimke
 * AUTHOR'S EMAIL : David.Schimke@freescale.com
 * -----------------------------------------------------------------------------
 * Release history
 * VERSION  Date       AUTHOR           DESCRIPTION
 *          08-2010    David Schimke    Initial Release
 *          01-2011    David Schimke    Add byte reverse to correct double word
 *                                      read of byte array inputs for little
 *                                      endian, header added, comment cleanup
 */
/********************************************************************************/
/********************************************************************************/

#include "aes_security.h"
#include "fsl_device_registers.h"

#define AES_128_NUMBER_ROUNDS 10

#define mmcau_1_cmd(c1) (0x80000000 + ((c1) << 22))
#define mmcau_2_cmds(c1, c2) (0x80100000 + ((c1) << 22) + ((c2) << 11))
#define mmcau_3_cmds(c1, c2, c3) (0x80100200 + ((c1) << 22) + ((c2) << 11) + c3)
#define byterev(d) (((d) >> 24) | (((d) >> 8) & 0xff00) | (((d) << 8) & 0xff0000) | ((d) << 24))

// Opcodes
#define TL 0
#define TS 0
#define CASR 0
#define CAA 1
#define CA0 2
#define CA1 3
#define CA2 4
#define CA3 5
#define CA4 6
#define CA5 7
#define CA6 8
#define CA7 9
#define CA8 10
#define CNOP 0x000
#define LDR 0x010
#define STR 0x020
#define ADR 0x030
#define RADR 0x040
#define ADRA 0x050
#define XOR 0x060
#define ROTL 0x070
#define MVRA 0x080
#define MVAR 0x090
#define AESS 0x0a0
#define AESIS 0x0b0
#define AESC 0x0c0
#define AESIC 0x0d0
#define AESR 0x0e0
#define AESIR 0x0f0
#define DESR 0x100
#define DESK 0x110
#define HASH 0x120
#define SHS 0x130
#define MDS 0x140
#define SHS2 0x150
#define ILL 0x1f0
#define IP 8
#define FP 4
#define DC 1
#define CP 2
#define KSL1 0
#define KSL2 1
#define KSR1 2
#define KSR2 3
#define HFF 0
#define HFG 1
#define HFH 2
#define HFI 3
#define HFP 2
#define HFC 4
#define HFM 5
#define HF2C 6
#define HF2M 7
#define HF2S 8
#define HF2T 9
#define HF2U 10
#define HF2V 11

#define MMCAU_INDIRECT_MASK 0x800

#define MMCAU_PPB_DIRECT ((volatile uint32_t *)CAU_BASE)
#define MMCAU_PPB_INDIRECT ((volatile uint32_t *)(CAU_BASE | MMCAU_INDIRECT_MASK))

/********************************************************************************/
/********************************************************************************/

/********************************************************************************/
/**/
/* AES: Performs an AES key expansion*/
/*   arguments*/
/*     *key         pointer to input key 128 bits in length */
/*     *keySchedule pointer to key schedule 128 bits in length */
/*     *rcon needs to be set to the following structure which must be located outside */
/*           outside of this function in order to make this code relocatable */
/*           unsigned int rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, */
/*                                    0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000}; */
/**/

void mmcau_aes_init(uint32_t *key, uint32_t *keySchedule, uint32_t *rcon)
{
    int32_t i, j;

    for (i = 0; i < 4; i++)
    {
        keySchedule[i] = byterev(key[i]);
    }

    *(MMCAU_PPB_INDIRECT + (LDR + CAA)) = keySchedule[i - 1]; /* CAA= key[3]*/

    for (j = 0; j < 10; j++)
    {
        *(MMCAU_PPB_INDIRECT + (ROTL + CAA)) = 8;      /* rotate 8*/
        *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(AESS + CAA); /* SubBytes*/
        *(MMCAU_PPB_INDIRECT + (XOR + CAA)) = rcon[j]; /* XOR rcon[j]*/

        /* 1st calculation for keySchedule[4+0,8+0,12+0,16+0,20+0,24+0,28+0,32+0,36+0,40+0]*/
        *(MMCAU_PPB_INDIRECT + (XOR + CAA)) = keySchedule[i - 4]; /*XOR key[i-4]*/
        keySchedule[i++] = *(MMCAU_PPB_INDIRECT + (STR + CAA));   /*store key[i]*/

        /* 2nd calculation for keySchedule[4+1,8+1,12+1,16+1,20+1,24+1,28+1,32+1,36+1,40+1]*/
        *(MMCAU_PPB_INDIRECT + (XOR + CAA)) = keySchedule[i - 4]; /*XOR key[i-4]*/
        keySchedule[i++] = *(MMCAU_PPB_INDIRECT + (STR + CAA));   /*store key[i]*/

        /* 3rd calculation for keySchedule[4+2,8+2,12+2,16+2,20+2,24+2,28+2,32+2,36+2,40+2]*/
        *(MMCAU_PPB_INDIRECT + (XOR + CAA)) = keySchedule[i - 4]; /*XOR key[i-4]*/
        keySchedule[i++] = *(MMCAU_PPB_INDIRECT + (STR + CAA));   /*store key[i]*/

        /* 4th calculation for keySchedule[4+3,8+3,12+3,16+3,20+3,24+3,28+3,32+3,36+3,40+3]*/
        *(MMCAU_PPB_INDIRECT + (XOR + CAA)) = keySchedule[i - 4]; /*XOR key[i-4]*/
        keySchedule[i++] = *(MMCAU_PPB_INDIRECT + (STR + CAA));   /*store key[i]*/
    }
}

/********************************************************************************/
/********************************************************************************/
/**/
/* AES: Encrypts a single 16-byte block*/
/*   arguments*/
/*           *in          pointer to 16-byte block of input plaintext*/
/*           *key         pointer to input key 128 bits in length*/
/*           *keySchedule pointer to key schedule 128 bits in length*/
/*           *out         pointer to 16-byte block of output ciphertext*/
/**/
/*   NOTE    Input and output blocks may overlap*/
/**/

void mmcau_aes_encrypt(uint32_t *in, uint32_t *key, uint32_t *keySchedule, uint32_t *out)
{
    int32_t i, j;

    /* load the 4 plain test bytes into the CAU's CA0 - CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR + CA0)) = byterev(in[0]); /* load in[0]-> CA0*/
    *(MMCAU_PPB_INDIRECT + (LDR + CA1)) = byterev(in[1]); /* load in[1]-> CA1*/
    *(MMCAU_PPB_INDIRECT + (LDR + CA2)) = byterev(in[2]); /* load in[2]-> CA2*/
    *(MMCAU_PPB_INDIRECT + (LDR + CA3)) = byterev(in[3]); /* load in[3]-> CA3*/

    /* XOR the first 4 keys into the CAU's CA0 - CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (XOR + CA0)) = keySchedule[0]; /* XOR keys*/
    *(MMCAU_PPB_INDIRECT + (XOR + CA1)) = keySchedule[1];
    *(MMCAU_PPB_INDIRECT + (XOR + CA2)) = keySchedule[2];
    *(MMCAU_PPB_INDIRECT + (XOR + CA3)) = keySchedule[3];

    /* send a series of cau commands to perform the encryption*/
    for (i = 0, j = 4; i < AES_128_NUMBER_ROUNDS - 1; i++, j += 4)
    {
        *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(AESS + CA0, AESS + CA1, AESS + CA2); /*Sbytes*/
        *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(AESS + CA3, AESR);                   /*Sbyte,Shft*/
        *(MMCAU_PPB_INDIRECT + (AESC + CA0)) = keySchedule[j];                  /* MixCols*/
        *(MMCAU_PPB_INDIRECT + (AESC + CA1)) = keySchedule[j + 1];
        *(MMCAU_PPB_INDIRECT + (AESC + CA2)) = keySchedule[j + 2];
        *(MMCAU_PPB_INDIRECT + (AESC + CA3)) = keySchedule[j + 3];
    }

    *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(AESS + CA0, AESS + CA1, AESS + CA2); /* SubBytes*/
    *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(AESS + CA3, AESR);                   /*SByte,Shft*/
    *(MMCAU_PPB_INDIRECT + (XOR + CA0)) = keySchedule[j];                   /* XOR keys*/
    *(MMCAU_PPB_INDIRECT + (XOR + CA1)) = keySchedule[j + 1];
    *(MMCAU_PPB_INDIRECT + (XOR + CA2)) = keySchedule[j + 2];
    *(MMCAU_PPB_INDIRECT + (XOR + CA3)) = keySchedule[j + 3];

    /* store the 16-byte ciphertext output block into memory*/
    out[0] = *(MMCAU_PPB_INDIRECT + (STR + CA0)); /* store 1st 4 bytes*/
    out[1] = *(MMCAU_PPB_INDIRECT + (STR + CA1)); /* store 2nd 4 bytes*/
    out[2] = *(MMCAU_PPB_INDIRECT + (STR + CA2)); /* store 3rd 4 bytes*/
    out[3] = *(MMCAU_PPB_INDIRECT + (STR + CA3)); /* store 4th 4 bytes*/

    out[0] = byterev(out[0]); /* Reverse 1st 4 bytes*/
    out[1] = byterev(out[1]); /* Reverse 2nd 4 bytes*/
    out[2] = byterev(out[2]); /* Reverse 3rd 4 bytes*/
    out[3] = byterev(out[3]); /* Reverse 4th 4 bytes*/
}

/********************************************************************************/
/********************************************************************************/
/**/
/* AES: Decrypts a single 16-byte block*/
/*   arguments*/
/*           *in          pointer to 16-byte block of input chiphertext*/
/*           *key         pointer to input key 128 bits in length*/
/*           *keySchedule pointer to key schedule 128 bits in length*/
/*           *out         pointer to 16-byte block of output plaintext*/
/**/
/*   NOTE    Input and output blocks may overlap*/
/**/

void mmcau_aes_decrypt(uint32_t *in, uint32_t *key, uint32_t *keySchedule, uint32_t *out)
{
    int32_t i;

    /* load the cipher bytes into the CAU's CA0 - CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR + CA0)) = byterev(in[0]); /* load in[0] -> CA0*/
    *(MMCAU_PPB_INDIRECT + (LDR + CA1)) = byterev(in[1]); /* load in[1] -> CA1*/
    *(MMCAU_PPB_INDIRECT + (LDR + CA2)) = byterev(in[2]); /* load in[2] -> CA2*/
    *(MMCAU_PPB_INDIRECT + (LDR + CA3)) = byterev(in[3]); /* load in[3] -> CA3*/

    /* the keySchedule index (i) is adjusted to define the end of the elements*/
    /* the adjustment factor = f(nr) is defined by the expression:*/
    /*   end of keySchedule = 4 x (nr + 1) for nr = {10, 12, 14}*/
    i = 4 * (AES_128_NUMBER_ROUNDS + 1);

    /* XOR the keys into the CAU's CA0 - CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (XOR + CA3)) = keySchedule[--i]; /* XOR keys*/
    *(MMCAU_PPB_INDIRECT + (XOR + CA2)) = keySchedule[--i];
    *(MMCAU_PPB_INDIRECT + (XOR + CA1)) = keySchedule[--i];
    *(MMCAU_PPB_INDIRECT + (XOR + CA0)) = keySchedule[--i];

    /* send a series of cau commands to perform the decryption*/
    while (i > 4)
    {
        *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(AESIR, AESIS + CA3, AESIS + CA2);
        /* InvShiftRows,InvSubBytes*/
        *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(AESIS + CA1, AESIS + CA0); /* InvSubByts*/
        *(MMCAU_PPB_INDIRECT + (AESIC + CA3)) = keySchedule[--i];     /* InvMxCols*/
        *(MMCAU_PPB_INDIRECT + (AESIC + CA2)) = keySchedule[--i];
        *(MMCAU_PPB_INDIRECT + (AESIC + CA1)) = keySchedule[--i];
        *(MMCAU_PPB_INDIRECT + (AESIC + CA0)) = keySchedule[--i];
    }

    *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(AESIR, AESIS + CA3, AESIS + CA2);
    /* InvShiftRows,InvSubBytes*/
    *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(AESIS + CA1, AESIS + CA0); /* InvSBytes*/
    *(MMCAU_PPB_INDIRECT + (XOR + CA3)) = keySchedule[--i];       /* XOR keys*/
    *(MMCAU_PPB_INDIRECT + (XOR + CA2)) = keySchedule[--i];
    *(MMCAU_PPB_INDIRECT + (XOR + CA1)) = keySchedule[--i];
    *(MMCAU_PPB_INDIRECT + (XOR + CA0)) = keySchedule[--i];

    /* store the 16-byte ciphertext output block into memory*/
    out[0] = *(MMCAU_PPB_INDIRECT + (STR + CA0)); /* store 1st 4 bytes*/
    out[1] = *(MMCAU_PPB_INDIRECT + (STR + CA1)); /* store 2nd 4 bytes*/
    out[2] = *(MMCAU_PPB_INDIRECT + (STR + CA2)); /* store 3rd 4 bytes*/
    out[3] = *(MMCAU_PPB_INDIRECT + (STR + CA3)); /* store 4th 4 bytes*/

    out[0] = byterev(out[0]); /* Reverse 1st 4 bytes*/
    out[1] = byterev(out[1]); /* Reverse 2nd 4 bytes*/
    out[2] = byterev(out[2]); /* Reverse 3rd 4 bytes*/
    out[3] = byterev(out[3]); /* Reverse 4th 4 bytes*/
}
