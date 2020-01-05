/*
 * Copyright 2016-2018, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "fsl_gpio.h"
#include "fsl_adc16.h"
#include "fsl_debug_console.h"

#include "HAL_I2C_driver.h"
#include "HAL_timer_driver.h"
#include "ntag_driver_intern.h"
#include "ntag_bridge.h"
#include "nfc_device.h"
#include "ndef_message.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
// Version of Board and FW, change if needed
// Notice that only one char per define is allowed

#define CHAR_BOARD_1 'K';
#define CHAR_BOARD_2 '6';
#define CHAR_BOARD_3 '4';

#define CHAR_FW_MAJ '2'
#define CHAR_FW_MIN '0'

// Write back the timing as a NDEF Message(useful when you have no display
// to look at the timing and want to read it out later
//#define WRITE_TIME_VIA_NDEF

// Following I2C NFC devices are supported:
// NTAG I2C (standard)
#define NTAG_I2C_PLUS // 2kB version

// Following Boards are supported:
// FRDM_K82F has to be defined in project properties defined symbols

// Following Polling methods are supported
// Polling (standard)
// Interrupted, switched via following define instead of Polling
#define INTERRUPT

#ifdef I2C_CMSIS
#define NTAG_I2C_MASTER_BASEADDR &Driver_I2C0
#define I2C_MASTER_IRQ I2C0_IRQn
#endif

#ifdef I2C_FSL
#define NTAG_I2C_MASTER_BASEADDR I2C0
#define I2C_MASTER_CLK_SRC I2C0_CLK_SRC
#endif

#define SW1_PRESSED (!GPIO_PinRead(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN))
#define SW2_PRESSED (!GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
#define SW3_PRESSED (SW1_PRESSED && SW2_PRESSED)

/* define the ADC input pin connected to the VOUT pin on NTAG I2C plus chip*/
#define DEMO_ADC16_BASE ADC1
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL 14U

typedef enum LED__
{
    LEDOFF = 0,
    REDLED,
    BLUELED,
    GREENLED
} LED;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void wait_for_RF_write_in_SRAM(void);
void LED_Demo(void);
void send_VersionInfo(void);
void Setup(void);
void LED_blink(LED led);
void SwitchLEDs(LED led);
void check_Buttons(uint8_t *Buttons);
short int write_back_Time_as_NDEF(int time1, int time2);
bool reset_AAR(void);
void factory_reset_Tag(void);
short int speedTest(void);
void Get_Volt(uint8_t *volt);
static void createCRCTableList(void);
static uint32_t calculateCRC(uint32_t crc, uint8_t *data, uint32_t length);

/*******************************************************************************
 * Variable
 ******************************************************************************/
/* for making led demo init only one time */
bool init_led_demo = false;

static NFC_HANDLE_T ntag_handle;
static uint8_t sram_buf[NTAG_MEM_SRAM_SIZE];
adc16_channel_config_t adc16ChannelConfigStruct;

/* CRC table list */
static uint32_t s_CRCTableList[256];

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t I2C0_GetFreq(void)
{
    return CLOCK_GetFreq(I2C0_CLK_SRC);
}

uint32_t I2C1_GetFreq(void)
{
    return CLOCK_GetFreq(I2C1_CLK_SRC);
}

uint32_t I2C2_GetFreq(void)
{
    return CLOCK_GetFreq(I2C2_CLK_SRC);
}


/*
 * main Program
 * @return should never return
 */
