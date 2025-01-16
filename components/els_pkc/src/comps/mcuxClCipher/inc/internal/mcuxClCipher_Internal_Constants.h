/*--------------------------------------------------------------------------*/
/* Copyright 2021-2022, 2024 NXP                                            */
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

#ifndef MCUXCLCIPHER_INTERNAL_CONSTANTS_H_
#define MCUXCLCIPHER_INTERNAL_CONSTANTS_H_

/* Options for the crypt mode function */
#define MCUXCLCIPHER_OPTION_INIT    0x00000001u
#define MCUXCLCIPHER_OPTION_PROCESS 0x00000002u
#define MCUXCLCIPHER_OPTION_FINISH  0x00000004u
#define MCUXCLCIPHER_OPTION_ONESHOT (MCUXCLCIPHER_OPTION_INIT | MCUXCLCIPHER_OPTION_PROCESS | MCUXCLCIPHER_OPTION_FINISH)

#endif /* MCUXCLCIPHER_INTERNAL_CONSTANTS_H_ */
