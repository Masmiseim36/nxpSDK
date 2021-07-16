/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
* \file iodefs.h
* Define IO event strings and types
*/
#ifndef GR_IODEFS_H
#define GR_IODEFS_H

#include <stdint.h>

/**
 * \file iodefs.h
 * These are standard event definitions
 * Their name and their expected data payload.
 *
 * This file defines the events in logical, order.  Please insert
 * new events accordingly.
 *
 */

/**
 * APPLICATION EVENTS
 */

/**
 * Event to notify the internal managers and plugins that everything is loaded
 * This is the first event in the system
 */
#define GR_EVENT_INTERNAL_INIT 			"gre.internalinit"
#define GR_EVENT_INTERNAL_INIT_FMT		NULL

/**
 * Event to notify that the system has been initialised and is ready to go.
 * Sent by io manager in response to the gre.internalinit
 */
#define GR_EVENT_APP_INIT      			"gre.init"
#define GR_EVENT_APP_INIT_FMT  			NULL //No event data

/**
 * Event that is delivered when the application is shutting down.
 */
#define GR_EVENT_APP_QUIT      			"gre.quit"
#define GR_EVENT_APP_QUIT_FMT  			NULL //No event data

/**
 * Event to load a new model if using sbengine and the "-l" option
 */
#define GR_EVENT_LOAD_APP				"gre.load_app"
#define GR_EVENT_LOAD_APP_FMT			"1s0 model"

/**
 * POINTER/TOUCH EVENTS
 */
#define	GR_EVENT_BTN_LEFT				0x0001
#define	GR_EVENT_BTN_MIDDLE				0x0002
#define	GR_EVENT_BTN_RIGHT				0x0004

#define GR_EVENT_PTR_FMT 				"4u1 button 4u1 timestamp 2u1 subtype 2s1 x 2s1 y 2s1 z 2s1 id 2s1 spare"
typedef struct gr_ptr_event {
	uint32_t	button;
	uint32_t	timestamp;
	uint16_t	subtype;
    int16_t     x;
    int16_t     y;
    int16_t		z;
    int16_t		id;
    int16_t		spare;
} gr_ptr_event_t;

typedef struct gr_ptr_mt_event {
	uint32_t	timestamp;
	uint16_t	npoints;
	uint16_t	data;
} gr_ptr_mt_event_t;

#define GR_MT_PTR_EVENT_STRFMT 			"4u1 timestamp 2s1 npoints 2s%d x 2s%d y 2s%d z 2s%d id 2s%d state 2s%d spare"

#define GR_PTR_STATE_PRESS				0x01
#define GR_PTR_STATE_MOTION	 			0x02
#define GR_PTR_STATE_RELEASE			0x03

/**
 * Event that is emitted when multi touch events occur
 */
#define GR_MT_EVENT						"gre.mtevent"

/**
 * Event that is emitted when a pointer hovers over a control
 */
#define GR_EVENT_MOTION    	 			"gre.motion"

/**
 * Event that is delivered when the mouse or touchscreen enters a controls area, if dragging a pointer.
 */
#define GR_EVENT_INBOUND   				"gre.inbound"

/**
 * Event that is delivered when the mouse or touchscreen leaves a controls area, if dragging a pointer.
 */
#define GR_EVENT_OUTBOUND  				"gre.outbound"

/**
 * Event that is delivered when a control's area is entered and there are currently other touch points in the control's area.
 */
#define GR_EVENT_MT_INBOUND  			"gre.mtinbound"

/**
 * Event that is delivered when a control's area is left and there are currently other touch points in the control's area.
 */
#define GR_EVENT_MT_OUTBOUND  			"gre.mtoutbound"

/**
 * Event that is sent when a mouse button or touchscreen has been press.
 */
#define GR_EVENT_PRESS     				"gre.press"

/**
 * Event to indicate when a mouse button or touchscreen has been released.
 */
#define GR_EVENT_RELEASE   				"gre.release"

/**
 * Event that is emitted when a mouse or touchscreen has had a valid press and release
 */
#define GR_EVENT_TOUCH					"gre.touch"

/**
 * Event that is emitted when a touchscreen has been pressed in a mutitouch environment,
 * a generic press has already been sent for the first id
 */
#define GR_EVENT_MT_PRESS				"gre.mtpress"

/**
 * Event that is emitted when a touchscreen has been released in a mutitouch environment
 */
#define GR_EVENT_MT_RELEASE				"gre.mtrelease"

/**
 * Event that is emitted when a touchscreen gets a motion in a multitouch environment
 */
#define GR_EVENT_MT_MOTION				"gre.mtmotion"

