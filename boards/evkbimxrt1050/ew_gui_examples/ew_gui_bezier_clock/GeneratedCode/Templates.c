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
* Version  : 9.30
* Date     : 14.02.2020  8:00:50
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#include "ewlocale.h"
#include "_TemplatesPickerControl.h"
#include "_TemplatesRotaryKnob.h"
#include "Templates.h"

/* Initializer for the class 'Templates::RotaryKnob' */
void TemplatesRotaryKnob__Init( TemplatesRotaryKnob _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( TemplatesRotaryKnob );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( TemplatesRotaryKnob );
}

/* Re-Initializer for the class 'Templates::RotaryKnob' */
void TemplatesRotaryKnob__ReInit( TemplatesRotaryKnob _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Templates::RotaryKnob' */
void TemplatesRotaryKnob__Done( TemplatesRotaryKnob _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Variants derived from the class : 'Templates::RotaryKnob' */
EW_DEFINE_CLASS_VARIANTS( TemplatesRotaryKnob )
EW_END_OF_CLASS_VARIANTS( TemplatesRotaryKnob )

/* Virtual Method Table (VMT) for the class : 'Templates::RotaryKnob' */
EW_DEFINE_CLASS( TemplatesRotaryKnob, CoreGroup, _None, _None, _None, _None, _None, 
                 _None, "Templates::RotaryKnob" )
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
  CoreGroup_drawContent,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_RestackTop,
  CoreGroup_Remove,
  CoreGroup_Add,
EW_END_OF_CLASS( TemplatesRotaryKnob )

/* Initializer for the class 'Templates::PickerControl' */
void TemplatesPickerControl__Init( TemplatesPickerControl _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( TemplatesPickerControl );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( TemplatesPickerControl );
}

/* Re-Initializer for the class 'Templates::PickerControl' */
void TemplatesPickerControl__ReInit( TemplatesPickerControl _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Templates::PickerControl' */
void TemplatesPickerControl__Done( TemplatesPickerControl _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Variants derived from the class : 'Templates::PickerControl' */
EW_DEFINE_CLASS_VARIANTS( TemplatesPickerControl )
EW_END_OF_CLASS_VARIANTS( TemplatesPickerControl )

/* Virtual Method Table (VMT) for the class : 'Templates::PickerControl' */
EW_DEFINE_CLASS( TemplatesPickerControl, CoreGroup, _None, _None, _None, _None, 
                 _None, _None, "Templates::PickerControl" )
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
  CoreGroup_drawContent,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_RestackTop,
  CoreGroup_Remove,
  CoreGroup_Add,
EW_END_OF_CLASS( TemplatesPickerControl )

/* Embedded Wizard */
