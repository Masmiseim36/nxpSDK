/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include "board.h"
#include "fsl_sai_sdma.h"
#include "fsl_debug_console.h"
#include "fsl_card.h"
#include "ff.h"
#include "diskio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include <string.h>

#include "fsl_wm8960.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI I2S2
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_I2C I2C2
#define DEMO_SAI_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / ((CLOCK_GetDiv(kCLOCK_Sai2PreDiv) + 1U) * (CLOCK_GetDiv(kCLOCK_Sai2Div) + 1U)))
#define DEMO_I2C_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (CLOCK_GetDiv(kCLOCK_PerclkDiv) + 1U))
#define DEMO_SAI_IRQ SAI2_IRQn
#define DMA_IRQ SDMA_IRQn
#define EXAMPLE_DMA SDMAARM
#define EXAMPLE_TX_CHANNEL (2)
#define EXAMPLE_RX_CHANNEL (1)
#define SAI_TX_SOURCE (38)
#define SAI_RX_SOURCE (37)

#define I2C_RELEASE_SDA_GPIO GPIO1
#define I2C_RELEASE_SDA_PIN 31U
#define I2C_RELEASE_SCL_GPIO GPIO1
#define I2C_RELEASE_SCL_PIN 30U
#define I2C_RELEASE_BUS_COUNT 100U
#define OVER_SAMPLE_RATE (384U)
#define SAMPLE_RATE (kSAI_SampleRate16KHz)
#define BUFFER_SIZE (1024)
#define BUFFER_NUM (4)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
void BOARD_EnableSaiMclkOutput(bool enable);
void SAI_UserIRQHandler(void);
static void txCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);
static void rxCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
const clock_audio_pll_config_t g_ccmConfigAudioPll = {
    .loopDivider = 30, .postDivider = 1, .numerator = 72, .denominator = 100};

AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t txHandle, 4) = {0};
sdma_handle_t dmaTxHandle = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t txContext, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t rxHandle, 4) = {0};
sdma_handle_t dmaRxHandle = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t rxContext, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4) = {0};
#if defined(DEMO_CODEC_WM8960)
wm8960_handle_t codecHandle = {0};
#elif defined(DEMO_CODEC_DA7212)
da7212_handle_t codecHandle = {0};
#else
sgtl_handle_t codecHandle = {0};
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {{0, 0, kI2C_Write, 0, 0, NULL, 0}, 0, 0, NULL, NULL};
#endif

volatile uint32_t beginCount = 0;
volatile uint32_t receiveCount = 0;
volatile uint32_t sendCount = 0;

/* static values for fatfs */
AT_NONCACHEABLE_SECTION(static FATFS g_fileSystem) = {0}; /* File system object */
AT_NONCACHEABLE_SECTION(static FIL g_fileObject) = {0};   /* File object */
AT_NONCACHEABLE_SECTION(static BYTE work[_MAX_SS]) = {0};

static SemaphoreHandle_t taskFinish = NULL;
static SemaphoreHandle_t transferFinish = NULL;
static SemaphoreHandle_t emptyblock = NULL;
static SemaphoreHandle_t fullblock = NULL;

/* Shell variables */
char cmd[10] = {0};      /* Command string */
char filepath[80] = {0}; /* File path string */
uint8_t cmdLen = 0;
uint8_t pathLen = 0;
bool duplexflag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
extern void USDHC1_DriverIRQHandler(void);
extern void GPIO_IRQ_Handler(void);


void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI2_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI2_MCLK_DIR_MASK);
    }
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;

    pin_config.direction = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    pin_config.interruptMode = kGPIO_NoIntmode;

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

/*Provides own implementation of vApplicationIRQHandler() to call SystemIrqHandler()
 *directly without saving the FPU registers on interrupt entry.
 */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
    SystemIrqHandler(ulICCIAR);
}
void SdcardWrite_Task(void *pvParameters);
void SdcardRead_Task(void *pvParameters);
void Playback_Task(void *pvParameters);
void Record_Task(void *pvParameters);
void Shell_Task(void *pvParameters);

