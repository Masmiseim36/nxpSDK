/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

#include "fsl_ftm.h"
#include "fsl_lpit.h"

#include "main.h"
#include "board.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "fsl_lpuart.h"
#include "pin_mux.h"

extern void TSI_DRV_IRQHandler(uint32_t instance);

static void pit_init(void);
static void ftm_init(void);

static void init_freemaster_lpuart(void);
static void keypad_callback(const struct nt_control *control, 
                            enum nt_control_keypad_event event,
                            uint32_t index);

static void aslider_callback(const struct nt_control *control, 
                             enum nt_control_aslider_event event,
                             uint32_t position);

static void arotary_callback(const struct nt_control *control, 
                             enum nt_control_arotary_event event, 
                             uint32_t position);

/* Call when the TSI counter overflows 65535 */
static void system_callback(uint32_t event, union nt_system_event_context * context);

static void SetHueBrightness(uint32_t hue_angle, uint32_t brightness);

#if defined(__ICCARM__)
  uint8_t nt_memory_pool[2600]; /* IAR EWARM compiler */
#else														
  uint8_t nt_memory_pool[2600] __attribute__((aligned (4)));  /* Keil, GCC compiler */
#endif	

uint32_t brightness_global, hue_angle_global;
uint8_t duty_cycle_red, duty_cycle_green, duty_cycle_blue;
tsi_status_t recalib_status;  

const ftm_config_t my_ftm_config = {    
    .prescale = kFTM_Prescale_Divide_1,                 /* Divide FTM clock by 1 */
    .bdmMode = kFTM_BdmMode_0,                          /* FTM behavior in BDM mode */
    .pwmSyncMode = kFTM_SoftwareTrigger,                /* Software trigger is used to update registers */
    .reloadPoints = 0,                                  /* No intermediate register load */
    .faultMode = kFTM_Fault_Disable,                    /* Fault control disabled for all channels */
    .faultFilterValue = 0,                              /* Disable the fault filter */
    .deadTimePrescale = kFTM_Deadtime_Prescale_1,       /* Divide the system clock by 1 */
    .deadTimeValue = 0,                                 /* No counts are inserted */
    .extTriggers = 0,                                   /* No external trigger */
    .chnlInitState = 0,                                 /* Initialization value is 0 for all channels */
    .chnlPolarity = 0x00,                               /* Active high polarity for all channels */
    .useGlobalTimeBase = false,                         /* Use internal FTM counter as timebase */
};

const ftm_chnl_pwm_signal_param_t my_ftm_channel_config[3] = {
    {
    .chnlNumber = kFTM_Chnl_0,   /*!< Channel 0 setup*/
    .level = kFTM_LowTrue,       /*!< PWM output active-high. */
    .dutyCyclePercent = 0U,      /*!< PWM pulse width 0%*/
    .firstEdgeDelayPercent = 0U, /*!< Symmetrical PWM */
    },
    {
    .chnlNumber = kFTM_Chnl_1,   /*!< Channel 1 setup*/
    .level = kFTM_LowTrue,       /*!< PWM output active-high. */
    .dutyCyclePercent = 0U,      /*!< PWM pulse width 0%*/
    .firstEdgeDelayPercent = 0U, /*!< Symmetrical PWM */
    },
    {
    .chnlNumber = kFTM_Chnl_2,   /*!< Channel 2 setup*/
    .level = kFTM_LowTrue,       /*!< PWM output active-high. */
    .dutyCyclePercent = 0U,      /*!< PWM pulse width 0%*/
    .firstEdgeDelayPercent = 0U, /*!< Symmetrical PWM */
    },
};

const lpit_config_t my_pit_config = {
    .enableRunInDebug = false,     /*!< Timer doesn't run in debug mode*/
    .enableRunInDoze  = false,     /*!< Timer doesn't run in doze mode*/
};

/*
 * This list describes all TSA tables that should be exported to the 
 * FreeMASTER application.
 */
#ifndef FMSTR_PE_USED
  FMSTR_TSA_TABLE_LIST_BEGIN()
      FMSTR_TSA_TABLE(nt_frmstr_tsa_table)
  FMSTR_TSA_TABLE_LIST_END()
#endif

      
#define nt_printf(...) /* do nothing - the debug lines are used by FreeMASTER */  
    
