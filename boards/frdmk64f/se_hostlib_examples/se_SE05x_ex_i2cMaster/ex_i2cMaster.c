/* Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */
#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxEnsure.h>
#include <nxLog_App.h>
#include <se05x_const.h>
#include <string.h>

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
#define I2C_SENSOR_BUS_ADDRESS 0x1D /* I2C bus address of sensor */
#define KEYSIZE 256
#define MAX_SIGNATURE_LEN 256
#define I2C_MAX_DATA 32
#define FXOS8700CQ_STATUS 0x00
#define FXOS8700CQ_WHOAMI 0x0D
#define FXOS8700CQ_XYZ_DATA_CFG 0x0E
#define FXOS8700CQ_CTRL_REG1 0x2A
#define FXOS8700CQ_WHOAMI_VAL 0xC7
#define ACCEL_READ_TIMES 100

/* ************************************************************************** */
/* Static Defines                                                              */
/* ************************************************************************** */
static ex_sss_boot_ctx_t gex_sss_boot_ctx;

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */
#define EX_SSS_BOOT_PCONTEXT (&gex_sss_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 1
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

SE05x_I2CM_cmd_t TLV[5];

#include <ex_sss_main_inc.h>
/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static smStatus_t i2cm_Write(
    ex_sss_boot_ctx_t *pCtx, uint8_t *writebuf, uint32_t writeLength)
{
    smStatus_t status;
    TLV[0].type = kSE05x_I2CM_Configure;
    TLV[0].cmd.cfg.I2C_addr = I2C_SENSOR_BUS_ADDRESS;
    TLV[0].cmd.cfg.I2C_baudRate = kSE05x_I2CM_Baud_Rate_400Khz;

    TLV[1].type = kSE05x_I2CM_Write;
    TLV[1].cmd.w.writeLength = writeLength;
    TLV[1].cmd.w.writebuf = writebuf;

    status = Se05x_i2c_master_txn(&pCtx->session, &TLV[0], 2);
    return status;
}
static smStatus_t i2cm_Read(
    ex_sss_boot_ctx_t *pCtx, uint8_t *readbuf, uint32_t readLength)
{
    smStatus_t status;
    TLV[0].type = kSE05x_I2CM_Configure;
    TLV[0].cmd.cfg.I2C_addr = I2C_SENSOR_BUS_ADDRESS;
    TLV[0].cmd.cfg.I2C_baudRate = kSE05x_I2CM_Baud_Rate_400Khz;

    TLV[1].type = kSE05x_I2CM_Read;
    TLV[1].cmd.rd.readLength = readLength;
    TLV[1].cmd.rd.rdBuf = readbuf;

    status = Se05x_i2c_master_txn(&pCtx->session, &TLV[0], 3);
    return status;
}
static smStatus_t i2cm_WriteRead(ex_sss_boot_ctx_t *pCtx,
    uint8_t *writebuf,
    uint32_t writeLength,
    uint8_t *readbuf,
    uint32_t readLength)
{
    smStatus_t status;
    TLV[0].type = kSE05x_I2CM_Configure;
    TLV[0].cmd.cfg.I2C_addr = I2C_SENSOR_BUS_ADDRESS;
    TLV[0].cmd.cfg.I2C_baudRate = kSE05x_I2CM_Baud_Rate_400Khz;

    TLV[1].type = kSE05x_I2CM_Write;
    TLV[1].cmd.w.writeLength = writeLength;
    TLV[1].cmd.w.writebuf = writebuf;

    TLV[2].type = kSE05x_I2CM_Read;
    TLV[2].cmd.rd.readLength = readLength;
    TLV[2].cmd.rd.rdBuf = readbuf;

    status = Se05x_i2c_master_txn(&pCtx->session, &TLV[0], 3);
    return status;
}

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    smStatus_t status;
    uint8_t pAcclwhoAmIreg[] = {0x0D};
    uint8_t pAcclCtrlReg1[] = {0x2A, 0x00};
    uint8_t pAcclCtrlReg2[] = {0x2A, 0x0D, 0x00};
    uint8_t pAcclxyzDataCfg[] = {0x0E, 0x01};
    uint8_t pacclRead[I2C_MAX_DATA];
    uint8_t status_value = 0;
    uint32_t i = 0;
#if LOG_INFO_ENABLED
    int16_t x, y, z;
#endif

    LOG_I("I2CM example to read Accelerometer value");
    /* ------------------------------------------------------------------------------------------------------------------- */
    status = i2cm_WriteRead(
        pCtx, pAcclwhoAmIreg, sizeof(pAcclwhoAmIreg), pacclRead, 1);
    ENSURE_OR_GO_EXIT(SM_OK == status);
    ENSURE_OR_GO_EXIT(kSE05x_I2CM_Success == TLV[1].cmd.w.wrStatus);

    status = i2cm_Write(pCtx, pAcclCtrlReg1, sizeof(pAcclCtrlReg1));
    ENSURE_OR_GO_EXIT(SM_OK == status);
    ENSURE_OR_GO_EXIT(kSE05x_I2CM_Success == TLV[1].cmd.w.wrStatus);

    status = i2cm_Write(pCtx, pAcclxyzDataCfg, sizeof(pAcclxyzDataCfg));
    ENSURE_OR_GO_EXIT(SM_OK == status);
    ENSURE_OR_GO_EXIT(kSE05x_I2CM_Success == TLV[1].cmd.w.wrStatus);

    status = i2cm_Write(pCtx, pAcclCtrlReg2, sizeof(pAcclCtrlReg2));
    ENSURE_OR_GO_EXIT(SM_OK == status);
    ENSURE_OR_GO_EXIT(kSE05x_I2CM_Success == TLV[1].cmd.w.wrStatus);

    for (i = 0; i < ACCEL_READ_TIMES; i++) {
        /*  wait for new data are ready. */
        while (status_value != 0xff) {
            status = i2cm_Read(pCtx, pacclRead, 1);
            ENSURE_OR_GO_EXIT(SM_OK == status);
            status_value = TLV[1].cmd.rd.rdBuf[0];
        }
        status = i2cm_Read(pCtx, pacclRead, 7);
        ENSURE_OR_GO_EXIT(SM_OK == status);

#if LOG_INFO_ENABLED
        x = ((int16_t)(
                ((TLV[1].cmd.rd.rdBuf[1] * 256U) | TLV[1].cmd.rd.rdBuf[2]))) /
            4U;
        y = ((int16_t)(
                ((TLV[1].cmd.rd.rdBuf[3] * 256U) | TLV[1].cmd.rd.rdBuf[4]))) /
            4U;
        z = ((int16_t)(
                ((TLV[1].cmd.rd.rdBuf[5] * 256U) | TLV[1].cmd.rd.rdBuf[6]))) /
            4U;
        LOG_I("x = %5d , y = %5d , z = %5d", x, y, z);
#endif
    }
    LOG_I("I2CM test completed !!!...");
    return kStatus_SSS_Success;

exit:
    return kStatus_SSS_Fail;
}
