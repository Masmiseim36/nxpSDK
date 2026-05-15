/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "fsl_soc_src.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_ele_base_api.h"
#include "board.h"
#include "fsl_xbar.h"
#include "fsl_iomuxc.h"
#include "fsl_ecat.h"
#include "ecat_def.h"
#include "ecatslv.h"
#include "ecat_hw.h"
#include "ecatappl.h"
#include "mu_ipc_shm_host.h"
/*${header:end}*/

/*${macro:start}*/
#define ELE_TRDC_AON_ID    0x74
#define ELE_TRDC_WAKEUP_ID 0x78
#define ELE_CORE_CM33_ID   0x1
#define ELE_CORE_CM7_ID    0x2
#define IPC_SHM_SIZE  4096
/*
 * Set ELE_STICK_FAILED_STS to 0 when ELE status check is not required,
 * which is useful when debug reset, where the core has already get the
 * TRDC ownership at first time and ELE is not able to release TRDC
 * ownership again for the following TRDC ownership request.
 */
#define ELE_STICK_FAILED_STS 1

#if ELE_STICK_FAILED_STS
#define ELE_IS_FAILED(x) (x != kStatus_Success)
#else
#define ELE_IS_FAILED(x) false
#endif
/*${macro:end}*/

UINT32 EcatTimerCnt;

#if defined(__ICCARM__) /* IAR Workbench */
#pragma location = "rpmsg_sh_mem_section"
static char ipc_shm_section[IPC_SHM_SIZE];
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
static char ipc_shm_section[IPC_SHM_SIZE] __attribute__((section("rpmsg_sh_mem_section")));
#elif defined(__GNUC__)
static char ipc_shm_section[IPC_SHM_SIZE] __attribute__((section(".noinit.$rpmsg_sh_mem")));
#else
#error "RPMsg: Please provide your definition of rpmsg_lite_base[]!"
#endif
/*${function:start}*/


/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    Prepare_CM7(CORE1_KICKOFF_ADDRESS);
}

#if !(defined(BOOT_CORE1_BY_MU) && BOOT_CORE1_BY_MU)
void APP_BootCore1(void)
{
    status_t sts;

    /* Enble CM7 */
    do
    {
        sts = ELE_BaseAPI_EnableAPC(MU_RT_S3MUA);
    } while (ELE_IS_FAILED(sts));

    /* Deassert Wait */
    BLK_CTRL_S_AONMIX->M7_CFG =
        (BLK_CTRL_S_AONMIX->M7_CFG & (~BLK_CTRL_S_AONMIX_M7_CFG_WAIT_MASK)) | BLK_CTRL_S_AONMIX_M7_CFG_WAIT(0);
}
#endif

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)__section_begin("__core1_image");
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
	BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

static void Ecat_KickOff(void)
{
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_RMII_SEL0_MASK;
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_RMII_SEL1_MASK;

    BLK_CTRL_WAKEUPMIX->MISC_IO_CTRL &= ~(1 << BLK_CTRL_WAKEUPMIX_MISC_IO_CTRL_ECAT_LINK_ACT0_POL_SHIFT);
    BLK_CTRL_WAKEUPMIX->MISC_IO_CTRL &= ~(1 << BLK_CTRL_WAKEUPMIX_MISC_IO_CTRL_ECAT_LINK_ACT1_POL_SHIFT);

    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG |= (1 << BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_EEPROM_SIZE_OPTION_SHIFT);

    SRC_GENERAL_REG->SRMASK &= ~(0x1 << SRC_GENERAL_SRMASK_ECAT_RSTO_MASK_SHIFT);

    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG |= (BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_PHY_OFFSET_VEC(2));
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_GLB_RST_MASK;
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG |= BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_GLB_EN_MASK;
}

/*!
 * @brief Function to copy core1 image to execution address.
 */
