/*Copyright 2022-2023 NXP
*
* NXP Confidential. This software is owned or controlled by NXP
* and may only be used strictly in accordance with the applicable license terms.
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read,
* and that you agree to comply with and are bound by, such license terms.
* If you do not agree to be bound by the applicable license terms,
* then you may not retain, install, activate or otherwise use the software.
*
*/


#ifndef RDSP_CONVERSA_DEVICE_CONFIG_H_
#define RDSP_CONVERSA_DEVICE_CONFIG_H_

/* Device Id
*    To specify on which device Conversa is running
*/
typedef enum
{
    Device_LPC55S69 = 1,                            // LPC55S69  	: Running on Cortex-M33
    Device_RW610_CM33_NO_DSP,						// RW610		: Running on Cortex-M33 (no DSP extensions)
    Device_IMXRT500_CM33,                           // I.MXRT500  	: Running on Cortex-M33
    Device_IMXRT500_FusionF1,                       // I.MXRT500  	: Running on FusionF1 DSP
    Device_IMXRT600_HIFI4,                          // I.MXRT600  	: Running on HiFi4 DSP
    Device_IMXRT1040_CM7,                           // I.MXRT1040 	: Running on Cortex-M7
    Device_IMXRT1050_CM7 ,                          // I.MXRT1050 	: Running on Cortex-M7
    Device_IMXRT1060_CM7 ,                          // I.MXRT1060 	: Running on Cortex-M7
    Device_IMXRT1160_CM7 ,                          // I.MXRT1160 	: Running on Cortex-M7
    Device_IMXRT1170_CM7,                           // I.MXRT1170 	: Running on Cortex-M7
    Device_IMX6ULL_CA7,                             // I.MX6ULL 	: Running on Cortex-A7
	Device_IMX7ULP_CA7,                             // I.MX7ULP 	: Running on Cortex-A7
    Device_IMX8ULP_CM33,                            // I.MX8ULP	  	: Running on Cortex-M33
    Device_IMX8_CA35,                               // I.MX8	  	: Running on Cortex-A35 (I.MX8QXP and I.MX8ULP)
    Device_IMX8M_CM4,                               // I.MX8M 		: Running on Cortex-M4 (i.MX8MMini and i.MX8MPlus)
    Device_IMX8M_CM7,                               // I.MX8M  		: Running on Cortex-M7 (i.MX8MNano and i.MX8MPlus)
    Device_IMX8M_CA53,                              // I.MX8M  		: Running on Cortex-A53 (i.MX8MNano, i.MX8MMini and i.MX8MPlus)
    Device_IMX93_CM33,                           	// I.MX93	  	: Running on Cortex-M33
    Device_IMX9_CA55,                           	// I.MX9	  	: Running on Cortex-A55
    Device_DUMMY_DEVICE  = 99
} RDSP_DeviceId_en;

#endif // RDSP_CONVERSA_DEVICE_CONFIG_H_
