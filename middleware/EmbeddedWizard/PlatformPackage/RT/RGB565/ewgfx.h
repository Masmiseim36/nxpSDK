/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software and related documentation ("Software") are intellectual
* property owned by TARA Systems and are copyright of TARA Systems.
* Any modification, copying, reproduction or redistribution of the Software in
* whole or in part by any means not in accordance with the End-User License
* Agreement for Embedded Wizard is expressly prohibited. The removal of this
* preamble is expressly prohibited.
*
********************************************************************************
*
* DESCRIPTION:
*   Embedded Wizard Graphics Engine provides a platform independent environment
*   for the execution of graphical operations as it is required by applications
*   developped with Embedded Wizard. Following features are available:
*
*   > 5 bitmap formats: Index8, Alpha8, Native, RGB565 and Screen. The Graphics
*     Engine provides a rich set of drawing operations with bitmaps as a source
*     and destination. Bitmaps can be copied, alpha-blended or even perspective
*     correct projected as it is necessary for 3D effects.
*
*     The denomination 'Native' refers to the bitmap format supported natively
*     by the particular target system (e.g. 32 bit RGBA8888). Native bitmaps can
*     serve as the destination and/or as the source for a drawing operation.
*
*     Index8 bitmaps provide a generic format of an 8 bit / pixel bitmap with a
*     color look-up table CLUT. When drawing an Index8 bitmap, its pixel values
*     will be translates through the CLUT in the particular native color format.
*     An advantage of Index8 bitmaps is the posibility to change its colors by
*     modifying the color entries within its CLUT.
*
*     Alpha8 bitmaps provide a generic format of an 8 bit / pixel bitmap with a
*     single alpha (A) channel - no color (RGB) information is stored there.
*     Alpha8 bitmaps are suitable for drawing patterns modulated by externally
*     specified colors. For example: text output.
*
*     RGB565 bitmaps exist optionally on platforms able to handle them. Unlike
*     other formats, RGB565 does not store alpha-channel. It contains pure color
*     information. The support of RGB565 depends on the used platform package.
*     Bitmaps in the RGB565 format can be used exclusively as source only.
*
*     The denomination 'Screen' refers to the format of the frame buffer in the
*     particular target system (e.g. 32 bit RGBA8888). Screen bitmaps can serve
*     exclusively as the destination for a drawing operation.
*
*     In contrast to the Native bitmap format, Index8 and Alpha8 can serve as
*     source only. It is not possible to draw to an Index8 or Alpha8 bitmap.
*
*     An additional feature are 'frames'. Except the 'Screen' bitmap format,
*     each bitmap can consist of several contents, so called frames. When
*     working with the bitmap, the desired frame needs to be selected. Within
*     a bitmap all frames always have the same size. The 'Screen' bitmap format
*     allows a single frame only.
*
*   > Rich set of drawing operations: paths, filled polygons, bitmap copying,
*     3D perspective correct projection and text output. All operations can be
*     performed with or without the alpha-blending. Alpha-blending allows fine
*     fading effects when several graphical objects are drawn one upon an other.
*
*     Additionally all drawn pixel can be modulated by color or opacity values
*     based on linear gradient calculation. In this manner, copied bitmap areas
*     can, for example, be faded out on its edges. A gradient can be vertical,
*     horizontal or both. In the last case for each corner of the gradient a
*     different color value can be set, so 4 colors (or opacity values) can be
*     mixed in the gradient area.
*
*     With 'Warp' operations, Graphics Engine provides 3D perspective correct
*     projection of bitmaps. Beside the 3D effect, this functionality allows 2D
*     scaling and rotating of bitmap areas. For best possible quality, all warp
*     operations are performed with increased sub-pixel precision. Additionally
*     bi-linear filters reduce artefacts, when bitmaps are warped.
*
*   > Resource management: bitmap and fonts can be loaded at the runtime into
*     the Graphics Engine with the objective to display images and output text.
*     The loaded resources remain in the Graphics Engine as long as they are
*     needed. Unused resources also remain in the Graphics Engine as long as
*     there is sufficient memory available. This kind of cache functionality
*     reduces the effort for repeated reload of frequently used resources.
*
*   > Optimization: before executing drawing operations, Graphics Engine can
*     analyse them in order to optimize the stream of drawing instructions. In
*     this manner superfluous drawing operations are detected and eliminated.
*     If possible, the order of operations is changed in order to reduce the
*     synchronization overhead between the hardware accelerator and software
*     drawing routines.
*
*   > Platform independency: The entire Graphics Engine is platform independent.
*     It handles the complex drawing operations and generates from them a stream
*     of primitive instructions destined for the particular graphics subsystem.
*
*     Graphics functionality, which is not supported by the underlying graphics
*     subsystem will be emulated by Graphics Engine's own software driver. In
*     this manner the entire range of drawing operations is always guaranteed
*     for all Embedded Wizard generated GUI applications.
*
*     The integration with the underlying (native) graphics subsystem will take
*     place through a well defined interface of external functions. Except few
*     of them, the most of these functions are optional - if a function is not
*     implemented, the corresponding functionality will be emulated as described
*     above.
*
*     Due to the wide range of possible graphical operations and the modes, the
*     interface to the underlying graphics subsystem is scalable. For example,
*     if the graphics hardware supports only one of the different copy modes,
*     this special mode can be redirected to the hardware while all other modes
*     continue using the software emulation.
*
*     Note the possible performance limits, when software emulation is used.
*
*   This is the public header file of the Embedded Wizard Graphics Engine
*   EWGFX. It declares the top level API of the engine:
*
*   > Bitmap and font management. Creation and destroying of bitmaps, loading
*     of bitmap and font resources.
*
*   > Drawing operations. Functions to perform all kinds of drawing operations
*     like drawing lines, attributed text, bitmaps, etc.
*
*   > Creation and management of path data.
*
*   > Text, flow text and attributed text processing and size calculation.
*     The attributed text provides a powerful way to layout and output strings
*     with different fonts, embedded images, etc. Multiline text can be drawn
*     with automatic line wraps, soft-hyphen, etc. Support for Bidirectional
*     text (e.g. Arabic, Hebrew) is included.
*
*******************************************************************************/

#ifndef EWGFX_H
#define EWGFX_H


/* Include platform specific configuration file */
#include <ewconfig.h>
#include "ewgfxdriver.h"
#include "ewextgfx.h"
#include "ewgfxdefs.h"

/* Include platform dependent bitmap and font resource provider declarations */
#include "ewextbmp.h"
#include "ewextfnt.h"
#include "ewgfxcore.h"


