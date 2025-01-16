/*
 * Copyright 2023-2024 NXP
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Public API for FLASH drivers
 */

#ifndef _FWK_NVS_PORT_H_
#define _FWK_NVS_PORT_H_

#include <stddef.h>
#include <stdint.h>
#include "zephyr_errno.h"

#include "fwk_hal_macros.h"
#include "fsl_os_abstraction.h"

/**
 * @brief FLASH internal Interface
 * @defgroup flash_internal_interface FLASH internal Interface
 * @ingroup io_interfaces
 * @{
 */

#ifndef off_t
typedef long int off_t;
#endif

#ifndef ssize_t
#ifdef __IAR_SYSTEMS_ICC__
typedef int32_t ssize_t;
#else
typedef _ssize_t ssize_t;
#endif
#endif

union physical_address
{
    uint32_t value;
    uint8_t *pointer;
};

struct k_mutex
{
    OSA_MUTEX_HANDLE_DEFINE(MutexId);
    bool created;
};

#ifndef gAppNvsInternalFlash_c
#define gAppNvsInternalFlash_c 1
#endif

#ifndef gAppNvsExternalFlash_c
#define gAppNvsExternalFlash_c 1
#endif

/* Index of the Internal Flash NV storage are in the flash map array */
#if defined gAppNvsInternalFlash_c && (gAppNvsInternalFlash_c > 0)
#define NVS_INTFLASH_AREA_ID 0U
#if defined gAppNvsExternalFlash_c && (gAppNvsExternalFlash_c > 0)
#define NVS_EXTFLASH_AREA_ID 1U
#endif
#else
#if defined gAppNvsExternalFlash_c && (gAppNvsExternalFlash_c > 0)
#define NVS_EXTFLASH_AREA_ID 0U
#endif
#endif

#define K_MUTEX_DEFINE(_mtx_) struct k_mutex _mtx_
#define K_FOREVER             ~0UL

#if (!(defined(__CC_ARM) || defined(__UVISION_VERSION) || defined(__ARMCC_VERSION)))
/*
 * Name: NV_STORAGE_START_ADDRESS
 * Description: NV_STORAGE_START_ADDRESS from linker command file is used by this code
 *              as Raw Sector Start Address.
 */
extern uint32_t NV_STORAGE_START_ADDRESS[];

/*
 * Name: NV_STORAGE_END_ADDRESS
 * Description: NV_STORAGE_END_ADDRESS from linker command file is used by this code
 *              as Raw Sector End Address.
 */
extern uint32_t NV_STORAGE_END_ADDRESS[];

/*
 * Name: NV_STORAGE_SECTOR_SIZE
 * Description: external symbol from linker command file, it represents the size
 *              of a FLASH sector
 */
extern uint32_t NV_STORAGE_SECTOR_SIZE[];

/*
 * Name:  NV_STORAGE_MAX_SECTORS
 * Description: external symbol from linker command file, it represents the sectors
 *              count used by the ENVM storage system; it has to be a multiple of 2
 */
extern uint32_t NV_STORAGE_MAX_SECTORS[];

extern uint32_t NV_STORAGE_SIZE[];

#else

extern uint32_t Image$$NVM_region$$ZI$$Base[];
extern uint32_t Image$$NVM_region$$ZI$$Limit[];
extern uint32_t Image$$NVM_region$$Length;

#define NV_STORAGE_START_ADDRESS (Image$$NVM_region$$ZI$$Base)
#define NV_STORAGE_END_ADDRESS   (Image$$NVM_region$$ZI$$Limit)
#define NV_STORAGE_SECTOR_SIZE   FSL_FEATURE_FLASH_SECTOR_SIZE_BYTES
#define NVM_LENGTH               (NV_STORAGE_END_ADDRESS - NV_STORAGE_START_ADDRESS)
#define NV_STORAGE_MAX_SECTORS   (NVM_LENGTH / NV_STORAGE_SECTOR_SIZE)
#endif /* __CC_ARM */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __syscall
#define __syscall
#endif
/**
 * @}
 */

/**
 * @brief FLASH Interface
 * @defgroup flash_interface FLASH Interface
 * @ingroup io_interfaces
 * @{
 */

/**
 * Flash memory parameters. Contents of this structure suppose to be
 * filled in during flash device initialization and stay constant
 * through a runtime.
 */
struct flash_parameters
{
    const size_t   write_block_size;
    const uint8_t  erase_value; /* Byte value of erased flash */
    const uint16_t sector_size; /* Minimal erase size */
};

/**
 * @}
 */

struct device
{
    const struct flash_driver_api *api;
};

struct flash_area
{
    uint8_t fa_id;   /*<  ID number */
    off_t   fa_off;  /*< Start offset from the beginning of the flash device */
    size_t  fa_size; /*<! Total size */

    const struct device *fa_dev;
};

