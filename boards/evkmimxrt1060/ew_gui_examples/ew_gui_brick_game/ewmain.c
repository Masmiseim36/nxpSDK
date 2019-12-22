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
********************************************************************************
*
* DESCRIPTION:
*   This file implements a generic framework for running Embedded Wizard
*   generated GUI applications on a dedicated target with or without the usage
*   of an operating system.
*   The module ewmain contains three major functions that are used within
*   every Embedded Wizard GUI application: EwInit() - EwProcess() - EwDone().
*   These functions represent the lifecycle of the entire GUI application.
*
*   EwInit() is responsible to initialize the system, to configure the display,
*   to get access to the desired input devices (like keyboard or touch),
*   to initialize the Embedded Wizard Runtime Environment / Graphics Engine,
*   to create an instance of the application class and to initialize all
*   needed peripheral components.
*
*   EwProcess() implements one cycle of the main loop. This function has to be
*   called in an (endless) loop and contains typically the following steps:
*   1. Processing data from your device driver(s)
*   2. Processing key events
*   3. Processing cursor or touch screen events
*   4. Processing timers
*   5. Processing signals
*   6. Updating the screen
*   7. Triggering the garbage collection
*
*   EwDone() is responsible to shutdown the application and to release all
*   used resources.
*
*   Important: Please be aware that every Embedded Wizard GUI application
*   requires the execution in a single GUI task!
*   If you are working with an operating system and your software is using
*   several threads/tasks, please take care to access your GUI application
*   only within the context of your GUI thread/task. Use operating system
*   services to exchange data or events between the GUI thread/task and other
*   worker threads/tasks.
*
*   In order to keep this framework independent from the particular GUI
*   application, the application class and the screen size are taken from the
*   generated code. In this manner, it is not necessary to modify this file
*   when creating new GUI applications. Just set the attributes 'ScreenSize'
*   and 'ApplicationClass' of the profile in the Embedded Wizard IDE.
*
*   For more information concerning the integration of an Embedded Wizard
*   generated GUI application into your main application, please see
*   https://doc.embedded-wizard.de/main-loop
*
*******************************************************************************/

#include "tlsf.h"

#include "ewrte.h"
#include "ewgfx.h"
#include "ewextgfx.h"
#include "ewgfxdefs.h"
#include "Core.h"
#include "Graphics.h"

#include "ew_bsp_clock.h"
#include "ew_bsp_event.h"
#include "ew_bsp_display.h"
#include "ew_bsp_touch.h"

#include "DeviceDriver.h"

/* set SDRAM_BASE_ADDR to 0x81000000 instead of 0x80000000 to reserve space for
   program execution in SDRAM (MIMXRT1052xxx6A_sdram_txt.icf) */
#define SDRAM_BASE_ADDR 0x81000000
#define SDRAM_SIZE_BYTES (16 * 1024 * 1024)

/* define physical dimension of the LCD framebuffer */
#define FRAME_BUFFER_WIDTH    480
#define FRAME_BUFFER_HEIGHT   272

/* calculated addresses for framebuffer(s) and memory manager */
#define FRAME_BUFFER_ADDR     (void*)(SDRAM_BASE_ADDR)
#define FRAME_BUFFER_SIZE     FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * FRAME_BUFFER_DEPTH

#ifdef EW_USE_DOUBLE_BUFFER
  #define DOUBLE_BUFFER_ADDR  (void*)((unsigned char*)FRAME_BUFFER_ADDR + FRAME_BUFFER_SIZE)
  #define DOUBLE_BUFFER_SIZE  FRAME_BUFFER_SIZE
#else
  #define DOUBLE_BUFFER_ADDR  (void*)(0)
  #define DOUBLE_BUFFER_SIZE  0
  #define NUMBER_OF_FIELDS    3
#endif