/**
 * Event that is fired to notify any listeners that the application is about to 'sleep'
 * This allows clients to gracefully kill some objects that will need to be recreated upon wake
 */
#define GR_EVENT_PRESLEEP 				"gre.presleep"
#define GR_EVENT_PRESLEEP_FMT  			NULL //No event data

/**
 * Event that is fired to notify any listeners a final warning that the application is about to 'sleep'
 * This allows clients to know that presleep has been completed, and perform any final teardown
 */
#define GR_EVENT_PRESLEEP_FINAL 		"gre.presleep_final"
#define GR_EVENT_PRESLEEP_FINAL_FMT  	NULL //No event data

/**
 * Event that is fired to notify any listeners that the application has woken up
 * This allows clients to recreate or reinitialize
 */
#define GR_EVENT_POSTSLEEP 				"gre.postsleep"
#define GR_EVENT_POSTSLEEP_FMT  		NULL //No event data
/**
 * KEY EVENTS
 */

#define GR_KEYMODBIT_SHIFT				0
#define GR_KEYMODBIT_CTRL				1
#define GR_KEYMODBIT_ALT				2

#define GR_KEYMOD_SHIFT					(1 << GR_KEYMODBIT_SHIFT)
#define GR_KEYMOD_CTRL					(1 << GR_KEYMODBIT_CTRL)
#define GR_KEYMOD_ALT					(1 << GR_KEYMODBIT_ALT)

typedef enum gr_key {
	GR_KEY_INVALID 		= 0,

	GR_KEY_BACKSPACE	= 0x08,
	GR_KEY_TAB			= 0x09,

	GR_KEY_CLEAR		= 0X0C,
	GR_KEY_ENTER		= 0x0D,

	GR_KEY_PAUSE		= 0x13,
	GR_KEY_CAPS_LOCK	= 0x14,

	GR_KEY_ESCAPE		= 0x1B,

	GR_KEY_SPACE		= 0x20,
	GR_KEY_PAGE_UP		= 0x21,
	GR_KEY_PAGE_DOWN	= 0x22,
	GR_KEY_END			= 0x23,
	GR_KEY_HOME			= 0x24,
	GR_KEY_LEFT 		= 0x25,
	GR_KEY_UP 			= 0x26,
	GR_KEY_RIGHT 		= 0x27,
	GR_KEY_DOWN 		= 0x28,

	GR_KEY_PRINT_SCREEN	= 0x2C,
	GR_KEY_INSERT		= 0x2D,
	GR_KEY_DELETE		= 0x2E,

	GR_KEY_0			= 0x30,
	GR_KEY_1			= 0x31,
	GR_KEY_2			= 0x32,
	GR_KEY_3			= 0x33,
	GR_KEY_4			= 0x34,
	GR_KEY_5			= 0x35,
	GR_KEY_6			= 0x36,
	GR_KEY_7			= 0x37,
	GR_KEY_8			= 0x38,
	GR_KEY_9			= 0x39,

	GR_KEY_A			= 0x41,
	GR_KEY_B			= 0x42,
	GR_KEY_C			= 0x43,
	GR_KEY_D			= 0x44,
	GR_KEY_E			= 0x45,
	GR_KEY_F			= 0x46,
	GR_KEY_G			= 0x47,
	GR_KEY_H			= 0x48,
	GR_KEY_I			= 0x49,
	GR_KEY_J			= 0x4A,
	GR_KEY_K			= 0x4B,
	GR_KEY_L			= 0x4C,
	GR_KEY_M			= 0x4D,
	GR_KEY_N			= 0x4E,
	GR_KEY_O			= 0x4F,
	GR_KEY_P			= 0x50,
	GR_KEY_Q			= 0x51,
	GR_KEY_R			= 0x52,
	GR_KEY_S			= 0x53,
	GR_KEY_T			= 0x54,
	GR_KEY_U			= 0x55,
	GR_KEY_V			= 0x56,
	GR_KEY_W			= 0x57,
	GR_KEY_X			= 0x58,
	GR_KEY_Y			= 0x59,
	GR_KEY_Z			= 0x5A,

	//L/R Windows keys or Command keys
	GR_KEY_LGUI 		= 0x5B,
	GR_KEY_RGUI 		= 0x5C,

	GR_KEY_NUMPAD0 		= 0x60,
	GR_KEY_NUMPAD1 		= 0x61,
	GR_KEY_NUMPAD2 		= 0x62,
	GR_KEY_NUMPAD3 		= 0x63,
	GR_KEY_NUMPAD4 		= 0x64,
	GR_KEY_NUMPAD5 		= 0x65,
	GR_KEY_NUMPAD6 		= 0x66,
	GR_KEY_NUMPAD7 		= 0x67,
	GR_KEY_NUMPAD8 		= 0x68,
	GR_KEY_NUMPAD9 		= 0x69,
	//0x6A-0x6F are keys on the numpad, the standard keys are further down
	GR_KEY_MULTIPLY		= 0x6A,
	GR_KEY_ADD			= 0x6B,
	//GR_KEY_SEPARATOR	= 0x6C,
	GR_KEY_SUBTRACT		= 0x6D,
	GR_KEY_DECIMAL		= 0x6E,
	GR_KEY_DIVIDE		= 0x6F,
	GR_KEY_F1	 		= 0x70,
	GR_KEY_F2	 		= 0x71,
	GR_KEY_F3	 		= 0x72,
	GR_KEY_F4	 		= 0x73,
	GR_KEY_F5	 		= 0x74,
	GR_KEY_F6	 		= 0x75,
	GR_KEY_F7	 		= 0x76,
	GR_KEY_F8	 		= 0x77,
	GR_KEY_F9	 		= 0x78,
	GR_KEY_F10	 		= 0x79,
	GR_KEY_F11	 		= 0x7A,
	GR_KEY_F12	 		= 0x7B,
	GR_KEY_F13	 		= 0x7C,
	GR_KEY_F14	 		= 0x7D,
	GR_KEY_F15	 		= 0x7E,
	GR_KEY_F16	 		= 0x7F,
	GR_KEY_F17	 		= 0x80,
	GR_KEY_F18	 		= 0x81,
	GR_KEY_F19	 		= 0x82,
	GR_KEY_F20	 		= 0x83,
	GR_KEY_F21	 		= 0x84,
	GR_KEY_F22	 		= 0x85,
	GR_KEY_F23	 		= 0x86,
	GR_KEY_F24	 		= 0x87,

	GR_KEY_NUM_LOCK		= 0x90,
	GR_KEY_SCROLL_LOCK	= 0x91,

	GR_KEY_LSHIFT 		= 0xA0,
	GR_KEY_RSHIFT 		= 0xA1,
	GR_KEY_LCTRL 		= 0xA2,
	GR_KEY_RCTRL 		= 0xA3,
	GR_KEY_LALT 		= 0xA4,
	GR_KEY_RALT 		= 0xA5,

	GR_KEY_SEMICOLON	= 0xBA,
	GR_KEY_EQUALS		= 0xBB,
	GR_KEY_COMMA		= 0xBC,
	GR_KEY_MINUS		= 0xBD,
	GR_KEY_PERIOD		= 0xBE,
	GR_KEY_FSLASH		= 0xBF,
	GR_KEY_GRAVE		= 0xC0,

	GR_KEY_LBRACKET		= 0xDB,
	GR_KEY_BSLASH		= 0xDC,
	GR_KEY_RBRACKET		= 0xDD,
	GR_KEY_QUOTE		= 0xDE,
} gr_key_t;

