/*******************************************************************************
 *
 * E M B E D D E D   W I Z A R D   P R O J E C T
 *
 *                                                Copyright (c) TARA Systems GmbH
 *                                    written by Paul Banach and Manfred Schweyer
 *
 ********************************************************************************
 *
 * This software is delivered "as is" and shows the usage of other software
 * components. It is provided as an example software which is intended to be
 * modified and extended according to particular requirements.
 *
 * TARA Systems hereby disclaims all warranties and conditions with regard to the
 * software, including all implied warranties and conditions of merchantability
 * and non-infringement of any third party IPR or other rights which may result
 * from the use or the inability to use the software.
 *
 ********************************************************************************
 *
 * DESCRIPTION:
 *   This file contains the necessary initialization of the ILI9341 display
 *   controller and the necessary function to update a rectangular area of the
 *   screen from an Embedded Wizard UI application.
 *   The data transfer is done by using the hardware SPI controller via DMA.
 *   The color format of the display is RGB565.
 *
 *   This module is just a template - adapt pin configuration and timing (!)
 *   according your hardware layout and hardware constraints.
 *   Adjust device parameters, initialization and gamma correction values
 *   according your display and your needs.
 *   This driver was tested on a Adafruit 1947 display module (3,2" TFT 320x240).
 *
 *******************************************************************************/

#include <string.h>

#include "ewconfig.h"
#include "ew_bsp_clock.h"

#include "fsl_lpspi.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi_cmsis.h"
#include "fsl_cache.h"
#include "ili9341_spi.h"

#if EW_USE_FREE_RTOS == 1
#include "FreeRTOS.h"
#include "task.h"
#endif

#define BOARD_LCD_SPI          Driver_SPI1
#define BOARD_LCD_SPI_BAUDRATE 80000000U /* Transfer baudrate */

/* Select PLL2_MAIN_CLK (528 MHz) as master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (2U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (5U)

#define BOARD_LCD_DC_GPIO     GPIO1 /* LCD data/command port */
#define BOARD_LCD_DC_GPIO_PIN 0x03  /* LCD data/command pin */

/* display specific settings */
#define DISPLAY_PIXEL_WIDTH  ((uint16_t)320)
#define DISPLAY_PIXEL_HEIGHT ((uint16_t)240)