#ifdef __cplusplus
  extern "C"
  {
#endif

/* The current version of the Graphics Engine. */
#define EW_GFX_VERSION 0x0009001E


/******************************************************************************
* STATUS VARIABLE:
*   EwFullScreenUpdate
*   EwFullOffScreenBufferUpdate
*   EwPreserveFramebufferContent
*
* DESCRIPTION:
*   The following variables reflect the current screen update mode as it is
*   predetermined by the used graphics subsystem. The variables are used
*   internally by the Mosaic framework only. You never should evaluate or
*   modify these variables directly.
*
******************************************************************************/
extern int EwFullScreenUpdate;
extern int EwFullOffScreenBufferUpdate;
extern int EwPreserveFramebufferContent;


/******************************************************************************
* CONFIG VARIABLE:
*   EwMaxSurfaceCacheSize
*   EwMaxGlyphSurfaceWidth
*   EwMaxGlyphSurfaceHeight
*   EwMaxIssueTasks
*   EwLazyLoadBitmaps
*   EwLazyLoadBitmapsIfAnimatedOnly
*   EwDiscardBitmaps
*   EwDiscardBitmapsIfAnimatedOnly
*   EwDiscardBitmapsIfNotUsedInCurrentUpdate
*
* DESCRIPTION:
*   The following variables exist for configuration purpose for diverse caches
*   used by the Graphics Engine. Modifying the variables is possible only just
*   before the Graphics Engine is initialized.
*
******************************************************************************/
extern int EwMaxSurfaceCacheSize;   /* = EW_MAX_SURFACE_CACHE_SIZE macro */
extern int EwMaxGlyphSurfaceWidth;  /* = EW_MAX_GLYPH_SURFACE_WIDTH macro */
extern int EwMaxGlyphSurfaceHeight; /* = EW_MAX_GLYPH_SURFACE_HEIGHT macro */
extern int EwMaxIssueTasks;         /* = EW_MAX_ISSUE_TASKS macro */
extern int EwLazyLoadBitmaps;       /* = EW_LAZY_LOAD_BITMAPS */
extern int EwDiscardBitmaps;        /* = EW_DISCARD_BITMAPS */
extern int EwLazyLoadBitmapsIfAnimatedOnly; /* = EW_LAZY_LOAD_BITMAPS_IF_ANIMATED_ONLY */
extern int EwDiscardBitmapsIfAnimatedOnly;  /* = EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY */
extern int EwDiscardBitmapsIfNotUsedInCurrentUpdate; /* = EW_DISCARD_BITMAPS_IF_NOT_USED_IN_CURRENT_UPDATE */
extern int EwDiscardBitmapsIfNotUsedInRecentUpdates; /* = EW_DISCARD_BITMAPS_IF_NOT_USED_IN_RECENT_UPDATES */


/*******************************************************************************
* MACRO:
*   EW_PATH_CAP_XXX
*   EW_PATH_JOIN_XXX
*
* DESCRIPTION:
*   The following enumerations define the possible styles used when stroking
*   paths. The enumeration EW_PATH_CAP_XXX controls the appearance of the ends
*   of a stroken path. The enumeration EW_PATH_JOIN_XXX controls how the join
*   points between two edges of the path should be connected.
*
* ELEMENTS:
*   EW_PATH_CAP_START_FLAT     - The begin of every path is squared off.
*   EW_PATH_CAP_START_SQUARE   - The begin of everry path is extended by the
*     half of the line thickness and then squared off.
*   EW_PATH_CAP_START_TRIANGLE - Every path does begin with a triangle.
*   EW_PATH_CAP_START_ROUND    - The begin of every path is rounded.
*
*   EW_PATH_CAP_END_FLAT       - The end of every path is squared off.
*   EW_PATH_CAP_END_SQUARE     - The end of every path end is extended by the
*     half of the line thickness and then squared off.
*   EW_PATH_CAP_END_TRIANGLE   - Every path does end with a triangle.
*   EW_PATH_CAP_END_ROUND      - The end of every path is rounded.
*
*   EW_PATH_CAP_FLAT           - The begin and the end of every path are squared
*     off at their endpoints.
*   EW_PATH_CAP_SQUARE         - The begin and the end of every path are extended
*     by the half of the line thickness and then squared off.
*   EW_PATH_CAP_TRIANGLE       - The paths start and end with a triangle.
*   EW_PATH_CAP_ROUND          - The begin and the end of every path are rounded.
*
*   EW_PATH_JOIN_BEVEL         - The path segments are joined by connecting their
*     outside edges with a single straight path segment.
*   EW_PATH_JOIN_MITER         - The path segments are joined by extending their
*     outside edges until they intersect.
*   EW_PATH_JOIN_ROUND         - The path corners are rounded.
*
*******************************************************************************/
#define EW_PATH_CAP_START_FLAT     0x00000000
#define EW_PATH_CAP_START_SQUARE   0x00000001
#define EW_PATH_CAP_START_TRIANGLE 0x00000002
#define EW_PATH_CAP_START_ROUND    0x00000003
#define EW_PATH_CAP_END_FLAT       0x00000000
#define EW_PATH_CAP_END_SQUARE     0x00000100
#define EW_PATH_CAP_END_TRIANGLE   0x00000200
#define EW_PATH_CAP_END_ROUND      0x00000300
#define EW_PATH_CAP_FLAT           0x00000000
#define EW_PATH_CAP_SQUARE         0x00000101
#define EW_PATH_CAP_TRIANGLE       0x00000202
#define EW_PATH_CAP_ROUND          0x00000303
#define EW_PATH_JOIN_BEVEL         0x00000000
#define EW_PATH_JOIN_MITER         0x00010000
#define EW_PATH_JOIN_ROUND         0x00020000


/*******************************************************************************
* MACRO:
*   EW_SVG_PARSER_XXX
*
* DESCRIPTION:
*   The following enumerations define the possible status values after parsing
*   an SVG string.
*
* ELEMENTS:
*   EW_SVG_PARSER_SUCCESS          - SVG string could be processed successfully.
*   EW_SVG_PARSER_EMPTY_STRING     - No SVG string content to parse.
*   EW_SVG_PARSER_SYNTAX_ERROR     - Parsing has been aborted due to a syntax
*     error in the SVG string.
*   EW_SVG_PARSER_INVALID_ARGUMENT - Parsing has been aborted due to a found
*     invalid argument in the SVG string (one parameter or combination of the
*     parameters is not correct).
*   EW_SVG_PARSER_OUT_OF_MEMORY    - The SVG string is too complex.
*
*******************************************************************************/
#define EW_SVG_PARSER_SUCCESS           0
#define EW_SVG_PARSER_EMPTY_STRING      1
#define EW_SVG_PARSER_SYNTAX_ERROR      2
#define EW_SVG_PARSER_INVALID_ARGUMENT  3
#define EW_SVG_PARSER_OUT_OF_MEMORY     4


/******************************************************************************
* TYPE:
*   XBitmapFrame
*
* DESCRIPTION:
*   The structure XBitmapFrame describes the location and an optional opaque
*   area of a single bitmap frame.
*
* ELEMENTS:
*   Surface    - Reference to the low-level surface, where the frame pixel are
*     stored.
*   Origin     - Frame origin relative to the top-left corner of the surface.
*   OpaqueRect - Optional opaque area relative to the top-left corner of the
*     frame. Empty rectangle, if no opaque area is available in the bitmap
*     frame.
*
******************************************************************************/
typedef struct
{
  struct XSurface*  Surface;
  XPoint            Origin;
  XRect             OpaqueRect;
} XBitmapFrame;


/******************************************************************************
* TYPE:
*   XBitmap
*
* DESCRIPTION:
*   The structure XBitmap describes the attributes of a bitmap. XBitmap serves
*   as the public and abstract representation of a surface.
*
* ELEMENTS:
*   Format         - Pixel format of the bitmap. (See EW_PIXEL_FORMAT_XXX).
*   FrameSize      - Size of a single frame within the bitmap.
*   FrameDelay     - Delay in milliseconds for animated bitmaps. If no animation
*     is specified for the bitmap, the value == 0.
*   NoOfFrames     - Number of frames, this bitmap resources consists of.
*   NoOfVirtFrames - Total number of frames, including all duplicates.
*   Mapping        - If not NULL refers to a list with indices to map between
*     virtual frames numbers and the numbers of really existing frames.
*   Resource       - Pointer to the original resource containing the pixel data
*     if the bitmap has been loaded from a resource.
*   Frames         - Pointer to an array containing the attributes of all
*     frames. At least one frame is available. Please note, it is possible for
*     a bitmap to appear as containing more frames than existing in this array.
*     In such case the existing frames are shared by several 'virtual' frames.
*     This is used mainly in animation or short video sequences.
*   NoOfSurfaces   - Number of low-level surfaces to store the pixel data of
*     the bitmap frames.
*   Surfaces       - Pointer to an array containing references to low-level
*     surfaces with the bitmap pixel data.
*
******************************************************************************/
typedef struct
{
  XInt32                Format;
  XPoint                FrameSize;
  XInt32                FrameDelay;
  XInt32                NoOfFrames;
  XInt32                NoOfVirtFrames;
  XUInt16*              Mapping;
  const struct XBmpRes* Resource;
  XBitmapFrame*         Frames;
  XInt32                NoOfSurfaces;
  struct XSurface**     Surfaces;
} XBitmap;


/******************************************************************************
* TYPE:
*   XFont
*
* DESCRIPTION:
*   The structure XFont describes the attributes of a loaded font resource.
*   XFont serves as the public and abstract representation of a font.
*
* ELEMENTS:
*   Ascent   - Ascent of the font (means the area above the baseline).
*   Descent  - Descent of the font (means the area below the baseline).
*   Leading  - Leading of the font (additional distance between two rows).
*   DefChar  - Default character code.
*   Tag      - Unique identification of the font. Used to find glyphs within
*     the glyph cache.
*   Handle   - Reference to the opened font resource. Used to find and load
*     glyphs.
*
******************************************************************************/
typedef struct
{
  XInt32            Ascent;
  XInt32            Descent;
  XInt32            Leading;
  XChar             DefChar;
  char              Reserved[2];
  unsigned long     Tag;
  unsigned long     Handle;
} XFont;


/******************************************************************************
* TYPE:
*   XGlyphMetrics
*
* DESCRIPTION:
*   The structure XGlyphMetrics covers the metrics of a single glyph.
*
* ELEMENTS:
*   Size    - Size of the glyph in pixel (means the size of the ink-box).
*   Origin  - Upper left corner of the glyph, relative to the current printing
*     position on the baseline.
*   Advance - Advance in horizontal direction added to the printing position.
*
*******************************************************************************/
typedef struct
{
  XPoint            Size;
  XPoint            Origin;
  XInt32            Advance;
} XGlyphMetrics;


/*******************************************************************************
* TYPE:
*   XBitmapLock
*
* DESCRIPTION:
*   The XBitmapLock structure provides adresses to color planes of a previously
*   locked bitmap. To lock a bitmap, use the function EwLockBitmap(). See the
*   description of this function for more details about locking and the access
*   to the bitmap memory.
*
*   Note, the pixel storage depends on the pixel format. For more details see
*   the 'ewextpxl_XXX.h' file (XXX stands for your target pixel format. e.g.
*   'ewextpxl_RGBA8888.h').
*
* ELEMENTS:
*   Pixel1  - First color plane pointer. The pointer refers to the first pixel
*     of the locked bitmap area.
*   Pitch1X - Distance between two pixel columns of the first color plane in
*     bytes.
*   Pitch1Y - Distance between two pixel rows of the first color plane in
*     bytes.
*   Pixel2  - Pointer to the second (optional/if any) color plane or zero.
*     If not zero, the pointer also refers to the first pixel of the locked
*     bitmap area.
*   Pitch2X - Distance between two pixel columns of the second color plane
*     in bytes or zero if not applicable.
*   Pitch2Y - Distance between two pixel rows of the second color plane in
*     bytes or zero if not applicable.
*
*******************************************************************************/
typedef struct
{
  void*             Pixel1;
  int               Pitch1X;
  int               Pitch1Y;
  void*             Pixel2;
  int               Pitch2X;
  int               Pitch2Y;
} XBitmapLock;


/*******************************************************************************
* TYPE:
*   XGlyphLock
*
* DESCRIPTION:
*   The XGlyphLock structure provides adress to the pixel information of a
*   previously locked glyph. To lock a glyph, use the function EwLockGlyph().
*   See the description of this function for more details about locking and the
*   access to the glyph memory.
*
*   Note, glyphs are stored in the Alpha8 pixel format -> one byte per pixel.
*   The pixel value 0 determines transparent pixel. The 255 full opaque pixel.
*   Values 1 .. 254 determine the semitransparent pixel.
*
* ELEMENTS:
*   Pixel   - Pointer to the glyph pixel memory. The pointer refers to the first
*     pixel of the locked glyph.
*   PitchX  - Distance between two pixel columns. Is always 1 byte.
*   PitchY  - Distance between two pixel rows in bytes.
*
*******************************************************************************/
typedef struct
{
  unsigned char*    Pixel;
  int               PitchX;
  int               PitchY;
} XGlyphLock;


/* Forward declaration */
typedef struct XViewport XViewport;


/*******************************************************************************
* TYPE:
*   XViewportProc
*
* DESCRIPTION:
*  The following type declares a prototype of a user defined function, which
*  can be passed as callback during the initialization of a viewport.
*
*  This callback function will then be called each time the viewport update
*  cycle terminates. In this manner some kind of viewport specific completion
*  can be performed.
*
* ARGUMENTS:
*   aViewport - Viewport which is performing the screen update.
*   aHandle   - Reference to the real viewport, the framebuffer, display, etc.
*    This value depends on the underlying graphics subsystem.
*   aDisplay1,
*   aDisplay2,
*   aDisplay3 - Platform dependent values, where already existing framebuffer
*     or drawing context, etc. are stored. These values are passed during the
*     initialization of the viewport (see EwInitViewport()).
*   aArea     - Area modified by the update relative to the top-left corner of
*     the viewport.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XViewportProc)
(
  XViewport*        aViewport,
  unsigned long     aHandle,
  void*             aDisplay1,
  void*             aDisplay2,
  void*             aDisplay3,
  XRect             aArea
);


/*******************************************************************************
* TYPE:
*   XViewport
*
* DESCRIPTION:
*   The structure XViewport stores the attributes of a viewport. A viewport
*   provides a kind of abstraction of the physical framebuffer or display
*   where graphical outputs are drawn.
*
* ELEMENTS:
*   Size       - Size of the viewport in pixel. This is the area where graphics
*     outputs will occur.
*   Bitmap     - Internal bitmap object to cover the access to the framebuffer
*     or to the display where the graphical outputs are drawn.
*   Frames     - The single entry of the bitmap frames array.
*   Surfaces   - The single entry of the bitmap surfaces array.
*   Surface    - Internal surface object to cover the access to the framebuffer
*     or to the display where the graphical outputs are drawn.
*   UpdateArea - The area relative to the top-left corner of the viewport for
*     the currently active update cycle.
*   Valid      - Flag indicating whether the screen update cycle is active. The
*     screen update begins with EwBeginUpdate() or EwBeginUpdateArea() and ends
*     with EwEndUpdate(). As long as Valid != 0, the internal bitmap and surface
*     objects are connected to the framebuffer and thay may be used as the
*     destination in all drawing operations.
*   Proc       - Optional callback function to call when viewport update has
*     been done. The callback function can then perform some particular tasks
*     to complete the screen update, etc. If 0, no callback is called.
*   Display1,
*   Display2,
*   Display3   - Platform dependent values, where already existing framebuffer
*     or drawing context, etc. are stored. These values are passed during the
*     initialization of the viewport (see EwInitViewport()).
*   Handle     - Reference to the real viewport, the framebuffer, display, etc.
*    This value depends on the underlying graphics subsystem.
*
*******************************************************************************/
struct XViewport
{
  XPoint            Size;
  XBitmap           Bitmap;
  XBitmapFrame      Frames;
  XSurface*         Surfaces;
  XSurface          Surface;
  XRect             UpdateArea;
  XBool             Valid;
  char              Reserved[3];
  XViewportProc     Proc;
  void*             Display1;
  void*             Display2;
  void*             Display3;
  unsigned long     Handle;
};


/*******************************************************************************
* TYPE:
*   XAttrSet
*
* DESCRIPTION:
*   The structure XAttrSet stores a set of fonts, bitmaps and colors used while
*   parsing and drawing of attributed strings. The set is a container with a
*   fixed, predefined number of resource and color entries. The desired size of
*   the set should be specified in the function call EwCreateAttrSet().
*
* ELEMENTS:
*   NoOfFonts   - Max. number of fonts.
*   NoOfBitmaps - Max. number of bitmaps.
*   NoOfColors  - Max. number of colors.
*   Fonts       - Pointer to the list of fonts.
*   Bitmaps     - Pointer to the list of bitmaps.
*   Colors      - Pointer to the list of colors.
*
*******************************************************************************/
typedef struct
{
  XInt32            NoOfFonts;
  XInt32            NoOfBitmaps;
  XInt32            NoOfColors;
  XFont**           Fonts;
  XBitmap**         Bitmaps;
  XColor*           Colors;
} XAttrSet;


/*******************************************************************************
* TYPE:
*   XAttrLink
*
* DESCRIPTION:
*   The structure XAttrLink describes a single link stored within an already
*   parsed, preprocessed attributed string.
*
* ELEMENTS:
*   Name        - The name of the link as zero terminated string.
*   StartPtr,
*   EndPtr      - Pointers to the statements within the attributed string
*     identifying the position where the first region of the affected link
*     begins and the last region ends.
*   NoOfRegions - Number of rectangular areas, the link consists of.
*   X, Y        - Offset to the left/top origin of the group statement
*     containing the link.
*
*******************************************************************************/
typedef struct
{
  XChar*            Name;
  unsigned short*   StartPtr;
  unsigned short*   EndPtr;
  XInt32            NoOfRegions;
  XInt32            X;
  XInt32            Y;
} XAttrLink;


/*******************************************************************************
* TYPE:
*   XAttrString
*
* DESCRIPTION:
*   The structure XAttrString is used to store the content of an already parsed,
*   preprocessed attributed string. It consists of a lot of drawing statements,
*   which describe how the attributed string should be drawn on the screen.
*   These drawing statements are for internal use only. They are binary coded
*   and optimized for fast drawing operations.
*
*   Before an attributed string can be drawn on the screen, it has to be parsed
*   (converted) into this internal attributed string format. This is done by the
*   function EwParseAttrString().
*
* ELEMENTS:
*   Size      - Length of the parsed, preprocessed attributed string in words.
*   Data      - Pointer to the first statement within the attributed string.
*   IsRTL     - != 0 if the first found paragraph in the attributed string has
*     the RTL base direction. Otherwise it is 0.
*   NoOfLinks - Number of links defined within the attributed string.
*   NamesArea - Size of the area containing the link names in characters.
*   Links     - Pointer to a table with the links.
*
*******************************************************************************/
typedef struct
{
  XInt32            Size;
  unsigned short*   Data;
  XBool             IsRTL;
  XInt32            NoOfLinks;
  XInt32            NamesArea;
  XAttrLink*        Links;
} XAttrString;


/*******************************************************************************
* TYPE:
*   XSubPath
*
* DESCRIPTION:
*   The structure XSubPath is used to store the coordinates of a sub-path used
*   in the XPath structure. In this manner several sub-paths can be managed from
*   one and the same XPath, every treated individually when stroking or filling
*   the path area.
*
*   The implementation assumes, that the structure is immediately followed by an
*   array of 'float' values storing the X, Y coordinate pairs of the sub-path.
*   The first pair is available in the member 'Data'.
*
* ELEMENTS:
*   Capacity  - The max. number of edges this sub-path can accomodate in 'Data'
*     without the necesity to reallocate it.
*   NoOfEdges - The number of edges stored already in 'Data'.
*   HasData   - If != 0, there are data stored in Data array.
*   IsOpened  - If != 0, the sub-path is opened,
*   IsClosed  - If != 0, the sub-path is closed. This means the first and the
*     last coordinate pair in 'Data' are equal and the path has been closed
*     by explicitly calling the function EwClosePath().
*   IsValid   - If != 0, the value 'Bounds' is up to date.
*   Bounds    - Rectangular area enclosing all corners of the sub-path.
*   Data      - The begin of an array containing the edge coordinates. The data
*     is stored pairwise as X and Y values. The number of coordinate pairs in
*     the array is equal ( NoOfEdges + 1 ).
*
*******************************************************************************/
typedef struct _XSubPath
{
  XInt32            Capacity;
  XInt32            NoOfEdges;
  XBool             HasData;
  XBool             IsOpened;
  XBool             IsClosed;
  XBool             IsValid;
  XRect             Bounds;
  float             Data[2];
} XSubPath;


/*******************************************************************************
* TYPE:
*   XPathMatrix
*
* DESCRIPTION:
*   The structure XPathMatrix is used to store the state of the path matrix in
*   a single chained list. Every time EwPushPathMatrix() is used, the actual
*   matrix of a path is copied in a new XPathMatrix object and stored within
*   the XPath.
*
* ELEMENTS:
*   Next   - Pointer to the next matrix stored on the stack.
*   Matrix - The stored content of the matrix.
*
*******************************************************************************/
typedef struct _XPathMatrix
{
  struct _XPathMatrix* Next;
  float                Matrix[6];
} XPathMatrix;


/*******************************************************************************
* TYPE:
*   XPath
*
* DESCRIPTION:
*   The structure XPath is used to store the path information used to fill and
*   stroke paths by using the functions EwFillPath() and EwStrokePath(). One
*   path can consist of any number of sub-paths. Every sub-path consists of one
*   or more edges (straight line segments) connected together. One path can be
*   considered as opened or closed.
*
* ELEMENTS:
*   MaxNoOfSubPaths - The max. number of sub-paths the path can manage.
*   NoOfOpenedPaths - Counter reflecting the number of sub-paths which are open.
*   NoOfClosedPaths - Counter reflecting the number of closed sub-paths.
*   NoOfEdges       - The number of edges stored totally in all sub-paths.
*   IsValid         - If != 0, the value 'Bounds' is up to date.
*   Bounds          - Rectangular area enclosing all corners of all sub-path.
*   Matrix          - 3x2 matrix describing transformations to apply on all
*     shapes added to the path.
*   MatrixStack     - Pointer to a list with stored path matrices.
*   SubPaths        - An array with pointers to the sub-paths managed by this
*     path.
*
*******************************************************************************/
typedef struct
{
  XInt32            MaxNoOfSubPaths;
  XInt32            NoOfOpenedPaths;
  XInt32            NoOfClosedPaths;
  XInt32            NoOfEdges;
  XBool             IsValid;
  char              Reserved[3];
  XRect             Bounds;
  float             Matrix[6];
  XPathMatrix*      MatrixStack;
  XSubPath*         SubPaths[1];
} XPath;


/*******************************************************************************
* FUNCTION:
*   EwConfigGraphicsEngine
*
* DESCRIPTION:
*   The pseudo function EwConfigGraphicsEngine() setups configuration variables
*   of the Graphics Engine according to macros found in 'ewconfig.h' file or in
*   the Make file.
*
*   This function has to be executed before EwInitGraphicsEngine().
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#define EwConfigGraphicsEngine()                                               \
  do                                                                           \
  {                                                                            \
    EwMaxSurfaceCacheSize                     = EW_MAX_SURFACE_CACHE_SIZE;     \
    EwMaxGlyphSurfaceWidth                    = EW_MAX_GLYPH_SURFACE_WIDTH;    \
    EwMaxGlyphSurfaceHeight                   = EW_MAX_GLYPH_SURFACE_HEIGHT;   \
    EwMaxIssueTasks                           = EW_MAX_ISSUE_TASKS;            \
    EwLazyLoadBitmaps                         = EW_LAZY_LOAD_BITMAPS;          \
    EwLazyLoadBitmapsIfAnimatedOnly           = EW_LAZY_LOAD_BITMAPS_IF_ANIMATED_ONLY; \
    EwDiscardBitmaps                          = EW_DISCARD_BITMAPS;            \
    EwDiscardBitmapsIfAnimatedOnly            = EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY; \
    EwDiscardBitmapsIfNotUsedInCurrentUpdate  = EW_DISCARD_BITMAPS_IF_NOT_USED_IN_CURRENT_UPDATE; \
    EwDiscardBitmapsIfNotUsedInRecentUpdates  = EW_DISCARD_BITMAPS_IF_NOT_USED_IN_RECENT_UPDATES; \
  }                                                                            \
  while ( 0 )


/*******************************************************************************
* FUNCTION:
*   EwInitGraphicsEngine
*
* DESCRIPTION:
*   The function EwInitGraphicsEngine() initializes the Graphics Engine.
*
* ARGUMENTS:
*   aArgs - Optional argument to pass to the init function of the underlying
*     platform specific adaptation layer.
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int EwInitGraphicsEngine
(
  void*             aArgs
);


/*******************************************************************************
* FUNCTION:
*   EwDoneGraphicsEngine
*
* DESCRIPTION:
*   The function EwDoneGraphicsEngine() deinitializes the Graphics Engine. If
*   necessary, still existing resources will be freed.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDoneGraphicsEngine
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwEnableGfxTaskTracing
*
* DESCRIPTION:
*   The function EwEnableGfxTaskTracing() activates or deactivates the internal
*   debugging mode of the Graphics Engine. With this mode enabled, the Graphics
*   Engine prints log messages for every performed drawing operation, resource
*   loading operation, etc.
*
*   Please note, in order to use the 'task tracing' the Graphics Engine has to
*   be rebuilt with the macro EW_SUPPORT_GFX_TASK_TRACING defined.
*
* ARGUMENTS:
*   aEnable - If this parameter is != 0, the mode is activated. To deactivate
*     it again, pass 0 in this parameter.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEnableGfxTaskTracing
(
  int               aEnable
);


/*******************************************************************************
* FUNCTION:
*   EwCreateBitmap
*
* DESCRIPTION:
*   The function EwCreateBitmap() creates a new bitmap with the given size and
*   color format. If no more used, the bitmap should be freed by calling the
*   function EwFreeBitmap().
*
* ARGUMENTS:
*   aFormat     - Color format of the bitmap. (See EW_PIXEL_FORMAT_XXX).
*   aFrameSize  - Size of a single bitmap frame.
*   aFrameDelay - Delay in milliseconds for animated bitmaps. If no animation
*     is specified for the bitmap, the value == 0.
*   aNoOfFrames - Number of frames to embed within the bitmap. At least one
*     frame is created.
*
* RETURN VALUE:
*   The function returns the pointer to the XBitmap structure, if the bitmap
*   was created successfully, otherwise 0.
*
*******************************************************************************/
XBitmap* EwCreateBitmap
(
  int               aFormat,
  XPoint            aFrameSize,
  XInt32            aFrameDelay,
  XInt32            aNoOfFrames
);


/*******************************************************************************
* FUNCTION:
*   EwLoadBitmap
*
* DESCRIPTION:
*   The function EwLoadBitmap() creates a new bitmap and loads it with the
*   content of the passed bitmap resource.
*
* ARGUMENTS:
*   aResource - Descriptor of the bitmap resource to load. The content of this
*     descriptor depends on the particular platform system.
*
* RETURN VALUE:
*   If sucessful, the function returns a new bitmap initialized with the content
*   of the resource. If failed, the function returns null.
*
*******************************************************************************/
XBitmap* EwLoadBitmap
(
  const struct XBmpRes* aResource
);


/*******************************************************************************
* FUNCTION:
*   EwFreeBitmap
*
* DESCRIPTION:
*   The function EwFreeBitmap() frees the given bitmap. Bitmaps may be created
*   by using EwCreateBitmap() or they may be loaded from a bitmap resource by
*   calling the function EwLoadBitmap().
*
* ARGUMENTS:
*   aBitmap - Bitmap to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeBitmap
(
  XBitmap*          aBitmap
);


/*******************************************************************************
* FUNCTION:
*   EwLockBitmap
*
* DESCRIPTION:
*   The function EwLockBitmap() provides a direct access to the pixel memory of
*   the given bitmap. The function returns a lock object containing pointers to
*   memory, where the caller can read/write the bitmap pixel values. Additional
*   pitch values also returned in the lock object allow the caller to calculate
*   the desired pixel addresses.
*
*   When finished the access cycle, the function EwUnlockBitmap() should be used
*   in order to release the lock, update the affected bitmap, flush CPU caches,
*   etc.
*
*   The memory returned by the function is not guaranteed to be the real video
*   memory of the bitmap. If necessary, the function will handle the access by
*   using a shadow memory area. Therefore to limit the effort of memory copy
*   operations, the desired bitmap area and the access mode should be specified.
*   Note the three parameters aArea, aRead and aWrite. Depending on the graphics
*   sub-system these three parameters may affect significantly the performance.
*
*   If there was not possible to lock the surface, or the desired access mode
*   is just not supported by the underlying graphics sub-system, the function
*   fails and returns zero. (e.g. OpenGL based sub-systems usually allow the
*   write access to bitmaps (textures) only. Read access may fail in this case)
*
*   Note, the pixel storage depends on the pixel format. For more details see
*   the 'ewextpxl_XXX.h' file (XXX stands for your target pixel format. e.g.
*   'ewextpxl_RGBA8888.h').
*
* ARGUMENTS:
*   aBitmap  - Bitmap to obtain the direct memory access.
*   aFrameNo - Frame within the destination bitmap affected by the access
*     operation.
*   aArea    - Area within the frame affected by the access operation (Relative
*     to the top-left corner of the bitmap frame). This is the area, the caller
*     intends to read/write the pixel data.
*   aRead    - Is != 0, if the caller intends to read the pixel information
*     from the bitmap memory. If == 0, the memory content may remain undefined
*     depending on the underlying graphics sub-system and its video-memory
*     management.
*   aWrite   - Is != 0, if the caller intends to overwrite the pixel information
*     within the bitmap memory. If == 0, any modifications within the memory may
*     remain ignored depending on the underlying graphics sub-system and its
*     video-memory management.
*
* RETURN VALUE:
*   If successful, the function returns a temporary bitmap lock object. If the
*   intended access is not possible or not supported by the underlying graphics
*   sub-system, 0 is returned.
*
*******************************************************************************/
XBitmapLock* EwLockBitmap
(
  XBitmap*          aBitmap,
  XInt32            aFrameNo,
  XRect             aArea,
  XBool             aRead,
  XBool             aWrite
);


/*******************************************************************************
* FUNCTION:
*   EwUnlockBitmap
*
* DESCRIPTION:
*   The function EwUnlockBitmap() provides a counterpart to EwLockBitmap(). When
*   called, the function releases the given lock and if necessary, transfers the
*   memory modifications back to the video memory of the bitmap and flushes the
*   CPU caches.
*
*   After calling this function, the aLock structure will become invalid.
*
* ARGUMENTS:
*   aLock - Lock object returned by the previous EwLockBitmap() call.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwUnlockBitmap
(
  XBitmapLock*      aLock
);


/*******************************************************************************
* FUNCTION:
*   EwModifyBitmapPalette
*
* DESCRIPTION:
*   The function EwModifyBitmapPalette() changes color values within the palette
*   of the given Index8 bitmap. If applied on a non Index8 bitmap, the function
*   will ignore the operation.
*
*   In case of a multi-frame bitmap, the modification affects all frames within
*   this bitmap.
*
* ARGUMENTS:
*   aBitmap - Index8 bitmap to modify its palette.
*   aIndex  - Number of the first palette entry affected by the modification.
*   aCount  - Number of entries to modify.
*   aColors - Array with aCount colors to store within the palette starting
*     with palette entry aIndex.
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwModifyBitmapPalette
(
  XBitmap*          aBitmap,
  XInt32            aIndex,
  XInt32            aCount,
  XColor*           aColors
);


/*******************************************************************************
* FUNCTION:
*   EwFlushBitmap
*
* DESCRIPTION:
*   The function EwFlushBitmap() has the job to flush any outstanding drawing
*   Graphics Engine operations for this bitmap as destination.
*
* ARGUMENTS:
*   aBitmap - Bitmap to flush drawing operations.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFlushBitmap
(
  XBitmap*          aBitmap
);


/*******************************************************************************
* FUNCTION:
*   EwLoadFont
*
* DESCRIPTION:
*   The function EwLoadFont() creates a new font and loads it with the content
*   of the passed font resource.
*
* ARGUMENTS:
*   aResource - Descriptor of the font resource to load. The content of this
*     descriptor depends on the particular platform system.
*
* RETURN VALUE:
*   If sucessful, the function returns a new font initialized with the content
*   of the resource. If failed, the function returns null.
*
*******************************************************************************/
XFont* EwLoadFont
(
  const struct XFntRes* aResource
);


/*******************************************************************************
* FUNCTION:
*   EwFreeFont
*
* DESCRIPTION:
*   The function EwFreeFont() frees the given font. Fonts can be loaded from a
*   font resource by calling the function EwLoadFont().
*
* ARGUMENTS:
*   aFont - Font to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeFont
(
  XFont*            aFont
);


/*******************************************************************************
* FUNCTION:
*   EwInitViewport
*
* DESCRIPTION:
*   The function EwInitViewport() prepares a new viewport. Viewports provide a
*   kind of abstraction of the physical framebuffer or display where graphical
*   outputs are drawn. Depending on the particular target system there are two
*   mayor approaches how Graphics Engine can deal with a framebuffer:
*
*   1. The creation and the initialization of the framebuffer are perfomed by
*      the Graphics Engine in the particular platform adaptation layer. In this
*      case Graphics Engine maintains the full control and the ownership over
*      the framebuffer. The Graphics Engine can show, hide and reconfigure the
*      framebuffer every time. Therefore the viewport API provides functions
*      which allow the main software or other external software parts to change
*      the framebuffer configuration - in a platform independent way.
*
*      In this use case, the arguments passed to EwInitViewport() are used to
*      create and configure a new framebuffer and finally to arrange it on the
*      display. These configuration can be changed later by EwConfigViewport().
*
*   2. The framebuffer creation and its initialization are tasks of the main
*      software, external frameworks, the operating system, etc. This means,
*      the framebuffer control lies completely beyond the Embedded Wizard. The
*      Graphics Engine is limited to draw into this framebuffer. The ownership
*      and the framebuffer configuration are aspects of the main software or
*      the external framework only.
*
*      In this use case, EwInitViewport() receives a reference to an existing
*      framebuffer or other kind of graphical context where drawing operation
*      will take place and stores it internally. Optionally, EwInitViewport()
*      can create a private, internal off-screen surface which is used as the
*      framebuffer. When screen update is performed, Graphics Engine copies
*      the affected area from the off-screen surface into the framebuffer or
*      display passed to the EwInitViewport() function.
*
*   After a viewport has been initialized, screen updates can be performed. To
*   do this the functions EwBeginUpdate(), EwBeginUpdateArea() and EwEndUpdate()
*   are intended. These functions cover the internal aspects of the framebuffer
*   access, the double buffering, V-Sync, etc.
*
*   If unused anymore, viewports should be freed by EwDoneViewport().
*
* ARGUMENTS:
*   aSize     - Size of the viewport in pixel. This is the area where graphics
*     outputs will occur. Depending on the use case, this parameter determines
*     the size of the framebuffer to create or the size of an already existing
*     framebuffer to use.
*   aExtent   - Position and size, where the created framebuffer should be shown
*     on the screen. If the size of aExtent differs from aSize, the framebuffer
*     content should be scaled in order to fit in the aExtent area.
*     Please note: The area is already expressed in coordinates valid within the
*     target display by taking in account all particular configuration aspects
*     like the default rotation of the surface contents.
*   aOrient   - Orientation hint. The meaning of this parameter depends on the
*     target system. For example, OpenGL target uses aOrient to determine the
*     screen rotation in degrees.
*   aOpacity  - Opacity value for the created framebuffer in the range 0 .. 255.
*     0 -> fully transparent. 255 -> fully opaque.
*   aDisplay1,
*   aDisplay2,
*   aDisplay3 - Platform dependent parameter, where already existing framebuffer
*     or drawing context, etc. are passed.
*   aProc     - Optional callback function to call when viewport update has
*     been done. The callback function can then perform some particular tasks
*     to complete the screen update, etc. If 0, no callback is called.
*
* RETURN VALUE:
*   If successful, the function returns a new viewport otherwise 0 is returned.
*
*******************************************************************************/
XViewport* EwInitViewport
(
  XPoint            aSize,
  XRect             aExtent,
  XInt32            aOrient,
  XInt32            aOpacity,
  void*             aDisplay1,
  void*             aDisplay2,
  void*             aDisplay3,
  XViewportProc     aProc
);


/*******************************************************************************
* FUNCTION:
*   EwDoneViewport
*
* DESCRIPTION:
*   The function EwDoneViewport() deinitializes the given viewport. If necessary
*   the previously created framebuffers are hidden and video memory is released.
*
* ARGUMENTS:
*   aViewport - Viewport to deinitialize.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDoneViewport
(
  XViewport*        aViewport
);


/*******************************************************************************
* FUNCTION:
*   EwConfigViewport
*
* DESCRIPTION:
*   The function EwConfigViewport() changes the configuration of the viewport.
*   Generally, the function can modify the settings of framebuffers belonging
*   to the viewport.
*
* ARGUMENTS:
*   aViewport - Viewport to reconfigure its framebuffer.
*   aPos      - Position, where the framebuffer should be shown on the screen.
*     Please note: The value is already expressed in coordinates valid within
*     the target display by taking in account all particular configuration
*     aspects like the default rotation of the surface contents.
*   aOpacity  - Opacity value for the framebuffer in the range 0 .. 255.
*     0 -> fully transparent. 255 -> fully opaque.
*
* RETURN VALUE:
*   If successful, the function returns != 0. If the used framebuffer doesn't
*   belong to the viewport, or if it doesn't support the parameter modification
*   0 is returned.
*
*******************************************************************************/
int EwConfigViewport
(
  XViewport*        aViewport,
  XPoint            aPos,
  XInt32            aOpacity
);


/*******************************************************************************
* FUNCTION:
*   EwBeginUpdate
*
* DESCRIPTION:
*   The function EwBeginUpdate() initiates the screen update cycle for the given
*   viewport and provides access to its framebuffer via the returned temporary
*   bitmap.
*
*   The returned bitmap covers the framebuffer and can serve as the destination
*   in all drawing operations until the function EwEndUpdate() is called. Due to
*   the platform specific framebuffer limitations the returned bitmap may not be
*   used as source in the drawing operations.
*
*   The bitmap remains valid until EwEndUpdate() is called. Afterwards it may
*   not be used.
*
* ARGUMENTS:
*   aViewport - Viewport to begin the drawing operations.
*
* RETURN VALUE:
*   If successful, the function returns a temporary bitmap providing the access
*   to the framebuffer of the viewport.
*
*******************************************************************************/
XBitmap* EwBeginUpdate
(
  XViewport*        aViewport
);


/*******************************************************************************
* FUNCTION:
*   EwBeginUpdateArea
*
* DESCRIPTION:
*   The function EwBeginUpdateArea() initiates the screen update cycle for the
*   given viewport and provides access to the specified area of its framebuffer
*   via the returned temporary bitmap.
*
*   The returned bitmap covers the area of the framebuffer and can serve as the
*   destination in all drawing operations until the function EwEndUpdate() is
*   called. Due to the platform specific framebuffer limitations the returned
*   bitmap may not be used as source in the drawing operations.
*
*   The bitmap remains valid until EwEndUpdate() is called. Afterwards it may
*   not be used.
*
* ARGUMENTS:
*   aViewport - Viewport to begin the drawing operations.
*   aArea     - Area to update relative to the top-left corner of the viewport.
*
* RETURN VALUE:
*   If successful, the function returns a temporary bitmap providing the access
*   to the framebuffer of the viewport.
*
*******************************************************************************/
XBitmap* EwBeginUpdateArea
(
  XViewport*        aViewport,
  XRect             aArea
);


/*******************************************************************************
* FUNCTION:
*   EwEndUpdate
*
* DESCRIPTION:
*   The function EwEndUpdate() finalizes the screen update cycle for the given
*   viewport. This function flushes all outstanding drawing operations, updates
*   the framebuffer state and frees the temporary bitmap object provided by the
*   previous EwBeginUpdate() or EwBeginUpdateArea() function call.
*
*   Depending on the particular platform double buffering is used or the update
*   is synchronized with the V-Sync.
*
*   If there was a callback function specified during the initialization of the
*   viewport, the function is called at the fin of the update cycle.
*
* ARGUMENTS:
*   aViewport - Viewport to finalize the drawing operations.
*   aArea     - Area modified by the update relative to the top-left corner of
*     the viewport.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEndUpdate
(
  XViewport*        aViewport,
  XRect             aArea
);


/*******************************************************************************
* FUNCTION:
*   EwFillRectangle
*
* DESCRIPTION:
*   The function EwFillRectangle() fills the rectangular area aDstRect of the
*   bitmap aDst with a color gradient specified by the four color parameters
*   aColorTL .. aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the color gradient (Relative to the top-
*     left corner of the destination bitmap frame).
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRectangle
(
  XBitmap*          aDst,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwDrawBorder
*
* DESCRIPTION:
*   The function EwDrawBorder() draws a rectangle border within the area
*   aDstRect of the bitmap aDst with a color gradient specified by the four
*   color parameters aColorTL .. aColorBL. The parameter aEdgeWidth determines
*   the width of the border's edge.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to draw the border with color gradient (Relative to the
*     top-left corner of the destination bitmap frame).
*   aEdgeWidth  - The width of the border in pixel.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawBorder
(
  XBitmap*          aDst,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XInt32            aEdgeWidth,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwCreatePath
*
* DESCRIPTION:
*   The function EwCreatePath() creates a new path and reserves memory for up
*   aMaxNoOfSubPaths sub-paths. The sub-paths are initially empty. Before data
*   can be stored in a sub-path the function EwInitSubPath() should be called.
*
*   Once the path is prepared and loaded with coordinate information, the path
*   can be passed to the EwFillPath() and EwStrokePath() functions. If not
*   used anymore the path should be freed by using the function EwFreePath().
*
* ARGUMENTS:
*   aMaxNoOfSubPaths - Max. number of sub-paths the new path can manage.
*
* RETURN VALUE:
*   If successful, the function returns a pointer to the newly created path.
*   Otherwise the function returns 0.
*
*******************************************************************************/
XPath* EwCreatePath
(
  XInt32            aMaxNoOfSubPaths
);


/*******************************************************************************
* FUNCTION:
*   EwCreatePathFromSVGString
*
* DESCRIPTION:
*   The function EwCreatePathFromSVGString() creates a new path according to
*   SVG path instructions found in the passed aSVGString parameter.
*
* ARGUMENTS:
*   aSVGString - String containing the SVG instructions.
*   aScaleX,
*   aScaleY    - Scaling factor to apply on the original SVG coordinates. The
*     values affect also how smooth curves are composed from line segments.
*     The larger a curve the more segments are used.
*   aDeltaX,
*   aDeltaY    - Additional offset to add to the original SVG coordinates.
*     This results in a translation of the SVG path.
*   aAngle     - Additional angle to apply a rotation transformation on the
*     original SVG coordinates. The angle is expressed in degree and measured
*     clockwise relative to the positive X-axis. The rotation is performed
*     around the origin position (X:0, Y:0) of the original SVG coordinates.
*
* RETURN VALUE:
*   If successful, the function returns a pointer to the newly created path.
*   Otherwise the function returns 0. If the specified SVG string contains
*   errors, the function stops and returns the already prepared path without
*   evaluating further SVG data. The success of the operation or error status
*   can be queried by the functions EwGetSVGParserStatus() and
*   EwGetSVGParserErrorPos().
*
*******************************************************************************/
XPath* EwCreatePathFromSVGString
(
  XString           aSVGString,
  XFloat            aScaleX,
  XFloat            aScaleY,
  XFloat            aDeltaX,
  XFloat            aDeltaY,
  XFloat            aAngle
);


/*******************************************************************************
* FUNCTION:
*   EwGetSVGParserStatus
*
* DESCRIPTION:
*   The function EwGetSVGParserStatus() returns the status resulting from the
*   preceding invocation of the function EwCreatePathFromSVGString(). Possible
*   values are defined in the macros EW_SVG_PARSER_XXX.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns EW_SVG_PARSER_SUCCESS if the parsed SVG string did not contain any
*   errors. Otherwise another value defined in EW_SVG_PARSER_XXX is returned.
*   In such case use the function EwGetSVGParserErrorPos() to get the position
*   within the parsed SVG string, which has raised the error status.
*
*******************************************************************************/
int EwGetSVGParserStatus
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwGetSVGParserErrorPos
*
* DESCRIPTION:
*   The function EwGetSVGParserErrorPos() returns the position within the SVG
*   string at which the function EwCreatePathFromSVGString() has encountered an
*   error. To query the error status use the function EwGetSVGParserStatus().
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns the position (in characters) within the SVG string starting with 0
*   for the first character. In the case there is no error recorded, a value -1
*   is returned.
*
*******************************************************************************/
XInt32 EwGetSVGParserErrorPos
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwFreePath
*
* DESCRIPTION:
*   The function EwFreePath() frees the memory occupied by the path and its all
*   sub-paths.
*
* ARGUMENTS:
*   aPath - Pointer to the path to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreePath
(
  XPath*            aPath
);


/*******************************************************************************
* FUNCTION:
*   EwGetMaxNoOfSubPaths
*
* DESCRIPTION:
*   The function EwGetMaxNoOfSubPaths() returns how many sub-paths the given
*   path can maximally store. The returned value corresponds to the parameter
*   passed in the invocation of the EwCreatePath() function.
*
* ARGUMENTS:
*   aPath - Pointer to the path to query the information.
*
* RETURN VALUE:
*   If successful, the function returns the max. number of sub-path the path
*   can store. Otherwise the function returns 0.
*
*******************************************************************************/
XInt32 EwGetMaxNoOfSubPaths
(
  XPath*            aPath
);


/*******************************************************************************
* FUNCTION:
*   EwInitSubPath
*
* DESCRIPTION:
*   The function EwInitSubPath() prepares a sub-path to be able to store up to
*   aMaxNoOfEdges path edges. With this operation memory for the sub-path data
*   is reserved. Initially the just initialized sub-path is considered as being
*   still empty. To fill the sub-path with edge coordinates use the functions
*   EwAddSubPathLine(), EwAddSubPathArc(), etc.
*
*   If the affected sub-path has been already initialized in the past, the old
*   information is discarded before initializing the new sub-path.
*
* ARGUMENTS:
*   aPath         - Pointer to the path to initialize the sub-path.
*   aSubPathNo    - Number identifying the sub-path within aPath to initialize.
*     The first sub-path has the number 0. The second 1, and so far.
*   aMaxNoOfEdges - The max. number of edges you intend to store in the sub-
*     path. This is the max. capacity for the sub-path. Passing 0 (zero) in
*     this  parameter results in the existing sub-path data being released and
*     the sub-path is considered as empty again.
*
* RETURN VALUE:
*   If successful, the function returns != 0. If there is no available memory
*   for the sub-path allocation, 0 is returned.
*
*******************************************************************************/
XBool EwInitSubPath
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XInt32            aMaxNoOfEdges
);


