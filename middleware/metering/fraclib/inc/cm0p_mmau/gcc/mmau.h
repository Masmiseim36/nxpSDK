/************************************************************************************************
*
* Copyright 2013-2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*
***************************************************************************//*!
* @file      mmau.h
* @version   1.0.0.0
* @date      Jul-23-2014
* @brief     Memory Mapped Arithmetic Unit (MMAU) software driver header file
******************************************************************************/
#ifndef __MMAU_H
#define __MMAU_H

/******************************************************************************
 * MMAU memory map for use with operations                                    *
 ******************************************************************************/
/* Registers decorated load/store addresses                                   */
#define MMAU__X0        0xF0004000l /* Accumulator register X0                */
#define MMAU__X1        0xF0004004l /* Accumulator register X1                */
#define MMAU__X2        0xF0004008l /* Accumulator register X2                */
#define MMAU__X3        0xF000400Cl /* Accumulator register X3                */
#define MMAU__A0        0xF0004010l /* Accumulator register A0                */
#define MMAU__A1        0xF0004014l /* Accumulator register A1                */
#define MMAU__A10       0xF0004010l /* Accumulator register pair A10          */
/* Unsigned integer instructions decorated load/store addresses               */
#define MMAU__REGRW     0xF0004000l /* Registers RW                           */
#define MMAU__UMUL      0xF0004020l /* A10=X2*X3                              */
#define MMAU__UMULD     0xF0004040l /* A10=X21*X3                             */
#define MMAU__UMULDA    0xF0004060l /* A10=A10*X3                             */
#define MMAU__UMAC      0xF00040A0l /* A10=X2*X3+A10                          */
#define MMAU__UMACD     0xF00040C0l /* A10=X21*X3+A10                         */
#define MMAU__UMACDA    0xF00040E0l /* A10=A10*X3+X21                         */
#define MMAU__UDIV      0xF0004120l /* X21/X3=A10                             */
#define MMAU__UDIVD     0xF0004140l /* A10=X2/X3                              */
#define MMAU__UDIVDA    0xF0004160l /* A10=X21/X3                             */
#define MMAU__UDIVDD    0xF0004180l /* A10=A10/X3                             */
#define MMAU__UDIVDDA   0xF00041A0l /* A10=A10/X32                            */
#define MMAU__USQR      0xF0004220l /* A10=SQR(X3)                            */
#define MMAU__USQRD     0xF0004240l /* A10=SQR(X32)                           */
#define MMAU__USQRDA    0xF0004260l /* A10=SQR(A10)                           */
/* Signed fractional instructions decorated load/store addresses              */
#define MMAU__QSQR      0xF00042A0l /* A10=SQR(X3)                            */
#define MMAU__QSQRD     0xF00042C0l /* A10=SQR(X32)                           */
#define MMAU__QSQRDA    0xF00042E0l /* A10=SQR(A10)                           */
#define MMAU__QDIV      0xF0004320l /* A10=X2/X3                              */
#define MMAU__QDIVD     0xF0004340l /* A10=X21/X3                             */
#define MMAU__QDIVDA    0xF0004360l /* A10=A10/X3                             */
#define MMAU__QMUL      0xF0004420l /* A10=X2*X3                              */
#define MMAU__QMULD     0xF0004440l /* A10=X21*X3                             */
#define MMAU__QMULDA    0xF0004460l /* A10=A10*X3                             */
#define MMAU__QMAC      0xF00044A0l /* A10=X2*X3+A10                          */
#define MMAU__QMACD     0xF00044C0l /* A10=X21*X3+A10                         */
#define MMAU__QMACDA    0xF00044E0l /* A10=A10*X3+X21                         */
/* Signed integer instructions decorated load/store addresses                 */
#define MMAU__SMUL      0xF0004620l /* A10=X2*X3                              */
#define MMAU__SMULD     0xF0004640l /* A10=X21*X3                             */
#define MMAU__SMULDA    0xF0004660l /* A10=A10*X3                             */
#define MMAU__SMAC      0xF00046A0l /* A10=X2*X3+A10                          */
#define MMAU__SMACD     0xF00046C0l /* A10=X21*X3+A10                         */
#define MMAU__SMACDA    0xF00046E0l /* A10=A10*X3+X21                         */
#define MMAU__SDIV      0xF0004720l /* A10=X2/X3                              */
#define MMAU__SDIVD     0xF0004740l /* A10=X21/X3                             */
#define MMAU__SDIVDA    0xF0004760l /* A10=A10/X3                             */
#define MMAU__SDIVDD    0xF0004780l /* A10=X10/X32                            */
#define MMAU__SDIVDDA   0xF00047A0l /* A10=A10/X32                            */
/* Auxiliary decorated load/store addresses                                   */
#define MMAU__SAT       0xF0004800l /* Saturation                             */

/******************************************************************************
 * MMAU internal state definition                                             *
 ******************************************************************************/
typedef struct { uint32 x0, x1, x2, x3, a0, a1, csr; } tMMAU_STATE;

/******************************************************************************
 * MMAU instruction status flags definitions used by MMAU_GetInstrFlags() and
 * MMAU_SetInstrFlags() macros
 *
 *//*! @addtogroup mmau_instruction_flags
 * @{
 * List of instruction flags. These flags are updated on each calculation and
 * can be set/cleared by the @ref MMAU_WriteInstrFlags macro.
 ******************************************************************************/
#define MMAU_Q  (1l<<0)   ///< Accumulation Overflow
#define MMAU_V  (1l<<1)   ///< Multiply or Divide Overflow
#define MMAU_DZ (1l<<2)   ///< Divide by Zero
#define MMAU_N  (1l<<3)   ///< Signed calculation result is negative
/*! @} End of mmau_instruction_flags                                          */

/******************************************************************************
 * MMAU interrupt flags definitions used by MMAU_GetIntFlags() and
 * MMAU_ClrIntFlags() macros
 *
 *//*! @addtogroup mmau_interrupt_flags
 * @{
 * List of interrupt flags. These flags are sticky version of the @ref
 * mmau_instruction_flags. They can only be set by MMAU execution of
 * instructions and cleared by the @ref MMAU_ClrIntFlags macro.
 ******************************************************************************/
#define MMAU_QIF  (1l<<4)   ///< Accumulation Overflow Interrupt Flag
#define MMAU_VIF  (1l<<5)   ///< Multiply or Divide Overflow Interrupt Flag
#define MMAU_DZIF (1l<<6)   ///< Divide by Zero Interrupt Flag
/*! @} End of mmau_interrupt_flags                                            */

/******************************************************************************
 * MMAU configuration structure definitions                                   *
 ******************************************************************************/
typedef struct { uint32 CR; } tMMAU;

/******************************************************************************
 * MMAU default configurations used by MMAU_InitCallback() function
 *
 *//*! @addtogroup mmau_callback_config
 * @{
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Disable all interrupts of the MMAU module.
 * @details This configuration structure configures interrupts of the MMAU as
 *          follows:
 *          - Divide-by-Zero Interrupt disabled.
 *          - Divide/Multiply Overflow (V flag) Interrupt disabled.
 *          - Accumulation Overflow (Q flag) Interrupt disabled.
 * @showinitializer
 ******************************************************************************/
#define MMAU_DZIE_DI_VIE_DI_QIE_DI_CONFIG                                      \
(tMMAU){                                                                       \
/* CR */ CLR(MMAU_CSR_DZIE_MASK)|CLR(MMAU_CSR_VIE_MASK)|CLR(MMAU_CSR_QIE_MASK) \
}

/***************************************************************************//*!
 * @brief   Disable all interrupts of the MMAU module.
 * @details This configuration structure configures interrupts of the MMAU as
 *          follows:
 *          - Divide-by-Zero Interrupt enabled.
 *          - Divide/Multiply Overflow (V flag) Interrupt disabled.
 *          - Accumulation Overflow (Q flag) Interrupt disabled.
 * @showinitializer
 ******************************************************************************/
#define MMAU_DZIE_EN_VIE_DI_QIE_DI_CONFIG                                      \
(tMMAU){                                                                       \
/* CR */ SET(MMAU_CSR_DZIE_MASK)|CLR(MMAU_CSR_VIE_MASK)|CLR(MMAU_CSR_QIE_MASK) \
}

/***************************************************************************//*!
 * @brief   Disable all interrupts of the MMAU module.
 * @details This configuration structure configures interrupts of the MMAU as
 *          follows:
 *          - Divide-by-Zero Interrupt disabled.
 *          - Divide/Multiply Overflow (V flag) Interrupt enabled.
 *          - Accumulation Overflow (Q flag) Interrupt disabled.
 * @showinitializer
 ******************************************************************************/
#define MMAU_DZIE_DI_VIE_EN_QIE_DI_CONFIG                                      \
(tMMAU){                                                                       \
/* CR */ CLR(MMAU_CSR_DZIE_MASK)|SET(MMAU_CSR_VIE_MASK)|CLR(MMAU_CSR_QIE_MASK) \
}

/***************************************************************************//*!
 * @brief   Disable all interrupts of the MMAU module.
 * @details This configuration structure configures interrupts of the MMAU as
 *          follows:
 *          - Divide-by-Zero Interrupt enabled.
 *          - Divide/Multiply Overflow (V flag) Interrupt enabled.
 *          - Accumulation Overflow (Q flag) Interrupt disabled.
 * @showinitializer
 ******************************************************************************/
#define MMAU_DZIE_EN_VIE_EN_QIE_DI_CONFIG                                      \
(tMMAU){                                                                       \
/* CR */ SET(MMAU_CSR_DZIE_MASK)|SET(MMAU_CSR_VIE_MASK)|CLR(MMAU_CSR_QIE_MASK) \
}

/***************************************************************************//*!
 * @brief   Disable all interrupts of the MMAU module.
 * @details This configuration structure configures interrupts of the MMAU as
 *          follows:
 *          - Divide-by-Zero Interrupt disabled.
 *          - Divide/Multiply Overflow (V flag) Interrupt disabled.
 *          - Accumulation Overflow (Q flag) Interrupt enabled.
 * @showinitializer
 ******************************************************************************/
#define MMAU_DZIE_DI_VIE_DI_QIE_EN_CONFIG                                      \
(tMMAU){                                                                       \
/* CR */ CLR(MMAU_CSR_DZIE_MASK)|CLR(MMAU_CSR_VIE_MASK)|SET(MMAU_CSR_QIE_MASK) \
}

/***************************************************************************//*!
 * @brief   Disable all interrupts of the MMAU module.
 * @details This configuration structure configures interrupts of the MMAU as
 *          follows:
 *          - Divide-by-Zero Interrupt enabled.
 *          - Divide/Multiply Overflow (V flag) Interrupt disabled.
 *          - Accumulation Overflow (Q flag) Interrupt enabled.
 * @showinitializer
 ******************************************************************************/
#define MMAU_DZIE_EN_VIE_DI_QIE_EN_CONFIG                                      \
(tMMAU){                                                                       \
/* CR */ SET(MMAU_CSR_DZIE_MASK)|CLR(MMAU_CSR_VIE_MASK)|SET(MMAU_CSR_QIE_MASK) \
}

/***************************************************************************//*!
 * @brief   Disable all interrupts of the MMAU module.
 * @details This configuration structure configures interrupts of the MMAU as
 *          follows:
 *          - Divide-by-Zero Interrupt disabled.
 *          - Divide/Multiply Overflow (V flag) Interrupt enabled.
 *          - Accumulation Overflow (Q flag) Interrupt enabled.
 * @showinitializer
 ******************************************************************************/