int main(void)
{
  int32_t result;
  bool recalib_enabled = false;  /* autotunning is off */
  bool one_key_only = false;     /* one key only valid is off */

  /* Init board hardware */
  BOARD_InitPins();
  
  BOARD_BootClockRUN();
  
  /*LED init*/
  LED_RED1_INIT(1);
  LED_GREEN1_INIT(1);
  LED_BLUE_INIT(1);
  
  /* Init FTM0 */
  ftm_init();
  
  /* Initialize the OS abstraction layer */
  NT_OSA_Init();
  
  /* FreeMASTER communication layer initialization */
  init_freemaster_lpuart();
  
  /* FreeMASTER initialization */
  (void)FMSTR_Init();
    
  if ((result = nt_init(&System_0, nt_memory_pool, sizeof(nt_memory_pool))) != NT_SUCCESS)
  {
    switch(result)
    {
    case NT_FAILURE:
      nt_printf("\nCannot initialize NXP Touch due to a non-specific error.\n");
      break;
    case NT_OUT_OF_MEMORY:
      nt_printf("\nCannot initialize NXP Touch due to a lack of free memory.\n");
      printf("\nCannot initialize NXP Touch due to a non-specific error.\n");
      break; 
    }
    while(1); /* add code to handle this error */
  }
  
  nt_printf("\nNXP Touch is successfully initialized.\n");
  
  nt_printf("Unused memory: %d bytes, you can make the memory pool smaller without affecting the functionality.\n", (int)nt_mem_get_free_size());
 
  /* Enable electrodes and controls */
  nt_enable();
  
  /* Disable FRDM-TOUCH board electrodes and controls if FRDM-TOUCH board is not connected */
  #if (NT_FRDM_TOUCH_SUPPORT) == 0 
    nt_electrode_disable(&El_3);
    nt_electrode_disable(&El_4);
    nt_electrode_disable(&El_5);
    nt_electrode_disable(&El_6);
    nt_electrode_disable(&El_7);
    nt_electrode_disable(&El_8);
    nt_electrode_disable(&El_9);
    nt_electrode_disable(&El_10);
    nt_electrode_disable(&El_11);
    nt_electrode_disable(&El_12);
    nt_control_disable(&ASlider_2);
    nt_control_disable(&ARotary_3);
  #endif
  
  /* Keypad electrodes*/
  nt_control_keypad_set_autorepeat_rate(&Keypad_1, 100, 1000);
  nt_control_keypad_register_callback(&Keypad_1, &keypad_callback);

  /* Slider electrodes */
  nt_control_aslider_register_callback(&ASlider_2, &aslider_callback);
  
  /* Rotary electrodes */
  nt_control_arotary_register_callback(&ARotary_3, &arotary_callback);
  
  /* System TSI overflow warning callback */
  nt_system_register_callback(&system_callback);
  
  /* Auto TSI register recalibration function, not used as default */
  if (recalib_enabled) 
  {   
      recalib_status = (tsi_status_t) nt_module_recalibrate(&nt_tsi_module);     
  } 
  if (one_key_only)
  nt_control_keypad_only_one_key_valid(&Keypad_1, true);
  
  pit_init();
  
  while(1)
  {
    nt_task();
    

    /* The FreeMASTER poll call must be called in the main application loop
    to handle the communication interface and protocol. 
       In the LONG_INTR FreeMASTER interrupt mode, all the processing is done 
    during the communication interrupt routine and the FMSTR_Poll() is 
    compiled empty. */
    FMSTR_Poll();      
  }
}


void LPIT0_IRQHandler(void)
{
  /* Clear the interrupt flag.*/
  LPIT_ClearStatusFlags(LPIT0, LPIT_MSR_TIF0_MASK);
  nt_trigger();
}

void TSI_IRQHandler(void)
{
  TSI_DRV_IRQHandler(0);
}


static void ftm_init(void)
{
  /* FTM0 Init */
  FTM_Init(FTM0, &my_ftm_config);
  
  /* Configure FTM0 channels 0, 1, 2 */
  FTM_SetupPwm(FTM0, my_ftm_channel_config, 3U, kFTM_EdgeAlignedPwm, 10000U, CLOCK_GetFreq(kCLOCK_CoreSysClk));
  
  /* Start timer */
  FTM_StartTimer(FTM0, kFTM_SystemClock);
}

static void pit_init(void)
{
  /* Set the source for the LPIT module */
  CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSircAsync);

  /* Init PIT */
  LPIT_Init(LPIT0, &my_pit_config);

  /* Set PIT0 period 5ms */
  LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_0, 5*CLOCK_GetFreq(kCLOCK_ScgSircAsyncDiv2Clk)/1000);

    /* Enable the PIT interrupt */
  LPIT_EnableInterrupts(LPIT0, LPIT_MIER_TIE0_MASK);
  EnableIRQ(LPIT0_IRQn);

  /* Start the PIT timer */
  LPIT_StartTimer(LPIT0, kLPIT_Chnl_0);
}

