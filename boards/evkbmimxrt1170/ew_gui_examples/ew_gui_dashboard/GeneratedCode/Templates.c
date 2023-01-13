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
#include "_TemplatesHorizontalValueBar.h"
#include "_TemplatesVerticalValueBar.h"
#include "Templates.h"

/* Initializer for the class 'Templates::HorizontalValueBar' */
void TemplatesHorizontalValueBar__Init( TemplatesHorizontalValueBar _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( TemplatesHorizontalValueBar );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( TemplatesHorizontalValueBar );
}

/* Re-Initializer for the class 'Templates::HorizontalValueBar' */
void TemplatesHorizontalValueBar__ReInit( TemplatesHorizontalValueBar _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Templates::HorizontalValueBar' */
void TemplatesHorizontalValueBar__Done( TemplatesHorizontalValueBar _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Variants derived from the class : 'Templates::HorizontalValueBar' */
EW_DEFINE_CLASS_VARIANTS( TemplatesHorizontalValueBar )
EW_END_OF_CLASS_VARIANTS( TemplatesHorizontalValueBar )

/* Virtual Method Table (VMT) for the class : 'Templates::HorizontalValueBar' */
EW_DEFINE_CLASS( TemplatesHorizontalValueBar, CoreGroup, _None, _None, _None, _None, 
                 _None, _None, "Templates::HorizontalValueBar" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( TemplatesHorizontalValueBar )

/* Initializer for the class 'Templates::VerticalValueBar' */
void TemplatesVerticalValueBar__Init( TemplatesVerticalValueBar _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( TemplatesVerticalValueBar );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( TemplatesVerticalValueBar );
}

/* Re-Initializer for the class 'Templates::VerticalValueBar' */
void TemplatesVerticalValueBar__ReInit( TemplatesVerticalValueBar _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Templates::VerticalValueBar' */
void TemplatesVerticalValueBar__Done( TemplatesVerticalValueBar _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Variants derived from the class : 'Templates::VerticalValueBar' */
EW_DEFINE_CLASS_VARIANTS( TemplatesVerticalValueBar )
EW_END_OF_CLASS_VARIANTS( TemplatesVerticalValueBar )

/* Virtual Method Table (VMT) for the class : 'Templates::VerticalValueBar' */
EW_DEFINE_CLASS( TemplatesVerticalValueBar, CoreGroup, _None, _None, _None, _None, 
                 _None, _None, "Templates::VerticalValueBar" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( TemplatesVerticalValueBar )

/* Embedded Wizard */