/* ILI9341 commands */
#define ILI9341_SWRESET            0x01 /* Software Reset */
#define ILI9341_READ_DISPLAY_ID    0x04 /* Read display identification information */
#define ILI9341_RDDST              0x09 /* Read Display Status */
#define ILI9341_RDDPM              0x0A /* Read Display Power Mode */
#define ILI9341_RDDMADCTL          0x0B /* Read Display MADCTL */
#define ILI9341_RDDCOLMOD          0x0C /* Read Display Pixel Format */
#define ILI9341_RDDIM              0x0D /* Read Display Image Format */
#define ILI9341_RDDSM              0x0E /* Read Display Signal Mode */
#define ILI9341_RDDSDR             0x0F /* Read Display Self-Diagnostic Result */
#define ILI9341_SPLIN              0x10 /* Enter Sleep Mode */
#define ILI9341_SLEEP_OUT          0x11 /* Sleep out register */
#define ILI9341_PTLON              0x12 /* Partial Mode ON */
#define ILI9341_NORMAL_MODE_ON     0x13 /* Normal Display Mode ON */
#define ILI9341_DINVOFF            0x20 /* Display Inversion OFF */
#define ILI9341_DINVON             0x21 /* Display Inversion ON */
#define ILI9341_GAMMA              0x26 /* Gamma register */
#define ILI9341_DISPLAY_OFF        0x28 /* Display off register */
#define ILI9341_DISPLAY_ON         0x29 /* Display on register */
#define ILI9341_COLUMN_ADDR        0x2A /* Colomn address register */
#define ILI9341_PAGE_ADDR          0x2B /* Page address register */
#define ILI9341_GRAM               0x2C /* GRAM register */
#define ILI9341_RGBSET             0x2D /* Color SET */
#define ILI9341_RAMRD              0x2E /* Memory Read */
#define ILI9341_PLTAR              0x30 /* Partial Area */
#define ILI9341_VSCRDEF            0x33 /* Vertical Scrolling Definition */
#define ILI9341_TEOFF              0x34 /* Tearing Effect Line OFF */
#define ILI9341_TEON               0x35 /* Tearing Effect Line ON */
#define ILI9341_MAC                0x36 /* Memory Access Control register*/
#define ILI9341_VSCRSADD           0x37 /* Vertical Scrolling Start Address */
#define ILI9341_IDMOFF             0x38 /* Idle Mode OFF */
#define ILI9341_IDMON              0x39 /* Idle Mode ON */
#define ILI9341_PIXEL_FORMAT       0x3A /* Pixel Format register */
#define ILI9341_WRITE_MEM_CONTINUE 0x3C /* Write Memory Continue */
#define ILI9341_READ_MEM_CONTINUE  0x3E /* Read Memory Continue */
#define ILI9341_SET_TEAR_SCANLINE  0x44 /* Set Tear Scanline */
#define ILI9341_GET_SCANLINE       0x45 /* Get Scanline */
#define ILI9341_WDB                0x51 /* Write Brightness Display register */
#define ILI9341_RDDISBV            0x52 /* Read Display Brightness */
#define ILI9341_WCD                0x53 /* Write Control Display register*/
#define ILI9341_RDCTRLD            0x54 /* Read CTRL Display */
#define ILI9341_WRCABC             0x55 /* Write Content Adaptive Brightness Control */
#define ILI9341_RDCABC             0x56 /* Read Content Adaptive Brightness Control */
#define ILI9341_WRITE_CABC         0x5E /* Write CABC Minimum Brightness */
#define ILI9341_READ_CABC          0x5F /* Read CABC Minimum Brightness */
#define ILI9341_READ_ID1           0xDA /* Read ID1 */
#define ILI9341_READ_ID2           0xDB /* Read ID2 */
#define ILI9341_READ_ID3           0xDC /* Read ID3 */
#define ILI9341_RGB_INTERFACE      0xB0 /* RGB Interface Signal Control */
#define ILI9341_FRMCTR1            0xB1 /* Frame Rate Control (In Normal Mode) */
#define ILI9341_FRMCTR2            0xB2 /* Frame Rate Control (In Idle Mode) */
#define ILI9341_FRMCTR3            0xB3 /* Frame Rate Control (In Partial Mode) */
#define ILI9341_INVTR              0xB4 /* Display Inversion Control */
#define ILI9341_BPC                0xB5 /* Blanking Porch Control register */
#define ILI9341_DFC                0xB6 /* Display Function Control register */
#define ILI9341_ETMOD              0xB7 /* Entry Mode Set */
#define ILI9341_BACKLIGHT1         0xB8 /* Backlight Control 1 */
#define ILI9341_BACKLIGHT2         0xB9 /* Backlight Control 2 */
#define ILI9341_BACKLIGHT3         0xBA /* Backlight Control 3 */
#define ILI9341_BACKLIGHT4         0xBB /* Backlight Control 4 */
#define ILI9341_BACKLIGHT5         0xBC /* Backlight Control 5 */
#define ILI9341_BACKLIGHT7         0xBE /* Backlight Control 7 */
#define ILI9341_BACKLIGHT8         0xBF /* Backlight Control 8 */
#define ILI9341_POWER1             0xC0 /* Power Control 1 register */
#define ILI9341_POWER2             0xC1 /* Power Control 2 register */
#define ILI9341_VCOM1              0xC5 /* VCOM Control 1 register */
#define ILI9341_VCOM2              0xC7 /* VCOM Control 2 register */
#define ILI9341_NVMWR              0xD0 /* NV Memory Write */
#define ILI9341_NVMPKEY            0xD1 /* NV Memory Protection Key */
#define ILI9341_RDNVM              0xD2 /* NV Memory Status Read */
#define ILI9341_READ_ID4           0xD3 /* Read ID4 */
#define ILI9341_PGAMMA             0xE0 /* Positive Gamma Correction register */
#define ILI9341_NGAMMA             0xE1 /* Negative Gamma Correction register */
#define ILI9341_DGAMCTRL1          0xE2 /* Digital Gamma Control 1 */
#define ILI9341_DGAMCTRL2          0xE3 /* Digital Gamma Control 2 */
#define ILI9341_INTERFACE          0xF6 /* Interface control register */
#define ILI9341_POWERA             0xCB /* Power control A register */
#define ILI9341_POWERB             0xCF /* Power control B register */
#define ILI9341_DTCA               0xE8 /* Driver timing control A */
#define ILI9341_DTCB               0xEA /* Driver timing control B */
#define ILI9341_POWER_SEQ          0xED /* Power on sequence register */
#define ILI9341_3GAMMA_EN          0xF2 /* 3 Gamma enable register */
#define ILI9341_PRC                0xF7 /* Pump ratio control register */

