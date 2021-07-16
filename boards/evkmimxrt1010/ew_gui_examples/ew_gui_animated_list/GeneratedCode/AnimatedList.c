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
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#include "ewlocale.h"
#include "_AnimatedListAnimatedList.h"
#include "_AnimatedListDayItemL.h"
#include "_AnimatedListDayItemS.h"
#include "_AnimatedListDayList.h"
#include "_AnimatedListDeviceClass.h"
#include "_AnimatedListHourItemL.h"
#include "_AnimatedListHourItemS.h"
#include "_AnimatedListMinItemL.h"
#include "_AnimatedListMinItemS.h"
#include "_AnimatedListTimeList.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreSlideTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreVerticalList.h"
#include "_CoreView.h"
#include "_EffectsInt32Effect.h"
#include "_ResourcesFont.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"
#include "AnimatedList.h"
#include "Core.h"
#include "Effects.h"
#include "Views.h"

/* Strings for the language 'Default'. */
static const unsigned short _StringsDefault0[] =
{
  0xFFFF, 0xFFFF, 0xC557, 0x0054, 0x0065, 0x0078, 0x0074, 0x0000, 0xC557, 0x0054,
  0x0069, 0x006D, 0x0065, 0x0000, 0xC557, 0x003A, 0x0000, 0xC557, 0x004D, 0x0075,
  0x006E, 0x0069, 0x0063, 0x0068, 0x002C, 0x0020, 0x0042, 0x0061, 0x0076, 0x0061,
  0x0072, 0x0069, 0x0061, 0x0000, 0xC557, 0x0030, 0x0030, 0x0000, 0xC557, 0x004D,
  0x006F, 0x006E, 0x0000, 0xC557, 0x0054, 0x0075, 0x0065, 0x0000, 0xC557, 0x0057,
  0x0065, 0x0064, 0x0000, 0xC557, 0x0054, 0x0068, 0x0075, 0x0000, 0xC557, 0x0046,
  0x0072, 0x0069, 0x0000, 0xC557, 0x0053, 0x0061, 0x0074, 0x0000, 0xC557, 0x0053,
  0x0075, 0x006E, 0x0000, 0xC557, 0x0044, 0x0061, 0x0079, 0x0000
};

/* Constant values used in this 'C' module only. */
static const XRect _Const0000 = {{ 0, 0 }, { 110, 40 }};
static const XRect _Const0001 = {{ 109, 0 }, { 110, 40 }};
static const XStringRes _Const0002 = { _StringsDefault0, 0x0003 };
static const XRect _Const0003 = {{ 0, 0 }, { 220, 240 }};
static const XRect _Const0004 = {{ 0, 0 }, { 110, 240 }};
static const XPoint _Const0005 = { 0, 160 };
static const XPoint _Const0006 = { 0, 239 };
static const XPoint _Const0007 = { 110, 239 };
static const XPoint _Const0008 = { 110, 0 };
static const XPoint _Const0009 = { 0, 0 };
static const XRect _Const000A = {{ 110, 0 }, { 220, 240 }};
static const XPoint _Const000B = { 219, 239 };
static const XPoint _Const000C = { 219, 0 };
static const XRect _Const000D = {{ 0, 40 }, { 220, 200 }};
static const XRect _Const000E = {{ 0, 200 }, { 110, 240 }};
static const XRect _Const000F = {{ 0, 40 }, { 110, 200 }};
static const XRect _Const0010 = {{ 110, 200 }, { 220, 240 }};
static const XRect _Const0011 = {{ 110, 40 }, { 220, 200 }};
static const XRect _Const0012 = {{ 110, 0 }, { 220, 40 }};
static const XRect _Const0013 = {{ 10, 40 }, { 90, 80 }};
static const XStringRes _Const0014 = { _StringsDefault0, 0x0009 };
static const XRect _Const0015 = {{ 100, 40 }, { 120, 195 }};
static const XStringRes _Const0016 = { _StringsDefault0, 0x000F };
static const XRect _Const0017 = {{ 10, 160 }, { 210, 200 }};
static const XStringRes _Const0018 = { _StringsDefault0, 0x0012 };
static const XRect _Const0019 = {{ 0, 0 }, { 110, 160 }};
static const XRect _Const001A = {{ 0, 0 }, { 100, 155 }};
static const XStringRes _Const001B = { _StringsDefault0, 0x0023 };
static const XRect _Const001C = {{ 10, 0 }, { 110, 155 }};
static const XRect _Const001D = {{ 0, 0 }, { 1, 40 }};
static const XStringRes _Const001E = { _StringsDefault0, 0x0027 };
static const XStringRes _Const001F = { _StringsDefault0, 0x002C };
static const XStringRes _Const0020 = { _StringsDefault0, 0x0031 };
static const XStringRes _Const0021 = { _StringsDefault0, 0x0036 };
static const XStringRes _Const0022 = { _StringsDefault0, 0x003B };
static const XStringRes _Const0023 = { _StringsDefault0, 0x0040 };
static const XStringRes _Const0024 = { _StringsDefault0, 0x0045 };
static const XRect _Const0025 = {{ 0, 0 }, { 100, 240 }};
static const XPoint _Const0026 = { 0, 240 };
static const XPoint _Const0027 = { 100, 240 };
static const XPoint _Const0028 = { 100, 0 };
static const XRect _Const0029 = {{ 0, 40 }, { 100, 200 }};
static const XRect _Const002A = {{ 0, 200 }, { 100, 240 }};
static const XRect _Const002B = {{ 0, 0 }, { 100, 40 }};
static const XRect _Const002C = {{ 10, 40 }, { 100, 80 }};
static const XStringRes _Const002D = { _StringsDefault0, 0x004A };
static const XRect _Const002E = {{ 0, 0 }, { 100, 160 }};
static const XRect _Const002F = {{ 20, 0 }, { 90, 160 }};
static const XRect _Const0030 = {{ 20, 0 }, { 100, 40 }};
static const XRect _Const0031 = {{ 0, 0 }, { 320, 240 }};
static const XColor _Const0032 = { 0x34, 0x3B, 0x56, 0xFF };
static const XRect _Const0033 = {{ 220, 0 }, { 320, 240 }};

#ifndef EW_DONT_CHECK_INDEX
  /* This function is used to check the indices when accessing an array.
     If you don't want this verification add the define EW_DONT_CHECK_INDEX
     to your Makefile or project settings. */
  static int EwCheckIndex( int aIndex, int aRange, const char* aFile, int aLine )
  {
    if (( aIndex < 0 ) || ( aIndex >= aRange ))
    {
      EwPrint( "[FATAL ERROR in %s:%d] Array index %d out of bounds %d",
                aFile, aLine, aIndex, aRange );
      EwPanic();
    }
    return aIndex;
  }

  #define EwCheckIndex( aIndex, aRange ) \
    EwCheckIndex( aIndex, aRange, __FILE__, __LINE__ )
