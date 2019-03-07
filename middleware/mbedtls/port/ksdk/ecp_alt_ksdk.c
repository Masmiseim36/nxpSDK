/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/threading.h"
#include "mbedtls/platform_util.h"

#include <string.h>

#if defined(MBEDTLS_ECP_ALT)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

 /*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Codes
 ******************************************************************************/
#if defined(MBEDTLS_MCUX_CASPER_ECC)

#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED) || \
    defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED) || \
    defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED) || \
    defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
#error "CASPER hw acceleration currently supported only for SECP256R1."
#endif

#if defined(MBEDTLS_ECP_MUL_COMB_ALT)
//#include "fsl_casper.h"
static void reverse_array(uint8_t *src, size_t src_len)
{
    int i;

    for (i = 0; i < src_len / 2; i++)
    {
        uint8_t tmp;

        tmp = src[i];
        src[i] = src[src_len - 1 - i];
        src[src_len - 1 - i] = tmp;
    }
}
#if 0
static void reverse_array_masked(uint8_t *src, size_t src_len, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    int i;
    uint8_t tempMask[4] = {0};
    
    if (f_rng != NULL)
    {
        f_rng(p_rng, tempMask, sizeof(tempMask));
    }
    
    size_t szLeft = src_len - (src_len & 0xfffffffc);
    for (int ii = 0; ii < (src_len & 0xfffffffc); ii += 4u)
    {
        ((uint32_t*)(src+ii))[0] ^= *(uint32_t*)tempMask;
    }
    if (szLeft)
    {
        for (int ii = 0; ii < szLeft; ii++)
        {
            *(src + src_len - szLeft + ii) ^= tempMask[ii];
        }
    }

    for (i = 0; i < src_len / 2; i++)
    {
        uint8_t tmp;

        tmp = src[i];
        src[i] = src[src_len - 1 - i];
        src[src_len - 1 - i] = tmp;
    }
    
    for (int ii = 0; ii < (src_len & 0xfffffffc); ii += 4u)
    {
        ((uint32_t*)(src+ii))[0] ^= *(uint32_t*)tempMask;
    }
    if (szLeft)
    {
        for (int ii = 0; ii < szLeft; ii++)
        {
            *(src + src_len - szLeft + ii) ^= tempMask[ii];
        }
    }
}
#endif

//int mbedtls_ecp_mul( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
//             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
//             int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
int ecp_mul_comb( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                         const mbedtls_mpi *m, const mbedtls_ecp_point *P,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng )
{
  struct _ecp
  {    
    uint8_t arr[68];
  } p;
  
  //uint32_t X1[8]; uint32_t Y1[8]; 
  uint32_t M[8] = {0};
  
    size_t olen = sizeof(p);
  
  mbedtls_ecp_point_write_binary(grp, P, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, &p.arr[3], sizeof(p.arr));
  reverse_array(&p.arr[4], 32);
  reverse_array(&p.arr[4+32], 32);
  CASPER_ecc_init ();
  //toMontgomery(X1, (void*)&p.arr[4]);
  //toMontgomery(Y1, (void*)&p.arr[4+32]);
  
  //CASPER_MEMCPY(&CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS], X1, NUM_LIMBS * sizeof(uint32_t));
  //CASPER_MEMCPY(&CASPER_MEM[INOUT_SCRATCH_START + 1 * CASPER_NUM_LIMBS], Y1, NUM_LIMBS * sizeof(uint32_t));
  if (mbedtls_mpi_size(m) > sizeof(M))
  {
   __BKPT(0);
  }
  mbedtls_mpi_write_binary(m, (void*)M, mbedtls_mpi_size(m));
  reverse_array((void*)M, 32);
  //reverse_array_masked(M, 32, f_rng, p_rng);
      
  //Jac_scalar_multiplication(&CASPER_MEM[INOUT_SCRATCH_START + 6 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 7 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 8 * CASPER_NUM_LIMBS],
  //                          &CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 1 * CASPER_NUM_LIMBS], 
   //                         M, NISTp256, NISTp256_q);

  //Jac_toAffine(&CASPER_MEM[INOUT_SCRATCH_START + 3 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 4 * CASPER_NUM_LIMBS], 
  //             &CASPER_MEM[INOUT_SCRATCH_START + 6 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 7 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 8 * CASPER_NUM_LIMBS]);
  //
  //uint32_t one[8] = { 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  //CASPER_MEMCPY(&CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS], one, NUM_LIMBS * sizeof(uint32_t));
  //multiply_casper(&CASPER_MEM[INOUT_SCRATCH_START + 5 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 3 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS]);
  //multiply_casper(&CASPER_MEM[INOUT_SCRATCH_START + 6 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 4 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS]);
  
  //CASPER_MEMCPY(&p.arr[4], &CASPER_MEM[INOUT_SCRATCH_START + 5 * CASPER_NUM_LIMBS], 32);
  //CASPER_MEMCPY(&p.arr[4+32], &CASPER_MEM[INOUT_SCRATCH_START + 6 * CASPER_NUM_LIMBS], 32);
  CASPER_ECC_SECP256R1_Mul(CASPER, (void*)&p.arr[4], (void*)&p.arr[4+32], (void*)&p.arr[4], (void*)&p.arr[4+32], (void*)M);
  reverse_array(&p.arr[4], 32);
  reverse_array(&p.arr[4+32], 32);
  mbedtls_ecp_point_read_binary(grp, R, &p.arr[3], 65);
  return 0;
}
#endif /* MBEDTLS_ECP_MUL_COMB_ALT */

