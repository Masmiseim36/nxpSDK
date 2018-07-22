/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* All rights reserved.
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
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
#include "fsl_debug_console.h"
#include "fsl_fxos.h"
#include "board.h"
#include "math.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* I2C source clock */
#define ACCEL_I2C_CLK_SRC I2C3_CLK_SRC
#define I2C_BAUDRATE 100000U
#define ACCEL_I2C_ADDR 1CU
#define HWTIMER_LL_DEVIF kSystickDevif
#define HWTIMER_LL_ID 0
#define HWTIMER_PERIOD 100000
/* multiplicative conversion constants */
#define DegToRad 0.017453292
#define RadToDeg 57.295779
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void Sensor_ReadData(int16_t *Ax, int16_t *Ay, int16_t *Az, int16_t *Mx, int16_t *My, int16_t *Mz);
void Magnetometer_Calibrate(void);
void HW_Timer_init(void);
void Delay(uint32_t ticks);
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
extern void FXOS_master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData);
lpi2c_master_handle_t g_MasterHandle;
#else
i2c_master_handle_t g_MasterHandle;
#endif

volatile uint16_t SampleEventFlag;
fxos_handle_t g_fxosHandle;
uint8_t g_sensor_address[] = {0x1CU, 0x1EU, 0x1DU, 0x1FU};

int16_t g_Ax_Raw = 0;
int16_t g_Ay_Raw = 0;
int16_t g_Az_Raw = 0;

double g_Ax = 0;
double g_Ay = 0;
double g_Az = 0;

int16_t g_Ax_buff[25] = {0};
int16_t g_Ay_buff[25] = {0};
int16_t g_Az_buff[25] = {0};

int16_t g_Mx_Raw = 0;
int16_t g_My_Raw = 0;
int16_t g_Mz_Raw = 0;

int16_t g_Mx_Offset = 0;
int16_t g_My_Offset = 0;
int16_t g_Mz_Offset = 0;

double g_Mx = 0;
double g_My = 0;
double g_Mz = 0;

double g_Yaw = 0;
double g_Yaw_LP = 0;
double g_Pitch = 0;
double g_Roll = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void HW_Timer_init(void)
{
    /* Configure the SysTick timer */
    SysTick_Config(SystemCoreClock / 1000U);
}

void SysTick_Handler(void)
{
    SampleEventFlag = 1;
}

void Delay(uint32_t ticks)
{
    while (ticks--)
    {
        __asm("nop");
    }
}

void Sensor_ReadData(int16_t *Ax, int16_t *Ay, int16_t *Az, int16_t *Mx, int16_t *My, int16_t *Mz)
{
    fxos_data_t fxos_data;

    if (FXOS_ReadSensorData(&g_fxosHandle, &fxos_data) != kStatus_Success)
    {
        PRINTF("Failed to read acceleration data!\r\n");
    }

    *Ax = (int16_t)((uint16_t)((uint16_t)fxos_data.accelXMSB << 8) | (uint16_t)fxos_data.accelXLSB)/4U;
    *Ay = (int16_t)((uint16_t)((uint16_t)fxos_data.accelYMSB << 8) | (uint16_t)fxos_data.accelYLSB)/4U;
    *Az = (int16_t)((uint16_t)((uint16_t)fxos_data.accelZMSB << 8) | (uint16_t)fxos_data.accelZLSB)/4U;
    *Mx = (int16_t)((uint16_t)((uint16_t)fxos_data.magXMSB << 8) | (uint16_t)fxos_data.magXLSB);
    *My = (int16_t)((uint16_t)((uint16_t)fxos_data.magYMSB << 8) | (uint16_t)fxos_data.magYLSB);
    *Mz = (int16_t)((uint16_t)((uint16_t)fxos_data.magZMSB << 8) | (uint16_t)fxos_data.magZLSB);
}

void Magnetometer_Calibrate(void)
{
    int16_t Mx_max = 0;
    int16_t My_max = 0;
    int16_t Mz_max = 0;
    int16_t Mx_min = 0;
    int16_t My_min = 0;
    int16_t Mz_min = 0;

    uint32_t times = 0;
    PRINTF("\r\nCalibrating magnetometer...");
    while (times < 5000)
    {
        Sensor_ReadData(&g_Ax_Raw, &g_Ay_Raw, &g_Az_Raw, &g_Mx_Raw, &g_My_Raw, &g_Mz_Raw);
        if (times == 0)
        {
            Mx_max = Mx_min = g_Mx_Raw;
            My_max = My_min = g_My_Raw;
            Mz_max = Mz_min = g_Mz_Raw;
        }
        if (g_Mx_Raw > Mx_max)
        {
            Mx_max = g_Mx_Raw;
        }
        if (g_My_Raw > My_max)
        {
            My_max = g_My_Raw;
        }
        if (g_Mz_Raw > Mz_max)
        {
            Mz_max = g_Mz_Raw;
        }
        if (g_Mx_Raw < Mx_min)
        {
            Mx_min = g_Mx_Raw;
        }
        if (g_My_Raw < My_min)
        {
            My_min = g_My_Raw;
        }
        if (g_Mz_Raw < Mz_min)
        {
            Mz_min = g_Mz_Raw;
        }
        if (times == 4999)
        {
            if ((Mx_max > (Mx_min + 500)) && (My_max > (My_min + 500)) && (Mz_max > (Mz_min + 500)))
            {
                g_Mx_Offset = (Mx_max + Mx_min) / 2;
                g_My_Offset = (My_max + My_min) / 2;
                g_Mz_Offset = (Mz_max + Mz_min) / 2;
                PRINTF("\r\nCalibrate magnetometer successfully!");
                PRINTF("\r\nMagnetometer offset Mx: %d - My: %d - Mz: %d \r\n", g_Mx_Offset, g_My_Offset, g_Mz_Offset);
            }
            else
            {
                PRINTF("Calibrating magnetometer failed! Press any key to recalibrate...\r\n");
                GETCHAR();
                PRINTF("\r\nCalibrating magnetometer...");
                times = 0;
            }
        }
        times++;
        Delay(3000);
    }
}

