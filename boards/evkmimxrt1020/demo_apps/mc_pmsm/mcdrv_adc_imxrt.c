/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_imxrt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/

static bool_t s_statusPass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Reads and calculates 3 phase samples based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShGet(mcdrv_adc_t *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;

    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)    
    {
        case 2:
        case 3:
            /* direct sensing of phase A and C, calculation of B */
            sIABCtemp.f16A = MLIB_ShLSat_F16(((frac16_t)((int16_t)
                             (this->sCurrSec23.pAdcBasePhaA->R[this->sCurrSec23.ui16RsltRegPhaA]) << 5U)
                               - this->sCurrSec23.ui16OffsetPhaA), 1);
            
            sIABCtemp.f16C = MLIB_ShLSat_F16(((frac16_t)((int16_t)
                             (this->sCurrSec23.pAdcBasePhaC->R[this->sCurrSec23.ui16RsltRegPhaC]) << 5U) 
                               - this->sCurrSec23.ui16OffsetPhaC), 1);
            
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));
            break;

        case 4:
        case 5:
            /* direct sensing of phase A and B, calculation of C */
            sIABCtemp.f16A = MLIB_ShLSat_F16(((frac16_t)((int16_t)
                             (this->sCurrSec45.pAdcBasePhaA->R[this->sCurrSec45.ui16RsltRegPhaA]) << 5U) 
                               - this->sCurrSec45.ui16OffsetPhaA), 1);
            
            sIABCtemp.f16B = MLIB_ShLSat_F16(((frac16_t)((int16_t)
                             (this->sCurrSec45.pAdcBasePhaB->R[this->sCurrSec45.ui16RsltRegPhaB]) << 5U)
                               - this->sCurrSec45.ui16OffsetPhaB), 1);
            
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));
            break;

        case 1:
        case 6:
        default:
            /* direct sensing of phase B and C, calculation of A */
            sIABCtemp.f16B = MLIB_ShLSat_F16(((frac16_t)((int16_t)
                             (this->sCurrSec16.pAdcBasePhaB->R[this->sCurrSec16.ui16RsltRegPhaB]) << 5U) 
                               - this->sCurrSec16.ui16OffsetPhaB), 1);
            
            sIABCtemp.f16C = MLIB_ShLSat_F16(((frac16_t)((int16_t)
                             (this->sCurrSec16.pAdcBasePhaC->R[this->sCurrSec16.ui16RsltRegPhaC]) << 5U) 
                               - this->sCurrSec16.ui16OffsetPhaC), 1);
            
            sIABCtemp.f16A = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16B, sIABCtemp.f16C));
            break;
    }

    /* pass measured phase currents to the main module structure */
    this->psIABC->f16A = sIABCtemp.f16A;
    this->psIABC->f16B = sIABCtemp.f16B;
    this->psIABC->f16C = sIABCtemp.f16C;

    return (s_statusPass);
}

/*!
 * @brief Set initial channel assignment for phase currents & DCB voltage
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssignInit(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;
    
    /* sector 1&6, direct sensing of phases B and C */
    this->sCurrSec16.pui16HCPhaB = 
      (uint16_t *)&this->sCurrSec16.pAdcBasePhaB->HC[this->sCurrSec16.ui16RsltRegPhaB];
    this->sCurrSec16.pui16HCPhaC = 
      (uint16_t *)&this->sCurrSec16.pAdcBasePhaC->HC[this->sCurrSec16.ui16RsltRegPhaC];
    
    /* sector 2&3, direct sensing of phases A and C */
    this->sCurrSec23.pui16HCPhaA = 
      (uint16_t *)&this->sCurrSec23.pAdcBasePhaA->HC[this->sCurrSec23.ui16RsltRegPhaA];
    this->sCurrSec23.pui16HCPhaC = 
      (uint16_t *)&this->sCurrSec23.pAdcBasePhaC->HC[this->sCurrSec23.ui16RsltRegPhaC];
    
    /* sector 4&5, direct sensing of phases A and B */
    this->sCurrSec45.pui16HCPhaA = 
      (uint16_t *)&this->sCurrSec45.pAdcBasePhaA->HC[this->sCurrSec45.ui16RsltRegPhaA];
    this->sCurrSec45.pui16HCPhaB = 
      (uint16_t *)&this->sCurrSec45.pAdcBasePhaB->HC[this->sCurrSec45.ui16RsltRegPhaB]; 
    
    /* Vdcb sampling */
    this->pui16HCVDcb = (uint16_t *)&this->pui32UdcbAdcBase->HC[this->ui16RsltRegVDcb];
    
    /* default channel assignment for default SVM sector = 2 */
    /* direct sensing of phases A and C */
    /* HCx update with ADC channel - phase current A */
    *this->sCurrSec23.pui16HCPhaA = ((this->sCurrSec23.ui16ChanNumPhaA)); 
    
    /* HCx update with ADC channel - phase current C */  
    *this->sCurrSec23.pui16HCPhaC = ((this->sCurrSec23.ui16ChanNumPhaC));
      
    /* set particular HCx for VDcb channel */
    *this->pui16HCVDcb = ((this->ui16ChanNumVDcb) | (ADC_HC_AIEN_MASK));
        
    return (s_statusPass);
}

    
/*!
 * @brief Set new channel assignment for next sampling based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssign(mcdrv_adc_t *this)
{
    
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)   
    {
        /* direct sensing of phases A and C */
        case 2:
        case 3:
            /* HCx update with ADC channel - phase current A */       
            *this->sCurrSec23.pui16HCPhaA = ((this->sCurrSec23.ui16ChanNumPhaA)) ;          
          
            /* HCx update with ADC channel - phase current C */ 
            *this->sCurrSec23.pui16HCPhaC = ((this->sCurrSec23.ui16ChanNumPhaC));
          
            break;

        /* direct sensing of phases A and B  */
        case 4:
        case 5:
            /* HCx update with ADC channel - phase current A */
           *this->sCurrSec45.pui16HCPhaA = ((this->sCurrSec45.ui16ChanNumPhaA));
          
            /* HCx update with ADC channel - phase current B */ 
           *this->sCurrSec45.pui16HCPhaB = ((this->sCurrSec45.ui16ChanNumPhaB));      
       
            break;

        /* direct sensing of phases B and C */
        case 1:
        case 6:
        default:
            /* HCx update with ADC channel - phase current B */             
           *this->sCurrSec16.pui16HCPhaB = ((this->sCurrSec16.ui16ChanNumPhaB));   
          
            /* HCx update with ADC channel - phase current C */ 
           *this->sCurrSec16.pui16HCPhaC = ((this->sCurrSec16.ui16ChanNumPhaC));
          
            break;
    }
    return (s_statusPass);
}

