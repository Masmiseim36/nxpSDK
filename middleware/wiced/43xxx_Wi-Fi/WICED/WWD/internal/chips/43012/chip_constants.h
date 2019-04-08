/*
 * $ Copyright Cypress Semiconductor $
 */

#ifndef INCLUDED_CHIP_CONSTANTS_H_
#define INCLUDED_CHIP_CONSTANTS_H_

#include "wwd_wlioctl.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *             Architecture Constants
 ******************************************************/

/* D11 registers and SHM */
#define D11_BASE_ADDR           0x18001000
#define D11_AXI_BASE_ADDR       0xE8000000
#define D11_SHM_BASE_ADDR       (D11_AXI_BASE_ADDR + 0x4000)

#define D11REG_ADDR(offset)     (D11_BASE_ADDR + offset)
#define D11IHR_ADDR(offset)     (D11_AXI_BASE_ADDR + 0x400 + (2 * offset))
#define D11SHM_ADDR(offset)     (D11_SHM_BASE_ADDR + offset)

#define M_DS1_CTRL_STATUS               (0xe0b*2)

/* RMC operational modes */
enum ds1_ctrl_status {
    DS1_SLEEP               = 0,     /* DS1_SLEEP  */
    MAC_ON                  = 1,     /* MAC_ON */
    RADIO_PHY_ON            = 2,     /* RADIO_PHY_ON  */
    DS1_EXIT                = 3      /* DS1_EXIT.*/
};

#define M_DS1_CTRL_SDIO                 (0xe0c*2)

#define C_DS1_CTRL_SDIO_DS1_SLEEP       (1 << 0)
#define C_DS1_CTRL_SDIO_MAC_ON          (1 << 1)
#define C_DS1_CTRL_SDIO_RADIO_PHY_ON    (1 << 2)
#define C_DS1_CTRL_SDIO_DS1_EXIT        (1 << 3)
#define C_DS1_CTRL_PROC_DONE            (1 << 8)
#define C_DS1_CTRL_REQ_VALID            (1 << 9)
/* MacControl register */
#define D11_MACCONTROL_REG          D11REG_ADDR(0x120)
#define D11_MACCONTROL_REG_SIZE     4
#define D11_MACCONTROL_REG_WAKE     (1 << 26)
#define D11_MACCONTROL_CLEAR_WAKE     (0xFBFFFFFF)
#define PMU_MINRESMASK              (PMU_BASE_ADDRESS + 0x618)
#define DEFAULT_43012_MIN_RES_MASK   0x0f8bfe77

/* General chip stats */
#define CHIP_RAM_SIZE      ( 0xA0000 )
#define SOCSRAM_SRMEM_SIZE ( 0 )

/* Backplane architecture */
#define CHIPCOMMON_BASE_ADDRESS  0x18000000    /* Chipcommon core register region   */
#define DOT11MAC_BASE_ADDRESS    0x18001000    /* dot11mac core register region     */
#define SDIO_BASE_ADDRESS        0x18002000    /* SDIOD Device core register region */
#define WLAN_ARMCM3_BASE_ADDRESS 0x18003000    /* ARMCM3 core register region       */
#define SOCSRAM_BASE_ADDRESS     0x18004000    /* SOCSRAM core register region      */
#define PMU_BASE_ADDRESS         0x18012000    /* PMU core register region          */
#define BACKPLANE_ADDRESS_MASK   0x7FFF
#define BACKPLANE_WINDOW_SIZE    ( BACKPLANE_ADDRESS_MASK + 1)

#define CHIP_STA_INTERFACE   0
#define CHIP_AP_INTERFACE    1

/* Maximum value of bus data credit difference */
#define CHIP_MAX_BUS_DATA_CREDIT_DIFF    20

/* Chipcommon registers */
#define CHIPCOMMON_GPIO_CONTROL ((uint32_t) (CHIPCOMMON_BASE_ADDRESS + 0x6C) )
#define CHIPCOMMON_SR_CONTROL0  ((uint32_t) (CHIPCOMMON_BASE_ADDRESS + 0x504) )
#define CHIPCOMMON_SR_CONTROL1  ((uint32_t) (CHIPCOMMON_BASE_ADDRESS + 0x508) )

/* SOCSRAM core registers */
#define SOCSRAM_BANKX_INDEX     ((uint32_t) (SOCSRAM_BASE_ADDRESS + 0x10) )
#define SOCSRAM_BANKX_PDA       ((uint32_t) (SOCSRAM_BASE_ADDRESS + 0x44) )

/* PMU core registers */
#define RETENTION_CTL           ((uint32_t) (PMU_BASE_ADDRESS + 0x670) )
#define RCTL_MACPHY_DISABLE     ( (uint32_t) (1 << 26) )
#define RCTL_LOGIC_DISABLE      ( (uint32_t) (1 << 27) )

/******************************************************
 *             SDIO Constants
 ******************************************************/
/* CurrentSdiodProgGuide r23 */

/* Base registers */
#define SDIO_CORE                    ((uint32_t) (SDIO_BASE_ADDRESS + 0x00) )
#define SDIO_INT_STATUS              ((uint32_t) (SDIO_BASE_ADDRESS + 0x20) )
#define SDIO_TO_SB_MAILBOX           ((uint32_t) (SDIO_BASE_ADDRESS + 0x40) )
#define SDIO_TO_SB_MAILBOX_DATA      ((uint32_t) (SDIO_BASE_ADDRESS + 0x48) )
#define SDIO_TO_HOST_MAILBOX_DATA    ((uint32_t) (SDIO_BASE_ADDRESS + 0x4C) )
#define SDIO_TO_SB_MAIL_BOX          ((uint32_t) (SDIO_BASE_ADDRESS + 0x40) )
#define SDIO_INT_HOST_MASK           ((uint32_t) (SDIO_BASE_ADDRESS + 0x24) )
#define SDIO_FUNCTION_INT_MASK       ((uint32_t) (SDIO_BASE_ADDRESS + 0x34) )

/* SDIO Function 0 (SDIO Bus) register addresses */

