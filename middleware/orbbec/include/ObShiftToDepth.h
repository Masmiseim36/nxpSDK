/*****************************************************************************
*									     *
*  OpenNI 2.x Alpha							     *
*  Copyright (C) 2012 PrimeSense Ltd.					     *
*									     *
*  This file is part of OpenNI. 					     *
*									     *
*  Licensed under the Apache License, Version 2.0 (the "License");	     *
*  you may not use this file except in compliance with the License.	     *
*  You may obtain a copy of the License at				     *
*									     *
*      http://www.apache.org/licenses/LICENSE-2.0			     *
*									     *
*  Unless required by applicable law or agreed to in writing, software	     *
*  distributed under the License is distributed on an "AS IS" BASIS,	     *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and	     *
*  limitations under the License.					     *
*									     *
*****************************************************************************/
#ifndef _SHIFTTODEPTH_H
#define _SHIFTTODEPTH_H
#include <stdint.h>
#include <stdbool.h>
//---------------------------------------------------------------------------
// Includes

//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------
typedef struct ShiftToDepthConfig
{
	/** The zero plane distance in depth units. */
	float fZeroPlaneDistance;
	/** The zero plane pixel size */
	float fZeroPlanePixelSize;
	/** The distance between the emitter and the Depth Cmos */
	float fEmitterDCmosDistance;
	/** The maximum possible shift value from this device. */
	uint32_t nDeviceMaxShiftValue;
	/** The maximum possible depth from this device (as opposed to a cut-off). */
	uint32_t nDeviceMaxDepthValue;

	uint32_t nConstShift;
	uint32_t nPixelSizeFactor;
	uint32_t nParamCoeff;
	uint32_t nShiftScale;

	uint16_t nDepthMinCutOff;
	uint16_t nDepthMaxCutOff;

	float fVerticalFOV;
	float fHorizontalFOV;
} ShiftToDepthConfig;

typedef struct ShiftToDepthTables
{
	bool bIsInitialized;
	/** The shift-to-depth table. */
	uint16_t *pShiftToDepthTable;
	/** The number of entries in the shift-to-depth table. */
	uint32_t nShiftsCount;
	/** The depth-to-shift table. */
	uint16_t *pDepthToShiftTable;
	/** The number of entries in the depth-to-shift table. */
	uint32_t nDepthsCount;
} ShiftToDepthTables;

enum OpticsRelationShip
{
	ProjectLeftOfCMOS,
	ProjectRightOfCMOS
};

#define _PIXEL_FORMAT_DEPTH_1_MM 1
#define _PIXEL_FORMAT_DEPTH_100_UM 100

#define _MIN_(a, b) (((a) < (b)) ? (a) : (b))
//---------------------------------------------------------------------------
// Functions Declaration
//---------------------------------------------------------------------------
int ShiftToDepthInit(ShiftToDepthTables *pShiftToDepth, const ShiftToDepthConfig *pConfig);
int ShiftToDepthUpdate(ShiftToDepthTables *pShiftToDepth, const ShiftToDepthConfig *pConfig, int zFactor);
int ShiftToDepthConvert(ShiftToDepthTables *pShiftToDepth, uint16_t *pInput, uint32_t nInputSize, uint16_t *pOutput);
int ShiftToDepthFree(ShiftToDepthTables *pShiftToDepth);

#endif // _SHIFTTODEPTH_H