#define LPSPI1_Freq CLOCK_GetFreq(kCLOCK_SysPllClk) / LPSPI_CLOCK_SOURCE_DIVIDER

static volatile uint8_t SpiTransferActive = 0;

/* Get LPSPI1_CLK_Freq (88MHz).  */
uint32_t LPSPI1_GetFreq(void)
{
    return LPSPI1_Freq;
}

static void SPI_MasterSignalEvent(uint32_t event)
{
    /* clear the flag */
    SpiTransferActive = 0;
}

static void WaitForCompletion(void)
{
    if (SpiTransferActive == 0)
        return;

    CPU_LOAD_SET_IDLE();
    while (SpiTransferActive)
    {
#if EW_USE_FREE_RTOS == 1

        vTaskDelay(1);

#endif
    }
    CPU_LOAD_SET_ACTIVE();
}

/* helper function to write a command register value */
static void ILI9341_WriteCmnd(uint8_t aCmnd)
{
    L1CACHE_CleanDCacheByRange((uint32_t)&aCmnd, 1);
    SpiTransferActive = 1;
    GPIO_PortClear(BOARD_LCD_DC_GPIO, 0x01 << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(&aCmnd, 1);
    WaitForCompletion();
}

/* helper function to read a single data byte in polling mode */
static uint8_t ILI9341_ReadByte(void)
{
    return 0;
}

/* helper function to write a single data byte */
static void ILI9341_WriteByte(uint8_t aData)
{
    L1CACHE_CleanDCacheByRange((uint32_t)&aData, 1);
    SpiTransferActive = 1;
    GPIO_PortSet(BOARD_LCD_DC_GPIO, 0x01 << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(&aData, 1);
    WaitForCompletion();
}

/* helper function to write a sequence of data bytes without waiting for completion */
static void ILI9341_WriteBytes(uint8_t *aData, int aNoOfBytes)
{
    L1CACHE_CleanDCacheByRange((uint32_t)aData, aNoOfBytes);
    SpiTransferActive = 1;
    GPIO_PortSet(BOARD_LCD_DC_GPIO, 0x01 << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(aData, aNoOfBytes);
}

/* helper function to initialize reset and backlight pins of LCD and to perform
   proper reset of display */
static void ILI9341_ResetDisplay()
{
}

/*******************************************************************************
 * FUNCTION:
 *   ILI9341_Init
 *
 * DESCRIPTION:
 *   The function initializes the SPI hardware and all necessary GPIO in order
 *   to reset and intialize the connected display hardware.
 *
 * ARGUMENTS:
 *   None
 *
 * RETURN VALUE:
 *   None
 *
 *******************************************************************************/
void ILI9341_Init(void)
{
    /* init structure for the data/command output pin */
    gpio_pin_config_t dc_config = {
        kGPIO_DigitalOutput,
        1,
    };

    /* init data/command GPIO output. */
    GPIO_PinInit(BOARD_LCD_DC_GPIO, BOARD_LCD_DC_GPIO_PIN, &dc_config);

    /* Set LPSPI_PODF. */
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLOCK_SOURCE_DIVIDER);
    /* Set Lpspi clock source. */
    CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLOCK_SOURCE_SELECT);

    /* SPI master init */
    BOARD_LCD_SPI.Initialize(SPI_MasterSignalEvent);
    BOARD_LCD_SPI.PowerControl(ARM_POWER_FULL);
    BOARD_LCD_SPI.Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_DATA_BITS(8), BOARD_LCD_SPI_BAUDRATE);

    /* perform reset of LCD */
    ILI9341_ResetDisplay();

    /* switch display off */
    ILI9341_WriteCmnd(ILI9341_DISPLAY_OFF);

    /* power control A */
    ILI9341_WriteCmnd(ILI9341_POWERA);
    ILI9341_WriteByte(0x39);
    ILI9341_WriteByte(0x2C);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0x34);
    ILI9341_WriteByte(0x02);

    /* power control B */
    ILI9341_WriteCmnd(ILI9341_POWERB);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0xC1); // or 0x83 or 0x81 ???
    ILI9341_WriteByte(0x30);

    /* driver timing control A */
    ILI9341_WriteCmnd(ILI9341_DTCA);
    ILI9341_WriteByte(0x85);
    ILI9341_WriteByte(0x00); // or 0x01
    ILI9341_WriteByte(0x78); // or 0x79

    /* driver timing control B */
    ILI9341_WriteCmnd(ILI9341_DTCB);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0x00);

    /* power on sequence control */
    ILI9341_WriteCmnd(ILI9341_POWER_SEQ);
    ILI9341_WriteByte(0x64);
    ILI9341_WriteByte(0x03);
    ILI9341_WriteByte(0x12);
    ILI9341_WriteByte(0x81);

    /* pump ratio control */
    ILI9341_WriteCmnd(ILI9341_PRC);
    ILI9341_WriteByte(0x20);

    /* power control 1 */
    ILI9341_WriteCmnd(ILI9341_POWER1);
    ILI9341_WriteByte(0x23);

    /* power control 2 */
    ILI9341_WriteCmnd(ILI9341_POWER2);
    ILI9341_WriteByte(0x10); // or 0x11

    /* VCOM 1 / 2 registers */
    ILI9341_WriteCmnd(ILI9341_VCOM1);
    ILI9341_WriteByte(0x3E); // or 0x35 or 0x45
    ILI9341_WriteByte(0x28); // or 0x15
    ILI9341_WriteCmnd(ILI9341_VCOM2);
    ILI9341_WriteByte(0x86); // or 0x90

    /* memory access control */
    ILI9341_WriteCmnd(ILI9341_MAC);
    ILI9341_WriteByte(0x40); // or 0x48 or 0xC8 ???

    /* pixel format set */
    ILI9341_WriteCmnd(ILI9341_PIXEL_FORMAT);
    ILI9341_WriteByte(0x55);

    /* frame rate */
    ILI9341_WriteCmnd(ILI9341_FRMCTR1);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0x18);

    /* display function control set */
    ILI9341_WriteCmnd(ILI9341_DFC);
    ILI9341_WriteByte(0x08);
    ILI9341_WriteByte(0x82);
    ILI9341_WriteByte(0x27);
    //  ILI9341_WriteByte( 0x00 );

    /* gamma configuration */
    ILI9341_WriteCmnd(ILI9341_3GAMMA_EN);
    ILI9341_WriteByte(0x00);

    ILI9341_WriteCmnd(ILI9341_GAMMA);
    ILI9341_WriteByte(0x01);

    ILI9341_WriteCmnd(ILI9341_PGAMMA);
    ILI9341_WriteByte(0x0F);
    ILI9341_WriteByte(0x31);
    ILI9341_WriteByte(0x2B);
    ILI9341_WriteByte(0x0C);
    ILI9341_WriteByte(0x0E);
    ILI9341_WriteByte(0x08);
    ILI9341_WriteByte(0x4E);
    ILI9341_WriteByte(0xF1);
    ILI9341_WriteByte(0x37);
    ILI9341_WriteByte(0x07);
    ILI9341_WriteByte(0x10);
    ILI9341_WriteByte(0x03);
    ILI9341_WriteByte(0x0E);
    ILI9341_WriteByte(0x09);
    ILI9341_WriteByte(0x00);

    ILI9341_WriteCmnd(ILI9341_NGAMMA);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0x0E);
    ILI9341_WriteByte(0x14);
    ILI9341_WriteByte(0x03);
    ILI9341_WriteByte(0x11);
    ILI9341_WriteByte(0x07);
    ILI9341_WriteByte(0x31);
    ILI9341_WriteByte(0xC1);
    ILI9341_WriteByte(0x48);
    ILI9341_WriteByte(0x08);
    ILI9341_WriteByte(0x0F);
    ILI9341_WriteByte(0x0C);
    ILI9341_WriteByte(0x31);
    ILI9341_WriteByte(0x36);
    ILI9341_WriteByte(0x0F);

    /* colomn address set */
    ILI9341_WriteCmnd(ILI9341_COLUMN_ADDR);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0xEF);

    /* page address set */
    ILI9341_WriteCmnd(ILI9341_PAGE_ADDR);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0x00);
    ILI9341_WriteByte(0x01);
    ILI9341_WriteByte(0x3F);

    /* entry mode set */
    // ILI9341_WriteCmnd( ILI9341_ETMOD );
    // ILI9341_WriteByte( 0x07 );

    /* wake up */
    ILI9341_WriteCmnd(ILI9341_SLEEP_OUT);
    // vTaskDelay( 200 / portTICK_RATE_MS );

    /* switch display on */
    ILI9341_WriteCmnd(ILI9341_DISPLAY_ON);
    // vTaskDelay( 200 / portTICK_RATE_MS );

    ILI9341_WriteCmnd(ILI9341_MAC);
    ILI9341_WriteByte(0x28); // or 0x48 or 0xC8 ???

    /* start writing to GRAM */
    ILI9341_WriteCmnd(ILI9341_GRAM);
}

