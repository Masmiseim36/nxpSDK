/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ISR_H_
#define _ISR_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/  

#undef  VECTOR_015
#define VECTOR_015 SYSTICK_Isr

#if APPLICATION_WITH_NXP_TOUCH
  
  #undef  VECTOR_029
  #define VECTOR_029 LPUART1_IRQHandler

  #undef  VECTOR_040
  #define VECTOR_040 TSI_Isr

#endif

/*******************************************************************************
 * API
 ******************************************************************************/

extern void SYSTICK_Isr(void);
extern void LPUART1_IRQHandler(void);
extern void TSI_Isr(void);



#endif  /* _ISR_H_ */