#define MEMORY_POOL_ADDR      (void*)((unsigned char*)FRAME_BUFFER_ADDR + FRAME_BUFFER_SIZE + DOUBLE_BUFFER_SIZE)
#define MEMORY_POOL_SIZE      SDRAM_SIZE_BYTES - FRAME_BUFFER_SIZE - DOUBLE_BUFFER_SIZE


/* During start of application, the RTC is set to default time,
   if the current RTC time is before minimum time. */
#define RTC_MINIMUM_TIME 978307200      /* 01/01/2001 */
#define RTC_DEFAULT_TIME 1551441600     /* 01/03/2019 */

#undef USE_TERMINAL_INPUT

#define CHECK_HANDLE( handle ) \
  if ( !handle )               \
  {                            \
    EwPrint( "[failed]\n" );   \
    return 0;                  \
  }                            \
  else                         \
    EwPrint( "[OK]\n" );

/* helper functions used within this module */
static void EwUpdate( XViewport* aViewport, CoreRoot aApplication );
static XEnum EwGetKeyCommand( void );

tlsf_t               MemPool;
static CoreRoot      RootObject;
static XViewport *   Viewport;


/*******************************************************************************
* FUNCTION:
*   EwInit
*
* DESCRIPTION:
*   EwInit() is responsible to initialize the system, to configure the display,
*   to get access to the desired input devices (like keyboard or touch),
*   to initialize the Embedded Wizard Runtime Environment / Graphics Engine,
*   to create an instance of the application class and to initialize all
*   needed peripheral components.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   1 if successful, 0 otherwise.
*
*******************************************************************************/
int EwInit( void )
{
  /* configure system tick counter */
  EwBspConfigSystemTick();

  /* configure realtime clock */
  EwPrint( "Initialize Realtime Clock...                 " );
  EwBspConfigRealTimeClock();

  /* set RTC, if current RTC time is before the minimum time */
  if ( EwBspGetTime() < RTC_MINIMUM_TIME )
    EwBspSetTime( RTC_DEFAULT_TIME );
  EwPrint( "[OK]\n" );

  /* initialize display */
  EwPrint( "Initialize Display...                        " );
  EwBspConfigDisplay( FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, FRAME_BUFFER_ADDR );
  EwPrint( "[OK]\n" );

  /* initialize touchscreen */
  EwPrint( "Initialize Touch Driver...                   " );
  EwBspConfigTouch( FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT );
  EwPrint( "[OK]\n" );

  /* initialize tlsf memory manager */
  EwPrint( "Initialize Memory Manager...                 " );
  MemPool = tlsf_create_with_pool( MEMORY_POOL_ADDR, MEMORY_POOL_SIZE );
  CHECK_HANDLE( MemPool );

  /* initialize the Graphics Engine and Runtime Environment */
  EwPrint( "Initialize Graphics Engine...                " );
  CHECK_HANDLE( EwInitGraphicsEngine( 0 ));

  /* create the applications root object ... */
  EwPrint( "Create Embedded Wizard Root Object...        " );
  RootObject = (CoreRoot)EwNewObjectIndirect( EwApplicationClass, 0 );
  CHECK_HANDLE( RootObject );

  EwLockObject( RootObject );
  CoreRoot__Initialize( RootObject, EwScreenSize );

  /* create Embedded Wizard viewport object to provide uniform access to the framebuffer */
  EwPrint( "Create Embedded Wizard Viewport...           " );
  Viewport = EwInitViewport( EwScreenSize, EwNewRect( 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT ),
    0, 255, FRAME_BUFFER_ADDR, DOUBLE_BUFFER_ADDR, 0, 0 );
  CHECK_HANDLE( Viewport );

  /* initialize your device driver(s) that provide data for your GUI */
  DeviceDriver_Initialize();

  return 1;
}