/*******************************************************************************
* FUNCTION:
*   EwBeginSubPath
*
* DESCRIPTION:
*   The function EwBeginSubPath() sets the start position for the sub-path. The
*   affected sub-path has to be initialized previously by calling the function
*   EwInitSubPath(). Beginning with the position, the path can be filled with
*   edges by calling functions like EwAddSubPathLine(), EwAddSubPathBezier2(),
*   etc.
*
*   Every sub-path contains exact one begin position. Calling this function for
*   a sub-path being already filled with edge coordinates will clear the actual
*   sub-path coordinate data and sets the new begin position.
*
* ARGUMENTS:
*   aPath      - Pointer to the path containing the affected sub-path.
*   aSubPathNo - Number identifying the sub-path within aPath to configure. The
*     first sub-path has the number 0. The second 1, and so far.
*   aX, aY     - The coordinates to be used as the start position for the sub-
*     path. The position is transformed by using the path transformation matrix.
*     See functions like EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBeginSubPath
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XFloat            aX,
  XFloat            aY
);


/*******************************************************************************
* FUNCTION:
*   EwAddSubPathLine
*
* DESCRIPTION:
*   The function EwAddSubPathLine() appends at the actual position in the sub-
*   path a new straight line segment consisting of one edge. Then the sub-path
*   actual position is moved to the end of the just added line.
*
*   The actual position results from the end position of the preceding sub-path
*   segment or it is the value specified in EwBeginSubPath() invocation. If the
*   sub-path is empty and no start position has been specified in the preceding
*   EwBeginSubPath() invocation, the function assumes the start position is X=0,
*   Y=0.
*
* ARGUMENTS:
*   aPath      - Pointer to the path containing the affected sub-path.
*   aSubPathNo - Number identifying the sub-path within aPath to add the line.
*     The first sub-path has the number 0. The second 1, and so far.
*   aX, aY     - The coordinates to be used as the end position for the new line
*     segment. The position is transformed by using the path transformation
*     matrix. See functions like EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*
* RETURN VALUE:
*   Returns the index of the node within the sub-path where the end position
*   of the new line segment has bee stored. Knowing this index, the position
*   can be modified later by using the function EwSetSubPathNode().
*
*   If the sub-path has not been initialized previously by calling the function
*   EwInitSubPath(), the sub-path has been closed by EwCloseSubPath() or the
*   entire memory reserved for the sub-path is already occupied by other path
*   information, the function fails and returns 0.
*
*******************************************************************************/
XInt32 EwAddSubPathLine
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XFloat            aX,
  XFloat            aY
);


