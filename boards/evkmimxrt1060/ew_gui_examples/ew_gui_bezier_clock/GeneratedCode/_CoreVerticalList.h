/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* The template of this heading text can be found in the file 'head.ewt' in the
* directory 'Platforms' of your Embedded Wizard installation directory. If you
* wish to adapt this text, please copy the template file 'head.ewt' into your
* project directory and edit the copy only. Please avoid any modifications of
* the original template file!
*
* Version  : 9.20
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef _CoreVerticalList_H
#define _CoreVerticalList_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_CoreGroup.h"

/* Forward declaration of the class Core::DialogContext */
#ifndef _CoreDialogContext_
  EW_DECLARE_CLASS( CoreDialogContext )
#define _CoreDialogContext_
#endif

/* Forward declaration of the class Core::Event */
#ifndef _CoreEvent_
  EW_DECLARE_CLASS( CoreEvent )
#define _CoreEvent_
#endif

/* Forward declaration of the class Core::KeyPressHandler */
#ifndef _CoreKeyPressHandler_
  EW_DECLARE_CLASS( CoreKeyPressHandler )
#define _CoreKeyPressHandler_
#endif

/* Forward declaration of the class Core::LayoutContext */
#ifndef _CoreLayoutContext_
  EW_DECLARE_CLASS( CoreLayoutContext )
#define _CoreLayoutContext_
#endif

/* Forward declaration of the class Core::SlideTouchHandler */
#ifndef _CoreSlideTouchHandler_
  EW_DECLARE_CLASS( CoreSlideTouchHandler )
#define _CoreSlideTouchHandler_
#endif

/* Forward declaration of the class Core::TaskQueue */
#ifndef _CoreTaskQueue_
  EW_DECLARE_CLASS( CoreTaskQueue )
#define _CoreTaskQueue_
#endif

/* Forward declaration of the class Core::VerticalList */
#ifndef _CoreVerticalList_
  EW_DECLARE_CLASS( CoreVerticalList )
#define _CoreVerticalList_
#endif

/* Forward declaration of the class Core::View */
#ifndef _CoreView_
  EW_DECLARE_CLASS( CoreView )
#define _CoreView_
#endif

/* Forward declaration of the class Effects::Fader */
#ifndef _EffectsFader_
  EW_DECLARE_CLASS( EffectsFader )
#define _EffectsFader_
#endif

/* Forward declaration of the class Effects::Int32Effect */
#ifndef _EffectsInt32Effect_
  EW_DECLARE_CLASS( EffectsInt32Effect )
#define _EffectsInt32Effect_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* The class Core::VerticalList provides a generic vertical list view. Its purpose 
   is the presentation and the management of list items. Within the view the items 
   can be scrolled and the user can select them. This is very useful to show e.g. 
   the records from a data base. Within the view the items are arranged vertically 
   from top to bottom. If horizontal arrangement is desired, please use the class 
   Core::HorizontalList.
   - All items within the list view are objects of a predetermined item class. Depending 
   on this class, simple text items, image items or even complex GUI components 
   can be shown there. This class is defined in the property @ItemClass. Please 
   note, that only classes derived from Core::View are able to be shown. For example, 
   if you want to create a list of images, assign Views::Image to this property. 
   The height of all items is determined by the property @ItemHeight.
   - The entire number of list items is defined by the property @NoOfItems. By changing 
   this property the list can grow or it can shrink. Usually this value corresponds 
   to the number of items within the associated data source, e.g. to the number 
   of records in a data base.
   - This view is optimized to handle with very large lists consisting of thousand 
   of items. To preserve the memory, only the visible items will be stored there. 
   These will be created, loaded and discarded again automatically by the list view. 
   The only thing you have to provide is an external slot method, which is connected 
   to the view's property @OnLoadItem. Its job is to load an item with its content 
   from a data source. The affected item and its list index are available in the 
   variables @View and @Item.
   - The content of the list can be scrolled by modifying the property @ScrollOffset. 
   Additionally a Core::SlideTouchHandler can be attached to the list view so the 
   user can scroll the items interactively by touching the screen. The property 
   @SlideHandler allows it. 
   - With the property @Endless the list can be configured to start just after the 
   last item again with the first item.
   - The both properties @PaddingTop and @PaddingBottom permit to reserve additional 
   space just before the first and after the last item.
   - Within the list view one item can be selected. The index of the affected item 
   is stored in the property @SelectedItem. If the list view is focused, this item 
   also becomes focused. Focused items automatically receive keyboard events.
   - The method @GetItemsArea() is useful if the area occupied by one or more items 
   should be calculated. In contrast the method @GetItemAtPosition() tries to determines 
   the item which is lying at a given position.
   - The method @InvalidateItems() forces the list to re-load one or more items. 
   This method is very useful if the content of the affected items has been altered 
   in the associated data source.
   - The method @EnsureVisible() has the job to scroll the list until the given 
   item is visible. The method @AdjustList() in contrast tries to scroll the list 
   so the entire list view area is filled with items. Usually this is used after 
   the list size changes. */