static void wav_header(uint8_t *header, uint32_t sampleRate, uint32_t bitsPerFrame, uint32_t fileSize)
{
    uint32_t totalDataLen = fileSize - 8U;
    uint32_t audioDataLen = fileSize - 44U;
    uint32_t byteRate = sampleRate * (bitsPerFrame / 8U) * 2U;
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';
    header[4] = (totalDataLen & 0xff); /* file-size (equals file-size - 8) */
    header[5] = ((totalDataLen >> 8U) & 0xff);
    header[6] = ((totalDataLen >> 16U) & 0xff);
    header[7] = ((totalDataLen >> 24U) & 0xff);
    header[8] = 'W'; /* Mark it as "WAVE" */
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';
    header[12] = 'f'; /* Mark the 'fmt ' chunk */
    header[13] = 'm';
    header[14] = 't';
    header[15] = ' ';
    header[16] = 16; /* Size of fmt chunk, always 16 */
    header[17] = 0;
    header[18] = 0;
    header[19] = 0;
    header[20] = 1; /* PCM format is 1 */
    header[21] = 0;
    header[22] = 2; /* Data channels */
    header[23] = 0;
    header[24] = (sampleRate & 0xff);
    header[25] = ((sampleRate >> 8U) & 0xff);
    header[26] = ((sampleRate >> 16U) & 0xff);
    header[27] = ((sampleRate >> 24U) & 0xff);
    header[28] = (byteRate & 0xff);
    header[29] = ((byteRate >> 8U) & 0xff);
    header[30] = ((byteRate >> 16U) & 0xff);
    header[31] = ((byteRate >> 24U) & 0xff);
    header[32] = (2 * bitsPerFrame / 8); /* Bytes for a frame */
    header[33] = 0;
    header[34] = 16; /* Bits per frame */
    header[35] = 0;
    header[36] = 'd'; /* Data chunk */
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';
    header[40] = (audioDataLen & 0xff); /* Data size, except the file header */
    header[41] = ((audioDataLen >> 8) & 0xff);
    header[42] = ((audioDataLen >> 16) & 0xff);
    header[43] = ((audioDataLen >> 24) & 0xff);
}

static void txCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t xHigherPriorityTaskWoken;

    sendCount++;
    /* Release the semaphore */
    xSemaphoreGiveFromISR(emptyblock, &xHigherPriorityTaskWoken);

    /* If a higher priority task waken by the seamphore, do a context switch */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    if (sendCount == beginCount)
    {
        if (duplexflag)
        {
            xSemaphoreGiveFromISR(transferFinish, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        SAI_TransferAbortSendSDMA(base, handle);
        sendCount = 0;
    }
}

static void rxCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t xHigherPriorityTaskWoken;

    receiveCount++;
    /* Release the semaphore */
    xSemaphoreGiveFromISR(fullblock, &xHigherPriorityTaskWoken);

    /* If a higher priority task waken by the seamphore, do a context switch */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    if (receiveCount == beginCount)
    {
        SAI_TransferAbortReceiveSDMA(base, handle);
        receiveCount = 0;
    }
}

