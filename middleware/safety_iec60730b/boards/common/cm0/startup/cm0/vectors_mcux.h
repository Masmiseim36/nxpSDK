/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _VECTORS_H_
#define _VECTORS_H_
 
#include "safety_config.h"

/*******************************************************************************
* API
******************************************************************************/
void default_isr(void); /* function prototype for default_isr in vectors.c */
void ResetISR(void);
void NMI_isr(void); /* included in vector.c */
void hard_fault_handler_c(uint32_t * hardfault_args);
typedef void pointer(void); /* Interrupt Vector Table Function Pointers */
extern uint32_t __BOOT_STACK_ADDRESS[];
extern void start(void);

/*******************************************************************************
* Definitions
******************************************************************************/
//				Address     Vector IRQ   Source module   Source description
#define VECTOR_000      (pointer*)__BOOT_STACK_ADDRESS	//          ARM core        Initial Supervisor SP
#define VECTOR_001      ResetISR	    // 0x0000_0004 1 -          ARM core        Initial Program Counter
#define VECTOR_002      NMI_isr         // 0x0000_0008 2 -          ARM core        Non-maskable Interrupt (NMI)
#define VECTOR_003      default_isr     // 0x0000_000C 3 -          ARM core        Hard Fault
#define VECTOR_004      default_isr     // 0x0000_0010 4 -
#define VECTOR_005      default_isr     // 0x0000_0014 5 -          ARM core         Bus Fault
#define VECTOR_006      default_isr     // 0x0000_0018 6 -          ARM core         Usage Fault
#define VECTOR_007      default_isr     // 0x0000_001C 7 -
#define VECTOR_008      default_isr     // 0x0000_0020 8 -
#define VECTOR_009      default_isr     // 0x0000_0024 9 -
#define VECTOR_010      default_isr     // 0x0000_0028 10 -
#define VECTOR_011      default_isr     // 0x0000_002C 11 -         ARM core         Supervisor call (SVCall)
#define VECTOR_012      default_isr     // 0x0000_0030 12 -         ARM core         Debug Monitor
#define VECTOR_013      default_isr     // 0x0000_0034 13 -
#define VECTOR_014      default_isr     // 0x0000_0038 14 -         ARM core         Pendable request for system service (PendableSrvReq)
#define VECTOR_015      default_isr     // 0x0000_003C 15 -         ARM core         System tick timer (SysTick)
#define VECTOR_016      default_isr     // 0x0000_0040 16     0     DMA              DMA Channel 0 transfer complete
#define VECTOR_017      default_isr     // 0x0000_0044 17     1     DMA              DMA Channel 1 transfer complete
#define VECTOR_018      default_isr     // 0x0000_0048 18     2     DMA              DMA Channel 2 transfer complete
#define VECTOR_019      default_isr     // 0x0000_004C 19     3     DMA              DMA Channel 3 transfer complete
#define VECTOR_020      default_isr     // 0x0000_0050 20     4     DMA              DMA Channel 4 transfer complete
#define VECTOR_021      default_isr     // 0x0000_0054 21     5     DMA              DMA Channel 5 transfer complete
#define VECTOR_022      default_isr     // 0x0000_0058 22     6     DMA              DMA Channel 6 transfer complete
#define VECTOR_023      default_isr     // 0x0000_005C 23     7     DMA              DMA Channel 7 transfer complete
#define VECTOR_024      default_isr     // 0x0000_0060 24     8     DMA              DMA Channel 8 transfer complete
#define VECTOR_025      default_isr     // 0x0000_0064 25     9     DMA              DMA Channel 9 transfer complete
#define VECTOR_026      default_isr     // 0x0000_0068 26    10     DMA              DMA Channel 10 transfer complete
#define VECTOR_027      default_isr     // 0x0000_006C 27    11     DMA              DMA Channel 11 transfer complete
#define VECTOR_028      default_isr     // 0x0000_0070 28    12     DMA              DMA Channel 12 transfer complete
#define VECTOR_029      default_isr     // 0x0000_0074 29    13     DMA              DMA Channel 13 transfer complete
#define VECTOR_030      default_isr     // 0x0000_0078 30    14     DMA              DMA Channel 14 transfer complete
#define VECTOR_031      default_isr     // 0x0000_007C 31    15     DMA              DMA Channel 15 transfer complete
#define VECTOR_032      default_isr     // 0x0000_0080 32    16     DMA              DMA Error Interrupt Channels 0-15
#define VECTOR_033      default_isr     // 0x0000_0084 33    17     MCM              Normal interrupt
#define VECTOR_034      default_isr     // 0x0000_0088 34    18     Flash memory     Command Complete
#define VECTOR_035      default_isr     // 0x0000_008C 35    19     Flash memory     Read Collision
#define VECTOR_036      default_isr     // 0x0000_0090 36    20     Mode Controller  Low Voltage Detect,Low Voltage Warning, Low Leakage Wakeup
#define VECTOR_037      default_isr     // 0x0000_0094 37    21     LLWU
#define VECTOR_038      default_isr     // 0x0000_0098 38    22     WDOG/EWM
#define VECTOR_039      default_isr     // 0x0000_009C 39    23	    RNGB
#define VECTOR_040      default_isr     // 0x0000_00A0 40    24     I2C0
#define VECTOR_041      default_isr     // 0x0000_00A4 41    25     I2C1
#define VECTOR_042      default_isr     // 0x0000_00A8 42    26     SPI0             Single interrupt vector for all sources
#define VECTOR_043      default_isr     // 0x0000_00AC 43    27     SPI1             Single interrupt vector for all sources
#define VECTOR_044      default_isr     // 0x0000_00B0 44    28     -
#define VECTOR_045      default_isr     // 0x0000_00B4 45    29     -
#define VECTOR_046      default_isr     // 0x0000_00B8 46    30     UART0            Single source for all LON source
#define VECTOR_047      default_isr     // 0x0000_00BC 47    31     UART0            Single source for all status source
#define VECTOR_048      default_isr     // 0x0000_00C0 48    32     UART0            Single source for all Error source
#define VECTOR_049      default_isr     // 0x0000_00C4 49    33     UART1            Single source for all status source
#define VECTOR_050      default_isr     // 0x0000_00C8 50    34     UART1            Single source for all Error source
#define VECTOR_051      default_isr     // 0x0000_00CC 51    35     UART2            Single source for all status source
#define VECTOR_052      default_isr     // 0x0000_00D0 52    36     UART2            Single source for all Error source
#define VECTOR_053      default_isr     // 0x0000_00D4 53    37     -
#define VECTOR_054      default_isr     // 0x0000_00D8 54    38     -
#define VECTOR_055      default_isr     // 0x0000_00DC 55    39     ADC0
#define VECTOR_056      default_isr     // 0x0000_00E0 56    40     CMP0
#define VECTOR_057      default_isr     // 0x0000_00E4 57    41     CMP1
#define VECTOR_058      default_isr     // 0x0000_00E8 58    42     FTM0             Single source for all source
#define VECTOR_059      default_isr     // 0x0000_00EC 59    43     FTM1             Single source for all source
#define VECTOR_060      default_isr     // 0x0000_00F0 60    44     FTM2             Single source for all source
#define VECTOR_061      default_isr     // 0x0000_00F4 61    45     -
#define VECTOR_062      default_isr     // 0x0000_00F8 62    46     Reserved
#define VECTOR_063      default_isr     // 0x0000_00FC 63    47     Reserved
#define VECTOR_064      default_isr     // 0x0000_0100 64    48     PIT              CH0
#define VECTOR_065      default_isr     // 0x0000_0104 65    49     PIT              CH1
#define VECTOR_066      default_isr     // 0x0000_0108 66    50     PIT              CH2
#define VECTOR_067      default_isr     // 0x0000_010C 67    51     PIT              CH3
#define VECTOR_068      default_isr     // 0x0000_0110 68    52     PDB
#define VECTOR_069      default_isr     // 0x0000_0114 69    53     USB_OTG
#define VECTOR_070      default_isr     // 0x0000_0118 70    54     -
#define VECTOR_071      default_isr     // 0x0000_011C 71    55     -
#define VECTOR_072      default_isr     // 0x0000_0120 72    56     DAC0
#define VECTOR_073      default_isr     // 0x0000_0124 73    57     MCG
#define VECTOR_074      default_isr     // 0x0000_0128 74    58     LowPowerTimer
#define VECTOR_075      default_isr     // 0x0000_012C 75    59     PORTA            PORTA PORTS_IRQHandler
#define VECTOR_076      default_isr     // 0x0000_0130 76    60     PORTB            PORTB
#define VECTOR_077      default_isr     // 0x0000_0134 77    61     PORTC            PORTC PORTS_IRQHandler
#define VECTOR_078      default_isr     // 0x0000_0138 78    62     PORTD            PORTD
#define VECTOR_079      default_isr     // 0x0000_013C 79    63     PORTE            PORTE
#define VECTOR_080      default_isr     // 0x0000_0140 80    64     SW 	             Software
#define VECTOR_081      default_isr     // 0x0000_0144 81    65     -
#define VECTOR_082      default_isr     // 0x0000_0148 82    66     -
#define VECTOR_083      default_isr     // 0x0000_014C 83    67     -
#define VECTOR_084      default_isr     // 0x0000_0150 84    68     -
#define VECTOR_085      default_isr     // 0x0000_0154 85    69     -
#define VECTOR_086      default_isr     // 0x0000_0158 86    70     -
#define VECTOR_087      default_isr     // 0x0000_015C 87    71     FTM3             Single interrupt vector for all sources
#define VECTOR_088      default_isr     // 0x0000_0160 88    72     DAC1
#define VECTOR_089      default_isr     // 0x0000_0164 89    73     ADC1


#if (!defined(_LPC845_H_) && !defined(_LPC824_H_) && !defined(_LPC804_H_) && !defined(_LPC802_H_))
#define CONFIG_1  (uint32_t)0xFFFFFFFF 
#define CONFIG_2  (uint32_t)0xFFFFFFFF 
#define CONFIG_3  (uint32_t)0xFFFFFFFF
#ifdef 	_MKE15Z7_H_
 #define CONFIG_4 (uint32_t)0xFFFF7DFE
#else
	#ifdef MKE02Z2_H_
		#define CONFIG_4 (uint32_t) 0xFFFEFFFF
	#else
		#define CONFIG_4 (uint32_t) 0xFFFFFFFE
	#endif
#endif  
#endif

#endif /* _VECTORS_H_ */
