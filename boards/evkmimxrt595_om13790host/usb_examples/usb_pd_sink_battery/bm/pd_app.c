/*
 * Copyright 2016 - 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdbool.h>
#include "usb_pd_config.h"
#include "usb_pd.h"
#include "usb_pd_i2c.h"
#include "pd_app.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pd_power_interface.h"
#include "pd_board_config.h"
#include "pd_power_nx20p3483.h"
#include "fsl_adapter_timer.h"
#include "pin_mux.h"
#include "board.h"

#include "fsl_adapter_gpio.h"
#include "fsl_inputmux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if (PD_DEMO_PORTS_COUNT > PD_CONFIG_MAX_PORT)
#error "please increase the instance count"
#endif

#ifndef PD_TIMER_INSTANCE
#define PD_TIMER_INSTANCE (0)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

uint32_t HW_I2CGetFreq(uint8_t i2cInstance);
void HW_I2CReleaseBus(void);
void PD_DemoInit(void);
void PD_DemoTaskFun(void);
void BOARD_InitHardware(void);
void PD_Demo1msIsrProcess(void);
uint32_t HW_TimerGetFreq(void);
void HW_TimerCallback(void *callbackParam);
void PD_DemoReset(pd_app_t *pdAppInstance);
pd_status_t PD_DpmAppCommandCallback(void *callbackParam, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static pd_sink_pdo_t s_PortSinkcaps[] = {{
                                             /* PDO1: fixed:5.0V, 3A */
                                             .fixedPDO.fixedSupply     = kPDO_Fixed,
                                             .fixedPDO.dualRoleData    = 1U,
                                             .fixedPDO.dualRolePower   = 1U,
                                             .fixedPDO.externalPowered = 1U,
#if ((defined PD_CONFIG_REVISION) && (PD_CONFIG_REVISION >= PD_SPEC_REVISION_30))
#if (defined PD_CONFIG_COMPLIANCE_TEST_ENABLE) && (PD_CONFIG_COMPLIANCE_TEST_ENABLE)
                                             .fixedPDO.frSwapRequiredCurrent = 0U,
#else
                                             .fixedPDO.frSwapRequiredCurrent = kFRSwap_CurrentDefaultUSB,
#endif
#endif
                                             .fixedPDO.higherCapability         = 1U,
                                             .fixedPDO.usbCommunicationsCapable = 0U,
                                             .fixedPDO.voltage                  = (5U * 1000U / 50U),
                                             .fixedPDO.operateCurrent           = (3U * 100U),
                                         },
                                         {
                                             /* PDO2: fixed: 9V, 2A */
                                             .fixedPDO.fixedSupply    = kPDO_Fixed,
                                             .fixedPDO.voltage        = (9U * 1000U / 50U),
                                             .fixedPDO.operateCurrent = (2U * 100U),
                                         }};

static pd_power_port_config_t s_Port1PowerConfig = {
    NULL,                                           /* source caps */
    (uint32_t *)&s_PortSinkcaps[0],                 /* self sink caps */
    0,                                              /* source cap count */
    sizeof(s_PortSinkcaps) / sizeof(pd_sink_pdo_t), /* sink cap count */
    kPowerConfig_SinkOnly,                          /* typec role */
    kCurrent_Invalid,                               /* source: Rp current level */
    kTypecTry_None,                                 /* drp try function */
    kDataConfig_DRD,                                /* data function */
    0,                                              /* support vconn */
    0,                                              /* reserved */
    NULL,
    NULL,
    NULL,
};

#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE)
static pd_phy_config_t s_Port1PhyConfig = {
    .i2cInstance   = kInterface_i2c0 + BOARD_PD_I2C_INDEX,
    .slaveAddress  = 0x52u,
    .i2cSrcClock   = 0u,
    .i2cReleaseBus = HW_I2CReleaseBus,
    .alertPort     = PD_PORT1_PHY_INTERRUPT_PORT,
    .alertPin      = PD_PORT1_PHY_INTERRUPT_PIN,
    .alertPriority = PD_PORT1_PHY_INTERRUPT_PRIORITY,
};

pd_instance_config_t g_Port1PDConfig = {
    kDeviceType_NormalPowerPort, /* normal power port */
    kPD_PhyPTN5110,
    &s_Port1PhyConfig,
    &s_Port1PowerConfig,
};
#endif

pd_instance_config_t *g_PortsConfigArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE)
    &g_Port1PDConfig,
#endif
};