static void keypad_callback(const struct nt_control *control, 
                            enum nt_control_keypad_event event,
                            uint32_t index)
{
  switch(event)
  {
  case NT_KEYPAD_RELEASE:

    switch (index) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            break;
    }
    break;
  case NT_KEYPAD_TOUCH:

    switch (index) {
        case 0:
              /* YELLOW on, full brightness */
              hue_angle_global = 12;
              brightness_global = 120;
              SetHueBrightness(hue_angle_global, brightness_global);
            break;
        case 1:
              /* CYAN on, full brightness */
              hue_angle_global = 36;
              brightness_global = 120;
              SetHueBrightness(hue_angle_global, brightness_global);
            break;
        case 2:
              /* RED on, full brightness */
              hue_angle_global = 0;
              brightness_global = 120;
              SetHueBrightness(hue_angle_global, brightness_global);
            break;
        case 3:
              /* GREEN on, full brightness */
              hue_angle_global = 24;
              brightness_global = 120;
              SetHueBrightness(hue_angle_global, brightness_global);
            break;
        case 4:
              /* BLUE on, full brightness */
              hue_angle_global = 48;
              brightness_global = 120;
              SetHueBrightness(hue_angle_global, brightness_global);
            break;
        case 5:
              /* WHITE on, full brightness */
              hue_angle_global = 73;
              brightness_global = 120;
              SetHueBrightness(hue_angle_global, brightness_global);
            break;
        default:
            break;
    }
    break;
    
  case NT_KEYPAD_AUTOREPEAT:

    switch (index) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            break;
    }
    break;
    
  case NT_KEYPAD_MULTI_TOUCH:

    switch (index) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
    break;
    
  default:
    break;
    
  }
}

static void aslider_callback(const struct nt_control *control, 
                             enum nt_control_aslider_event event,
                             uint32_t position)
{
  switch(event)
  {
  case NT_ASLIDER_INITIAL_TOUCH:
    break;
  case NT_ASLIDER_MOVEMENT:
    break;
  case NT_ASLIDER_ALL_RELEASE:
    break;
  default:
    break; 
  }
  
  /* Recalculate and set RGB LED */
  brightness_global = position;
  SetHueBrightness(hue_angle_global, brightness_global);
}

static void arotary_callback(const struct nt_control *control, 
                             enum nt_control_arotary_event event, 
                             uint32_t position)
{
  switch (event) {
    case NT_AROTARY_MOVEMENT:
      break;
    case NT_AROTARY_ALL_RELEASE:
      break;
    case NT_AROTARY_INITIAL_TOUCH:
      break;
    default:
      break;
  }
  
  /* Recalculate and set RGB LED */
  hue_angle_global = position;  
  SetHueBrightness(hue_angle_global, brightness_global);
}

/* Call on the TSI CNTR overflow 16-bit range (65535) */
void system_callback(uint32_t event, union nt_system_event_context * context)
{
  switch(event)
    {
      case NT_SYSTEM_EVENT_DATA_OVERFLOW:
        /* TSI counter overflow warning */
        nt_printf("TSI CNTR overflow warning on electrode %d.\n", (int)index);
        break;
      default:
        break;    
    
  }
}

/*!
 * @brief Function calculating RGB values based on Hue and Brightness
 */
static void SetHueBrightness(uint32_t hue_angle, uint32_t brightness)
{
  /* Calculate Hue */
  if(hue_angle<=24)
  {
    duty_cycle_red   = 24-hue_angle;
    duty_cycle_green = hue_angle;
    duty_cycle_blue  = 0;
  }
  if(hue_angle>24 && hue_angle<=48)
  {
    duty_cycle_red   = 0;
    duty_cycle_green = 48-hue_angle;
    duty_cycle_blue  = hue_angle-24;
  }
  if(hue_angle>48)
  {
    duty_cycle_red   = hue_angle-48;
    duty_cycle_green = 0;
    duty_cycle_blue  = 72-hue_angle;
  }
  
  /* If hue_angle > 72, set RGB to white */
  if(hue_angle > 72)
  {
    duty_cycle_red   = 16;
    duty_cycle_green = 16;
    duty_cycle_blue  = 16;
  }
  
  /* Slider never returns position < 60 */
  brightness = brightness<60 ? 0 : (brightness-60);
  
  /* Set Hue multiplied by Brightness */
  FTM0->CONTROLS[kFTM_Chnl_0].CnV = (duty_cycle_blue * (brightness));
  FTM0->CONTROLS[kFTM_Chnl_1].CnV = (duty_cycle_green * (brightness));
  FTM0->CONTROLS[kFTM_Chnl_2].CnV = (duty_cycle_red * (brightness));
  
  /* Software trigger to update registers */
  FTM_SetSoftwareTrigger(FTM0, true);
}

/*!
 * @brief LPUART Module initialization (LPUART is a the standard block included e.g. in K66F)
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    CLOCK_SetIpSrc(kCLOCK_Lpuart1, BOARD_DEBUG_UART_CLKSRC);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200U;
    config.enableTx = false;
    config.enableRx = false;

    LPUART_Init(BOARD_DEBUG_UART_BASEADDR, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(BOARD_DEBUG_UART_BASEADDR);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
}

/*!
 * @brief An example of a direct interrupt vector installation - overriding the weak
 * symbol which is put into the vector table. Note that the function name
 * contains the UART instance number. Replace the UART1 in the name with the
 * proper instance number (for example, the value of the BOARD_DEBUG_UART_INSTANCE
 * macro defined in board.h)
 * 
 * This 'direct' approach may be used instead of OSA_InstallIntHandler
 * (above), for example, when the ROM-based interrupt table is used.
 */
 
 void BOARD_UART_IRQ_HANDLER(void)
 {
     //FMSTR_SerialIsr();
 }
