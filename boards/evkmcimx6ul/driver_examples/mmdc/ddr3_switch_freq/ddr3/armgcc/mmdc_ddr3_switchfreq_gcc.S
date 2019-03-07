/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 * this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
    .text
    .arm

    .global  mmdc_ddr3_switch_freq_start
    .global  mmdc_ddr3_switch_freq_end

.macro exit_configuration_mode

       mov r4, #0
       str r4, [r0, #0x1c]

    .endm

.macro switch_freq_24mhz
            /*set periph2_clk2 source from OSC*/
            ldr r4, [r1, #0x18]
            ldr r5, =(1 << 20)
            orr r4, r4, r5
            str r4, [r1, #0x18]
            /* set the periph2_clk2 clock divider to 1, fabric_mmdc_podf t0 1 */
            ldr r4, [r1, #0x14]
            bic r4, r4, #0x3f
            str r4, [r1, #0x14]
            /* set periph2_clk source from periph2_clk2 */
            ldr r4, [r1, #0x14]
            orr r4, r4, #(1 << 26)
            str r4, [r1, #0x14]
            /* wait ccm_handshake */
       poll_handshake0:
            ldr r4, [r1, #0x48]
            cmp r4, #0
            bne poll_handshake0

    .endm

.macro switch_freq_400mhz
           /* check the periph2_clk  source */
           ldr r4, [r1, #0x14]
           ands r4, r4, #(1 << 26)
           beq check_pre_periph2_clock
           /* set periph2_clk source from PLL */
           ldr r4, [r1, #0x14]
           bic r4, r4, #(1 << 26)
           str r4, [r1, #0x14]
           /* wait ccm_handshake */
       poll_handshake1:
           ldr r4, [r1, #0x48]
           cmp r4, #0
           bne poll_handshake1

       check_pre_periph2_clock:
           ldr r4, [r1, #0x18]
           ands r4, r4, #(1 << 21)
           beq check_fabric_mmdc_podf
           /* set pre_periph2_clock source from PLL2 PFD2 */
           ldr r4, [r1, #0x18]
           bic r4, r4, #(3 << 21)
           orr r4, r4, #(1 << 21)
           str r4, [r1, #0x18]

       check_fabric_mmdc_podf:
           ldr r4, [r1, #0x14]
           bic r4, r4, #0x38
           str r4, [r1, #0x14]
           /* wait ccm_handshake */
       poll_handshake2:
           ldr r4, [r1, #0x48]
           cmp r4, #0
           bne poll_handshake2

    .endm

/*!
* @brief change ddr3 device work freq 
* @param r0,mmdc base address
* @param r1,ccm base address
* @param r2,iomux base address
* @param r3,parameter start address
*/
.align 2
.type mmdc_ddr3_switch_freq_start, %function
mmdc_ddr3_switch_freq_start:
    /* push r4-r12 to full decrease stack */
    stmfd   sp!, {r4-r12}

    /* mask interrupt */
    CPSID I
    /* Disable data cache. */
    mrc p15, 0, r4, c1, c0, 0
    bic r4, r4, #0x4
    mcr p15, 0, r4, c1, c0, 0

    /* restore number of delay unit for switch to low freq */
    ldr r7, [r0, #0x8b8]
    mov r7, r7, lsr #16

    /* load paramter */
    ldr r4, [r3]
    ldr r8, [r3, r4]
    ldr r4, [r3, #4]
    ldr r9, [r3, r4]

    /* disable power down timer,precharge timer */
    ldr r4, [r0, #4]
    ldr r5, =0x7700ff00
    bic r4, r4, r5
    str r4, [r0, #4]
    /* disable automatic power saving */
    ldr r4, [r0, #0x404]
    orr r4, r4, #0x1
    str r4, [r0, #0x404]

    /* enter configuration mode */
    ldr r4, =0x8000
    str r4, [r0, #0x1c]
poll_config_ack0:
    ldr r4, [r0, #0x1c]
    and r4, r4, #0x4000
    cmp r4, #0x4000
    bne poll_config_ack0

    /* send precharge all command to channel0 */
    ldr r4, =0x00008050
    str r4, [r0, #0x1c]
    /* send precharge all command to channel1*/
    ldr r4, =0x00008058
    str r4, [r0, #0x1c]

    /* disable reordering */
    ldr r4, [r0, #0x400]
    bic r4, r4, #0x06000000
    orr r4, r4, #0x04000000
    str r4, [r0, #0x400]

    ldr r5, =125000000
    cmp r8, r5
    blt mmdc_ddr3_switch_low_freq

    /* switch to high freq */

    /* enter DVFS mode */
    ldr r4, [r0, #0x404]
    orr r4, r4, #0x00200000
    str r4, [r0, #0x404]

    /* exit configuration mode */
    exit_configuration_mode

poll_dvfs_ack0:
    ldr r4, [r0, #0x404]
    tst r4, #0x02000000
    beq poll_dvfs_ack0

    switch_freq_400mhz         

    /* enter step by step mode */
    ldr r4, [r0, #0x410]
    orr r4, r4, #0x100
    str r4, [r0, #0x410]

    /* exit self-refresh mode by clear DVFS */
    ldr r4, [r0, #0x404]
    bic r4, r4, #0x00200000
    str r4, [r0, #0x404]

poll_dvfs_ack1:
    ldr r4, [r0, #0x404]
    tst r4, #0x02000000
    bne poll_dvfs_ack1

    /* check dll status */
    cmp r9, #1
    beq restore_calibration
    /* force ZQ calibration */
    ldr r4, [r0, #0x800]
    ldr r5, =0x00010003
    bic r4, r4, r5 
    orr r4, r4, r5
    str r4, [r0, #0x800]
    /* enable DQS gating */
    ldr r4, [r0, #0x83C]
    bic r4, r4, #0x20000000
    str r4, [r0, #0x83C]
    /*enable measurement unit*/
    ldr r4, [r0, #0x8b8]
    bic r4, r4, #0x400
    /* perform force measurement */
    orr r4, r4, #0x800
    str r4, [r0, #0x8b8]
poll_self_clear0:
    ldr r4, [r0, #0x8b8]
    and r4, r4, #0x800
    cmp r4, #0x800
    beq poll_self_clear0

    /* disable pull down resistor for DQS */
    ldr r4, [r2, #0x280]
    ldr r5, =0xF038
    bic r4, r4, r5
    orr r4, r4, #0x38
    str r4, [r2, #0x280]
    ldr r4, [r2, #0x284]
    bic r4, r4, r5
    orr r4, r4, #0x38
    str r4, [r2, #0x284]
        
    /* enter configuration mode */
    ldr r4, =0x8000
    str r4, [r0, #0x1c]
poll_config_ack1:
    ldr r4, [r0, #0x1c]
    and r4, r4, #0x4000
    cmp r4, #0x4000
    bne poll_config_ack1

    /* restore 400MHZ timing */
    ldr r4, [r3, #8]
    add r4, r3, r4
    ldr r5, [r4, #0]
    ldr r6, [r4, #4]
    str r6, [r0, r5]
    add r4, r4, #8

    /* enable DLL */
    ldr r5, =0x00028031
    str r5, [r0, #0x1C]
    ldr r5, =0x00028039
    str r5, [r0, #0x1C]

    /* delay serveral clk */
    ldr r5, =0x100
delay_loop:
    subs r5, r5, #1
    bgt delay_loop

    /* reset DLL */
    ldr r5, =0x09208030
    str r5, [r0, #0x1C]
    ldr r5, =0x09208038
    str r5, [r0, #0x1C]

    /* delay serveral clk */
    ldr r5, =0x100
delay_loop1:
    subs r5, r5, #1
    bgt delay_loop1

    /* config RTT_NOM = RZQ/2 */
    ldr r5, =0x00428031
    str r5, [r0, #0x1C]
    ldr r5, =0x00428039
    str r5, [r0, #0x1C]

    /* config MR2 */
    ldr r5, [r4, #0]
    ldr r6, [r4, #4]
    str r6, [r0, r5]
    add r4, r4, #8
    /* restore timing information */
    ldr r5, [r4, #0]
    ldr r6, [r4, #4]
    str r6, [r0, r5]
    add r4, r4, #8
    ldr r5, [r4, #0]
    ldr r6, [r4, #4]
    str r6, [r0, r5]
    add r4, r4, #8

    /* issue a ZQ command */
    ldr r5, =0x04008040
    str r5, [r0, #0x1C]
    ldr r5, =0x04008048
    str r5, [r0, #0x1C]

    /* restore ODT */
    ldr r5, [r4, #0]
    ldr r6, [r4, #4]
    str r6, [r0, r5]
    add r4, r4, #8

    /* delay serveral clk */
    ldr r5, =0x100
delay_loop2:
    subs r5, r5, #1
    bgt delay_loop2

restore_calibration:
    ldr r4, [r3, #12]
    add r4, r3, r4
    ldr r7, [r4, #4]
    add r4, r4, #8

continue_update_calibration:
    ldr r5, [r4, #0]
    ldr r6, [r4, #4]
    str r6, [r0, r5]
    add r4, r4, #8
    sub r7, r7, #1
    cmp r7, #0
    bgt continue_update_calibration

    /* perform force measurement */
    ldr r4, =0x800
    str r4, [r0, #0x8b8]
poll_self_clear2:
    ldr r4, [r0, #0x8b8]
    and r4, r4, #0x800
    cmp r4, #0x800
    beq poll_self_clear2

    /* clear SBS */
    ldr r4, [r0, #0x410]
    bic r4, r4, #0x100
    str r4, [r0, #0x410]

    /* exit configuration mode */
    exit_configuration_mode
poll_config_ack3:
    ldr r4, [r0, #0x1c]
    and r4, r4, #0x4000
    cmp r4, #0x4000
    beq poll_config_ack3

    b done

mmdc_ddr3_switch_low_freq:
    /* check dll status */
    cmp r9, #0
    beq skip_dll_off

    /* disable the DLL and RTT_NOM for channel 0*/
    ldr r4, =0x00018031
    str r4, [r0, #0x1c]
    /* disable the DLL and RTT_NOM for channel 1 */
    ldr r4, =0x00018039
    str r4, [r0, #0x1c]
    /* config the CL to 6 for channel 0 */
    ldr r4, =0x08208030
    str r4, [r0, #0x1c]
    /* config the CL to 6 for channel 1 */
    ldr r4, =0x08208038
    str r4, [r0, #0x1c]
    /* config the CWL to 6 for channel 0 */
    ldr r4, =0x00088032
    str r4, [r0, #0x1c]
    /* config the CWL to 6 for channel 1 */
    ldr r4, =0x0008803A
    str r4, [r0, #0x1c]

    /* delay serveral clk */
    ldr r4, =0x100
delay_loop3:
    subs r4, r4, #1
    bgt delay_loop3
skip_dll_off:
    /* enter DVFS mode */
    ldr r4, [r0, #0x404]
    orr r4, r4, #0x00200000
    str r4, [r0, #0x404]

    /* exit configuration mode */
    exit_configuration_mode

poll_dvfs_ack2:
    ldr r4, [r0, #0x404]
    tst r4, #0x02000000
    beq poll_dvfs_ack2

    switch_freq_24mhz

    /* enter step by step mode */
    ldr r4, [r0, #0x410]
    orr r4, r4, #0x100
    str r4, [r0, #0x410]

    /* exit self-refresh mode by clear DVFS */
    ldr r4, [r0, #0x404]
    bic r4, r4, #0x00200000
    str r4, [r0, #0x404]

poll_dvfs_ack3:
    ldr r4, [r0, #0x404]
    tst r4, #0x02000000
    bne poll_dvfs_ack3

    /* enter configuration mode */
    ldr r4, =0x8000
    str r4, [r0, #0x1c]
poll_config_ack2:
    ldr r4, [r0, #0x1c]
    and r4, r4, #0x4000
    cmp r4, #0x4000
    bne poll_config_ack2

    /* update mmdc controller CL & CWL */
    ldr r4, [r0, #0xc]
    bic r4, r4, #0xf
    orr r4, r4, #0x3
    str r4, [r0, #0xc]
    ldr r4, [r0, #0x10]
    bic r4, r4, #0x7
    orr r4, r4, #0x4
    str r4, [r0, #0x10]
    /* set the read/write additional latency */
    ldr r4, [r0, #0x18]
    ldr r5, =0x301c0
    bic r4, r4, r5
    ldr r5, =0x10080
    orr r4, r4, r5
    str r4, [r0, #0x18]

    /* update iomux enable pull down resistor on DQS/change driver strength */
    ldr r4, [r2, #0x280]
    ldr r5, =0xF038
    ldr r6, =0x3028
    bic r4, r4, r5
    orr r4, r4, r6
    str r4, [r2, #0x280]
    ldr r4, [r2, #0x284]
    bic r4, r4, r5
    orr r4, r4, r6
    str r4, [r2, #0x284]

    /* disable ODT */
    mov r4, #0
    str r4, [r0, #0x818]

    /* disable DQS */
    ldr r4, [r0, #0x83C]
    orr r4, r4, #0x20000000
    str r4, [r0, #0x83C]

    /* add workaround for low freq */
    ldr r4, [r0, #0x8b8]
    ldr r6, =0x3ff
    and r7, r7, r6
    bic r4, r4, #0x3f
    orr r4, r4, r7, lsl #1
    orr r4, r4, #0x400
    str r4, [r0, #0x8b8]
    /* perform force measure */
    orr r4, r4, #0x800
    str r4, [r0, #0x8b8]
poll_self_clear1:
    ldr r4, [r0, #0x8b8]
    and r4, r4, #0x800
    cmp r4, #0x800
    beq poll_self_clear1

    /* clear SBS */
    ldr r4, [r0, #0x410]
    bic r4, r4, #0x100
    str r4, [r0, #0x410]

    /* exit configuration mode */
    exit_configuration_mode
poll_config_ack4:
    ldr r4, [r0, #0x1c]
    and r4, r4, #0x4000
    cmp r4, #0x4000
    beq poll_config_ack4

done:
    /* enable automatic power saving timer */
    ldr r4, [r0, #0x404]
    bic r4, r4, #1
    str r4, [r0, #0x404]
    /* enable mmdc power down timer */
    ldr r4, [r0, #4]
    ldr r5, =(0x55 << 8)
    orr r4, r4, r5
    str r4, [r0, #4]

    /* enable data cache */
    mrc p15, 0, r4, c1, c0, 0
    orr r4, r4, #0x4
    mcr p15, 0, r4, c1, c0, 0

    /* enable interrupt */
    CPSIE I
    /* restore registers */
    ldmfd   sp!, {r4-r12}
    mov pc, lr

    /* make sure the literal pool include in the text area*/
    .ltorg

.align 2
.type mmdc_ddr3_switch_freq_end, %function
mmdc_ddr3_switch_freq_end:

.end