void Init_Task(void *pvParameters)
{
    sai_config_t config;
    uint32_t mclkSourceClockHz = 0U;
    sai_transfer_format_t format;
    sdma_config_t dmaConfig = {0};
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
#else
    i2c_master_config_t i2cConfig = {0};
#endif
    uint32_t i2cSourceClock;
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    char input;

    GIC_SetPriority(SD_HOST_IRQ, 21U);
    GIC_EnableIRQ(USDHC1_IRQn);
    GIC_SetPriority(DMA_IRQ, 21U);
    GIC_SetPriority(DEMO_SAI_IRQ, 21U);

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    SDMA_GetDefaultConfig(&dmaConfig);
    SDMA_Init(EXAMPLE_DMA, &dmaConfig);
    SDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL, &txContext);
    SDMA_CreateHandle(&dmaRxHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL, &rxContext);

    /* Init SAI module */
    /*
     * config.masterSlave = kSAI_Master;
     * config.mclkSource = kSAI_MclkSourceSysclk;
     * config.protocol = kSAI_BusLeftJustified;
     * config.syncMode = kSAI_ModeAsync;
     * config.mclkOutputEnable = true;
     */
    SAI_TxGetDefaultConfig(&config);
    SAI_TxInit(DEMO_SAI, &config);

    /* Initialize SAI Rx */
    SAI_RxGetDefaultConfig(&config);
    SAI_RxInit(DEMO_SAI, &config);

    /* Enable SAI error interrupt */
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    GIC_EnableIRQ(DEMO_SAI_IRQ);

    /* Configure the audio format */
    format.bitWidth = kSAI_WordWidth16bits;
    format.channel = 0U;
    format.sampleRate_Hz = SAMPLE_RATE;
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
#else
    format.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    format.protocol = config.protocol;
    format.stereo = kSAI_Stereo;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif

    /* Configure Sgtl5000 I2C */
    codecHandle.base = DEMO_I2C;
    codecHandle.i2cHandle = &i2cHandle;
    i2cSourceClock = DEMO_I2C_CLK_FREQ;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#else
    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#endif

#if defined(DEMO_CODEC_WM8960)
    WM8960_Init(&codecHandle, NULL);
    WM8960_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
#elif defined(DEMO_CODEC_DA7212)
    DA7212_Init(&codecHandle, NULL);
    DA7212_ConfigAudioFormat(&codecHandle, format.sampleRate_Hz, format.masterClockHz, format.bitWidth);
    DA7212_ChangeOutput(&codecHandle, kDA7212_Output_HP);
#else
    /* Use default settings for sgtl5000 */
    SGTL_Init(&codecHandle, NULL);
    /* Configure codec format */
    SGTL_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
#endif

    /* Set priority of each channel */
    SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_RX_CHANNEL, 2U);
    SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_TX_CHANNEL, 2U);

    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &txHandle, txCallback, NULL, &dmaTxHandle, SAI_TX_SOURCE);
    SAI_TransferRxCreateHandleSDMA(DEMO_SAI, &rxHandle, rxCallback, NULL, &dmaRxHandle, SAI_RX_SOURCE);

    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
    SAI_TransferTxSetFormatSDMA(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
    SAI_TransferRxSetFormatSDMA(DEMO_SAI, &rxHandle, &format, mclkSourceClockHz, format.masterClockHz);

/* If there is SDCard, Initialize SDcard and Fatfs */
#if _USE_MKFS
    PRINTF(
        "\r\nAfter card inserted, Please choose if card need to be formated, y means format, n means no need "
        "format.\r\n");
    input = GETCHAR();
    PUTCHAR(input);
    if (input == 'y')
    {
        PRINTF("\r\n");
        PRINTF("The time may be long if the card capacity is big.\r\n");
        if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof(work)))
        {
            PRINTF("Make file system failed.\r\n");
            configASSERT(pdFALSE);
        }
        PRINTF("\r\nFile system ready......\r\n");
    }
    else if (input == 'n')
    {
        PRINTF("\r\nNo need to format sdcard...\r\n");
        PRINTF("\r\n");
    }
    else
    {
        PRINTF("\r\nInvalid input! Please input y or n!\r\n");
    }

#endif /* _USE_MKFS */

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        configASSERT(pdFALSE);
    }

#if (_FS_RPATH >= 2U)
    if (f_chdrive((char const *)driverNumberBuffer))
    {
        PRINTF("Change drive failed.\r\n");
        configASSERT(pdFALSE);
    }
#endif

    /* Create shell task */
    if (xTaskCreate(Shell_Task, "Shell Task", 256, NULL, tskIDLE_PRIORITY + 2U, NULL) != pdPASS)
    {
        PRINTF("Shell Task creation failed!.\r\n");
    }
    /* Delete the init task */
    vTaskDelete(NULL);
}

