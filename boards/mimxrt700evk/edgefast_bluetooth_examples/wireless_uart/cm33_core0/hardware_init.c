/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_adapter_uart.h"

#include "controller_hci_uart.h"
#include "usb_host_config.h"
#include "usb_phy.h"
#include "usb_host.h"
#include "fsl_xspi.h"
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#include "fsl_cache.h"
#endif /* CONFIG_BT_SMP */
#if (defined(BUTTON_COUNT) && (BUTTON_COUNT > 0U))
#include "fsl_component_button.h"
#endif
#include "fsl_component_timer_manager.h"
/*${header:end}*/

/*${macro:start}*/
#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif
/*${macro:end}*/

/*${variable:start}*/
#if (defined(BUTTON_COUNT) && (BUTTON_COUNT > 0U))
button_config_t g_buttonConfig[] = {{
    .gpio =
        {
            .direction       = kHAL_GpioDirectionIn,
            .port            = BOARD_USER_BUTTON_GPIO,
            .pin             = BOARD_USER_BUTTON_GPIO_PIN,
            .pinStateDefault = 1,
        },
}};
extern BUTTON_HANDLE_ARRAY_DEFINE(s_buttonHandle, BUTTON_COUNT);
#endif
/*${variable:end}*/

/*${function:start}*/
void BOARD_Init_M2(void);
void BOARD_Init_BT_UART(void);
void BOARD_Init_Timer(void);
void BOARD_Init_Button(void);

void  BOARD_InitHardware(void)
{
    BOARD_Init_M2();

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    BOARD_InitAHBSC();

    BOARD_Init_Button();
    BOARD_Init_BT_UART();
    BOARD_Init_Timer();
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
    GlikeyWriteEnable(GLIKEY3, 1U);                                    /* Enable SYSCON0_SEC_CLK_CTRL write */
    SYSCON0->SEC_CLK_CTRL |= SYSCON0_SEC_CLK_CTRL_TRNG_REFCLK_EN_MASK; /* Enable TRNG REF CLOCK */

    CLOCK_AttachClk(kFRO1_DIV2_to_TRNG);                               /* Max 96MHZ with 1.0V nomral supply. */
    CLOCK_SetClkDiv(kCLOCK_DivTrngClk, 1U);
#endif /* CONFIG_BT_SMP */
}

void BOARD_Init_M2(void)
{
    CLOCK_EnableClock(kCLOCK_Gpio3);
    CLOCK_EnableClock(kCLOCK_Gpio7);
    RESET_PeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO7_RST_SHIFT_RSTn);
}

void BOARD_Init_BT_UART(void)
{
    CLOCK_AttachClk(BOARD_BT_UART_FCCLK_ATTACH);
    CLOCK_SetClkDiv(BOARD_BT_UART_FCCLK_DIV, 1U);

    /* Attach FC1 clock to LP_FLEXCOMM3(HCI uart). */
    CLOCK_AttachClk(BOARD_BT_UART_CLK_ATTACH);
}

void BOARD_Init_Timer(void)
{
    timer_config_t timerConfig;
    osa_status_t status;

    (void)memset(&timerConfig, 0, sizeof(timer_config_t));
    timerConfig.instance    = 0;
    timerConfig.srcClock_Hz = CLOCK_GetFreq(kCLOCK_BusClk);
    status                  = (osa_status_t)TM_Init(&timerConfig);
    assert(status == (osa_status_t)kStatus_TimerSuccess);
}

void BOARD_Init_Button(void)
{
    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio0);
}

#if (defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || defined(WIFI_IW416_BOARD_MURATA_1XK_M2)  || \
      defined(WIFI_IW612_BOARD_MURATA_2EL_M2))      
int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc        = BOARD_BT_UART_CLK_FREQ;
    config->defaultBaudrate = 115200u;
    config->runningBaudrate = BOARD_BT_UART_BAUDRATE;
    config->instance        = BOARD_BT_UART_INSTANCE;
    config->enableRxRTS = 1u;
    config->enableTxCTS = 1u;
    return 0;
}
#endif

void USB_HostClockInit(void)
{
    uint32_t usbClockFreq = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* Power on COM VDDN domain for USB or eUSB */
    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);    
        
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    { 
        /* Power on usb ram araay as need, powered USB0RAM array*/
        POWER_DisablePD(kPDRUNCFG_APD_USB0_SRAM);
        POWER_DisablePD(kPDRUNCFG_PPD_USB0_SRAM);
        /* Apply the config */
        POWER_ApplyPD();
        /* disable the read and write gate */
        SYSCON4->USB0_MEM_CTRL |= (SYSCON4_USB0_MEM_CTRL_MEM_WIG_MASK | SYSCON4_USB0_MEM_CTRL_MEM_RIG_MASK |
                                     SYSCON4_USB0_MEM_CTRL_MEM_STDBY_MASK);
        /* Enable the USBPHY0 CLOCK */
        SYSCON4->USBPHY0_CLK_ACTIVE |= SYSCON4_USBPHY0_CLK_ACTIVE_IPG_CLK_ACTIVE_MASK;
        CLOCK_AttachClk(k32KHZ_WAKE_to_USB);
        CLOCK_AttachClk(kOSC_CLK_to_USB_24MHZ);
        CLOCK_EnableClock(kCLOCK_Usb0);
        CLOCK_EnableClock(kCLOCK_UsbphyRef);
        RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
        RESET_PeripheralReset(kUSBPHY0_RST_SHIFT_RSTn);
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
        USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);
    }
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
/* USB_HOST_CONFIG_EHCI */

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}

/*${function:end}*/
