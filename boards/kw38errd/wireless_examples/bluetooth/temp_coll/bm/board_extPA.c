
/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "board_extPA.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "nxp_xcvr_ext_ctrl.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

/* Enable LDO high trim, required by the BLE LL */
#define USE_HIGH_PWR_PA

#if defined (gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d == 1)
#define cExtPaPins_d 4U
#else
#define cExtPaPins_d 3U
#endif

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

enum {
    EXTPA_TX_BYPASS = 0,
#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)
    EXTPA_TX_HIGH_EFFICIENT,
#endif
    EXTPA_TX_LINEAR
} ext_pa_mode_e;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

static GPIO_Type *const maGpioBases[] = GPIO_BASE_PTRS;
static PORT_Type *const maPortBases[] = PORT_BASE_PTRS;

static const int8_t extPaTableBypass[] = {
    -31+0,  /*-31.1 dBm*/                         -25+0,  /*-25 dBm*/
    -19+0,  /*-19 dBm*/                           -16+0,  /*-15.6 dBm => rounded to -16 dBm*/
    -13+0,  /*-13.1 dBm*/                         -11+0,  /*-11.2 dBm*/
    -10+0,  /*-9.6 dBm => rounded to -10 dBm*/     -8+0,  /*-8.3 dBm*/
     -7+0,  /*-7.2 dBm*/                           -6+0,  /*-6.2 dBm*/
     -5+0,  /*-5.3 dBm*/                           -5+0,  /*-4.5 dBm => rounded to -5 dBm*/
     -4+0,  /*-3.8 dBm => rounded to -4 dBm*/      -3+0,  /*-3.1 dBm*/
     -3+0,  /*-2.5 dBm => rounded to -3 dBm*/      -2+0,  /*-1.9 dBm => rounded to -2 dBm*/
     -1+0,  /*-1.4 dBm => rounded to -1 dBm*/      -1+0,  /*-1.0 dBm*/
      0+0,  /*-0.5 dBm => rounded t0 0 dBm*/        0+0,  /*-0.1 dBm*/
      0+0,  /*0.3 dBm*/                             1+0,  /*0.7 dBm => rounded to 1 dBm*/
      1+0,  /*1.1 dBm*/                             1+0,  /*1.4 dBm*/
      2+0,  /*1.8 dBm => rounded to 2 dBm*/         2+0,  /*2.1 dBm*/
      2+0,  /*2.4 dBm*/                             3+0,  /*2.6 dBm => rounded to 3 dBm*/
      3+0,  /*2.9 dBm => rounded to 3 dBm*/         3+0,  /*3.1 dBm*/
      3+0,  /*3.3 dBm*/                             4+0,  /*3.5 dBm => rounded to 4 dBm*/

#ifdef USE_HIGH_PWR_PA
    /*high_tx_power[] value were obtained from the MKW37 datasheet section 4.3 table 7*/
    -30+0,  /*-29.55 dBm => rounded to -30 dbm*/   -23+0,  /*-23.4 dBm*/
    -17+0,  /*-17.45 dBm*/                         -14+0,  /*-13.94 dBm => rounded to -14 dBm*/
    -11+0,  /*-11.45 dBm*/                         -10+0,  /*-9.53 dBm => rounded to -10 dBm*/
     -8+0,  /*-7.96 dBm => rounded to -8 dBm*/      -7+0,  /*-6.63 dBm => rounded to -7 dBm*/
     -5+0,  /*-5.5 dBm => rounded to -5 dBm */      -4+0,  /*-4.49 dBm*/
     -4+0,  /*-3.59 dBm == rounded to -4 dBm*/      -3+0,  /*-2.77 dBm => rounded to -3 dBm*/
     -2+0,  /*-2.03 dBm*/                           -1+0,  /*-1.35 dBm*/
     -1+0,  /*-0.72 dBm => rounded to -1 dBm*/       0+0,  /*-0.14 dBm*/
      0+0,  /*0.34 dBm*/                             1+0,  /*0.85 dBm => rounded to 1 dBm*/
      1+0,  /*1.32 dBm*/                             0+0,  /*1.77 dBm => rounded to 2 dBm*/
      2+0,  /*2.20 dBm*/                             3+0,  /*2.62 dBm => rounded to 3 dBm*/
      3+0,  /*3.01 dBm*/                             3+0,  /*3.38 dBm*/
      4+0,  /*3.7 dBm => rounded to 4 dBm*/          4+0,  /*4 dBm*/
      4+0,  /*4.4 dBm*/                              5+0,  /*4.6 dBm => rounded to 5 dBm*/
      5+0,  /*4.9 dBm => rounded to 3 dBm*/          5+0,  /*5.2 dBm*/
      5+0,   /*5.4 dBm*/                             6+0   /*5.6 dBm => rounded to 6 dBm*/
#endif /*USE_HIGH_PWR_PA*/
};

