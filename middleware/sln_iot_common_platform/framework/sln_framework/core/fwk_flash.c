/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "fwk_sln_platform.h"

#include "fwk_log.h"
#include "fwk_message.h"

#include "fwk_flash.h"

static flash_dev_t *s_FlashDev = NULL;

sln_flash_status_t FWK_Flash_Init()
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->init != NULL)
    {
        ret = s_FlashDev->ops->init(s_FlashDev);
    }
    return ret;
}

sln_flash_status_t FWK_Flash_Deinit()
{
    return kStatus_HAL_FlashSuccess;
}

sln_flash_status_t FWK_Flash_Format()
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->save != NULL)
    {
        ret = s_FlashDev->ops->format(s_FlashDev);
    }
    return ret;
}

sln_flash_status_t FWK_Flash_Save(const char *path, void *buf, unsigned int size)
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->save != NULL)
    {
        ret = s_FlashDev->ops->save(s_FlashDev, path, buf, size);
    }
    return ret;
}
sln_flash_status_t FWK_Flash_Read(const char *path, void *buf, unsigned int size)
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->read != NULL)
    {
        ret = s_FlashDev->ops->read(s_FlashDev, path, buf, size);
    }
    return ret;
}

sln_flash_status_t FWK_Flash_Append(const char *path, void *buf, unsigned int size, bool overwrite)
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->append != NULL)
    {
        ret = s_FlashDev->ops->append(s_FlashDev, path, buf, size, overwrite);
    }
    return ret;
}

sln_flash_status_t FWK_Flash_Mkdir(const char *path)
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->mkdir != NULL)
    {
        ret = s_FlashDev->ops->mkdir(s_FlashDev, path);
    }
    return ret;
}

sln_flash_status_t FWK_Flash_Rm(const char *path)
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->rm != NULL)
    {
        ret = s_FlashDev->ops->rm(s_FlashDev, path);
    }
    return ret;
}
sln_flash_status_t FWK_Flash_Rename(const char *oldPath, const char *newPath)
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->rename != NULL)
    {
        ret = s_FlashDev->ops->rename(s_FlashDev, oldPath, newPath);
    }
    return ret;
}

sln_flash_status_t FWK_Flash_Cleanup(uint32_t timeout)
{
    sln_flash_status_t ret = kStatus_HAL_FlashFail;
    if (s_FlashDev != NULL && s_FlashDev->ops->cleanup != NULL)
    {
        ret = s_FlashDev->ops->cleanup(s_FlashDev, timeout);
    }
    return ret;
}

int FWK_Flash_DeviceRegister(const flash_dev_t *dev)
{
    if (s_FlashDev == NULL)
    {
        s_FlashDev = dev;
        return 0;
    }
    else
    {
        LOGE("Only one flash device supported at a time");
        return -1;
    }
}