/**
 * @brief Structure for transfer flash sector boundaries
 *
 * This template is used for presentation of flash memory structure. It
 * consumes much less RAM than @ref flash_area
 * Note: notion of sector is independent from flash erase unit in this case.
 */
struct flash_sector
{
    /** Sector offset from the beginning of the flash device */
    off_t fs_off;
    /** Sector size in bytes */
    size_t fs_size;
};

/**
 * @addtogroup flash_internal_interface
 * @{
 */

typedef int (*flash_api_init)(void);

typedef int (*flash_api_read)(const struct device *dev, off_t offset, void *data, size_t len);

/**
 * @brief Flash write implementation handler type
 *
 * @note Any necessary write protection management must be performed by
 * the driver, with the driver responsible for ensuring the "write-protect"
 * after the operation completes (successfully or not) matches the write-protect
 * state when the operation was started.
 */
typedef int (*flash_api_write)(const struct device *dev, off_t offset, const void *data, size_t len);

/**
 * @brief Flash blank check implementation handler type
 *
 * @note verify that area [offset .. offset+size] is still unprogrammed
 */
typedef int (*flash_api_blank_check)(const struct device *dev, off_t offset, size_t len);

/**
 * @brief Flash erase implementation handler type
 *
 * @note Any necessary erase protection management must be performed by
 * the driver, with the driver responsible for ensuring the "erase-protect"
 * after the operation completes (successfully or not) matches the erase-protect
 * state when the operation was started.
 */
typedef int (*flash_api_erase)(const struct device *dev, off_t offset, size_t size);

typedef const struct flash_parameters *(*flash_api_get_parameters)(const struct device *dev);

#if defined(CONFIG_FLASH_PAGE_LAYOUT)
/**
 * @brief Retrieve a flash device's layout.
 *
 * A flash device layout is a run-length encoded description of the
 * pages on the device. (Here, "page" means the smallest erasable
 * area on the flash device.)
 *
 * For flash memories which have uniform page sizes, this routine
 * returns an array of length 1, which specifies the page size and
 * number of pages in the memory.
 *
 * Layouts for flash memories with nonuniform page sizes will be
 * returned as an array with multiple elements, each of which
 * describes a group of pages that all have the same size. In this
 * case, the sequence of array elements specifies the order in which
 * these groups occur on the device.
 *
 * @param dev         Flash device whose layout to retrieve.
 * @param layout      The flash layout will be returned in this argument.
 * @param layout_size The number of elements in the returned layout.
 */
typedef void (*flash_api_pages_layout)(const struct device              *dev,
                                       const struct flash_pages_layout **layout,
                                       size_t                           *layout_size);
#endif /* CONFIG_FLASH_PAGE_LAYOUT */

typedef int (*flash_api_sfdp_read)(const struct device *dev, off_t offset, void *data, size_t len);
typedef int (*flash_api_read_jedec_id)(const struct device *dev, uint8_t *id);
typedef int (*flash_api_ex_op)(const struct device *dev, uint16_t code, const uintptr_t in, void *out);

struct flash_driver_api
{
    flash_api_init           init;
    flash_api_read           read;
    flash_api_write          write;
    flash_api_erase          erase;
    flash_api_blank_check    blank_check;
    flash_api_get_parameters get_parameters;
#if defined(CONFIG_FLASH_PAGE_LAYOUT)
    flash_api_pages_layout page_layout;
#endif /* CONFIG_FLASH_PAGE_LAYOUT */
#if defined(CONFIG_FLASH_JESD216_API)
    flash_api_sfdp_read     sfdp_read;
    flash_api_read_jedec_id read_jedec_id;
#endif /* CONFIG_FLASH_JESD216_API */
#if defined(CONFIG_FLASH_EX_OP_ENABLED)
    flash_api_ex_op ex_op;
#endif /* CONFIG_FLASH_EX_OP_ENABLED */
};

/**
 * @}
 */

/**
 * @addtogroup flash_interface
 * @{
 */

/**
 *  @brief  Initialize flash
 *
 *  @return  0 on success, negative errno code on fail.
 */
static inline int flash_init(const struct device *dev)
{
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;

    return api->init();
}

/**
 *  @brief  Read data from flash
 *
 *  All flash drivers support reads without alignment restrictions on
 *  the read offset, the read size, or the destination address.
 *
 *  @param  dev             : flash dev
 *  @param  offset          : Offset (byte aligned) to read
 *  @param  data            : Buffer to store read data
 *  @param  len             : Number of bytes to read.
 *
 *  @return  0 on success, negative errno code on fail.
 */

static inline int flash_read(const struct device *dev, off_t offset, void *data, size_t len)
{
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;

    return api->read(dev, offset, data, len);
}