/* SDIO Device CCCR offsets */
/* TODO: What does CIS/CCCR stand for? */
/* CCCR accesses do not require backpane clock */
#define SDIOD_CCCR_REV             ( (uint32_t)  0x00 )    /* CCCR/SDIO Revision */
#define SDIOD_CCCR_SDREV           ( (uint32_t)  0x01 )    /* SD Revision */
#define SDIOD_CCCR_IOEN            ( (uint32_t)  0x02 )    /* I/O Enable */
#define SDIOD_CCCR_IORDY           ( (uint32_t)  0x03 )    /* I/O Ready */
#define SDIOD_CCCR_INTEN           ( (uint32_t)  0x04 )    /* Interrupt Enable */
#define SDIOD_CCCR_INTPEND         ( (uint32_t)  0x05 )    /* Interrupt Pending */
#define SDIOD_CCCR_IOABORT         ( (uint32_t)  0x06 )    /* I/O Abort */
#define SDIOD_CCCR_BICTRL          ( (uint32_t)  0x07 )    /* Bus Interface control */
#define SDIOD_CCCR_CAPABLITIES     ( (uint32_t)  0x08 )    /* Card Capabilities */
#define SDIOD_CCCR_CISPTR_0        ( (uint32_t)  0x09 )    /* Common CIS Base Address Pointer Register 0 (LSB) */
#define SDIOD_CCCR_CISPTR_1        ( (uint32_t)  0x0A )    /* Common CIS Base Address Pointer Register 1 */
#define SDIOD_CCCR_CISPTR_2        ( (uint32_t)  0x0B )    /* Common CIS Base Address Pointer Register 2 (MSB - only bit 1 valid)*/
#define SDIOD_CCCR_BUSSUSP         ( (uint32_t)  0x0C )    /*  */
#define SDIOD_CCCR_FUNCSEL         ( (uint32_t)  0x0D )    /*  */
#define SDIOD_CCCR_EXECFLAGS       ( (uint32_t)  0x0E )    /*  */
#define SDIOD_CCCR_RDYFLAGS        ( (uint32_t)  0x0F )    /*  */
#define SDIOD_CCCR_BLKSIZE_0       ( (uint32_t)  0x10 )    /* Function 0 (Bus) SDIO Block Size Register 0 (LSB) */
#define SDIOD_CCCR_BLKSIZE_1       ( (uint32_t)  0x11 )    /* Function 0 (Bus) SDIO Block Size Register 1 (MSB) */
#define SDIOD_CCCR_POWER_CONTROL   ( (uint32_t)  0x12 )    /* Power Control */
#define SDIOD_CCCR_SPEED_CONTROL   ( (uint32_t)  0x13 )    /* Bus Speed Select  (control device entry into high-speed clocking mode)  */
#define SDIOD_CCCR_UHS_I           ( (uint32_t)  0x14 )    /* UHS-I Support */
#define SDIOD_CCCR_DRIVE           ( (uint32_t)  0x15 )    /* Drive Strength */
#define SDIOD_CCCR_INTEXT          ( (uint32_t)  0x16 )    /* Interrupt Extension */
#define SDIOD_CCCR_BRCM_CARDCAP    ( (uint32_t)  0xF0 )    /* Brcm Card Capability */
#define SDIOD_SEP_INT_CTL          ( (uint32_t)  0xF2 )    /* Separate Interrupt Control*/
#define SDIOD_CCCR_F1INFO          ( (uint32_t) 0x100 )    /* Function 1 (Backplane) Info */
#define SDIOD_CCCR_F1HP            ( (uint32_t) 0x102 )    /* Function 1 (Backplane) High Power */
#define SDIOD_CCCR_F1CISPTR_0      ( (uint32_t) 0x109 )    /* Function 1 (Backplane) CIS Base Address Pointer Register 0 (LSB) */
#define SDIOD_CCCR_F1CISPTR_1      ( (uint32_t) 0x10A )    /* Function 1 (Backplane) CIS Base Address Pointer Register 1       */
#define SDIOD_CCCR_F1CISPTR_2      ( (uint32_t) 0x10B )    /* Function 1 (Backplane) CIS Base Address Pointer Register 2 (MSB - only bit 1 valid) */
#define SDIOD_CCCR_F1BLKSIZE_0     ( (uint32_t) 0x110 )    /* Function 1 (Backplane) SDIO Block Size Register 0 (LSB) */
#define SDIOD_CCCR_F1BLKSIZE_1     ( (uint32_t) 0x111 )    /* Function 1 (Backplane) SDIO Block Size Register 1 (MSB) */
#define SDIOD_CCCR_F2INFO          ( (uint32_t) 0x200 )    /* Function 2 (WLAN Data FIFO) Info */
#define SDIOD_CCCR_F2HP            ( (uint32_t) 0x202 )    /* Function 2 (WLAN Data FIFO) High Power */
#define SDIOD_CCCR_F2CISPTR_0      ( (uint32_t) 0x209 )    /* Function 2 (WLAN Data FIFO) CIS Base Address Pointer Register 0 (LSB) */
#define SDIOD_CCCR_F2CISPTR_1      ( (uint32_t) 0x20A )    /* Function 2 (WLAN Data FIFO) CIS Base Address Pointer Register 1       */
#define SDIOD_CCCR_F2CISPTR_2      ( (uint32_t) 0x20B )    /* Function 2 (WLAN Data FIFO) CIS Base Address Pointer Register 2 (MSB - only bit 1 valid) */
#define SDIOD_CCCR_F2BLKSIZE_0     ( (uint32_t) 0x210 )    /* Function 2 (WLAN Data FIFO) SDIO Block Size Register 0 (LSB) */
#define SDIOD_CCCR_F2BLKSIZE_1     ( (uint32_t) 0x211 )    /* Function 2 (WLAN Data FIFO) SDIO Block Size Register 1 (MSB) */
#define SDIOD_CCCR_F3INFO          ( (uint32_t) 0x300 )    /* Function 3 (Bluetooth Data FIFO) Info */
#define SDIOD_CCCR_F3HP            ( (uint32_t) 0x302 )    /* Function 3 (Bluetooth Data FIFO) High Power */
#define SDIOD_CCCR_F3CISPTR_0      ( (uint32_t) 0x309 )    /* Function 3 (Bluetooth Data FIFO) CIS Base Address Pointer Register 0 (LSB) */
#define SDIOD_CCCR_F3CISPTR_1      ( (uint32_t) 0x30A )    /* Function 3 (Bluetooth Data FIFO) CIS Base Address Pointer Register 1       */
#define SDIOD_CCCR_F3CISPTR_2      ( (uint32_t) 0x30B )    /* Function 3 (Bluetooth Data FIFO) CIS Base Address Pointer Register 2 (MSB - only bit 1 valid) */
#define SDIOD_CCCR_F3BLKSIZE_0     ( (uint32_t) 0x310 )    /* Function 3 (Bluetooth Data FIFO) SDIO Block Size Register 0 (LSB) */
#define SDIOD_CCCR_F3BLKSIZE_1     ( (uint32_t) 0x311 )    /* Function 3 (Bluetooth Data FIFO) SDIO Block Size Register 1 (MSB) */


/* SDIO Function 1 (Backplane) register addresses */
/* Addresses 0x00000000 - 0x0000FFFF are directly access the backplane
 * throught the backplane window. Addresses above 0x0000FFFF are
 * registers relating to backplane access, and do not require a backpane
 * clock to access them
 */
#define SDIO_GPIO_SELECT              ( (uint32_t) 0x10005 )
#define SDIO_GPIO_OUTPUT              ( (uint32_t) 0x10006 )
#define SDIO_GPIO_ENABLE              ( (uint32_t) 0x10007 )
#define SDIO_FUNCTION2_WATERMARK      ( (uint32_t) 0x10008 )
#define SDIO_DEVICE_CONTROL           ( (uint32_t) 0x10009 )
#define SDIO_BACKPLANE_ADDRESS_LOW    ( (uint32_t) 0x1000A )
#define SDIO_BACKPLANE_ADDRESS_MID    ( (uint32_t) 0x1000B )
#define SDIO_BACKPLANE_ADDRESS_HIGH   ( (uint32_t) 0x1000C )
#define SDIO_FRAME_CONTROL            ( (uint32_t) 0x1000D )
#define SDIO_CHIP_CLOCK_CSR           ( (uint32_t) 0x1000E )
#define SDIO_PULL_UP                  ( (uint32_t) 0x1000F )
#define SDIO_READ_FRAME_BC_LOW        ( (uint32_t) 0x1001B )
#define SDIO_READ_FRAME_BC_HIGH       ( (uint32_t) 0x1001C )
#define SDIO_MES_BUSY_CTRL            ( (uint32_t) 0x1001D )
#define SDIO_WAKEUP_CTRL              ( (uint32_t) 0x1001E )
#define SDIO_SLEEP_CSR                ( (uint32_t) 0x1001F )

#define I_HMB_SW_MASK                 ( (uint32_t) 0x000000F0 )
#define I_HMB_FRAME_IND               ( 1 << 6 )
#define I_HMB_HOST_INT                ( 1 << 7 )

#define FRAME_AVAILABLE_MASK          I_HMB_SW_MASK

/******************************************************
 *             SPI Constants
 ******************************************************/
/* GSPI v1 */
#define SPI_FRAME_CONTROL              ( (uint32_t) 0x1000D )