#else
  #define EwCheckIndex( aIndex, aRange ) aIndex
#endif

/* Include a file containing the font resource : 'AnimatedList::ListFontS' */
#include "_AnimatedListListFontS.h"

/* Table with links to derived variants of the font resource : 'AnimatedList::ListFontS' */
EW_RES_WITHOUT_VARIANTS( AnimatedListListFontS )

/* Include a file containing the font resource : 'AnimatedList::DescrFont' */
#include "_AnimatedListDescrFont.h"

/* Table with links to derived variants of the font resource : 'AnimatedList::DescrFont' */
EW_RES_WITHOUT_VARIANTS( AnimatedListDescrFont )

/* Include a file containing the font resource : 'AnimatedList::ListFontM' */
#include "_AnimatedListListFontM.h"

/* Table with links to derived variants of the font resource : 'AnimatedList::ListFontM' */
EW_RES_WITHOUT_VARIANTS( AnimatedListListFontM )

/* Color constant according UI design. */
const XColor AnimatedListButtonBgColor = { 0xE7, 0xEA, 0xEA, 0xFF };

/* Color constant according UI design. */
const XColor AnimatedListCaptionColor = { 0x34, 0x3B, 0x56, 0xFF };

/* Color constant according UI design. */
const XColor AnimatedListButtonTextColor = { 0x69, 0xD4, 0xB8, 0xFF };

/* Color constant according UI design. */
const XColor AnimatedListListBgColor = { 0xFF, 0xFF, 0xFF, 0x40 };

/* Color constant according UI design. */
const XColor AnimatedListListTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

/* Include a file containing the font resource : 'AnimatedList::ListFontL' */
#include "_AnimatedListListFontL.h"

/* Table with links to derived variants of the font resource : 'AnimatedList::ListFontL' */
EW_RES_WITHOUT_VARIANTS( AnimatedListListFontL )