#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)
static const int8_t extPaTableHighEfficient[] = {
    -31+18,  /*-31.1 dBm*/                         -25+18,  /*-25 dBm*/
    -19+18,  /*-19 dBm*/                           -16+18,  /*-15.6 dBm => rounded to -16 dBm*/
    -13+18,  /*-13.1 dBm*/                         -11+18,  /*-11.2 dBm*/
    -10+18,  /*-9.6 dBm => rounded to -10 dBm*/     -8+18,  /*-8.3 dBm*/
     -7+18,  /*-7.2 dBm*/                           -6+18,  /*-6.2 dBm*/
     -5+18,  /*-5.3 dBm*/                           -5+18,  /*-4.5 dBm => rounded to -5 dBm*/
     -4+18,  /*-3.8 dBm => rounded to -4 dBm*/      -3+18,  /*-3.1 dBm*/
     -3+18,  /*-2.5 dBm => rounded to -3 dBm*/      -2+18,  /*-1.9 dBm => rounded to -2 dBm*/
     -1+18,  /*-1.4 dBm => rounded to -1 dBm*/      -1+18,  /*-1.0 dBm*/
      0+18,  /*-0.5 dBm => rounded t0 0 dBm*/        0+18,  /*-0.1 dBm*/
      0+18,  /*0.3 dBm*/                             1+18,  /*0.7 dBm => rounded to 1 dBm*/
      1+18,  /*1.1 dBm*/                             1+18,  /*1.4 dBm*/
      2+18,  /*1.8 dBm => rounded to 2 dBm*/         2+18,  /*2.1 dBm*/
      2+18,  /*2.4 dBm*/                             3+18,  /*2.6 dBm => rounded to 3 dBm*/
      3+18,  /*2.9 dBm => rounded to 3 dBm*/         3+18,  /*3.1 dBm*/
      3+18,  /*3.3 dBm*/                             4+18,  /*3.5 dBm => rounded to 4 dBm*/

#ifdef USE_HIGH_PWR_PA
    /*high_tx_power[] value were obtained from the MKW37 datasheet section 4.3 table 7*/
    -30+18,  /*-29.55 dBm => rounded to -30 dbm*/   -23+18,  /*-23.4 dBm*/
    -17+18,  /*-17.45 dBm*/                         -14+18,  /*-13.94 dBm => rounded to -14 dBm*/
    -11+18,  /*-11.45 dBm*/                         -10+18,  /*-9.53 dBm => rounded to -10 dBm*/
     -8+18,  /*-7.96 dBm => rounded to -8 dBm*/      -7+18,  /*-6.63 dBm => rounded to -7 dBm*/
     -5+18,  /*-5.5 dBm => rounded to -5 dBm */      -4+18,  /*-4.49 dBm*/
     -4+18,  /*-3.59 dBm == rounded to -4 dBm*/      -3+18,  /*-2.77 dBm => rounded to -3 dBm*/
     -2+18,  /*-2.03 dBm*/                           -1+18,  /*-1.35 dBm*/
     -1+18,  /*-0.72 dBm => rounded to -1 dBm*/       0+18,  /*-0.14 dBm*/
      0+18,  /*0.34 dBm*/                             1+18,  /*0.85 dBm => rounded to 1 dBm*/
      1+18,  /*1.32 dBm*/                             0+18,  /*1.77 dBm => rounded to 2 dBm*/
      2+18,  /*2.20 dBm*/                             3+18,  /*2.62 dBm => rounded to 3 dBm*/
      3+18,  /*3.01 dBm*/                             3+18,  /*3.38 dBm*/
      4+18,  /*3.7 dBm => rounded to 4 dBm*/          4+18,  /*4 dBm*/
      4+18,  /*4.4 dBm*/                              5+18,  /*4.6 dBm => rounded to 5 dBm*/
      5+18,  /*4.9 dBm => rounded to 3 dBm*/          5+18,  /*5.2 dBm*/
      5+18,   /*5.4 dBm*/                             6+18   /*5.6 dBm => rounded to 6 dBm*/
#endif /*USE_HIGH_PWR_PA*/
};
#endif