#if defined(MBEDTLS_ECP_MULADD_ALT)
int mbedtls_ecp_muladd( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
             const mbedtls_mpi *n, const mbedtls_ecp_point *Q )
{
  struct _ecp
  {    
    uint8_t arr[68];
  } p1, p2;
  
  //uint32_t X1[8]; uint32_t Y1[8]; 
  uint32_t M[8] = {0};
  //uint32_t X2[8]; uint32_t Y2[8]; 
  uint32_t N[8] = {0};
  
  size_t olen1= sizeof(p1);
  
  mbedtls_ecp_point_write_binary(grp, P, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen1, &p1.arr[3], sizeof(p1.arr));
  reverse_array(&p1.arr[4], 32);
  reverse_array(&p1.arr[4+32], 32);
  CASPER_ecc_init ();
  //toMontgomery(X1, (void*)&p1.arr[4]);
  //toMontgomery(Y1, (void*)&p1.arr[4+32]);
  
  //CASPER_MEMCPY(&CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS], X1, NUM_LIMBS * sizeof(uint32_t));
  //CASPER_MEMCPY(&CASPER_MEM[INOUT_SCRATCH_START + 1 * CASPER_NUM_LIMBS], Y1, NUM_LIMBS * sizeof(uint32_t));
  if (mbedtls_mpi_size(m) > sizeof(M))
  {
    __BKPT(0);
  }
  mbedtls_mpi_write_binary(m, (void*)M, mbedtls_mpi_size(m));
  reverse_array((void*)M, 32);
  /* */
  size_t olen2= sizeof(p2);
  
  mbedtls_ecp_point_write_binary(grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen2, &p2.arr[3], sizeof(p2.arr));
  reverse_array(&p2.arr[4], 32);
  reverse_array(&p2.arr[4+32], 32);
  //toMontgomery(X2, (void*)&p2.arr[4]);
  //toMontgomery(Y2, (void*)&p2.arr[4+32]);
  
  //CASPER_MEMCPY(&CASPER_MEM[INOUT_SCRATCH_START + 2 * CASPER_NUM_LIMBS], X2, NUM_LIMBS * sizeof(uint32_t));
  //CASPER_MEMCPY(&CASPER_MEM[INOUT_SCRATCH_START + 3 * CASPER_NUM_LIMBS], Y2, NUM_LIMBS * sizeof(uint32_t));
  if (mbedtls_mpi_size(n) > sizeof(N))
  {
    __BKPT(0);
  }
  mbedtls_mpi_write_binary(n, (void*)N, mbedtls_mpi_size(n));
  reverse_array((void*)N, 32);
      
  //double_scalar_multiplication(&CASPER_MEM[INOUT_SCRATCH_START + 4 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 5 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 6 * CASPER_NUM_LIMBS],
  //                             &CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 1 * CASPER_NUM_LIMBS], M,
  //                             &CASPER_MEM[INOUT_SCRATCH_START + 2 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 3 * CASPER_NUM_LIMBS], N);

  //Jac_toAffine(&CASPER_MEM[LUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS], &CASPER_MEM[LUT_SCRATCH_START + 1 * CASPER_NUM_LIMBS],
  //                 &CASPER_MEM[INOUT_SCRATCH_START + 4 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 5 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 6 * CASPER_NUM_LIMBS]);
  
  //uint32_t one[8] = { 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  //CASPER_MEMCPY(&CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS], one, NUM_LIMBS * sizeof(uint32_t));
  //multiply_casper(&CASPER_MEM[INOUT_SCRATCH_START + 1 * CASPER_NUM_LIMBS], &CASPER_MEM[LUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS]);
  //multiply_casper(&CASPER_MEM[INOUT_SCRATCH_START + 2 * CASPER_NUM_LIMBS], &CASPER_MEM[LUT_SCRATCH_START + 1 * CASPER_NUM_LIMBS], &CASPER_MEM[INOUT_SCRATCH_START + 0 * CASPER_NUM_LIMBS]);
  
  //CASPER_MEMCPY(&p1.arr[4], &CASPER_MEM[INOUT_SCRATCH_START + 1 * CASPER_NUM_LIMBS], 32);
  //CASPER_MEMCPY(&p1.arr[4+32], &CASPER_MEM[INOUT_SCRATCH_START + 2 * CASPER_NUM_LIMBS], 32);
  CASPER_ECC_SECP256R1_MulAdd(CASPER,
                              (void*)&p1.arr[4], (void*)&p1.arr[4+32],
                              (void*)&p1.arr[4], (void*)&p1.arr[4+32],(void*)M,
                              (void*)&p2.arr[4], (void*)&p2.arr[4+32],(void*)N);
  reverse_array(&p1.arr[4], 32);
  reverse_array(&p1.arr[4+32], 32);
  mbedtls_ecp_point_read_binary(grp, R, &p1.arr[3], 65);
  return 0;
}
#endif /* MBEDTLS_ECP_MULADD_ALT */

#endif /* MBEDTLS_MCUX_CASPER_ECC */
 
#endif /* MBEDTLS_ECP_ALT */
#endif /* MBEDTLS_ECP_C */