void Shell_Task(void *pvParameters)
{
    char input;
    bool cmdflag = false;
    bool pathflag = false;

    /* Create a task finish binary semaphore */
    taskFinish = xSemaphoreCreateBinary();

    if (taskFinish == NULL)
    {
        PRINTF("\r\nSeamphore create failed, quit...\r\n");
        return;
    }

    while (1)
    {
        cmdLen = 0;
        pathLen = 0;
        memset(cmd, 0, sizeof(cmd));
        memset(filepath, 0, sizeof(filepath));
        cmdflag = false;
        pathflag = false;
        duplexflag = false;
        PRINTF("\rCommand including like below:\r\n");
        PRINTF("\rplayback <filepath> Playback the wav file, make sure the path existed.\r\n");
        PRINTF("\rrecord <filepath> Record a 10s audio data, save to the file path in sdcard as a wav file\r\n");
        PRINTF("\rduplex Playback the data recorded by sai now\r\n");
        PRINTF("\r\n");

        /* Get the cmd */
        while (!cmdflag)
        {
            input = GETCHAR();
            PUTCHAR(input);
            if (input == ' ')
            {
                cmdflag = true;
            }
            else if ((input == '\r') || (input == '\n'))
            {
                cmdflag = true;
                pathflag = true;
            }
            else
            {
                cmd[cmdLen] = input;
                cmdLen++;
            }
        }

        /* Get the file path */
        while (!pathflag)
        {
            input = GETCHAR();
            PUTCHAR(input);
            if ((input == '\r') || (input == '\n'))
            {
                pathflag = true;
            }
            else
            {
                filepath[pathLen] = input;
                pathLen++;
            }
        }
        PRINTF("\r\n");

        /* Compare the command */
        if (strcmp(cmd, "playback") == 0)
        {
            if (xTaskCreate(Playback_Task, "Playback Task", 512, NULL, tskIDLE_PRIORITY + 4U, NULL) != pdPASS)
            {
                PRINTF("\r\nPlyback Task creation failed!.\r\n");
            }
        }
        else if (strcmp(cmd, "record") == 0)
        {
            if (xTaskCreate(Record_Task, "Record Task", 512, NULL, tskIDLE_PRIORITY + 4U, NULL) != pdPASS)
            {
                PRINTF("\r\nRecord Task creation failed!.\r\n");
            }
        }
        else if (strcmp(cmd, "duplex") == 0)
        {
            duplexflag = true;
            if (xTaskCreate(Record_Task, "Duplex Record Task", 512, NULL, tskIDLE_PRIORITY + 4U, NULL) != pdPASS)
            {
                PRINTF("\r\nDuplex Task creation failed!.\r\n");
            }
        }
        else
        {
            PRINTF("\r\n Invalid input command!\r\n");
            continue;
        }

        /* Wait for the task finished */
        xSemaphoreTake(taskFinish, 0xFFFFFFFFU);
    }
}

void SdcardRead_Task(void *pvParameters)
{
    uint32_t index = 0;
    unsigned int bytesRead = 0;

    while (index < beginCount)
    {
        xSemaphoreTake(emptyblock, 0xFFFFFFFFU);
        /* Write data into sdcard */
        f_read(&g_fileObject, audioBuff + (index % BUFFER_NUM) * BUFFER_SIZE, BUFFER_SIZE, &bytesRead);
        index++;
        xSemaphoreGive(fullblock);
    }

    vTaskDelete(NULL);
}

