/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*!=================================================================================================
\file       app_reporting.c
\brief      ZLO Demo : LightSensor Report (Implementation)
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>
#include "dbg.h"
#include "ZTimer.h"
#include "app_main.h"
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "app_pdum_cfg.h"
#include "app_common.h"
#include "PDM_IDs.h"
#include "zcl_options.h"
#include "NVM_Interface.h"
#include "zcl_common.h"
#include "app_reporting.h"
#include "App_OccupancySensor.h"
#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_REPORT
    #define TRACE_REPORT   TRUE
#else
    #define TRACE_REPORT   FALSE
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/*There is just one attributes at this point - Occupancy Attribute */

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern const uint8 u8MyEndpoint;
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/*Just one reports for time being*/
PRIVATE tsReports asSavedReports[OCCUPANCY_NUMBER_OF_REPORTS];
NVM_RegisterDataSet(asSavedReports, 1, sizeof(asSavedReports), PDM_ID_APP_REPORTS, gNVM_MirroredInRam_c);
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


/****************************************************************************
 *
 * NAME: eRestoreReports
 *
 * DESCRIPTION:
 * Loads the reporting information from the EEPROM/PDM
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC PDM_teStatus eRestoreReports( void )
{
    /* Restore any report data that is previously saved to flash */
    uint16 u16ByteRead;
    PDM_teStatus eStatusReportReload = PDM_eReadDataFromRecord(PDM_ID_APP_REPORTS,
                                                              asSavedReports,
                                                              sizeof(asSavedReports),
                                                              &u16ByteRead);

    DBG_vPrintf(TRACE_REPORT,"\nAPP Report: eStatusReportReload=%d",eStatusReportReload);
    /* Restore any application data previously saved to flash */

    return  (eStatusReportReload);
}

/****************************************************************************
 *
 * NAME: vMakeSupportedAttributesReportable
 *
 * DESCRIPTION:
 * Makes the attributes reportable for Occupancy attribute
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vMakeSupportedAttributesReportable(void)
{
    uint16 u16AttributeEnum;
    uint16 u16ClusterId;
    uint8 i;

    tsZCL_AttributeReportingConfigurationRecord*    psAttributeReportingConfigurationRecord;

    for(i=0; i<OCCUPANCY_NUMBER_OF_REPORTS; i++)
    {
        u16AttributeEnum=asSavedReports[i].sAttributeReportingConfigurationRecord.u16AttributeEnum;
        u16ClusterId= asSavedReports[i].u16ClusterID;
        psAttributeReportingConfigurationRecord = &(asSavedReports[i].sAttributeReportingConfigurationRecord);
        DBG_vPrintf(TRACE_REPORT, "Cluster %04x Attribute %04x Min %d Max %d IntV %d Direct %d Change %d\n",
                u16ClusterId,
                u16AttributeEnum,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u16MinimumReportingInterval,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u16MaximumReportingInterval,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u16TimeoutPeriodField,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u8DirectionIsReceived,
                asSavedReports[i].sAttributeReportingConfigurationRecord.uAttributeReportableChange.zint8ReportableChange);
        eZCL_CreateLocalReport( u8MyEndpoint, u16ClusterId, 0, TRUE, psAttributeReportingConfigurationRecord);
    }
}

/****************************************************************************
 *
 * NAME: vLoadDefaultConfigForReportable
 *
 * DESCRIPTION:
 * Loads a default configuration
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vLoadDefaultConfigForReportable(void)
{
    FLib_MemSet(asSavedReports, 0 ,sizeof(asSavedReports));
    int i;
    for (i=0; i<OCCUPANCY_NUMBER_OF_REPORTS; i++)
    {
        asSavedReports[i] = asDefaultReports[i];
    }

#if TRACE_REPORT

    DBG_vPrintf(TRACE_REPORT,"\nLoaded Defaults Records \n");
    for(i=0; i <OCCUPANCY_NUMBER_OF_REPORTS; i++)
    {
        DBG_vPrintf(TRACE_REPORT,"Cluster %04x Type %d Attr %04x Min %d Max %d IntV %d Direct %d Change %d\n",
                asSavedReports[i].u16ClusterID,
                asSavedReports[i].sAttributeReportingConfigurationRecord.eAttributeDataType,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u16AttributeEnum,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u16MinimumReportingInterval,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u16MaximumReportingInterval,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u16TimeoutPeriodField,
                asSavedReports[i].sAttributeReportingConfigurationRecord.u8DirectionIsReceived,
                asSavedReports[i].sAttributeReportingConfigurationRecord.uAttributeReportableChange.zuint8ReportableChange);
    }
#endif

    /*Save this Records*/
    PDM_eSaveRecordData(PDM_ID_APP_REPORTS,
                        asSavedReports,
                        sizeof(asSavedReports));

}