/* Register addresses */
#define SPI_BUS_CONTROL                ( (uint32_t) 0x0000 )
#define SPI_RESPONSE_DELAY             ( (uint32_t) 0x0001 )
#define SPI_STATUS_ENABLE              ( (uint32_t) 0x0002 )
#define SPI_RESET_BP                   ( (uint32_t) 0x0003 )    /* (corerev >= 1) */
#define SPI_INTERRUPT_REGISTER         ( (uint32_t) 0x0004 )    /* 16 bits - Interrupt status */
#define SPI_INTERRUPT_ENABLE_REGISTER  ( (uint32_t) 0x0006 )    /* 16 bits - Interrupt mask */
#define SPI_STATUS_REGISTER            ( (uint32_t) 0x0008 )    /* 32 bits */
#define SPI_FUNCTION1_INFO             ( (uint32_t) 0x000C )    /* 16 bits */
#define SPI_FUNCTION2_INFO             ( (uint32_t) 0x000E )    /* 16 bits */
#define SPI_FUNCTION3_INFO             ( (uint32_t) 0x0010 )    /* 16 bits */
#define SPI_READ_TEST_REGISTER         ( (uint32_t) 0x0014 )    /* 32 bits */
#define SPI_RESP_DELAY_F0              ( (uint32_t) 0x001c )    /* 8 bits (corerev >= 3) */
#define SPI_RESP_DELAY_F1              ( (uint32_t) 0x001d )    /* 8 bits (corerev >= 3) */
#define SPI_RESP_DELAY_F2              ( (uint32_t) 0x001e )    /* 8 bits (corerev >= 3) */
#define SPI_RESP_DELAY_F3              ( (uint32_t) 0x001f )    /* 8 bits (corerev >= 3) */

/******************************************************
 *             Bit Masks
 ******************************************************/

/* SDIOD_CCCR_REV Bits */
#define SDIO_REV_SDIOID_MASK       ( (uint32_t)  0xF0 )    /* SDIO spec revision number */
#define SDIO_REV_CCCRID_MASK       ( (uint32_t)  0x0F )    /* CCCR format version number */

/* SDIOD_CCCR_SDREV Bits */
#define SD_REV_PHY_MASK            ( (uint32_t)  0x0F )    /* SD format version number */

/* SDIOD_CCCR_IOEN Bits */
#define SDIO_FUNC_ENABLE_1         ( (uint32_t)  0x02 )    /* function 1 I/O enable */
#define SDIO_FUNC_ENABLE_2         ( (uint32_t)  0x04 )    /* function 2 I/O enable */
#define SDIO_FUNC_ENABLE_3         ( (uint32_t)  0x08 )    /* function 3 I/O enable */

/* SDIOD_CCCR_IORDY Bits */
#define SDIO_FUNC_READY_1          ( (uint32_t)  0x02 )    /* function 1 I/O ready */
#define SDIO_FUNC_READY_2          ( (uint32_t)  0x04 )    /* function 2 I/O ready */
#define SDIO_FUNC_READY_3          ( (uint32_t)  0x08 )    /* function 3 I/O ready */

/* SDIOD_CCCR_INTEN Bits */
#define INTR_CTL_MASTER_EN         ( (uint32_t)  0x01 )    /* interrupt enable master */
#define INTR_CTL_FUNC1_EN          ( (uint32_t)  0x02 )    /* interrupt enable for function 1 */
#define INTR_CTL_FUNC2_EN          ( (uint32_t)  0x04 )    /* interrupt enable for function 2 */

/* SDIOD_SEP_INT_CTL Bits */
#define SEP_INTR_CTL_MASK          ( (uint32_t)  0x01 )    /* out-of-band interrupt mask */
#define SEP_INTR_CTL_EN            ( (uint32_t)  0x02 )    /* out-of-band interrupt output enable */
#define SEP_INTR_CTL_POL           ( (uint32_t)  0x04 )    /* out-of-band interrupt polarity */

/* SDIOD_CCCR_INTPEND Bits */
#define INTR_STATUS_FUNC1          ( (uint32_t)  0x02 )    /* interrupt pending for function 1 */
#define INTR_STATUS_FUNC2          ( (uint32_t)  0x04 )    /* interrupt pending for function 2 */
#define INTR_STATUS_FUNC3          ( (uint32_t)  0x08 )    /* interrupt pending for function 3 */

/* SDIOD_CCCR_IOABORT Bits */
#define IO_ABORT_RESET_ALL         ( (uint32_t)  0x08 )    /* I/O card reset */
#define IO_ABORT_FUNC_MASK         ( (uint32_t)  0x07 )    /* abort selction: function x */

/* SDIOD_CCCR_BICTRL Bits */
#define BUS_CARD_DETECT_DIS        ( (uint32_t)  0x80 )    /* Card Detect disable */
#define BUS_SPI_CONT_INTR_CAP      ( (uint32_t)  0x40 )    /* support continuous SPI interrupt */
#define BUS_SPI_CONT_INTR_EN       ( (uint32_t)  0x20 )    /* continuous SPI interrupt enable */
#define BUS_SD_DATA_WIDTH_MASK     ( (uint32_t)  0x03 )    /* bus width mask */
#define BUS_SD_DATA_WIDTH_4BIT     ( (uint32_t)  0x02 )    /* bus width 4-bit mode */
#define BUS_SD_DATA_WIDTH_1BIT     ( (uint32_t)  0x00 )    /* bus width 1-bit mode */

/* SDIOD_CCCR_CAPABLITIES Bits */
#define SDIO_CAP_4BLS              ( (uint32_t)  0x80 )    /* 4-bit support for low speed card */
#define SDIO_CAP_LSC               ( (uint32_t)  0x40 )    /* low speed card */
#define SDIO_CAP_E4MI              ( (uint32_t)  0x20 )    /* enable interrupt between block of data in 4-bit mode */
#define SDIO_CAP_S4MI              ( (uint32_t)  0x10 )    /* support interrupt between block of data in 4-bit mode */
#define SDIO_CAP_SBS               ( (uint32_t)  0x08 )    /* support suspend/resume */
#define SDIO_CAP_SRW               ( (uint32_t)  0x04 )    /* support read wait */
#define SDIO_CAP_SMB               ( (uint32_t)  0x02 )    /* support multi-block transfer */
#define SDIO_CAP_SDC               ( (uint32_t)  0x01 )    /* Support Direct commands during multi-byte transfer */

/* SDIOD_CCCR_POWER_CONTROL Bits */
#define SDIO_POWER_SMPC            ( (uint32_t)  0x01 )    /* supports master power control (RO) */
#define SDIO_POWER_EMPC            ( (uint32_t)  0x02 )    /* enable master power control (allow > 200mA) (RW) */

/* SDIOD_CCCR_SPEED_CONTROL Bits */
#define SDIO_SPEED_SHS             ( (uint32_t)  0x01 )    /* supports high-speed [clocking] mode (RO) */
#define SDIO_SPEED_EHS             ( (uint32_t)  0x02 )    /* enable high-speed [clocking] mode (RW) */

/* SDIOD_CCCR_BRCM_CARDCAP Bits */
#define SDIOD_CCCR_BRCM_CARDCAP_CMD14_SUPPORT ( (uint32_t)  0x02 )  /* Supports CMD14 */
#define SDIOD_CCCR_BRCM_CARDCAP_CMD14_EXT     ( (uint32_t)  0x04 )  /* CMD14 is allowed in FSM command state */
#define SDIOD_CCCR_BRCM_CARDCAP_CMD_NODEC     ( (uint32_t)  0x08 )  /* sdiod_aos does not decode any command */

/* SDIO_FUNCTION_INT_MASK Bits*/
#define SDIO_FUNC_MASK_F1          ( (uint32_t)  0x01 )    /* interrupt mask enable for function 1 */
#define SDIO_FUNC_MASK_F2          ( (uint32_t)  0x02 )    /* interrupt mask enable for function 2 */