#define MMAU_DZIE_DI_VIE_EN_QIE_EN_CONFIG                                      \
(tMMAU){                                                                       \
/* CR */ CLR(MMAU_CSR_DZIE_MASK)|SET(MMAU_CSR_VIE_MASK)|SET(MMAU_CSR_QIE_MASK) \
}

/***************************************************************************//*!
 * @brief   Enable all interrupts of the MMAU module.
 * @details This configuration structure configures interrupts of the MMAU as
 *          follows:
 *          - Divide-by-Zero Interrupt enabled.
 *          - Divide/Multiply Overflow (V flag) Interrupt enabled.
 *          - Accumulation Overflow (Q flag) Interrupt enabled.
 * @showinitializer
 ******************************************************************************/
#define MMAU_DZIE_EN_VIE_EN_QIE_EN_CONFIG                                      \
(tMMAU){                                                                       \
/* CR */ SET(MMAU_CSR_DZIE_MASK)|SET(MMAU_CSR_VIE_MASK)|SET(MMAU_CSR_QIE_MASK) \
}
/*! @} End of mmau_callback_config                                            */

/******************************************************************************
 * MMAU callback registered by the MMAU_InstallCallback() function
 *
 *//*! @addtogroup mmau_callback
 * @{
 ******************************************************************************/
/*! @brief MMAU_CALLBACK type declaration                                     */
/*! MMAU_CALLBACK type declaration                                            */
typedef enum
{
  DZIF_CALLBACK =1, ///< For divide, divide-by-zero error
  VIF_CALLBACK  =2, ///< Product in MUL, MAC or quotient of a divide overflows
  QIF_CALLBACK  =4  ///< Accumulation overflows during a MAC instruction
} MMAU_CALLBACK_TYPE;

/*! @brief MMAU_CALLBACK function declaration                                 */
typedef void (*MMAU_CALLBACK)(MMAU_CALLBACK_TYPE type);
/*! @} End of mmau_callback                                                   */

/******************************************************************************
 * MMAU instruction macros.
 *
 *//*! @addtogroup mmau_macros
 * @{
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   DMA request enable/disable.
 * @details Call @ref MMAU_EnableDMA macro to configure MMAU to allow (true) or
 *          prevent (false) the DMA request to fetch the result and program new
 *          computation instruction.
 * @param   mode      Enable (true) or disable (false) DMA request generation.
 ******************************************************************************/
#define MMAU_EnableDMA(mode)  { MMAU_CSR=((MMAU_CSR&~(1ul<<16))|((mode)<<16)); }

/***************************************************************************//*!
 * @brief   Sets CPU/DMA access to MMAU Operand, Accumulator and Control/Status
 *          registers.
 * @details Call @ref MMAU_SetAccess macro to set CPU/DMA access mode to MMAU
 *          Operand, Accumulator and Control/Status registers. MMAU registers
 *          can either be accessed in Supervisor Mode (true) or in both User and
 *          Supervisor Modes (false). In Supervisor Mode, when CPU/DMA access
 *          registers in User Mode, MMAU will terminate the access with an bus
 *          error.
 * @param   mode      Supervisor Mode (true) or both User and Supervisor Modes
 *                    (false).
 ******************************************************************************/
#define MMAU_SetAccess(mode)  { MMAU_CSR=((MMAU_CSR&~(1ul<<17))|((mode)<<17)); }

/***************************************************************************//*!
 * @brief   Get instruction result flags.
 * @details Call @ref MMAU_GetInstrFlags macro to get instruction result flags.
 * @return  @ref uint32 @ref mmau_instruction_flags.
 * @see     @ref MMAU_WriteInstrFlags.
 ******************************************************************************/
#define MMAU_GetInstrFlags()  (MMAU_CSR&(MMAU_Q|MMAU_V|MMAU_DZ|MMAU_N))

/***************************************************************************//*!
 * @brief   Write instruction result flags.
 * @details Call @ref MMAU_WriteInstrFlags macro to write instruction result
 *          flags.
 * @param   mask      Select one or more OR'ed @ref mmau_instruction_flags.
 * @see     @ref MMAU_GetInstrFlags.
 ******************************************************************************/
#define MMAU_WriteInstrFlags(mask)  { MMAU_CSR_IF_CLR=(mask); }

/***************************************************************************//*!
 * @brief   Get interrupt flags.
 * @details Call @ref MMAU_GetIntFlags macro to get interrupt flags.
 * @return  @ref uint32 @ref mmau_interrupt_flags.
 * @see     @ref MMAU_ClrIntFlags.
 ******************************************************************************/
#define MMAU_GetIntFlags()          (MMAU_CSR&(MMAU_QIF|MMAU_VIF|MMAU_DZIF))

/***************************************************************************//*!
 * @brief   Clear interrupt flags.
 * @details Call @ref MMAU_ClrIntFlags macro to clear interrupt flags.
 * @param   mask      Select one or more OR'ed @ref mmau_interrupt_flags.
 * @see     @ref MMAU_GetIntFlags.
 ******************************************************************************/
#define MMAU_ClrIntFlags(mask)      { MMAU_CSR_IF_CLR=(mask); }

#define store_state(p)                                                         \
{                                                                              \
  register uint32 _src = (uint32)(MMAU__REGRW|MMAU__X0);                       \
  register uint32 _dst = (uint32)p;                                            \
  __asm volatile                                                               \
  (                                                                            \
    "ldm %0 ,{%0,r2,r3,r4,r5,r6,r7}\n"                                         \
    "stm %1!,{%0,r2,r3,r4,r5,r6,r7}\n"                                         \
    :"=l"(_src),"=l"(_dst):"0"(_src),"1"(_dst):"r2","r3","r4","r5","r6","r7"   \
  );                                                                           \
}
/***************************************************************************//*!
 * @brief   Store MMAU internal state to the software stack.
 * @details The @ref MMAU_StoreState function stores MMAU internal state
 *          including operand, accumulator and control/status registers to the
 *          software stack.
 * @note    Call this function at entry point of any interrupt service routine
 *          which uses @ref mmau_macros. At the exit of such interrupt service
 *          routine you should call @ref MMAU_RestoreState function.
 * @see     @ref MMAU_RestoreState
 ******************************************************************************/
#define MMAU_StoreState() tMMAU_STATE volatile __tmp; store_state(&__tmp)

#define restore_state(p)                                                       \
{                                                                              \
  register uint32 _src = (uint32)p;                                            \
  register uint32 _dst = (uint32)(MMAU__REGRW|MMAU__X0);                       \
  __asm volatile                                                               \
  (                                                                            \
    "ldm %0 ,{%0,r2,r3,r4,r5,r6,r7}\n"                                         \
    "stm %1!,{%0,r2,r3,r4,r5,r6,r7}\n"                                         \
    :"=l"(_src),"=l"(_dst):"0"(_src),"1"(_dst):"r2","r3","r4","r5","r6","r7"   \
  );                                                                           \
}
/***************************************************************************//*!
 * @brief   Restore MMAU internal state from the software stack.
 * @details The @ref MMAU_RestoreState function restores MMAU internal state
 *          including operand, accumulator and control/status registers from the
 *          software stack.
 * @note    Call this function at exit of any interrupt service routine
 *          which uses @ref mmau_macros. At entry point of such interrupt
 *          service routine you should call @ref MMAU_StoreState function.
 * @see     @ref MMAU_StoreState
 ******************************************************************************/
#define MMAU_RestoreState() restore_state(&__tmp)

/***************************************************************************//*!
 * @brief   Installs callback function for MMAU interrupt vector 36.
 * @details This function installs callback function for MMAU interrupt vector
 *          30.
 * @param   cfg       Select one of the @ref mmau_callback_config.
 * @param   ip        Select one of the Select one of the @ref cm0plus_prilvl.
 * @param   callback  Pointer to the @ref mmau_callback.
 * @note    Implemented as a function call.
 ******************************************************************************/
#define MMAU_InstallCallback(cfg,ip,callback)                                  \
                                          MMAU_InstallCallback(cfg,ip,callback)
/*! @} End of mmau_macros                                                     */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void MMAU_InstallCallback(tMMAU cfg, uint8 ip, MMAU_CALLBACK pCallback);

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
extern void mmau_isr (void);

/******************************************************************************
 * MMAU instruction set.
 *
 *//*! @addtogroup uint_instructions
 * @{
 ******************************************************************************/

/***************************************************************************//*!
 * @brief   Load A10 accumulator register of the MMAU by 64-bit unsigned value.
 * @details The @ref ulda_d function loads A10 accumulator register of the MMAU
 *          by 64-bit unsigned value.
 * @param   dval    @ref uint64 unsigned value.
 ******************************************************************************/
#define ulda_d(dval)                                                           \
({                                                                             \
  register uint32 addr = (MMAU__REGRW|MMAU__A10);                              \
  register uint64 inp  = (dval);                                               \
  __asm volatile ("stm %0!,{%Q1,%R1}":"=l"(addr):"l"(inp),"0"(addr));          \
})

/***************************************************************************//*!
 * @brief   Read 32-bit unsigned value from the A0 accumulator register of the
 *          MMAU.
 * @details The @ref l_urda function reads 32-bit unsigned value from the A0
 *          accumulator register of the MMAU.
 * @return  @ref uint32 unsigned value.
 ******************************************************************************/