#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE > 0)
pd_app_t g_PDAppInstancePort1;
#endif
#if (defined PD_DEMO_PORT2_ENABLE) && (PD_DEMO_PORT2_ENABLE > 0)
pd_app_t g_PDAppInstancePort2;
#endif
#if (defined PD_DEMO_PORT3_ENABLE) && (PD_DEMO_PORT3_ENABLE > 0)
pd_app_t g_PDAppInstancePort3;
#endif
#if (defined PD_DEMO_PORT4_ENABLE) && (PD_DEMO_PORT4_ENABLE > 0)
pd_app_t g_PDAppInstancePort4;
#endif

pd_app_t *g_PDAppInstanceArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE > 0)
    &g_PDAppInstancePort1,
#endif
#if (defined PD_DEMO_PORT2_ENABLE) && (PD_DEMO_PORT2_ENABLE > 0)
    &g_PDAppInstancePort2,
#endif
#if (defined PD_DEMO_PORT3_ENABLE) && (PD_DEMO_PORT3_ENABLE > 0)
    &g_PDAppInstancePort3,
#endif
#if (defined PD_DEMO_PORT4_ENABLE) && (PD_DEMO_PORT4_ENABLE > 0)
    &g_PDAppInstancePort4,
#endif
};

pd_power_handle_callback_t callbackFunctions = {
    NULL,
    NULL,
    NULL,
    NULL,
    PD_PowerSnkDrawTypeCVbus,
    PD_PowerSnkDrawRequestVbus,
    PD_PowerSnkStopDrawVbus,
    PD_PowerSnkGotoMinReducePower,
    PD_PowerControlVconn,
};

TIMER_HANDLE_DEFINE(g_PDTimerHandle);
/*******************************************************************************
 * Code
 ******************************************************************************/
#define I2C_RELEASE_BUS_COUNT 100U
#define PD_I2C_SCL            (21U)
#define PD_I2C_SDA            (22U)
#define PD_I2C_PORT           (4U)


uint32_t HW_TimerGetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

uint32_t HW_I2CGetFreq(uint8_t instance)
{
    return CLOCK_GetFreq(kCLOCK_Flexcomm11Clk);
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void HW_I2CReleaseBus(void)
{
    hal_gpio_pin_config_t config;
    GPIO_HANDLE_DEFINE(pdI2cSclGpio);
    GPIO_HANDLE_DEFINE(pdI2cSdaGpio);
    uint8_t i = 0;

    /* Config pin mux as gpio */
    I2C11_DeinitPins();

    /* Init I2C GPIO */
    config.direction = kHAL_GpioDirectionOut;
    config.port      = PD_I2C_PORT;
    config.pin       = PD_I2C_SCL;
    config.level     = 1;
    HAL_GpioInit((hal_gpio_handle_t)pdI2cSclGpio, &config);

    config.pin = PD_I2C_SDA;
    HAL_GpioInit((hal_gpio_handle_t)pdI2cSdaGpio, &config);

    i2c_release_bus_delay();

    /* Drive SDA low first to simulate a start */
    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 0U);
        i2c_release_bus_delay();

        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 1U);
        i2c_release_bus_delay();

        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 0U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 0U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 1U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 1U);
    i2c_release_bus_delay();
    /* De-init I2C GPIO */
    HAL_GpioDeinit((hal_gpio_handle_t)pdI2cSclGpio);
    HAL_GpioDeinit((hal_gpio_handle_t)pdI2cSdaGpio);

    /* re-configure pin mux as i2c */
    I2C11_InitPins();
}


static void PD_GpioInit(pd_app_t *pdAppInstance)
{
    pd_demo_io_init_t portsDemoPinConfigArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE)
        {
            .extraEnSrcPort = PD_PORT1_EXTRA_SRC_PORT,
            .extraEnSrcPin  = PD_PORT1_EXTRA_SRC_PIN,
        }
#endif
    };
    pd_demo_io_init_t *demoGpioPinConfig = &portsDemoPinConfigArray[pdAppInstance->portNumber - 1];
    hal_gpio_pin_config_t config;

    config.direction = kHAL_GpioDirectionOut;
    config.port      = demoGpioPinConfig->extraEnSrcPort;
    config.pin       = demoGpioPinConfig->extraEnSrcPin;
    config.level     = 1U;
    HAL_GpioInit((hal_gpio_handle_t *)(&pdAppInstance->gpioExtraSrcHandle[0]), &config);
}

