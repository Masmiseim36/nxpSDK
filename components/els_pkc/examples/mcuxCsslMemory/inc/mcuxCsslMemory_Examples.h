/*--------------------------------------------------------------------------*/
/* Copyright 2023 NXP                                                       */
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

#ifndef MCUXCSSLMEMORY_EXAMPLES_H_
#define MCUXCSSLMEMORY_EXAMPLES_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MCUXCSSL_MEMORY_EX_FUNCTION(_name) __attribute__((section(".example"))) bool _name(void)

#define MCUXCSSLMEMORY_EX_OK    true
#define MCUXCSSLMEMORY_EX_ERROR false

MCUXCSSL_MEMORY_EX_FUNCTION(mcuxCsslMemory_Compare_example);
MCUXCSSL_MEMORY_EX_FUNCTION(mcuxCsslMemory_Copy_example);
MCUXCSSL_MEMORY_EX_FUNCTION(mcuxCsslMemory_Clear_example);
MCUXCSSL_MEMORY_EX_FUNCTION(mcuxCsslMemory_Set_example);

#endif /* MCUXCSSLMEMORY_EXAMPLES_H_ */