void Playback_Task(void *pvParameters)
{
    unsigned int bytesRead = 0;
    uint8_t header[44] = {0};
    uint32_t dataSize = 0;
    uint32_t playbackCount = 0;
    uint32_t bufferIndex = 0;
    FRESULT error;
    sai_transfer_t xfer = {0};

    PRINTF("\r\n Playback task begin...\r\n");
    fullblock = xSemaphoreCreateCounting(BUFFER_NUM, 0);
    if (fullblock == NULL)
    {
        PRINTF("\r\nCreate seamphore failed in playback task, quit...\r\n");
        return;
    }

    emptyblock = xSemaphoreCreateCounting(BUFFER_NUM, BUFFER_NUM);
    if (emptyblock == NULL)
    {
        PRINTF("\r\nCreate seamphore failed in playback task, quit...\r\n");
        return;
    }

    /* Open file */
    error = f_open(&g_fileObject, _T(filepath), (FA_READ));
    if (error)
    {
        PRINTF("Open file failed.\r\n");
        return;
    }

    /* Get the file header */
    error = f_read(&g_fileObject, header, 44U, &bytesRead);

    if ((error) || (bytesRead != 44U))
    {
        PRINTF("\r\nRead wav file header failed...\r\n");
        return;
    }

    dataSize = *(uint32_t *)(header + 40U);
    beginCount = dataSize / BUFFER_SIZE;
    playbackCount = beginCount;
    xfer.dataSize = BUFFER_SIZE;

    /* Create sdcard read task */
    if (xTaskCreate(SdcardRead_Task, "Sdcard Read task", 256, NULL, tskIDLE_PRIORITY + 4U, NULL) != pdPASS)
    {
        PRINTF("Sdcard read Task creation failed!.\r\n");
        return;
    }

    /* Do a loop to transfer data */
    while (playbackCount > 0)
    {
        /* Wait for a semaphore */
        xSemaphoreTake(fullblock, 0xFFFFFFFFU);

        /* Start a transfer */
        xfer.data = audioBuff + bufferIndex * BUFFER_SIZE;
        SAI_TransferSendSDMA(DEMO_SAI, &txHandle, &xfer);
        bufferIndex = (bufferIndex + 1U) % BUFFER_NUM;
        playbackCount--;
    }

    /* Task finished, release the task seamphore and delete task */
    xSemaphoreGive(taskFinish);
    vSemaphoreDelete(fullblock);
    vSemaphoreDelete(emptyblock);

    if (f_close(&g_fileObject))
    {
        PRINTF("\r\nClose file failed.\r\n");
    }

    vTaskDelete(NULL);
}

void SdcardWrite_Task(void *pvParameters)
{
    uint32_t index = 0;
    unsigned int bytesWritten = 0;

    while (index < beginCount)
    {
        xSemaphoreTake(fullblock, 0xFFFFFFFFU);
        /* Write data into sdcard */
        f_write(&g_fileObject, audioBuff + (index % BUFFER_NUM) * BUFFER_SIZE, BUFFER_SIZE, &bytesWritten);
        index++;
        xSemaphoreGive(emptyblock);
    }
    xSemaphoreGive(transferFinish);
    vTaskDelete(NULL);
}

void DuplexPlayback_Task(void *pvParameters)
{
    uint32_t index = 0;
    uint32_t buffOffset = 0;
    sai_transfer_t xfer = {0};

    xfer.dataSize = BUFFER_SIZE;
    while (index < beginCount)
    {
        xSemaphoreTake(fullblock, 0xFFFFFFFFU);
        buffOffset = (index % BUFFER_NUM) * BUFFER_SIZE;
        xfer.data = audioBuff + buffOffset;
        SAI_TransferSendSDMA(DEMO_SAI, &txHandle, &xfer);
        index++;
    }

    vTaskDelete(NULL);
}