EW_DEFINE_FIELDS( CoreVerticalList, CoreGroup )
  EW_ARRAY   ( itemsPool,       CoreView, [8])
  EW_VARIABLE( scrollEffect,    EffectsInt32Effect )
  EW_VARIABLE( View,            CoreView )
  EW_PROPERTY( SlideHandler,    CoreSlideTouchHandler )
  EW_PROPERTY( OnUpdate,        XSlot )
  EW_PROPERTY( OnLoadItem,      XSlot )
  EW_VARIABLE( invalidTail,     XInt32 )
  EW_VARIABLE( invalidHead,     XInt32 )
  EW_VARIABLE( validTail,       XInt32 )
  EW_VARIABLE( validHead,       XInt32 )
  EW_VARIABLE( itemsPoolSize,   XInt32 )
  EW_VARIABLE( Item,            XInt32 )
  EW_PROPERTY( PaddingBottom,   XInt32 )
  EW_PROPERTY( ScrollOffset,    XInt32 )
  EW_PROPERTY( SelectedItem,    XInt32 )
  EW_PROPERTY( PaddingTop,      XInt32 )
  EW_PROPERTY( ItemHeight,      XInt32 )
  EW_PROPERTY( NoOfItems,       XInt32 )
  EW_PROPERTY( ItemClass,       XClass )
  EW_PROPERTY( Endless,         XBool )
  EW_VARIABLE( loading,         XBool )
EW_END_OF_FIELDS( CoreVerticalList )