#define l_urda()                                                               \
({                                                                             \
  register uint32 out = (MMAU__REGRW|MMAU__A0);                                \
  __asm volatile ("ldr %0,[%0,#+0]":"=l"(out):"0"(out));                       \
  (uint32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Read 64-bit unsigned value from the A10 accumulator register of the
 *          MMAU.
 * @details The @ref d_urda function reads 64-bit unsigned value from the A10
 *          accumulator register of the MMAU.
 * @return  @ref uint64 unsigned value.
 ******************************************************************************/
#define d_urda(void)                                                           \
({                                                                             \
  register union {struct {uint32 lo, hi;} l; uint64 d;} out;                   \
  out.l.lo=(MMAU__REGRW|MMAU__A10);                                            \
  __asm volatile ("ldm %Q0,{%Q0,%R0}":"=l"(out.d):"0"(out.d));                 \
  (uint64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit unsigned values returning a 64-bit unsigned
 *          product.
 * @details The @ref d_umul_ll function multiplies two 32-bit unsigned values
 *          returning a 64-bit unsigned product.
 * @param   lval1   @ref uint32 unsigned value.
 * @param   lval2   @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_umul_ll(lval1,lval2)                                                 \
({                                                                             \
  register uint32 addr = (MMAU__UMUL|MMAU__X2);                                \
  register union {struct {uint32 lo, hi;} l; uint64 d;} out;                   \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (uint64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit unsigned values.
 * @details The @ref umul_ll function multiplies two 32-bit unsigned values.
 * @param   lval1   @ref uint32 unsigned value.
 * @param   lval2   @ref uint32 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define umul_ll(lval1,lval2)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__UMUL|MMAU__X2);                                \
  register uint32 inp1 = (lval1);                                              \
  register uint32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit unsigned value with 32-bit unsigned value returning
 *          a 64-bit unsigned product.
 * @details The @ref d_umul_dl function multiplies 64-bit unsigned value with
 *          32-bit unsigned value returning a 64-bit unsigned product.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_umul_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__UMULD|MMAU__X1);                               \
  register uint64 out  = (dval);                                               \
  register uint32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit unsigned value with 32-bit unsigned value
 *          returning saturated 64-bit unsigned product.
 * @details The @ref d_umuls_dl function multiplies 64-bit unsigned value with
 *          32-bit unsigned value returning saturated 64-bit unsigned product.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_umuls_dl(dval,lval)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__UMULD|MMAU__X1|MMAU__SAT);                     \
  register uint64 out  = (dval);                                               \
  register uint32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit unsigned value with 32-bit unsigned value.
 * @details The @ref umul_dl function multiplies 64-bit unsigned value with
 *          32-bit unsigned value.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define umul_dl(dval,lval)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__UMULD|MMAU__X1);                               \
  register uint64 inp1 = (dval);                                               \
  register uint32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit unsigned value with 32-bit unsigned
 *          value.
 * @details The @ref umuls_dl function multiplies 64-bit unsigned value with
 *          32-bit unsigned value.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define umuls_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__UMULD|MMAU__X1|MMAU__SAT);                     \
  register uint64 inp1 = (dval);                                               \
  register uint32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit unsigned value with 64-bit unsigned value stored in
 *          the A10 register of the MMAU returning a 64-bit unsigned product.
 * @details The @ref d_umula_l function multiplies 32-bit unsigned value with
 *          64-bit unsigned value stored in the A10 register of the MMAU
 *          returning a 64-bit unsigned product.
 * @param   lval    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_umula_l(lval)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__UMULDA|MMAU__X3);                              \
  register union {struct {uint32 lo, hi;} l; uint64 d;} out;                   \
  out.l.lo=(lval);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (uint64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit unsigned value with 64-bit unsigned value
 *          stored in the A10 register of the MMAU returning saturated 64-bit
 *          unsigned product.
 * @details The @ref d_umulas_l function multiplies 32-bit unsigned value with
 *          64-bit unsigned value stored in the A10 register of the MMAU
 *          returning saturated 64-bit unsigned product.
 * @param   lval    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_umulas_l(lval)                                                       \
({                                                                             \
  register uint32 addr = (MMAU__UMULDA|MMAU__X3|MMAU__SAT);                    \
  register union {struct {uint32 lo, hi;} l; uint64 d;} out;                   \
  out.l.lo=(lval);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (uint64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit unsigned value with 64-bit unsigned value stored in
 *          the A10 register of the MMAU.
 * @details The @ref umula_l function multiplies 32-bit unsigned value with
 *          64-bit unsigned value stored in the A10 register of the MMAU.
 * @param   lval    @ref uint32 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define umula_l(lval)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__UMULDA|MMAU__X3);                              \
  register uint32 inp  = (lval);                                               \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit unsigned value with 64-bit unsigned value
 *          stored in the A10 register of the MMAU.
 * @details The @ref umulas_l function multiplies 32-bit unsigned value with
 *          64-bit unsigned value stored in the A10 register of the MMAU.
 * @param   lval    @ref uint32 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define umulas_l(lval)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__UMULDA|MMAU__X3|MMAU__SAT);                    \
  register uint32 inp  = (uint32)(lval);                                       \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit unsigned values and add product with value
 *          stored in the A10 register of the MMAU returning a 64-bit unsigned
 *          A10 register value.
 * @details The @ref d_umac_ll function multiplies two 32-bit unsigned values
 *          and add product with value stored in the A10 register of the MMAU
 *          returning a 64-bit unsigned A10 register value.
 * @param   lval1   @ref uint32 unsigned value.
 * @param   lval2   @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_umac_ll(lval1,lval2)                                                 \
({                                                                             \
  register uint32 addr = (MMAU__UMAC|MMAU__X2);                                \
  register union {struct {uint32 lo, hi;} l; uint64 d;} out;                   \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (uint64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit unsigned values and add product with
 *          value stored in the A10 register of the MMAU returning a 64-bit
 *          unsigned A10 register value.
 * @details The @ref d_umacs_ll function multiplies two 32-bit unsigned values
 *          and add product with value stored in the A10 register of the MMAU
 *          returning saturated 64-bit unsigned A10 register value.
 * @param   lval1   @ref uint32 unsigned value.
 * @param   lval2   @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_umacs_ll(lval1,lval2)                                                \
({                                                                             \
  register uint32 addr = (MMAU__UMAC|MMAU__X2|MMAU__SAT);                      \
  register union {struct {uint32 lo, hi;} l; uint64 d;} out;                   \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (uint64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit unsigned values and add product with value
 *          stored in the A10 register of the MMAU.
 * @details The @ref umac_ll function multiplies two 32-bit unsigned values and
 *          add product with value stored in the A10 register of the MMAU.
 * @param   lval1   @ref uint32 unsigned value.
 * @param   lval2   @ref uint32 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define umac_ll(lval1,lval2)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__UMAC|MMAU__X2);                                \
  register uint32 inp1 = (lval1);                                              \
  register uint32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit unsigned values and add product with
 *          value stored in the A10 register of the MMAU.
 * @details The @ref umacs_ll function multiplies two 32-bit unsigned values and
 *          add product with value stored in the A10 register of the MMAU.
 * @param   lval1   @ref uint32 unsigned value.
 * @param   lval2   @ref uint32 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define umacs_ll(lval1,lval2)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__UMAC|MMAU__X2|MMAU__SAT);                      \
  register uint32 inp1 = (lval1);                                              \
  register uint32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit unsigned value with 32-bit unsigned value and add
 *          product with value stored in the A10 register of the MMAU returning
 *          a 64-bit unsigned A10 register value.
 * @details The @ref d_umac_dl function multiplies 64-bit unsigned value with
 *          32-bit unsigned value and add product with value stored in the A10
 *          register of the MMAU returning a 64-bit unsigned A10 register value.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_umac_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__UMACD|MMAU__X1);                               \
  register uint64 out  = (dval);                                               \
  register uint32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit unsigned value with 32-bit unsigned value
 *          and add product with value stored in the A10 register of the MMAU
 *          returning saturated 64-bit unsigned A10 register value.
 * @details The @ref d_umacs_dl function multiplies 64-bit unsigned value with
 *          32-bit unsigned value and add product with value stored in the A10
 *          register of the MMAU returning saturated 64-bit unsigned A10
 *          register value.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_umacs_dl(dval,lval)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__UMACD|MMAU__X1|MMAU__SAT);                     \
  register uint64 out  = (dval);                                               \
  register uint32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit unsigned value with 32-bit unsigned value and add
 *          product with value stored in the A10 register of the MMAU.
 * @details The @ref umac_dl function multiplies 64-bit unsigned value with
 *          32-bit unsigned value and add product with value stored in the A10
 *          register of the MMAU.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define umac_dl(dval,lval)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__UMACD|MMAU__X1);                               \
  register uint64 inp1 = (dval);                                               \
  register uint32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit unsigned value with 32-bit unsigned value
 *          and add product with value stored in the A10 register of the MMAU.
 * @details The @ref umacs_dl function multiplies 64-bit unsigned value with
 *          32-bit unsigned value and add product with value stored in the A10
 *          register of the MMAU.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define umacs_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__UMACD|MMAU__X1|MMAU__SAT);                     \
  register uint64 inp1 = (dval);                                               \
  register uint32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit unsigned value by value stored in the A10 register
 *          of the MMAU and add product with 64-bit unsigned value returning a
 *          64-bit unsigned A10 register value.
 * @details The @ref d_umaca_dl function multiplies 32-bit unsigned value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit unsigned value returning a 64-bit unsigned A10 register
 *          value.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_umaca_dl(dval,lval)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__UMACDA|MMAU__X1);                              \
  register uint64 out  = (dval);                                               \
  register uint32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit unsigned value by value stored in the A10
 *          register of the MMAU and add product with 64-bit unsigned value
 *          returning a saturated 64-bit unsigned A10 register value.
 * @details The @ref d_umacas_dl function multiplies 32-bit unsigned value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit unsigned value returning saturated 64-bit unsigned A10
 *          register value.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_umacas_dl(dval,lval)                                                 \
({                                                                             \
  register uint32 addr = (MMAU__UMACDA|MMAU__X1|MMAU__SAT);                    \
  register uint64 out  = (dval);                                               \
  register uint32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit unsigned value by value stored in the A10 register
 *          of the MMAU and add product with 64-bit unsigned value.
 * @details The @ref umaca_dl function multiplies 32-bit unsigned value by value
 *          stored in the A10 register of the MMAU and add product with 64-bit
 *          unsigned value.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define umaca_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__UMACDA|MMAU__X1);                              \
  register uint64 inp1 = (dval);                                               \
  register uint32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit unsigned value by value stored in the A10
 *          register of the MMAU and add product with 64-bit unsigned value.
 * @details The @ref umacas_dl function multiplies 32-bit unsigned value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit unsigned value.
 * @param   dval    @ref uint64 unsigned value.
 * @param   lval    @ref uint32 unsigned value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define umacas_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__UMACDA|MMAU__X1|MMAU__SAT);                    \
  register uint64 inp1 = (dval);                                               \
  register uint32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit unsigned values returning a 32-bit unsigned
 *          quotient.
 * @details The @ref l_udiv_ll function divides two 32-bit unsigned values
 *          returning a 32-bit unsigned quotient.
 * @param   lnum    @ref uint32 unsigned value.
 * @param   lden    @ref uint32 unsigned value.
 * @return  @ref uint32 unsigned value.
 * @note    Quotient is stored in A0 register of the MMAU for next computation.
 ******************************************************************************/
#define l_udiv_ll(lnum,lden)                                                   \
({                                                                             \
  register uint32 out = (MMAU__UDIV|MMAU__X2);                                 \
  register uint32 inp1 = (lnum);                                               \
  register uint32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]\n"                                                        \
    "ldr %0,[%0,#+8]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (uint32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit unsigned values.
 * @details The @ref udiv_ll function divides two 32-bit unsigned values.
 * @param   lnum    @ref uint32 unsigned value.
 * @param   lden    @ref uint32 unsigned value.
 * @note    Quotient is stored in A0 register of the MMAU for next computation.
 ******************************************************************************/
#define udiv_ll(lnum,lden)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__UDIV|MMAU__X2);                                \
  register uint32 inp1 = (lnum);                                               \
  register uint32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit unsigned value by 32-bit unsigned value returning a
 *          64-bit unsigned quotient.
 * @details The @ref d_udiv_dl function divides 64-bit unsigned value by 32-bit
 *          unsigned value returning a 64-bit unsigned quotient.
 * @param   dnum    @ref uint64 unsigned value.
 * @param   lden    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_udiv_dl(dnum,lden)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__UDIVD|MMAU__X1);                               \
  register uint64 out  = (dnum);                                               \
  register uint32 inp  = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit unsigned value by 32-bit unsigned value.
 * @details The @ref udiv_dl function divides 64-bit unsigned value by 32-bit
 *          unsigned value.
 * @param   dnum    @ref uint64 unsigned value.
 * @param   lden    @ref uint32 unsigned value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define udiv_dl(dnum,lden)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__UDIVD|MMAU__X1);                               \
  register uint64 inp1 = (dnum);                                               \
  register uint32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide two 64-bit unsigned values returning a 64-bit unsigned
 *          quotient.
 * @details The @ref d_udiv_dd function divides two 64-bit unsigned values
 *          returning a 64-bit unsigned quotient.
 * @param   dnum    @ref uint64 unsigned value.
 * @param   dden    @ref uint64 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_udiv_dd(dnum,dden)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__UDIVDD|MMAU__X0);                              \
  register uint64 out  = (dnum);                                               \
  register uint64 inp  = (dden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%Q2,%R2}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide two 64-bit unsigned values.
 * @details The @ref udiv_dd function divides two 64-bit unsigned values.
 * @param   dnum    @ref uint64 unsigned value.
 * @param   dden    @ref uint64 unsigned value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define udiv_dd(dnum,dden)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__UDIVDD|MMAU__X0);                              \
  register uint64 inp1 = (dnum);                                               \
  register uint64 inp2 = (dden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%Q2,%R2}  ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit unsigned value stored in the A10 register of the MMAU
 *          by 32-bit unsigned value returning a 64-bit unsigned quotient.
 * @details The @ref d_udiva_l function divides 64-bit unsigned value stored in
 *          the A10 register of the MMAU by 32-bit unsigned value returning a
 *          64-bit unsigned quotient.
 * @param   lden    @ref uint32 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_udiva_l(lden)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__UDIVDA|MMAU__X3);                              \
  register union {struct {uint32 lo, hi;} l; uint64 d;} out;                   \
  out.l.lo=(lden);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (uint64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit unsigned value stored in the A10 register of the MMAU
 *          by 32-bit unsigned value.
 * @details The @ref udiva_l function divides 64-bit unsigned value stored in
 *          the A10 register of the MMAU by 32-bit unsigned value.
 * @param   lden    @ref uint32 unsigned value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define udiva_l(lden)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__UDIVDA|MMAU__X3);                              \
  register uint32 inp  = (lden);                                               \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit unsigned value stored in the A10 register of the MMAU
 *          by 64-bit unsigned value returning a 64-bit unsigned quotient.
 * @details The @ref d_udiva_d function divides 64-bit unsigned value stored in
 *          the A10 register of the MMAU by 64-bit unsigned value returning a
 *          64-bit unsigned quotient.
 * @param   dden    @ref uint64 unsigned value.
 * @return  @ref uint64 unsigned value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_udiva_d(dden)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__UDIVDDA|MMAU__X2);                             \
  register uint64 out = (dden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"0"(addr),"1"(out)              \
  );                                                                           \
  (uint64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit unsigned value stored in the A10 register of the MMAU
 *          by 64-bit unsigned value.
 * @details The @ref udiva_d function divides 64-bit unsigned value stored in
 *          the A10 register of the MMAU by 64-bit unsigned value.
 * @param   dden    @ref uint64 unsigned value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define udiva_d(dden)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__UDIVDDA|MMAU__X2);                             \
  register uint64 inp  = (dden);                                               \
  __asm volatile ("stm %0!,{%Q1,%R1}":"=l"(addr):"l"(inp),"0"(addr));          \
})