int main(void)
{
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig;
#else
    i2c_master_config_t i2cConfig;
#endif
    uint32_t i2cSourceClock;
    uint16_t i = 0;
    uint16_t loopCounter = 0;
    double sinAngle = 0;
    double cosAngle = 0;
    double Bx = 0;
    double By = 0;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HW_Timer_init();

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    i2cSourceClock = LPI2C_CLOCK_FREQUENCY;
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
    LPI2C_MasterInit(BOARD_ACCEL_I2C_BASEADDR, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(BOARD_ACCEL_I2C_BASEADDR, &g_MasterHandle, FXOS_master_callback, NULL);
#else
    i2cSourceClock = CLOCK_GetFreq(ACCEL_I2C_CLK_SRC);
    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableHighDrive = false;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(BOARD_ACCEL_I2C_BASEADDR, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(BOARD_ACCEL_I2C_BASEADDR, &g_MasterHandle, NULL, NULL);
#endif
    g_fxosHandle.base = BOARD_ACCEL_I2C_BASEADDR;
    g_fxosHandle.i2cHandle = &g_MasterHandle;

    /* Find sensor on board */
    for (i = 0; i < 4; i++)
    {
        g_fxosHandle.xfer.slaveAddress = g_sensor_address[i];
        if (FXOS_Init(&g_fxosHandle) == kStatus_Success)
        {
            break;
        }
        else /* Not found any sensor on board */
        {
            if (i == 3)
            {
                PRINTF("\r\nFailed to initialize the Accelerometer device!");
                while (1)
                {
                }
            }
        }
    }

    PRINTF("To calibrate Magnetometer, roll the board on all orientations to get max and min values");
    PRINTF("\r\nPress any key to start calibrating...\r\n");
    GETCHAR();
    Magnetometer_Calibrate();

    /* Infinite loops */
    for (;;)
    {
        if (SampleEventFlag == 1) /* Fix loop */
        {
            SampleEventFlag = 0;
            g_Ax_Raw = 0;
            g_Ay_Raw = 0;
            g_Az_Raw = 0;
            g_Ax = 0;
            g_Ay = 0;
            g_Az = 0;

            /* Read sensor data */
            Sensor_ReadData(&g_Ax_Raw, &g_Ay_Raw, &g_Az_Raw, &g_Mx_Raw, &g_My_Raw, &g_Mz_Raw);

            /* Average accel value */
            for (i = 1; i < 25; i++)
            {
                g_Ax_buff[i] = g_Ax_buff[i - 1];
                g_Ay_buff[i] = g_Ay_buff[i - 1];
                g_Az_buff[i] = g_Az_buff[i - 1];
            }
            g_Ax_buff[0] = g_Ax_Raw;
            g_Ay_buff[0] = g_Ay_Raw;
            g_Az_buff[0] = g_Az_Raw;
            for (i = 0; i < 25; i++)
            {
                g_Ax += (double)g_Ax_buff[i];
                g_Ay += (double)g_Ay_buff[i];
                g_Az += (double)g_Az_buff[i];
            }
            g_Ax /= 25;
            g_Ay /= 25;
            g_Az /= 25;

            /* Calculate magnetometer values */
            g_Mx = g_Mx_Raw - g_Mx_Offset;
            g_My = g_My_Raw - g_My_Offset;
            g_Mz = g_Mz_Raw - g_Mz_Offset;
            /* Calculate roll angle g_Roll (-180deg, 180deg) and sin, cos */
            g_Roll = atan2(g_Ay, g_Az) * RadToDeg;
            sinAngle = sin(g_Roll * DegToRad);
            cosAngle = cos(g_Roll * DegToRad);

            /* De-rotate by roll angle g_Roll */
            By = g_My * cosAngle - g_Mz * sinAngle;
            g_Mz = g_Mz * cosAngle + g_My * sinAngle;
            g_Az = g_Ay * sinAngle + g_Az * cosAngle;

            /* Calculate pitch angle g_Pitch (-90deg, 90deg) and sin, cos*/
            g_Pitch = atan(-g_Ax / g_Az) * RadToDeg;
            sinAngle = sin(g_Pitch * DegToRad);
            cosAngle = cos(g_Pitch * DegToRad);

            /* De-rotate by pitch angle g_Pitch */
            Bx = g_Mx * cosAngle + g_Mz * sinAngle;

            /* Calculate yaw = ecompass angle psi (-180deg, 180deg) */
            g_Yaw = atan2(-By, Bx) * RadToDeg;
            g_Yaw_LP += (g_Yaw - g_Yaw_LP) * 0.01;

            if (++loopCounter > 10)
            {
                PRINTF("\rCompass Angle: %3.1lf", g_Yaw_LP);
                loopCounter = 0;
            }
        }
    } /* End infinite loops */
}