/**
 * Event that is emitted when a key is pressed.
 */
#define GR_EVENT_KEY_DOWN				"gre.keydown"
/**
 * Event that is emitted when a key is release.
 */
#define GR_EVENT_KEY_UP					"gre.keyup"
/**
 * Event that is emitted when a key is continually pressed.
 */
#define GR_EVENT_KEY_REPEAT				"gre.keyrepeat"

#define GR_EVENT_KEY_FMT				"4u1 code 2u1 key 2u1 modifiers"
typedef struct gr_key_event {
    uint32_t	code;			///< Unicode value if applicable else 0
	uint16_t	key;			///< GR_KEY value if applicable else 0
    uint16_t	modifiers;		///< GR_KEYMOD bitfield mask
} gr_key_event_t;

/**
 * SCREEN EVENTS
 */

/**
 * Event that is emitted before and after a screen is shown.
 */
#define GR_EVENT_SCREEN_SHOW_PRE		"gre.screenshow.pre"
#define GR_EVENT_SCREEN_SHOW_POST		"gre.screenshow.post"
/**
 * Event that is emitted before and after a screen is hidden.
 */
#define GR_EVENT_SCREEN_HIDE_PRE		"gre.screenhide.pre"
#define GR_EVENT_SCREEN_HIDE_POST		"gre.screenhide.post"

#define GR_EVENT_SCREEN_FMT				"1s0 name"
typedef struct gr_screen_event {
    char		name[1];
} gr_screen_event_t;

//TODO:  Figure out what this is for and who uses it.
#define GR_EVENT_REDRAW    				"gre.redraw"
#define GR_EVENT_REDRAW_FMT  			"4s1 x 4s1 y 4u1 width 4u1 height"
typedef struct gr_redraw_event {
    int32_t     x;
    int32_t     y;
    uint32_t    width;
    uint32_t    height;
} gr_redraw_event_t;

