/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "sln_flash.h"
#include "flexspi_hyper_flash_ops.h"
#include "fsl_flexspi.h"

extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];

#ifdef __REDLIB__
size_t safe_strlen(const char *ptr, size_t max)
{
    char *p = (char *)ptr;

    while (max && *p)
    {
        p++;
        max--;
    }

    return (size_t)(p - ptr);
}
#endif

void SLN_ram_memset(void *dst, uint8_t data, size_t len)
{
    uint8_t *ptr = (uint8_t *)dst;

    while (len)
    {
        *ptr++ = data;
        len--;
    }
}

void SLN_ram_memcpy(void *dst, void *src, size_t len)
{
    uint8_t *ptrDst = (uint8_t *)dst;
    uint8_t *ptrSrc = (uint8_t *)src;

    while (len)
    {
        *ptrDst++ = *ptrSrc++;
        len--;
    }
}

uint32_t SLN_ram_disable_irq(void)
{
    uint32_t regPrimask = 0;

    // Get primask
    __ASM volatile("MRS %0, primask" : "=r"(regPrimask)::"memory");

    // Disable irq
    __ASM volatile("cpsid i" : : : "memory");

    return regPrimask;
}

void SLN_ram_enable_irq(uint32_t priMask)
{
    __ASM volatile("MSR primask, %0" : : "r"(priMask) : "memory");
}

void SLN_ram_disable_d_cache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    register uint32_t ccsidr;
    register uint32_t sets;
    register uint32_t ways;

    SCB->CSSELR = 0U; /*(0U << 1U) | 0U;*/ /* Level 1 data cache */
    __ASM volatile("dsb 0xF" ::: "memory");

    SCB->CCR &= ~(uint32_t)SCB_CCR_DC_Msk; /* disable D-Cache */
    __ASM volatile("dsb 0xF" ::: "memory");

    ccsidr = SCB->CCSIDR;

    /* clean & invalidate D-Cache */
    sets = (uint32_t)(CCSIDR_SETS(ccsidr));
    do
    {
        ways = (uint32_t)(CCSIDR_WAYS(ccsidr));
        do
        {
            SCB->DCCISW = (((sets << SCB_DCCISW_SET_Pos) & SCB_DCCISW_SET_Msk) |
                           ((ways << SCB_DCCISW_WAY_Pos) & SCB_DCCISW_WAY_Msk));
#if defined(__CC_ARM)
            __schedule_barrier();
#endif
        } while (ways-- != 0U);
    } while (sets-- != 0U);

    __ASM volatile("dsb 0xF" ::: "memory");
    __ASM volatile("isb 0xF" ::: "memory");
#endif
}

void SLN_ram_enable_d_cache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    uint32_t ccsidr;
    uint32_t sets;
    uint32_t ways;

    SCB->CSSELR = 0U; /*(0U << 1U) | 0U;*/ /* Level 1 data cache */
    __ASM volatile("dsb 0xF" ::: "memory");

    ccsidr = SCB->CCSIDR;

    /* invalidate D-Cache */
    sets = (uint32_t)(CCSIDR_SETS(ccsidr));
    do
    {
        ways = (uint32_t)(CCSIDR_WAYS(ccsidr));
        do
        {
            SCB->DCISW =
                (((sets << SCB_DCISW_SET_Pos) & SCB_DCISW_SET_Msk) | ((ways << SCB_DCISW_WAY_Pos) & SCB_DCISW_WAY_Msk));
#if defined(__CC_ARM)
            __schedule_barrier();
#endif
        } while (ways-- != 0U);
    } while (sets-- != 0U);
    __ASM volatile("dsb 0xF" ::: "memory");

    SCB->CCR |= (uint32_t)SCB_CCR_DC_Msk; /* enable D-Cache */

    __ASM volatile("dsb 0xF" ::: "memory");
    __ASM volatile("isb 0xF" ::: "memory");
#endif
}

void SLN_Flash_Init(void)
{
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");
}

status_t SLN_Write_Flash_Page(uint32_t address, uint8_t *data, uint32_t len)
{
    status_t status = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    /* Setup page size write buffer */
    uint8_t tempPage[FLASH_PAGE_SIZE];

    SLN_ram_memset(tempPage, 0xFF, FLASH_PAGE_SIZE);

    SLN_ram_memcpy(tempPage, data, len);

    /* Program page. */
    status = flexspi_nor_flash_page_program_with_buffer(FLEXSPI, address, (void *)tempPage);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}

static status_t SLN_Write_Flash_Page2(uint32_t address, uint8_t *data, uint32_t len)
{
    status_t status = 0;

    /* Setup page size write buffer */
    uint8_t tempPage[FLASH_PAGE_SIZE];

    SLN_ram_memset(tempPage, 0xFF, FLASH_PAGE_SIZE);

    SLN_ram_memcpy(tempPage, data, len);

    /*Program page. */
    status = flexspi_nor_flash_page_program_with_buffer(FLEXSPI, address, (void *)tempPage);

    return status;
}

static status_t SLN_Erase_Sector2(uint32_t address)
{
    status_t status = 0;

    /* Erase sectors. */
    status = flexspi_nor_flash_erase_sector(FLEXSPI, address);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    return status;
}

int32_t SLN_Write_Sector(uint32_t address, uint8_t *buf, uint32_t len)
{
    int32_t status     = kStatus_Success;
    uint32_t pageCount = 0;
    uint32_t pageMod   = 0;
    uint32_t toCopy    = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    status = SLN_Erase_Sector2(address);

    if (kStatus_Success == status)
    {
        // Adjust total write length to fit in sector
        len = (SECTOR_SIZE < len) ? SECTOR_SIZE : len;

        pageCount = len / FLASH_PAGE_SIZE;
        pageMod   = len % FLASH_PAGE_SIZE;

        if (pageMod)
        {
            pageCount++;
        }

        do
        {
            // How much should we copy? SLN_Write_Flash_Page will fill end of page with ones.
            toCopy = (len > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : len;

            // Write a page worth of data to NVM
            status = SLN_Write_Flash_Page2(address, (uint8_t *)buf, toCopy);

            if (kStatus_Success != status)
            {
                break;
            }

            address += FLASH_PAGE_SIZE; // Increment the destination NVM address
            buf += FLASH_PAGE_SIZE;     // Increment the source RAM address
            len -= FLASH_PAGE_SIZE;     // Decrement the total length
        } while (--pageCount);
    }

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}

status_t SLN_Erase_Sector(uint32_t address)
{
    status_t status = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    /* Erase sectors. */
    status = flexspi_nor_flash_erase_sector(FLEXSPI, address);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}

/* NOTE: SLN_Erase_Sector must be called prior to writing pages in a sector
 *       Afterwards, multiple pages can be written in that sector
 * NOTE: This function must always be used for writing full write pages (512 bytes) */
status_t SLN_Write_Flash_At_Address(uint32_t address, uint8_t *data)
{
    status_t status = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    /*Program page. */
    status = flexspi_nor_flash_page_program_with_buffer(FLEXSPI, address, (void *)data);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}

status_t SLN_Read_Flash_At_Address(uint32_t address, uint8_t *data, uint32_t size)
{
    SLN_ram_memcpy(data, (void *)(FlexSPI_AMBA_BASE + address), size);

    return kStatus_Success;
}

uint32_t SLN_Flash_Get_Read_Address(uint32_t address)
{
    return FlexSPI_AMBA_BASE + address;
}