/*******************************************************************************
* FUNCTION:
*   EwDone
*
* DESCRIPTION:
*   EwDone() is responsible to shutdown the application and to release all
*   used resources.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwDone( void )
{
  /* deinitialize your device driver(s) */
  DeviceDriver_Deinitialize();

  /* destroy the applications root object and release unused resources and memory */
  EwPrint( "Shutting down Application...                 " );
  EwDoneViewport( Viewport );
  EwUnlockObject( RootObject );
  EwReclaimMemory();
  EwPrint( "[OK]\n" );

  /* deinitialize the Graphics Engine */
  EwPrint( "Deinitialize Graphics Engine...              " );
  EwDoneGraphicsEngine();
  EwPrint( "[OK]\n" );

  /* deinitialize tlsf memory manager */
  tlsf_destroy( MemPool );
}


/*******************************************************************************
* FUNCTION:
*   EwProcess
*
* DESCRIPTION:
*   EwProcess() implements one cycle of the main loop. This function has to be
*   called in an (endless) loop and contains typically the following steps:
*   1. Processing data from your device driver(s)
*   2. Processing key events
*   3. Processing cursor or touch screen events
*   4. Processing timers
*   5. Processing signals
*   6. Updating the screen
*   7. Triggering the garbage collection
*   For more information concerning the integration of an Embedded Wizard
*   generated GUI application into your main application, please see
*   https://doc.embedded-wizard.de/main-loop
*
* ARGUMENTS:
*   None.
*
* RETURN VALUE:
*   1, if further processing is needed, 0 otherwise.
*
*******************************************************************************/
int EwProcess( void )
{
  int           timers  = 0;
  int           signals = 0;
  int           events  = 0;
  int           devices = 0;
  XEnum         cmd     = CoreKeyCodeNoKey;
  static int    touched = 0;
  static XPoint touchPos;

  /* process data of your device driver(s) and update the GUI
     application by setting properties or by triggering events */
  devices = DeviceDriver_ProcessData();

  /* receive keyboard inputs */
  cmd = EwGetKeyCommand();

  if ( cmd != CoreKeyCodeNoKey )
  {
    if ( cmd == CoreKeyCodePower )
      return 0;

    /* feed the application with a 'press' and 'release' event */
    events |= CoreRoot__DriveKeyboardHitting( RootObject, cmd, 0, 1 );
    events |= CoreRoot__DriveKeyboardHitting( RootObject, cmd, 0, 0 );
  }

  /* receive touch inputs and provide the application with them */
  if ( EwBspGetTouchPosition( &touchPos ))
  {
    /* begin of touch cycle */
    if ( touched == 0 )
      CoreRoot__DriveCursorHitting( RootObject, 1, 0, touchPos );

    /* movement during touch cycle */
    else if ( touched == 1 )
      CoreRoot__DriveCursorMovement( RootObject, touchPos );

    touched = 1;
    events  = 1;
  }
  /* end of touch cycle */
  else if ( touched == 1 )
  {
    CoreRoot__DriveCursorHitting( RootObject, 0, 0, touchPos );
    touched = 0;
    events  = 1;
  }

  /* process expired timers */
  timers = EwProcessTimers();

  /* process the pending signals */
  signals = EwProcessSignals();

  /* refresh the screen, if something has changed and draw its content */
  if ( devices || timers || signals || events )
  {
    if ( CoreRoot__DoesNeedUpdate( RootObject ))
      EwUpdate( Viewport, RootObject );

    /* after each processed message start the garbage collection */
    EwReclaimMemory();

    /* print current memory statistic to serial interface - uncomment if needed */
    //  EwPrintProfilerStatistic( 0 );
  }
  else
  {
    /* otherwise sleep/suspend the UI application until a certain event occurs or a timer expires... */
    EwBspWaitForSystemEvent( EwNextTimerExpiration());
  }

  return 1;
}