static const int8_t extPaTableLinear[] = {
    -31+18,  /*-31.1 dBm*/                         -25+18,  /*-25 dBm*/
    -19+18,  /*-19 dBm*/                           -16+18,  /*-15.6 dBm => rounded to -16 dBm*/
    -13+18,  /*-13.1 dBm*/                         -11+18,  /*-11.2 dBm*/
    -10+18,  /*-9.6 dBm => rounded to -10 dBm*/     -8+18,  /*-8.3 dBm*/
     -7+18,  /*-7.2 dBm*/                           -6+18,  /*-6.2 dBm*/
     -5+18,  /*-5.3 dBm*/                           -5+18,  /*-4.5 dBm => rounded to -5 dBm*/
     -4+18,  /*-3.8 dBm => rounded to -4 dBm*/      -3+18,  /*-3.1 dBm*/
     -3+18,  /*-2.5 dBm => rounded to -3 dBm*/      -2+18,  /*-1.9 dBm => rounded to -2 dBm*/
     -1+18,  /*-1.4 dBm => rounded to -1 dBm*/      -1+18,  /*-1.0 dBm*/
      0+18,  /*-0.5 dBm => rounded t0 0 dBm*/        0+18,  /*-0.1 dBm*/
      0+18,  /*0.3 dBm*/                             1+18,  /*0.7 dBm => rounded to 1 dBm*/
      1+18,  /*1.1 dBm*/                             1+18,  /*1.4 dBm*/
      2+18,  /*1.8 dBm => rounded to 2 dBm*/         2+18,  /*2.1 dBm*/
      2+18,  /*2.4 dBm*/                             3+18,  /*2.6 dBm => rounded to 3 dBm*/
      3+18,  /*2.9 dBm => rounded to 3 dBm*/         3+18,  /*3.1 dBm*/
      3+18,  /*3.3 dBm*/                             4+18,  /*3.5 dBm => rounded to 4 dBm*/

#ifdef USE_HIGH_PWR_PA
    /*high_tx_power[] value were obtained from the MKW37 datasheet section 4.3 table 7*/
    -30+18,  /*-29.55 dBm => rounded to -30 dbm*/   -23+18,  /*-23.4 dBm*/
    -17+18,  /*-17.45 dBm*/                         -14+18,  /*-13.94 dBm => rounded to -14 dBm*/
    -11+18,  /*-11.45 dBm*/                         -10+18,  /*-9.53 dBm => rounded to -10 dBm*/
     -8+18,  /*-7.96 dBm => rounded to -8 dBm*/      -7+18,  /*-6.63 dBm => rounded to -7 dBm*/
     -5+18,  /*-5.5 dBm => rounded to -5 dBm */      -4+18,  /*-4.49 dBm*/
     -4+18,  /*-3.59 dBm == rounded to -4 dBm*/      -3+18,  /*-2.77 dBm => rounded to -3 dBm*/
     -2+18,  /*-2.03 dBm*/                           -1+18,  /*-1.35 dBm*/
     -1+18,  /*-0.72 dBm => rounded to -1 dBm*/       0+18,  /*-0.14 dBm*/
      0+18,  /*0.34 dBm*/                             1+18,  /*0.85 dBm => rounded to 1 dBm*/
      1+18,  /*1.32 dBm*/                             0+18,  /*1.77 dBm => rounded to 2 dBm*/
      2+18,  /*2.20 dBm*/                             3+18,  /*2.62 dBm => rounded to 3 dBm*/
      3+18,  /*3.01 dBm*/                             3+18,  /*3.38 dBm*/
      4+18,  /*3.7 dBm => rounded to 4 dBm*/          4+18,  /*4 dBm*/
      4+18,  /*4.4 dBm*/                              5+18,  /*4.6 dBm => rounded to 5 dBm*/
      5+18,  /*4.9 dBm => rounded to 3 dBm*/          5+18,  /*5.2 dBm*/
      5+18,   /*5.4 dBm*/                             6+18   /*5.6 dBm => rounded to 6 dBm*/
#endif /*USE_HIGH_PWR_PA*/
};

