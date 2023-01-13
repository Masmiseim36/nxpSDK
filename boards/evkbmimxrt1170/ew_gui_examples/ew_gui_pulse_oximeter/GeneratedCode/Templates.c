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

#include "ewlocale.h"
#include "_TemplatesDeviceClass.h"
#include "Templates.h"

/* Initializer for the class 'Templates::DeviceClass' */
void TemplatesDeviceClass__Init( TemplatesDeviceClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( TemplatesDeviceClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( TemplatesDeviceClass );
}

/* Re-Initializer for the class 'Templates::DeviceClass' */
void TemplatesDeviceClass__ReInit( TemplatesDeviceClass _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Templates::DeviceClass' */
void TemplatesDeviceClass__Done( TemplatesDeviceClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'Templates::DeviceClass' */
EW_DEFINE_CLASS_VARIANTS( TemplatesDeviceClass )
EW_END_OF_CLASS_VARIANTS( TemplatesDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Templates::DeviceClass' */
EW_DEFINE_CLASS( TemplatesDeviceClass, XObject, _None, _None, _None, _None, _None, 
                 _None, "Templates::DeviceClass" )
EW_END_OF_CLASS( TemplatesDeviceClass )

/* Embedded Wizard */