/*******************************************************************************
 * FUNCTION:
 *   ILI9341_ReadID
 *
 * DESCRIPTION:
 *   The function assumes a connected and intialized ILI9341 display and tries
 *   to read its ID.
 *
 * ARGUMENTS:
 *   None
 *
 * RETURN VALUE:
 *   If successful, the function returns the ID of the display - 0 otherwise.
 *
 *******************************************************************************/
uint16_t ILI9341_ReadID(void)
{
    uint16_t result;

    /* send command */
    ILI9341_WriteCmnd(ILI9341_READ_ID4);

    /* read dummy data */
    ILI9341_ReadByte();

    /* read register values */
    ILI9341_ReadByte();
    result = ILI9341_ReadByte() << 8;
    result |= ILI9341_ReadByte();

    return result;
}

/*******************************************************************************
 * FUNCTION:
 *   ILI9341_WaitForCompletion
 *
 * DESCRIPTION:
 *   The function ensures that the previous SPI data transfer is completed..
 *
 * ARGUMENTS:
 *   None
 *
 * RETURN VALUE:
 *   None
 *
 *******************************************************************************/
void ILI9341_WaitForCompletion(void)
{
    WaitForCompletion();
}

/*******************************************************************************
 * FUNCTION:
 *   ILI9341_WriteDataWindow
 *
 * DESCRIPTION:
 *   The function sets the destination position and size within the framebuffer
 *   of the display according the given rectangle and starts the transfer of
 *   the given data. The function does NOT wait for the completion of the transfer.
 *
 * ARGUMENTS:
 *   aX       - Horizontal position for next write access.
 *   aY       - Vertical position for next write access.
 *   aW       - Width of the data window in pixel.
 *   aH       - Height of the data window in pixel.
 *
 * RETURN VALUE:
 *   None
 *
 *******************************************************************************/