#define GR_EVENT_REDRAW_COMPLETE		"gre.redraw.complete"
#define GR_EVENT_REDRAW_COMPLETE_FMT	"4s1 x 4s1 y 4u1 width 4u1 height"

/*
 * Enable/Disable Software cursor in the screen manager
 */
#define GR_EVENT_SWCURSOR_ENABLE 		"gre.swcursor.enable"
#define GR_EVENT_SWCURSOR_DISABLE 		"gre.swcursor.disable"
#define GR_EVENT_SWCURSOR_FMT			NULL

/**
 * Focus events
 */
#define GR_EVENT_GOT_FOCUS      		"gre.gotfocus"
#define GR_EVENT_GOT_FOCUS_FMT  		NULL //No event data

#define GR_EVENT_LOST_FOCUS      		"gre.lostfocus"
#define GR_EVENT_LOST_FOCUS_FMT  		NULL //No event data

/**
 * Table events
 */
#define GR_EVENT_TABLE_VIEWPORT  		"gre.table.viewport"
#define GR_EVENT_TABLE_VIEWPORT_FMT  	"4u1 top_row 4u1 left_col 4u1 bot_row 4u1 right_col 1s0 table"
typedef struct gr_table_viewport_event {
    uint32_t    top_row;		///< Top row visible
    uint32_t    left_col;		///< Left column visible
    uint32_t    bot_row;		///< Bottom row visible
    uint32_t    right_col;		///< Right column visible
    char		table[1];		///< The table this is for
} gr_table_viewport_event_t;

#define GR_EVENT_CELL_GOT_FOCUS     	"gre.cell.gotfocus"
#define GR_EVENT_CELL_FOCUS_FMT  		"4u1 row 4u1 col 1s0 table"

#define GR_EVENT_CELL_LOST_FOCUS    	"gre.cell.lostfocus"
typedef struct gr_table_focus_event {
    uint32_t    row;		///< current row
    uint32_t    col;		///< current col
    char		table[1];	///< The table for the event
} gr_table_focus_event_t;

/**
 * Animation events
 */
#define GR_EVENT_ANIMATION_COMPLETE		"gre.animate.complete."
#define GR_EVENT_ANIMATION_STOP			"gre.animate.stop."
#define GR_EVENT_ANIMATION_COMPLETE_FMT	"1s0 id"

/**
 * Event that is fired when scroll/jog wheel is used
 */
#define GR_EVENT_SCROLL_WHEEL 			"gre.scrollwheel"
#define GR_EVENT_SCROLL_WHEEL_FMT		"4s1 value"
typedef struct {
	int 		value;
	int 		extra[2];
} gr_scroll_wheel_event_t;

/*
 * Table croll events
 */
#define GR_EVENT_TABLE_DRAG_START		"gre.table.drag_start"
#define GR_EVENT_TABLE_DRAG_STOP		"gre.table.drag_stop"
#define GR_EVENT_TABLE_SCROLL_TRIGGER	"gre.table.scroll_trigger"
#define GR_EVENT_TABLE_SCROLL_START		"gre.table.scroll_start"
#define GR_EVENT_TABLE_SCROLL_STOP		"gre.table.scroll_stop"
#define GR_EVENT_TABLE_SCROLL_CANCEL	"gre.table.scroll_cancel"
#define GR_EVENT_TABLE_SCROLL_COMPLETE	"gre.table.scroll_complete"

/*
 * Generic scroll events
 */
#define GR_EVENT_DRAG_START				"gre.drag.start"
#define GR_EVENT_DRAG_STOP				"gre.drag.stop"
#define GR_EVENT_SCROLL_TRIGGER			"gre.scroll.trigger"
#define GR_EVENT_SCROLL_START			"gre.scroll.start"
#define GR_EVENT_SCROLL_STOP			"gre.scroll.stop"
#define GR_EVENT_SCROLL_CANCEL			"gre.scroll.cancel"
#define GR_EVENT_SCROLL_COMPLETE		"gre.scroll.complete"


#define GR_EVENT_SCROLL_FMT				"1s0 name"
typedef struct {
    char		name[1];
} gr_scroll_event_t;

/*
 * Event that is emitted when app gains focus
 */
#define GR_EVENT_MOBILE_ON_RESUME 		"gre.mobile.on_resume"
/*
 * Event that is emitted when app loses focus
 */
#define GR_EVENT_MOBILE_ON_PAUSE  		"gre.mobile.on_pause"
/*
 * Event that is emitted when app is sent to the background
 */
#define GR_EVENT_MOBILE_ON_BACKGROUND 	"gre.mobile.on_background"

#endif /* GR_IODEFS_H */
