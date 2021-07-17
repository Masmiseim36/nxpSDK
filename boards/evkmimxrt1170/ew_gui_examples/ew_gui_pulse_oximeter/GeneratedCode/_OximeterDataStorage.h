/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software 
* components. It is provided as an example software which is intended to be 
* modified and extended according to particular requirements.
* 
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability 
* and non-infringement of any third party IPR or other rights which may result 
* from the use or the inability to use the software.
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* Version  : 10.0
* Date     : 17.02.2021  8:00:50
* Profile  : iMX_RT
* Platform : NXP.iMX_RT_VGLite.RGB565
*
*******************************************************************************/

#ifndef _OximeterDataStorage_H
#define _OximeterDataStorage_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

/* Forward declaration of the class Oximeter::DataStorage */
#ifndef _OximeterDataStorage_
  EW_DECLARE_CLASS( OximeterDataStorage )
#define _OximeterDataStorage_
#endif


/* Deklaration of class : 'Oximeter::DataStorage' */
EW_DEFINE_FIELDS( OximeterDataStorage, XObject )
  EW_ARRAY   ( Data,            XFloat, [1000])
  EW_PROPERTY( NoOfData,        XInt32 )
  EW_PROPERTY( Current,         XInt32 )
  EW_ARRAY   ( Slope,           XFloat, [1000])
EW_END_OF_FIELDS( OximeterDataStorage )

/* Virtual Method Table (VMT) for the class : 'Oximeter::DataStorage' */
EW_DEFINE_METHODS( OximeterDataStorage, XObject )
EW_END_OF_METHODS( OximeterDataStorage )

/* 'C' function for method : 'Oximeter::DataStorage.OnSetCurrent()' */
void OximeterDataStorage_OnSetCurrent( OximeterDataStorage _this, XInt32 value );

/* 'C' function for method : 'Oximeter::DataStorage.AddData()' */
void OximeterDataStorage_AddData( OximeterDataStorage _this, XFloat aData );

/* 'C' function for method : 'Oximeter::DataStorage.GetData()' */
XFloat OximeterDataStorage_GetData( OximeterDataStorage _this, XInt32 aIndex );

/* 'C' function for method : 'Oximeter::DataStorage.GetSlope()' */
XFloat OximeterDataStorage_GetSlope( OximeterDataStorage _this, XInt32 aIndex );

/* Default onget method for the property 'Current' */
XInt32 OximeterDataStorage_OnGetCurrent( OximeterDataStorage _this );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterDataStorage_H */

/* Embedded Wizard */