/* GSPI */
#define SPI_READ_TEST_REGISTER_VALUE   ( (uint32_t) 0xFEEDBEAD )
#define SPI_READ_TEST_REG_LSB          ( ( ( SPI_READ_TEST_REGISTER_VALUE      )     ) & 0xff )
#define SPI_READ_TEST_REG_LSB_SFT1     ( ( ( SPI_READ_TEST_REGISTER_VALUE << 1 )     ) & 0xff )
#define SPI_READ_TEST_REG_LSB_SFT2     ( ( ( SPI_READ_TEST_REGISTER_VALUE << 1 ) + 1 ) & 0xff )
#define SPI_READ_TEST_REG_LSB_SFT3     ( ( ( SPI_READ_TEST_REGISTER_VALUE +1  ) << 1 ) & 0xff )


/* SPI_BUS_CONTROL Bits */
#define WORD_LENGTH_32             ( (uint32_t)  0x01 )    /* 0/1 16/32 bit word length */
#define ENDIAN_BIG                 ( (uint32_t)  0x02 )    /* 0/1 Little/Big Endian */
#define CLOCK_PHASE                ( (uint32_t)  0x04 )    /* 0/1 clock phase delay */
#define CLOCK_POLARITY             ( (uint32_t)  0x08 )    /* 0/1 Idle state clock polarity is low/high */
#define HIGH_SPEED_MODE            ( (uint32_t)  0x10 )    /* 1/0 High Speed mode / Normal mode */
#define INTERRUPT_POLARITY_HIGH    ( (uint32_t)  0x20 )    /* 1/0 Interrupt active polarity is high/low */
#define WAKE_UP                    ( (uint32_t)  0x80 )    /* 0/1 Wake-up command from Host to WLAN */

/* SPI_RESPONSE_DELAY Bit mask */
#define RESPONSE_DELAY_MASK 0xFF    /* Configurable rd response delay in multiples of 8 bits */

/* SPI_STATUS_ENABLE Bits */
#define STATUS_ENABLE              ( (uint32_t)  0x01 )    /* 1/0 Status sent/not sent to host after read/write */
#define INTR_WITH_STATUS           ( (uint32_t)  0x02 )    /* 0/1 Do-not / do-interrupt if status is sent */
#define RESP_DELAY_ALL             ( (uint32_t)  0x04 )    /* Applicability of resp delay to F1 or all func's read */
#define DWORD_PKT_LEN_EN           ( (uint32_t)  0x08 )    /* Packet len denoted in dwords instead of bytes */
#define CMD_ERR_CHK_EN             ( (uint32_t)  0x20 )    /* Command error check enable */
#define DATA_ERR_CHK_EN            ( (uint32_t)  0x40 )    /* Data error check enable */



/* SPI_RESET_BP Bits*/
#define RESET_ON_WLAN_BP_RESET     ( (uint32_t)  0x04 )    /* enable reset for WLAN backplane */
#define RESET_SPI                  ( (uint32_t)  0x80 )    /* reset the above enabled logic */



/* SPI_INTERRUPT_REGISTER and SPI_INTERRUPT_ENABLE_REGISTER Bits */
#define DATA_UNAVAILABLE           ( (uint32_t) 0x0001 )   /* Requested data not available; Clear by writing a "1" */
#define F2_F3_FIFO_RD_UNDERFLOW    ( (uint32_t) 0x0002 )
#define F2_F3_FIFO_WR_OVERFLOW     ( (uint32_t) 0x0004 )
#define COMMAND_ERROR              ( (uint32_t) 0x0008 )   /* Cleared by writing 1 */
#define DATA_ERROR                 ( (uint32_t) 0x0010 )   /* Cleared by writing 1 */
#define F2_PACKET_AVAILABLE        ( (uint32_t) 0x0020 )
#define F3_PACKET_AVAILABLE        ( (uint32_t) 0x0040 )
#define F1_OVERFLOW                ( (uint32_t) 0x0080 )   /* Due to last write. Bkplane has pending write requests */
#define MISC_INTR0                 ( (uint32_t) 0x0100 )
#define MISC_INTR1                 ( (uint32_t) 0x0200 )
#define MISC_INTR2                 ( (uint32_t) 0x0400 )
#define MISC_INTR3                 ( (uint32_t) 0x0800 )
#define MISC_INTR4                 ( (uint32_t) 0x1000 )
#define F1_INTR                    ( (uint32_t) 0x2000 )
#define F2_INTR                    ( (uint32_t) 0x4000 )
#define F3_INTR                    ( (uint32_t) 0x8000 )




/* SPI_STATUS_REGISTER Bits */
#define STATUS_DATA_NOT_AVAILABLE  ( (uint32_t) 0x00000001 )
#define STATUS_UNDERFLOW           ( (uint32_t) 0x00000002 )
#define STATUS_OVERFLOW            ( (uint32_t) 0x00000004 )
#define STATUS_F2_INTR             ( (uint32_t) 0x00000008 )
#define STATUS_F3_INTR             ( (uint32_t) 0x00000010 )
#define STATUS_F2_RX_READY         ( (uint32_t) 0x00000020 )
#define STATUS_F3_RX_READY         ( (uint32_t) 0x00000040 )
#define STATUS_HOST_CMD_DATA_ERR   ( (uint32_t) 0x00000080 )
#define STATUS_F2_PKT_AVAILABLE    ( (uint32_t) 0x00000100 )
#define STATUS_F2_PKT_LEN_MASK     ( (uint32_t) 0x000FFE00 )
#define STATUS_F2_PKT_LEN_SHIFT    ( (uint32_t) 9          )
#define STATUS_F3_PKT_AVAILABLE    ( (uint32_t) 0x00100000 )
#define STATUS_F3_PKT_LEN_MASK     ( (uint32_t) 0xFFE00000 )
#define STATUS_F3_PKT_LEN_SHIFT    ( (uint32_t) 21         )





/* SDIO_CHIP_CLOCK_CSR Bits */
#define SBSDIO_FORCE_ALP           ( (uint32_t)  0x01 )    /* Force ALP request to backplane */
#define SBSDIO_FORCE_HT            ( (uint32_t)  0x02 )    /* Force HT request to backplane */
#define SBSDIO_FORCE_ILP           ( (uint32_t)  0x04 )    /* Force ILP request to backplane */
#define SBSDIO_ALP_AVAIL_REQ       ( (uint32_t)  0x08 )    /* Make ALP ready (power up xtal) */
#define SBSDIO_HT_AVAIL_REQ        ( (uint32_t)  0x10 )    /* Make HT ready (power up PLL) */
#define SBSDIO_FORCE_HW_CLKREQ_OFF ( (uint32_t)  0x20 )    /* Squelch clock requests from HW */
#define SBSDIO_ALP_AVAIL           ( (uint32_t)  0x40 )    /* Status: ALP is ready */
#define SBSDIO_HT_AVAIL            ( (uint32_t)  0x80 )    /* Status: HT is ready */
#define SBSDIO_Rev8_HT_AVAIL       ( (uint32_t)  0x40 )
#define SBSDIO_Rev8_ALP_AVAIL      ( (uint32_t)  0x80 )


/* SDIO_FRAME_CONTROL Bits */
#define SFC_RF_TERM                ( (uint32_t) (1 << 0) ) /* Read Frame Terminate */
#define SFC_WF_TERM                ( (uint32_t) (1 << 1) ) /* Write Frame Terminate */
#define SFC_CRC4WOOS               ( (uint32_t) (1 << 2) ) /* HW reports CRC error for write out of sync */
#define SFC_ABORTALL               ( (uint32_t) (1 << 3) ) /* Abort cancels all in-progress frames */

/* SDIO_TO_SB_MAIL_BOX bits corresponding to intstatus bits */
#define SMB_NAK                    ( (uint32_t) (1 << 0) ) /* To SB Mailbox Frame NAK */
#define SMB_INT_ACK                ( (uint32_t) (1 << 1) ) /* To SB Mailbox Host Interrupt ACK */
#define SMB_USE_OOB                ( (uint32_t) (1 << 2) ) /* To SB Mailbox Use OOB Wakeup */
#define SMB_DEV_INT                ( (uint32_t) (1 << 3) ) /* To SB Mailbox Miscellaneous Interrupt */