/***************************************************************************//*!
 * @brief   Compute and return a 16-bit unsigned square root of the 32-bit
 *          unsigned radicand.
 * @details The @ref s_usqr_l function computes and returns a 16-bit unsigned
 *          square root of the 32-bit unsigned radicand.
 * @param   lrad    @ref uint32 unsigned radicand.
 * @return  @ref uint16 unsigned square root.
 * @note    Square root is stored in A0 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define s_usqr_l(lrad)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__USQR|MMAU__X3);                                \
  register uint32 tmp  = (lrad);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %0,[%1,#+0]\n"                                                        \
    "ldr %0,[%1,#+4]  ":"=l"(tmp):"l"(addr),"0"(tmp)                           \
  );                                                                           \
  (uint16)tmp;                                                                 \
})

/***************************************************************************//*!
 * @brief   Compute a 16-bit unsigned square root of the 32-bit unsigned
 *          radicand.
 * @details The @ref usqr_l function computes a 16-bit unsigned square root of
 *          the 32-bit unsigned radicand.
 * @param   lrad    @ref uint32 unsigned radicand.
 * @note    Square root is stored in A0 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define usqr_l(lrad)                                                           \
({                                                                             \
  register uint32 addr = (MMAU__USQR|MMAU__X3);                                \
  register uint32 inp  = (lrad);                                               \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Compute and return a 32-bit unsigned square root of the 64-bit
 *          unsigned radicand.
 * @details The @ref l_usqr_d function computes and returns a 32-bit unsigned
 *          square root of the 64-bit unsigned radicand.
 * @param   drad    @ref uint64 unsigned radicand.
 * @return  @ref uint32 unsigned square root.
 * @note    Square root is stored in A0 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_usqr_d(drad)                                                         \
({                                                                             \
  register uint32 out = (MMAU__USQRD|MMAU__X2);                                \
  register uint64 inp = (drad);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0 ,{%0}       ":"=l"(out):"l"(inp),"0"(out)                          \
  );                                                                           \
  (uint32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Compute a 32-bit unsigned square root of the 64-bit unsigned
 *          radicand.
 * @details The @ref usqr_d function computes a 32-bit unsigned square root of
 *          the 64-bit unsigned radicand.
 * @param   drad    @ref uint64 unsigned radicand.
 * @note    Square root is stored in A0 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define usqr_d(drad)                                                           \
({                                                                             \
  register uint32 addr = (MMAU__USQRD|MMAU__X2);                               \
  register uint64 inp  = (drad);                                               \
  __asm volatile ("stm %0!,{%Q1,%R1}":"=l"(addr):"l"(inp),"0"(addr));          \
})

/***************************************************************************//*!
 * @brief   Compute and return a 32-bit unsigned square root of the radicand
 *          stored in the A10 register of the MMAU.
 * @details The @ref l_usqra function computes and returns a 32-bit unsigned
 *          square root of the radicand stored in the A10 register of the MMAU.
 * @return  @ref uint32 unsigned square root.
 * @note    Square root is stored in A0 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_usqra()                                                              \
({                                                                             \
  register uint32 out = (MMAU__REGRW|MMAU__A0);                                \
  __asm volatile ("ldr %0,[%0,#+0]":"=l"(out):"0"(out));/* dummy read */       \
  out = (MMAU__USQRDA|MMAU__A0);                                               \
  __asm volatile ("ldr %0,[%0,#+0]":"=l"(out):"0"(out));                       \
  (uint32)out;                                                                 \
})

/*! @} End of uint_instructions                                               */

/******************************************************************************
 * MMAU instruction set.
 *
 *//*! @addtogroup int_instructions
 * @{
 ******************************************************************************/

/***************************************************************************//*!
 * @brief   Load A10 accumulator register of the MMAU by 64-bit integer value.
 * @details The @ref slda_d function loads A10 accumulator register of the MMAU
 *          by 64-bit integer value.
 * @param   dval    @ref int64 integer value.
 ******************************************************************************/
#define slda_d(dval)                                                           \
({                                                                             \
  register uint32 addr = (MMAU__REGRW|MMAU__A10);                              \
  register int64 inp  = (dval);                                                \
  __asm volatile ("stm %0!,{%Q1,%R1}":"=l"(addr):"l"(inp),"0"(addr));          \
})

/***************************************************************************//*!
 * @brief   Read 32-bit integer value from the A0 accumulator register of the
 *          MMAU.
 * @details The @ref l_srda function reads 32-bit integer value from the A0
 *          accumulator register of the MMAU.
 * @return  @ref int32 integer value.
 ******************************************************************************/
#define l_srda()                                                               \
({                                                                             \
  register uint32 out = (MMAU__REGRW|MMAU__A0);                                \
  __asm volatile ("ldr %0,[%0,#+0]":"=l"(out):"0"(out));                       \
  (int32)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Read 64-bit integer value from the A10 accumulator register of the
 *          MMAU.
 * @details The @ref d_srda function reads 64-bit integer value from the A10
 *          accumulator register of the MMAU.
 * @return  @ref int64 integer value.
 ******************************************************************************/
#define d_srda(void)                                                           \
({                                                                             \
  register union {struct {uint32 lo, hi;} l; int64 d;} out;                    \
  out.l.lo=(MMAU__REGRW|MMAU__A10);                                            \
  __asm volatile ("ldm %Q0,{%Q0,%R0}":"=l"(out.d):"0"(out.d));                 \
  (int64)out.d;                                                                \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit integer values returning a 64-bit integer
 *          product.
 * @details The @ref d_smul_ll function multiplies two 32-bit integer values
 *          returning a 64-bit integer product.
 * @param   lval1   @ref int32 integer value.
 * @param   lval2   @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_smul_ll(lval1,lval2)                                                 \
({                                                                             \
  register uint32 addr = (MMAU__SMUL|MMAU__X2);                                \
  register union {struct {int32 lo, hi;} l; int64 d;} out;                     \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (int64)out.d;                                                                \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit integer values.
 * @details The @ref smul_ll function multiplies two 32-bit integer values.
 * @param   lval1   @ref int32 integer value.
 * @param   lval2   @ref int32 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define smul_ll(lval1,lval2)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__SMUL|MMAU__X2);                                \
  register int32 inp1 = (lval1);                                               \
  register int32 inp2 = (lval2);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit integer value with 32-bit integer value returning a
 *          64-bit integer product.
 * @details The @ref d_smul_dl function multiplies 64-bit integer value with
 *          32-bit integer value returning a 64-bit integer product.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_smul_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__SMULD|MMAU__X1);                               \
  register int64 out  = (dval);                                                \
  register int32 inp  = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit integer value with 32-bit integer value
 *          returning saturated 64-bit integer product.
 * @details The @ref d_smuls_dl function multiplies 64-bit integer value with
 *          32-bit integer value returning saturated 64-bit integer product.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_smuls_dl(dval,lval)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__SMULD|MMAU__X1|MMAU__SAT);                     \
  register int64 out  = (dval);                                                \
  register int32 inp  = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit integer value with 32-bit integer value.
 * @details The @ref smul_dl function multiplies 64-bit integer value with
 *          32-bit integer value.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define smul_dl(dval,lval)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__SMULD|MMAU__X1);                               \
  register int64 inp1 = (dval);                                                \
  register int32 inp2 = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit integer value with 32-bit integer value.
 * @details The @ref smuls_dl function multiplies 64-bit integer value with
 *          32-bit integer value.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define smuls_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__SMULD|MMAU__X1|MMAU__SAT);                     \
  register int64 inp1 = (dval);                                                \
  register int32 inp2 = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit integer value with 64-bit integer value stored in
 *          the A10 register of the MMAU returning a 64-bit integer product.
 * @details The @ref d_smula_l function multiplies 32-bit integer value with
 *          64-bit integer value stored in the A10 register of the MMAU
 *          returning a 64-bit integer product.
 * @param   lval    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_smula_l(lval)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__SMULDA|MMAU__X3);                              \
  register union {struct {int32 lo, hi;} l; int64 d;} out;                     \
  out.l.lo=(lval);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (int64)out.d;                                                                \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit integer value with 64-bit integer value
 *          stored in the A10 register of the MMAU returning saturated 64-bit
 *          integer product.
 * @details The @ref d_smulas_l function multiplies 32-bit integer value with
 *          64-bit integer value stored in the A10 register of the MMAU
 *          returning saturated 64-bit integer product.
 * @param   lval    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_smulas_l(lval)                                                       \
({                                                                             \
  register uint32 addr = (MMAU__SMULDA|MMAU__X3|MMAU__SAT);                    \
  register union {struct {int32 lo, hi;} l; int64 d;} out;                     \
  out.l.lo=(lval);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (int64)out.d;                                                                \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit integer value with 64-bit integer value stored in
 *          the A10 register of the MMAU.
 * @details The @ref smula_l function multiplies 32-bit integer value with
 *          64-bit integer value stored in the A10 register of the MMAU.
 * @param   lval    @ref int32 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define smula_l(lval)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__SMULDA|MMAU__X3);                              \
  register int32 inp  = (lval);                                                \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit integer value with 64-bit integer value
 *          stored in the A10 register of the MMAU.
 * @details The @ref smulas_l function multiplies 32-bit integer value with
 *          64-bit integer value stored in the A10 register of the MMAU.
 * @param   lval    @ref int32 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define smulas_l(lval)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__SMULDA|MMAU__X3|MMAU__SAT);                    \
  register int32 inp  = (int32)(lval);                                         \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit integer values and add product with value stored
 *          in the A10 register of the MMAU returning a 64-bit integer A10
 *          register value.
 * @details The @ref d_smac_ll function multiplies two 32-bit integer values and
 *          add product with value stored in the A10 register of the MMAU
 *          returning a 64-bit integer A10 register value.
 * @param   lval1   @ref int32 integer value.
 * @param   lval2   @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_smac_ll(lval1,lval2)                                                 \