void Record_Task(void *pvParameters)
{
    unsigned int bytesWritten = 0;
    uint8_t header[44] = {0};
    uint32_t i = 0;
    uint32_t fileSize = 10 * SAMPLE_RATE * 2U * 2U + 44U;
    FRESULT error;
    sai_transfer_t xfer = {0};

    PRINTF("\r\nRecord task begin...\r\n");
    receiveCount = 0;
    sendCount = 0;
    fullblock = xSemaphoreCreateCounting(BUFFER_NUM, 0);
    if (fullblock == NULL)
    {
        PRINTF("\r\nCreate seamphore failed in record task, quit...\r\n");
        return;
    }

    emptyblock = xSemaphoreCreateCounting(BUFFER_NUM, BUFFER_NUM);
    if (emptyblock == NULL)
    {
        PRINTF("\r\nCreate seamphore failed in record task, quit...\r\n");
        return;
    }

    transferFinish = xSemaphoreCreateBinary();
    if (transferFinish == NULL)
    {
        PRINTF("\r\nCreate seamphore failed in record task, quit...\r\n");
        return;
    }

    /* Open file */
    if (!duplexflag)
    {
        error = f_open(&g_fileObject, _T(filepath), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
        if (error)
        {
            if (error == FR_EXIST)
            {
                PRINTF("File exists.\r\n");
            }
            else
            {
                PRINTF("Open file failed.\r\n");
                return;
            }
        }

        /* Write the data into the sdcard */
        wav_header(header, SAMPLE_RATE, 16, fileSize);

        /* Write the wav header */
        error = f_write(&g_fileObject, header, 44U, &bytesWritten);
        if ((error) || (bytesWritten != 44))
        {
            PRINTF("Write file failed. \r\n");
            return;
        }
    }
    /* Start to record */
    beginCount = 10 * SAMPLE_RATE * 2U * 2U / BUFFER_SIZE;
    xfer.dataSize = BUFFER_SIZE;

    if (!duplexflag)
    {
        if (xTaskCreate(SdcardWrite_Task, "Sdcard Write task", 256, NULL, tskIDLE_PRIORITY + 4U, NULL) != pdPASS)
        {
            PRINTF("Sdcard write Task creation failed!.\r\n");
            return;
        }
    }
    else
    {
        if (xTaskCreate(DuplexPlayback_Task, "Duplex Playback task", 256, NULL, tskIDLE_PRIORITY + 4U, NULL) != pdPASS)
        {
            PRINTF("Duplex Playback Task creation failed!.\r\n");
            return;
        }
    }

    /* Start record */
    while (i < beginCount)
    {
        xSemaphoreTake(emptyblock, 0xFFFFFFFFU);
        xfer.data = audioBuff + (i % BUFFER_NUM) * BUFFER_SIZE;
        SAI_TransferReceiveSDMA(DEMO_SAI, &rxHandle, &xfer);
        i++;
    }

    /* Wait all transfer finish */
    xSemaphoreTake(transferFinish, 0xFFFFFFFFU);

    /* Task finished, release the task seamphore and delete task */
    xSemaphoreGive(taskFinish);
    vSemaphoreDelete(fullblock);
    vSemaphoreDelete(transferFinish);

    /* Close the file */
    if (f_close(&g_fileObject))
    {
        PRINTF("\r\nClose file failed.\r\n");
    }

    vTaskDelete(NULL);
}

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_USDHC_ConfigurePins();
    BOARD_BootClockRUN();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_EnableSaiMclkOutput(true);
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Enable PLL4 368.64M */
    CLOCK_InitAudioPll(&g_ccmConfigAudioPll);
    /* Set SAI source to PLL4*/
    CLOCK_SetMux(kCLOCK_Sai2Mux, 2);
    /* Set SAI pre-divider to 6 */
    CLOCK_SetDiv(kCLOCK_Sai2PreDiv, 5U);
    /* Set SAI divider to 10 */
    CLOCK_SetDiv(kCLOCK_Sai2Div, 9U);
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);

    /* Enable SPBA */
    CLOCK_EnableClock(kCLOCK_Spba);

    /* Install IRQ Handler */
    SystemInitIrqTable();

    /* Install user defined SAI IRQ handler */
    SystemInstallIrqHandler(SAI2_IRQn, (system_irq_handler_t)SAI_UserIRQHandler, NULL);
    SystemInstallIrqHandler(USDHC1_IRQn, (system_irq_handler_t)(uint32_t)USDHC1_DriverIRQHandler, NULL);
    SystemInstallIrqHandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)GPIO_IRQ_Handler, NULL);
    PRINTF("SAI example started!\n\r");
    if (xTaskCreate(Init_Task, "Demo Init Task", 1024, NULL, tskIDLE_PRIORITY + 5U, NULL) != pdPASS)
    {
        PRINTF("Init Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Start scheduling. */
    vTaskStartScheduler();
    for (;;)
        ;
}

void SAI_UserIRQHandler(void)
{
    /* Clear the FEF flag */
    if (SAI_TxGetStatusFlag(DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
        SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
    }

    if (SAI_RxGetStatusFlag(DEMO_SAI) & kSAI_FIFOErrorFlag)
    {
        SAI_RxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
        SAI_RxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
    }
}