int main(void)
{
    uint8_t err;
    uint8_t Buttons = 0;
    char command;

    /* Initialize Board hardware and peripherals */
    Setup();

    createCRCTableList();

    /* Print a note to terminal. */
    PRINTF("\r\n\r\n NTAG I2C demo example\r\n\r\n");
#ifdef I2C_FSL
    PRINTF("\r\n\r\n FSL I2C Driver \r\n");
#endif
#ifdef I2C_CMSIS
    PRINTF("\r\n\r\n CMSIS I2C Driver \r\n");
#endif

    HAL_Timer_delay_ms(1000);
    /* If both buttons are pressed on start-up, reset the tag memory to the default (smart poster NDEF) */
    check_Buttons(&Buttons);
    if (Buttons == 0x07)
        factory_reset_Tag();

#ifdef INTERRUPT
    /* If Interrupted Mode is enabled set the FD Pin to react on the SRAM */
    NFC_SetFDOffFunction(ntag_handle, I2C_LAST_DATA_READ_OR_WRITTEN_OR_RF_SWITCHED_OFF_11b);
    NFC_SetFDOnFunction(ntag_handle, DATA_READY_BY_I2C_OR_DATA_READ_BY_RF_11b);
#endif

    /* Main Loop */
    while (1)
    {
        /* Enable Passthrough Mode RF->I2C */
        err = NFC_SetTransferDir(ntag_handle, RF_TO_I2C);
        HAL_Timer_delay_ms(5);
        err = NFC_SetPthruOnOff(ntag_handle, TRUE);

        /* wait for RF Write in the SRAM terminator page */
        wait_for_RF_write_in_SRAM();
        /* get the SRAM Data */
        memset(sram_buf, 0, NFC_MEM_SRAM_SIZE);
        err = NFC_ReadBytes(ntag_handle, NFC_MEM_ADDR_START_SRAM, sram_buf, NFC_MEM_SRAM_SIZE);
        /* Check for requested action */
        command = sram_buf[NFC_MEM_SRAM_SIZE - 4];
        switch (command)
        {
            case 'S':
            {
                /* run speed test */
                PRINTF("\r\nPerforming the speed test");
                SwitchLEDs(LEDOFF);
                err = speedTest();

                /* let the Green LED blink if Speedtest was successful */
                if (err)
                {
                    SwitchLEDs(REDLED);
                    HAL_Timer_delay_ms(100);
                    SwitchLEDs(LEDOFF);
                    PRINTF("\r\nSpeed test failed!");
                }
                else
                {
                    LED_blink(GREENLED);
                }
                break;
            }
            case 'L':
            {
                LED_Demo();
                break;
            }
            case 'V':
            {
                send_VersionInfo();
                break;
            }
            case 'R':
            {
                PRINTF("\r\nFactory Reset");
                factory_reset_Tag();
                break;
            }
        }
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * Implements the LED Demo app functionality
 */
void LED_Demo(void)
{
    uint8_t err                 = 0;
    static LED led_state        = LEDOFF;
    static uint8_t button_state = 0;
    /* make a init only one time */
    if (!init_led_demo)
    {
        /* Init output LEDs. */
        LED_RED_INIT(1);
        LED_GREEN_INIT(1);
        LED_BLUE_INIT(1);
        init_led_demo = true;
        PRINTF("\r\nLED Demo");
    }

    /* turn on the according LED */
    LED number = (LED)(sram_buf[NFC_MEM_SRAM_SIZE - 3] - '0');
    SwitchLEDs(number);
    if (led_state != number)
    {
        PRINTF("\r\nLED status: %i", number);
        led_state = number;
    }

    /* show energy harvesting voltage */
    uint8_t Volt[2];
    Get_Volt(Volt);

    sram_buf[NFC_MEM_SRAM_SIZE - 8] = Volt[0];
    sram_buf[NFC_MEM_SRAM_SIZE - 7] = Volt[1];

    /* Check pressed buttons */
    uint8_t Buttons = 0;
    check_Buttons(&Buttons);
    if (button_state != Buttons)
    {
        PRINTF("\r\nButton status: %i", Buttons);
        button_state = Buttons;
    }

    sram_buf[NFC_MEM_SRAM_SIZE - 2] = Buttons;
    /* write back Data */
    err += NFC_SetTransferDir(ntag_handle, I2C_TO_RF);
    err += NFC_WriteBytes(ntag_handle, NFC_MEM_ADDR_START_SRAM, sram_buf, NFC_MEM_SRAM_SIZE);

    /* waiting till RF has read */
    err += NFC_WaitForEvent(ntag_handle, NTAG_EVENT_RF_READ_SRAM, SRAM_TIMEOUT, FALSE);
    if (err)
    {
        PRINTF("\r\nAn error occurred during I2C<->NTAG transfer or no NFC field present");
    }
}

//---------------------------------------------------------------------

void ADC_Init(void)
{
    adc16_config_t adc16ConfigStruct;

    /*
     * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
     * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
     * adc16ConfigStruct.enableAsynchronousClock = true;
     * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
     * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
     * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
     * adc16ConfigStruct.enableHighSpeed = false;
     * adc16ConfigStruct.enableLowPower = false;
     * adc16ConfigStruct.enableContinuousConversion = false;
     */
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        /*        PRINTF("ADC16_DoAutoCalibration() Done.\r\n"); */
    }
    else
    {
        /*        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n"); */
    }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

    adc16ChannelConfigStruct.channelNumber                        = DEMO_ADC16_USER_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
}

/*
 * Get measured voltage from ADC
 * @param Buffer Volt in encoded format, should return 0 when not implemented
 */
void Get_Volt(uint8_t *volt)
{
    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    while (0U ==
           (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
    {
    }
    uint16_t ADCValue  = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    uint16_t ADCresult = 0x27FD80 / ADCValue; /* 0x280 {=3,3V on display} * 0xFFF {=3,3V ADCValue} = 0x27FD80 */
    volt[0]            = ADCresult & 0x00FF;
    volt[1]            = (ADCresult & 0xFF00) >> 8;
    /*    PRINTF("\r\nADC value : 0x%04X, ADC result : 0x%04X", ADCValue, ADCresult); */
}
void HW_getTemp(uint8_t Buffer[])
{
#if defined(Board_Demo_v1_4) || defined(Board_NTAG_I2C_Explorer)
    uint8_t RX_Buffer[HAL_I2C_RX_RESERVED_BYTES + 2];
    uint8_t TX_Buffer[HAL_I2C_TX_RESERVED_BYTES + 1];

    /* get temperature value from sensor */
    TX_Buffer[HAL_I2C_TX_RESERVED_BYTES + 0] = 0;
    HAL_I2C_SendBytes(i2cHandleMaster, TEMP_I2C_ADDRESS >> 1, TX_Buffer, 1);
    HAL_I2C_RecvBytes(i2cHandleMaster, TEMP_I2C_ADDRESS >> 1, RX_Buffer, 2);

    Buffer[0] = RX_Buffer[HAL_I2C_RX_RESERVED_BYTES + 0];
    Buffer[1] = RX_Buffer[HAL_I2C_TX_RESERVED_BYTES + 1];
#else
    Buffer[0] = 0;
    Buffer[1] = 0;
    return;
#endif
}
/*
 * Resets the memory to its default values
 */
#if defined(NTAG_I2C_PLUS)
void factory_reset_Tag(void)
{
    PRINTF("\r\nFactory Reset of Tag memory");
    SwitchLEDs(REDLED);
    HAL_Timer_delay_ms(100);

    /* reset default eeprom memory values (smart poster) */
    NFC_WriteBytes(ntag_handle, NTAG_MEM_ADRR_I2C_ADDRESS, Default_BeginingOfMemory, Default_BeginingOfMemory_length);

    /* reset pages from 8 to 56 */
    uint8_t page = 8;
    while (page < 56)
    {
        NFC_WriteBlock(ntag_handle, page, Null_Block, NTAG_I2C_BLOCK_SIZE);
        page++;
    }
    /* reset pages 56,57,58 */
    NFC_WriteBlock(ntag_handle, 56, Default_Page_56, NTAG_I2C_BLOCK_SIZE);
    NFC_WriteBlock(ntag_handle, 57, Default_Page_57, NTAG_I2C_BLOCK_SIZE);
    NFC_WriteBlock(ntag_handle, 58, Default_Page_58, NTAG_I2C_BLOCK_SIZE);

    SwitchLEDs(GREENLED);
    HAL_Timer_delay_ms(100);
}
#else
void factory_reset_Tag(void)
{
    /* config registers memory address for NTAG I2C 1K version */
    uint8_t config = NTAG_MEM_BLOCK_CONFIGURATION_1k;

    /* default config register values as defined by the datasheet */
    uint8_t default_config_reg[NTAG_I2C_BLOCK_SIZE] = {0x01, 0x00, 0xF8, 0x48, 0x08, 0x01, 0x00, 0x00,
                                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    SwitchLEDs(REDLED);
    HAL_Timer_delay_ms(100);

    /* reset default eeprom memory values (smart poster) */
    NFC_WriteBytes(ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY, Default_NDEF_Message, Default_NDEF_Message_length);

    /* reset default config registers */
    NTAG_WriteBlock(ntag_handle, config, default_config_reg, NTAG_I2C_BLOCK_SIZE);

    SwitchLEDs(GREENLED);
    HAL_Timer_delay_ms(100);
}
#endif

/*
 * Waits till RF has written in the terminater page of the SRAM
 */
void wait_for_RF_write_in_SRAM(void)
{
    uint32_t counter = HAL_Timer_getTime_ms();

    /* wait for RF Write in the SRAM terminator page */
    while (NFC_WaitForEvent(ntag_handle, NTAG_EVENT_RF_WROTE_SRAM, SRAM_TIMEOUT, FALSE))
    {
        /* check if PT is off(happens when NTAG is not in the field anymore) */
        /* and switch it back on */
        uint8_t reg = 0;
        NFC_ReadRegister(ntag_handle, NFC_MEM_OFFSET_NC_REG, &reg);
        if (!(reg & NFC_NC_REG_MASK_PTHRU_ON_OFF))
        {
            NFC_SetTransferDir(ntag_handle, RF_TO_I2C);
            NFC_SetPthruOnOff(ntag_handle, TRUE);
        }
        if ((HAL_Timer_getTime_ms() - counter > 100))
        {
            break;
        }
    }
}
/*
 * Sends board and FW version info
 */
void send_VersionInfo(void)
{
    /* ---- Version information ---- */
    memset(sram_buf, 0, NFC_MEM_SRAM_SIZE);
    int index = 0;
    memcpy(&sram_buf[index], "Board Ver.: ", 12);
    index             = 12;
    sram_buf[index++] = CHAR_BOARD_1;
    sram_buf[index++] = CHAR_BOARD_2;
    sram_buf[index++] = CHAR_BOARD_3;
    sram_buf[index++] = '\n';

    memcpy(&sram_buf[index], "FW    Ver.: ", 12);
    index += 12;
    sram_buf[index++] = CHAR_FW_MAJ;
    sram_buf[index++] = '.';
    sram_buf[index++] = CHAR_FW_MIN;
    sram_buf[index++] = '\n';

    /* write back Version information */
    NFC_SetTransferDir(ntag_handle, I2C_TO_RF);
    NFC_WriteBytes(ntag_handle, NFC_MEM_ADDR_START_SRAM, sram_buf, NFC_MEM_SRAM_SIZE);

    /* waiting till RF has read */
    NFC_WaitForEvent(ntag_handle, NTAG_EVENT_RF_READ_SRAM, SRAM_TIMEOUT, FALSE);
    uint8_t print_buf[32];
    memcpy(&print_buf, &sram_buf, 31);
    print_buf[15] = ' ';
    print_buf[31] = '\0';
    PRINTF("\n\r%s", print_buf);
}

/*
 * Performs the Speedtest
 * @return returns an NTAG_ERROR_CODE
 */
short int speedTest(void)
{
    uint8_t err;
    uint32_t start_reader;
    uint32_t stop_reader;

    uint32_t start_tag;
    uint32_t stop_tag;

    /* to ensure init system clock back to default for LED demo */
    init_led_demo = false;

    int counter = 0;
    memset(sram_buf, 0, NFC_MEM_SRAM_SIZE);
    err = NFC_SetPthruOnOff(ntag_handle, TRUE);

    if (err != NTAG_ERR_OK)
        return err;

    /* save start timer */
    start_reader = HAL_Timer_getTime_ms();

    /* Reset the seed to its initial value for the CRC32 calculation */
    uint32_t seed  = 0xFFFFFFFF;
    uint32_t crcRx = 0;

    int lastBlock = true;
    /* Begin to Read Data */
    do
    {
        /* wait for data write by RF */
        if (NFC_WaitForEvent(ntag_handle, NTAG_EVENT_RF_WROTE_SRAM, SRAM_TIMEOUT, TRUE))
            return NTAG_ERR_COMMUNICATION;

        /* data is ready, read whole SRAM */
        err = NFC_ReadBytes(ntag_handle, NFC_MEM_ADDR_START_SRAM, sram_buf, NFC_MEM_SRAM_SIZE);
        if (err)
            return NTAG_ERR_COMMUNICATION;
        counter++;

        /* Set LED */
        SwitchLEDs(counter % 2 ? LEDOFF : BLUELED);

        /* The last block contains the "finish_S" string to notify the microcontroller about the message transmission
           completion */
        lastBlock = strncmp((char *)&sram_buf[0], "finish_S", 8);

        /* Update content for the CRC32 calculation */
        if (lastBlock != 0)
        {
            crcRx = calculateCRC(seed, sram_buf, NFC_MEM_SRAM_SIZE);
            seed  = crcRx;
        }
        else
        {
            crcRx = calculateCRC(seed, sram_buf, NFC_MEM_SRAM_SIZE - 4);
        }
    } while (lastBlock != 0);

    /* Get the CRC values in reception */
    uint32_t crcReceived = (uint8_t)sram_buf[NFC_MEM_SRAM_SIZE - 1] << 24 |
                           (uint8_t)sram_buf[NFC_MEM_SRAM_SIZE - 2] << 16 |
                           (uint8_t)sram_buf[NFC_MEM_SRAM_SIZE - 3] << 8 | (uint8_t)sram_buf[NFC_MEM_SRAM_SIZE - 4];

    /* save stop timer */
    stop_reader = HAL_Timer_getTime_ms();

    /* switch PT direction I2C -> RF */
    NFC_SetTransferDir(ntag_handle, I2C_TO_RF);

    /* clear sram Buffer */
    memset(sram_buf, 0, NFC_MEM_SRAM_SIZE);

    int pack = counter;

    /* save start timer */
    start_tag = HAL_Timer_getTime_ms();

    /* Reset the seed to its initial value for the CRC32 calculation */
    seed           = 0xFFFFFFFF;
    uint32_t crcTx = 0;

    /* Begin to Write Data */
    for (; counter != 0; counter--)
    {
        /* write number do identify blocks */
        sram_buf[0] = pack - counter;

        /* for last block write, write finish identifier
           The last block contains the "finish_S" string to notify the LPC board about the message transmission
           completion */
        if (counter == 1)
        {
            memcpy(sram_buf, "finish_S", 8);

            /* Update content for the CRC32 calculation (last 4 bytes are the CRC32) */
            crcTx = calculateCRC(seed, sram_buf, NFC_MEM_SRAM_SIZE - 4);

            /* Write the result of the prev CRC check */
            if (crcRx != crcReceived)
                sram_buf[NFC_MEM_SRAM_SIZE - 5] = 0x01;
            else
                sram_buf[NFC_MEM_SRAM_SIZE - 5] = 0x00;

            // Append the CRC32 to the block as this is the last block to be transmitted */
            sram_buf[NFC_MEM_SRAM_SIZE - 4] = (crcTx >> 0) & 0xFF;
            sram_buf[NFC_MEM_SRAM_SIZE - 3] = (crcTx >> 8) & 0xFF;
            sram_buf[NFC_MEM_SRAM_SIZE - 2] = (crcTx >> 16) & 0xFF;
            sram_buf[NFC_MEM_SRAM_SIZE - 1] = (crcTx >> 24) & 0xFF;
        }
        else
        {
            /* Update content for the CRC32 calculation */
            crcTx = calculateCRC(seed, sram_buf, NFC_MEM_SRAM_SIZE);
            seed  = crcTx;
        }

        /* Write Data to SRAM */
        err = NFC_WriteBytes(ntag_handle, NFC_MEM_ADDR_START_SRAM, sram_buf, NFC_MEM_SRAM_SIZE);

        if (err)
            return NTAG_ERR_COMMUNICATION;
        /* wait for data read */
        if (NFC_WaitForEvent(ntag_handle, NTAG_EVENT_RF_READ_SRAM, SRAM_TIMEOUT, TRUE))
            return NTAG_ERR_COMMUNICATION;

        /* SetLED */
        SwitchLEDs(counter % 2 ? LEDOFF : BLUELED);
    }

    /* stop time */
    stop_tag = HAL_Timer_getTime_ms();

    /* calculate Time */
    uint32_t time1 = stop_reader - start_reader;
    uint32_t time2 = stop_tag - start_tag;

    PRINTF("\r\nSpeed test time1: %i ms, time2: %i ms", time1, time2);

#ifdef WRITE_TIME_VIA_NDEF
    /* Writes the Time in the UserMemory as a NDEF Message */
    write_back_Time_as_NDEF(time1, time2);
#endif

    /* Check the integrity of the data received (not done before to avoid stopping the execution of the rest of the
       SpeedTest demo) */
    if (crcRx != crcReceived)
        return NTAG_ERR_COMMUNICATION;

    return NTAG_ERR_OK;
}

/*
 * Setup the Chip
 */
void Setup(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitButtons();
    BOARD_InitLEDs();
    BOARD_InitDEBUG_UART();
    GPIOC->PDDR |= (1U << 4U);
    GPIO_PinWrite(GPIOC, 4U, 1U); /* Turn the VCC for NTAG chip ON */
    I2C0_InitPins();

    LED_RED_INIT(0);
    LED_GREEN_INIT(0);
    LED_BLUE_INIT(0);
    SwitchLEDs(LEDOFF);

    ADC_Init();

    /* enable delay timer */
    HAL_Timer_Init();

    /* Initialize the Interrupt Service Routine */
    HAL_ISR_Init();

    /* Initialize I2C */
    HAL_I2C_InitDevice(HAL_I2C_INIT_DEFAULT, I2C_MASTER_CLK_SRC, NTAG_I2C_MASTER_BASEADDR);
    SystemCoreClockUpdate();

    /* Initialize the NTAG I2C components */
    ntag_handle = NFC_InitDevice((NTAG_ID_T)0, NTAG_I2C_MASTER_BASEADDR);
    HAL_ISR_RegisterCallback((ISR_SOURCE_T)0, ISR_LEVEL_LO, NULL, NULL);
}

/*
 * Switches LEDs off
 */
static void ledsOFF()
{
    /*#if defined FRDM_K82F || defined FRDM_K64F*/
    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_OFF();
    /*#endif*/
}

/*
 * Switches only red LED on
 */
static void Red()
{
    /*#if defined FRDM_K82F || defined FRDM_K64F*/
    LED_BLUE_OFF();
    LED_RED_ON();
    LED_GREEN_OFF();
    /*#endif*/
}

/*
 * Switches only green LED on
 */
static void Green()
{
    /*#if defined FRDM_K82F || defined FRDM_K64F*/
    LED_BLUE_OFF();
    LED_RED_OFF();
    LED_GREEN_ON();
    /*#endif*/
}

/*
 * Switches only blue LED on
 */
static void Blue()
{
    /*#if defined FRDM_K82F || defined FRDM_K64F*/
    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_ON();
    /*#endif*/
}

/*
 * Switch the LED
 * @param led
 */
void SwitchLEDs(LED led)
{
    if (led == LEDOFF)
        ledsOFF();
    else if (led == REDLED)
        Red();
    else if (led == GREENLED)
        Green();
    else if (led == BLUELED)
        Blue();
}
/*
 * Let a LED blink (blocking function ~ 700ms)
 * @param LED LED to blink
 */
void LED_blink(LED led)
{
    SwitchLEDs(LEDOFF);
    HAL_Timer_delay_ms(100);
    SwitchLEDs(led);
    HAL_Timer_delay_ms(100);
    SwitchLEDs(LEDOFF);
    HAL_Timer_delay_ms(100);
    SwitchLEDs(led);
    HAL_Timer_delay_ms(100);
    SwitchLEDs(LEDOFF);
    HAL_Timer_delay_ms(100);
    SwitchLEDs(led);
    HAL_Timer_delay_ms(100);
    SwitchLEDs(LEDOFF);
    HAL_Timer_delay_ms(100);
}

/*
 * Checks Buttons and gives them back as a bitfield
 * @param Buttons Bitfield in which the button state is written(first Button = LSB, true = pressed, false = not pressed)
 */
void check_Buttons(uint8_t *Buttons)
{
    if (SW1_PRESSED)
    {
        *Buttons |= 0x01;
    }
    else
    {
        *Buttons &= ~0x01;
    }

    if (SW2_PRESSED)
    {
        *Buttons |= 0x04;
    }
    else
    {
        *Buttons &= ~0x04;
    }
    /* SW3 is emulated by pressing both SW1 and SW2 */
    if (SW3_PRESSED)
    {
        *Buttons |= 0x02;
    }
    else
    {
        *Buttons &= ~0x02;
    }
}

void inttoa(int n, char s[])
{
    int i, j, sign;
    char c;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */
    i = 0;
    do
    {                          /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0);   /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c    = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
/*
 * Writes the Timing in the User Memory formated as NDEF Message
 * @param time1 first time to write
 * @param time2 second time to write
 */
short int write_back_Time_as_NDEF(int time1, int time2)
{
    uint8_t err;
    /* check for overflow */
    if (time1 >= 99999)
        time1 = 99999;
    if (time2 >= 99999)
        time2 = 99999;

    /* write back times as ndef */
    int index = 0;

    /* Header of the NDEF Message */
    sram_buf[index++] = 0x03;
    sram_buf[index++] = 0x1C; /* Message length */
    sram_buf[index++] = 0xd1;

    sram_buf[index++] = 0x01; /* Text Record */
    sram_buf[index++] = 0x18; /* Record length */
    sram_buf[index++] = 0x54;
    sram_buf[index++] = 0x02;

    sram_buf[index++] = 0x65;
    sram_buf[index++] = 0x6e;

    char temp[5];
    uint8_t k;

    memset(temp, 0, 5);
    inttoa(time1, temp);

    sram_buf[index++] = '(';

    for (k = 0; k < 5 - strlen(temp); k++)
        sram_buf[index++] = '0';

    for (k = 0; k < strlen(temp); k++)
        sram_buf[index++] = temp[k];

    sram_buf[index++] = ')';

    memset(temp, 0, 5);
    inttoa(time2, temp);

    sram_buf[index++] = '(';

    for (k = 0; k < 5 - strlen(temp); k++)
        sram_buf[index++] = '0';

    for (k = 0; k < strlen(temp); k++)
        sram_buf[index++] = temp[k];

    sram_buf[index++] = ')';

    memset(temp, 0, 5);

    sram_buf[index++] = '(';

    for (k = 0; k < 5 - strlen(temp); k++)
        sram_buf[index++] = '0';

    for (k = 0; k < strlen(temp); k++)
        sram_buf[index++] = temp[k];

    sram_buf[index++] = ')';

    /* end of NDEF Message */
    sram_buf[index++] = 0xFE;
    sram_buf[index++] = 0x00;

    /* Disable the PT */
    err = NFC_SetPthruOnOff(ntag_handle, FALSE);

    /* get the timeout time */
    int current_loop = HAL_Timer_getTime_ms() + SRAM_TIMEOUT;

    uint8_t reg = 0;
    /* write back the Result in EEPROM as NDEF */
    do
    {
        err = NFC_WriteBytes(ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY, sram_buf, index);

        NFC_ReadRegister(ntag_handle, NFC_MEM_OFFSET_NS_REG, &reg);
        while (reg & NFC_NS_REG_MASK_I2C_LOCKED)
        {
            NFC_ReadRegister(ntag_handle, NFC_MEM_OFFSET_NS_REG, &reg);
        }

        if (current_loop < HAL_Timer_getTime_ms())
            return NTAG_ERR_COMMUNICATION;

    } while (err != NTAG_ERR_OK);

    return NTAG_ERR_OK;
}

/*
 * Writes the Android Application Record of the NTAG I2C Demo Android App in the User Memory
 */
bool reset_AAR(void)
{
    bool err;
    err =
        NFC_WriteBytes(ntag_handle, NFC_MEM_ADDR_START_USER_MEMORY, Default_NDEF_Message, Default_NDEF_Message_length);

    return err;
}

/*!
 * @brief CRC table creation function.
 *
 * This function creates the CRC table for CRC calculation.
 *
 * @return None.
 */
static void createCRCTableList(void)
{
    /* CRC32 - polynomial reserved */
    uint32_t polynomial = 0xEDB88320U;
    for (uint16_t index = 0U; index < 256U; index++)
    {
        uint32_t crcElement = index;
        uint32_t topBit     = 0x00000001U;
        for (uint8_t i = 0U; i < 8U; i++)
        {
            if (crcElement & topBit)
            {
                crcElement = (crcElement >> 1U) ^ polynomial;
            }
            else
            {
                crcElement = (crcElement >> 1U);
            }
        }
        s_CRCTableList[index] = crcElement;
    }
}

/*!
 * @brief CRC calculation function.
 *
 * This function calculates the CRC over a buffer.
 *
 * @return CRC value.
 */
static uint32_t calculateCRC(uint32_t crc, uint8_t *data, uint32_t length)
{
    uint8_t crcIndex   = 0U;
    uint32_t crcReturn = crc;
    for (uint32_t i = 0U; i < length; i++)
    {
        crcIndex  = (uint8_t)((crcReturn & 0x000000FFU) ^ data[i]);
        crcReturn = s_CRCTableList[crcIndex] ^ (crcReturn >> 8U);
    }
    return (crcReturn);
}