/*******************************************************************************
* FUNCTION:
*   EwUpdate
*
* DESCRIPTION:
*   The function EwUpdate performs the screen update of the dirty area.
*
* ARGUMENTS:
*   aViewPort    - Viewport used for the screen update.
*   aApplication - Root object used for the screen update.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
static void EwUpdate( XViewport* aViewport, CoreRoot aApplication )
{
  XBitmap*       bitmap;
  GraphicsCanvas canvas     = EwNewObject( GraphicsCanvas, 0 );
  XRect          updateRect = {{ 0, 0 }, { 0, 0 }};

#if EW_USE_DOUBLE_BUFFER

  bitmap = EwBeginUpdate( aViewport );

  /* let's redraw the dirty area of the screen. Cover the returned bitmap
     objects within a canvas, so Mosaic can draw to it. */
  if ( bitmap && canvas )
  {
    GraphicsCanvas__AttachBitmap( canvas, (XUInt32)bitmap );
    updateRect = CoreRoot__UpdateGE20( aApplication, canvas );
    GraphicsCanvas__DetachBitmap( canvas );
  }

  /* complete the update */
  if ( bitmap )
    EwEndUpdate( aViewport, updateRect );

#else

  int field = 0;

  /* start screen update */
  int regions = CoreRoot__BeginUpdate( aApplication );

  /* iterate through all fields (horizontal stripes) of the display */
  while ( regions && ( field < NUMBER_OF_FIELDS ))
  {
    /* determine rectangular area of current field */
    #if EW_SURFACE_ROTATION == 0
      updateRect = EwNewRect( 0, field * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS,
        FRAME_BUFFER_WIDTH, ( field + 1 ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS );
    #endif

    #if EW_SURFACE_ROTATION == 90
      updateRect = EwNewRect( field * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS, 0,
        ( field + 1 ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS, FRAME_BUFFER_WIDTH );
    #endif

    #if EW_SURFACE_ROTATION == 180
      updateRect = EwNewRect( 0, FRAME_BUFFER_HEIGHT - ( field + 1 ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS,
        FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT - field * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS );
    #endif

    #if EW_SURFACE_ROTATION == 270
      updateRect = EwNewRect( FRAME_BUFFER_HEIGHT - ( field + 1 ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS,
        0, FRAME_BUFFER_HEIGHT - field * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS, FRAME_BUFFER_WIDTH );
    #endif

    /* next field */
    field++;

    /* sync on start line of next field to ensure save drawing operation */
    EwBspSyncOnDisplayLine(( field % NUMBER_OF_FIELDS ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS );

    /* draw area into current field */
    bitmap = EwBeginUpdateArea( aViewport, updateRect );
    GraphicsCanvas__AttachBitmap( canvas, (XUInt32)bitmap );
    CoreRoot__UpdateCanvas( aApplication, canvas, updateRect.Point1 );
    GraphicsCanvas__DetachBitmap( canvas );
    EwEndUpdate( aViewport, updateRect );
  }
  CoreRoot__EndUpdate( aApplication );

#endif
}


/*******************************************************************************
* FUNCTION:
*   EwGetKeyCommand
*
* DESCRIPTION:
*   The function EwGetKeyCommand reads the next key code from the console and
*   translates it into an Embedded Wizard key code. The mapping between the key
*   code from the console and the resulting Embedded Wizard key code can be
*   adapted to the needs of your application.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns the next EmWi key code or CoreKeyCodeNoKey if no key code available.
*
*******************************************************************************/
static XEnum EwGetKeyCommand( void )
{
  #ifdef USE_TERMINAL_INPUT
    switch ( EwBspGetCharacter())
    {
      case 0x65 : EwPrint("Key 'Exit' pressed\n");  return CoreKeyCodeExit;
      case 0x38 : EwPrint("Key 'Up' pressed\n");    return CoreKeyCodeUp;
      case 0x32 : EwPrint("Key 'Down' pressed\n");  return CoreKeyCodeDown;
      case 0x36 : EwPrint("Key 'Right' pressed\n"); return CoreKeyCodeRight;
      case 0x34 : EwPrint("Key 'Left' pressed\n");  return CoreKeyCodeLeft;
      case 0x35 : EwPrint("Key 'OK' pressed\n");    return CoreKeyCodeOk;
      case 0x6D : EwPrint("Key 'Menu' pressed\n");  return CoreKeyCodeMenu;
      case 0x70 : EwPrint("Key 'Power' pressed\n"); return CoreKeyCodePower;
    }
  #endif
  return CoreKeyCodeNoKey;
}


/*******************************************************************************
* FUNCTION:
*   EwPrintSystemInfo
*
* DESCRIPTION:
*   This function prints system and configuration information - very helpful in
*   case of any support issues.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwPrintSystemInfo( void )
{
  #define PLATFORM_STRING "IMXRT1060-EVK"

  #if ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888 )
    #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "RGBA8888"
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB888 )
    #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "RGB888"
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA4444 )
    #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "RGBA4444"
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB565 )
    #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "RGB565"
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_Index8 )
    #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "Index8"
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_LumA44 )
    #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "LumA44"
  #endif

  #define EW_STRINGIZE( aArg )      EW_STRINGIZE_ARG( aArg )
  #define EW_STRINGIZE_ARG( aArg )  #aArg

  #if defined __ICCARM__
    #define TOOLCHAIN_STRING "IAR Embedded Workbench"
    #define COMPILER_VERSION_STRING EW_STRINGIZE(__VER__)
  #elif defined __CC_ARM || (__ARMCC_VERSION >= 6000000)
    #define TOOLCHAIN_STRING "Keil MDK"
    #define COMPILER_VERSION_STRING EW_STRINGIZE(__ARMCC_VERSION)
  #elif defined __GNUC__
    #define TOOLCHAIN_STRING "GCC"
    #define COMPILER_VERSION_STRING EW_STRINGIZE(__GNUC__) "." \
      EW_STRINGIZE(__GNUC_MINOR__) "." EW_STRINGIZE(__GNUC_PATCHLEVEL__)
  #else
    #define TOOLCHAIN_STRING "unknown"
  #endif

  #ifdef EW_DONT_USE_PATH_FUNCTIONS
    #define VECTOR_GRAPHICS_SUPPORT_STRING "disabled"
  #else
    #define VECTOR_GRAPHICS_SUPPORT_STRING "enabled"
  #endif

  #ifdef EW_DONT_USE_WARP_FUNCTIONS
    #define WARP_FUNCTION_SUPPORT_STRING "disabled"
  #else
    #define WARP_FUNCTION_SUPPORT_STRING "enabled"
  #endif

  #ifdef EW_DONT_USE_INDEX8_SURFACES
    #define INDEX8_SURFACE_SUPPORT_STRING "disabled"
  #else
    #define INDEX8_SURFACE_SUPPORT_STRING "enabled"
  #endif

  #ifdef EW_DONT_USE_RGB565_SURFACES
    #define RGB565_SURFACE_SUPPORT_STRING "disabled"
  #else
    #define RGB565_SURFACE_SUPPORT_STRING "enabled"
  #endif

  #ifdef EW_DONT_USE_BIDI_FUNCTIONS
    #define BIDI_TEXT_SUPPORT_STRING "disabled"
  #else
    #define BIDI_TEXT_SUPPORT_STRING "enabled"
  #endif

  #if ( EW_USE_PXP_GRAPHICS_ACCELERATOR == 1 )
    #define GRAPHICS_ACCELERATOR_STRING "PXP"
  #else
    #define GRAPHICS_ACCELERATOR_STRING "none"
  #endif

  #if ( EW_USE_FREE_RTOS == 1 )
    #define OPERATING_SYSTEM_STRING "FreeRTOS"
  #else
    #define OPERATING_SYSTEM_STRING "none"
  #endif

  #define EXTERNAL_FLASH_STRING "QSPI Flash"


  EwPrint( "---------------------------------------------\n" );
  EwPrint( "Target system                                %s      \n", PLATFORM_STRING );
  EwPrint( "Color format                                 %s      \n", EW_FRAME_BUFFER_COLOR_FORMAT_STRING );
  EwPrint( "MemoryPool address                           0x%08X  \n", MEMORY_POOL_ADDR );
  EwPrint( "MemoryPool size                              %u bytes\n", MEMORY_POOL_SIZE );
  #if ( EW_USE_SCRATCHPAD_BUFFER == 1 )
  EwPrint( "Scratch-pad buffer address                   0x%08X  \n", SCRATCHPAD_BUFFER_ADDR );
  EwPrint( "Scratch-pad buffer size                      %u pixel\n", SCRATCHPAD_BUFFER_SIZE * FRAME_BUFFER_DEPTH );
  #else
  EwPrint( "Framebuffer address                          0x%08X  \n", FRAME_BUFFER_ADDR );
  #endif
  #if ( EW_USE_DOUBLE_BUFFER == 1 )
  EwPrint( "Doublebuffer address                         0x%08X  \n", DOUBLE_BUFFER_ADDR );
  #endif
  EwPrint( "Framebuffer size                             %u x %u \n", FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT );
  EwPrint( "EwScreeenSize                                %d x %d \n", EwScreenSize.X, EwScreenSize.Y );
  EwPrint( "Graphics accelerator                         %s      \n", GRAPHICS_ACCELERATOR_STRING );
  EwPrint( "Vector graphics support                      %s      \n", VECTOR_GRAPHICS_SUPPORT_STRING );
  EwPrint( "Warp function support                        %s      \n", WARP_FUNCTION_SUPPORT_STRING );
  EwPrint( "Index8 bitmap resource format                %s      \n", INDEX8_SURFACE_SUPPORT_STRING );
  EwPrint( "RGB565 bitmap resource format                %s      \n", RGB565_SURFACE_SUPPORT_STRING );
  EwPrint( "Bidirectional text support                   %s      \n", BIDI_TEXT_SUPPORT_STRING );
  EwPrint( "Operating system                             %s      \n", OPERATING_SYSTEM_STRING );
  EwPrint( "External flash device                        %s      \n", EXTERNAL_FLASH_STRING );
  #ifdef EW_BITMAP_PIXEL_SECTION_NAME
  EwPrint( "Linker section for bitmap pixel data         %s      \n", EW_STRINGIZE( EW_BITMAP_PIXEL_SECTION_NAME ));
  #endif
  #ifdef EW_FONT_PIXEL_SECTION_NAME
  EwPrint( "Linker section for font pixel data           %s      \n", EW_STRINGIZE( EW_FONT_PIXEL_SECTION_NAME ));
  #endif
  EwPrint( "Toolchain                                    %s      \n", TOOLCHAIN_STRING );
  #ifdef COMPILER_VERSION_STRING
  EwPrint( "C-Compiler version                           %s      \n", COMPILER_VERSION_STRING );
  #endif
  EwPrint( "Build date and time                          %s, %s  \n", __DATE__, __TIME__ );
  EwPrint( "Runtime Environment (RTE) version            %u.%02u \n", EW_RTE_VERSION >> 16, EW_RTE_VERSION & 0xFF );
  EwPrint( "Graphics Engine (GFX) version                %u.%02u \n", EW_GFX_VERSION >> 16, EW_GFX_VERSION & 0xFF );
  EwPrint( "Max surface cache size                       %u bytes\n", EW_MAX_SURFACE_CACHE_SIZE );
  EwPrint( "Glyph cache size                             %u x %u \n", EW_MAX_GLYPH_SURFACE_WIDTH, EW_MAX_GLYPH_SURFACE_HEIGHT );
  EwPrint( "Max issue tasks                              %u      \n", EW_MAX_ISSUE_TASKS );
  EwPrint( "Surface rotation                             %u      \n", EW_SURFACE_ROTATION );
  EwPrint( "---------------------------------------------\n" );
}


/* msy, mli */
