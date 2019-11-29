/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_REPORTING_H_
#define APP_REPORTING_H_

/*!
\file       app_reporting.h
\brief      ZLO Demo : LightSensor Report
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "zcl.h"
#include "PDM.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
    uint16 u16ClusterID;
    tsZCL_AttributeReportingConfigurationRecord sAttributeReportingConfigurationRecord;
}tsReports;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC PDM_teStatus eRestoreReports(void);
PUBLIC void vMakeSupportedAttributesReportable(void);
PUBLIC void vLoadDefaultConfigForReportable(void);
PUBLIC void vSaveReportableRecord(uint16 u16ClusterID, tsZCL_AttributeReportingConfigurationRecord* psAttributeReportingConfigurationRecord);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/

/****************************************************************************/
/****************************************************************************/

#endif //APP_REPORTING_H_