/* SDIO_WAKEUP_CTRL bits */
#define SBSDIO_WCTRL_WAKE_TILL_ALP_AVAIL     ( (uint32_t) (1 << 0) ) /* WakeTillAlpAvail bit */
#define SBSDIO_WCTRL_WAKE_TILL_HT_AVAIL      ( (uint32_t) (1 << 1) ) /* WakeTillHTAvail bit */

/* SDIO_SLEEP_CSR bits */
#define SBSDIO_SLPCSR_KEEP_SDIO_ON           ( (uint32_t) (1 << 0) ) /* KeepSdioOn bit */
#define SBSDIO_SLPCSR_DEVICE_ON              ( (uint32_t) (1 << 1) ) /* DeviceOn bit */

#define WL_CHANSPEC_CTL_SB_LLL      0x0000
#define WL_CHANSPEC_CTL_SB_LLU      0x0100
#define WL_CHANSPEC_CTL_SB_LUL      0x0200
#define WL_CHANSPEC_CTL_SB_LUU      0x0300
#define WL_CHANSPEC_CTL_SB_ULL      0x0400
#define WL_CHANSPEC_CTL_SB_ULU      0x0500
#define WL_CHANSPEC_CTL_SB_UUL      0x0600
#define WL_CHANSPEC_CTL_SB_UUU      0x0700
#define WL_CHANSPEC_CTL_SB_LL       WL_CHANSPEC_CTL_SB_LLL
#define WL_CHANSPEC_CTL_SB_LU       WL_CHANSPEC_CTL_SB_LLU
#define WL_CHANSPEC_CTL_SB_UL       WL_CHANSPEC_CTL_SB_LUL
#define WL_CHANSPEC_CTL_SB_UU       WL_CHANSPEC_CTL_SB_LUU
#define WL_CHANSPEC_CTL_SB_L        WL_CHANSPEC_CTL_SB_LLL
#define WL_CHANSPEC_CTL_SB_U        WL_CHANSPEC_CTL_SB_LLU
#define WL_CHANSPEC_CTL_SB_LOWER    WL_CHANSPEC_CTL_SB_LLL
#define WL_CHANSPEC_CTL_SB_UPPER    WL_CHANSPEC_CTL_SB_LLU
#define WL_CHANSPEC_CTL_SB_NONE     WL_CHANSPEC_CTL_SB_LLL
#define WL_CHANSPEC_BAND_MASK       0xc000
#define WL_CHANSPEC_BAND_SHIFT        14
#define WL_CHANSPEC_BAND_5G        0xc000
#define WL_CHANSPEC_BAND_2G        0x0000
#define INVCHANSPEC            255

#define WL_CHANSPEC_CTL_SB_MASK        0x0700
#define WL_CHANSPEC_CTL_SB_SHIFT         8
#define WL_CHANSPEC_CTL_SB_LOWER    WL_CHANSPEC_CTL_SB_LLL
#define WL_CHANSPEC_CTL_SB_UPPER    WL_CHANSPEC_CTL_SB_LLU
 #define WL_CHANSPEC_CTL_SB_NONE        WL_CHANSPEC_CTL_SB_LLL
#define WL_CHANSPEC_BW_MASK        0x3800
#define WL_CHANSPEC_BW_SHIFT            11
#define WL_CHANSPEC_BW_10        0x0800
#define WL_CHANSPEC_BW_20        0x1000
#define WL_CHANSPEC_BW_40        0x1800

/* CIS accesses require backpane clock */


#undef CHIP_HAS_BSSID_CNT_IN_ASSOC_PARAMS
#undef CHIP_FIRMWARE_SUPPORTS_PM_LIMIT_IOVAR

struct ether_addr;
struct wl_join_scan_params;

typedef struct wl_assoc_params
{
    struct ether_addr bssid;
#ifdef CHIP_HAS_BSSID_CNT_IN_ASSOC_PARAMS
    uint16_t    bssid_cnt;
#endif /* ifdef CHIP_HAS_BSSID_CNT_IN_ASSOC_PARAMS */
    uint32_t     chanspec_num;
    chanspec_t  chanspec_list[1];
} wl_assoc_params_t;
#define WL_ASSOC_PARAMS_FIXED_SIZE     (sizeof(wl_assoc_params_t) - sizeof(wl_chanspec_t))
typedef wl_assoc_params_t wl_reassoc_params_t;
#define WL_REASSOC_PARAMS_FIXED_SIZE    WL_ASSOC_PARAMS_FIXED_SIZE
typedef wl_assoc_params_t wl_join_assoc_params_t;
#define WL_JOIN_ASSOC_PARAMS_FIXED_SIZE        WL_ASSOC_PARAMS_FIXED_SIZE
typedef struct wl_join_params
{
    wlc_ssid_t         ssid;
    struct wl_assoc_params  params;
} wl_join_params_t;
#define WL_JOIN_PARAMS_FIXED_SIZE     (sizeof(wl_join_params_t) - sizeof(wl_chanspec_t))

/* extended join params */
typedef struct wl_extjoin_params {
    wlc_ssid_t ssid;                /* {0, ""}: wildcard scan */
    struct wl_join_scan_params scan_params;
    wl_join_assoc_params_t assoc_params;   /* optional field, but it must include the fixed portion
                     * of the wl_join_assoc_params_t struct when it does
                     * present.
                     */
} wl_extjoin_params_t;
#define WL_EXTJOIN_PARAMS_FIXED_SIZE    (sizeof(wl_extjoin_params_t) - sizeof(chanspec_t))
#define WL_COUNTERS_SZ  1400

/** Top structure of counters IOVar buffer */
typedef struct {
    uint16_t  version;              /**< see definition of WL_CNT_T_VERSION */
    uint16_t datalen;              /**< length of data including all paddings. */
    uint8_t  data [WL_COUNTERS_SZ];/**< variable length payload:
                                     * 1 or more bcm_xtlv_t type of tuples.
                                     * each tuple is padded to multiple of 4 bytes.
                                     * 'datalen' field of this structure includes all paddings.
                                   */
} wl_cnt_info_t;

typedef wl_cnt_info_t  wiced_counters_t;