({                                                                             \
  register uint32 addr = (MMAU__SMAC|MMAU__X2);                                \
  register union {struct {int32 lo, hi;} l; int64 d;} out;                     \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (int64)out.d;                                                                \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit integer values and add product with
 *          value stored in the A10 register of the MMAU returning a 64-bit
 *          integer A10 register value.
 * @details The @ref d_smacs_ll function multiplies two 32-bit integer values
 *          and add product with value stored in the A10 register of the MMAU
 *          returning saturated 64-bit integer A10 register value.
 * @param   lval1   @ref int32 integer value.
 * @param   lval2   @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_smacs_ll(lval1,lval2)                                                \
({                                                                             \
  register uint32 addr = (MMAU__SMAC|MMAU__X2|MMAU__SAT);                      \
  register union {struct {int32 lo, hi;} l; int64 d;} out;                     \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (int64)out.d;                                                                \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit integer values and add product with value stored
 *          in the A10 register of the MMAU.
 * @details The @ref smac_ll function multiplies two 32-bit integer values and
 *          add product with value stored in the A10 register of the MMAU.
 * @param   lval1   @ref int32 integer value.
 * @param   lval2   @ref int32 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define smac_ll(lval1,lval2)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__SMAC|MMAU__X2);                                \
  register int32 inp1 = (lval1);                                               \
  register int32 inp2 = (lval2);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit integer values and add product with
 *          value stored in the A10 register of the MMAU.
 * @details The @ref smacs_ll function multiplies two 32-bit integer values and
 *          add product with value stored in the A10 register of the MMAU.
 * @param   lval1   @ref int32 integer value.
 * @param   lval2   @ref int32 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define smacs_ll(lval1,lval2)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__SMAC|MMAU__X2|MMAU__SAT);                      \
  register int32 inp1 = (lval1);                                               \
  register int32 inp2 = (lval2);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit integer value with 32-bit integer value and add
 *          product with value stored in the A10 register of the MMAU returning
 *          a 64-bit integer A10 register value.
 * @details The @ref d_smac_dl function multiplies 64-bit integer value with
 *          32-bit integer value and add product with value stored in the A10
 *          register of the MMAU returning a 64-bit integer A10 register value.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_smac_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__SMACD|MMAU__X1);                               \
  register int64 out  = (dval);                                                \
  register int32 inp  = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit integer value with 32-bit integer value
 *          and add product with value stored in the A10 register of the MMAU
 *          returning saturated 64-bit integer A10 register value.
 * @details The @ref d_smacs_dl function multiplies 64-bit integer value with
 *          32-bit integer value and add product with value stored in the A10
 *          register of the MMAU returning saturated 64-bit integer A10 register
 *          value.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_smacs_dl(dval,lval)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__SMACD|MMAU__X1|MMAU__SAT);                     \
  register int64 out  = (dval);                                                \
  register int32 inp  = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit integer value with 32-bit integer value and add
 *          product with value stored in the A10 register of the MMAU.
 * @details The @ref smac_dl function multiplies 64-bit integer value with
 *          32-bit integer value and add product with value stored in the A10
 *          register of the MMAU.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define smac_dl(dval,lval)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__SMACD|MMAU__X1);                               \
  register int64 inp1 = (dval);                                                \
  register int32 inp2 = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit integer value with 32-bit integer value
 *          and add product with value stored in the A10 register of the MMAU.
 * @details The @ref smacs_dl function multiplies 64-bit integer value with
 *          32-bit integer value and add product with value stored in the A10
 *          register of the MMAU.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define smacs_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__SMACD|MMAU__X1|MMAU__SAT);                     \
  register int64 inp1 = (dval);                                                \
  register int32 inp2 = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit integer value by value stored in the A10 register of
 *          the MMAU and add product with 64-bit integer value returning a
 *          64-bit integer A10 register value.
 * @details The @ref d_smaca_dl function multiplies 32-bit integer value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit integer value returning a 64-bit integer A10 register value.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_smaca_dl(dval,lval)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__SMACDA|MMAU__X1);                              \
  register int64 out  = (dval);                                                \
  register int32 inp  = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit integer value by value stored in the A10
 *          register of the MMAU and add product with 64-bit integer value
 *          returning a saturated 64-bit integer A10 register value.
 * @details The @ref d_smacas_dl function multiplies 32-bit integer value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit integer value returning saturated 64-bit integer A10 register
 *          value.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_smacas_dl(dval,lval)                                                 \
({                                                                             \
  register uint32 addr = (MMAU__SMACDA|MMAU__X1|MMAU__SAT);                    \
  register int64 out  = (dval);                                                \
  register int32 inp  = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit integer value by value stored in the A10 register of
 *          the MMAU and add product with 64-bit integer value.
 * @details The @ref smaca_dl function multiplies 32-bit integer value by value
 *          stored in the A10 register of the MMAU and add product with 64-bit
 *          integer value.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define smaca_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__SMACDA|MMAU__X1);                              \
  register int64 inp1 = (dval);                                                \
  register int32 inp2 = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit integer value by value stored in the A10
 *          register of the MMAU and add product with 64-bit integer value.
 * @details The @ref smacas_dl function multiplies 32-bit integer value by value
 *          stored in the A10 register of the MMAU and add product with 64-bit
 *          integer value.
 * @param   dval    @ref int64 integer value.
 * @param   lval    @ref int32 integer value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define smacas_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__SMACDA|MMAU__X1|MMAU__SAT);                    \
  register int64 inp1 = (dval);                                                \
  register int32 inp2 = (lval);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit integer values returning a 32-bit integer
 *          quotient.
 * @details The @ref l_sdiv_ll function divides two 32-bit integer values
 *          returning a 32-bit integer quotient.
 * @param   lnum    @ref int32 integer value.
 * @param   lden    @ref int32 integer value.
 * @return  @ref int32 integer value.
 * @note    Quotient is stored in A0 register of the MMAU for next computation.
 ******************************************************************************/
#define l_sdiv_ll(lnum,lden)                                                   \
({                                                                             \
  register uint32 out = (MMAU__SDIV|MMAU__X2);                                 \
  register int32 inp1 = (lnum);                                                \
  register int32 inp2 = (lden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]\n"                                                        \
    "ldr %0,[%0,#+8]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (int32)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit integer values returning a 32-bit integer
 *          quotient.
 * @details The @ref l_sdivs_ll function divides two 32-bit integer values
 *          returning a 32-bit integer quotient.
 * @param   lnum    @ref int32 integer value.
 * @param   lden    @ref int32 integer value.
 * @return  @ref int32 integer value.
 * @note    Saturated quotient is stored in A0 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_sdivs_ll(lnum,lden)                                                  \
({                                                                             \
  register uint32 out = (MMAU__SDIV|MMAU__X2|MMAU__SAT);                       \
  register int32 inp1 = (lnum);                                                \
  register int32 inp2 = (lden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]\n"                                                        \
    "ldr %0,[%0,#+8]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (int32)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit integer values.
 * @details The @ref sdiv_ll function divides two 32-bit integer values.
 * @param   lnum    @ref int32 integer value.
 * @param   lden    @ref int32 integer value.
 * @note    Quotient is stored in A0 register of the MMAU for next computation.
 ******************************************************************************/
#define sdiv_ll(lnum,lden)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__SDIV|MMAU__X2);                                \
  register int32 inp1 = (lnum);                                                \
  register int32 inp2 = (lden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit integer values.
 * @details The @ref sdivs_ll function divides two 32-bit integer values.
 * @param   lnum    @ref int32 integer value.
 * @param   lden    @ref int32 integer value.
 * @note    Saturated quotient is stored in A0 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define sdivs_ll(lnum,lden)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__SDIV|MMAU__X2|MMAU__SAT);                      \
  register int32 inp1 = (lnum);                                                \
  register int32 inp2 = (lden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value by 32-bit integer value returning a
 *          64-bit integer quotient.
 * @details The @ref d_sdiv_dl function divides 64-bit integer value by 32-bit
 *          integer value returning a 64-bit integer quotient.
 * @param   dnum    @ref int64 integer value.
 * @param   lden    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_sdiv_dl(dnum,lden)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__SDIVD|MMAU__X1);                               \
  register int64 out  = (dnum);                                                \
  register int32 inp  = (lden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value by 32-bit integer value returning a
 *          64-bit integer quotient.
 * @details The @ref d_sdivs_dl function divides 64-bit integer value by 32-bit
 *          integer value returning a 64-bit integer quotient.
 * @param   dnum    @ref int64 integer value.
 * @param   lden    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_sdivs_dl(dnum,lden)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__SDIVD|MMAU__X1|MMAU__SAT);                     \
  register int64 out  = (dnum);                                                \
  register int32 inp  = (lden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value by 32-bit integer value.
 * @details The @ref sdiv_dl function divides 64-bit integer value by 32-bit
 *          integer value.
 * @param   dnum    @ref int64 integer value.
 * @param   lden    @ref int32 integer value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define sdiv_dl(dnum,lden)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__SDIVD|MMAU__X1);                               \
  register int64 inp1 = (dnum);                                                \
  register int32 inp2 = (lden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value by 32-bit integer value.
 * @details The @ref sdivs_dl function divides 64-bit integer value by 32-bit
 *          integer value.
 * @param   dnum    @ref int64 integer value.
 * @param   lden    @ref int32 integer value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define sdivs_dl(dnum,lden)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__SDIVD|MMAU__X1|MMAU__SAT);                     \
  register int64 inp1 = (dnum);                                                \
  register int32 inp2 = (lden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide two 64-bit integer values returning a 64-bit integer
 *          quotient.
 * @details The @ref d_sdiv_dd function divides two 64-bit integer values
 *          returning a 64-bit integer quotient.
 * @param   dnum    @ref int64 integer value.
 * @param   dden    @ref int64 integer value.
 * @return  @ref int64 integer value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_sdiv_dd(dnum,dden)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDD|MMAU__X0);                              \
  register int64 out  = (dnum);                                                \
  register int64 inp  = (dden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%Q2,%R2}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Divide two 64-bit integer values returning a 64-bit integer
 *          quotient.
 * @details The @ref d_sdivs_dd function divides two 64-bit integer values
 *          returning a 64-bit integer quotient.
 * @param   dnum    @ref int64 integer value.
 * @param   dden    @ref int64 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_sdivs_dd(dnum,dden)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDD|MMAU__X0|MMAU__SAT);                    \
  register int64 out  = (dnum);                                                \
  register int64 inp  = (dden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%Q2,%R2}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Divide two 64-bit integer values.
 * @details The @ref sdiv_dd function divides two 64-bit integer values.
 * @param   dnum    @ref int64 integer value.
 * @param   dden    @ref int64 integer value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define sdiv_dd(dnum,dden)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDD|MMAU__X0);                              \
  register int64 inp1 = (dnum);                                                \
  register int64 inp2 = (dden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%Q2,%R2}  ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide two 64-bit integer values.
 * @details The @ref sdivs_dd function divides two 64-bit integer values.
 * @param   dnum    @ref int64 integer value.
 * @param   dden    @ref int64 integer value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define sdivs_dd(dnum,dden)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDD|MMAU__X0|MMAU__SAT);                    \
  register int64 inp1 = (dnum);                                                \
  register int64 inp2 = (dden);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%Q2,%R2}  ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value stored in the A10 register of the MMAU
 *          by 32-bit integer value returning a 64-bit integer quotient.
 * @details The @ref d_sdiva_l function divides 64-bit integer value stored in
 *          the A10 register of the MMAU by 32-bit integer value returning a
 *          64-bit integer quotient.
 * @param   lden    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_sdiva_l(lden)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDA|MMAU__X3);                              \
  register union {struct {int32 lo, hi;} l; int64 d;} out;                     \
  out.l.lo=(lden);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (int64)out.d;                                                                \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value stored in the A10 register of the MMAU
 *          by 32-bit integer value returning saturated 64-bit integer quotient.
 * @details The @ref d_sdivas_l function divides 64-bit integer value stored in
 *          the A10 register of the MMAU by 32-bit integer value returning
 *          a saturated 64-bit integer quotient.
 * @param   lden    @ref int32 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_sdivas_l(lden)                                                       \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDA|MMAU__X3|MMAU__SAT);                    \
  register union {struct {int32 lo, hi;} l; int64 d;} out;                     \
  out.l.lo=(lden);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (int64)out.d;                                                                \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value stored in the A10 register of the MMAU
 *          by 32-bit integer value.
 * @details The @ref sdiva_l function divides 64-bit integer value stored in the
 *          A10 register of the MMAU by 32-bit integer value.
 * @param   lden    @ref int32 integer value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define sdiva_l(lden)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDA|MMAU__X3);                              \
  register int32 inp  = (lden);                                                \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value stored in the A10 register of the MMAU
 *          by 32-bit integer value.
 * @details The @ref sdivas_l function divides 64-bit integer value stored in
 *          the A10 register of the MMAU by 32-bit integer value.
 * @param   lden    @ref int32 integer value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define sdivas_l(lden)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDA|MMAU__X3|MMAU__SAT);                    \
  register int32 inp  = (int32)(lden);                                         \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value stored in the A10 register of the MMAU
 *          by 64-bit integer value returning a 64-bit integer quotient.
 * @details The @ref d_sdiva_d function divides 64-bit integer value stored in
 *          the A10 register of the MMAU by 64-bit integer value returning a
 *          64-bit integer quotient.
 * @param   dden    @ref int64 integer value.
 * @return  @ref int64 integer value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_sdiva_d(dden)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDDA|MMAU__X2);                             \
  register int64 out = (dden);                                                 \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"0"(addr),"1"(out)              \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value stored in the A10 register of the MMAU
 *          by 64-bit integer value returning saturated 64-bit integer quotient.
 * @details The @ref d_sdivas_d function divides 64-bit integer value stored in
 *          the A10 register of the MMAU by 64-bit integer value returning
 *          a saturated 64-bit integer quotient.
 * @param   dden    @ref int64 integer value.
 * @return  @ref int64 integer value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_sdivas_d(dden)                                                       \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDDA|MMAU__X2|MMAU__SAT);                   \
  register int64 out = (dden);                                                 \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"0"(addr),"1"(out)              \
  );                                                                           \
  (int64)out;                                                                  \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value stored in the A10 register of the MMAU
 *          by 64-bit integer value.
 * @details The @ref sdiva_d function divides 64-bit integer value stored in the
 *          A10 register of the MMAU by 64-bit integer value.
 * @param   dden    @ref int64 integer value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define sdiva_d(dden)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDDA|MMAU__X2);                             \
  register int64 inp  = (dden);                                                \
  __asm volatile ("stm %0!,{%Q1,%R1}":"=l"(addr):"l"(inp),"0"(addr));          \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit integer value stored in the A10 register of the MMAU
 *          by 64-bit integer value.
 * @details The @ref sdivas_d function divides 64-bit integer value stored in
 *          the A10 register of the MMAU by 64-bit integer value.
 * @param   dden    @ref int64 integer value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define sdivas_d(dden)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__SDIVDDA|MMAU__X2|MMAU__SAT);                   \
  register int64 inp  = (dden);                                                \
  __asm volatile ("stm %0!,{%Q1,%R1}":"=l"(addr):"l"(inp),"0"(addr));          \
})