static const gpioOutputPinConfig_t *ext_pa_gpio_config = NULL;

static uint32_t xcvr_misc_fad_ctrl;
static uint32_t xcvr_tsm_timing49 = 0;
static uint32_t xcvr_tsm_timing50 = 0;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Public Variables
*************************************************************************************
********************************************************************************** */

/* Public variable used by Link layer to get dbm conversion table 
   Do not rename this variable as it is used by BLE LL */
const int8_t *BOARD_PaTable;


/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

BOARD_ExtPaStatus_t BOARD_ExtPaInit(const gpioOutputPinConfig_t  *pinCfg)
{
    ext_pa_gpio_config = pinCfg;
    
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);

    for (int i = 0 ; i<cExtPaPins_d; i++)\
    {
        gpio_pin_config_t pin_config = {
            .pinDirection = kGPIO_DigitalOutput,
            .outputLogic = pinCfg[i].outputLogic
        };
        /* Initialize GPIO functionality   */
        GPIO_PinInit(maGpioBases[pinCfg[i].gpioPort], pinCfg[i].gpioPin, &pin_config);

        /* Initialize PinMux for this pin */
        PORT_SetPinMux(maPortBases[pinCfg[i].gpioPort], pinCfg[i].gpioPin, kPORT_MuxAsGpio);
    }

    /* Configure CTX and CRX to pins PTAC2 and PTAC3 respectively  */
    PORT_SetPinMux(PORTC, 2, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTC, 3, kPORT_MuxAlt2);

    /* update the PA table according to the PIN config */
    BOARD_ExtPaUpdatePaTable();

    return BOARD_ExtPaSuccess;
}


/* Update the PA table according to the PA configuration */
void BOARD_ExtPaUpdatePaTable(void)
{
#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)    
    uint8_t chl = GpioReadOutputPin((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CHL]);
#else
    /* CHL is pulled high in this case */
    uint8_t chl = 1U;
#endif
    uint8_t cps = GpioReadOutputPin((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CPS]);

    if( cps != 0U )
    {
        /* bypass mode */
        BOARD_PaTable = &extPaTableBypass[0];
    }
    else if (chl != 0 )
    {
        /* linear mode */   
        BOARD_PaTable = &extPaTableLinear[0];    
    }
    else
    {
#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)
        /* high efficiency mode */   
        BOARD_PaTable = &extPaTableHighEfficient[0];  
#else
        assert(0);
#endif
    }
}