static pd_status_t PD_DpmConnectCallback(void *callbackParam, uint32_t event, void *param)
{
    pd_status_t status      = kStatus_PD_Error;
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

    switch (event)
    {
        case PD_DISCONNECTED:
            PD_PowerSnkStopDrawVbus(pdAppInstance, kVbusPower_Stable);
            PD_DemoReset(pdAppInstance);
            PRINTF("port %d disconnect\r\n", pdAppInstance->portNumber);
            status = kStatus_PD_Success;
            break;

        case PD_CONNECT_ROLE_CHANGE:
        case PD_CONNECTED:
        {
            uint8_t getInfo;

            PD_NX20PExitDeadBatteryMode(pdAppInstance->portNumber);
            PD_DemoReset(pdAppInstance);
            pdAppInstance->partnerSourceCapNumber = 0;
            PD_Control(pdAppInstance->pdHandle, PD_CONTROL_GET_POWER_ROLE, &getInfo);

            PRINTF((event == PD_CONNECTED) ? "port %d connected," : "port %d connect change,",
                   pdAppInstance->portNumber);
            PRINTF(" power role:%s,", (getInfo == kPD_PowerRoleSource) ? "Source" : "Sink");
            PD_Control(pdAppInstance->pdHandle, PD_CONTROL_GET_DATA_ROLE, &getInfo);
            PRINTF(" data role:%s,", (getInfo == kPD_DataRoleDFP) ? "DFP" : "UFP");
            PD_Control(pdAppInstance->pdHandle, PD_CONTROL_GET_VCONN_ROLE, &getInfo);
            PRINTF(" vconn source:%s\r\n", (getInfo == kPD_IsVconnSource) ? "yes" : "no");
            status = kStatus_PD_Success;
            break;
        }

        default:
            break;
    }

    return status;
}

static pd_status_t PD_DpmDemoAppCallback(void *callbackParam, uint32_t event, void *param)
{
    pd_status_t status      = kStatus_PD_Error;
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

    switch (event)
    {
        case PD_FUNCTION_DISABLED:
            /* need hard or software reset */
            status = kStatus_PD_Success;
            break;

        case PD_CONNECTED:
        case PD_CONNECT_ROLE_CHANGE:
        case PD_DISCONNECTED:
            status = PD_DpmConnectCallback(callbackParam, event, param);
            break;

        case PD_DPM_OVP_OCP_FAULT:
            PD_NX20PClearInt(pdAppInstance->portNumber);
            break;

        case PD_DPM_VBUS_ALARM:
            /* Users need to take care of this. Some corrective actions may be taken, such as disconnect
               or debounce time to turn off the power switch. It is up to the system power management. */
            break;

        default:
            status = PD_DpmAppCommandCallback(callbackParam, event, param);
            break;
    }
    return status;
}

static void PD_AppInit(void)
{
    uint8_t index;
    pd_app_t *pdAppInstance;
    pd_app_t *pdAppInstanceArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE > 0)
        &g_PDAppInstancePort1,
#else
        NULL,
#endif
#if (defined PD_DEMO_PORT2_ENABLE) && (PD_DEMO_PORT2_ENABLE > 0)
        &g_PDAppInstancePort2,
#else
        NULL,
#endif
#if (defined PD_DEMO_PORT3_ENABLE) && (PD_DEMO_PORT3_ENABLE > 0)
        &g_PDAppInstancePort3,
#else
        NULL,
#endif
#if (defined PD_DEMO_PORT4_ENABLE) && (PD_DEMO_PORT4_ENABLE > 0)
        &g_PDAppInstancePort4,
#else
        NULL,