/* Initializer for the class 'AnimatedList::HourItemS' */
void AnimatedListHourItemS__Init( AnimatedListHourItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListHourItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Right, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListHourItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0000 );
  CoreView_OnSetLayout((CoreView)&_this->Background, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Background, _Const0000 );
  ViewsRectangle_OnSetColor( &_this->Background, AnimatedListListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Right, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Right, _Const0001 );
  ViewsRectangle_OnSetColor( &_this->Right, AnimatedListButtonLineColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0000 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0002 ));
  ViewsText_OnSetColor( &_this->Text, AnimatedListListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Background ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Right ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &AnimatedListListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'AnimatedList::HourItemS' */
void AnimatedListHourItemS__ReInit( AnimatedListHourItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Background );
  ViewsRectangle__ReInit( &_this->Right );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'AnimatedList::HourItemS' */
void AnimatedListHourItemS__Done( AnimatedListHourItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Background );
  ViewsRectangle__Done( &_this->Right );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'AnimatedList::HourItemS.OnSetCaption()' */
void AnimatedListHourItemS_OnSetCaption( AnimatedListHourItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'AnimatedList::HourItemS' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListHourItemS )
EW_END_OF_CLASS_VARIANTS( AnimatedListHourItemS )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::HourItemS' */
EW_DEFINE_CLASS( AnimatedListHourItemS, CoreGroup, Background, Background, Background, 
                 Background, Caption, _None, "AnimatedList::HourItemS" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListHourItemS )

/* Initializer for the class 'AnimatedList::TimeList' */
void AnimatedListTimeList__Init( AnimatedListTimeList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListTimeList );

  /* ... then construct all embedded objects */
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandlerH, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerH, &_this->_XObject, 0 );
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandlerM, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerM, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomListH, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainListH, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopListH, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomListM, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainListM, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopListM, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );
  ViewsText__Init( &_this->DotText, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TimezoneText, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListIn, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeCaption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeDuration, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListOut, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32EffectH, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32EffectM, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AutoDemoEffectH, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AutoDemoEffectM, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListTimeList );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0003 );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandlerH, _Const0004 );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandlerH, _Const0005 );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandlerH, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandlerH, _Const0006 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandlerH, _Const0007 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandlerH, _Const0008 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandlerH, _Const0009 );
  _this->SimpleTouchHandlerH.RetargetCondition = CoreRetargetReasonWipeDown | CoreRetargetReasonWipeUp;
  CoreRectView__OnSetBounds( &_this->SlideTouchHandlerM, _Const000A );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandlerM, _Const0005 );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandlerM, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandlerM, _Const0007 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandlerM, _Const000B );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandlerM, _Const000C );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandlerM, _Const0008 );
  _this->SimpleTouchHandlerM.RetargetCondition = CoreRetargetReasonWipeDown | CoreRetargetReasonWipeUp;
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const000D );
  ViewsRectangle_OnSetColor( &_this->Rectangle, AnimatedListButtonBgColor );
  CoreRectView__OnSetBounds( &_this->BottomListH, _Const000E );
  CoreGroup__OnSetOpacity( &_this->BottomListH, 0 );
  CoreVerticalList_OnSetEndless( &_this->BottomListH, 1 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomListH, 40 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomListH, 24 );
  CoreVerticalList_OnSetItemClass( &_this->BottomListH, EW_CLASS( AnimatedListHourItemS ));
  CoreRectView__OnSetBounds( &_this->MainListH, _Const000F );
  CoreVerticalList_OnSetEndless( &_this->MainListH, 1 );
  CoreVerticalList_OnSetItemHeight( &_this->MainListH, 160 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainListH, 24 );
  CoreVerticalList_OnSetItemClass( &_this->MainListH, EW_CLASS( AnimatedListHourItemL ));
  CoreRectView__OnSetBounds( &_this->TopListH, _Const0000 );
  CoreGroup__OnSetOpacity( &_this->TopListH, 0 );
  CoreVerticalList_OnSetEndless( &_this->TopListH, 1 );
  CoreVerticalList_OnSetItemHeight( &_this->TopListH, 40 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopListH, 24 );
  CoreVerticalList_OnSetItemClass( &_this->TopListH, EW_CLASS( AnimatedListHourItemS ));
  CoreRectView__OnSetBounds( &_this->BottomListM, _Const0010 );
  CoreGroup__OnSetOpacity( &_this->BottomListM, 0 );
  CoreVerticalList_OnSetEndless( &_this->BottomListM, 1 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomListM, 40 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomListM, 60 );
  CoreVerticalList_OnSetItemClass( &_this->BottomListM, EW_CLASS( AnimatedListMinItemS ));
  CoreRectView__OnSetBounds( &_this->MainListM, _Const0011 );
  CoreVerticalList_OnSetEndless( &_this->MainListM, 1 );
  CoreVerticalList_OnSetItemHeight( &_this->MainListM, 160 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainListM, 60 );
  CoreVerticalList_OnSetItemClass( &_this->MainListM, EW_CLASS( AnimatedListMinItemL ));
  CoreRectView__OnSetBounds( &_this->TopListM, _Const0012 );
  CoreGroup__OnSetOpacity( &_this->TopListM, 0 );
  CoreVerticalList_OnSetEndless( &_this->TopListM, 1 );
  CoreVerticalList_OnSetItemHeight( &_this->TopListM, 40 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopListM, 60 );
  CoreVerticalList_OnSetItemClass( &_this->TopListM, EW_CLASS( AnimatedListMinItemS ));
  CoreRectView__OnSetBounds( &_this->Caption, _Const0013 );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const0014 ));
  ViewsText_OnSetColor( &_this->Caption, AnimatedListCaptionColor );
  CoreView_OnSetLayout((CoreView)&_this->DotText, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->DotText, _Const0015 );
  ViewsText_OnSetWrapText( &_this->DotText, 1 );
  ViewsText_OnSetAlignment( &_this->DotText, ViewsTextAlignmentAlignHorzCenter | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->DotText, EwLoadString( &_Const0016 ));
  ViewsText_OnSetColor( &_this->DotText, AnimatedListButtonTextColor );
  CoreRectView__OnSetBounds( &_this->TimezoneText, _Const0017 );
  ViewsText_OnSetAlignment( &_this->TimezoneText, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TimezoneText, EwLoadString( &_Const0018 ));
  ViewsText_OnSetColor( &_this->TimezoneText, AnimatedListCaptionColor );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListIn, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListIn, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListIn, 300 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeCaption, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeCaption, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeDuration, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeDuration, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeDuration, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListOut, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListOut, 1000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->FadeListOut, 2000 );
  _this->FadeListOut.Value2 = 0;
  _this->FadeListOut.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32EffectH, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32EffectH, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32EffectH, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32EffectM, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32EffectM, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32EffectM, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->AutoDemoEffectH, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AutoDemoEffectH, 300 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->AutoDemoEffectM, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AutoDemoEffectM, 600 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandlerH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandlerH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandlerM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandlerM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomListH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainListH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopListH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomListM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainListM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopListM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Caption ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->DotText ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TimezoneText ), 0 );
  _this->SlideTouchHandlerH.OnEnd = EwNewSlot( _this, AnimatedListTimeList_OnEnd );
  _this->SlideTouchHandlerH.OnStart = EwNewSlot( _this, AnimatedListTimeList_OnStart );
  _this->SimpleTouchHandlerH.OnRelease = EwNewSlot( _this, AnimatedListTimeList_OnReleaseH );
  _this->SimpleTouchHandlerH.OnPress = EwNewSlot( _this, AnimatedListTimeList_OnStart );
  _this->SlideTouchHandlerM.OnEnd = EwNewSlot( _this, AnimatedListTimeList_OnEnd );
  _this->SlideTouchHandlerM.OnStart = EwNewSlot( _this, AnimatedListTimeList_OnStart );
  _this->SimpleTouchHandlerM.OnRelease = EwNewSlot( _this, AnimatedListTimeList_OnReleaseM );
  _this->SimpleTouchHandlerM.OnPress = EwNewSlot( _this, AnimatedListTimeList_OnStart );
  _this->BottomListH.OnLoadItem = EwNewSlot( _this, AnimatedListTimeList_OnLoadBottomH );
  _this->MainListH.OnUpdate = EwNewSlot( _this, AnimatedListTimeList_OnUpdateH );
  _this->MainListH.OnLoadItem = EwNewSlot( _this, AnimatedListTimeList_OnLoadMainH );
  CoreVerticalList_OnSetSlideHandler( &_this->MainListH, &_this->SlideTouchHandlerH );
  _this->TopListH.OnLoadItem = EwNewSlot( _this, AnimatedListTimeList_OnLoadTopH );
  _this->BottomListM.OnLoadItem = EwNewSlot( _this, AnimatedListTimeList_OnLoadBottomM );
  _this->MainListM.OnUpdate = EwNewSlot( _this, AnimatedListTimeList_OnUpdateM );
  _this->MainListM.OnLoadItem = EwNewSlot( _this, AnimatedListTimeList_OnLoadMainM );
  CoreVerticalList_OnSetSlideHandler( &_this->MainListM, &_this->SlideTouchHandlerM );
  _this->TopListM.OnLoadItem = EwNewSlot( _this, AnimatedListTimeList_OnLoadTopM );
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &AnimatedListDescrFont, 
  ResourcesFont ));
  ViewsText_OnSetFont( &_this->DotText, EwLoadResource( &AnimatedListListFontL, 
  ResourcesFont ));
  ViewsText_OnSetFont( &_this->TimezoneText, EwLoadResource( &AnimatedListDescrFont, 
  ResourcesFont ));
  _this->FadeListIn.Super1.OnAnimate = EwNewSlot( _this, AnimatedListTimeList_onAnimate );
  _this->FadeListIn.Outlet = EwNewRef( &_this->TopListH, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  _this->FadeCaption.Outlet = EwNewRef( &_this->Caption, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  _this->FadeDuration.Outlet = EwNewRef( &_this->TimezoneText, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  _this->FadeListOut.Super1.OnFinished = EwNewSlot( _this, AnimatedListTimeList_onFinished );
  _this->FadeListOut.Super1.OnAnimate = EwNewSlot( _this, AnimatedListTimeList_onAnimate );
  _this->FadeListOut.Outlet = EwNewRef( &_this->TopListH, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
}

/* Re-Initializer for the class 'AnimatedList::TimeList' */
void AnimatedListTimeList__ReInit( AnimatedListTimeList _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandlerH );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerH );
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandlerM );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerM );
  ViewsRectangle__ReInit( &_this->Rectangle );
  CoreVerticalList__ReInit( &_this->BottomListH );
  CoreVerticalList__ReInit( &_this->MainListH );
  CoreVerticalList__ReInit( &_this->TopListH );
  CoreVerticalList__ReInit( &_this->BottomListM );
  CoreVerticalList__ReInit( &_this->MainListM );
  CoreVerticalList__ReInit( &_this->TopListM );
  ViewsText__ReInit( &_this->Caption );
  ViewsText__ReInit( &_this->DotText );
  ViewsText__ReInit( &_this->TimezoneText );
  EffectsInt32Effect__ReInit( &_this->FadeListIn );
  EffectsInt32Effect__ReInit( &_this->FadeCaption );
  EffectsInt32Effect__ReInit( &_this->FadeDuration );
  EffectsInt32Effect__ReInit( &_this->FadeListOut );
  EffectsInt32Effect__ReInit( &_this->Int32EffectH );
  EffectsInt32Effect__ReInit( &_this->Int32EffectM );
  EffectsInt32Effect__ReInit( &_this->AutoDemoEffectH );
  EffectsInt32Effect__ReInit( &_this->AutoDemoEffectM );
}