/* XXX When new reason codes are added to list, Please update wl_reinit_names also */
/* Reinit reason codes */
enum {
    WL_REINIT_RC_NONE             = 0,
    WL_REINIT_RC_PS_SYNC          = 1,
    WL_REINIT_RC_PSM_WD           = 2,
    WL_REINIT_RC_MAC_WAKE         = 3,
    WL_REINIT_RC_MAC_SUSPEND      = 4,
    WL_REINIT_RC_MAC_SPIN_WAIT    = 5,
    WL_REINIT_RC_AXI_BUS_ERROR    = 6,
    WL_REINIT_RC_DEVICE_REMOVED   = 7,
    WL_REINIT_RC_PCIE_FATAL_ERROR = 8,
    WL_REINIT_RC_OL_FW_TRAP       = 9,
    WL_REINIT_RC_FIFO_ERR         = 10,
    WL_REINIT_RC_INV_TX_STATUS    = 11,
    WL_REINIT_RC_MQ_ERROR         = 12,
    WL_REINIT_RC_PHYTXERR_THRESH  = 13,
    WL_REINIT_RC_USER_FORCED      = 14,
    WL_REINIT_RC_FULL_RESET       = 15,
    WL_REINIT_RC_AP_BEACON        = 16,
    WL_REINIT_RC_PM_EXCESSED      = 17,
    WL_REINIT_RC_NO_CLK           = 18,
    WL_REINIT_RC_SW_ASSERT        = 19,
    WL_REINIT_RC_PSM_JMP0         = 20,
    WL_REINIT_RC_PSM_RUN          = 21,
    WL_REINIT_RC_ENABLE_MAC       = 22,
    WL_REINIT_RC_SCAN_TIMEOUT     = 23,
    WL_REINIT_RC_JOIN_TIMEOUT     = 24,
    /* Below error codes are generated during D3 exit validation */
    WL_REINIT_RC_LINK_NOT_ACTIVE  = 25,
    WL_REINIT_RC_PCI_CFG_RD_FAIL  = 26,
    WL_REINIT_RC_INV_VEN_ID       = 27,
    WL_REINIT_RC_INV_DEV_ID       = 28,
    WL_REINIT_RC_INV_BAR0         = 29,
    WL_REINIT_RC_INV_BAR2         = 30,
    WL_REINIT_RC_AER_UC_FATAL     = 31,
    WL_REINIT_RC_AER_UC_NON_FATAL = 32,
    WL_REINIT_RC_AER_CORR         = 33,
    WL_REINIT_RC_AER_DEV_STS      = 34,
    WL_REINIT_RC_PCIe_STS         = 35,
    WL_REINIT_RC_MMIO_RD_FAIL     = 36,
    WL_REINIT_RC_MMIO_RD_INVAL    = 37,
    WL_REINIT_RC_MMIO_ARM_MEM_RD_FAIL = 38,
    WL_REINIT_RC_MMIO_ARM_MEM_INVAL   = 39,
    WL_REINIT_RC_SROM_LOAD_FAILED     = 40,
    WL_REINIT_RC_PHY_CRASH            = 41,
    WL_REINIT_TX_STALL                = 42,
    WL_REINIT_RC_TX_FLOW_CONTROL_BLOCKED    = 43,
    WL_REINIT_RC_RX_HC_FAIL           = 44,
    WL_REINIT_RC_RX_DMA_STALL         = 45,
    WL_REINIT_UTRACE_BUF_OVERLAP_SR   = 46,
    WL_REINIT_UTRACE_TPL_OUT_BOUNDS   = 47,
    WL_REINIT_UTRACE_TPL_OSET_STRT0   = 48,
    WL_REINIT_RC_PHYTXERR             = 49,
    WL_REINIT_RC_PSM_FATAL_SUSP       = 50,
    WL_REINIT_RC_TX_FIFO_SUSP         = 51,
    WL_REINIT_RC_MAC_ENABLE       = 52,
    WL_REINIT_RC_LAST   /* This must be the last entry */
};

#define NREINITREASONCOUNT  8
/* XXX NREINITREASONCOUNT is 8 in other branches.
 * Any change to this will break wl tool compatibility with other branches
 * #define NREINITREASONCOUNT   WL_REINIT_RC_LAST
 */

#define REINITRSNIDX(_x)    (((_x) < WL_REINIT_RC_LAST) ? (_x) : 0)


