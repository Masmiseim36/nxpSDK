/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

        SECTION ISTACK:DATA:NOROOT(3)
        SECTION CSTACK:DATA:NOROOT(3)
        SECTION RSTACK:DATA:NOROOT(3)

        PUBLIC  LPM_Suspend
        PUBLIC  LPM_ResumeIdle
        PUBLIC  LPM_ResumeDsm
        PUBLIC  LPM_CoreStateSave
        PUBLIC  LPM_CoreStateRestore
        SECTION .text:CODE:REORDER:NOROOT(2)

        EXTERN  LPM_GicStateSave
        EXTERN  LPM_GicStateRestore
        EXTERN  LPM_SystemResumeIdle
        EXTERN  LPM_SystemResumeDsm

        ARM

LPM_Suspend
        PUSH    {R4-R11, LR}                 ; Save return context

        MOV     R5, R0                       ; R5 stores system suspend function

        BL      LPM_GicStateSave

        SUB     SP, SP, #0x28                ; Reserve enough space for Core context
        MOV     R0, SP
        BL      LPM_CoreStateSave

        MRS     R0, CPSR
        PUSH    {R0}                         ; Save running mode

        MOV     R6, SP                       ; Save DDR SP
        LDR     SP, =SFE(RSTACK)             ; Set stack to OCRAM
        MOV     R0, R6                       ; Set 1st param to DDR SP
        BLX     R5                           ; Won't return unless wakeup before suspend
        MOV     SP, R6                       ; Restore DDR SP

        POP     {R0}                         ; Mode doesn't change, so discard it

        MOV     R0, SP
        BL      LPM_CoreStateRestore
        ADD     SP, SP, #0x28

        BL      LPM_GicStateRestore

        POP     {R4-R11, LR}
        MOV     PC, LR

LPM_ResumeIdle
        CPS     #0x12                        ; Switch to IRQ mode
        LDR     SP, =SFE(ISTACK)             ; Initialize IRQ handler stack
        CPS     #0x13                        ; Switch to SVC mode
        LDR     SP, =SFE(CSTACK)             ; Initialize SVC handler stack
        CPS     #0x1F                        ; Switch back to SYSTEM mode
        LDR     SP, =SFE(RSTACK)             ; Initialize resume stack

        BL      LPM_SystemResumeIdle         ; Resume system power, clock etc, return suspend SP

        ; First restore CPSR
        LDR     R1, [R0]
        MSR     CPSR_C, R1                   ; Restore running mode: SVC(bare metal), SYSTEM(FreeRTOS)
        ; Now switch back to SP in DDR
        ADD     SP, R0, #4

        MOV     R0, SP
        BL      LPM_CoreStateRestore
        ADD     SP, SP, #0x28

        BL      LPM_GicStateRestore

        POP     {R4-R11, LR}
        MOV     PC, LR

LPM_ResumeDsm
        CPS     #0x12                        ; Switch to IRQ mode
        LDR     SP, =SFE(ISTACK)             ; Initialize IRQ handler stack
        CPS     #0x13                        ; Switch to SVC mode
        LDR     SP, =SFE(CSTACK)             ; Initialize SVC handler stack
        CPS     #0x1F                        ; Switch back to SYSTEM mode
        LDR     SP, =SFE(RSTACK)             ; Initialize resume stack

        BL      LPM_SystemResumeDsm          ; Resume from deep sleep mode, return suspend SP

        ; First restore CPSR
        LDR     R1, [R0]
        MSR     CPSR_C, R1                   ; Restore running mode: SVC(bare metal), SYSTEM(FreeRTOS)
        ; Now switch back to SP in DDR
        ADD     SP, R0, #4

        MOV     R0, SP
        BL      LPM_CoreStateRestore
        ADD     SP, SP, #0x28

        BL      LPM_GicStateRestore

        POP     {R4-R11, LR}
        MOV     PC, LR

LPM_CoreStateSave
        PUSH    {R4 - R10}
        ; Save System Control Registers
        MRC     P15, 0, R1, C1, C0, 0        ; SCTLR
        MRC     P15, 0, R2, C1, C0, 1        ; ACTLR
        MRC     P15, 0, R3, C1, C0, 2        ; CPACR

        ; Save MMU Configuration
        MRC     P15, 0, R4, C2, C0, 0        ; TTBR0
        MRC     P15, 0, R5, C2, C0, 1        ; TTBR1
        MRC     P15, 0, R6, C2, C0, 2        ; TTBCR
        MRC     P15, 0, R7, C3, C0, 0        ; DACR

        ; Save Others
        MRC     P15, 0, R8, C12, C0, 0       ; VBAR
        MRC     P15, 0, R9, C13, C0, 1       ; CONTEXTIDR

        ; As Idle Task doesn't touch FPU, we don't need to save
        ; FPU registers except FPEXC 
        VMRS    R10, FPEXC                   ; FPEXC

        STMIA   R0!, {R1 - R10}

        POP     {R4 - R10}
        MOV     PC, LR

LPM_CoreStateRestore
        PUSH    {R4 - R10}
        LDMIA   R0!, {R1 - R10}

        MOV     R0, #0
        ; Invalidate ICache
        MCR     P15, 0, R0, C7, C5, 0
        MCR     P15, 0, R0, C7, C5, 6
        ; Invalidate TLB
        MCR     P15, 0, R0, C8, C7, 0

        ; Restore Others
        MCR     P15, 0, R8, C12, C0, 0       ; VBAR
        MCR     P15, 0, R9, C13, C0, 1       ; CONTEXTIDR
 
        ; Restore MMU
        MCR     P15, 0, R7, C3, C0, 0        ; DACR
        MCR     P15, 0, R4, C2, C0, 0        ; TTBR0
        MCR     P15, 0, R5, C2, C0, 1        ; TTBR1
        MCR     P15, 0, R6, C2, C0, 2        ; TTBCR

        ; Restore System Control Register
        MCR     P15, 0, R3, C1, C0, 2        ; CPACR
        MCR     P15, 0, R2, C1, C0, 1        ; ACTLR
        MCR     P15, 0, R1, C1, C0, 0        ; SCTLR

        VMSR    FPEXC, R10                   ; FPEXC

        ISB
        DSB

        POP     {R4-R10}
        MOV     PC, LR

        END