/* Finalizer method for the class 'AnimatedList::TimeList' */
void AnimatedListTimeList__Done( AnimatedListTimeList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandlerH );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerH );
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandlerM );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerM );
  ViewsRectangle__Done( &_this->Rectangle );
  CoreVerticalList__Done( &_this->BottomListH );
  CoreVerticalList__Done( &_this->MainListH );
  CoreVerticalList__Done( &_this->TopListH );
  CoreVerticalList__Done( &_this->BottomListM );
  CoreVerticalList__Done( &_this->MainListM );
  CoreVerticalList__Done( &_this->TopListM );
  ViewsText__Done( &_this->Caption );
  ViewsText__Done( &_this->DotText );
  ViewsText__Done( &_this->TimezoneText );
  EffectsInt32Effect__Done( &_this->FadeListIn );
  EffectsInt32Effect__Done( &_this->FadeCaption );
  EffectsInt32Effect__Done( &_this->FadeDuration );
  EffectsInt32Effect__Done( &_this->FadeListOut );
  EffectsInt32Effect__Done( &_this->Int32EffectH );
  EffectsInt32Effect__Done( &_this->Int32EffectM );
  EffectsInt32Effect__Done( &_this->AutoDemoEffectH );
  EffectsInt32Effect__Done( &_this->AutoDemoEffectM );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method to load content of main scroll list. */
void AnimatedListTimeList_OnLoadMainH( AnimatedListTimeList _this, XObject sender )
{
  AnimatedListHourItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainListH.View, AnimatedListHourItemL );
  itemNo = _this->MainListH.Item;
  caption = EwNewStringInt( itemNo, 0, 10 );
  AnimatedListHourItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainListH.Super2.Bounds ), _this->MainListH.ItemHeight )));
}

/* Slot method to load content of upper scroll list. */
void AnimatedListTimeList_OnLoadTopH( AnimatedListTimeList _this, XObject sender )
{
  AnimatedListHourItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopListH.View, AnimatedListHourItemS );
  itemNo = _this->TopListH.Item;
  caption = EwNewStringInt( itemNo, 0, 10 );
  AnimatedListHourItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopListH.Super2.Bounds ), _this->TopListH.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void AnimatedListTimeList_OnUpdateH( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopListH, (( _this->MainListH.ScrollOffset 
  * _this->TopListH.ItemHeight ) / _this->MainListH.ItemHeight ) + EwGetRectH( _this->TopListH.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomListH, (( _this->MainListH.ScrollOffset 
  * _this->BottomListH.ItemHeight ) / _this->MainListH.ItemHeight ) - _this->BottomListH.ItemHeight );
}

/* Slot method to load content of lower scroll list. */
void AnimatedListTimeList_OnLoadBottomH( AnimatedListTimeList _this, XObject sender )
{
  AnimatedListHourItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomListH.View, AnimatedListHourItemS );
  itemNo = _this->BottomListH.Item;
  caption = EwNewStringInt( itemNo, 0, 10 );
  AnimatedListHourItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomListH.Super2.Bounds ), _this->BottomListH.ItemHeight )));
}

/* Slot method to change the selection, when the item has reached its parking position. */
void AnimatedListTimeList_OnEnd( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  AnimatedListDeviceClass_OnSetHour( EwGetAutoObject( &AnimatedListDevice, AnimatedListDeviceClass ), 
  -_this->MainListH.ScrollOffset / _this->MainListH.ItemHeight );
  AnimatedListDeviceClass_OnSetMinute( EwGetAutoObject( &AnimatedListDevice, AnimatedListDeviceClass ), 
  -_this->MainListM.ScrollOffset / _this->MainListM.ItemHeight );
  _this->FadeListOut.Value1 = _this->TopListH.Super1.Opacity;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 255;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
}

/* Slot method to make the upper/lower list visible. */
void AnimatedListTimeList_OnStart( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->TopListH.Super1.Opacity != 255 ) && !_this->FadeListIn.Super1.Enabled )
  {
    _this->FadeListIn.Value1 = _this->TopListH.Super1.Opacity;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListIn, 1 );
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeDuration, 0 );
  _this->FadeDuration.Value1 = _this->TimezoneText.Opacity;
  _this->FadeDuration.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeDuration, 1 );

  if ( _this->TopListH.Super1.Opacity == 0 )
    EwPostSignal( _this->OnAppear, ((XObject)_this ));
}

/* This is a slot method. */
void AnimatedListTimeList_onAnimate( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->TopListM, _this->TopListH.Super1.Opacity );
  CoreGroup__OnSetOpacity( &_this->BottomListH, _this->TopListH.Super1.Opacity );
  CoreGroup__OnSetOpacity( &_this->BottomListM, _this->TopListH.Super1.Opacity );
}

/* Slot method to load content of main scroll list. */
void AnimatedListTimeList_OnLoadMainM( AnimatedListTimeList _this, XObject sender )
{
  AnimatedListMinItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainListM.View, AnimatedListMinItemL );
  itemNo = _this->MainListM.Item;
  caption = EwNewStringInt( itemNo, 2, 10 );
  AnimatedListMinItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainListM.Super2.Bounds ), _this->MainListM.ItemHeight )));
}

/* Slot method to load content of upper scroll list. */
void AnimatedListTimeList_OnLoadTopM( AnimatedListTimeList _this, XObject sender )
{
  AnimatedListMinItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopListM.View, AnimatedListMinItemS );
  itemNo = _this->TopListM.Item;
  caption = EwNewStringInt( itemNo, 2, 10 );
  AnimatedListMinItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopListM.Super2.Bounds ), _this->TopListM.ItemHeight )));
}

/* Slot method to load content of lower scroll list. */
void AnimatedListTimeList_OnLoadBottomM( AnimatedListTimeList _this, XObject sender )
{
  AnimatedListMinItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomListM.View, AnimatedListMinItemS );
  itemNo = _this->BottomListM.Item;
  caption = EwNewStringInt( itemNo, 2, 10 );
  AnimatedListMinItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomListM.Super2.Bounds ), _this->BottomListM.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void AnimatedListTimeList_OnUpdateM( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopListM, (( _this->MainListM.ScrollOffset 
  * _this->TopListM.ItemHeight ) / _this->MainListM.ItemHeight ) + EwGetRectH( _this->TopListM.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomListM, (( _this->MainListM.ScrollOffset 
  * _this->BottomListM.ItemHeight ) / _this->MainListM.ItemHeight ) - _this->BottomListM.ItemHeight );
}