/** wlc layer counters */
typedef struct {
    /* transmit stat counters */
    uint32_t  txframe;    /**< tx data frames */
    uint32_t  txbyte;     /**< tx data bytes */
    uint32_t  txretrans;  /**< tx mac retransmits */
    uint32_t  txerror;    /**< tx data errors (derived: sum of others) */
    uint32_t  txctl;      /**< tx management frames */
    uint32_t  txprshort;  /**< tx short preamble frames */
    uint32_t  txserr;     /**< tx status errors */
    uint32_t  txnobuf;    /**< tx out of buffers errors */
    uint32_t  txnoassoc;  /**< tx discard because we're not associated */
    uint32_t  txrunt;     /**< tx runt frames */
    uint32_t  txchit;     /**< tx header cache hit (fastpath) */
    uint32_t  txcmiss;    /**< tx header cache miss (slowpath) */

    /* transmit chip error counters */
    uint32_t  txuflo;     /**< tx fifo underflows */
    uint32_t  txphyerr;   /**< tx phy errors (indicated in tx status) */
    uint32_t  txphycrs;   /**< PR8861/8963 counter */

    /* receive stat counters */
    uint32_t  rxframe;    /**< rx data frames */
    uint32_t  rxbyte;     /**< rx data bytes */
    uint32_t  rxerror;    /**< rx data errors (derived: sum of others) */
    uint32_t  rxctl;      /**< rx management frames */
    uint32_t  rxnobuf;    /**< rx out of buffers errors */
    uint32_t  rxnondata;  /**< rx non data frames in the data channel errors */
    uint32_t  rxbadds;    /**< rx bad DS errors */
    uint32_t  rxbadcm;    /**< rx bad control or management frames */
    uint32_t  rxfragerr;  /**< rx fragmentation errors */
    uint32_t  rxrunt;     /**< rx runt frames */
    uint32_t  rxgiant;    /**< rx giant frames */
    uint32_t  rxnoscb;    /**< rx no scb error */
    uint32_t  rxbadproto; /**< rx invalid frames */
    uint32_t  rxbadsrcmac;    /**< rx frames with Invalid Src Mac */
    uint32_t  rxbadda;    /**< rx frames tossed for invalid da */
    uint32_t  rxfilter;   /**< rx frames filtered out */

    /* receive chip error counters */
    uint32_t  rxoflo;     /**< rx fifo overflow errors */
    uint32_t  rxuflo[NFIFO];  /**< rx dma descriptor underflow errors */

    uint32_t  d11cnt_txrts_off;   /**< d11cnt txrts value when reset d11cnt */
    uint32_t  d11cnt_rxcrc_off;   /**< d11cnt rxcrc value when reset d11cnt */
    uint32_t  d11cnt_txnocts_off; /**< d11cnt txnocts value when reset d11cnt */

    /* misc counters */
    uint32_t  dmade;      /**< tx/rx dma descriptor errors */
    uint32_t  dmada;      /**< tx/rx dma data errors */
    uint32_t  dmape;      /**< tx/rx dma descriptor protocol errors */
    uint32_t  reset;      /**< reset count */
    uint32_t  tbtt;       /**< cnts the TBTT int's */
    uint32_t  txdmawar;   /**< # occurrences of PR15420 workaround */
    uint32_t  pkt_callback_reg_fail;  /**< callbacks register failure */

    /* 802.11 MIB counters, pp. 614 of 802.11 reaff doc. */
    uint32_t  txfrag;     /**< dot11TransmittedFragmentCount */
    uint32_t  txmulti;    /**< dot11MulticastTransmittedFrameCount */
    uint32_t  txfail;     /**< dot11FailedCount */
    uint32_t  txretry;    /**< dot11RetryCount */
    uint32_t  txretrie;   /**< dot11MultipleRetryCount */
    uint32_t  rxdup;      /**< dot11FrameduplicateCount */
    uint32_t  txrts;      /**< dot11RTSSuccessCount */
    uint32_t  txnocts;    /**< dot11RTSFailureCount */
    uint32_t  txnoack;    /**< dot11ACKFailureCount */
    uint32_t  rxfrag;     /**< dot11ReceivedFragmentCount */
    uint32_t  rxmulti;    /**< dot11MulticastReceivedFrameCount */
    uint32_t  rxcrc;      /**< dot11FCSErrorCount */
    uint32_t  txfrmsnt;   /**< dot11TransmittedFrameCount (bogus MIB?) */
    uint32_t  rxundec;    /**< dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t  tkipmicfaill;   /**< TKIPLocalMICFailures */
    uint32_t  tkipcntrmsr;    /**< TKIPCounterMeasuresInvoked */
    uint32_t  tkipreplay; /**< TKIPReplays */
    uint32_t  ccmpfmterr; /**< CCMPFormatErrors */
    uint32_t  ccmpreplay; /**< CCMPReplays */
    uint32_t  ccmpundec;  /**< CCMPDecryptErrors */
    uint32_t  fourwayfail;    /**< FourWayHandshakeFailures */
    uint32_t  wepundec;   /**< dot11WEPUndecryptableCount */
    uint32_t  wepicverr;  /**< dot11WEPICVErrorCount */
    uint32_t  decsuccess; /**< DecryptSuccessCount */
    uint32_t  tkipicverr; /**< TKIPICVErrorCount */
    uint32_t  wepexcluded;    /**< dot11WEPExcludedCount */

    uint32_t  txchanrej;  /**< Tx frames suppressed due to channel rejection */
    uint32_t  psmwds;     /**< Count PSM watchdogs */
    uint32_t  phywatchdog;    /**< Count Phy watchdogs (triggered by ucode) */

    /* MBSS counters, AP only */
    uint32_t  prq_entries_handled;    /**< PRQ entries read in */
    uint32_t  prq_undirected_entries; /**<    which were bcast bss & ssid */
    uint32_t  prq_bad_entries;    /**<    which could not be translated to info */
    uint32_t  atim_suppress_count;    /**< TX suppressions on ATIM fifo */
    uint32_t  bcn_template_not_ready; /**< Template marked in use on send bcn ... */
    uint32_t  bcn_template_not_ready_done; /**< ...but "DMA done" interrupt rcvd */
    uint32_t  late_tbtt_dpc;  /**< TBTT DPC did not happen in time */

    /* per-rate receive stat counters */
    uint32_t  rx1mbps;    /**< packets rx at 1Mbps */
    uint32_t  rx2mbps;    /**< packets rx at 2Mbps */
    uint32_t  rx5mbps5;   /**< packets rx at 5.5Mbps */
    uint32_t  rx6mbps;    /**< packets rx at 6Mbps */
    uint32_t  rx9mbps;    /**< packets rx at 9Mbps */
    uint32_t  rx11mbps;   /**< packets rx at 11Mbps */
    uint32_t  rx12mbps;   /**< packets rx at 12Mbps */
    uint32_t  rx18mbps;   /**< packets rx at 18Mbps */
    uint32_t  rx24mbps;   /**< packets rx at 24Mbps */
    uint32_t  rx36mbps;   /**< packets rx at 36Mbps */
    uint32_t  rx48mbps;   /**< packets rx at 48Mbps */
    uint32_t  rx54mbps;   /**< packets rx at 54Mbps */
    uint32_t  rx108mbps;  /**< packets rx at 108mbps */
    uint32_t  rx162mbps;  /**< packets rx at 162mbps */
    uint32_t  rx216mbps;  /**< packets rx at 216 mbps */
    uint32_t  rx270mbps;  /**< packets rx at 270 mbps */
    uint32_t  rx324mbps;  /**< packets rx at 324 mbps */
    uint32_t  rx378mbps;  /**< packets rx at 378 mbps */
    uint32_t  rx432mbps;  /**< packets rx at 432 mbps */
    uint32_t  rx486mbps;  /**< packets rx at 486 mbps */
    uint32_t  rx540mbps;  /**< packets rx at 540 mbps */

    uint32_t  rfdisable;  /**< count of radio disables */

    uint32_t  txexptime;  /**< Tx frames suppressed due to timer expiration */

    uint32_t  txmpdu_sgi; /**< count for sgi transmit */
    uint32_t  rxmpdu_sgi; /**< count for sgi received */
    uint32_t  txmpdu_stbc;    /**< count for stbc transmit */
    uint32_t  rxmpdu_stbc;    /**< count for stbc received */

    uint32_t  rxundec_mcst;   /**< dot11WEPUndecryptableCount */

    /* WPA2 counters (see rxundec for DecryptFailureCount) */
    uint32_t  tkipmicfaill_mcst;  /**< TKIPLocalMICFailures */
    uint32_t  tkipcntrmsr_mcst;   /**< TKIPCounterMeasuresInvoked */
    uint32_t  tkipreplay_mcst;    /**< TKIPReplays */
    uint32_t  ccmpfmterr_mcst;    /**< CCMPFormatErrors */
    uint32_t  ccmpreplay_mcst;    /**< CCMPReplays */
    uint32_t  ccmpundec_mcst; /**< CCMPDecryptErrors */
    uint32_t  fourwayfail_mcst;   /**< FourWayHandshakeFailures */
    uint32_t  wepundec_mcst;  /**< dot11WEPUndecryptableCount */
    uint32_t  wepicverr_mcst; /**< dot11WEPICVErrorCount */
    uint32_t  decsuccess_mcst;    /**< DecryptSuccessCount */
    uint32_t  tkipicverr_mcst;    /**< TKIPICVErrorCount */
    uint32_t  wepexcluded_mcst;   /**< dot11WEPExcludedCount */

    uint32_t  dma_hang;   /**< count for dma hang */
    uint32_t  reinit;     /**< count for reinit */

    uint32_t  pstatxucast;    /**< count of ucast frames xmitted on all psta assoc */
    uint32_t  pstatxnoassoc;  /**< count of txnoassoc frames xmitted on all psta assoc */
    uint32_t  pstarxucast;    /**< count of ucast frames received on all psta assoc */
    uint32_t  pstarxbcmc; /**< count of bcmc frames received on all psta */
    uint32_t  pstatxbcmc; /**< count of bcmc frames transmitted on all psta */

    uint32_t  cso_passthrough; /**< hw cso required but passthrough */
    uint32_t  cso_normal; /**< hw cso hdr for normal process */
    uint32_t  chained;    /**< number of frames chained */
    uint32_t  chainedsz1; /**< number of chain size 1 frames */
    uint32_t  unchained;  /**< number of frames not chained */
    uint32_t  maxchainsz; /**< max chain size so far */
    uint32_t  currchainsz;    /**< current chain size */
    uint32_t  pciereset;  /**< Secondary Bus Reset issued by driver */
    uint32_t  cfgrestore; /**< configspace restore by driver */
    uint32_t  reinitreason[NREINITREASONCOUNT]; /**< reinitreason counters; 0: Unknown reason */
    uint32_t  rxrtry;
    uint32_t  rxmpdu_mu;      /**< Number of MU MPDUs received */

    /* detailed control/management frames */
    uint32_t  txbar;      /**< Number of TX BAR */
    uint32_t  rxbar;      /**< Number of RX BAR */
    uint32_t  txpspoll;   /**< Number of TX PS-poll */
    uint32_t  rxpspoll;   /**< Number of RX PS-poll */
    uint32_t  txnull;     /**< Number of TX NULL_DATA */
    uint32_t  rxnull;     /**< Number of RX NULL_DATA */
    uint32_t  txqosnull;  /**< Number of TX NULL_QoSDATA */
    uint32_t  rxqosnull;  /**< Number of RX NULL_QoSDATA */
    uint32_t  txassocreq; /**< Number of TX ASSOC request */
    uint32_t  rxassocreq; /**< Number of RX ASSOC request */
    uint32_t  txreassocreq;   /**< Number of TX REASSOC request */
    uint32_t  rxreassocreq;   /**< Number of RX REASSOC request */
    uint32_t  txdisassoc; /**< Number of TX DISASSOC */
    uint32_t  rxdisassoc; /**< Number of RX DISASSOC */
    uint32_t  txassocrsp; /**< Number of TX ASSOC response */
    uint32_t  rxassocrsp; /**< Number of RX ASSOC response */
    uint32_t  txreassocrsp;   /**< Number of TX REASSOC response */
    uint32_t  rxreassocrsp;   /**< Number of RX REASSOC response */
    uint32_t  txauth;     /**< Number of TX AUTH */
    uint32_t  rxauth;     /**< Number of RX AUTH */
    uint32_t  txdeauth;   /**< Number of TX DEAUTH */
    uint32_t  rxdeauth;   /**< Number of RX DEAUTH */
    uint32_t  txprobereq; /**< Number of TX probe request */
    uint32_t  rxprobereq; /**< Number of RX probe request */
    uint32_t  txprobersp; /**< Number of TX probe response */
    uint32_t  rxprobersp; /**< Number of RX probe response */
    uint32_t  txaction;   /**< Number of TX action frame */
    uint32_t  rxaction;   /**< Number of RX action frame */
    uint32_t  ampdu_wds;  /**< Number of AMPDU watchdogs */
    uint32_t  txlost;     /**< Number of lost packets reported in txs */
    uint32_t  txdatamcast;    /**< Number of TX multicast data packets */
    uint32_t  txdatabcast;    /**< Number of TX broadcast data packets */
    uint32_t  psmxwds;    /**< Number of PSMx watchdogs */
    uint32_t  rxback;
    uint32_t  txback;
    uint32_t  p2p_tbtt;   /**< Number of P2P TBTT Events */
    uint32_t  p2p_tbtt_miss;  /**< Number of P2P TBTT Events Miss */

    /* XXX: Do not remove or rename inthe middle of this struct.
     * All counter variables have to be of uint32.
     * Please follow the instruction in
     * http://hwnbu-twiki.sj.broadcom.com/bin/view/Mwgroup/WlCounters#Counter_Edition
     */
} wl_cnt_wlc_t;
/* Reinit reasons - do not put anything else other than reinit reasons here */
typedef struct {
    uint32_t rsn[WL_REINIT_RC_LAST];
} reinit_rsns_t;