static void APP_CopyCore1Image(void)
{
#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* Calculate size of the image  - not required on MCUXpresso IDE. MCUXpresso copies the secondary core
       image to the target memory during startup automatically */
    uint32_t core1_image_size = get_core1_image_size();

  //  PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);

    /* Copy Secondary core application from FLASH to the target memory. */
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_CleanInvalidateDCache_by_Addr((void *)CORE1_BOOT_ADDRESS, core1_image_size);
#endif
#ifdef CORE1_IMAGE_FLUSH_CACHE
    CORE1_IMAGE_FLUSH_CACHE(CORE1_BOOT_ADDRESS, core1_image_size);
#endif
    memcpy((void *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_CleanInvalidateDCache_by_Addr((void *)CORE1_BOOT_ADDRESS, core1_image_size);
#endif
#ifdef CORE1_IMAGE_FLUSH_CACHE
    CORE1_IMAGE_FLUSH_CACHE(CORE1_BOOT_ADDRESS, core1_image_size);
#endif
#endif
}

UINT16 HW_Init(void)
{
    UINT32 intMask;
    UINT16 led_startus = 0;
    xbar_control_config_t xbaraConfig;

    /* Init board hardware. */
	BOARD_InitHardware();

    PRINTF("Start the SSC servo_motor example...\r\n");
	MU_ipc_shm_init((uint32_t)ipc_shm_section, IPC_SHM_SIZE);
	APP_CopyCore1Image();
	MU_Init(IPC_SHM_MU);
    /* Boot core 1. */
#if BOOT_CORE1_BY_MU
    MU_BootOtherCore(APP_MU, APP_CORE1_BOOT_MODE);
#else
    APP_BootCore1();
#endif

    /* Reset ecat PHY */
    SDK_DelayAtLeastUs(15000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    Ecat_KickOff();

    SDK_DelayAtLeastUs(90000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /*set port0 page register*/
    ECAT_EscMdioWrite(ECAT, 0x00, 31, 0x07);

    /*enable prot0 coustomized LED */
    ECAT_EscMdioRead(ECAT, 0x00, 19, &led_startus);
    ECAT_EscMdioWrite(ECAT, 0x00, 19, led_startus | (1 << 3));

    /*Set led1 to LINK100 and set led0 to ACK*/
    ECAT_EscMdioRead(ECAT, 0x00, 17, &led_startus);
    ECAT_EscMdioWrite(ECAT, 0x00, 17, led_startus | (1 << 3) | (1 << 5));

    /*set port1 page register*/
    ECAT_EscMdioWrite(ECAT, 0x01, 31, 0x07);

    /*enable prot1 coustomized LED */
    ECAT_EscMdioRead(ECAT, 0x01, 19, &led_startus);
    ECAT_EscMdioWrite(ECAT, 0x01, 19, led_startus | (1 << 3));

    /*Set led1 to LINK100 and set led0 to ACK*/
    ECAT_EscMdioRead(ECAT, 0x01, 17, &led_startus);
    ECAT_EscMdioWrite(ECAT, 0x01, 17, led_startus | (1 << 3) | (1 << 5));

    /*Disable phy eee mode*/
    ECAT_EscMdioWrite(ECAT, 0x00, 31, 4);
    ECAT_EscMdioWrite(ECAT, 0x00, 16, 0x4077);
    ECAT_EscMdioWrite(ECAT, 0x00, 31, 0);
    ECAT_EscMdioWrite(ECAT, 0x00, 13, 0x0007);
    ECAT_EscMdioWrite(ECAT, 0x00, 14, 0x003c);
    ECAT_EscMdioWrite(ECAT, 0x00, 13, 0x4007);
    ECAT_EscMdioWrite(ECAT, 0x00, 14, 0x0);
    ECAT_EscMdioWrite(ECAT, 0x00, 0, 0x1200);
    ECAT_EscMdioWrite(ECAT, 0x01, 31, 4);
    ECAT_EscMdioWrite(ECAT, 0x01, 16, 0x4077);
    ECAT_EscMdioWrite(ECAT, 0x01, 31, 0);
    ECAT_EscMdioWrite(ECAT, 0x01, 13, 0x0007);
    ECAT_EscMdioWrite(ECAT, 0x01, 14, 0x003c);
    ECAT_EscMdioWrite(ECAT, 0x01, 13, 0x4007);
    ECAT_EscMdioWrite(ECAT, 0x01, 14, 0x0);
    ECAT_EscMdioWrite(ECAT, 0x01, 0, 0x1200);

    /*config Sync0/1 IRQ*/
    XBAR_Init(kXBAR_DSC1);

    XBAR_SetSignalsConnection(kXBAR1_InputEcatSyncOut0, kXBAR1_OutputDma4MuxReq154);
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL1 = 0x0;
    xbaraConfig.activeEdge                   = kXBAR_EdgeRising;
    xbaraConfig.requestType                  = kXBAR_RequestInterruptEnable;
    XBAR_SetOutputSignalConfig(kXBAR1_OutputDma4MuxReq154, &xbaraConfig);
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL1 |= 0xff << 16;
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL1 |= 0xff << 8;
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL2 |= 3;
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL2 |= 3 << 4;

    XBAR_SetSignalsConnection(kXBAR1_InputEcatSyncOut1, kXBAR1_OutputDma4MuxReq155);
    xbaraConfig.activeEdge  = kXBAR_EdgeRising;
    xbaraConfig.requestType = kXBAR_RequestInterruptEnable;
    XBAR_SetOutputSignalConfig(kXBAR1_OutputDma4MuxReq155, &xbaraConfig);

    do
    {
        intMask = 0x93;
        HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0;
        HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while (intMask != 0x93);

    intMask = 0x00;

    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

    /* Set systick reload value to generate 1ms interrupt */
    SysTick_Config(SystemCoreClock / 1000U);

    /*Enable PDI IRQ*/
    EnableIRQ(ECAT_INT_IRQn);
    NVIC_EnableIRQ(XBAR1_CH0_CH1_IRQn);
    return 0;
}

void ECAT_INT_IRQHandler(void)
{
    PDI_Isr();

    SDK_ISR_EXIT_BARRIER;
}

/*config Sync0/1 IRQ*/
void XBAR1_CH0_CH1_IRQHandler(void)
{
    bool status;
    XBAR_GetOutputStatusFlag(kXBAR1_OutputDma4MuxReq154, &status);
    if (status)
    {
        XBAR_ClearOutputStatusFlag(kXBAR1_OutputDma4MuxReq154);
        Sync0_Isr();
    }

    XBAR_GetOutputStatusFlag(kXBAR1_OutputDma4MuxReq155, &status);
    if (status)
    {
        XBAR_ClearOutputStatusFlag(kXBAR1_OutputDma4MuxReq155);
        Sync1_Isr();
    }

    SDK_ISR_EXIT_BARRIER;
}

void HW_Release(void)
{

}

void SysTick_Handler(void)
{
    /* Clear interrupt flag.*/
#if ECAT_TIMER_INT
    ECAT_CheckTimer();
#endif
    EcatTimerCnt++;
    SDK_ISR_EXIT_BARRIER;
}

UINT16 HW_GetTimer(void)
{
    return EcatTimerCnt;
}

void HW_ClearTimer(void)
{
    EcatTimerCnt = 0;
}

void ENABLE_ESC_INT(void)
{
    NVIC_EnableIRQ(ECAT_INT_IRQn);
    NVIC_EnableIRQ(XBAR1_CH0_CH1_IRQn);
    NVIC_EnableIRQ(GPT1_IRQn);
	NVIC_EnableIRQ(IPC_SHM_MU_IRQn);
}

void DISABLE_ESC_INT(void)
{
    NVIC_DisableIRQ(XBAR1_CH0_CH1_IRQn);
    NVIC_DisableIRQ(ECAT_INT_IRQn);
    NVIC_DisableIRQ(GPT1_IRQn);
	NVIC_DisableIRQ(IPC_SHM_MU_IRQn);
}

void HW_SetLed(UINT8 RunLed, UINT8 ErrorLed)
{
}

/*${function:end}*/
