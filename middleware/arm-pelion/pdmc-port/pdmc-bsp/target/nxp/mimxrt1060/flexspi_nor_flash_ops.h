
#ifndef FLEXSPI_NOR_FLASH_OPS_H
#define FLEXSPI_NOR_FLASH_OPS_H

#include "fsl_flexspi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLEXSPI_INSTANCE FLEXSPI
#define FLEXSPI_INSTANCE_AMBA_BASE FlexSPI_AMBA_BASE
#define FLEXSPI_INSTANCE_CLOCK kCLOCK_FlexSpi

#define START_ADDRESS (FLEXSPI_INSTANCE_AMBA_BASE)

#define FLASH_SIZE (8 * 1024 * 1024)
#define SECTOR_SIZE (4 * 1024)
#define FLASH_PAGE_SIZE (256)
#define ERASE_VALUE (0xFF)
#define FLASH_SIZE_KB (FLASH_SIZE / 1024)

#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL 7
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST 13
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD 0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS 1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE 2
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR 3
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD 4
#define NOR_CMD_LUT_SEQ_IDX_READID 8
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG 9
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI 10
#define NOR_CMD_LUT_SEQ_IDX_EXITQPI 11
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG 12
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP 5

#define CUSTOM_LUT_LENGTH 60
#define FLASH_QUAD_ENABLE 0x40
#define FLASH_BUSY_STATUS_POL 1
#define FLASH_BUSY_STATUS_OFFSET 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void flexspi_nor_flash_init(FLEXSPI_Type *base);

status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);

status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);

status_t flexspi_nor_erase_chip(FLEXSPI_Type *base);

status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);

status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);

#endif /* FLEXSPI_NOR_FLASH_OPS_H */