/* This is a slot method. */
void AnimatedListTimeList_onFinished( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->FadeListOut.Value == 0 )
  {
    EwPostSignal( _this->OnDisappear, ((XObject)_this ));
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeDuration, 0 );
    _this->FadeDuration.Value1 = _this->Caption.Opacity;
    _this->FadeDuration.Value2 = 255;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeDuration, 1 );
  }
}

/* 'C' function for method : 'AnimatedList::TimeList.OnReleaseH()' */
void AnimatedListTimeList_OnReleaseH( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandlerH.AutoDeflected && ( _this->TopListH.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandlerH.CurrentPos.Y < _this->MainListH.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandlerH.CurrentPos.Y - _this->MainListH.Super2.Bounds.Point1.Y ) 
      / _this->TopListH.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandlerH.CurrentPos.Y > _this->MainListH.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandlerH.CurrentPos.Y - _this->MainListH.Super2.Bounds.Point2.Y ) 
        / _this->BottomListH.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainListH.ScrollOffset / _this->MainListH.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainListH, item, 1, &_this->Int32EffectH, 
      EwNullSlot );
    }
  }

  EwSignal( EwNewSlot( _this, AnimatedListTimeList_OnEnd ), ((XObject)_this ));
}

/* 'C' function for method : 'AnimatedList::TimeList.OnReleaseM()' */
void AnimatedListTimeList_OnReleaseM( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandlerM.AutoDeflected && ( _this->TopListM.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandlerM.CurrentPos.Y < _this->MainListM.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandlerM.CurrentPos.Y - _this->MainListM.Super2.Bounds.Point1.Y ) 
      / _this->TopListM.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandlerM.CurrentPos.Y > _this->MainListM.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandlerM.CurrentPos.Y - _this->MainListM.Super2.Bounds.Point2.Y ) 
        / _this->BottomListM.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainListM.ScrollOffset / _this->MainListM.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainListM, item, 1, &_this->Int32EffectM, 
      EwNullSlot );
    }
  }

  EwSignal( EwNewSlot( _this, AnimatedListTimeList_OnEnd ), ((XObject)_this ));
}

/* Slot method to trigger the next phase of the auto demo. */
void AnimatedListTimeList_AutoDemo( AnimatedListTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( EwNewSlot( _this, AnimatedListTimeList_OnStart ), ((XObject)_this ));
  CoreVerticalList_EnsureVisible( &_this->MainListH, EwMathRandInt32( 0, 12 ), 1, 
  &_this->AutoDemoEffectH, EwNullSlot );
  CoreVerticalList_EnsureVisible( &_this->MainListM, EwMathRandInt32( 0, 59 ), 1, 
  &_this->AutoDemoEffectM, EwNewSlot( _this, AnimatedListTimeList_OnEnd ));
}

/* Variants derived from the class : 'AnimatedList::TimeList' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListTimeList )
EW_END_OF_CLASS_VARIANTS( AnimatedListTimeList )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::TimeList' */
EW_DEFINE_CLASS( AnimatedListTimeList, CoreGroup, OnAppear, OnAppear, SlideTouchHandlerH, 
                 SlideTouchHandlerH, _None, _None, "AnimatedList::TimeList" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListTimeList )

/* Initializer for the class 'AnimatedList::HourItemL' */
void AnimatedListHourItemL__Init( AnimatedListHourItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListHourItemL );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListHourItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0019 );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const001A );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzRight | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const001B ));
  ViewsText_OnSetColor( &_this->Text, AnimatedListButtonTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &AnimatedListListFontL, ResourcesFont ));
}

/* Re-Initializer for the class 'AnimatedList::HourItemL' */
void AnimatedListHourItemL__ReInit( AnimatedListHourItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'AnimatedList::HourItemL' */
void AnimatedListHourItemL__Done( AnimatedListHourItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'AnimatedList::HourItemL.OnSetCaption()' */
void AnimatedListHourItemL_OnSetCaption( AnimatedListHourItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'AnimatedList::HourItemL' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListHourItemL )
EW_END_OF_CLASS_VARIANTS( AnimatedListHourItemL )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::HourItemL' */
EW_DEFINE_CLASS( AnimatedListHourItemL, CoreGroup, Text, Text, Text, Text, Caption, 
                 _None, "AnimatedList::HourItemL" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListHourItemL )

/* Initializer for the class 'AnimatedList::MinItemL' */
void AnimatedListMinItemL__Init( AnimatedListMinItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListMinItemL );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListMinItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0019 );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const001C );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const001B ));
  ViewsText_OnSetColor( &_this->Text, AnimatedListButtonTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &AnimatedListListFontL, ResourcesFont ));
}