/* Virtual Method Table (VMT) for the class : 'Core::VerticalList' */
EW_DEFINE_METHODS( CoreVerticalList, CoreGroup )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( CoreVerticalList _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreView _this, CoreEvent aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreGroup _this, XRect aArea, XInt32 
    aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
  EW_METHOD( ArrangeView,       XPoint )( CoreRectView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreRectView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreRectView _this )
  EW_METHOD( ChangeViewState,   void )( CoreGroup _this, XSet aSetState, XSet aClearState )
  EW_METHOD( OnSetBounds,       void )( CoreGroup _this, XRect value )
  EW_METHOD( drawContent,       void )( CoreGroup _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( OnSetFocus,        void )( CoreGroup _this, CoreView value )
  EW_METHOD( OnSetBuffered,     void )( CoreGroup _this, XBool value )
  EW_METHOD( OnSetOpacity,      void )( CoreGroup _this, XInt32 value )
  EW_METHOD( IsDialog,          XBool )( CoreGroup _this, XBool aRecursive )
  EW_METHOD( DispatchEvent,     XObject )( CoreVerticalList _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateLayout,      void )( CoreGroup _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( CoreVerticalList _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( FindSiblingView,   CoreView )( CoreVerticalList _this, CoreView aView, 
    XSet aFilter )
  EW_METHOD( RestackBehind,     void )( CoreVerticalList _this, CoreView aView, 
    CoreView aSibling )
  EW_METHOD( RestackTop,        void )( CoreVerticalList _this, CoreView aView )
  EW_METHOD( Restack,           void )( CoreVerticalList _this, CoreView aView, 
    XInt32 aOrder )
  EW_METHOD( Remove,            void )( CoreVerticalList _this, CoreView aView )
  EW_METHOD( Add,               void )( CoreVerticalList _this, CoreView aView, 
    XInt32 aOrder )
EW_END_OF_METHODS( CoreVerticalList )

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void CoreVerticalList_Draw( CoreVerticalList _this, GraphicsCanvas aCanvas, XRect 
  aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend );

/* The method DispatchEvent() feeds the component with the event passed in the parameter 
   aEvent and propagates it along the so-called focus path. This focus path leads 
   to the currently selected keyboard event receiver view. If the event is rejected 
   by the view, the same operation is repeated for the next superior view. This 
   permits the hierarchical event dispatching until a willing view has handled the 
   event or all views in the focus path have been evaluated. If the event remains 
   still unhandled, it will be passed to the component itself.
   The focus path is established by the property @Focus. 
   DispatchEvent() returns the value returned by the @HandleEvent() method of the 
   view which has handled the event. In the case, the event was not handled, the 
   method returns 'null'. */
XObject CoreVerticalList_DispatchEvent( CoreVerticalList _this, CoreEvent aEvent );

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void CoreVerticalList_UpdateViewState( CoreVerticalList _this, XSet aState );

/* The method FindSiblingView() searches for a sibling view of the view specified 
   in the parameter aView - aView itself will be excluded from the search operation.
   The method combines the functionality of @FindNextView() and @FindPrevView() 
   and tries to find any neighbor view (regarding the Z-order not the position).
   The additional parameter aFilter can be used to limit the search operation to 
   special views only, e.g. to visible and touchable views.
   If there are no other views complying the filter condition, the method returns 
   'null'. In contrast to other find methods, FindSiblingView() will fail, if it 
   has been invoked with aView == 'null'. */
CoreView CoreVerticalList_FindSiblingView( CoreVerticalList _this, CoreView aView, 
  XSet aFilter );

/* The method RestackBehind() arranges the view aView behind the view aSibling. 
   If no sibling is specified (aSibling == 'null'), the view is arranged in front 
   of all other views of the component. This method modifies the Z-order of the 
   view.
   Please note, changing the Z-order of views within a component containing a Core::Outline 
   view can cause this outline to update its automatic row or column formation. */
void CoreVerticalList_RestackBehind( CoreVerticalList _this, CoreView aView, CoreView 
  aSibling );

/* The method RestackTop() elevates the view aView to the top of its component. 
   After this operation the view is not covered by its sibling views anymore. This 
   method modifies the Z-order of the view.
   Please note, changing the Z-order of views within a component containing a Core::Outline 
   view can cause this outline to update its automatic row or column formation. */
void CoreVerticalList_RestackTop( CoreVerticalList _this, CoreView aView );

/* The method Restack() changes the Z-order of views in the component. Depending 
   on the parameter aOrder the method will elevate or lower the given view aView. 
   If aOrder is negative, the view will be lowered to the background. If aOrder 
   is positive, the view will be elevated to the foreground. If aOrder == 0, no 
   modification will take place.
   The absolute value of aOrder determines the number of sibling views the view 
   has to skip over in order to reach its new Z-order.
   Please note, changing the Z-order of views within a component containing a Core::Outline 
   view can cause this outline to update its automatic row or column formation. */
void CoreVerticalList_Restack( CoreVerticalList _this, CoreView aView, XInt32 aOrder );

/* The method Remove() removes the given view aView from the component. After this 
   operation the view doesn't belong anymore to the component - the view is not 
   visible and it can't receive any events.
   Please note, removing of views from a component containing a Core::Outline view 
   can cause this outline to update its automatic row or column formation.
   Please note, it the removed view is currently selected by the @Focus property, 
   the framework will automatically select other sibling view, which will be able 
   to react to keyboard events. */
void CoreVerticalList_Remove( CoreVerticalList _this, CoreView aView );

/* The method Add() inserts the given view aView into this component and places 
   it at a Z-order position resulting from the parameter aOrder. The parameter determines 
   the number of sibling views the view has to skip over starting with the top most 
   view. If aOrder == 0, the newly added view will obtain the top most position. 
   If the value is negative, the view will be lowered to the background accordingly 
   to the absolute value of aOrder. After this operation the view belongs to the 
   component - the view can appear on the screen and it can receive events.
   Please note, adding of views to a component containing a Core::Outline view can 
   cause this outline to update its automatic row or column formation. */
void CoreVerticalList_Add( CoreVerticalList _this, CoreView aView, XInt32 aOrder );

/* 'C' function for method : 'Core::VerticalList.releaseTailItem()' */
CoreView CoreVerticalList_releaseTailItem( CoreVerticalList _this );

/* 'C' function for method : 'Core::VerticalList.confirmTailItem()' */
CoreView CoreVerticalList_confirmTailItem( CoreVerticalList _this );

/* 'C' function for method : 'Core::VerticalList.releaseHeadItem()' */
CoreView CoreVerticalList_releaseHeadItem( CoreVerticalList _this );

/* 'C' function for method : 'Core::VerticalList.confirmHeadItem()' */
CoreView CoreVerticalList_confirmHeadItem( CoreVerticalList _this );

/* 'C' function for method : 'Core::VerticalList.onSlideSlot()' */
void CoreVerticalList_onSlideSlot( CoreVerticalList _this, XObject sender );

/* 'C' function for method : 'Core::VerticalList.onStartSlideSlot()' */
void CoreVerticalList_onStartSlideSlot( CoreVerticalList _this, XObject sender );

/* 'C' function for method : 'Core::VerticalList.OnSetEndless()' */
void CoreVerticalList_OnSetEndless( CoreVerticalList _this, XBool value );

/* 'C' function for method : 'Core::VerticalList.OnSetSlideHandler()' */
void CoreVerticalList_OnSetSlideHandler( CoreVerticalList _this, CoreSlideTouchHandler 
  value );

/* 'C' function for method : 'Core::VerticalList.OnSetScrollOffset()' */
void CoreVerticalList_OnSetScrollOffset( CoreVerticalList _this, XInt32 value );

/* 'C' function for method : 'Core::VerticalList.OnSetItemHeight()' */
void CoreVerticalList_OnSetItemHeight( CoreVerticalList _this, XInt32 value );

/* 'C' function for method : 'Core::VerticalList.OnSetNoOfItems()' */
void CoreVerticalList_OnSetNoOfItems( CoreVerticalList _this, XInt32 value );

/* The method GetViewForItem() returns the view corresponding to the item with the 
   number specified in the parameter aItem. The first item has the number 0, the 
   second 1, and so far.
   Since this list component manages the views internally within a small cache maintaining 
   only such views which are visible, the method may return 'null' when asking for 
   the view corresponding to an item lying outside the visible area of the list. 
   Similarly, the method returns 'null' when it has been used in context of the 
   @OnLoadItem slot method. @OnLoadItem is signaled while the list updates and rearranges 
   its items. Trying to access the views during this phase would interfere with 
   the update process returning incorrect views. Don't call GetViewForItem() from 
   @OnLoadItem slot method.
   Please note, if the list is configured with the property @Endless == 'true', 
   multiple copies of one and the same item may be displayed on the screen. In such 
   case the method returns the view corresponding to the first matching item lying 
   actually within the cache. */
CoreView CoreVerticalList_GetViewForItem( CoreVerticalList _this, XInt32 aItem );

/* The method GetItemAtPosition() tries to determine an item at the given position 
   aPosition. This position is valid in the coordinate space of the view's @Owner. 
   If an item could be found, the method returns its index. The first item has the 
   index 0, the second 1, and so far. If no item is found, the method returns -1. */
XInt32 CoreVerticalList_GetItemAtPosition( CoreVerticalList _this, XPoint aPos );

/* The method GetItemsArea() determines a rectangular area within the list view 
   occupied by one or more items. The index of the item to start the calculation 
   is specified in the parameter aFirstItem. The parameter aLastItem specifies the 
   index of the last affected item. The first item has the index 0, the second 1, 
   and so far. The determined area is expressed in coordinates relative to the top-left 
   corner of the view's @Owner. If none of the specified items does exist, the method 
   returns an empty area.
   Please note, if the list is configured with the property @Endless == 'true', 
   multiple copies of one and the same item may be displayed on the screen. In such 
   case the method returns the area corresponding to the first set of items lying 
   actually within the list view. */
XRect CoreVerticalList_GetItemsArea( CoreVerticalList _this, XInt32 aFirstItem, 
  XInt32 aLastItem );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreVerticalList_H */

/* Embedded Wizard */