/*******************************************************************************
* FUNCTION:
*   EwAddSubPathBezier2
*
* DESCRIPTION:
*   The function EwAddSubPathBezier2() appends at the actual position in the
*   sub-path a new quadratic Bézier curve composed of exact aNoOfEdges straight
*   line segments. Then the sub-path actual position is moved to the end of the
*   just added curve.
*
*   The actual position results from the end position of the preceding sub-path
*   segment or it is the value specified in EwBeginSubPath() invocation. If the
*   sub-path is empty and no start position has been specified in the preceding
*   EwBeginSubPath() invocation, the function assumes the start position is X=0,
*   Y=0.
*
* ARGUMENTS:
*   aPath      - Pointer to the path containing the affected sub-path.
*   aSubPathNo - Number identifying the sub-path within aPath to add the curve.
*     The first sub-path has the number 0. The second 1, and so far.
*   aCPX, aCPY - The coordinates of the Bézier curve control point. The position
*     is transformed by using the path transformation matrix. See functions like
*     EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*   aX, aY     - The coordinates to be used as the end position for the Bézier
*     curve. The position is transformed by using the path transformation matrix.
*     See functions like EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*   aNoOfEdges - Number of straight line segments the Bézier curve should be
*     composed of. The more edges the smoother the resulting curve.
*
* RETURN VALUE:
*   Returns the index of the node within the sub-path where the position of the
*   first Bézier line segment has bee stored. Knowing this index and the number
*   of segments the Bézier curve is composed of (aNoOfEdges), the position of
*   every Bézier line segment can be modified later. See EwSetSubPathNode().
*
*   If the sub-path has not been initialized previously by calling the function
*   EwInitSubPath(), the sub-path has been closed by EwCloseSubPath() or there
*   is no sufficient memory in the sub-path for the new Bézier curve segments,
*   the function fails and returns 0.
*
*******************************************************************************/
XInt32 EwAddSubPathBezier2
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XFloat            aCPX,
  XFloat            aCPY,
  XFloat            aX,
  XFloat            aY,
  XInt32            aNoOfEdges
);


/*******************************************************************************
* FUNCTION:
*   EwAddSubPathBezier3
*
* DESCRIPTION:
*   The function EwAddSubPathBezier3() appends at the actual position in the
*   sub-path a new cubic Bézier curve composed of exact aNoOfEdges straight line
*   segments. Then the sub-path actual position is moved to the end of the just
*   added curve.
*
*   The actual position results from the end position of the preceding sub-path
*   segment or it is the value specified in EwBeginSubPath() invocation. If the
*   sub-path is empty and no start position has been specified in the preceding
*   EwBeginSubPath() invocation, the function assumes the start position is X=0,
*   Y=0.
*
* ARGUMENTS:
*   aPath        - Pointer to the path containing the affected sub-path.
*   aSubPathNo   - Number identifying the sub-path within aPath to add the
*     curve. The first sub-path has the number 0. The second 1, and so far.
*   aCP1X, aCP1Y - The coordinates of the first Bézier curve control point. The
*     position is transformed by using the path transformation matrix. See
*     functions like EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*   aCP2X, aCP2Y - The coordinates of the second Bézier curve control point. The
*     position is transformed by using the path transformation matrix. See
*     functions like EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*   aX, aY       - The coordinates to be used as the end position for the Bézier
*     curve. The position is transformed by using the path transformation matrix.
*     See functions like EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*   aNoOfEdges   - Number of straight line segments the Bézier curve should be
*     composed of. The more edges the smoother the resulting curve.
*
* RETURN VALUE:
*   Returns the index of the node within the sub-path where the position of the
*   first Bézier line segment has bee stored. Knowing this index and the number
*   of segments the Bézier curve is composed of (aNoOfEdges), the position of
*   every Bézier line segment can be modified later. See EwSetSubPathNode().
*
*   If the sub-path has not been initialized previously by calling the function
*   EwInitSubPath(), the sub-path has been closed by EwCloseSubPath() or there
*   is no sufficient memory in the sub-path for the new Bézier curve segments,
*   the function fails and returns 0.
*
*******************************************************************************/
XInt32 EwAddSubPathBezier3
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XFloat            aCP1X,
  XFloat            aCP1Y,
  XFloat            aCP2X,
  XFloat            aCP2Y,
  XFloat            aX,
  XFloat            aY,
  XInt32            aNoOfEdges
);


/*******************************************************************************
* FUNCTION:
*   EwAddSubPathArc
*
* DESCRIPTION:
*   The function EwAddSubPathArc() adds to the sub-path a new arc curve composed
*   of exact aNoOfEdges straight line segments. If the sub-path contains already
*   any segment information, the start of the arc curve is connected with the
*   end of the last segment by an additional straight line.
*
*   Similarly, if start position for the sub-path has been specified by using
*   EwBeginSubPath(), an additional line segment is added to connect the curve
*   with the specified sub-path start position.
*
*   After the curve is stored in the path, the sub-path actual position is moved
*   to refer to the end position of the arc.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to add the curve.
*     The first sub-path has the number 0. The second 1, and so far.
*   aCenterX,
*   aCenterY    - The coordinates of the center of the arc to calculate. The
*     position is transformed by using the path transformation matrix. See
*     functions like EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*   aRadiusX,
*   aRadiusY    - Radius if the arc in X and Y direction.
*   aStartAngle,
*   aEndAngle   - Start- and end-angle of the arc expressed in degree and
*     measured clockwise relative to the positive X-axis.
*   aNoOfEdges  - Number of straight line segments the arc curve should be
*     composed of. The more edges the smoother the resulting curve.
*
* RETURN VALUE:
*   Returns the index of the node within the sub-path where the start position
*   of the first arc line segment has bee stored. Knowing this index and the
*   number of segments the arc curve is composed of (aNoOfEdges), the position
*   of every arc line segment can be modified later. See EwSetSubPathNode().
*
*   If the sub-path has not been initialized previously by calling the function
*   EwInitSubPath(), the sub-path has been closed by EwCloseSubPath() or there
*   is no sufficient memory in the sub-path for the new arc curve segments, the
*   function fails and returns 0.
*
*******************************************************************************/
XInt32 EwAddSubPathArc
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XFloat            aCenterX,
  XFloat            aCenterY,
  XFloat            aRadiusX,
  XFloat            aRadiusY,
  XFloat            aStartAngle,
  XFloat            aEndAngle,
  XInt32            aNoOfEdges
);


/*******************************************************************************
* FUNCTION:
*   EwAddSubPathCopy
*
* DESCRIPTION:
*   The function EwAddSubPathCopy() adds to the sub-path a copy of another sub-
*   path. In this manner contents from different paths can be combined together
*   without the necessity to re-calculate them again.
*
*   If the destination sub-path contains already any segment information, the
*   start of the copied source sub-path is connected with the end of the last
*   segment by an additional straight line.
*
*   Similarly, if start position for the sub-path has been specified by using
*   EwBeginSubPath(), an additional line segment is added to connect it with
*   the copied sub-path.
*
*   If the transformation matrix of the destination path has been configured,
*   the copied source coordinates are transformed by using the matrix. (See
*   EwTranslatePath(), EwScalePath(), etc.)
*
*   After the copy is stored in the path, the sub-path actual position is moved
*   to refer to the end position of the copied sub-path.
*
* ARGUMENTS:
*   aPath         - Pointer to the destination path containing the affected
*     sub-path to add the copy.
*   aSubPathNo    - Number identifying the sub-path within aPath to add the
*     copy. The first sub-path has the number 0. The second 1, and so far.
*   aSrcPath      - Pointer to the source path containing the sub-path to copy.
*   aSrcSubPathNo - Number identifying the sub-path within aSrcPath to create
*     a copy. The first sub-path has the number 0. The second 1, and so far.
*   aSrcNodeNo    - The number of the node in the source sub-path to start the
*     copy operation. If this parameter is == 0, the operation starts with the
*     first node (the begin) of the source sub-path.
*   aNoOfEdges    - The number of edges to copy from the source sub-path
*     beginning with the node specified in the parameter aSrcNodeNo. In total,
*     ( aNoOfEdges + 1 ) will be copied. Specifying -1 in this parameter means
*     that all edges until the source sub-path end should be copied. If there
*     are less edges available in the source path, the function fails.
*
* RETURN VALUE:
*   Returns the index of the node within the destination sub-path where the
*   start position of the first copied segment has bee stored. Knowing this
*   index and the number of copied segments the, the position of every segment
*   can be modified later. See EwSetSubPathNode().
*
*   If the sub-path has not been initialized previously by calling the function
*   EwInitSubPath(), the sub-path has been closed by EwCloseSubPath(), there
*   is no sufficient memory in the destination sub-path for the complete copy
*   of the source sub-path or the source sub-path or the requested nodes do
*   not exist, the function fails and returns 0.
*
*******************************************************************************/
XInt32 EwAddSubPathCopy
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XPath*            aSrcPath,
  XInt32            aSrcSubPathNo,
  XInt32            aSrcNodeNo,
  XInt32            aNoOfEdges
);


/*******************************************************************************
* FUNCTION:
*   EwCloseSubPath
*
* DESCRIPTION:
*   The function EwCloseSubPath() marks the affected sub-path as closed. The
*   function verifies whether the first and last position of the path are equal
*   and if this is not the case, adds an additional line segment to the path in
*   order to close it.
*
*   Once the function is called, no additional path information can be added to
*   the affected path unless it is initialized again by using EwInitSubPath()
*   or cleared by using EwBeginSubPath().
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to close. The
*     first sub-path has the number 0. The second 1, and so far.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCloseSubPath
(
  XPath*            aPath,
  XInt32            aSubPathNo
);


/*******************************************************************************
* FUNCTION:
*   EwShiftSubPathNodes
*
* DESCRIPTION:
*   The function EwShiftSubPathNodes() removes from the sub-path the specified
*   number of leading coordinate values. This results in the sub-path content
*   being shifted and the number of sub-path edges being reduced by the given
*   aNoOfNodes parameter. Thereupon new edge data can be appended to the path,
*   e.g. by using the function EwAddSubPathLine().
*
*   Applying the function on a sub-path closed by a preceding EwCloseSubPath()
*   is not possible and causes the function to return without any modification
*   on the sub-path data.
*
*   Moreover, the operation is limited to leave at least one node in the path.
*   In other words, it is not possible to clear the path completely. Use the
*   functions EwBeginSubPath() or EwInitSubPath() in such application cases.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to close. The
*     first sub-path has the number 0. The second 1, and so far.
*   aNoOfNodes  - The number of X,Y coordinate pairs to remove from the sub-
*     path starting with the very first node # 0.
*   aTranslateX,
*   aTranslateY - Offset to add to all coordinates after the content is
*     shifted.
*
* RETURN VALUE:
*   Returns the number of nodes removed from the sub-path or 0 if the sub-path
*   doesn't exist, is not initialized (see EwInitSubPath()), is empty or it has
*   been closed by preceding EwCloseSubPath() invocation.
*
*******************************************************************************/
XInt32 EwShiftSubPathNodes
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XInt32            aNoOfNodes,
  XFloat            aTranslateX,
  XFloat            aTranslateY
);