/* Re-Initializer for the class 'AnimatedList::MinItemL' */
void AnimatedListMinItemL__ReInit( AnimatedListMinItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'AnimatedList::MinItemL' */
void AnimatedListMinItemL__Done( AnimatedListMinItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'AnimatedList::MinItemL.OnSetCaption()' */
void AnimatedListMinItemL_OnSetCaption( AnimatedListMinItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'AnimatedList::MinItemL' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListMinItemL )
EW_END_OF_CLASS_VARIANTS( AnimatedListMinItemL )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::MinItemL' */
EW_DEFINE_CLASS( AnimatedListMinItemL, CoreGroup, Text, Text, Text, Text, Caption, 
                 _None, "AnimatedList::MinItemL" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListMinItemL )

/* Initializer for the class 'AnimatedList::MinItemS' */
void AnimatedListMinItemS__Init( AnimatedListMinItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListMinItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Left, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Right, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListMinItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0000 );
  CoreView_OnSetLayout((CoreView)&_this->Background, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Background, _Const0000 );
  ViewsRectangle_OnSetColor( &_this->Background, AnimatedListListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Left, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Left, _Const001D );
  ViewsRectangle_OnSetColor( &_this->Left, AnimatedListButtonLineColor );
  CoreView_OnSetLayout((CoreView)&_this->Right, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Right, _Const0001 );
  ViewsRectangle_OnSetColor( &_this->Right, AnimatedListButtonLineColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0000 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0002 ));
  ViewsText_OnSetColor( &_this->Text, AnimatedListListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Background ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Left ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Right ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &AnimatedListListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'AnimatedList::MinItemS' */
void AnimatedListMinItemS__ReInit( AnimatedListMinItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Background );
  ViewsRectangle__ReInit( &_this->Left );
  ViewsRectangle__ReInit( &_this->Right );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'AnimatedList::MinItemS' */
void AnimatedListMinItemS__Done( AnimatedListMinItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Background );
  ViewsRectangle__Done( &_this->Left );
  ViewsRectangle__Done( &_this->Right );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'AnimatedList::MinItemS.OnSetCaption()' */
void AnimatedListMinItemS_OnSetCaption( AnimatedListMinItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'AnimatedList::MinItemS' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListMinItemS )
EW_END_OF_CLASS_VARIANTS( AnimatedListMinItemS )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::MinItemS' */
EW_DEFINE_CLASS( AnimatedListMinItemS, CoreGroup, Background, Background, Background, 
                 Background, Caption, _None, "AnimatedList::MinItemS" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListMinItemS )

/* Initializer for the class 'AnimatedList::DeviceClass' */
void AnimatedListDeviceClass__Init( AnimatedListDeviceClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListDeviceClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListDeviceClass );

  /* ... and initialize objects, variables, properties, etc. */
  _this->WeekDays[ 0 ] = EwShareString( EwLoadString( &_Const001E ));
  _this->WeekDays[ 1 ] = EwShareString( EwLoadString( &_Const001F ));
  _this->WeekDays[ 2 ] = EwShareString( EwLoadString( &_Const0020 ));
  _this->WeekDays[ 3 ] = EwShareString( EwLoadString( &_Const0021 ));
  _this->WeekDays[ 4 ] = EwShareString( EwLoadString( &_Const0022 ));
  _this->WeekDays[ 5 ] = EwShareString( EwLoadString( &_Const0023 ));
  _this->WeekDays[ 6 ] = EwShareString( EwLoadString( &_Const0024 ));
}

/* Re-Initializer for the class 'AnimatedList::DeviceClass' */
void AnimatedListDeviceClass__ReInit( AnimatedListDeviceClass _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'AnimatedList::DeviceClass' */
void AnimatedListDeviceClass__Done( AnimatedListDeviceClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'AnimatedList::DeviceClass.OnSetHour()' */
void AnimatedListDeviceClass_OnSetHour( AnimatedListDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 23 )
    value = 23;

  if ( value == _this->Hour )
    return;

  _this->Hour = value;
  EwNotifyRefObservers( EwNewRef( _this, AnimatedListDeviceClass_OnGetHour, AnimatedListDeviceClass_OnSetHour ), 
    0 );
}

/* 'C' function for method : 'AnimatedList::DeviceClass.OnSetMinute()' */
void AnimatedListDeviceClass_OnSetMinute( AnimatedListDeviceClass _this, XInt32 
  value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 59 )
    value = 59;

  if ( value == _this->Minute )
    return;

  _this->Minute = value;
  EwNotifyRefObservers( EwNewRef( _this, AnimatedListDeviceClass_OnGetMinute, AnimatedListDeviceClass_OnSetMinute ), 
    0 );
}

/* 'C' function for method : 'AnimatedList::DeviceClass.OnSetWeekDay()' */
void AnimatedListDeviceClass_OnSetWeekDay( AnimatedListDeviceClass _this, XInt32 
  value )
{
  if ( value < 0 )
    value = 0;

  if ( value >= 7 )
    value = 6;

  if ( value == _this->WeekDay )
    return;

  _this->WeekDay = value;
  EwNotifyRefObservers( EwNewRef( _this, AnimatedListDeviceClass_OnGetWeekDay, AnimatedListDeviceClass_OnSetWeekDay ), 
    0 );
}

/* Default onget method for the property 'Hour' */
XInt32 AnimatedListDeviceClass_OnGetHour( AnimatedListDeviceClass _this )
{
  return _this->Hour;
}

/* Default onget method for the property 'Minute' */
XInt32 AnimatedListDeviceClass_OnGetMinute( AnimatedListDeviceClass _this )
{
  return _this->Minute;
}

/* Default onget method for the property 'WeekDay' */
XInt32 AnimatedListDeviceClass_OnGetWeekDay( AnimatedListDeviceClass _this )
{
  return _this->WeekDay;
}

/* Variants derived from the class : 'AnimatedList::DeviceClass' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListDeviceClass )
EW_END_OF_CLASS_VARIANTS( AnimatedListDeviceClass )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::DeviceClass' */
EW_DEFINE_CLASS( AnimatedListDeviceClass, XObject, WeekDays, WeekDays, WeekDays, 
                 WeekDays, WeekDays, Hour, "AnimatedList::DeviceClass" )
EW_END_OF_CLASS( AnimatedListDeviceClass )

/* User defined auto object: 'AnimatedList::Device' */
EW_DEFINE_AUTOOBJECT( AnimatedListDevice, AnimatedListDeviceClass )

/* Initializer for the auto object 'AnimatedList::Device' */
void AnimatedListDevice__Init( AnimatedListDeviceClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'AnimatedList::Device' */
EW_DEFINE_AUTOOBJECT_VARIANTS( AnimatedListDevice )
EW_END_OF_AUTOOBJECT_VARIANTS( AnimatedListDevice )

/* Initializer for the class 'AnimatedList::DayList' */
void AnimatedListDayList__Init( AnimatedListDayList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListDayList );

  /* ... then construct all embedded objects */
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopList, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListIn, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeCaption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListOut, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32Effect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AutoDemoEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListDayList );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0025 );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandler, _Const0025 );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandler, _Const0005 );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandler, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const0026 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const0027 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const0028 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const0009 );
  _this->SimpleTouchHandler.RetargetCondition = CoreRetargetReasonWipeDown | CoreRetargetReasonWipeUp;
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0029 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, AnimatedListButtonBgColor );
  CoreRectView__OnSetBounds( &_this->BottomList, _Const002A );
  CoreGroup__OnSetOpacity( &_this->BottomList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomList, 40 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomList, 7 );
  CoreVerticalList_OnSetItemClass( &_this->BottomList, EW_CLASS( AnimatedListDayItemS ));
  CoreRectView__OnSetBounds( &_this->MainList, _Const0029 );
  CoreVerticalList_OnSetItemHeight( &_this->MainList, 160 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainList, 7 );
  CoreVerticalList_OnSetItemClass( &_this->MainList, EW_CLASS( AnimatedListDayItemL ));
  CoreRectView__OnSetBounds( &_this->TopList, _Const002B );
  CoreGroup__OnSetOpacity( &_this->TopList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->TopList, 40 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopList, 7 );
  CoreVerticalList_OnSetItemClass( &_this->TopList, EW_CLASS( AnimatedListDayItemS ));
  CoreRectView__OnSetBounds( &_this->Caption, _Const002C );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const002D ));
  ViewsText_OnSetColor( &_this->Caption, AnimatedListCaptionColor );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListIn, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListIn, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListIn, 300 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeCaption, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeCaption, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListOut, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListOut, 1000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->FadeListOut, 2000 );
  _this->FadeListOut.Value2 = 0;
  _this->FadeListOut.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32Effect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32Effect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32Effect, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->AutoDemoEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AutoDemoEffect, 300 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Caption ), 0 );
  _this->SlideTouchHandler.OnEnd = EwNewSlot( _this, AnimatedListDayList_OnEnd );
  _this->SlideTouchHandler.OnStart = EwNewSlot( _this, AnimatedListDayList_OnStart );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, AnimatedListDayList_OnRelease );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, AnimatedListDayList_OnStart );
  _this->BottomList.OnLoadItem = EwNewSlot( _this, AnimatedListDayList_OnLoadBottom );
  _this->MainList.OnUpdate = EwNewSlot( _this, AnimatedListDayList_OnUpdate );
  _this->MainList.OnLoadItem = EwNewSlot( _this, AnimatedListDayList_OnLoadMain );
  CoreVerticalList_OnSetSlideHandler( &_this->MainList, &_this->SlideTouchHandler );
  _this->TopList.OnLoadItem = EwNewSlot( _this, AnimatedListDayList_OnLoadTop );
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &AnimatedListDescrFont, 
  ResourcesFont ));
  _this->FadeListIn.Super1.OnAnimate = EwNewSlot( _this, AnimatedListDayList_onAnimate );
  _this->FadeListIn.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  _this->FadeCaption.Outlet = EwNewRef( &_this->Caption, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  _this->FadeListOut.Super1.OnAnimate = EwNewSlot( _this, AnimatedListDayList_onAnimate );
  _this->FadeListOut.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
}