/** MACSTAT counters for ucode (corerev >= 40) */
typedef struct {
    /* MAC counters: 32-bit version of d11.h's macstat_t */
    uint32_t  txallfrm;      /**< total number of frames sent, incl. Data, ACK, RTS, CTS,
                              * Control Management (includes retransmissions)
                              */
    uint32_t  txrtsfrm;      /**< number of RTS sent out by the MAC */
    uint32_t  txctsfrm;      /**< number of CTS sent out by the MAC */
    uint32_t  txackfrm;      /**< number of ACK frames sent out */
    uint32_t  txdnlfrm;      /**< number of Null-Data transmission generated from template  */
    uint32_t  txbcnfrm;      /**< beacons transmitted */
    uint32_t  txfunfl[6];    /**< per-fifo tx underflows */
    uint32_t  txampdu;       /**< number of AMPDUs transmitted */
    uint32_t  txmpdu;        /**< number of MPDUs transmitted */
    uint32_t  txtplunfl;     /**< Template underflows (mac was too slow to transmit ACK/CTS
                              * or BCN)
                              */
    uint32_t  txphyerror;    /**< Transmit phy error, type of error is reported in tx-status for
                              * driver enqueued frames
                              */
    uint32_t  pktengrxducast;/**< unicast frames rxed by the pkteng code */
    uint32_t  pktengrxdmcast;/**< multicast frames rxed by the pkteng code */
    uint32_t  rxfrmtoolong;  /**< Received frame longer than legal limit (2346 bytes) */
    uint32_t  rxfrmtooshrt;  /**< Received frame did not contain enough bytes for its frame type */
    uint32_t  rxanyerr;      /**< Any RX error that is not counted by other counters. */
    uint32_t  rxbadfcs;      /**< number of frames for which the CRC check failed in the MAC */
    uint32_t  rxbadplcp;     /**< parity check of the PLCP header failed */
    uint32_t  rxcrsglitch;   /**< PHY was able to correlate the preamble but not the header */
    uint32_t  rxstrt;        /**< Number of received frames with a good PLCP
                              * (i.e. passing parity check)
                              */
    uint32_t  rxdtucastmbss; /**< number of received DATA frames with good FCS and matching RA */
    uint32_t  rxmgucastmbss; /**< number of received mgmt frames with good FCS and matching RA */
    uint32_t  rxctlucast;    /**< number of received CNTRL frames with good FCS and matching RA */
    uint32_t  rxrtsucast;    /**< number of unicast RTS addressed to the MAC (good FCS) */
    uint32_t  rxctsucast;    /**< number of unicast CTS addressed to the MAC (good FCS) */
    uint32_t  rxackucast;    /**< number of ucast ACKS received (good FCS) */
    uint32_t  rxdtocast;     /**< number of received DATA frames (good FCS and not matching RA) */
    uint32_t  rxmgocast;     /**< number of received MGMT frames (good FCS and not matching RA) */
    uint32_t  rxctlocast;    /**< number of received CNTRL frame (good FCS and not matching RA) */
    uint32_t  rxrtsocast;    /**< number of received RTS not addressed to the MAC */
    uint32_t  rxctsocast;    /**< number of received CTS not addressed to the MAC */
    uint32_t  rxdtmcast;     /**< number of RX Data multicast frames received by the MAC */
    uint32_t  rxmgmcast;     /**< number of RX Management multicast frames received by the MAC */
    uint32_t  rxctlmcast;    /**< number of RX Control multicast frames received by the MAC
                              * (unlikely to see these)
                              */
    uint32_t  rxbeaconmbss;   /**< beacons received from member of BSS */
    uint32_t  rxdtucastobss; /**< number of unicast frames addressed to the MAC from
                              * other BSS (WDS FRAME)
                              */
    uint32_t  rxbeaconobss;  /**< beacons received from other BSS */
    uint32_t  rxrsptmout;    /**< number of response timeouts for transmitted frames
                              * expecting a response
                              */
    uint32_t  bcntxcancl;    /**< transmit beacons canceled due to receipt of beacon (IBSS) */
    uint32_t  rxnodelim;     /**< number of no valid delimiter detected by ampdu parser */
    uint32_t  rxf0ovfl;      /**< number of receive fifo 0 overflows */
    uint32_t  rxf1ovfl;      /**< number of receive fifo 1 overflows */
    uint32_t  rxhlovfl;      /**< number of length / header fifo overflows */
    uint32_t  missbcn_dbg;   /**< number of beacon missed to receive */
    uint32_t  pmqovfl;       /**< number of PMQ overflows */
    uint32_t  rxcgprqfrm;    /**< number of received Probe requests that made it into
                              * the PRQ fifo
                              */
    uint32_t  rxcgprsqovfl;  /**< Rx Probe Request Que overflow in the AP */
    uint32_t  txcgprsfail;   /**< Tx Probe Response Fail. AP sent probe response but did
                              * not get ACK
                              */
    uint32_t  txcgprssuc;    /**< Tx Probe Response Success (ACK was received) */
    uint32_t  prs_timeout;   /**< number of probe requests that were dropped from the PRQ
                              * fifo because a probe response could not be sent out within
                              * the time limit defined in M_PRS_MAXTIME
                              */
    uint32_t  txrtsfail;     /**< number of rts transmission failure that reach retry limit */
    uint32_t  txucast;       /**< number of unicast tx expecting response other than cts/cwcts */
    uint32_t  txinrtstxop;   /**< number of data frame transmissions during rts txop */
    uint32_t  rxback;        /**< blockack rxcnt */
    uint32_t  txback;        /**< blockack txcnt */
    uint32_t  bphy_rxcrsglitch;   /**< PHY count of bphy glitches */
    uint32_t  rxdrop20s;    /**< drop secondary cnt */
    uint32_t  rxtoolate;    /**< receive too late */
    uint32_t  bphy_badplcp; /**< number of bad PLCP reception on BPHY rate */
    /* XXX:  All counter variables have to be of uint32. */
} wl_cnt_ge40mcst_v1_t;

#ifdef __cplusplus
} /* extern "C" */
#endif


extern wwd_result_t wwd_unpack_wl_counters_xtlv( wiced_counters_t* data, reinit_rsns_t* reinit_rns, wl_cnt_wlc_t* wl_cnt_wlc, wl_cnt_ge40mcst_v1_t* wl_cnt_ge40 );
#endif /* ifndef INCLUDED_CHIP_CONSTANTS_H_ */