BOARD_ExtPaStatus_t BOARD_ExtPaXcvrInit(bool_t init_all)
{
    BOARD_ExtPaStatus_t status;
    if( init_all )
    {
        xcvr_pa_fem_config_t xcvr_pa_fem;
        
        assert(ext_pa_gpio_config!=NULL);
        
        //assert(1==GpioReadOutputPin((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CSD]));
        //assert(0==GpioReadOutputPin((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CPS]));
        //assert(1==GpioReadOutputPin((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[ANT_SEL]));
        
        /* PA/FEM control single mode, minimum delays, TX/RX under TSM control, FAD disabled */
        xcvr_pa_fem.op_mode                 = XCVR_ANTX_SINGLE_MODE;
        xcvr_pa_fem.ant_sel_pins_enable     = 0; /* No ANT selection */
        xcvr_pa_fem.tx_rx_switch_pins_enable = 1; /* TX/RX switch pins active */
        xcvr_pa_fem.high_z_enable           = 0; /* No High Z */
        xcvr_pa_fem.use_fad_state_machine   = 0; /* FAD state machine off */

    #if 1 // keep as KW41 - XCVR driver will configure CTX and CRX pads
        xcvr_pa_fem.ant_a_pad_control       = XCVR_FAD_TSM_GPIO;
        xcvr_pa_fem.ant_b_pad_control       = XCVR_FAD_TSM_GPIO;
        xcvr_pa_fem.tx_switch_pad_control   = XCVR_FAD_STATE_MACHINE;
        xcvr_pa_fem.rx_switch_pad_control   = XCVR_FAD_STATE_MACHINE;
    #else // test code
        xcvr_pa_fem.ant_a_pad_control       = XCVR_FAD_STATE_MACHINE; /* leave ANT A under FAD control (disabled) */
        xcvr_pa_fem.ant_b_pad_control       = XCVR_FAD_STATE_MACHINE; /* leave ANT B under FAD control (disabled) */
        xcvr_pa_fem.tx_switch_pad_control   = XCVR_FAD_TSM_GPIO;      /* TX switch under TSM control */
        xcvr_pa_fem.rx_switch_pad_control   = XCVR_FAD_TSM_GPIO;      /* RX switch under TSM control */
    #endif

        xcvr_pa_fem.pa_tx_wu                = 1; /* 1 usec warmup time for external PA */
        xcvr_pa_fem.pa_tx_wd                = 0; /* 0 usec warmdown time for external PA */
        xcvr_pa_fem.lna_rx_wu               = 1; /* 1 usec warmup time for external LNA */
        xcvr_pa_fem.lna_rx_wd               = 0; /* 0 usec warmdown time for external LNA */

        xcvr_pa_fem.tx_switch_pol_control   = XCVR_FAD_ACTIVE_HIGH; /* TX switch signaling is active high */
        xcvr_pa_fem.rx_switch_pol_control   = XCVR_FAD_ACTIVE_HIGH; /* RX switch signaling is active high */

        /* Initialize CTX and CRX pads and XCVR*/
        if( XCVR_ExternalFadPaFemInit(&xcvr_pa_fem) ==  gXcvrSuccess_c)
        {
            /* save the timing for later usage */
            xcvr_tsm_timing49 = XCVR_TSM->TIMING49;
            xcvr_tsm_timing50 = XCVR_TSM->TIMING50;
            xcvr_misc_fad_ctrl = XCVR_MISC->FAD_CTRL;
                
            status = BOARD_ExtPaSuccess;
        }
        else
        {
            assert(0);
            status = BOARD_ExtPaInvalidParamter;
        }
    }
    else
    {
        assert(xcvr_tsm_timing49!=0);
        assert(xcvr_tsm_timing50!=0);

        /* save the timing for later usage */
        XCVR_TSM->TIMING49 = xcvr_tsm_timing49;
        XCVR_TSM->TIMING50 = xcvr_tsm_timing50;
        XCVR_MISC->FAD_CTRL = xcvr_misc_fad_ctrl;
        
        status = BOARD_ExtPaSuccess;
    }
    
    /* update the PA table according to the PIN config */
    BOARD_ExtPaUpdatePaTable();
    
    return status;
}

void  BOARD_ExtPaSleep(void)
{
    assert(ext_pa_gpio_config);

    GpioClearPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CSD]);

    return;
}

void  BOARD_ExtPaActive(void)
{
    assert(ext_pa_gpio_config);

    GpioSetPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CSD]);

    return;
}


void  BOARD_ExtPaBypassEnable(void)
{
    assert(ext_pa_gpio_config);

    GpioSetPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CPS]);

    BOARD_ExtPaUpdatePaTable();
    
    return;
}


void  BOARD_ExtPaBypassDisable(void)
{
    assert(ext_pa_gpio_config);

    GpioClearPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CPS]);

    BOARD_ExtPaUpdatePaTable();
    
    return;
}


void  BOARD_ExtPaSelTxModeLinear(void)
{
    assert(ext_pa_gpio_config);

    GpioClearPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CPS]);

#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)
    GpioSetPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CHL]);
#endif
   
    BOARD_ExtPaUpdatePaTable();

    return;
}

#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)
void  BOARD_ExtPaSelTxModeHighEfficiency(void)
{
    assert(ext_pa_gpio_config);

    GpioClearPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CPS]);

    GpioClearPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[CHL]);

    BOARD_ExtPaUpdatePaTable();

    return;
}
#endif

void  BOARD_ExtPaSelAnt1(void)
{
    assert(ext_pa_gpio_config);

    GpioClearPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[ANT_SEL]);

    return;
}


void  BOARD_ExtPaSelAnt2(void)
{
    assert(ext_pa_gpio_config);

    GpioSetPinOutput((const gpioOutputPinConfig_t*)&ext_pa_gpio_config[ANT_SEL]);

    return;
}

void  BOARD_ExtPaGetConvTable(const int8_t **ConvPaTablePtr)
{
    assert(ext_pa_gpio_config);

    *ConvPaTablePtr = BOARD_PaTable;

    return;
}