/* Re-Initializer for the class 'AnimatedList::DayList' */
void AnimatedListDayList__ReInit( AnimatedListDayList _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  CoreVerticalList__ReInit( &_this->BottomList );
  CoreVerticalList__ReInit( &_this->MainList );
  CoreVerticalList__ReInit( &_this->TopList );
  ViewsText__ReInit( &_this->Caption );
  EffectsInt32Effect__ReInit( &_this->FadeListIn );
  EffectsInt32Effect__ReInit( &_this->FadeCaption );
  EffectsInt32Effect__ReInit( &_this->FadeListOut );
  EffectsInt32Effect__ReInit( &_this->Int32Effect );
  EffectsInt32Effect__ReInit( &_this->AutoDemoEffect );
}

/* Finalizer method for the class 'AnimatedList::DayList' */
void AnimatedListDayList__Done( AnimatedListDayList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  CoreVerticalList__Done( &_this->BottomList );
  CoreVerticalList__Done( &_this->MainList );
  CoreVerticalList__Done( &_this->TopList );
  ViewsText__Done( &_this->Caption );
  EffectsInt32Effect__Done( &_this->FadeListIn );
  EffectsInt32Effect__Done( &_this->FadeCaption );
  EffectsInt32Effect__Done( &_this->FadeListOut );
  EffectsInt32Effect__Done( &_this->Int32Effect );
  EffectsInt32Effect__Done( &_this->AutoDemoEffect );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method to load content of main scroll list. */
void AnimatedListDayList_OnLoadMain( AnimatedListDayList _this, XObject sender )
{
  AnimatedListDayItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainList.View, AnimatedListDayItemL );
  itemNo = _this->MainList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 7 ))
    caption = EwGetAutoObject( &AnimatedListDevice, AnimatedListDeviceClass )->WeekDays[ 
    EwCheckIndex( itemNo, 7 )];

  AnimatedListDayItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainList.Super2.Bounds ), _this->MainList.ItemHeight )));
}

/* Slot method to load content of upper scroll list. */
void AnimatedListDayList_OnLoadTop( AnimatedListDayList _this, XObject sender )
{
  AnimatedListDayItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopList.View, AnimatedListDayItemS );
  itemNo = _this->TopList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 7 ))
    caption = EwGetAutoObject( &AnimatedListDevice, AnimatedListDeviceClass )->WeekDays[ 
    EwCheckIndex( itemNo, 7 )];

  AnimatedListDayItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopList.Super2.Bounds ), _this->TopList.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void AnimatedListDayList_OnUpdate( AnimatedListDayList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopList, (( _this->MainList.ScrollOffset 
  * _this->TopList.ItemHeight ) / _this->MainList.ItemHeight ) + EwGetRectH( _this->TopList.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomList, (( _this->MainList.ScrollOffset 
  * _this->BottomList.ItemHeight ) / _this->MainList.ItemHeight ) - _this->BottomList.ItemHeight );
}

/* Slot method to load content of lower scroll list. */
void AnimatedListDayList_OnLoadBottom( AnimatedListDayList _this, XObject sender )
{
  AnimatedListDayItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomList.View, AnimatedListDayItemS );
  itemNo = _this->BottomList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 7 ))
    caption = EwGetAutoObject( &AnimatedListDevice, AnimatedListDeviceClass )->WeekDays[ 
    EwCheckIndex( itemNo, 7 )];

  AnimatedListDayItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomList.Super2.Bounds ), _this->BottomList.ItemHeight )));
}

/* Slot method to change the selection, when the item has reached its parking position. */
void AnimatedListDayList_OnEnd( AnimatedListDayList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  AnimatedListDeviceClass_OnSetWeekDay( EwGetAutoObject( &AnimatedListDevice, AnimatedListDeviceClass ), 
  -_this->MainList.ScrollOffset / _this->MainList.ItemHeight );
  _this->FadeListOut.Value1 = _this->TopList.Super1.Opacity;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 255;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
}

/* Slot method to make the upper/lower list visible. */
void AnimatedListDayList_OnStart( AnimatedListDayList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->TopList.Super1.Opacity != 255 ) && !_this->FadeListIn.Super1.Enabled )
  {
    _this->FadeListIn.Value1 = _this->TopList.Super1.Opacity;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListIn, 1 );
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
}

/* This is a slot method. */
void AnimatedListDayList_onAnimate( AnimatedListDayList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->BottomList, _this->TopList.Super1.Opacity );
}

/* 'C' function for method : 'AnimatedList::DayList.OnRelease()' */
void AnimatedListDayList_OnRelease( AnimatedListDayList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandler.AutoDeflected && ( _this->TopList.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandler.CurrentPos.Y < _this->MainList.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point1.Y ) 
      / _this->TopList.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandler.CurrentPos.Y > _this->MainList.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point2.Y ) 
        / _this->BottomList.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainList.ScrollOffset / _this->MainList.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainList, item, 1, &_this->Int32Effect, 
      EwNullSlot );
    }
  }

  EwSignal( EwNewSlot( _this, AnimatedListDayList_OnEnd ), ((XObject)_this ));
}

/* Slot method to trigger the next phase of the auto demo. */
void AnimatedListDayList_AutoDemo( AnimatedListDayList _this, XObject sender )
{
  XInt32 val;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( EwNewSlot( _this, AnimatedListDayList_OnStart ), ((XObject)_this ));
  val = EwGetAutoObject( &AnimatedListDevice, AnimatedListDeviceClass )->WeekDay 
  + 1;

  if ( val >= 7 )
    val = 0;

  CoreVerticalList_EnsureVisible( &_this->MainList, val, 1, &_this->AutoDemoEffect, 
  EwNewSlot( _this, AnimatedListDayList_OnEnd ));
}

