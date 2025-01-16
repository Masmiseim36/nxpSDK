/*--------------------------------------------------------------------------*/
/* Copyright 2022, 2024 NXP                                                 */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms.  If you do not agree to be bound by the applicable        */
/* license terms, then you may not retain, install, activate or otherwise   */
/* use the software.                                                        */
/*--------------------------------------------------------------------------*/

/** @file  mcuxClOsccaMacModes_Helper.c
 *  @brief internal API to implement the SM4 CMAC Key generation of mcuxClOsccaMacModes component */

#include <mcuxClToolchain.h>
#include <mcuxClMac.h>
#include <mcuxClKey.h>
#include <mcuxClMemory.h>
#include <internal/mcuxClKey_Types_Internal.h>
#include <internal/mcuxClKey_Functions_Internal.h>
#include <internal/mcuxClOsccaSm4_Internal.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClOscca_FunctionIdentifiers.h>
#include <internal/mcuxClOsccaMacModes_Internal_Functions.h>


#ifdef MCUXCL_FEATURE_MACMODES_CMAC_SM4
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClOsccaMacModes_SM4_Gen_K1K2)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClOsccaMacModes_SM4_Gen_K1K2(uint8_t *output, uint8_t * input)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClOsccaMacModes_SM4_Gen_K1K2);
    uint8_t mask;
    uint8_t i;

    for(i = 0U; i <= MCUXCLOSCCASM4_BLOCK_SIZE - 1U; i++)
    {
        output[i] = (uint8_t)((input[i] << 1) & 0xffU);
        uint8_t carryBit = input[i+1U] & 0x80U;
        if((i < MCUXCLOSCCASM4_BLOCK_SIZE - 1U) && (carryBit != 0U))
        {
            MCUX_CSSL_ANALYSIS_START_SUPPRESS_FALSE_POSITIVE_INTEGER_CONVERSION_MISINTERPRETS_DATA("A logical OR-operation with two values of the same width does not misinterpret or wrap data.")
            output[i] = output[i] | (uint8_t)0x01U;
            MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_FALSE_POSITIVE_INTEGER_CONVERSION_MISINTERPRETS_DATA()
        }
    }

    mask = ((input[0u] >> 7U) != 0U) ? 0xffU : 0x00U;
    output[ MCUXCLOSCCASM4_BLOCK_SIZE - 1U] ^= 0x87U & mask;

    MCUX_CSSL_FP_FUNCTION_EXIT_VOID(mcuxClOsccaMacModes_SM4_Gen_K1K2);
}
#endif /* MCUXCL_FEATURE_MACMODES_CMAC_SM4 */