#endif
    };

    for (index = 0U; index < 4U; ++index)
    {
        if (pdAppInstanceArray[index] != NULL)
        {
            pdAppInstanceArray[index]->portNumber = (index + 1U);
        }
    }

    for (index = 0U; index < PD_DEMO_PORTS_COUNT; ++index)
    {
        pdAppInstance                = g_PDAppInstanceArray[index];
        pdAppInstance->pdHandle      = NULL;
        pdAppInstance->pdConfigParam = g_PortsConfigArray[index];
        ((pd_phy_config_t *)pdAppInstance->pdConfigParam->phyConfig)->i2cSrcClock =
            HW_I2CGetFreq(((pd_phy_config_t *)pdAppInstance->pdConfigParam->phyConfig)->i2cInstance);

        if (PD_InstanceInit(&pdAppInstance->pdHandle, PD_DpmDemoAppCallback, &callbackFunctions, pdAppInstance,
                            g_PortsConfigArray[index]) != kStatus_PD_Success)
        {
            PRINTF("pd port %d init fail\r\n", pdAppInstance->portNumber);
        }
        else
        {
            PD_GpioInit(pdAppInstance);
            PD_PowerBoardControlInit(pdAppInstance->portNumber, pdAppInstance->pdHandle);

            pdAppInstance->msgSop                 = kPD_MsgSOP;
            pdAppInstance->partnerSourceCapNumber = 0;
            /* alternate mode (VDM) */
            pdAppInstance->selfVdmIdentity.idHeaderVDO.vdoValue                      = 0;
            pdAppInstance->selfVdmIdentity.idHeaderVDO.bitFields.modalOperateSupport = 1;
            pdAppInstance->selfVdmIdentity.idHeaderVDO.bitFields.connectorType = kConnectorType_UsbTypecReceptacle;
#if ((defined PD_CONFIG_REVISION) && (PD_CONFIG_REVISION >= PD_SPEC_REVISION_30))
            pdAppInstance->selfVdmIdentity.idHeaderVDO.bitFields.productTypeDFP = 2; /* PDUSB Host */
#endif
            pdAppInstance->selfVdmIdentity.idHeaderVDO.bitFields.productTypeUFPOrCablePlug = 2; /* PDUSB Peripheral */
            pdAppInstance->selfVdmIdentity.idHeaderVDO.bitFields.usbCommunicationCapableAsDevice = 0;
            pdAppInstance->selfVdmIdentity.idHeaderVDO.bitFields.usbCommunicationCapableAsHost   = 0;
            pdAppInstance->selfVdmIdentity.idHeaderVDO.bitFields.usbVendorID                     = PD_VENDOR_VID;
            pdAppInstance->selfVdmIdentity.pid                                                   = PD_CONFIG_PID;
            pdAppInstance->selfVdmIdentity.certStatVDO                                           = PD_CONFIG_XID;
            pdAppInstance->selfVdmIdentity.bcdDevice                                             = PD_CONFIG_BCD_DEVICE;
            pdAppInstance->selfVdmSVIDs = ((uint32_t)PD_VENDOR_VID << 16); /* only one SVID (display port) */
            PRINTF("pd port %d init success\r\n", pdAppInstance->portNumber);
        }
    }
}

#if (defined(__DSC__) && defined(__CW__))
static void HW_GpioPDPHYPortsIntCallback(void *callbackParam)
{
    uint8_t intPinState;
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

    PD_PTN5110IsrFunction(pdAppInstance->pdHandle);
}
#endif

static void PD_AppTimerInit(void)
{
    hal_timer_config_t halTimerConfig;
    halTimerConfig.timeout     = 1000;
    halTimerConfig.srcClock_Hz = HW_TimerGetFreq();
    halTimerConfig.instance    = PD_TIMER_INSTANCE;
    HAL_TimerInit((hal_timer_handle_t)&g_PDTimerHandle[0], &halTimerConfig);

    HAL_TimerInstallCallback((hal_timer_handle_t)&g_PDTimerHandle[0], HW_TimerCallback, NULL);
    HAL_TimerEnable((hal_timer_handle_t)&g_PDTimerHandle[0]);
}

void HW_TimerCallback(void *callbackParam)
{
    /* Callback into timer service */
    uint8_t index;
    for (index = 0; index < PD_DEMO_PORTS_COUNT; ++index)
    {
        PD_TimerIsrFunction(g_PDAppInstanceArray[index]->pdHandle);
#if (defined(__DSC__) && defined(__CW__))
        /* DSC doesn't support low level interrupt, workaround this to poll
         * alert pin and external power pin state in the 1ms timer IRQ */
        HW_GpioPDPHYPortsIntCallback(g_PDAppInstanceArray[index]);
#endif
    }

    PD_Demo1msIsrProcess();
}

int main(void)
{
    uint8_t index;

    /* Use 48 MHz clock for the FLEXCOMM11 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM11);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    NVIC_SetPriority(BOARD_UART_IRQ, 5);
    I2C11_InitPins();

    /* @TEST_ANCHOR */
    PD_AppTimerInit();
    PD_AppInit();
    PD_DemoInit();

    while (1)
    {
#if (defined PD_CONFIG_COMMON_TASK) && (PD_CONFIG_COMMON_TASK)
        PD_Task();
#else
        for (index = 0; index < PD_DEMO_PORTS_COUNT; ++index)
        {
            PD_InstanceTask(g_PDAppInstanceArray[index]->pdHandle);
        }
#endif
        PD_DemoTaskFun();
    }
}