/* Variants derived from the class : 'AnimatedList::DayList' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListDayList )
EW_END_OF_CLASS_VARIANTS( AnimatedListDayList )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::DayList' */
EW_DEFINE_CLASS( AnimatedListDayList, CoreGroup, SlideTouchHandler, SlideTouchHandler, 
                 SlideTouchHandler, SlideTouchHandler, _None, _None, "AnimatedList::DayList" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListDayList )

/* Initializer for the class 'AnimatedList::DayItemL' */
void AnimatedListDayItemL__Init( AnimatedListDayItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListDayItemL );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListDayItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const002E );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const002F );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0002 ));
  ViewsText_OnSetColor( &_this->Text, AnimatedListButtonTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &AnimatedListListFontM, ResourcesFont ));
}

/* Re-Initializer for the class 'AnimatedList::DayItemL' */
void AnimatedListDayItemL__ReInit( AnimatedListDayItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'AnimatedList::DayItemL' */
void AnimatedListDayItemL__Done( AnimatedListDayItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'AnimatedList::DayItemL.OnSetCaption()' */
void AnimatedListDayItemL_OnSetCaption( AnimatedListDayItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'AnimatedList::DayItemL' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListDayItemL )
EW_END_OF_CLASS_VARIANTS( AnimatedListDayItemL )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::DayItemL' */
EW_DEFINE_CLASS( AnimatedListDayItemL, CoreGroup, Text, Text, Text, Text, Caption, 
                 _None, "AnimatedList::DayItemL" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListDayItemL )

/* Initializer for the class 'AnimatedList::DayItemS' */
void AnimatedListDayItemS__Init( AnimatedListDayItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListDayItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Left, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListDayItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const002B );
  CoreView_OnSetLayout((CoreView)&_this->Background, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Background, _Const002B );
  ViewsRectangle_OnSetColor( &_this->Background, AnimatedListListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Left, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Left, _Const001D );
  ViewsRectangle_OnSetColor( &_this->Left, AnimatedListButtonLineColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0030 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0002 ));
  ViewsText_OnSetColor( &_this->Text, AnimatedListListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Background ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Left ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &AnimatedListListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'AnimatedList::DayItemS' */
void AnimatedListDayItemS__ReInit( AnimatedListDayItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Background );
  ViewsRectangle__ReInit( &_this->Left );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'AnimatedList::DayItemS' */
void AnimatedListDayItemS__Done( AnimatedListDayItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Background );
  ViewsRectangle__Done( &_this->Left );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'AnimatedList::DayItemS.OnSetCaption()' */
void AnimatedListDayItemS_OnSetCaption( AnimatedListDayItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'AnimatedList::DayItemS' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListDayItemS )
EW_END_OF_CLASS_VARIANTS( AnimatedListDayItemS )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::DayItemS' */
EW_DEFINE_CLASS( AnimatedListDayItemS, CoreGroup, Background, Background, Background, 
                 Background, Caption, _None, "AnimatedList::DayItemS" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListDayItemS )

/* Color constant according UI design. */
const XColor AnimatedListButtonLineColor = { 0x00, 0x00, 0x00, 0x7E };

/* Initializer for the class 'AnimatedList::AnimatedList' */
void AnimatedListAnimatedList__Init( AnimatedListAnimatedList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( AnimatedListAnimatedList );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  AnimatedListTimeList__Init( &_this->TimeList, &_this->_XObject, 0 );
  AnimatedListDayList__Init( &_this->DayList, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->AutoDemoTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( AnimatedListAnimatedList );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0031 );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0031 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, _Const0032 );
  CoreRectView__OnSetBounds( &_this->TimeList, _Const0003 );
  CoreRectView__OnSetBounds( &_this->DayList, _Const0033 );
  CoreTimer_OnSetPeriod( &_this->AutoDemoTimer, 1500 );
  CoreTimer_OnSetBegin( &_this->AutoDemoTimer, 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TimeList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->DayList ), 0 );
  _this->Device = EwGetAutoObject( &AnimatedListDevice, AnimatedListDeviceClass );
  _this->AutoDemoTimer.OnTrigger = EwNewSlot( _this, AnimatedListAnimatedList_AutoDemoSlot );
}

/* Re-Initializer for the class 'AnimatedList::AnimatedList' */
void AnimatedListAnimatedList__ReInit( AnimatedListAnimatedList _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Rectangle );
  AnimatedListTimeList__ReInit( &_this->TimeList );
  AnimatedListDayList__ReInit( &_this->DayList );
  CoreTimer__ReInit( &_this->AutoDemoTimer );
}

/* Finalizer method for the class 'AnimatedList::AnimatedList' */
void AnimatedListAnimatedList__Done( AnimatedListAnimatedList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Rectangle );
  AnimatedListTimeList__Done( &_this->TimeList );
  AnimatedListDayList__Done( &_this->DayList );
  CoreTimer__Done( &_this->AutoDemoTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method to trigger the next phase of the auto demo. */
void AnimatedListAnimatedList_AutoDemoSlot( AnimatedListAnimatedList _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  switch ( _this->AutoState )
  {
    case 1 :
      EwPostSignal( EwNewSlot( &_this->DayList, AnimatedListDayList_AutoDemo ), 
        ((XObject)_this ));
    break;

    case 2 :
      EwPostSignal( EwNewSlot( &_this->TimeList, AnimatedListTimeList_AutoDemo ), 
        ((XObject)_this ));
    break;

    case 3 :
      EwPostSignal( EwNewSlot( &_this->TimeList, AnimatedListTimeList_AutoDemo ), 
        ((XObject)_this ));
    break;

    case 4 :
      EwPostSignal( EwNewSlot( &_this->TimeList, AnimatedListTimeList_AutoDemo ), 
        ((XObject)_this ));
    break;

    default : 
      ;
  }

  _this->AutoState = _this->AutoState + 1;

  if ( _this->AutoState > 5 )
    _this->AutoState = 0;
}

/* Variants derived from the class : 'AnimatedList::AnimatedList' */
EW_DEFINE_CLASS_VARIANTS( AnimatedListAnimatedList )
EW_END_OF_CLASS_VARIANTS( AnimatedListAnimatedList )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::AnimatedList' */
EW_DEFINE_CLASS( AnimatedListAnimatedList, CoreGroup, Device, Rectangle, Rectangle, 
                 Rectangle, AutoState, AutoState, "AnimatedList::AnimatedList" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( AnimatedListAnimatedList )

/* Embedded Wizard */