/*******************************************************************************
* FUNCTION:
*   EwSetSubPathNode
*
* DESCRIPTION:
*   The function EwSetSubPathNode() modifies the X,Y coordinate of the given
*   node within the path. In this manner it is not necessary to re-initialize
*   and re-calculate the complete path if only few path corners do move.
*
*   Trying to modify a not existing node is ignored.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to modify a node.
*     The first sub-path has the number 0. The second 1, and so far.
*   aNodeNo     - The path node (the corner) affected by the modification. The
*     nodes are counted starting with 0.
*   aX, aY      - The coordinates to assign to the affected node. The position
*     is transformed by using the path transformation matrix. See functions like
*     EwScalePathMatrix(), EwRotatePathMatrix(), etc.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetSubPathNode
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XInt32            aNodeNo,
  XFloat            aX,
  XFloat            aY
);


/*******************************************************************************
* FUNCTION:
*   EwGetSubPathNodeX
*
* DESCRIPTION:
*   The function EwGetSubPathNodeX() returns the X coordinate of the given
*   node within the path.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to get the node
*     coordinate. The first sub-path has the number 0. The second 1, and so far.
*   aNodeNo     - The path node (the corner) affected by the query ioperation.
*     The nodes are counted starting with 0.
*
* RETURN VALUE:
*   Returns the X coordinate stored in the specified node or 0.0 if the sub-
*   path doesn't exist, is not initialized (see EwInitSubPath()) or the desired
*   node is not available in the sub-apth.
*
*******************************************************************************/
XFloat EwGetSubPathNodeX
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XInt32            aNodeNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetSubPathNodeY
*
* DESCRIPTION:
*   The function EwGetSubPathNodeY() returns the Y coordinate of the given
*   node within the path.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to get the node
*     coordinate. The first sub-path has the number 0. The second 1, and so far.
*   aNodeNo     - The path node (the corner) affected by the query ioperation.
*     The nodes are counted starting with 0.
*
* RETURN VALUE:
*   Returns the Y coordinate stored in the specified node or 0.0 if the sub-
*   path doesn't exist, is not initialized (see EwInitSubPath()) or the desired
*   node is not available in the sub-apth.
*
*******************************************************************************/
XFloat EwGetSubPathNodeY
(
  XPath*            aPath,
  XInt32            aSubPathNo,
  XInt32            aNodeNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfSubPathEdges
*
* DESCRIPTION:
*   The function EwGetNoOfSubPathEdges() returns how many edges (straight line
*   segments) the affected sub-path actually store.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to query the
*     information. The first sub-path has the number 0. The second 1, and so
*     far.
*
* RETURN VALUE:
*   Returns the number of edges existing actually in the sub-path or 0 if the
*   sub-path doesn't exist, is not initialized (see EwInitSubPath()) or is
*   empty.
*
*******************************************************************************/
XInt32 EwGetNoOfSubPathEdges
(
  XPath*            aPath,
  XInt32            aSubPathNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfFreeSubPathEdges
*
* DESCRIPTION:
*   The function EwGetNoOfFreeSubPathEdges() returns how many edges (straight
*   line segments) can still be added to the affected sub-path.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to query the
*     information. The first sub-path has the number 0. The second 1, and so
*     far.
*
* RETURN VALUE:
*   Returns the number of edges the sub-path can additionally accomodate or 0
*   if the sub-path doesn't exist, is not initialized (see EwInitSubPath()), is
*   closed (see EwCloseSubPath()) or is full.
*
*******************************************************************************/
XInt32 EwGetNoOfFreeSubPathEdges
(
  XPath*            aPath,
  XInt32            aSubPathNo
);


/*******************************************************************************
* FUNCTION:
*   EwIsSubPathClosed
*
* DESCRIPTION:
*   The function EwIsSubPathClosed() returns != 0 if the affected sub-path has
*   been closed by calling the function EwCloseSubPath(). Once closed, no new
*   path segments can be added to the sub-path.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to query the
*     information. The first sub-path has the number 0. The second 1, and so
*     far.
*
* RETURN VALUE:
*   Returns != 0 if the sub-path has been closed.
*
*******************************************************************************/
XBool EwIsSubPathClosed
(
  XPath*            aPath,
  XInt32            aSubPathNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetSubPathBounds
*
* DESCRIPTION:
*   The function EwGetSubPathBounds() returns the rectangular area enclosing
*   all nodes of the affected sub-path. The position and the size of the area
*   are rounded to the near integer value.
*
* ARGUMENTS:
*   aPath       - Pointer to the path containing the affected sub-path.
*   aSubPathNo  - Number identifying the sub-path within aPath to query the
*     information. The first sub-path has the number 0. The second 1, and so
*     far.
*
* RETURN VALUE:
*   Returns the area. If the specified sub-path does not exist, the function
*   returns an empty rectangle.
*
*******************************************************************************/
XRect EwGetSubPathBounds
(
  XPath*            aPath,
  XInt32            aSubPathNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetPathBounds
*
* DESCRIPTION:
*   The function EwGetPathBounds() returns the rectangular area enclosing all
*   nodes of all sub-paths of the given path. The position and the size of the
*   area are rounded to the near integer value.
*
* ARGUMENTS:
*   aPath - Pointer to the path to query the information.
*
* RETURN VALUE:
*   Returns the area. If the specified path does not contain any edges, the
*   function returns an empty rectangle.
*
*******************************************************************************/
XRect EwGetPathBounds
(
  XPath*            aPath
);


/*******************************************************************************
* FUNCTION:
*   EwTranslatePathMatrix
*
* DESCRIPTION:
*   The function EwTranslatePathMatrix() applies the given displacement to the
*   path matrix. This corresponds to the translation of the origin of the path
*   coordinate system by the given values in the X- and Y-direction.
*
*   The modification of the path matrix affects the position of line segments
*   added later to the sub-paths of the path.
*
* ARGUMENTS:
*   aPath    - Pointer to the path to apply the transformation.
*   aDeltaX,
*   aDeltaY  - Displacement to apply on the transformation matrix.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwTranslatePathMatrix
(
  XPath*            aPath,
  XFloat            aDeltaX,
  XFloat            aDeltaY
);


/*******************************************************************************
* FUNCTION:
*   EwScalePathMatrix
*
* DESCRIPTION:
*   The function EwScalePathMatrix() applies the given factors to the path
*   matrix. This corresponds to the scaling of the path coordinate system by
*   the given values in the X- and Y-direction.
*
*   The modification of the path matrix affects the position of line segments
*   added later to the sub-paths of the path.
*
* ARGUMENTS:
*   aPath    - Pointer to the path to apply the transformation.
*   aScaleX,
*   aScaleY  - Scaling factors to apply on the transformation matrix.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwScalePathMatrix
(
  XPath*            aPath,
  XFloat            aScaleX,
  XFloat            aScaleY
);


/*******************************************************************************
* FUNCTION:
*   EwRotatePathMatrix
*
* DESCRIPTION:
*   The function EwRotatePathMatrix() applies the given angle to the path matrix.
*   This corresponds to the rotation of the path coordinate system around its
*   origin position.
*
*   The modification of the path matrix affects the position of line segments
*   added later to the sub-paths of the path.
*
* ARGUMENTS:
*   aPath  - Pointer to the path to apply the transformation.
*   aAngle - Rotation angle to apply on the transformation matrix. The angle is
*     expressed in degree and measured clockwise.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwRotatePathMatrix
(
  XPath*            aPath,
  XFloat            aAngle
);


/*******************************************************************************
* FUNCTION:
*   EwPushPathMatrix
*
* DESCRIPTION:
*   The function EwPushPathMatrix() stores the current state of the path matrix
*   on its internal stack. This is very useful during creation of complex paths.
*
*   To restore the matrix again use the function EwPopPathMatrix().
*
* ARGUMENTS:
*   aPath  - Pointer to the path to store its matrix.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwPushPathMatrix
(
  XPath*            aPath
);


/*******************************************************************************
* FUNCTION:
*   EwPopPathMatrix
*
* DESCRIPTION:
*   The function EwPopPathMatrix() restores the current state of the path matrix
*   from its internal stack. The matrix has to be stored by the preceding call
*   to the EwPushPathMatrix() function.
*
*   If the stack is already empty, the identity matrix is loaded.
*
* ARGUMENTS:
*   aPath  - Pointer to the path to restore its matrix.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwPopPathMatrix
(
  XPath*            aPath
);


/*******************************************************************************
* FUNCTION:
*   EwInitPathMatrix
*
* DESCRIPTION:
*   The function EwInitPathMatrix() loads the identity matrix in the path.
*
* ARGUMENTS:
*   aPath  - Pointer to the path to load the matrix.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwInitPathMatrix
(
  XPath*            aPath
);


/*******************************************************************************
* FUNCTION:
*   EwFillPath
*
* DESCRIPTION:
*   The function EwFillPath() fills within the rectangular area aDstRect of
*   the given bitmap aDst a polygon determined by the data stored in aPath. The
*   polygon is filled with a color gradient specified by the four color
*   parameters aColorTL .. aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst            - Destination bitmap.
*   aPath           - Data object providing the path information consisting of
*     one or more sub-paths. Sub-paths, which are opened, are automatically
*     closed by connecting their start and end positions with a straight line
*     segment. All path coordinates are assumed as being relative to the top-
*     left corner of the aDstRect area, or if aFlipY is != 0, relative to the
*     bottom-left corner.
*   aDstFrameNo     - Frame within the destination bitmap affected by the
*     drawing operation.
*   aClipRect       - Area to limit the drawing operation (Relative to the top-
*     left corner of the destination bitmap frame).
*   aDstRect        - Area to fill with the color gradient (Relative to the top-
*     left corner of the destination bitmap frame).
*   aFlipY          - If != 0, the coordinate system of the path is vertically
*     mirrored and its origin is moved to the bottom-left edge of aDstRect.
*   aOffset         - Offset to move the origin of the path coordinate system.
*     Changing this value scrolls the displayed path content.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL        - Color values corresponding to the four corners of aDstRect.
*   aBlend          - If != 0, the drawn pixel will be alpha-blended with the
*     pixel in the background.
*   aAntialiased    - If != 0, the function applies antialiasing to the pixel.
*     The antialiasing is based on supersampling with 4 samples in X and Y
*     direction.
*   aNonZeroWinding - Controls the fill rule to be used by the algorithm. If
*     this parameter is == 0, the even-odd fill rule is used. If this parameter
*     is != 0, the non-zero winding rule is used.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillPath
(
  XBitmap*          aDst,
  XPath*            aPath,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XBool             aFlipY,
  XPoint            aOffset,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend,
  XBool             aAntialiased,
  XBool             aNonZeroWinding
);


/*******************************************************************************
* FUNCTION:
*   EwStrokePath
*
* DESCRIPTION:
*   The function EwStrokePath() strokes within the rectangular area aDstRect of
*   the given bitmap aDst a path determined by the data stored in aPath. The
*   path is stroked with line thickness specified in the parameter aWidth and
*   color gradient specified by the four color parameters aColorTL .. aColorBL.
*   With the parameter aStyle the cap and join points of the path are configured.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst            - Destination bitmap.
*   aPath           - Data object providing the path information consisting of
*     one or more sub-paths. Sub-paths, which are opened, are automatically
*     closed by connecting their start and end positions with a straight line
*     segment. All path coordinates are assumed as being relative to the top-
*     left corner of the aDstRect area, or if aFlipY is != 0, relative to the
*     bottom-left corner.
*   aDstFrameNo     - Frame within the destination bitmap affected by the
*     drawing operation.
*   aClipRect       - Area to limit the drawing operation (Relative to the top-
*     left corner of the destination bitmap frame).
*   aDstRect        - Area to fill with the color gradient (Relative to the top-
*     left corner of the destination bitmap frame).
*   aFlipY          - If != 0, the coordinate system of the path is vertically
*     mirrored and its origin is moved to the bottom-left edge of aDstRect.
*   aOffset         - Offset to move the origin of the path coordinate system.
*     Changing this value scrolls the displayed path content.
*   aWidth          - The width in pixel to stroke the path.
*   aStyle          - Flags specifying how the path caps and join points should
*     appear. See the enumeration EW_PATH_CAP_XXX and EW_PATH_JOIN_XXX.
*   aMiterLimit     - In case aStyle is configured with EW_PATH_JOIN_MITER, this
*     parameter imposes a limit on the ratio between the miter length and the
*     half of the line thickness aWidth. If the limit is exceeded for a corner,
*     the corner appears as bevel (EW_PATH_JOIN_BEVEL) instead of miter.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL        - Color values corresponding to the four corners of aDstRect.
*   aBlend          - If != 0, the drawn pixel will be alpha-blended with the
*     pixel in the background.
*   aAntialiased    - If != 0, the function applies antialiasing to the pixel.
*     The antialiasing is based on supersampling with 4 samples in X and Y
*     direction.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwStrokePath
(
  XBitmap*          aDst,
  XPath*            aPath,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XBool             aFlipY,
  XPoint            aOffset,
  XFloat            aWidth,
  XUInt32           aStyle,
  XFloat            aMiterLimit,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend,
  XBool             aAntialiased
);


/*******************************************************************************
* FUNCTION:
*   EwGetBitmapFromFillPath
*
* DESCRIPTION:
*   The function EwGetBitmapFromFillPath() creates an ALPHA8 bitmap consisting
*   of a single frame in the size aSize and fills within the bitmap a polygon
*   determined by the data stored in aPath. Finally, the function returns the
*   bitmap. Such bitmap can be used in copy operations to speed-up the screen
*   update without the necessity to rasterize the polygon again and again.
*
* ARGUMENTS:
*   aSize           - Size of the bitmap to create.
*   aPath           - Data object providing the path information consisting of
*     one or more sub-paths. Sub-paths, which are opened, are automatically
*     closed by connecting their start and end positions with a straight line
*     segment. All path coordinates are assumed as being relative to the top-
*     left corner of the bitmap area, or if aFlipY is != 0, relative to the
*     bottom-left corner.
*   aFlipY          - If != 0, the coordinate system of the path is vertically
*     mirrored and its origin is moved to the bottom-left edge of the bitmap.
*   aOffset         - Offset to move the origin of the path coordinate system.
*     Changing this value scrolls the displayed path content.
*   aAntialiased    - If != 0, the function applies antialiasing to the pixel.
*     The antialiasing is based on supersampling with 4 samples in X and Y
*     direction.
*   aNonZeroWinding - Controls the fill rule to be used by the algorithm. If
*     this parameter is == 0, the even-odd fill rule is used. If this parameter
*     is != 0, the non-zero winding rule is used.
*
* RETURN VALUE:
*   If successful, the function returns the just created bitmap. If the path
*   is empty or the operation fails, the function returns 0.
*
*******************************************************************************/
XBitmap* EwGetBitmapFromFillPath
(
  XPoint            aSize,
  XPath*            aPath,
  XBool             aFlipY,
  XPoint            aOffset,
  XBool             aAntialiased,
  XBool             aNonZeroWinding
);


/*******************************************************************************
* FUNCTION:
*   EwGetBitmapFromStrokePath
*
* DESCRIPTION:
*   The function EwGetBitmapFromStrokePath() creates an ALPHA8 bitmap containing
*   a single frame in the size aSize and strokes within it a path determined by
*   the data stored in aPath. The path is stroked with line thickness specified
*   in the parameter aWidth. With the parameter aStyle the cap and join points
*   of the path are configured. Finally, the function returns the bitmap. Such
*   bitmap can be used in copy operations to speed-up the screen update without
*   the necessity to rasterize the polygon again and again.
*
* ARGUMENTS:
*   aSize           - Size of the bitmap to create.
*   aPath           - Data object providing the path information consisting of
*     one or more sub-paths. Sub-paths, which are opened, are automatically
*     closed by connecting their start and end positions with a straight line
*     segment. All path coordinates are assumed as being relative to the top-
*     left corner of the bitmap area, or if aFlipY is != 0, relative to the
*     bottom-left corner.
*   aFlipY          - If != 0, the coordinate system of the path is vertically
*     mirrored and its origin is moved to the bottom-left edge of the bitmap.
*   aOffset         - Offset to move the origin of the path coordinate system.
*     Changing this value scrolls the displayed path content.
*   aWidth          - The width in pixel to stroke the path.
*   aStyle          - Flags specifying how the path caps and join points should
*     appear. See the enumeration EW_PATH_CAP_XXX and EW_PATH_JOIN_XXX.
*   aMiterLimit     - In case aStyle is configured with EW_PATH_JOIN_MITER, this
*     parameter imposes a limit on the ratio between the miter length and the
*     half of the line thickness aWidth. If the limit is exceeded for a corner,
*     the corner appears as bevel (EW_PATH_JOIN_BEVEL) instead of miter.
*   aAntialiased    - If != 0, the function applies antialiasing to the pixel.
*     The antialiasing is based on supersampling with 4 samples in X and Y
*     direction.
*
* RETURN VALUE:
*   If successful, the function returns the just created bitmap. If the path
*   is empty or the operation fails, the function returns 0.
*
*******************************************************************************/
XBitmap* EwGetBitmapFromStrokePath
(
  XPoint            aSize,
  XPath*            aPath,
  XBool             aFlipY,
  XPoint            aOffset,
  XFloat            aWidth,
  XUInt32           aStyle,
  XFloat            aMiterLimit,
  XBool             aAntialiased
);


/*******************************************************************************
* FUNCTION:
*   EwCopyBitmap
*
* DESCRIPTION:
*   The function EwCopyBitmap() copies a rectangular area from the bitmap aSrc
*   to the bitmap aDst. The areas affected by this operation are determined by
*   the both arguments aDstRect and aSrcPos. Optionally the copied pixel can be
*   modulated by a color gradient specified by the four parameters aColorTL ..
*   aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aSrc        - Source bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aSrcFrameNo - Source bitmap frame affected by the operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the copied pixel (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcPos     - Origin of the area to copy from the source bitmap (Relative
*     to the top-left corner of the source bitmap frame). The size of the
*     source area corresponds to the size of the destination area as it is
*     specified in aDstRect.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyBitmap
(
  XBitmap*          aDst,
  XBitmap*          aSrc,
  XInt32            aDstFrameNo,
  XInt32            aSrcFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XPoint            aSrcPos,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwTileBitmap
*
* DESCRIPTION:
*   The function EwTileBitmap() fills the rectangular area aDstRect of the aDst
*   bitmap with multiple copies of an area aSrcRect from the bitmap aSrc. The
*   copied pixel can optionally be modulated by a color gradient specified by
*   the four color parameters aColorTL .. aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aSrc        - Source bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aSrcFrameNo - Source bitmap frame affected by the operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the copied pixel (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcRect    - Area to copy from the source bitmap (Relative to the top-left
*     corner of the source bitmap frame).
*   aSrcPos     - Position within the source area to start the operation. This
*     is an offset, which allows the scrolling of the drawn content within the
*     destination area. The source position has to lie within the aSrcRect.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwTileBitmap
(
  XBitmap*          aDst,
  XBitmap*          aSrc,
  XInt32            aDstFrameNo,
  XInt32            aSrcFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XRect             aSrcRect,
  XPoint            aSrcPos,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwWarpBitmap
*
* DESCRIPTION:
*   The function EwWarpBitmap() performs the projection of a rectangular source
*   bitmap area on a 4 corner polygon within the destination bitmap. The kind
*   of transformation is predetermined by the given polygon coordinates. That
*   way scaling, rotating, mirroring and even 3D perspective warp effects can
*   be performed. In case of a perspective correct projection, the coefficients
*   aW1 .. aW4 controls the perspective warp.
*
*   Please note that only convex polygons can be drawn with this technique.
*   Drawing non convex polygons can produce unexpected outputs depending on the
*   underlying graphics subsystem or the 3D hardware engine.
*
*   When performing the projection, the function assumes that the 4 corners of
*   of the source bitmap area are mapped to the four corners of the destination
*   polygon. The top-left source corner corresponds to the first polygon corner.
*   The top-right to the second, bottom-right to the third and the bottom-left
*   to the fourth.
*
*   The copied pixel can optionally be modulated by a color gradient specified
*   by the four color parameters aColor1 .. aColor4.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aSrc        - Source bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aSrcFrameNo - Source bitmap frame affected by the operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstX1,
*   aDstY1,
*   aDstW1,
*   ...
*   aDstX4,
*   aDstY4,
*   aDstW4      - Coordinates of the polygon to fill with the source bitmap as
*     floating point values for sub-pixel precision (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcRect    - Area to copy from the source bitmap (Relative to the top-left
*     corner of the source bitmap frame).
*   aColor1,
*   aColor2,
*   aColor3,
*   aColor4     - Color values corresponding to the four corners of the polygon.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*   aFilter     - If != 0, the source bitmap pixel will be bi-linear filtered
*     in order to get better output.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpBitmap
(
  XBitmap*          aDst,
  XBitmap*          aSrc,
  XInt32            aDstFrameNo,
  XInt32            aSrcFrameNo,
  XRect             aClipRect,
  XFloat            aDstX1,
  XFloat            aDstY1,
  XFloat            aDstW1,
  XFloat            aDstX2,
  XFloat            aDstY2,
  XFloat            aDstW2,
  XFloat            aDstX3,
  XFloat            aDstY3,
  XFloat            aDstW3,
  XFloat            aDstX4,
  XFloat            aDstY4,
  XFloat            aDstW4,
  XRect             aSrcRect,
  XColor            aColor1,
  XColor            aColor2,
  XColor            aColor3,
  XColor            aColor4,
  XBool             aBlend,
  XBool             aFilter
);


/*******************************************************************************
* FUNCTION:
*   EwDrawLine
*
* DESCRIPTION:
*   The function EwDrawLine() draws a line between the coordinates aDstPos1 and
*   aDstPos2 within the destination bitmap aDst. The line will be drawn with a
*   color gradient specified by the both color values aColor1 and aColor2.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstPos1,
*   aDstPos2    - Start and end coordinates of the line (Relative to the top-
*     left corner of the destination bitmap frame).
*   aColor1,
*   aColor2     - Color values corresponding to the start and end of the line.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawLine
(
  XBitmap*          aDst,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XPoint            aDstPos1,
  XPoint            aDstPos2,
  XColor            aColor1,
  XColor            aColor2,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwDrawText
*
* DESCRIPTION:
*   The function EwDrawText() prints a single text row within the destination
*   bitmap aDst. The font and the characters to draw are specified in aFont and
*   aString. The area, where the text will be drawn is defined by aDstRect and
*   by aSrcPos. The copied pixel are modulated by a color gradient specified by
*   the four color parameters aColorTL .. aColorBL.
*
*   The parameter aMinWidth controls optionally the minimum width of the text to
*   draw. If applied, the function extends white space characters in order to
*   stretch the text. By using this parameter justified text output is possible.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst         - Destination bitmap.
*   aFont        - Font to draw the glyphs.
*   aString      - Text to process.
*   aCount       - Maximum number of text characters to draw or -1 if the entire
*     text should be drawn.
*   aDstFrameNo  - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect    - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect     - Area to fill with the text glyphs (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcPos      - Displacement for the text baseline position relative to the
*     corner of aDstRect, which by taking in account the specified aOrientation
*     parameter serves as the origin for the drawing operation.
*   aMinWidth    - Minimum width of the text to draw. If text width is less than
*     aMinWidth, white space characters will be stretched automatically.
*   aOrientation - Orientation how to rotate the text. This parameter can assume
*     the values 0, 90, 180 or 270. The specified aOrientation implies the corner
*     of aDstRect serving as the origin relative to which the text is drawn. For
*     example, if aOrientation is 270, the text is drawn relative to the bottom-
*     right corner of aDstRect.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL     - Color values corresponding to the four corners of aDstRect.
*   aBlend       - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawText
(
  XBitmap*          aDst,
  XFont*            aFont,
  XChar*            aString,
  XInt32            aCount,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XPoint            aSrcPos,
  XInt32            aMinWidth,
  XInt32            aOrientation,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwGetTextExtent
*
* DESCRIPTION:
*   The function EwGetTextExtent() calculates an area required to draw the text
*   aString with the font aFont. The size of the resulting rectangle determines
*   the space where character glyphs will be drawn. The origin of the rectangle
*   specifies the distance between a print position and the first drawn glyph.
*
*   The origin of the calculated area can be passed as aSrcPos to the function
*   EwDrawText(), if seamless text output relative to the top-left corner of
*   the destination area is desired.
*
* ARGUMENTS:
*   aFont   - Font to use for the text extent calculation.
*   aString - Text to process.
*   aCount  - Maximum number of text characters to evaluate or -1 if the entire
*     text should be evaluated.
*
* RETURN VALUE:
*   Returns the rectangular area needed to display the string.
*
*******************************************************************************/
XRect EwGetTextExtent
(
  XFont*            aFont,
  XChar*            aString,
  XInt32            aCount
);


/*******************************************************************************
* FUNCTION:
*   EwGetTextAdvance
*
* DESCRIPTION:
*   The function EwGetTextAdvance() calculates the advance of the entire text
*   aString with the font aFont. The value is calculated by the sum of advance
*   values of glyphs to display the text.
*
* ARGUMENTS:
*   aFont   - Font to use for the text calculation.
*   aString - Text to process.
*   aCount  - Maximum number of text characters to evaluate or -1 if the entire
*     text should be evaluated.
*
* RETURN VALUE:
*   Returns the advance width of the text to display.
*
*******************************************************************************/
XInt32 EwGetTextAdvance
(
  XFont*           aFont,
  XChar*           aString,
  XInt32           aCount
);


/*******************************************************************************
* FUNCTION:
*   EwGetTextColumnAtPosition
*
* DESCRIPTION:
*   The function EwGetTextColumnAtPosition() determines the character number in
*   the given text aString, which corresponds to a glyph enclosing the position
*   aX when aString would be printed by using the font aFont. The algorithm to
*   calculate the position is equal to the used by EwDrawText() to raster the
*   text.
*
*   The parameter aMinWidth controls optionally the minimum width of the text.
*   If applied, the function assumes all white space characters to be extended
*   in order to stretch the text. By using this parameter justified text output
*   is possible.
*
* ARGUMENTS:
*   aFont       - Font to be used for the calculation.
*   aString     - Text to process.
*   aCount      - Maximum number of text characters to draw or -1 if the entire
*     text should be drawn.
*   aX          - Position in pixel relative to the left edge of the text row
*     as if it would be rasterized by EwDrawText().
*   aMinWidth   - Minimum width of the text to consider. If text width is less
*     than aMinWidth, the function will calculate with white space characters
*     being stretched accordingly.
*
* RETURN VALUE:
*   Returns the number of the character in the string corresponding to a glyph
*   enclosing the position aX. The first character has the number 0, the second
*   1, and so far. If aX lies on the left of the first character the function
*   returns -1; If aX lies on the right of the last character, the function
*   returns the number of the character following it, which corresponds to the
*   number of characters in aString.
*
*******************************************************************************/
XInt32 EwGetTextColumnAtPosition
(
  XFont*            aFont,
  XChar*            aString,
  XInt32            aCount,
  XInt32            aX,
  XInt32            aMinWidth
);


/*******************************************************************************
* FUNCTION:
*   EwGetPositionAtTextColumn
*
* DESCRIPTION:
*   The function EwGetPositionForTextColumn() determines the start position for
*   the glyph corresponding to the character aColumn within the string aString
*   when aString would be printed by using the font aFont. The algorithm to
*   calculate the position is equal to the used by EwDrawText() to raster the
*   text.
*
*   The parameter aMinWidth controls optionally the minimum width of the text.
*   If applied, the function assumes all white space characters to be extended
*   in order to stretch the text. By using this parameter justified text output
*   is possible.
*
* ARGUMENTS:
*   aFont       - Font to be used for the calculation.
*   aString     - Text to process.
*   aCount      - Maximum number of text characters to draw or -1 if the entire
*     text should be drawn.
*   aColumn     - Number of the interesting character. The first character has
*     the number 0, the second 1, and so far.
*   aMinWidth   - Minimum width of the text to consider. If text width is less
*     than aMinWidth, the function will calculate with white space characters
*     being stretched accordingly.
*
* RETURN VALUE:
*   Returns the position in pixel of the found glyph relative to the left edge
*   of the text row as if it would be rasterized by EwDrawText(). If aColumn
*   is < 0, the function returns -1; If aColumn addresses a character not
*   existing in the string, the function returns the total length of the string.
*
*******************************************************************************/
XInt32 EwGetPositionAtTextColumn
(
  XFont*            aFont,
  XChar*            aString,
  XInt32            aCount,
  XInt32            aColumn,
  XInt32            aMinWidth
);


/*******************************************************************************
* FUNCTION:
*   EwIsGlyphAvailable
*
* DESCRIPTION:
*   The function EwIsGlyphAvailable() verifies, whether the given glyph is
*   available in the font.
*
* ARGUMENTS:
*   aFont     - Font to verify the glyph existence.
*   aCharCode - Character code of the glyph.
*
* RETURN VALUE:
*   If the glyph is available in the font, the function returns != 0. Otherwise
*   0 (zero) is returned.
*
*******************************************************************************/
int EwIsGlyphAvailable
(
  XFont*            aFont,
  XChar             aCharCode
);


/*******************************************************************************
* FUNCTION:
*   EwGetGlyphMetrics
*
* DESCRIPTION:
*   The function EwGetGlyphMetrics() determines the glyph metrics of the given
*   font glyph. The function is useful when special application dependent text
*   flow calculations are necessary. The caller can determine the size and the
*   position of each glyph within the text.
*
* ARGUMENTS:
*   aFont     - Font to get the glyph metrics.
*   aCharCode - Character code of the glyph.
*   aMetrics  - Pointer to the structure where the metrics should be returned.
*
* RETURN VALUE:
*   If successful, the function returns != 0. When the desired glyph doesn't
*   exist in the font and there is no default glyph defined, zero is returned.
*
*******************************************************************************/
int EwGetGlyphMetrics
(
  XFont*            aFont,
  XChar             aCharCode,
  XGlyphMetrics*    aMetrics
);


/*******************************************************************************
* FUNCTION:
*   EwGetKerning
*
* DESCRIPTION:
*   The function EwGetKerning() determines the kerning to apply between the
*   given glyphs.
*
* ARGUMENTS:
*   aFont      - Font to determine the kerning.
*   aCharCode1 - Character code of the first glyph.
*   aCharCode2 - Character code of the second glyph.
*
* RETURN VALUE:
*   The function returns the kerning value for the given pair of glyphs or 0
*   if the glyphs are not affected by kerning or at least one of the glyphs is
*   not existing in the font.
*
*******************************************************************************/
XInt32 EwGetKerning
(
  XFont*            aFont,
  XChar             aCharCode1,
  XChar             aCharCode2
);


/*******************************************************************************
* FUNCTION:
*   EwLockGlyph
*
* DESCRIPTION:
*   The function EwLockGlyph() provides a direct access to the pixel memory of
*   the given glyph. The function returns a lock object containing pointers to
*   memory, where the caller can read the glyph pixel values. Additional pitch
*   values also returned in the lock object allow the caller to calculate the
*   desired pixel addresses.
*
*   When finished the access cycle, the function EwUnlockGlyph() should be used
*   in order to release the lock.
*
*   Note, glyphs are stored in the Alpha8 pixel format -> one byte per pixel.
*   The pixel value 0 determines transparent pixel. The 255 full opaque pixel.
*   Values 1 .. 254 determine the semitransparent pixel.
*
* ARGUMENTS:
*   aFont     - Font to lock the glyph pixel memory.
*   aCharCode - Character code of the glyph.
*
* RETURN VALUE:
*   If successful, the function returns a temporary glyph lock object. If the
*   intended glyph doesn't exist, 0 is returned.
*
*******************************************************************************/
XGlyphLock* EwLockGlyph
(
  XFont*            aFont,
  XChar             aCharCode
);


/*******************************************************************************
* FUNCTION:
*   EwUnlockGlyph
*
* DESCRIPTION:
*   The function EwUnlockGlyph() provides a counterpart to EwLockGlyph(). When
*   called, the function releases the given lock.
*
*   After calling this function, the aLock structure will become invalid.
*
* ARGUMENTS:
*   aLock - Lock object returned by the previous EwLockGlyph() call.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwUnlockGlyph
(
  XGlyphLock*       aLock
);


/*******************************************************************************
* FUNCTION:
*   EwDrawBitmapFrame
*
* DESCRIPTION:
*   The function EwDrawBitmapFrame() draws a free scalable frame by composing it
*   from up to nine bitmap segments. These segments do correspond to the 4 frame
*   corners, 4 edges and to the frame interior areas. By filling these areas with
*   repeated copies of the bitmap segments, flexible scalable frame is drawn.
*
*   The segments come from the source bitmap aSrc, which for this purpose has to
*   be organized as 3 x 3 matrix of equal sized tiles --> the size of the source
*   area aSrcRect has to a multiple of 3.
*
*   The resulting size of the drawn frame is specified by aDstRect parameter.
*   The aDrawXXX parameters control which edges are drawn. Optionally the copied
*   pixel can be modulated by a color gradient specified by the four parameters
*   aColorTL .. aColorBL.
*
*   An additional clipping area aClipRect limits the operation. All pixel lying
*   outside this area will not be drawn. The last aBlend parameter controls the
*   mode how drawn pixel are combined with the pixel already existing in the
*   destination bitmap. If aBlend != 0, the drawn pixel are alpha-blended with
*   the background, otherwise the drawn pixel will overwrite the old content.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aSrc        - Source bitmap.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aSrcFrameNo - Source bitmap frame affected by the operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the segments (Relative to the top-left
*     corner of the destination bitmap frame).
*   aSrcRect    - Area of the source bitmap to use as source for the drawing
*     operation. This area is assumed as composed of 3x3 equal sized tiles.
*     The area lies relative to the top-left corner of the source bitmap frame.
*   aDrawLeftEdge,
*   aDrawTopEdge,
*   aDrawRightEdge,
*   aDrawBottomEdge,
*   aDrawInterior - Flags to control, whether the corresponding area of the
*     frame is drawn or not. Not drawn edges are filled by the interior area.
*   aColorTL,
*   aColorTR,
*   aColorBR,
*   aColorBL    - Color values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawBitmapFrame
(
  XBitmap*          aDst,
  XBitmap*          aSrc,
  XInt32            aDstFrameNo,
  XInt32            aSrcFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XRect             aSrcRect,
  XBool             aDrawLeftEdge,
  XBool             aDrawTopEdge,
  XBool             aDrawRightEdge,
  XBool             aDrawBottomEdge,
  XBool             aDrawInterior,
  XColor            aColorTL,
  XColor            aColorTR,
  XColor            aColorBR,
  XColor            aColorBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwParseFlowString
*
* DESCRIPTION:
*   The function EwParseFlowString() converts the given source string aString
*   into a new string consisting of an array of wrapped text lines. These lines
*   can then be drawn on the screen by using EwDrawText() function, so multiline
*   flow text output is possible.
*
*   The function parses the passed string and tries to wrap it in separate text
*   rows with the specified width aWidth. The function stops if the string has
*   been processed or the resulting set of text rows has reached aMaxNoOfRows.
*
*   The automatic line wrap is performed primarily between words. Additionally
*   the zero-width-space-sign) and soft-hyphen are possible candidates for the
*   wrap position. These can be specified as special characters '^' or '\x00A0'
*   (for the zero-width-space) and '~' or '\x00AD' (for soft-hyphen).
*
*   The both special characters '^' and '~' are usually ignored and they are not
*   displayed until the line wrap took place at its position. Then the '~' sign
*   is converted into the hyphen '-'. The zero-width-space in contrast breaks a
*   row only and remains invisible. These both special characters provide a
*   flexibel way to output multi-line text.
*
*   Beside the automatic text wrap, an explicit linefeed is possible when the
*   '\n' sign has been found in the string.
*
*   In order to be able to output the special characters as regular signs, the
*   character '%' can be applied in front of the affected sign to convert it
*   to a regular sign (e.g. "%~" for the '~' sign or "%%" for the '%' sign).
*
*   The returned string contains an array of text lines. These are stored in
*   following structure:
*
*   +-------------+
*   | No of lines |
*   +-------------+------+------+------+------+------+     +------+
*   |    Offset   | Char | Char | Char | Char | Char | ... | Char |
*   +-------------+------+------+------+------+------+     +------+
*   |    Offset   | Char | Char | Char | Char | Char | ... | Char |
*   +-------------+------+------+------+------+------+     +------+
*      ...
*   +-------------+------+------+------+------+------+     +------+
*   |    Offset   | Char | Char | Char | Char | Char | ... | Char |
*   +-------------+------+------+------+------+------+     +------+
*   |      0      |
*   +-------------+
*
*   The first character contains the number of lines inside the string. Each
*   line starts with the offset (in character) to the next line, followed by the
*   signs belonging to the line. The lines are not terminated. The entire string
*   is terminated with 0.
*
* ARGUMENTS:
*   aFont        - Pointer to the used font.
*   aString      - String to wrap.
*   aWidth       - Width to wrap the text rows. If this parameter is < 0, the
*     text is not automatically wrapped in rows expcept the linefeed signs.
*   aMaxNoOfRows - Limit when to stop the text processing. If this parameter
*     is < 0, the entire string is processed.
*   aBidi        - If not NULL refers the Bidi context to be used by the
*     function while processing the string.
*
* RETURN VALUE:
*   The function returns a string containing an array of text lines.
*
*   Please note, the returned string is controlled by the Embedded Wizard
*   Runtime Environment EWRTE. If not used anymore, the string is released
*   automatically.
*
*******************************************************************************/
XString EwParseFlowString
(
  XFont*            aFont,
  XChar*            aString,
  XInt32            aWidth,
  XInt32            aMaxNoOfRows,
  XHandle           aBidi
);


/*******************************************************************************
* FUNCTION:
*   EwGetFlowTextAdvance
*
* DESCRIPTION:
*   The function EwGetFlowTextAdvance() calculates the advance of the widest
*   text row in the multi-line text block aFlowString by using the font aFont.
*   The value is calculated by the sum of advance values of all glyphs needed to
*   display the respective text row and then by selecting the result of the row
*   which is the widest one.
*
* ARGUMENTS:
*   aFont       - Font to use for the text calculation.
*   aFlowString - Text to process as already parsed flow string (see the
*     function EwParseFlowString())
*
* RETURN VALUE:
*   Returns the advance value for the widest text row.
*
*******************************************************************************/
XInt32 EwGetFlowTextAdvance
(
  XFont*           aFont,
  XChar*           aFlowString
);


/*******************************************************************************
* FUNCTION:
*   EwCreateAttrSet
*
* DESCRIPTION:
*   The function EwCreateAttrSet() creates and initializes a new XAttrSet. The
*   set is used as a simple container for storing of fonts, bitmaps and colors.
*   These resources are necessary for parsing and drawing of attributed strings.
*
*   The size of the set (the max. number of entries, it can store) is passed in
*   the arguments aNoOfFonts, aNoOfBitmaps and aNoOfColors. After creation, the
*   size of the set is fixed and can not be changed any more.
*
*   All entries of a newly created set are always initialized with 0. To start
*   working with attributed strings, the set should be loaded with necessary
*   fonts, bitmaps and colors. The functions, defined below allow an access to
*   these entries.
*
* ARGUMENTS:
*   aNoOfFonts   - Max. number of fonts.
*   aNoOfBitmaps - Max. number of bitmaps.
*   aNoOfColors  - Max. number of colors.
*
* RETURN VALUE:
*   If successful, the function returns a pointer to the newly created set.
*   Otherwise the function returns 0.
*
*******************************************************************************/
XAttrSet* EwCreateAttrSet
(
  XInt32            aNoOfFonts,
  XInt32            aNoOfBitmaps,
  XInt32            aNoOfColors
);


/*******************************************************************************
* FUNCTION:
*   EwFreeAttrSet
*
* DESCRIPTION:
*   The function EwFreeAttrSet() frees the memory occupied by the set. The
*   function frees the memory only - the corresponding fonts, bitmaps are not
*   affected!
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeAttrSet
(
  XAttrSet*         aAttrSet
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrFonts
*
* DESCRIPTION:
*   The function EwGetNoOfAttrFonts() determinate how many fonts can be stored
*   within the given set.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*
* RETURN VALUE:
*   If successful, the function returns the max. number of fonts. Otherwise the
*   function returns 0.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrFonts
(
  XAttrSet*         aAttrSet
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrBitmaps
*
* DESCRIPTION:
*   The function EwGetNoOfAttrBitmaps() determinate how many bitmaps can be
*   stored within the given set.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*
* RETURN VALUE:
*   If successful, the function returns the max. number of bitmaps. Otherwise
*   the function returns 0.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrBitmaps
(
  XAttrSet*         aAttrSet
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrColors
*
* DESCRIPTION:
*   The function EwGetNoOfAttrColors() determinate how many colors can be stored
*   within the given set.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*
* RETURN VALUE:
*   If successful, the function returns the max. number of colors. Otherwise
*   the function returns 0.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrColors
(
  XAttrSet*         aAttrSet
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrFont
*
* DESCRIPTION:
*   The function EwGetAttrFont() returns the font entry aFontNo from the given
*   set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*   aFontNo  - Number of the affected font entry within the set. The first
*     entry has the number 0, the second 1, and so far.
*
* RETURN VALUE:
*   If successful, the function returns the pointer to the corresponding font
*   object. Otherwise the function returns 0.
*
*******************************************************************************/
XFont* EwGetAttrFont
(
  XAttrSet*         aAttrSet,
  XInt32            aFontNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrBitmap
*
* DESCRIPTION:
*   The function EwGetAttrBitmap() returns the bitmap entry aBitmapNo from the
*   given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet  - Pointer to the set.
*   aBitmapNo - Number of the affected bitmap entry within the set. The first
*     entry has the number 0, the second 1, and so far.
*
* RETURN VALUE:
*   If successful, the function returns the pointer to the corresponding bitmap
*   object. Otherwise the function returns 0.
*
*******************************************************************************/
XBitmap* EwGetAttrBitmap
(
  XAttrSet*         aAttrSet,
  XInt32            aBitmapNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrColor
*
* DESCRIPTION:
*   The function EwGetAttrColor() returns the color entry aColorNo from the
*   given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*   aColorNo - Number of the affected color entry within the set. The first
*     entry has the number 0, the second 1, and so far.
*
* RETURN VALUE:
*   If successful, the function returns the affected color value. Otherwise the
*   function returns 0.
*
*******************************************************************************/
XColor EwGetAttrColor
(
  XAttrSet*         aAttrSet,
  XInt32            aColorNo
);


/*******************************************************************************
* FUNCTION:
*   EwSetAttrFont
*
* DESCRIPTION:
*   The function EwSetAttrFont() assigns new font aFont to the entry aFontNo
*   within the given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*   aFontNo  - Number of the affected font entry. The first entry has the number
*     0, the second 1, and so far.
*   aFont    - Pointer to the font object to be stored within the set. If aFont
*     is 0, the affected font entry is set to zero.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetAttrFont
(
  XAttrSet*         aAttrSet,
  XInt32            aFontNo,
  XFont*            aFont
);


/*******************************************************************************
* FUNCTION:
*   EwSetAttrBitmap
*
* DESCRIPTION:
*   The function EwSetAttrBitmap() assigns new bitmap aBitmap to the entry
*   aBitmapNo within the given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet  - Pointer to the set.
*   aBitmapNo - Number of the affected bitmap entry. The first entry has the
*     number 0, the second 1, and so far.
*   aBitmap   - Pointer to the bitmap object to be stored within the set. If
*     aBitmap is 0, the affected bitmap entry is set to zero.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetAttrBitmap
(
  XAttrSet*         aAttrSet,
  XInt32            aBitmapNo,
  XBitmap*          aBitmap
);


/*******************************************************************************
* FUNCTION:
*   EwSetAttrColor
*
* DESCRIPTION:
*   The function EwSetAttrColor() assigns new color value aColor to the entry
*   aColorNo within the given set aAttrSet.
*
* ARGUMENTS:
*   aAttrSet - Pointer to the set.
*   aColorNo - Number of the affected color entry. The first entry has the
*     number 0, the second 1, and so far.
*   aColor   - The color value to store within the set.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetAttrColor
(
  XAttrSet*         aAttrSet,
  XInt32            aColorNo,
  XColor            aColor
);


/*******************************************************************************
* FUNCTION:
*   EwParseAttrString
*
* DESCRIPTION:
*   The function EwParseAttrString() converts the given source string aString
*   into a list of drawing statements. Unlike simple text drawing operations,
*   attributed strings may appear with multiple fonts, colors and images - like
*   HTML.
*
*   The appearance of the string is determinated by the used attributes and by
*   the passed aAttrSet container. The attributes describe what to do with the
*   text, which font/color should be used or what images should be displayed
*   together with the text. The attributes are always enclosed in '{ .... }'
*   braces. For example, the attribute for color selection looks like this:
*
*     "The text with other {clr1}text color{clr0}."
*
*   The attribute {clr1} forces the Graphics Engine to draw the following text
*   with the color number 1. The value of the desired color is stored in the
*   aAttrSet container. In the same manner a font selection can be applied or
*   an image can be displayed together with the text. The attributed strings
*   use a small but very powerfull set of attributes. They control the entire
*   layout and the appearance of the displayed text.
*
*   The function EwParseAttrString() returns a memory block containing the
*   preprocessed drawing statements. These statements can then be executed
*   very fast by the EwDrawAttrText() function. In this manner the parsing of
*   the string is done only once - the text can be displayed several times.
*
* ARGUMENTS:
*   aAttrSet        - Pointer to the set containing fonts, bitmaps and colors
*     for the attributed string.
*   aString         - Source string containing the text and the attributes.
*   aWidth          - Width of the rectangular area used for the text
*     formatting and line wrapping.
*   aEnableBidiText - If != 0, the Unicode Bidi Algorithm is applied on every
*     paragraph content.
*
* RETURN VALUE:
*   If successful, the function returns a pointer to a memory area containing
*   drawing statements. If the parsing is failed, or the memory is exhausted
*   the function returns 0.
*
*******************************************************************************/
XAttrString* EwParseAttrString
(
  XAttrSet*         aAttrSet,
  XChar*            aString,
  XInt32            aWidth,
  XBool             aEnableBidiText
);


/*******************************************************************************
* FUNCTION:
*   EwFreeAttrString
*
* DESCRIPTION:
*   The function EwFreeAttrString() releases the memory area reserved by the
*   function EwParseAttrString() for drawing statements.
*
* ARGUMENTS:
*   aAttrString - Pointer to the attributed string to free the memory.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeAttrString
(
  XAttrString*      aAttrString
);


/*******************************************************************************
* FUNCTION:
*   EwDrawAttrText
*
* DESCRIPTION:
*   The function EwDrawAttrText() executes the drawing statements from the
*   attributed string aAttrString and draws the text and images into the aDst
*   bitmap at the origin (aDstX, aDstY). The drawing area is clipped by the
*   aClipping rectangle. The necessary fonts, bitmaps and colors are passed in
*   the aAttrSet container.
*
* ARGUMENTS:
*   aDst        - Destination bitmap.
*   aAttrSet    - Pointer to the set of fonts, bitmaps and colors.
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString()
*     function.
*   aDstFrameNo - Frame within the destination bitmap affected by the drawing
*     operation.
*   aClipRect   - Area to limit the drawing operation (Relative to the top-left
*     corner of the destination bitmap frame).
*   aDstRect    - Area to fill with the text (Relative to the top-left corner
*     of the destination bitmap frame).
*   aSrcPos     - Text output origin (relative to the top-left corner of the
*     text area).
*   aOpacityTL,
*   aOpacityTR,
*   aOpacityBR,
*   aOpacityBL  - Opacity values corresponding to the four corners of aDstRect.
*   aBlend      - If != 0, the drawn pixel will be alpha-blended with the pixel
*     in the background.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDrawAttrText
(
  XBitmap*          aDst,
  XAttrSet*         aAttrSet,
  XAttrString*      aAttrString,
  XInt32            aDstFrameNo,
  XRect             aClipRect,
  XRect             aDstRect,
  XPoint            aSrcPos,
  XInt32            aOpacityTL,
  XInt32            aOpacityTR,
  XInt32            aOpacityBR,
  XInt32            aOpacityBL,
  XBool             aBlend
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrTextSize
*
* DESCRIPTION:
*   The function EwGetAttrTextSize() calculates the rectangular area necessary
*   to draw the given attributed string. The rectangular area is calculated by
*   building an union of all drawing statements of the attributed string.
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString()
*     function.
*
* RETURN VALUE:
*   Returns the size of the rectangular area needed to display the string.
*
*******************************************************************************/
XPoint EwGetAttrTextSize
(
  XAttrString*      aAttrString
);


/*******************************************************************************
* FUNCTION:
*   EwIsAttrTextRTL
*
* DESCRIPTION:
*   The function EwIsAttrTextRTL() returns the basic paragraph direction
*   resulting from the very first processed paragraph in the attributed string.
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString()
*     function.
*
* RETURN VALUE:
*   Returns != 0 if the first paragraph of the attributed string has RTL
*   paragraph direction. Otherwise return 0.
*
*******************************************************************************/
XBool EwIsAttrTextRTL
(
  XAttrString*      aAttrString
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrLinks
*
* DESCRIPTION:
*   The function EwGetNoOfAttrLinks() returns the total number of links stored
*   within the given attributed string aAttrString.
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString()
*     function.
*
* RETURN VALUE:
*   The function returns the number of links or zero if no links are defined in
*   the given attributed string.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrLinks
(
  XAttrString*      aAttrString
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrLinkName
*
* DESCRIPTION:
*   The function EwGetAttrLinkName() returns the name of a link, which is stored
*   within the given attributed string aAttrString. The number of the desired
*   link is passed in the argument aLinkNo. The first link has the number 0,
*   the second 1, ...
*
*   The total number of available links can be determinated by the call to the
*   function EwGetNoOfAttrLinks().
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString()
*     function.
*   aLinkNo     - The number of the affected link. The first link has the
*     number 0, the second 1, ...
*
* RETURN VALUE:
*   The function returns a pointer to the zero terminated string containing the
*   link's name. If the link has no name or the desired link does not exist,
*   null pointer is returned.
*
*   Please note, the returned string is controlled by the Embedded Wizard
*   Runtime Environment EWRTE. If not used anymore, the string is released
*   automatically.
*
*******************************************************************************/
XString EwGetAttrLinkName
(
  XAttrString*      aAttrString,
  XInt32            aLinkNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetNoOfAttrLinkRegions
*
* DESCRIPTION:
*   The function EwGetNoOfAttrLinkRegions() returns the number of rectangular
*   text areas enclosed by the given link aLinkNo within the attributed string
*   aAttrString.
*
*   Due to the line wrap, the text enclosed by a single link can be wrapped in
*   several text lines, so the link area may become a very complex polygon. To
*   describe this polygon, it is divided in several rectangular areas, one for
*   each text line enclosed by the link. To get the origin and the size of an
*   area, the function EwGetAttrLinkRect() should be used.
*
*   The regions are very useful, if a selection frame or an other kind of
*   decoration should be drawn together with the link.
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString()
*     function.
*   aLinkNo     - Number of the affected link. The first link has the number 0,
*     the second 1, ...
*
* RETURN VALUE:
*   The function returns the number of rectangular areas or zero if the desired
*   link is not defined within the attributed string, or the link does not
*   enclose any text.
*
*******************************************************************************/
XInt32 EwGetNoOfAttrLinkRegions
(
  XAttrString*      aAttrString,
  XInt32            aLinkNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrLinkRect
*
* DESCRIPTION:
*   The function EwGetAttrLinkRect() returns the origin and the size of an area
*   occupied by the link aLinkNo within the attributed string aAttrString.
*
*   Due to the line wrap, the text enclosed by a single link can be wrapped in
*   several text lines, so the link area may become a very complex polygon. To
*   describe this polygon, it is divided in several rectangular areas, one for
*   each text line enclosed by the link. The number of the desired rectangular
*   area should be passed in the argument aRegionNo. The first region has the
*   number 0, the second 1, ...
*
*   The total number of available regions can be determinated by the call to the
*   function EwGetNoOfAttrLinkRegions().
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString()
*     function.
*   aLinkNo     - Number of the affected link. The first link has the number 0,
*     the second 1, ...
*   aRegionNo   - Number of the affected region. The first region has the number
*     0, the second 1, ...
*
* RETURN VALUE:
*   Returns the origin and the size of the area relative to the origin of the
*   drawn attributed text. If the desired link or region do not exist, an empty
*   rectangle is returned.
*
*******************************************************************************/
XRect EwGetAttrLinkRect
(
  XAttrString*      aAttrString,
  XInt32            aLinkNo,
  XInt32            aRegionNo
);


/*******************************************************************************
* FUNCTION:
*   EwGetAttrLinkBaseline
*
* DESCRIPTION:
*   The function EwGetAttrLinkBaseline() returns the vertical offset to the base
*   line of the text enclosed by the link aLinkNo within the attributed string
*   aAttrString. The base line is used for vertical text alignment.
*
*   Due to the line wrap, the text enclosed by a single link can be wrapped in
*   several text lines. For each text line a different base line may be used.
*   This function provides you with an access to this base line offset for each
*   text region enclosed by the link. The number of the desired region should be
*   passed in the argument aRegionNo. The first region has the number 0, the
*   second 1, ...
*
*   The total number of available regions can be determinated by the call to the
*   function EwGetNoOfAttrLinkRegions().
*
* ARGUMENTS:
*   aAttrString - Pointer to the memory area containing the parsed, preprocessed
*     drawing statements. This area is created by the EwParseAttrString()
*     function.
*   aLinkNo     - Number of the affected link. The first link has the number 0,
*     the second 1, ...
*   aRegionNo   - Number of the affected region. The first region has the number
*     0, the second 1, ...
*
* RETURN VALUE:
*   The function returns the vertical offset to the base line of the affected
*   region relative to the origin of the drawn attributed text. If the desired
*   link or region do not exist, the function returns 0.
*
*******************************************************************************/
XInt32 EwGetAttrLinkBaseline
(
  XAttrString*      aAttrString,
  XInt32            aLinkNo,
  XInt32            aRegionNo
);


/*******************************************************************************
* FUNCTION:
*   EwCreateBidi
*
* DESCRIPTION:
*   The function EwCreateBidi() creates a new Bidi-Context able to manage Bidi
*   information for at the least as many characters as specified in parameter
*   aMaxSize.
*
*   The Bidi-information in the context is not initialized. Use the function
*   EwBidiInit() for this purpose. If the returned Bidi-Context is not needed
*   anymore, it has to be released by using the function EwFreeBidi().
*
* ARGUMENTS:
*   aMaxSize - The desired capacity for the Bidi-Context expressed in text
*     characters this context should be able to process maximally.
*
* RETURN VALUE:
*   The function returns a pointer referring to the Bidi-Context.
*
*******************************************************************************/
XHandle EwCreateBidi
(
  XInt32            aMaxSize
);


/*******************************************************************************
* FUNCTION:
*   EwFreeBidi
*
* DESCRIPTION:
*   The function EwFreeBidi() frees all resources associated with the given
*   Bidi-Context aBidi again.
*
* ARGUMENTS:
*   aBidi - The Bidi-Context to release.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeBidi
(
  XHandle           aBidi
);


/*******************************************************************************
* FUNCTION:
*   EwBidiIsNeeded
*
* DESCRIPTION:
*   The function EwBidiIsNeeded() returns != 0 if further Bidi processing is
*   required. If the original text does not contain any RTL signs nor Bidi
*   control signs, the function returns 0.
*
* ARGUMENTS:
*   aBidi - The Bidi-Context to query the mode.
*
* RETURN VALUE:
*   Returns != 0 if the context describes text which needs Bidi processing.
*
*******************************************************************************/
XBool EwBidiIsNeeded
(
  XHandle           aBidi
);


/*******************************************************************************
* FUNCTION:
*   EwBidiIsRTL
*
* DESCRIPTION:
*   The function EwBidiIsRTL() returns the basic paragraph direction stored in
*   the given Bidi-Context.
*
* ARGUMENTS:
*   aBidi - The Bidi-Context to query the paragraph direction.
*
* RETURN VALUE:
*   Returns != 0 if the context describes text with RTL paragraph direction.
*   Otherwise return 0.
*
*******************************************************************************/
XBool EwBidiIsRTL
(
  XHandle           aBidi
);


/*******************************************************************************
* FUNCTION:
*   EwBidiGetCharLevel
*
* DESCRIPTION:
*   The function EwBidiGetCharLevel() returns the embedding level corresponding
*   to the character with the specified number.
*
* ARGUMENTS:
*   aBidi  - The Bidi-Context to query the embedding level.
*   aIndex - Index identifying the character. The characters are counted
*     starting with 0.
*
* RETURN VALUE:
*   Returns a number in range 0 .. 127 identifying the embedding level of the
*   respective character as it is stored in the Bidi context. If the index
*   addresses a non existing character, the paragraph level is returned. If
*   the context is not valid, 0 is returned.
*
*******************************************************************************/
XInt32 EwBidiGetCharLevel
(
  XHandle           aBidi,
  XInt32            aIndex
);


/*******************************************************************************
* FUNCTION:
*   EwBidiInit
*
* DESCRIPTION:
*   The function EwBidiInit() evaluates the characters in aString and stores in
*   the Bidi-Context aBidi for every evaluated character the corresponding Bidi
*   type. Finally the function determines the paragraph's base direction and
*   stores it also in the context.
*
*   Please note, before invoking this function you should create a new Bidi
*   context with enough capacity so all string characters can be stored in it.
*   If the passed context is invalid or too small, the function fails.
*
* ARGUMENTS:
*   aBidi          - Bidi-Context where to store the Bidi-Information.
*   aString        - The original string to process.
*   aCount         - The number of characters in aString to process. If this
*     parameter is negative, the function processes all characters found in the
*     string.
*   aBaseDirection - Determines the default paragraph level. If this parameter
*     is 0, the paragraph level is set to 0 (LTR). If this parameter > 0, the
*     paragraph level is set 1 (RTL). If this parameter is < 0, the paragraph
*     level is determined automatically depending on the first strong character
*     in the string.
*
* RETURN VALUE:
*   The function returns != 0 if the original string contains at least one RTL
*   or any Bidi-Control sign. If the string does not contain any of those signs
*   the function returns 0. Thereupon no further Bidi processing is required.
*
*******************************************************************************/
XBool EwBidiInit
(
  XHandle           aBidi,
  XChar*            aString,
  XInt32            aCount,
  XInt32            aBaseDirection
);


/*******************************************************************************
* FUNCTION:
*   EwBidiProcess
*
* DESCRIPTION:
*   The function EwBidiProcess() performs the main part of the Bidi-Algorithm
*   deriving from the characters in the given string aString and their Bidi-
*   Types provided in aBidi the corresponding direction levels. Finally the
*   estimated levels are stored in aBidi for every character in aString.
*
* ARGUMENTS:
*   aBidi   - The Bidi-Context containing Bidi-Types for the characters in the
*     string aString.
*   aString - Characters to apply the Bidi-Algorithm.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBidiProcess
(
  XHandle           aBidi,
  XChar*            aString
);


/*******************************************************************************
* FUNCTION:
*   EwBidiApplyShaping
*
* DESCRIPTION:
*   The function EwBidiApplyShaping() performs the part of the Bidi-Algorithm
*   needed to handle the special case of Arabic glyphs and mandatory ligatures.
*   The function evaluates the characters in the string aString and adapts all
*   found Arabic glyphs accordingly.
*
* ARGUMENTS:
*   aBidi   - The Bidi-Context containing Bidi-Types for the characters in the
*     string aString.
*   aString - Characters to apply the Shaping-Algorithm.
*   aAttrs  - In case of an attributed string points to an array containing
*     the attributes (in particular the font numbers) for the corresponding
*     characters from aString. If aString is not an attributed string, aAttrs
*     is NULL.
*   aFonts  - An array containing the fonts used in aString. The fonts are
*     selected by the font-number value from aAttrs attributes. If aString
*     is not an attributed string, aFonts contains just one entry valid for
*     all characters in the string.
*   aNoOfFonts - Number of fonts in the array aFonts.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBidiApplyShaping
(
  XHandle           aBidi,
  XChar*            aString,
  XUInt32*          aAttrs,
  XFont**           aFonts,
  XInt32            aNoOfFonts
);


/*******************************************************************************
* FUNCTION:
*   EwBidiMirrorGlyphs
*
* DESCRIPTION:
*   The function EwBidiMirrorGlyphs() performs the part of the Bidi-Algorithm
*   responsable for the mirroring of glyphs within level-runs with reversed
*   writing direction. For every character within aString which is signed as
*   RTL the function verifies whether there is mirror version of the sign and
*   replaces it with this version.
*
* ARGUMENTS:
*   aBidi   - The Bidi-Context containing Bidi-Types for the characters in the
*     string aString.
*   aString - Characters to mirror.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBidiMirrorGlyphs
(
  XHandle           aBidi,
  XChar*            aString
);


/*******************************************************************************
* FUNCTION:
*   EwBidiCompleteRow
*
* DESCRIPTION:
*   The function EwBidiCompleteRow() performs the part of the Bidi-Algorithm
*   after text wrap has been applied on a row. Accordingly it runs the final
*   rules of the Bidi-Algorithm on a single text row only.
*
* ARGUMENTS:
*   aBidi     - The Bidi-Context containing Bidi-Types for the text row.
*   aRowStart - The number of the data entry in aBidi addressing the begin of
*     the row to process.
*   aRowEnd   - The number of the data entry in aBidi addressing the end of
*     the row to process.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBidiCompleteRow
(
  XHandle           aBidi,
  XInt32            aRowStart,
  XInt32            aRowEnd
);


/*******************************************************************************
* FUNCTION:
*   EwBidiReorderChars
*
* DESCRIPTION:
*   The function EwBidiReorderChars() performs the reordering of the orignal
*   text entities within a text row according to the previously estimated Bidi
*   run levels. The operation is limited to a single text row.
*
* ARGUMENTS:
*   aBidi     - The Bidi-Context containing Bidi-Types for the text row.
*   aRowStart - The number of the data entry in aBidi addressing the begin of
*     the row to process.
*   aRowEnd   - The number of the data entry in aBidi addressing the end of
*     the row to process.
*   aChars    - Pointer addressing the first character of the affected text
*     row to reorder its content.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBidiReorderChars
(
  XHandle           aBidi,
  XInt32            aRowStart,
  XInt32            aRowEnd,
  XChar*            aChars
);


/*******************************************************************************
* FUNCTION:
*   EwBidiReorderDWords
*
* DESCRIPTION:
*   The function EwBidiReorderDWords() performs the reordering of the orignal
*   text entities within a text row according to the previously estimated Bidi
*   run levels. The operation is limited to a single text row and unlike the
*   above function it is intended to handle with text entities of 32-bit size.
*
* ARGUMENTS:
*   aBidi     - The Bidi-Context containing Bidi-Types for the text row.
*   aRowStart - The number of the data entry in aBidi addressing the begin of
*     the row to process.
*   aRowEnd   - The number of the data entry in aBidi addressing the end of
*     the row to process.
*   aDWords   - Pointer addressing the first entity of the affected text row
*     to reorder its content.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBidiReorderDWords
(
  XHandle           aBidi,
  XInt32            aRowStart,
  XInt32            aRowEnd,
  XUInt32*          aDWords
);


/*******************************************************************************
* FUNCTION:
*   EwBidiReorderIndex
*
* DESCRIPTION:
*   The function EwBidiReorderIndex() estimates the resulting index of a given
*   text entity after applying the Bidi reorder algorithm. In other words, it
*   returns the display position of a character within the original text row
*   after the row has been reordered.
*
* ARGUMENTS:
*   aBidi     - The Bidi-Context containing Bidi-Types for the text row.
*   aRowStart - The number of the data entry in aBidi addressing the begin of
*     the row to process.
*   aRowEnd   - The number of the data entry in aBidi addressing the end of
*     the row to process.
*   aIndex    - The number of the entity within the text row to get its display
*     position.
*
* RETURN VALUE:
*   Returns the display position for the given text entity aIndex.
*
*******************************************************************************/
XInt32 EwBidiReorderIndex
(
  XHandle           aBidi,
  XInt32            aRowStart,
  XInt32            aRowEnd,
  XInt32            aIndex
);


/*******************************************************************************
* FUNCTION:
*   EwBidiReverseReorderIndex
*
* DESCRIPTION:
*   The function EwBidiReverseReorderIndex() estimates the original index of a
*   given text entity before applying the Bidi reorder algorithm. In other
*   words, it returns the position of the character within the original text
*   row corresponding to the given display position in the reordered row.
*
* ARGUMENTS:
*   aBidi     - The Bidi-Context containing Bidi-Types for the text row.
*   aRowStart - The number of the data entry in aBidi addressing the begin of
*     the row to process.
*   aRowEnd   - The number of the data entry in aBidi addressing the end of
*     the row to process.
*   aIndex    - The number of the entity within the final reordered text row
*     to get its original position.
*
* RETURN VALUE:
*   Returns the original position for the given text entity aIndex.
*
*******************************************************************************/
XInt32 EwBidiReverseReorderIndex
(
  XHandle           aBidi,
  XInt32            aRowStart,
  XInt32            aRowEnd,
  XInt32            aIndex
);


/*******************************************************************************
* FUNCTION:
*   EwBidiGetTypesAndLevels
*
* DESCRIPTION:
*   The function EwBidiGetTypesAndLevels() exists for test purpose only. The
*   function copies all estimates Bidi types and levels from aBidi context to
*   the corresponding entries in the both arrays aTypes and aLevels. Note, the
*   arrays have to be large enough!
*
* ARGUMENTS:
*   aBidi   - The Bidi-Context containing the estimated types and levels.
*   aTypes  - Array to fill with the estimated Bidi types.
*   aLevels - Array to fill with the estimated embedding levels.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBidiGetTypesAndLevels
(
  XHandle           aBidi,
  unsigned char*    aTypes,
  unsigned char*    aLevels
);


/*******************************************************************************
* FUNCTION:
*   EwIndexToColor
*
* DESCRIPTION:
*   The function EwIndexToColor() returns the color entry from the global CLUT
*   (palette).
*
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aIndex - Index of the desired color entry within the global CLUT/palette.
*
* RETURN VALUE:
*   The color value or a transparent color if there is no global CLUT available
*   or the entry index is invalid.
*
*******************************************************************************/
XColor EwIndexToColor
(
  XInt32            aIndex
);


/*******************************************************************************
* FUNCTION:
*   EwColorToIndex
*
* DESCRIPTION:
*   The function EwColorToIndex() determines an entry within the global CLUT/
*   palette, which corresponds to the given color (nearest color matching).
*
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aColor - Color to be found in the global CLUT/palette.
*
* RETURN VALUE:
*   The determined global CLUT/palette entry in range 0 .. 255. If there is no
*   global CLUT/palette functionality available, the returned value is 0.
*
*******************************************************************************/
XUInt8 EwColorToIndex
(
  XColor            aColor
);


/*******************************************************************************
* FUNCTION:
*   EwAllocUserColor
*
* DESCRIPTION:
*   The function EwAllocUserColor() tries to store the given user defined color
*   within an empty area in the global CLUT/palette. User defined colors exist
*   for external applications (native applications) only. In this manner the
*   external application can manage its own/private set of color values.
*
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aColor - Color to be stored in the global CLUT/palette.
*
* RETURN VALUE:
*   The global CLUT/palette entry, where the color could be stored. If there
*   are no entries available for new colors, 0 is returned.
*
*******************************************************************************/
XUInt8 EwAllocUserColor
(
  XColor           aColor
);


/*******************************************************************************
* FUNCTION:
*   EwFreeUserColor
*
* DESCRIPTION:
*   The function EwFreeUserColor() removes the user defined color entry from
*   the global CLUT/palette. After this the affected entry remains empty. User
*   defined colors exist for external applications (native applications) only.
*   In this manner the external application can manage its own/private set of
*   color values.
*
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aIndex - Index of the user color entry within the global CLUT/palette.
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwFreeUserColor
(
  XInt32            aIndex
);


/*******************************************************************************
* FUNCTION:
*   EwSetUserColor
*
* DESCRIPTION:
*   The function EwSetUserColor() changes the color value of an user defined
*   color within the global CLUT/palette. User defined colors exist for external
*   applications (native applications) only. In this manner the application can
*   manage its own/private set of color values.
*
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aIndex - Index of the user color entry within the global CLUT/palette.
*   aColor - New color to be replace the old color value.
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwSetUserColor
(
  XInt32            aIndex,
  XColor            aColor
);


/*******************************************************************************
* FUNCTION:
*   EwFindUserColor
*
* DESCRIPTION:
*   The function EwFindUserColor() searches the global CLUT/palette for an entry
*   containing exactly the given user defined color. If no corresponding entry
*   could be found, the function returns 0. In this case, the best fit color can
*   be found by using the function EwColorToIndex().
*
*   User defined colors exist for external applications (native applications)
*   only. In this manner the application can manage its own/private set of color
*   values.
*
*   This functionality is available on Index8, palette based target systems
*   only.
*
* ARGUMENTS:
*   aColor - Color to be found within the user defined color area of the global
*     CLUT/palette.
*
* RETURN VALUE:
*   The global CLUT/palette entry, where the desired color is stored. If no
*   corresponding entry could be found, the function returns 0.
*
*******************************************************************************/
XUInt8 EwFindUserColor
(
  XColor            aColor
);


/*******************************************************************************
* FUNCTION:
*   EwGetOverlayArea
*
* DESCRIPTION:
*  The function EwGetOverlayArea() is used internaly by the Mosaic framework
*  to obtain the additional area occupied by the overlay image. The Mosaic
*  framework uses the returned area to optimize the screen redrawing. Without
*  this information, the update process would result in an incomplete screen
*  showing only parts of the overlay.
*
*  Please note, this function can't supress the overlay from being shown.
*  It exists only for optimization purpose.
*
* ARGUMENTS:
*   aVisible - If != 0, the function returns the currently visible area of
*     the overlay. If the overlay is hidden, the function will return an
*     empty area.
*   aUpdate  - If != 0, the function returns the current update area of the
*     overlay. The update area results when overlay changes between the
*     visible and hidden state. If no state transition is recorded, an empty
*     area is returned.
*
* RETURN VALUE:
*   The currently visible area or the update area of the overlay. If no area
*   is available, the function returns an empty rectangle.
*
*******************************************************************************/
XRect EwGetOverlayArea
(
  XBool             aVisible,
  XBool             aUpdate
);


#ifdef __cplusplus
  }
#endif

#endif /* EWGFX_H */


/* pba */