/*!
 * @brief Initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibInit(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;

    /* clear offset values */
    this->sCurrSec16.ui16OffsetPhaB = 0;
    this->sCurrSec16.ui16OffsetPhaC = 0;
    this->sCurrSec23.ui16OffsetPhaA = 0;
    this->sCurrSec23.ui16OffsetPhaC = 0;
    this->sCurrSec45.ui16OffsetPhaA = 0;
    this->sCurrSec45.ui16OffsetPhaB = 0;

    this->sCurrSec16.ui16CalibPhaB = 0;
    this->sCurrSec16.ui16CalibPhaC = 0;
    this->sCurrSec23.ui16CalibPhaA = 0;
    this->sCurrSec23.ui16CalibPhaC = 0;
    this->sCurrSec45.ui16CalibPhaA = 0;
    this->sCurrSec45.ui16CalibPhaB = 0;

    /* initialize offset filters */
    this->sCurrSec16.ui16FiltPhaB.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec16.ui16FiltPhaC.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec23.ui16FiltPhaA.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec23.ui16FiltPhaC.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec45.ui16FiltPhaA.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec45.ui16FiltPhaB.u16Sh = this->ui16OffsetFiltWindow;

    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec16.ui16FiltPhaB);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec16.ui16FiltPhaC);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec23.ui16FiltPhaA);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec23.ui16FiltPhaC);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec45.ui16FiltPhaA);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec45.ui16FiltPhaB);

    return (s_statusPass);
}

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalib(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* sensing of offset IA -> ADCA and IC -> ADCC */
            this->sCurrSec23.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec23.pAdcBasePhaA->R[this->sCurrSec23.ui16RsltRegPhaA] << 5), 
                                    &this->sCurrSec23.ui16FiltPhaA);
            this->sCurrSec23.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec23.pAdcBasePhaC->R[this->sCurrSec23.ui16RsltRegPhaC] << 5), 
                                    &this->sCurrSec23.ui16FiltPhaC);
            break;

        case 4:
        case 5:
            /* sensing of offset IA -> ADCA and IB -> ADCC */
            this->sCurrSec45.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec45.pAdcBasePhaA->R[this->sCurrSec45.ui16RsltRegPhaA] << 5), 
                                    &this->sCurrSec45.ui16FiltPhaA);
            this->sCurrSec45.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec45.pAdcBasePhaB->R[this->sCurrSec45.ui16RsltRegPhaB] << 5), 
                                    &this->sCurrSec45.ui16FiltPhaB);
            break;

        case 1:
        case 6:
        default:
            /* sensing of offset IB -> ADCA and IC -> ADCC */
            this->sCurrSec16.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec16.pAdcBasePhaB->R[this->sCurrSec16.ui16RsltRegPhaB] << 5),
                                    &this->sCurrSec16.ui16FiltPhaB);
            this->sCurrSec16.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec16.pAdcBasePhaC->R[this->sCurrSec16.ui16RsltRegPhaC] << 5), 
                                    &this->sCurrSec16.ui16FiltPhaC);
            break;
    }

    return (s_statusPass);
}

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibSet(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;

    /* pass calibration data for sector 1 and 6 */
    this->sCurrSec16.ui16OffsetPhaB = this->sCurrSec16.ui16CalibPhaB;
    this->sCurrSec16.ui16OffsetPhaC = this->sCurrSec16.ui16CalibPhaC;

    /* pass calibration data for sector 2 and 3 */
    this->sCurrSec23.ui16OffsetPhaA = this->sCurrSec23.ui16CalibPhaA;
    this->sCurrSec23.ui16OffsetPhaC = this->sCurrSec23.ui16CalibPhaC;

    /* pass calibration data for sector 4 and 5 */
    this->sCurrSec45.ui16OffsetPhaA = this->sCurrSec45.ui16CalibPhaA;
    this->sCurrSec45.ui16OffsetPhaB = this->sCurrSec45.ui16CalibPhaB;

    return (s_statusPass);
}

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_VoltDcBusGet(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;

    /* read DC-bus voltage sample from defined ADCx result register */ 
    *this->pf16UDcBus = (frac16_t)((this->pui32UdcbAdcBase->R[this->ui16RsltRegVDcb]) << 5);

    return (s_statusPass);
}

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;

    /* read Auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = (frac16_t)(this->pui32AuxAdcBase->R[this->ui16RsltRegAux]);

    return (s_statusPass);
}