/****************************************************************************
 *
 * NAME: vSaveReportableRecord
 *
 * DESCRIPTION:
 * Loads a default configuration
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vSaveReportableRecord(  uint16 u16ClusterID,
                                    tsZCL_AttributeReportingConfigurationRecord* psAttributeReportingConfigurationRecord)
{
    int iIndex = 0;

    /*For MeasuredValue attribute in Illuminance Measurement Cluster*/
    asSavedReports[iIndex].u16ClusterID=u16ClusterID;
    FLib_MemCpy( &(asSavedReports[iIndex].sAttributeReportingConfigurationRecord),
            psAttributeReportingConfigurationRecord,
            sizeof(tsZCL_AttributeReportingConfigurationRecord) );

    DBG_vPrintf(TRACE_REPORT,"Cluster %04x Type %d Attrib %04x Min %d Max %d IntV %d Direction %d Change %d\n",
            asSavedReports[iIndex].u16ClusterID,
            asSavedReports[iIndex].sAttributeReportingConfigurationRecord.eAttributeDataType,
            asSavedReports[iIndex].sAttributeReportingConfigurationRecord.u16AttributeEnum,
            asSavedReports[iIndex].sAttributeReportingConfigurationRecord.u16MinimumReportingInterval,
            asSavedReports[iIndex].sAttributeReportingConfigurationRecord.u16MaximumReportingInterval,
            asSavedReports[iIndex].sAttributeReportingConfigurationRecord.u16TimeoutPeriodField,
            asSavedReports[iIndex].sAttributeReportingConfigurationRecord.u8DirectionIsReceived,
            asSavedReports[iIndex].sAttributeReportingConfigurationRecord.uAttributeReportableChange.zuint8ReportableChange );

    /*Save this Records*/
    PDM_eSaveRecordData(PDM_ID_APP_REPORTS,
                        asSavedReports,
                        sizeof(asSavedReports));
}

/****************************************************************************
 *
 * NAME: vSendImmediateReport
 *
 * DESCRIPTION:
 * Method to send a report to all bound nodes.
 *
 ****************************************************************************/
PUBLIC void vSendImmediateReport(void)
{
    PDUM_thAPduInstance myPDUM_thAPduInstance;
    tsZCL_Address sDestinationAddress;

    DBG_vPrintf(TRACE_REPORT, "\nAPP Report: Sending Report");

    /* get buffer to write the response in*/
    myPDUM_thAPduInstance = hZCL_AllocateAPduInstance();
    /* no buffers - return*/
    if(myPDUM_thAPduInstance == PDUM_INVALID_HANDLE)
    {
        DBG_vPrintf(TRACE_REPORT, "\nAPP Report: PDUM_INVALID_HANDLE");
    }

    /* Set the address mode to send to all bound device and don't wait for an ACK*/
    sDestinationAddress.eAddressMode = E_ZCL_AM_BOUND_NO_ACK;

    /* Send the report with all attributes.*/
    if (E_ZCL_SUCCESS != eZCL_ReportAllAttributes(&sDestinationAddress,
                                                  MEASUREMENT_AND_SENSING_CLUSTER_ID_OCCUPANCY_SENSING,
                                                  1,
                                                  0,
                                                  myPDUM_thAPduInstance))
    {

    	DBG_vPrintf(TRACE_REPORT, "\nAPP Report: Error Sending Report");
    }

    DBG_vPrintf(TRACE_REPORT, "\nAPP Report: Report Sent");
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
