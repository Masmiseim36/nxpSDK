/*
* Copyright (c) 2016-2018, NXP Semiconductor
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/* DO NOT USE INCLUDE GUARDS - BREAKS LIST GENERATION     */

//                 LABEL          CHANNEL  PIN  /* FUNCTION */
#ifdef __LPC11U37H__
ISR_CALLBACK_ENTRY(ISR_GPIO_INT0, 0, 43)
#endif
#if defined(FRDM_K82F)
ISR_CALLBACK_ENTRY(ISR_INT_PORTC, 0, 10) /* PTC10  */
#endif
#if defined(FRDM_K64F)
ISR_CALLBACK_ENTRY(ISR_INT_PORTA, 0, 2) /* PTA2  */
#endif
#if defined(FRDM_KL27Z)
ISR_CALLBACK_ENTRY(ISR_INT_PORTE, 0, 24) /* PTE24  */
#endif
#if defined(FRDM_KL43Z)
ISR_CALLBACK_ENTRY(ISR_INT_PORTA, 0, 5) /* PTA5  */
#endif
#if defined(FRDM_K22F)
ISR_CALLBACK_ENTRY(ISR_INT_PORTB, 0, 18) /* PTB18  */
#endif
#if defined(FRDM_KL02Z)
ISR_CALLBACK_ENTRY(ISR_INT_PORTA, 0, 12) /* PTA12  */
#endif
#if defined(FRDM_KW41Z)
ISR_CALLBACK_ENTRY(ISR_INT_PORTC, 0, 17) /* PTC17  */
#endif

/* DO NOT USE INCLUDE GUARDS - BREAKS LIST GENERATION     */