void ILI9341_WriteDataWindow(uint16_t aX, uint16_t aY, uint16_t aW, uint16_t aH, uint8_t *aData)
{
    int xEnd = aX + aW - 1;
    int yEnd = aY + aH - 1;

    if (xEnd >= DISPLAY_PIXEL_WIDTH)
        xEnd = DISPLAY_PIXEL_WIDTH - 1;
    if (yEnd >= DISPLAY_PIXEL_HEIGHT)
        yEnd = DISPLAY_PIXEL_HEIGHT - 1;

    /* colomn address set */
    ILI9341_WriteCmnd(ILI9341_COLUMN_ADDR);
    ILI9341_WriteByte(aX >> 8);
    ILI9341_WriteByte(aX & 0xFF);
    ILI9341_WriteByte(xEnd >> 8);
    ILI9341_WriteByte(xEnd & 0xFF);

    /* page address set */
    ILI9341_WriteCmnd(ILI9341_PAGE_ADDR);
    ILI9341_WriteByte(aY >> 8);
    ILI9341_WriteByte(aY & 0xFF);
    ILI9341_WriteByte(yEnd >> 8);
    ILI9341_WriteByte(yEnd & 0xFF);

    /* start writing to GRAM */
    ILI9341_WriteCmnd(ILI9341_GRAM);

    ILI9341_WriteBytes(aData, aW * aH * 2);
}

/* msy */