/*! @} End of int_instructions                                               */

/******************************************************************************
 * MMAU instruction set.
 *
 *//*! @addtogroup frac_instructions
 * @{
 ******************************************************************************/

/***************************************************************************//*!
 * @brief   Load A10 accumulator register of the MMAU by 64-bit fractional
 *          value.
 * @details The @ref lda_d function loads A10 accumulator register of the MMAU
 *          by 64-bit fractional value.
 * @param   dval    @ref frac64 fractional value.
 ******************************************************************************/
#define lda_d(dval)                                                            \
({                                                                             \
  register uint32 addr = (MMAU__REGRW|MMAU__A10);                              \
  register frac64 inp  = (dval);                                               \
  __asm volatile ("stm %0!,{%Q1,%R1}":"=l"(addr):"l"(inp),"0"(addr));          \
})

/***************************************************************************//*!
 * @brief   Load upper A1 accumulator register of the MMAU by 32-bit fractional
 *          value and clear lower A0 accumulator register.
 * @details The @ref lda_l function loads upper A1 accumulator register of the
 *          MMAU by 32-bit fractional value and clears lower A0 accumulator
 *          register.
 * @param   lval    @ref frac32 fractional value.
 ******************************************************************************/
#define lda_l(lval)                                                            \
({                                                                             \
  register uint32 out = (MMAU__REGRW|MMAU__A10);                               \
  register frac32 inp = (lval);                                                \
  register frac32 tmp = 0l;                                                    \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0, #+0]\n"                                                       \
    "str %2,[%0, #+4]\n"::"l"(out),"l"(tmp),"l"(lval)                          \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Read 32-bit fractional value from the A1 accumulator register of the
 *          MMAU.
 * @details The @ref l_rda function reads 32-bit fractional value from the A1
 *          accumulator register of the MMAU.
 * @return  @ref frac32 fractional value.
 ******************************************************************************/
#define l_rda()                                                                \
({                                                                             \
  register uint32 out = (MMAU__REGRW|MMAU__A1);                                \
  __asm volatile ("ldr %0,[%0,#+0]":"=l"(out):"0"(out));                       \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Read 64-bit fractional value from the A10 accumulator register of
 *          the MMAU.
 * @details The @ref d_rda function reads 64-bit fractional value from the A10
 *          accumulator register of the MMAU.
 * @return  @ref frac64 fractional value.
 ******************************************************************************/
#define d_rda(void)                                                            \
({                                                                             \
  register union {struct {uint32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo=(MMAU__REGRW|MMAU__A10);                                            \
  __asm volatile ("ldm %Q0,{%Q0,%R0}":"=l"(out.d):"0"(out.d));                 \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values returning a 32-bit fractional
 *          product.
 * @details The @ref l_mul_ll function multiplies two 32-bit fractional values
 *          returning a 32-bit fractional product.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define l_mul_ll(lval1,lval2)                                                  \
({                                                                             \
  register uint32 out = (MMAU__QMUL|MMAU__X2);                                 \
  register frac32 inp1 = (lval1);                                              \
  register frac32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0, #+0]\n"                                                       \
    "str %2,[%0, #+4]\n"                                                       \
    "ldr %0,[%0,#+12]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit fractional values returning saturated
 *          32-bit fractional product.
 * @details The @ref l_muls_ll function multiplies two 32-bit fractional values
 *          returning saturated 32-bit fractional product.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_muls_ll(lval1,lval2)                                                 \
({                                                                             \
  register uint32 out = (MMAU__QMUL|MMAU__X2|MMAU__SAT);                       \
  register frac32 inp1 = (lval1);                                              \
  register frac32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0, #+0]\n"                                                       \
    "str %2,[%0, #+4]\n"                                                       \
    "ldr %0,[%0,#+12]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values returning a 64-bit fractional
 *          product.
 * @details The @ref d_mul_ll function multiplies two 32-bit fractional values
 *          returning a 64-bit fractional product.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_mul_ll(lval1,lval2)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__QMUL|MMAU__X2);                                \
  register union {struct {frac32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit fractional values returning saturated
 *          64-bit fractional product.
 * @details The @ref d_muls_ll function multiplies two 32-bit fractional values
 *          returning saturated 64-bit fractional product.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_muls_ll(lval1,lval2)                                                 \
({                                                                             \
  register uint32 addr = (MMAU__QMUL|MMAU__X2|MMAU__SAT);                      \
  register union {struct {frac32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values.
 * @details The @ref mul_ll function multiplies two 32-bit fractional values.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define mul_ll(lval1,lval2)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__QMUL|MMAU__X2);                                \
  register frac32 inp1 = (lval1);                                              \
  register frac32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit fractional values.
 * @details The @ref muls_ll function multiplies two 32-bit fractional values.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define muls_ll(lval1,lval2)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__QMUL|MMAU__X2|MMAU__SAT);                      \
  register frac32 inp1 = (lval1);                                              \
  register frac32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value
 *          returning a 32-bit fractional product.
 * @details The @ref l_mul_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value returning a 32-bit fractional product.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define l_mul_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 out  = (MMAU__QMULD|MMAU__X1);                               \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldr %0 ,[%0,#+4] ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit fractional value with 32-bit fractional
 *          value returning saturated 32-bit fractional product.
 * @details The @ref l_muls_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value returning saturated 32-bit fractional
 *          product.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_muls_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 out  = (MMAU__QMULD|MMAU__X1|MMAU__SAT);                     \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldr %0 ,[%0,#+4] ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value
 *          returning a 64-bit fractional product.
 * @details The @ref d_mul_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value returning a 64-bit fractional product.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_mul_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__QMULD|MMAU__X1);                               \
  register frac64 out  = (dval);                                               \
  register frac32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (frac64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit fractional value with 32-bit fractional
 *          value returning saturated 64-bit fractional product.
 * @details The @ref d_muls_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value returning saturated 64-bit fractional
 *          product.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_muls_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__QMULD|MMAU__X1|MMAU__SAT);                     \
  register frac64 out  = (dval);                                               \
  register frac32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (frac64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value.
 * @details The @ref mul_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define mul_dl(dval,lval)                                                      \
({                                                                             \
  register uint32 addr = (MMAU__QMULD|MMAU__X1);                               \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit fractional value with 32-bit fractional
 *          value.
 * @details The @ref muls_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define muls_dl(dval,lval)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__QMULD|MMAU__X1|MMAU__SAT);                     \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit fractional value with 64-bit fractional value stored
 *          in the A10 register of the MMAU returning a 32-bit fractional
 *          product.
 * @details The @ref l_mula_l function multiplies 32-bit fractional value with
 *          64-bit fractional value stored in the A10 register of the MMAU
 *          returning a 32-bit fractional product.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define l_mula_l(lval)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__QMULDA|MMAU__X3);                              \
  register frac32 tmp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %0,[%1,#+0]\n"                                                        \
    "ldr %0,[%1,#+8]  ":"=l"(tmp):"l"(addr),"0"(tmp)                           \
  );                                                                           \
  (frac32)tmp;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit fractional value with 64-bit fractional
 *          value stored in the A10 register of the MMAU returning saturated
 *          32-bit fractional product.
 * @details The @ref l_mulas_l function multiplies 32-bit fractional value with
 *          64-bit fractional value stored in the A10 register of the MMAU
 *          returning saturated 32-bit fractional product.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_mulas_l(lval)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__QMULDA|MMAU__X3|MMAU__SAT);                    \
  register frac32 tmp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %0,[%1,#+0]\n"                                                        \
    "ldr %0,[%1,#+8]  ":"=l"(tmp):"l"(addr),"0"(tmp)                           \
  );                                                                           \
  (frac32)tmp;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit fractional value with 64-bit fractional value stored
 *          in the A10 register of the MMAU returning a 64-bit fractional
 *          product.
 * @details The @ref d_mula_l function multiplies 32-bit fractional value with
 *          64-bit fractional value stored in the A10 register of the MMAU
 *          returning a 64-bit fractional product.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_mula_l(lval)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__QMULDA|MMAU__X3);                              \
  register union {struct {frac32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo=(lval);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit fractional value with 64-bit fractional
 *          value stored in the A10 register of the MMAU returning saturated
 *          64-bit fractional product.
 * @details The @ref d_mulas_l function multiplies 32-bit fractional value with
 *          64-bit fractional value stored in the A10 register of the MMAU
 *          returning saturated 64-bit fractional product.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_mulas_l(lval)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__QMULDA|MMAU__X3|MMAU__SAT);                    \
  register union {struct {frac32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo=(lval);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit fractional value with 64-bit fractional value stored
 *          in the A10 register of the MMAU.
 * @details The @ref mula_l function multiplies 32-bit fractional value with
 *          64-bit fractional value stored in the A10 register of the MMAU.
 * @param   lval    @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define mula_l(lval)                                                           \
({                                                                             \
  register uint32 addr = (MMAU__QMULDA|MMAU__X3);                              \
  register frac32 inp  = (lval);                                               \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit fractional value with 64-bit fractional
 *          value stored in the A10 register of the MMAU.
 * @details The @ref mulas_l function multiplies 32-bit fractional value with
 *          64-bit fractional value stored in the A10 register of the MMAU.
 * @param   lval    @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define mulas_l(lval)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__QMULDA|MMAU__X3|MMAU__SAT);                    \
  register frac32 inp  = (frac32)(lval);                                       \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values and add product with value
 *          stored in the A10 register of the MMAU returning a 32-bit fractional
 *          A10 register value.
 * @details The @ref l_mac_ll function multiplies two 32-bit fractional values
 *          and add product with value stored in the A10 register of the MMAU
 *          returning a 32-bit fractional A1 register value.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define l_mac_ll(lval1,lval2)                                                  \
({                                                                             \
  register uint32 out = (MMAU__QMAC|MMAU__X2);                                 \
  register frac32 inp1 = (lval1);                                              \
  register frac32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0, #+0]\n"                                                       \
    "str %2,[%0, #+4]\n"                                                       \
    "ldr %0,[%0,#+12]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit fractional values and add product
 *          with value stored in the A10 register of the MMAU returning a 32-bit
 *          fractional A10 register value.
 * @details The @ref l_macs_ll function multiplies two 32-bit fractional values
 *          and add product with value stored in the A10 register of the MMAU
 *          returning saturated 32-bit fractional A1 register value.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_macs_ll(lval1,lval2)                                                 \
({                                                                             \
  register uint32 out = (MMAU__QMAC|MMAU__X2|MMAU__SAT);                       \
  register frac32 inp1 = (lval1);                                              \
  register frac32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0, #+0]\n"                                                       \
    "str %2,[%0, #+4]\n"                                                       \
    "ldr %0,[%0,#+12]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values and add product with value
 *          stored in the A10 register of the MMAU returning a 64-bit fractional
 *          A10 register value.
 * @details The @ref d_mac_ll function multiplies two 32-bit fractional values
 *          and add product with value stored in the A10 register of the MMAU
 *          returning a 64-bit fractional A10 register value.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_mac_ll(lval1,lval2)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__QMAC|MMAU__X2);                                \
  register union {struct {frac32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit fractional values and add product
 *          with value stored in the A10 register of the MMAU returning a 64-bit
 *          fractional A10 register value.
 * @details The @ref d_macs_ll function multiplies two 32-bit fractional values
 *          and add product with value stored in the A10 register of the MMAU
 *          returning saturated 64-bit fractional A10 register value.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_macs_ll(lval1,lval2)                                                 \
({                                                                             \
  register uint32 addr = (MMAU__QMAC|MMAU__X2|MMAU__SAT);                      \
  register union {struct {frac32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo= (lval1);                                                           \
  out.l.hi= (lval2);                                                           \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)          \
  );                                                                           \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values and add product with value
 *          stored in the A10 register of the MMAU.
 * @details The @ref mac_ll function multiplies two 32-bit fractional values and
 *          add product with value stored in the A10 register of the MMAU.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define mac_ll(lval1,lval2)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__QMAC|MMAU__X2);                                \
  register frac32 inp1 = (lval1);                                              \
  register frac32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply two 32-bit fractional values and add product
 *          with value stored in the A10 register of the MMAU.
 * @details The @ref macs_ll function multiplies two 32-bit fractional values
 *          and add product with value stored in the A10 register of the MMAU.
 * @param   lval1   @ref frac32 fractional value.
 * @param   lval2   @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define macs_ll(lval1,lval2)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__QMAC|MMAU__X2|MMAU__SAT);                      \
  register frac32 inp1 = (lval1);                                              \
  register frac32 inp2 = (lval2);                                              \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value and
 *          add product with value stored in the A10 register of the MMAU
 *          returning a 32-bit fractional A10 register value.
 * @details The @ref l_mac_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value and add product with value stored in the A10
 *          register of the MMAU returning a 32-bit fractional A1 register
 *          value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define l_mac_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 out  = (MMAU__QMACD|MMAU__X1);                               \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldr %0 ,[%0,#+4] ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit fractional value with 32-bit fractional
 *          value and add product with value stored in the A10 register of the
 *          MMAU returning saturated 32-bit fractional A10 register value.
 * @details The @ref l_macs_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value and add product with value stored in the A10
 *          register of the MMAU returning saturated 32-bit fractional A1
 *          register value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_macs_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 out  = (MMAU__QMACD|MMAU__X1|MMAU__SAT);                     \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldr %0 ,[%0,#+4] ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value and
 *          add product with value stored in the A10 register of the MMAU
 *          returning a 64-bit fractional A10 register value.
 * @details The @ref d_mac_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value and add product with value stored in the A10
 *          register of the MMAU returning a 64-bit fractional A10 register
 *          value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_mac_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__QMACD|MMAU__X1);                               \
  register frac64 out  = (dval);                                               \
  register frac32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (frac64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit fractional value with 32-bit fractional
 *          value and add product with value stored in the A10 register of the
 *          MMAU returning saturated 64-bit fractional A10 register value.
 * @details The @ref d_macs_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value and add product with value stored in the A10
 *          register of the MMAU returning saturated 64-bit fractional A10
 *          register value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_macs_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__QMACD|MMAU__X1|MMAU__SAT);                     \
  register frac64 out  = (dval);                                               \
  register frac32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (frac64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value and
 *          add product with value stored in the A10 register of the MMAU.
 * @details The @ref mac_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value and add product with value stored in the A10
 *          register of the MMAU.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define mac_dl(dval,lval)                                                      \
({                                                                             \
  register uint32 addr = (MMAU__QMACD|MMAU__X1);                               \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 64-bit fractional value with 32-bit fractional
 *          value and add product with value stored in the A10 register of the
 *          MMAU.
 * @details The @ref macs_dl function multiplies 64-bit fractional value with
 *          32-bit fractional value and add product with value stored in the A10
 *          register of the MMAU.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define macs_dl(dval,lval)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__QMACD|MMAU__X1|MMAU__SAT);                     \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit fractional value by value stored in the A10 register
 *          of the MMAU and add product with 64-bit fractional value returning a
 *          32-bit fractional A10 register value.
 * @details The @ref l_maca_dl function multiplies 32-bit fractional value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit fractional value returning a 32-bit fractional A1 register
 *          value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define l_maca_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 out  = (MMAU__QMACDA|MMAU__X1);                              \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldr %0 ,[%0,#+4] ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit fractional value by value stored in the
 *          A10 register of the MMAU and add product with 64-bit fractional
 *          value returning a saturated 32-bit fractional A10 register value.
 * @details The @ref l_macas_dl function multiplies 32-bit fractional value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit fractional value returning saturated 32-bit fractional A1
 *          register value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_macas_dl(dval,lval)                                                  \
({                                                                             \
  register uint32 out  = (MMAU__QMACDA|MMAU__X1|MMAU__SAT);                    \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldr %0 ,[%0,#+4] ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                 \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit fractional value by value stored in the A10 register
 *          of the MMAU and add product with 64-bit fractional value returning a
 *          64-bit fractional A10 register value.
 * @details The @ref d_maca_dl function multiplies 32-bit fractional value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit fractional value returning a 64-bit fractional A10 register
 *          value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_maca_dl(dval,lval)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__QMACDA|MMAU__X1);                              \
  register frac64 out  = (dval);                                               \
  register frac32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (frac64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit fractional value by value stored in the
 *          A10 register of the MMAU and add product with 64-bit fractional
 *          value returning a saturated 64-bit fractional A10 register value.
 * @details The @ref d_macas_dl function multiplies 32-bit fractional value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit fractional value returning saturated 64-bit fractional A10
 *          register value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_macas_dl(dval,lval)                                                  \
({                                                                             \
  register uint32 addr = (MMAU__QMACDA|MMAU__X1|MMAU__SAT);                    \
  register frac64 out  = (dval);                                               \
  register frac32 inp  = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (frac64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Multiply 32-bit fractional value by value stored in the A10 register
 *          of the MMAU and add product with 64-bit fractional value.
 * @details The @ref maca_dl function multiplies 32-bit fractional value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit fractional value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @note    Product is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define maca_dl(dval,lval)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__QMACDA|MMAU__X1);                              \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Saturating multiply 32-bit fractional value by value stored in the
 *          A10 register of the MMAU and add product with 64-bit fractional
 *          value.
 * @details The @ref macas_dl function multiplies 32-bit fractional value by
 *          value stored in the A10 register of the MMAU and add product with
 *          64-bit fractional value.
 * @param   dval    @ref frac64 fractional value.
 * @param   lval    @ref frac32 fractional value.
 * @note    Saturated product is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define macas_dl(dval,lval)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__QMACDA|MMAU__X1|MMAU__SAT);                    \
  register frac64 inp1 = (dval);                                               \
  register frac32 inp2 = (lval);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit fractional values returning a 32-bit fractional
 *          quotient.
 * @details The @ref l_div_ll function divides two 32-bit fractional values
 *          returning a 32-bit fractional quotient.
 * @param   lnum    @ref frac32 fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Quotient is stored in A1 register of the MMAU for next computation.
 ******************************************************************************/
#define l_div_ll(lnum,lden)                                                    \
({                                                                             \
  register uint32 out = (MMAU__QDIV|MMAU__X2);                                 \
  register frac32 inp1 = (lnum);                                               \
  register frac32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0, #+0]\n"                                                       \
    "str %2,[%0, #+4]\n"                                                       \
    "ldr %0,[%0,#+12]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit fractional values returning a 32-bit fractional
 *          quotient.
 * @details The @ref l_divs_ll function divides two 32-bit fractional values
 *          returning a 32-bit fractional quotient.
 * @param   lnum    @ref frac32 fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Saturated quotient is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_divs_ll(lnum,lden)                                                   \
({                                                                             \
  register uint32 out = (MMAU__QDIV|MMAU__X2|MMAU__SAT);                       \
  register frac32 inp1 = (lnum);                                               \
  register frac32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0, #+0]\n"                                                       \
    "str %2,[%0, #+4]\n"                                                       \
    "ldr %0,[%0,#+12]  ":"=l"(out):"l"(inp1),"l"(inp2),"0"(out)                \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit fractional values.
 * @details The @ref div_ll function divides two 32-bit fractional values.
 * @param   lnum    @ref frac32 fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @note    Quotient is stored in A1 register of the MMAU for next computation.
 ******************************************************************************/
#define div_ll(lnum,lden)                                                      \
({                                                                             \
  register uint32 addr = (MMAU__QDIV|MMAU__X2);                                \
  register frac32 inp1 = (lnum);                                               \
  register frac32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide two 32-bit fractional values.
 * @details The @ref divs_ll function divides two 32-bit fractional values.
 * @param   lnum    @ref frac32 fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @note    Saturated quotient is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define divs_ll(lnum,lden)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__QDIV|MMAU__X2|MMAU__SAT);                      \
  register frac32 inp1 = (lnum);                                               \
  register frac32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0,#+0]\n"                                                        \
    "str %2,[%0,#+4]  "::"l"(addr),"l"(inp1),"l"(inp2)                         \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value by 32-bit fractional value returning
 *          a 64-bit fractional quotient.
 * @details The @ref d_div_dl function divides 64-bit fractional value by 32-bit
 *          fractional value returning a 64-bit fractional quotient.
 * @param   dnum    @ref frac64 fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_div_dl(dnum,lden)                                                    \
({                                                                             \
  register uint32 addr = (MMAU__QDIVD|MMAU__X1);                               \
  register frac64 out  = (dnum);                                               \
  register frac32 inp  = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (frac64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value by 32-bit fractional value returning
 *          a 64-bit fractional quotient.
 * @details The @ref d_divs_dl function divides 64-bit fractional value by
 *          32-bit fractional value returning a 64-bit fractional quotient.
 * @param   dnum    @ref frac64 fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_divs_dl(dnum,lden)                                                   \
({                                                                             \
  register uint32 addr = (MMAU__QDIVD|MMAU__X1|MMAU__SAT);                     \
  register frac64 out  = (dnum);                                               \
  register frac32 inp  = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "stm %0!,{%2}     \n"                                                      \
    "ldm %0!,{%Q1,%R1}  ":"=l"(addr),"=l"(out):"l"(inp),"0"(addr),"1"(out)     \
  );                                                                           \
  (frac64)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value by 32-bit fractional value.
 * @details The @ref div_dl function divides 64-bit fractional value by 32-bit
 *          fractional value.
 * @param   dnum    @ref frac64 fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define div_dl(dnum,lden)                                                      \
({                                                                             \
  register uint32 addr = (MMAU__QDIVD|MMAU__X1);                               \
  register frac64 inp1 = (dnum);                                               \
  register frac32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value by 32-bit fractional value.
 * @details The @ref divs_dl function divides 64-bit fractional value by 32-bit
 *          fractional value.
 * @param   dnum    @ref frac64 fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define divs_dl(dnum,lden)                                                     \
({                                                                             \
  register uint32 addr = (MMAU__QDIVD|MMAU__X1|MMAU__SAT);                     \
  register frac64 inp1 = (dnum);                                               \
  register frac32 inp2 = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "str %2 ,[%0,#+0]   ":"=l"(addr):"l"(inp1),"l"(inp2),"0"(addr)             \
  );                                                                           \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value stored in the A10 register of the
 *          MMAU by 32-bit fractional value returning a 32-bit fractional
 *          quotient.
 * @details The @ref l_diva_l function divides 64-bit fractional value stored in
 *          the A10 register of the MMAU by 32-bit fractional value returning a
 *          32-bit fractional quotient.
 * @param   lden    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Quotient is stored in A1 register of the MMAU for next computation.
 ******************************************************************************/
#define l_diva_l(lden)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__QDIVDA|MMAU__X3);                              \
  register frac32 tmp  = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %0,[%1,#+0]\n"                                                        \
    "ldr %0,[%1,#+8]  ":"=l"(tmp):"l"(addr),"0"(tmp)                           \
  );                                                                           \
  (frac32)tmp;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value stored in the A10 register of the
 *          MMAU by 32-bit fractional value returning saturated 32-bit
 *          fractional quotient.
 * @details The @ref l_divas_l function divides 64-bit fractional value stored
 *          in the A10 register of the MMAU by 32-bit fractional value returning
 *          a saturated 32-bit fractional quotient.
 * @param   lden    @ref frac32 fractional value.
 * @return  @ref frac32 fractional value.
 * @note    Saturated quotient is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_divas_l(lden)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__QDIVDA|MMAU__X3|MMAU__SAT);                    \
  register frac32 tmp  = (lden);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %0,[%1,#+0]\n"                                                        \
    "ldr %0,[%1,#+8]  ":"=l"(tmp):"l"(addr),"0"(tmp)                           \
  );                                                                           \
  (frac32)tmp;                                                                 \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value stored in the A10 register of the
 *          MMAU by 32-bit fractional value returning a 64-bit fractional
 *          quotient.
 * @details The @ref d_diva_l function divides 64-bit fractional value stored in
 *          the A10 register of the MMAU by 32-bit fractional value returning a
 *          64-bit fractional quotient.
 * @param   lden    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define d_diva_l(lden)                                                         \
({                                                                             \
  register uint32 addr = (MMAU__QDIVDA|MMAU__X3);                              \
  register union {struct {frac32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo=(lden);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value stored in the A10 register of the
 *          MMAU by 32-bit fractional value returning saturated 64-bit
 *          fractional quotient.
 * @details The @ref d_divas_l function divides 64-bit fractional value stored
 *          in the A10 register of the MMAU by 32-bit fractional value returning
 *          a saturated 64-bit fractional quotient.
 * @param   lden    @ref frac32 fractional value.
 * @return  @ref frac64 fractional value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define d_divas_l(lden)                                                        \
({                                                                             \
  register uint32 addr = (MMAU__QDIVDA|MMAU__X3|MMAU__SAT);                    \
  register union {struct {frac32 lo, hi;} l; frac64 d;} out;                   \
  out.l.lo=(lden);                                                             \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1}  \n"                                                        \
    "ldm %0!,{%Q1,%R1}":"=l"(addr),"=l"(out.d):"0"(addr),"1"(out.d)            \
  );                                                                           \
  (frac64)out.d;                                                               \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value stored in the A10 register of the
 *          MMAU by 32-bit fractional value.
 * @details The @ref diva_l function divides 64-bit fractional value stored in
 *          the A10 register of the MMAU by 32-bit fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @note    Quotient is stored in A10 register of the MMAU for next computation.
 ******************************************************************************/
#define diva_l(lden)                                                           \
({                                                                             \
  register uint32 addr = (MMAU__QDIVDA|MMAU__X3);                              \
  register frac32 inp  = (lden);                                               \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Divide 64-bit fractional value stored in the A10 register of the
 *          MMAU by 32-bit fractional value.
 * @details The @ref divas_l function divides 64-bit fractional value stored in
 *          the A10 register of the MMAU by 32-bit fractional value.
 * @param   lden    @ref frac32 fractional value.
 * @note    Saturated quotient is stored in A10 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define divas_l(lden)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__QDIVDA|MMAU__X3|MMAU__SAT);                    \
  register frac32 inp  = (frac32)(lden);                                       \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Compute and return a 16-bit fractional square root of the 32-bit
 *          fractional radicand.
 * @details The @ref s_sqr_l function computes and returns a 16-bit fractional
 *          square root of the 32-bit fractional radicand.
 * @param   lrad    @ref frac32 fractional radicand.
 * @return  @ref frac16 fractional square root.
 * @note    Square root is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define s_sqr_l(lrad)                                                          \
({                                                                             \
  register uint32 addr = (MMAU__QSQR|MMAU__X3);                                \
  register frac32 tmp  = (lrad);                                               \
  __asm volatile                                                               \
  (                                                                            \
    "str %0,[%1,#+0]\n"                                                        \
    "ldr %0,[%1,#+8]\n"                                                        \
    "asr %0,%0,#+16   ":"=l"(tmp):"l"(addr),"0"(tmp):"cc"                      \
  );                                                                           \
  (frac16)tmp;                                                                 \
})

/***************************************************************************//*!
 * @brief   Compute a 16-bit fractional square root of the 32-bit fractional
 *          radicand.
 * @details The @ref sqr_l function computes a 16-bit fractional square root of
 *          the 32-bit fractional radicand.
 * @param   lrad    @ref frac32 fractional radicand.
 * @note    Square root is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define sqr_l(lrad)                                                            \
({                                                                             \
  register uint32 addr = (MMAU__QSQR|MMAU__X3);                                \
  register frac32 inp  = (lrad);                                               \
  __asm volatile ("str %0,[%1,#+0]"::"l"(inp),"l"(addr));                      \
})

/***************************************************************************//*!
 * @brief   Compute and return a 32-bit fractional square root of the 32-bit
 *          fractional radicand.
 * @details The @ref l_sqr_l function computes and returns a 32-bit fractional
 *          square root of the 32-bit fractional radicand.
 * @param   lrad    @ref frac32 fractional radicand.
 * @return  @ref frac32 fractional square root.
 * @note    Square root is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_sqr_l(lrad)                                                          \
({                                                                             \
  register uint32 out = (MMAU__QSQRD|MMAU__X2);                                \
  register frac32 inp = (lrad);                                                \
  register frac32 tmp = 0l;                                                    \
  __asm volatile                                                               \
  (                                                                            \
    "str %1,[%0, #+0]\n"                                                       \
    "str %2,[%0, #+4]\n"                                                       \
    "ldr %0,[%0,#+12]  ":"=l"(out):"l"(tmp),"l"(lrad),"0"(out)                 \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Compute and return a 32-bit fractional square root of the 64-bit
 *          fractional radicand.
 * @details The @ref l_sqr_d function computes and returns a 32-bit fractional
 *          square root of the 64-bit fractional radicand.
 * @param   drad    @ref frac64 fractional radicand.
 * @return  @ref frac32 fractional square root.
 * @note    Square root is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_sqr_d(drad)                                                          \
({                                                                             \
  register uint32 out = (MMAU__QSQRD|MMAU__X2);                                \
  register frac64 inp = (drad);                                                \
  __asm volatile                                                               \
  (                                                                            \
    "stm %0!,{%Q1,%R1}\n"                                                      \
    "ldr %0 ,[%0,#+4]   ":"=l"(out):"l"(inp),"0"(out)                          \
  );                                                                           \
  (frac32)out;                                                                 \
})

/***************************************************************************//*!
 * @brief   Compute a 32-bit fractional square root of the 64-bit fractional
 *          radicand.
 * @details The @ref sqr_d function computes a 32-bit fractional square root of
 *          the 64-bit fractional radicand.
 * @param   drad    @ref frac64 fractional radicand.
 * @note    Square root is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define sqr_d(drad)                                                            \
({                                                                             \
  register uint32 addr = (MMAU__QSQRD|MMAU__X2);                               \
  register frac64 inp  = (drad);                                               \
  __asm volatile ("stm %0!,{%Q1,%R1}":"=l"(addr):"l"(inp),"0"(addr));          \
})

/***************************************************************************//*!
 * @brief   Compute and return a 32-bit fractional square root of the radicand
 *          stored in the A10 register of the MMAU.
 * @details The @ref l_sqra function computes and returns a 32-bit fractional
 *          square root of the radicand stored in the A10 register of the MMAU.
 * @return  @ref frac32 fractional square root.
 * @note    Square root is stored in A1 register of the MMAU for next
 *          computation.
 ******************************************************************************/
#define l_sqra()                                                               \
({                                                                             \
  register uint32 out = (MMAU__REGRW|MMAU__A1);                                \
  __asm volatile ("ldr %0,[%0,#+0]":"=l"(out):"0"(out));/* dummy read */       \
  out = (MMAU__QSQRDA|MMAU__A1);                                               \
  __asm volatile ("ldr %0,[%0,#+0]":"=l"(out):"0"(out));                       \
  (frac32)out;                                                                 \
})

/*! @} End of frac_instructions                                               */
#endif /* __MMAU_H */