/**
 *  @brief  Write buffer into flash memory.
 *
 *  All flash drivers support a source buffer located either in RAM or
 *  SoC flash, without alignment restrictions on the source address.
 *  Write size and offset must be multiples of the minimum write block size
 *  supported by the driver.
 *
 *  Any necessary write protection management is performed by the driver
 *  write implementation itself.
 *
 *  @param  dev             : flash device
 *  @param  offset          : starting offset for the write
 *  @param  data            : data to write
 *  @param  len             : Number of bytes to write
 *
 *  @return  0 on success, negative errno code on fail.
 */
static inline int flash_write(const struct device *dev, off_t offset, const void *data, size_t len)
{
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;
    int                            rc;

    rc = api->write(dev, offset, data, len);

    return rc;
}

/**
 *  @brief  Erase part or all of a flash memory
 *
 *  Acceptable values of erase size and offset are subject to
 *  hardware-specific multiples of page size and offset. Please check
 *  the API implemented by the underlying sub driver, for example by
 *  using flash_get_page_info_by_offs() if that is supported by your
 *  flash driver.
 *
 *  Any necessary erase protection management is performed by the driver
 *  erase implementation itself.
 *
 *  @param  dev             : flash device
 *  @param  offset          : erase area starting offset
 *  @param  size            : size of area to be erased
 *
 *  @return  0 on success, negative errno code on fail.
 *
 *  @see flash_get_page_info_by_offs()
 *  @see flash_get_page_info_by_idx()
 */
static inline int flash_erase(const struct device *dev, off_t offset, size_t size)
{
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;
    int                            rc;

    rc = api->erase(dev, offset, size);

    return rc;
}

static inline int flash_blank_check(const struct device *dev, off_t offset, size_t len)
{
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;

    return api->blank_check(dev, offset, len);
}

struct flash_pages_info
{
    off_t    start_offset; /* offset from the base of flash address */
    size_t   size;
    uint32_t index;
};

/**
 *  @brief  Get the size and start offset of flash page at certain flash offset.
 *
 *  @param  dev flash device
 *  @param  offset Offset within the page
 *  @param  info Page Info structure to be filled
 *
 *  @return  0 on success, -EINVAL if page of the offset doesn't exist.
 */
__syscall int flash_get_page_info_by_offs(const struct device *dev, off_t offset, struct flash_pages_info *info);

/**
 *  @brief  Get the size and start offset of flash page of certain index.
 *
 *  @param  dev flash device
 *  @param  page_index Index of the page. Index are counted from 0.
 *  @param  info Page Info structure to be filled
 *
 *  @return  0 on success, -EINVAL  if page of the index doesn't exist.
 */
__syscall int flash_get_page_info_by_idx(const struct device *dev, uint32_t page_index, struct flash_pages_info *info);

/**
 *  @brief  Get the total number of flash pages.
 *
 *  @param  dev flash device
 *
 *  @return  Number of flash pages.
 */
__syscall size_t flash_get_page_count(const struct device *dev);

/**
 * @brief Callback type for iterating over flash pages present on a device.
 *
 * The callback should return true to continue iterating, and false to halt.
 *
 * @param info Information for current page
 * @param data Private data for callback
 * @return True to continue iteration, false to halt iteration.
 * @see flash_page_foreach()
 */
typedef bool (*flash_page_cb)(const struct flash_pages_info *info, void *data);

/**
 * @brief Iterate over all flash pages on a device
 *
 * This routine iterates over all flash pages on the given device,
 * ordered by increasing start offset. For each page, it invokes the
 * given callback, passing it the page's information and a private
 * data object.
 *
 * @param dev Device whose pages to iterate over
 * @param cb Callback to invoke for each flash page
 * @param data Private data for callback function
 */
void flash_page_foreach(const struct device *dev, flash_page_cb cb, void *data);

#if defined(CONFIG_FLASH_JESD216_API)
/**
 * @brief Read data from Serial Flash Discoverable Parameters
 *
 * This routine reads data from a serial flash device compatible with
 * the JEDEC JESD216 standard for encoding flash memory
 * characteristics.
 *
 * Availability of this API is conditional on selecting
 * @c CONFIG_FLASH_JESD216_API and support of that functionality in
 * the driver underlying @p dev.
 *
 * @param dev device from which parameters will be read
 * @param offset address within the SFDP region containing data of interest
 * @param data where the data to be read will be placed
 * @param len the number of bytes of data to be read
 *
 * @retval 0 on success
 * @retval -ENOTSUP if the flash driver does not support SFDP access
 * @retval negative values for other errors.
 */
__syscall int flash_sfdp_read(const struct device *dev, off_t offset, void *data, size_t len);

static inline int z_impl_flash_sfdp_read(const struct device *dev, off_t offset, void *data, size_t len)
{
    int                            rv  = -ENOTSUP;
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;

    if (api->sfdp_read != NULL)
    {
        rv = api->sfdp_read(dev, offset, data, len);
    }
    return rv;
}

/**
 * @brief Read the JEDEC ID from a compatible flash device.
 *
 * @param dev device from which id will be read
 * @param id pointer to a buffer of at least 3 bytes into which id
 * will be stored
 *
 * @retval 0 on successful store of 3-byte JEDEC id
 * @retval -ENOTSUP if flash driver doesn't support this function
 * @retval negative values for other errors
 */
__syscall int flash_read_jedec_id(const struct device *dev, uint8_t *id);

static inline int z_impl_flash_read_jedec_id(const struct device *dev, uint8_t *id)
{
    int                            rv  = -ENOTSUP;
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;

    if (api->read_jedec_id != NULL)
    {
        rv = api->read_jedec_id(dev, id);
    }
    return rv;
}
#endif /* CONFIG_FLASH_JESD216_API */

/**
 *  @brief  Get the minimum write block size supported by the driver
 *
 *  The write block size supported by the driver might differ from the write
 *  block size of memory used because the driver might implements write-modify
 *  algorithm.
 *
 *  @param  dev flash device
 *
 *  @return  write block size in bytes.
 */
static inline size_t flash_get_write_block_size(const struct device *dev)
{
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;

    return api->get_parameters(dev)->write_block_size;
}

/**
 *  @brief  Get pointer to flash_parameters structure
 *
 *  Returned pointer points to a structure that should be considered
 *  constant through a runtime, regardless if it is defined in RAM or
 *  Flash.
 *  Developer is free to cache the structure pointer or copy its contents.
 *
 *  @return pointer to flash_parameters structure characteristic for
 *          the device.
 */
static inline const struct flash_parameters *flash_get_parameters(const struct device *dev)
{
    const struct flash_driver_api *api = (const struct flash_driver_api *)dev->api;

    return api->get_parameters(dev);
}

/**
 *  @brief Execute flash extended operation on given device
 *
 *  Besides of standard flash operations like write or erase, flash controllers
 *  also support additional features like write protection or readout
 *  protection. These features are not available in every flash controller,
 *  what's more controllers can implement it in a different way.
 *
 *  It doesn't make sense to add a separate flash API function for every flash
 *  controller feature, because it could be unique (supported on small number of
 *  flash controllers) or the API won't be able to represent the same feature on
 *  every flash controller.
 *
 *  @param dev Flash device
 *  @param code Operation which will be executed on the device.
 *  @param in Pointer to input data used by operation. If operation doesn't
 *            need any input data it could be NULL.
 *  @param out Pointer to operation output data. If operation doesn't produce
 *             any output it could be NULL.
 *
 *  @retval 0 on success.
 *  @retval -ENOTSUP if given device doesn't support extended operation.
 *  @retval -ENOSYS if support for extended operations is not enabled in Kconfig
 *  @retval negative value on extended operation errors.
 */
__syscall int flash_ex_op(const struct device *dev, uint16_t code, const uintptr_t in, void *out);

int flash_area_open(uint8_t id, const struct flash_area **fap);

const struct device *flash_area_get_device(const struct flash_area *fa);

/**
 * Retrieve info about sectors within the area.
 *
 * @param[in]  fa_id    Given flash area ID
 * @param[out] sectors  buffer for sectors data
 * @param[in,out] count On input Capacity of @p sectors, on output number of
 * sectors Retrieved.
 *
 * @return  0 on success, negative errno code on fail. Especially returns
 * -ENOMEM if There are too many flash pages on the flash_area to fit in the
 * array.
 */
int flash_area_get_sectors(int fa_id, uint32_t *count, struct flash_sector *sectors);

__syscall int k_mutex_lock(struct k_mutex *mutex, uint32_t timeout);

__syscall int k_mutex_unlock(struct k_mutex *mutex);

__syscall int k_mutex_init(struct k_mutex *mutex);
__syscall int k_mutex_destroy(struct k_mutex *mutex);

#ifndef WITH_LOGS

#define LOG_MODULE_REGISTER(x, y)
#define LOG_MODULE_DECLARE(...)

#undef LOG_WRN
#undef LOG_INF
#undef LOG_DBG

#undef LOG_HEXDUMP_ERR
#undef LOG_HEXDUMP_WRN
#undef LOG_HEXDUMP_INF
#undef LOG_HEXDUMP_DBG

#define LOG_ERR(...) (void)0
#define LOG_WRN(...) (void)0
#define LOG_DBG(...) (void)0
#define LOG_INF(...) (void)0

#define LOG_HEXDUMP_ERR(...) (void)0
#define LOG_HEXDUMP_WRN(...) (void)0
#define LOG_HEXDUMP_DBG(...) (void)0
#define LOG_HEXDUMP_INF(...) (void)0
#else
/* TO DO */
#endif
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* _FWK_NVS_PORT_H_ */
