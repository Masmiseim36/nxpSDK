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
*   This is an internal header of the Embedded Wizard Graphics Engine EWGFX.
*
*   The module 'ewgfxdriver' implements the platform independent layer to the
*   software pixel driver. Its functionality provides a kind of reserve in case
*   the underlying graphics subsystem can not handle the necessary operations.
*   For example, if there is no support for index8 surfaces, 'ewgfxdriver' will
*   recompense it by a software emulated index8 surface including all necessary
*   drawing operations.
*
*   This header file provides a generic and platform independent interface to
*   the software pixel driver functionality. The interface consists of two
*   categories of functions:
*
*   1. 'per row' worker functions. These functions build the low-level software
*      pixel driver. They are intended to perform drawing operations optimized
*      for the particular pixel format and the drawing mode. The declarations,
*      any way, are platform independent.
*
*   2. 'per area' drawing functions. These functions provide the top-level view
*      to the software pixel driver. They are intended to drive the 'per row'
*      worker functions for the affected fill/copy/warp operation. When called,
*      the top-level function will receive a pointer to one of the low-level
*      worker functions. The passed worker function can be used thereupon to
*      perform row-wise the operation without any dependency to the underlying
*      pixel format.
*
*   Additionally, the header file provides generic declarations to simplify
*   the integration of external graphics subsystem drivers into the Embedded
*   Wizard Graphics Engine.
*
*******************************************************************************/

#ifndef EWGFXDRIVER_H
#define EWGFXDRIVER_H


#ifdef __cplusplus
  extern "C"
  {
#endif


/*******************************************************************************
* MACRO:
*   EW_RED
*   EW_GREEN
*   EW_BLUE
*   EW_ALPHA
*
* DESCRIPTION:
*   The following macros provide a generic way to extract the RGBA color
*   channels from the RGBA8888 aColor value as it is passed to all driver
*   functions.
*
*   Note, the RGBA8888 color format is the internal, platform independent way
*   to store color values.
*
*******************************************************************************/
#define EW_RED( aColor )   ((( aColor ) >> 0  ) & 0xFF )
#define EW_GREEN( aColor ) ((( aColor ) >> 8  ) & 0xFF )
#define EW_BLUE( aColor )  ((( aColor ) >> 16 ) & 0xFF )
#define EW_ALPHA( aColor ) ((( aColor ) >> 24 ) & 0xFF )


/*******************************************************************************
* MACRO:
*   EW_PIXEL_FORMAT_XXX
*
* DESCRIPTION:
*   The following enumeration defines the 3 fundamental surface formats. They
*   are needed amongst other things to specify the desired pixel format, when
*   creating new surfaces or when accessing surface memory directly.
*
* ELEMENTS:
*   EW_PIXEL_FORMAT_SCREEN - Represents the pixel format of the frame buffer.
*     Usually this format corresponds to the EW_PIXEL_FORMAT_NATIVE. In special
*     cases, however, it is necessary to distinguish between operations to run
*     on the frame buffer or on the normal (native) destination when these use
*     different color formats.
*     The pixel format does not make any assumption regarding the respective
*     color space, color channels, etc. In this manner the Graphics Engine
*     remains platform and color independent.
*     Please note, surfaces with this color format can serve as a destination
*     only in all drawing operations.
*   EW_PIXEL_FORMAT_NATIVE - Represents the main pixel format valid within the
*     particular graphics subsystem without any assumptions about the respective
*     color space, color channels, etc. This is the pixel format, the subsystem
*     is working with internally. In this manner the Graphics Engine remains
*     platform and color independent.
*     Please note, surfaces with this color format can serve as a destination
*     and source in all drawing operations.
*   EW_PIXEL_FORMAT_INDEX8 - Universal 8 bit per pixel CLUT format. This format
*     expects an additional color look-up table for translation between an 8 bit
*     index and the native color value.
*     Please note, surfaces with this color format can serve as a source only
*     in all drawing operations.
*   EW_PIXEL_FORMAT_ALPHA8 - Universal 8 bit per pixel alpha only format. Useful
*     as storage for glyphs, masks, etc.
*     Please note, surfaces with this color format can serve as a source only
*     in all drawing operations.
*   EW_PIXEL_FORMAT_RGB565 - Represents a color-only pixel format with 16-bit
*     per pixel. The support for this format is optional depending on the used
*     target system and the Native color format in the target system.
*     Please note, surfaces with this color format can serve as a source only
*     in all drawing operations.
*
*******************************************************************************/
#define EW_PIXEL_FORMAT_SCREEN -1
#define EW_PIXEL_FORMAT_NATIVE  0
#define EW_PIXEL_FORMAT_INDEX8  1
#define EW_PIXEL_FORMAT_ALPHA8  2
#define EW_PIXEL_FORMAT_RGB565  3


/*******************************************************************************
* MACRO:
*   EW_DRIVER_VARIANT_XXX
*
* DESCRIPTION:
*   Following macros specify the basic color formats supported by the Graphics
*   Engine. These are used for verification purpose and plausibility tests.
*
*******************************************************************************/
#define EW_DRIVER_VARIANT_RGBA8888  1
#define EW_DRIVER_VARIANT_RGBA4444  2
#define EW_DRIVER_VARIANT_RGB565A8  3
#define EW_DRIVER_VARIANT_RGB555A8  4
#define EW_DRIVER_VARIANT_YUVA8888  5
#define EW_DRIVER_VARIANT_LUMA44    6
#define EW_DRIVER_VARIANT_RGB565    7
#define EW_DRIVER_VARIANT_RGB888    8
#define EW_DRIVER_VARIANT_INDEX8    11
#define EW_DRIVER_VARIANT_ALPHA8    12
#define EW_DRIVER_VARIANT_RGB565_RGBA8888                                      \
  (( EW_DRIVER_VARIANT_RGB565 << 8 ) | EW_DRIVER_VARIANT_RGBA8888 )
#define EW_DRIVER_VARIANT_RGB888_RGBA8888                                      \
  (( EW_DRIVER_VARIANT_RGB888 << 8 ) | EW_DRIVER_VARIANT_RGBA8888 )


/*******************************************************************************
* VARIABLE:
*   EwPixelDriverVariant
*
* DESCRIPTION:
*   This global variable stores the target basic color format supported by this
*   pixel driver. It is used at the runtime to verify, whether correct pixel
*   driver are linked together with the application.
*
*   This variable can take one of EW_DRIVER_VARIANT_XXX values.
*
*******************************************************************************/
extern const int EwPixelDriverVariant;


/*******************************************************************************
* TYPE:
*   XSurfaceMemory
*
* DESCRIPTION:
*   The XSurfaceMemory structure provides a lightweight storage for adresses to
*   color planes and (if any) to the color table of a previously locked surface.
*   It is used by the software pixel driver to read and modify surface content.
*
* ELEMENTS:
*   Pixel1  - Pointer to the first surface plane.
*   Pitch1X - Distance between two pixel columns of the first surface plane.
*   Pitch1Y - Distance between two pixel rows of the first surface plane.
*   Pixel2  - Pointer to the second (optional) surface plane.
*   Pitch2X - Distance between two pixel columns of the second surface plane.
*   Pitch2Y - Distance between two pixel rows of the second surface plane.
*   Clut    - Pointer to the (optional) color table of the surface. In order
*     to cover all color formats, a single entry of the clut is always 32 bit
*     wide - even when the target color format is 16 or 8 bit wide.
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
  unsigned int*     Clut;
} XSurfaceMemory;


/*******************************************************************************
* TYPE:
*   XGradient
*
* DESCRIPTION:
*   The XGradient structure provides a storage for the components of a 2D color
*   or 2D opacity gradient. It defines a start value and slopes. For precision
*   purpose, the values are codes as 12.20 fixed point numbers.
*
* ELEMENTS:
*   R0, R1, R2, R3 - Startvalues and slopes for the red channel.
*   G0, G1, G2, G3 - Startvalues and slopes for the green channel.
*   B0, B1, B2, B3 - Startvalues and slopes for the blue channel.
*   A0, A1, A2, A3 - Startvalues and slopes for the alpha channel.
*   Width, Height  - Size of the gradient area.
*   InvWidth,
*   InvHeight      - Reciprocal values of the gradient area width and height.
*   IsVertical     - != 0 if the gradient has a vertical trend.
*   IsHorizontal   - != 0 if the gradient has a horizontal trend.
*
*******************************************************************************/
typedef struct
{
  int               R0, R1, R2, R3;
  int               G0, G1, G2, G3;
  int               B0, B1, B2, B3;
  int               A0, A1, A2, A3;
  int               Width;
  int               Height;
  int               InvWidth;
  int               InvHeight;
  int               IsVertical;
  int               IsHorizontal;
} XGradient;


/*******************************************************************************
* PROTOTYPE:
*   XLineWorker
*
* DESCRIPTION:
*   The following type declares a prototype for a worker function, which should
*   perform the low level software 'draw line pixel' operation.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aX, aY    - Position of the pixel to draw.
*   aGradient - Color information to draw the pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XLineWorker)
(
  XSurfaceMemory*   aDst,
  int               aX,
  int               aY,
  XGradient*        aGradient
);


/*******************************************************************************
* PROTOTYPE:
*   XFillWorker
*
* DESCRIPTION:
*   The following type declares a prototype for a worker function, which should
*   perform the low level software 'fill pixel row' operation.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the affected row.
*   aWidth    - Length of the row in pixel.
*   aGradient - Color information to fill the row.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XFillWorker)
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* PROTOTYPE:
*   XCopyWorker
*
* DESCRIPTION:
*   The following type declares a prototype for a worker function, which should
*   perform the low level software 'copy pixel row' operation.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Length of the row in pixel.
*   aGradient - Color or opacity information to modulate the copied pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XCopyWorker)
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* PROTOTYPE:
*   XWarpWorker
*
* DESCRIPTION:
*   The following type declares a prototype for a worker function, which should
*   perform the low level software 'warp pixel row' operation.
*
* ARGUMENTS:
*   aDst          - Pointer to the first pixel of the destination row.
*   aSrc          - Pointer to the first pixel of the source surface area to
*     project on the destination row.
*   aWidth        - Length of the row in pixel.
*   aS, aT        - Source surface coordinates corresponding to the first pixel
*     of the destination row.
*   aSS, aTS      - Factors to interpolate the s and t coefficients for next
*     following pixel of the destination row.
*   aSrcWidth,
*   aSrcHeight    - Size of the source area in pixel to limit the operation.
*   aGradient     - Color or opacity information to modulate the copied pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XWarpWorker)
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* PROTOTYPE:
*   XLineDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which should perform the 'draw line' operation by using solid or
*   gradient color values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the screen/native destination surface.
*   aDstX1,
*   aDstY1      - Start position to draw the line (relative to the top-left
*      corner of the destination surface).
*   aDstX2,
*   aDstY2      - End position to draw the line (relative to the top-left
*      corner of the destination surface). The pixel at the end position does
*      not belong to the line - it is invisible. In this manner polylines can
*      be drawn.
*   aClipX,
*   aClipY,
*   aClipWidth,
*   aClipHeight - Optional clipping area relative to the top-left corner of the
*     destination surface. No pixel outside this area may be drawn.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 2 RGBA8888 color values. The both color values do
*     correspond to the start and to the end pixel of the drawn line.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XLineDriver)
(
  unsigned long     aDstHandle,
  int               aDstX1,
  int               aDstY1,
  int               aDstX2,
  int               aDstY2,
  int               aClipX,
  int               aClipY,
  int               aClipWidth,
  int               aClipHeight,
  int               aBlend,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XFillDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which should perform the 'fill rectangular area' operation by
*   using solid or gradient color values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the screen/native destination surface.
*   aDstX,
*   aDstY       - Origin of the area to fill (relative to the top-left corner
*      of the destination surface).
*   aWidth,
*   aHeight     - Size of the area to fill.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 RGBA8888 color values. The four color values do
*     correspond to the four corners of the area: top-left, top-right, bottom-
*     right and bottom-left.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XFillDriver)
(
  unsigned long     aDstHandle,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XCopyDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which should perform the 'copy rectangular area' operation from a
*   native, index8, alpha8 or rgb565 surface to a screen or native surface. The
*   copied pixel can optionally be modulated by solid or gradient color/opacity
*   values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the screen/native destination surface.
*   aSrcHandle  - Handle to the native/index8/alpha8/rgb565 source surface.
*   aDstX,
*   aDstY       - Origin of the area to fill with the copied source surface
*     pixel (relative to the top-left corner of the destination surface).
*   aWidth,
*   aHeight     - Size of the area to fill with the copied source surface pixel.
*   aSrcX,
*   aSrcY       - Origin of the area to copy from the source surface.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 color values. These four values do correspond
*     to the four corners of the area: top-left, top-right, bottom-right and
*     bottom-left.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XCopyDriver)
(
  unsigned long     aDstHandle,
  unsigned long     aSrcHandle,
  int               aDstX,
  int               aDstY,
  int               aSrcX,
  int               aSrcY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XTileDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which will perform the 'multiple copy rectangular area' operation
*   from a native, index8, alpha8 or rgb565 surface to a screen or native surface.
*   The copied pixel can be modulated by solid or gradient color/opacity values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the screen/native destination surface.
*   aSrcHandle  - Handle to the native/index8/alpha8/rgb565 source surface.
*   aDstX,
*   aDstY       - Origin of the area to fill with the copied source surface
*     pixel (relative to the top-left corner of the destination surface).
*   aDstWidth,
*   aDstHeight  - Size of the area to fill with the copied source surface pixel.
*   aSrcX,
*   aSrcY       - Origin of the area to copy from the source surface.
*   aSrcWidth,
*   aSrcHeight  - Size of the source area to copy.
*   aOfsX,
*   aOfsY       - Offset to the first visible source pixel at the origin of the
*     destination area.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 color values. These four values do correspond
*     to the four corners of the area: top-left, top-right, bottom-right and
*     bottom-left.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XTileDriver)
(
  unsigned long     aDstHandle,
  unsigned long     aSrcHandle,
  int               aDstX,
  int               aDstY,
  int               aDstWidth,
  int               aDstHeight,
  int               aSrcX,
  int               aSrcY,
  int               aSrcWidth,
  int               aSrcHeight,
  int               aOfsX,
  int               aOfsY,
  int               aBlend,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XWarpDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which can perform the 'copy and warp rectangular area' operation
*   from native, index8, alpha8 or rgb565 surface to a screen or native surface.
*   The copied pixel can optionally be modulated by solid or gradient color/
*   opacity values.
*
*   The warp operation abstracts the scale, rotate and perspective projection
*   operations. The operation is limited to convex polygons only. In case of a
*   non convex polygon, the operation may fail.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the screen/native destination surface.
*   aSrcHandle  - Handle to the native/index8/alpha8/rgb565 source surface.
*   DstX1,
*   DstY1,
*   DstW1,
*   ...
*   DstX4,
*   DstY4,
*   DstW4       - Coordinates of the polygon to fill with the source bitmap as
*     floating point values for sub-pixel precision (Relative to the top-left
*     corner of the destination bitmap). The 'W' coefficients do control the
*     perspective distortion as it is realized in 3D graphics hardware (see
*     OpenGL homogeneous coordinates for more information).
*   aSrcX,
*   aSrcY       - Origin of the area to copy from the source surface.
*   aSrcWidth,
*   aSrcHeight  - Size of the source area to copy.
*   aClipX,
*   aClipY,
*   aClipWidth,
*   aClipHeight - Optional clipping area relative to the top-left corner of the
*     destination surface. No pixel outside this area may be drawn.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aFilter     - != 0 if the bi-linear filter should be applied to the copied
*      pixel. This argument is optional and controls the output quality only.
*   aColors     - Array with 4 color values. These four values do correspond to
*     the respective corners of the polygon.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XWarpDriver)
(
  unsigned long     aDstHandle,
  unsigned long     aSrcHandle,
  float             aDstX1,
  float             aDstY1,
  float             aDstW1,
  float             aDstX2,
  float             aDstY2,
  float             aDstW2,
  float             aDstX3,
  float             aDstY3,
  float             aDstW3,
  float             aDstX4,
  float             aDstY4,
  float             aDstW4,
  int               aSrcX,
  int               aSrcY,
  int               aSrcWidth,
  int               aSrcHeight,
  int               aClipX,
  int               aClipY,
  int               aClipWidth,
  int               aClipHeight,
  int               aBlend,
  int               aFilter,
  unsigned long*    aColors
);


/*******************************************************************************
* PROTOTYPE:
*   XPolygonDriver
*
* DESCRIPTION:
*   The following type declares a prototype for an external graphics subsystem
*   function, which should perform the 'fill polygon' operation by using solid
*   or gradient color values. The polygon is described by values in the aPaths
*   parameter.
*
*  aPaths stores the edges as a serie of X,Y pairs starting always with a value
*  specifying the number of existing edges. With this approach one path can
*  consist of several sub-paths. The end of the list is determined by a sub-
*  path with 0 edges.
*
*  +-------+------+------+------+------+------+------+-------+     +-----+
*  | edges |  X0  |  Y0  |  X1  |  Y1  |  X2  |  Y2  | edges | ... |  0  |
*  +-------+------+------+------+------+------+------+-------+     +-----+

*
* ARGUMENTS:
*   aDstHandle      - Handle to the screen/native destination surface.
*   aPaths          - An array containing the path data. The array starts with
*     the number of edges a path is composed of. Then follow the coordinates of
*     all path corners as X,Y pairs. After the last coordinate pair next path
*     can follow starting again with the number of edges. The end of the path
*     data is signed with 0. The X,Y coordinates are stored as signed integer
*     with 4-bit fixpoint precision.
*   aDstX,
*   aDstY           - Origin of the area to fill (relative to the top-left
*     corner of the destination surface).
*   aWidth,
*   aHeight         - Size of the area to fill.
*   aBlend          - = 0 if the operation should be performed with alpha
*     blending.
*   aAntialiased    - If != 0, the antialiasing should be applied to every
*     pixel.
*   aNonZeroWinding - Controls the fill rule to be used by the algorithm. If
*    this parameter is == 0, the even-odd fill rule is used. If this parameter
*    is != 0, the non-zero winding rule is used.
*   aColors         - Array with 4 RGBA8888 color values. The four color values
*     do correspond to the four corners of the area: top-left, top-right,
*     bottom-right and bottom-left.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XPolygonDriver)
(
  unsigned long     aDstHandle,
  int*              aPaths,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  int               aAntialiased,
  int               aNonZeroWinding,
  unsigned long*    aColors
);


/*******************************************************************************
* FUNCTION:
*   EwCreateScreenSurface
*
* DESCRIPTION:
*   The function EwCreateScreenSurface() has the job to create and return a new
*   screen surface with the given size. If no surface could be created due to
*   memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function creates a pure software surface
*   within the CPU address space only.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateScreenSurface
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyScreenSurface
*
* DESCRIPTION:
*   The function EwDestroyScreenSurface() has the job to release resources of
*   a previously created screen surface.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function destroys pure software surfaces
*   previously created by the function EwCreateScreenSurface() only.
*
* ARGUMENTS:
*   aSurface - Handle to the surface to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyScreenSurface
(
  unsigned long     aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwGetScreenSurfaceMemory
*
* DESCRIPTION:
*   The function EwGetScreenSurfaceMemory() has the job to obtain pointers to
*   the memory planes of the given surface.
*
*   The returned pointers are calculated for the given pixel position aX, aY.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function can handle with the pure software
*   surfaces previously created by the function EwCreateScreenSurface() only.
*
* ARGUMENTS:
*   aSurface     - Handle to the surface to obtain the addresses.
*   aX, aY       - Position within the surface to calculate the pixel address.
*   aReserved    - Should be 0.
*   aWriteAccess - If != 0 the caller has the intention to modify the returned
*     memory content.
*   aMemory      - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwGetScreenSurfaceMemory
(
  unsigned long     aSurface,
  int               aX,
  int               aY,
  int               aReserved,
  int               aWriteAccess,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwGetScreenSurfaceMemSize
*
* DESCRIPTION:
*   The function EwGetScreenSurfaceMemSize() has the job to determine the number
*   of memory bytes occupied by a screen surface with the given size.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface in pixel.
*
* RETURN VALUE:
*   If successful, the function return the surface size in bytes.
*
*******************************************************************************/
int EwGetScreenSurfaceMemSize
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateNativeSurface
*
* DESCRIPTION:
*   The function EwCreateNativeSurface() has the job to create and return a new
*   native surface with the given size. If no surface could be created due to
*   memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function creates a pure software surface
*   within the CPU address space only.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateNativeSurface
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateConstNativeSurface
*
* DESCRIPTION:
*   The function EwCreateConstNativeSurface() has the job to create and return
*   a new native surface with the given size and pixel data content. Important
*   here is the fact that the function doesn't reserve any memory to store the
*   pixel information. Instead it, the surface associates the pixel data passed
*   in the parameter aMemory.
*
*   This function is thus intended to create surfaces from the pixel data stored
*   directly within a ROM area. Accordingly it is essential that the ROM content
*   does exactly correspond to the native surface pixel format. It includes the
*   order of color channels, premultiplication with alpha value, etc..
*
*   If no surface could be created due to memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality.
*
*   Please note, the surfaces created with this function are signed internally
*   as constant. Trying to obtrain write-access to such surface will result in
*   a runtime error.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*   aMemory - Structure to pass the ROM address information.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateConstNativeSurface
(
  int               aWidth,
  int               aHeight,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyNativeSurface
*
* DESCRIPTION:
*   The function EwDestroyNativeSurface() has the job to release resources of
*   a previously created native surface.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function destroys pure software surfaces
*   previously created by the function EwCreateNativeSurface() only.
*
* ARGUMENTS:
*   aSurface - Handle to the surface to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyNativeSurface
(
  unsigned long     aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwGetNativeSurfaceMemory
*
* DESCRIPTION:
*   The function EwGetNativeSurfaceMemory() has the job to obtain pointers to
*   the memory planes and if existing to the color table of the given surface.
*
*   The returned pointers are calculated for the given pixel position aX, aY and
*   in case of an existing color table, to the entry at the position aIndex.
*
*   Please note, if the affected surface has been created with constant pixel
*   information by using EwCreateConstNativeSurface(), you can't obtain the
*   write access to such surface. In such case the function will fail with the
*   return value 0.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function can handle with the pure software
*   surfaces previously created by the function EwCreateNativeSurface() and
*   EwCreateConstNativeSurface() only.
*
* ARGUMENTS:
*   aSurface     - Handle to the surface to obtain the addresses.
*   aX, aY       - Position within the surface to calculate the pixel address.
*   aIndex       - Index within the color table to calculate the CLUT address.
*   aWriteAccess - If != 0 the caller has the intention to modify the returned
*     memory content.
*   aMemory      - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwGetNativeSurfaceMemory
(
  unsigned long     aSurface,
  int               aX,
  int               aY,
  int               aIndex,
  int               aWriteAccess,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwGetNativeSurfaceMemSize
*
* DESCRIPTION:
*   The function EwGetNativeSurfaceMemSize() has the job to determine the number
*   of memory bytes occupied by a native surface with the given size.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface in pixel.
*
* RETURN VALUE:
*   If successful, the function return the surface size in bytes.
*
*******************************************************************************/
int EwGetNativeSurfaceMemSize
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateConstIndex8Surface
*
* DESCRIPTION:
*   The function EwCreateConstIndex8Surface() has the job to create and return
*   a new index8 surface with the given size and pixel data content. Important
*   here is the fact that the function doesn't reserve any memory to store the
*   pixel information. Instead it, the surface associates the pixel data passed
*   in the parameter aMemory.
*
*   This function is thus intended to create surfaces from the pixel data stored
*   directly within a ROM area. Accordingly it is essential that the ROM content
*   does exactly correspond to the index8 surface pixel format. It includes the
*   order of CLUT color channels, premultiplication with alpha value, etc..
*
*   If no surface could be created due to memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality.
*
*   Please note, the surfaces created with this function are signed internally
*   as constant. Trying to obtrain write-access to such surface will result in
*   a runtime error.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*   aMemory - Structure to pass the ROM address information.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateConstIndex8Surface
(
  int               aWidth,
  int               aHeight,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwCreateIndex8Surface
*
* DESCRIPTION:
*   The function EwCreateIndex8Surface() has the job to create and return a new
*   index8 surface with the given size. If no surface could be created due to
*   memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function creates a pure software surface
*   within the CPU address space only.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateIndex8Surface
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyIndex8Surface
*
* DESCRIPTION:
*   The function EwDestroyIndex8Surface() has the job to release resources of
*   a previously created index8 surface.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function destroys pure software surfaces
*   previously created by the function EwCreateIndex8Surface() only.
*
* ARGUMENTS:
*   aSurface - Handle to the surface to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyIndex8Surface
(
  unsigned long     aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwGetIndex8SurfaceMemory
*
* DESCRIPTION:
*   The function EwGetIndex8SurfaceMemory() has the job to obtain the pointer to
*   the index8 memory plane and to the color table of the given surface.
*
*   The returned pointer is calculated for the given pixel position aX, aY and
*   in case of the color table, to the entry at the position aIndex.
*
*   Please note, if the affected surface has been created with constant pixel
*   information by using EwCreateConstIndex8Surface(), you can't obtain the
*   write access to such surface. In such case the function will fail with the
*   return value 0.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function can handle with the pure software
*   surfaces previously created by the function EwCreateIndex8Surface() and
*   EwCreateConstIndex8Surface() only.
*
* ARGUMENTS:
*   aSurface     - Handle to the surface to obtain the addresses.
*   aX, aY       - Position within the surface to calculate the pixel address.
*   aIndex       - Index within the color table to calculate the CLUT address.
*   aWriteAccess - If != 0 the caller has the intention to modify the returned
*     memory content.
*   aMemory      - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwGetIndex8SurfaceMemory
(
  unsigned long     aSurface,
  int               aX,
  int               aY,
  int               aIndex,
  int               aWriteAccess,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwGetIndex8SurfaceMemSize
*
* DESCRIPTION:
*   The function EwGetIndex8SurfaceMemSize() has the job to determine the number
*   of memory bytes occupied by an index8 surface with the given size.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface in pixel.
*
* RETURN VALUE:
*   If successful, the function return the surface size in bytes.
*
*******************************************************************************/
int EwGetIndex8SurfaceMemSize
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateAlpha8Surface
*
* DESCRIPTION:
*   The function EwCreateAlpha8Surface() has the job to create and return a new
*   alpha8 surface with the given size. If no surface could be created due to
*   memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function creates a pure software surface
*   within the CPU address space only.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateAlpha8Surface
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateConstAlpha8Surface
*
* DESCRIPTION:
*   The function EwCreateConstAlpha8Surface() has the job to create and return
*   a new alpha8 surface with the given size and pixel data content. Important
*   here is the fact that the function doesn't reserve any memory to store the
*   pixel information. Instead it, the surface associates the pixel data passed
*   in the parameter aMemory.
*
*   This function is thus intended to create surfaces from the pixel data stored
*   directly within a ROM area. Accordingly it is essential that the ROM content
*   does exactly correspond to the alpha8 surface pixel format.
*
*   If no surface could be created due to memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality.
*
*   Please note, the surfaces created with this function are signed internally
*   as constant. Trying to obtrain write-access to such surface will result in
*   a runtime error.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*   aMemory - Structure to pass the ROM address information.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateConstAlpha8Surface
(
  int               aWidth,
  int               aHeight,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyAlpha8Surface
*
* DESCRIPTION:
*   The function EwDestroyAlphaSurface() has the job to release resources of
*   a previously created alpha8 surface.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function destroys pure software surfaces
*   previously created by the function EwCreateAlpha8Surface() only.
*
* ARGUMENTS:
*   aSurface - Handle to the surface to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyAlpha8Surface
(
  unsigned long     aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwGetAlpha8SurfaceMemory
*
* DESCRIPTION:
*   The function EwGetAlpha8SurfaceMemory() has the job to obtain pointers to
*   the memory planes of the given surface.
*
*   The returned pointer is calculated for the given pixel position aX, aY.
*
*   Please note, if the affected surface has been created with constant pixel
*   information by using EwCreateConstAlpha8Surface(), you can't obtain the
*   write access to such surface. In such case the function will fail with the
*   return value 0.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function can handle with the pure software
*   surfaces previously created by the function EwCreateIAlpha8Surface() and
*   EwCreateConstAlpha8Surface() only.
*
* ARGUMENTS:
*   aSurface     - Handle to the surface to obtain the addresses.
*   aX, aY       - Position within the surface to calculate the pixel address.
*   aReserved    - Should be 0.
*   aWriteAccess - If != 0 the caller has the intention to modify the returned
*     memory content.
*   aMemory      - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwGetAlpha8SurfaceMemory
(
  unsigned long     aSurface,
  int               aX,
  int               aY,
  int               aReserved,
  int               aWriteAccess,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwGetAlpha8SurfaceMemSize
*
* DESCRIPTION:
*   The function EwGetAlpha8SurfaceMemSize() has the job to determine the number
*   of memory bytes occupied by an alpha8 surface with the given size.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface in pixel.
*
* RETURN VALUE:
*   If successful, the function return the surface size in bytes.
*
*******************************************************************************/
int EwGetAlpha8SurfaceMemSize
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwCreateConstRGB565Surface
*
* DESCRIPTION:
*   The function EwCreateConstRGB565Surface() has the job to create and return
*   a new rgb565 surface with the given size and pixel data content. Important
*   here is the fact that the function doesn't reserve any memory to store the
*   pixel information. Instead it, the surface associates the pixel data passed
*   in the parameter aMemory.
*
*   This function is thus intended to create surfaces from the pixel data stored
*   directly within a ROM area. Accordingly it is essential that the ROM content
*   does exactly correspond to the rgb565 surface pixel format. It includes the
*   order of CLUT color channels, premultiplication with alpha value, etc..
*
*   If no surface could be created due to memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality.
*
*   Please note, the surfaces created with this function are signed internally
*   as constant. Trying to obtrain write-access to such surface will result in
*   a runtime error.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*   aMemory - Structure to pass the ROM address information.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateConstRGB565Surface
(
  int               aWidth,
  int               aHeight,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwCreateRGB565Surface
*
* DESCRIPTION:
*   The function EwCreateRGB565Surface() has the job to create and return a new
*   rgb565 surface with the given size. If no surface could be created due to
*   memory deficit, 0 is returned.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function creates a pure software surface
*   within the CPU address space only.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface to create.
*
* RETURN VALUE:
*   If successful, returns a handle to the new surface. Otherwise 0 is returned.
*
*******************************************************************************/
unsigned long EwCreateRGB565Surface
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyRGB565Surface
*
* DESCRIPTION:
*   The function EwDestroyRGB565Surface() has the job to release resources of
*   a previously created rgb565 surface.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function destroys pure software surfaces
*   previously created by the function EwCreateRGB565Surface() only.
*
* ARGUMENTS:
*   aSurface - Handle to the surface to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyRGB565Surface
(
  unsigned long     aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwGetRGB565SurfaceMemory
*
* DESCRIPTION:
*   The function EwGetRGB565SurfaceMemory() has the job to obtain the pointer to
*   the rgb565 memory plane and to the color table of the given surface.
*
*   The returned pointer is calculated for the given pixel position aX, aY and
*   in case of the color table, to the entry at the position aIndex.
*
*   Please note, if the affected surface has been created with constant pixel
*   information by using EwCreateConstRGB565Surface(), you can't obtain the
*   write access to such surface. In such case the function will fail with the
*   return value 0.
*
*   The function is used, if the underlying graphics subsystem doesn't provide
*   any adequate functionality. The function can handle with the pure software
*   surfaces previously created by the function EwCreateRGB565Surface() and
*   EwCreateConstRGB565Surface() only.
*
* ARGUMENTS:
*   aSurface     - Handle to the surface to obtain the addresses.
*   aX, aY       - Position within the surface to calculate the pixel address.
*   aIndex       - Index within the color table to calculate the CLUT address.
*   aWriteAccess - If != 0 the caller has the intention to modify the returned
*     memory content.
*   aMemory      - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwGetRGB565SurfaceMemory
(
  unsigned long     aSurface,
  int               aX,
  int               aY,
  int               aIndex,
  int               aWriteAccess,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwGetRGB565SurfaceMemSize
*
* DESCRIPTION:
*   The function EwGetRGB565SurfaceMemSize() has the job to determine the number
*   of memory bytes occupied by an rgb565 surface with the given size.
*
* ARGUMENTS:
*   aWidth,
*   aHeight - Size of the surface in pixel.
*
* RETURN VALUE:
*   If successful, the function return the surface size in bytes.
*
*******************************************************************************/
int EwGetRGB565SurfaceMemSize
(
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwInitColorGradient
*
* DESCRIPTION:
*   The function EwInitColorGradient() has the job to initialize a rectangular,
*   linear gradient from 4 color values. The color values do correspond to the
*   four corners of the rectangular gradient: top-left, top-right, bottom-right
*   and bottom-left.
*
* ARGUMENTS:
*   aWidth,
*   aHeight   - Size of the gradient area in pixel.
*   aColors   - Array of 4 color values in the universal RGBA8888 color format.
*   aGradient - Destination gradient structure to initialize.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwInitColorGradient
(
  int               aWidth,
  int               aHeight,
  unsigned int*     aColors,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwInitOpacityGradient
*
* DESCRIPTION:
*   The function EwInitOpacityGradient() has the job to initialize a new linear,
*   rectangular gradient from 4 color values. The color values do correspond to
*   the four corners of the gradient area: top-left, top-right, bottom-right and
*   bottom-left.
*
* ARGUMENTS:
*   aWidth,
*   aHeight   - Size of the gradient area in pixel.
*   aColors   - Array of 4 color values in the universal RGBA8888 color format.
*   aGradient - Destination gradient structure to initialize.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwInitOpacityGradient
(
  int               aWidth,
  int               aHeight,
  unsigned int*     aColors,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwGetColorFromGradient
*
* DESCRIPTION:
*   The function EwGetColorFromGradient() has the job to interpolate the color
*   value for the given position within a color gradient.
*
* ARGUMENTS:
*   aGradient  - Color gradient to get the color.
*   aX, aY     - Position relative to the upper-left corner of the gradient to
*     get the color value.
*
* RETURN VALUE:
*   Interpolated color value in the universal RGBA8888 color format.
*
*******************************************************************************/
unsigned int EwGetColorFromGradient
(
  XGradient*        aGradient,
  int               aX,
  int               aY
);


/*******************************************************************************
* FUNCTION:
*   EwGetOpacityFromGradient
*
* DESCRIPTION:
*   The function EwGetOpacityFromGradient() has the job to interpolate the
*   opacity value for the given position within a color gradient.
*
* ARGUMENTS:
*   aGradient  - Color gradient to get the value.
*   aX, aY     - Position relative to the upper-left corner of the gradient to
*     get the opacity value.
*
* RETURN VALUE:
*   Interpolated color value in the universal RGBA8888 color format. Only the
*   alpha channel is valid.
*
*******************************************************************************/
unsigned int EwGetOpacityFromGradient
(
  XGradient*        aGradient,
  int               aX,
  int               aY
);


/*******************************************************************************
* FUNCTION:
*   EwRasterAlpha8Polygon
*
* DESCRIPTION:
*  The function EwRasterAlpha8Polygon() implements an algorithm to estimate the
*  content of an ALPHA8 bitmap from polygon data determined by edges in the
*  array aPaths. aPaths stores the edges as a serie of X,Y pairs starting always
*  with a value specifying the number of existing edges. With this approach one
*  path can consist of several sub-paths. The end of the list is determined by a
*  sub-path with 0 edges.
*
*  +-------+------+------+------+------+------+------+-------+     +-----+
*  | edges |  X0  |  Y0  |  X1  |  Y1  |  X2  |  Y2  | edges | ... |  0  |
*  +-------+------+------+------+------+------+------+-------+     +-----+
*
*  The function evaluates the path data for intersections between the edges and
*  the pixel within the destination area aDstX, aDstY, aWidth and aHeight. Then
*  the affected pixel are set or cleared according to whether they lie inside
*  or outside the polygon.
*
* ARGUMENTS:
*   aDst            - Pointer to the first pixel of the destination ALPHA8
*     surface. The caller is responsable for clearing the surface before.
*   aPaths          - An array containing the path data. The array starts with
*     the number of edges a path is composed of. Then follow the coordinates of
*     all path corners as X,Y pairs. After the last coordinate pair next path
*     can follow starting again with the number of edges. The end of the path
*     data is signed with 0. The X,Y coordinates are stored as signed integer
*     with 4-bit fixpoint precision. The coordinates are valid relative to the
*     top-left corner of the destination area aDstX, aDstY.
*   aDstX,
*   aDstY           - Origin of the area to fill (relative to the top-left
*     corner of the destination surface).
*   aWidth,
*   aHeight         - Size of the area to fill.
*   aX, aY          - Path coordinate at the top-left corner of aDstX, aDstY
*     area. This value is expressed in in 4-bit fixpoint precision.
*   aAntialiased    - If != 0, the function applies antialiasing to the pixel.
*     The antialiasing is based on supersampling with 4 samples in X and Y
*     direction.
*   aNonZeroWinding - Controls the fill rule to be used by the algorithm. If
*    this parameter is == 0, the even-odd fill rule is used. If this parameter
*    is != 0, the non-zero winding rule is used.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwRasterAlpha8Polygon
(
  XSurfaceMemory*   aDst,
  int*              aPaths,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aX,
  int               aY,
  int               aAntialiased,
  int               aNonZeroWinding
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateLine
*
* DESCRIPTION:
*   The function EwEmulateLine() drives a draw line operation by using solid or
*   gradient color values.
*
*   The function provides the top-level interface to the software pixel driver.
*   The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst        - Pointer to the first pixel of the destination surface.
*   aDstX1,
*   aDstY1      - Start position to draw the line (relative to the top-left
*     corner of the destination surface).
*   aDstX2,
*   aDstY2      - End position to draw the line (relative to the top-left
*      corner of the destination surface). The pixel at the end position does
*      not belong to the line - it is invisible. In this manner polylines can
*      be drawn.
*   aClipX1,
*   aClipY1,
*   aClipX2,
*   aClipY2     - Optional clipping area relative to the top-left corner of the
*     destination surface. No pixel outside this area may be drawn.
*   aGradient   - Color information to modulate the drawn pixel.
*   aWorker     - Low-level worker function to perform the operation.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateLine
(
  XSurfaceMemory*   aDst,
  int               aDstX1,
  int               aDstY1,
  int               aDstX2,
  int               aDstY2,
  int               aClipX1,
  int               aClipY1,
  int               aClipX2,
  int               aClipY2,
  XGradient*        aGradient,
  XLineWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateFill
*
* DESCRIPTION:
*   The function EwEmulateFill() drives a fill operation for a rectangular area
*   by using solid or gradient color values.
*
*   The function provides the top-level interface to the software pixel driver.
*   The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aDstX,
*   aDstY     - Origin of the area to fill (relative to the top-left corner of
*     the destination surface).
*   aWidth,
*   aHeight   - Size of the area to fill.
*   aGradient - Color information to modulate the filled pixel.
*   aGrdX,
*   aGrdY     - Origin of the affected area in the gradient coordinate space.
*   aWorker   - Low-level worker function to perform the operation row-wise.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateFill
(
  XSurfaceMemory*   aDst,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  XGradient*        aGradient,
  int               aGrdX,
  int               aGrdY,
  XFillWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateCopy
*
* DESCRIPTION:
*   The function EwEmulateCopy() drives a copy operation for a rectangular area
*   from a native, index8, alpha8 or rgb565 surface to a screen or native surface.
*   The function modulates the copied pixel by solid or gradient opacity values.
*
*   The function provides the top-level interface to the software pixel driver.
*   The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aSrc      - Pointer to the first pixel of the source surface.
*   aDstX,
*   aDstY     - Origin of the area to fill (relative to the top-left corner of
*     the destination surface).
*   aWidth,
*   aHeight   - Size of the area to fill.
*   aSrcX,
*   aSrcY     - Origin of the source area to copy (relative to the top-left
*     corner of the source surface).
*   aGradient - Color information to modulate the copied pixel.
*   aGrdX,
*   aGrdY     - Origin of the affected area in the gradient coordinate space.
*   aWorker   - Low-level worker function to perform the operation row-wise.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateCopy
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aSrcX,
  int               aSrcY,
  XGradient*        aGradient,
  int               aGrdX,
  int               aGrdY,
  XCopyWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateWarp
*
* DESCRIPTION:
*   The function EwEmulateWarp() drives a warp operation for a rectangular area
*   of a native, index8, alpha8 or rgb565 surface to a polygon within a screen 
*   or native surface. The function modulates the copied pixel by solid or 
*   gradient opacity values.
*
*   The function provides the top-level interface to the software pixel driver.
*   The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst        - Pointer to the first pixel of the destination surface.
*   aSrc        - Pointer to the first pixel of the source surface.
*   DstX1,
*   DstY1,
*   DstW1,
*   ...
*   DstX4,
*   DstY4,
*   DstW4       - Coordinates of the polygon to fill with the source bitmap
*     (Relative to the top-left corner of the destination bitmap). The 'X', 'Y'
*     coefficients are specified in 29.3 fixed point format due to the
*     sub-pixel precision. The 'W' coefficients in 16.16.
*   aSrcX,
*   aSrcY       - Origin of the source area to copy (relative to the top-left
*     corner of the source surface).
*   aSrcWidth,
*   aSrcHeight  - Size of the source area to copy and warp.
*   aClipX1,
*   aClipY1,
*   aClipX2,
*   aClipY2     - Optional clipping area relative to the top-left corner of the
*     destination surface. No pixel outside this area may be drawn.
*   aGradient   - Color information to modulate the copied pixel.
*   aWorker     - Low-level worker function to perform the operation row-wise.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateWarp
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aDstX1,
  int               aDstY1,
  int               aDstW1,
  int               aDstX2,
  int               aDstY2,
  int               aDstW2,
  int               aDstX3,
  int               aDstY3,
  int               aDstW3,
  int               aDstX4,
  int               aDstY4,
  int               aDstW4,
  int               aSrcX,
  int               aSrcY,
  int               aSrcWidth,
  int               aSrcHeight,
  int               aClipX1,
  int               aClipY1,
  int               aClipX2,
  int               aClipY2,
  XGradient*        aGradient,
  XWarpWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwEmulateFillPolygon
*
* DESCRIPTION:
*  The function EwEmulateFillPolygon() drives a fill polygon operation with
*  polygon data provided in the array aPaths. aPaths stores polygon edges as a
*  serie of X,Y coordinate pairs starting always with a value specifying the
*  number of edges the path is composed of. With this approach one path can
*  consist of several sub-paths. The end of the list is determined by a sub-
*  path with 0 edges:
*
*  +-------+------+------+------+------+------+------+-------+     +-----+
*  | edges |  X0  |  Y0  |  X1  |  Y1  |  X2  |  Y2  | edges | ... |  0  |
*  +-------+------+------+------+------+------+------+-------+     +-----+
*
*  The function evaluates the path data for intersections between the edges and
*  the pixel within the destination area aDstX, aDstY, aWidth and aHeight. The
*  function modulates the pixel by solid or gradient color values.
*
*  The function provides the top-level interface to the software pixel driver.
*  The real drawing operation will be driven by the passed worker function.
*
* ARGUMENTS:
*   aDst            - Pointer to the first pixel of the destination surface.
*   aPaths          - An array containing the path data. The array starts with
*     the number of edges a path is composed of. Then follow the coordinates of
*     all path corners as X,Y pairs. After the last coordinate pair next path
*     can follow starting again with the number of edges. The end of the path
*     data is signed with 0. The X,Y coordinates are stored as signed integer
*     with 4-bit fixpoint precision. The coordinates are valid relative to the
*     top-left corner of the destination bitmap.
*   aDstX,
*   aDstY           - Origin of the area to fill (relative to the top-left
*     corner of the destination surface).
*   aWidth,
*   aHeight         - Size of the area to fill.
*   aAntialiased    - If != 0, the function applies antialiasing to the pixel.
*     The antialiasing is based on supersampling with 4 samples in X and Y
*     direction.
*   aNonZeroWinding - Controls the fill rule to be used by the algorithm. If
*    this parameter is == 0, the even-odd fill rule is used. If this parameter
*    is != 0, the non-zero winding rule is used.
*   aGradient       - Color information to modulate the copied pixel.
*   aGrdX,
*   aGrdY           - Origin of the affected area in the gradient coordinate
*     space.
*   aWorker         - Low-level worker function to perform the final copy
*     operation of the rasterized pixel row-wise.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEmulateFillPolygon
(
  XSurfaceMemory*   aDst,
  int*              aPaths,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aAntialiased,
  int               aNonZeroWinding,
  XGradient*        aGradient,
  int               aGrdX,
  int               aGrdY,
  XCopyWorker       aWorker
);


/*******************************************************************************
* FUNCTION:
*   EwPackColor
*
* DESCRIPTION:
*   The following EwPackColor() function has the job to convert the given RGBA
*   color channels into a generic packed 32 bit format as it is used internally
*   by the Graphics Engine.
*
*   The generic packed 32 bit color format provides an abstraction of a 32 bit
*   color value consisting of three color channels and one alpha channel. The
*   content of the three color chanels is a subject of the particular target
*   system - Graphics Engine doesn't need to know anything about the meaning of
*   these channels - they are abstract.
*
*   The generic packed 32 bit color format stores the channels in the following
*   order:
*
*              31           24           16             8            0
*               +------------+---------------------------------------+
*               |   alpha    |  channel 3 |  channel 2  |  channel 1 |
*               +------------+---------------------------------------+
*
*   Depending on the target color format, the function can modify and convert
*   the values of the channels. For example, the RGB -> YUV conversion can be
*   executed or the color channels can be pre-multiplied by the alpha value.
*
*   The resulting 32 bit value is particular for the respective target color
*   format. It is predestined to be used more effectively durring all drawing
*   operations.
*
* ARGUMENTS:
*   aRed,
*   aGreen,
*   aBlue,
*   aAlpha  - Non-premultiplied color channel values in the range 0 .. 255.
*
* RETURN VALUE:
*   Packed 32 bit color value. Particular for the target color format.
*
*******************************************************************************/
unsigned int EwPackColor
(
  int               aRed,
  int               aGreen,
  int               aBlue,
  int               aAlpha
);


/*******************************************************************************
* FUNCTION:
*   EwPackClutEntry
*
* DESCRIPTION:
*   The following EwPackClutEntry() function has the job to convert the given
*   RGBA color channels into a 32 bit wide clut entry as it is used by Index8
*   surfaces internally.
*
*   Like the EwPackColor() function, the resulting value depends on the used
*   native color format. Therefore, the order of color channels, color spaces,
*   etc. are not predetermined here. The result is just a 32 bit value without
*   any assumptions about its structure.
*
*   The resulting 32 bit value is particular for the respective target color
*   format. It is predestined to be used more effectively durring all drawing
*   Index8 bitmaps.
*
* ARGUMENTS:
*   aRed,
*   aGreen,
*   aBlue,
*   aAlpha  - Non-premultiplied color channel values in the range 0 .. 255.
*
* RETURN VALUE:
*   32 bit clut entry. Particular for the target color format.
*
*******************************************************************************/
unsigned int EwPackClutEntry
(
  int               aRed,
  int               aGreen,
  int               aBlue,
  int               aAlpha
);


/*******************************************************************************
* FUNCTION:
*   EwSetPixelSolid
*   EwScreenSetPixelSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to draw a single pixel with a solid color. The new pixel overwrites the
*   existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aX, aY    - Position to draw the pixel.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetPixelSolid
(
  XSurfaceMemory*   aDst,
  int               aX,
  int               aY,
  XGradient*        aGradient
);

void EwScreenSetPixelSolid
(
  XSurfaceMemory*   aDst,
  int               aX,
  int               aY,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwSetPixelSolidBlend
*   EwScreenSetPixelSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to draw a single pixel with a solid color. The new pixel will be alpha
*   blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination surface.
*   aX, aY    - Position to draw the pixel.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetPixelSolidBlend
(
  XSurfaceMemory*   aDst,
  int               aX,
  int               aY,
  XGradient*        aGradient
);

void EwScreenSetPixelSolidBlend
(
  XSurfaceMemory*   aDst,
  int               aX,
  int               aY,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwFillRowSolid
*   EwScreenFillRowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a solid color. The new pixel overwrites
*   the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aWidth    - Width of the row.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRowSolid
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenFillRowSolid
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwFillRowSolidBlend
*   EwScreenFillRowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a solid color. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aWidth    - Width of the row.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRowSolidBlend
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenFillRowSolidBlend
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwFillRowGradient
*   EwScreenFillRowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a color gradient. The new pixel will
*   overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aWidth    - Width of the row.
*   aGradient - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRowGradient
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenFillRowGradient
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwFillRowGradientBlend
*   EwScreenFillRowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a color gradient. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aWidth    - Width of the row.
*   aGradient - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFillRowGradientBlend
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenFillRowGradientBlend
(
  XSurfaceMemory*   aDst,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRow
*   EwScreenCopyNativeRow
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed without any additional opacity values. The new pixel overwrites
*   the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRow
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyNativeRow
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowBlend
*   EwScreenCopyNativeRowBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed without any additional opacity values. The new pixel will be
*   alpha blended withe the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyNativeRowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowSolid
*   EwScreenCopyNativeRowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed with an additional solid opacity value. The new pixel will
*   overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyNativeRowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowSolidBlend
*   EwScreenCopyNativeRowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed with an additional solid opacity value. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyNativeRowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowGradient
*   EwScreenCopyNativeRowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed with an additional opacity gradient. The new pixel overwrites
*   the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyNativeRowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyNativeRowGradientBlend
*   EwScreenCopyNativeRowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row between two native surfaces. The operation
*   is executed with an additional opacity gradient. The new pixel will be alpha
*   blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyNativeRowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyNativeRowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8Row
*   EwScreenCopyIndex8Row
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The
*   operation is executed without any additional opacity values. The new pixel
*   overwrites the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyIndex8Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowBlend
*   EwScreenCopyIndex8RowBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The
*   operation is executed without any additional opacity values. The new pixel
*   will be alpha blended withe the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyIndex8RowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowSolid
*   EwScreenCopyIndex8RowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The
*   operation is executed with an additional solid opacity value. The new pixel
*   will overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyIndex8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowSolidBlend
*   EwScreenCopyIndex8RowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The
*   operation is executed with an additional solid opacity value. The new pixel
*   will be alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyIndex8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowGradient
*   EwScreenCopyIndex8RowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The
*   operation is executed with an additional opacity gradient. The new pixel
*   overwrites the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyIndex8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyIndex8RowGradientBlend
*   EwScreenCopyIndex8RowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an index8 to a native surface. The
*   operation is executed with an additional opacity gradient. The new pixel
*   will be alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyIndex8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyIndex8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyAlpha8RowSolid
*   EwScreenCopyAlpha8RowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a solid color additionally modulated by
*   the opacity values from the source alpha8 surface. The new pixel overwrite
*   the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyAlpha8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyAlpha8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyAlpha8RowSolidBlend
*   EwScreenCopyAlpha8RowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a solid color additionally modulated by
*   the opacity values from the source alpha8 surface. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid color information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyAlpha8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyAlpha8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyAlpha8RowGradient
*   EwScreenCopyAlpha8RowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a color gradient additionally modulated
*   by the opacity values from the source alpha8 surface. The new pixel will
*   overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyAlpha8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyAlpha8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyAlpha8RowGradientBlend
*   EwScreenCopyAlpha8RowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a color gradient additionally modulated
*   by the opacity values from the source alpha8 surface. The new pixel will be
*   alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyAlpha8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyAlpha8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyRGB565Row
*   EwScreenCopyRGB565Row
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an rgb565 to a native surface. The
*   operation is executed without any additional opacity values. The new pixel
*   overwrites the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyRGB565Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyRGB565Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyRGB565RowSolid
*   EwScreenCopyRGB565RowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an rgb565 to a native surface. The
*   operation is executed with an additional solid opacity value. The new pixel
*   will overwrite the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyRGB565RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyRGB565RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyRGB565RowSolidBlend
*   EwScreenCopyRGB565RowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an rgb565 to a native surface. The
*   operation is executed with an additional solid opacity value. The new pixel
*   will be alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Solid opacity information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyRGB565RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyRGB565RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyRGB565RowGradient
*   EwScreenCopyRGB565RowGradient
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an rgb565 to a native surface. The
*   operation is executed with an additional opacity gradient. The new pixel
*   overwrites the existing pixel within the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyRGB565RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyRGB565RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwCopyRGB565RowGradientBlend
*   EwScreenCopyRGB565RowGradientBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to copy a horizontal pixel row from an rgb565 to a native surface. The
*   operation is executed with an additional opacity gradient. The new pixel
*   will be alpha blended with the existing pixel of the destination.
*
* ARGUMENTS:
*   aDst      - Pointer to the first pixel of the destination row.
*   aSrc      - Pointer to the first pixel of the source row.
*   aWidth    - Width of the row.
*   aGradient - Opacity gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopyRGB565RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);

void EwScreenCopyRGB565RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRow
*   EwScreenWarpNativeRow
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will overwrite the existing pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRow
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRow
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowFilter
*   EwScreenWarpNativeRowFilter
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The resulting pixel will overwrite the existing pixel in
*   the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowFilter
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowFilter
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowBlend
*   EwScreenWarpNativeRowBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowFilterBlend
*   EwScreenWarpNativeRowFilterBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The new pixel will be alpha-blended with the existing
*   pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowFilterBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowFilterBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowGradient
*   EwWarpNativeRowSolid
*   EwScreenWarpNativeRowGradient
*   EwScreenWarpNativeRowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The new
*   pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpNativeRowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowFilterGradient
*   EwWarpNativeRowFilterSolid
*   EwScreenWarpNativeRowFilterGradient
*   EwScreenWarpNativeRowFilterSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter. The
*   new pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpNativeRowFilterSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowFilterSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowGradientBlend
*   EwWarpNativeRowSolidBlend
*   EwScreenWarpNativeRowGradientBlend
*   EwScreenWarpNativeRowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The
*   resulting pixel will be alpha-blended with the pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpNativeRowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpNativeRowFilterGradientBlend
*   EwWarpNativeRowFilterSolidBlend
*   EwScreenWarpNativeRowFilterGradientBlend
*   EwScreenWarpNativeRowFilterSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The resulting pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpNativeRowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpNativeRowFilterSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpNativeRowFilterSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8Row
*   EwScreenWarpIndex8Row
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will overwrite the existing pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowFilter
*   EwScreenWarpIndex8RowFilter
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The resulting pixel will overwrite the existing pixel in
*   the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowFilter
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowFilter
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowBlend
*   EwScreenWarpIndex8RowBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowFilterBlend
*   EwScreenWarpIndex8RowFilterBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The new pixel will be alpha-blended with the existing
*   pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowFilterBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowFilterBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowGradient
*   EwWarpIndex8RowSolid
*   EwScreenWarpIndex8RowGradient
*   EwScreenWarpIndex8RowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The new
*   pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpIndex8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowFilterGradient
*   EwWarpIndex8RowFilterSolid
*   EwScreenWarpIndex8RowFilterGradient
*   EwScreenWarpIndex8RowFilterSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The new pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpIndex8RowFilterSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowFilterSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowGradientBlend
*   EwWarpIndex8RowSolidBlend
*   EwScreenWarpIndex8RowGradientBlend
*   EwScreenWarpIndex8RowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The
*   resulting pixel will be alpha-blended with the pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpIndex8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpIndex8RowFilterGradientBlend
*   EwWarpIndex8RowFilterSolidBlend
*   EwScreenWarpIndex8RowFilterGradientBlend
*   EwScreenWarpIndex8RowFilterSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The resulting pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpIndex8RowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpIndex8RowFilterSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpIndex8RowFilterSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpAlpha8RowGradient
*   EwWarpAlpha8RowSolid
*   EwScreenWarpAlpha8RowGradient
*   EwScreenWarpAlpha8RowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The new
*   pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpAlpha8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpAlpha8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpAlpha8RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpAlpha8RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpAlpha8RowFilterGradient
*   EwWarpAlpha8RowFilterSolid
*   EwScreenWarpAlpha8RowFilterGradient
*   EwScreenWarpAlpha8RowFilterSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter. The
*   new pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpAlpha8RowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpAlpha8RowFilterSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpAlpha8RowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpAlpha8RowFilterSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpAlpha8RowGradientBlend
*   EwWarpAlpha8RowSolidBlend
*   EwScreenWarpAlpha8RowGradientBlend
*   EwScreenWarpAlpha8RowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The
*   resulting pixel will be alpha-blended with the pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpAlpha8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpAlpha8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpAlpha8RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpAlpha8RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpAlpha8RowFilterGradientBlend
*   EwWarpAlpha8RowFilterSolidBlend
*   EwScreenWarpAlpha8RowFilterGradientBlend
*   EwScreenWarpAlpha8RowFilterSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The resulting pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpAlpha8RowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpAlpha8RowFilterSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpAlpha8RowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpAlpha8RowFilterSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpRGB565Row
*   EwScreenWarpRGB565Row
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface. The new pixel will overwrite the existing pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpRGB565Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565Row
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpRGB565RowFilter
*   EwScreenWarpRGB565RowFilter
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface.  For better output quality the source pixel are interpolated by a
*   bi-linear filter. The resulting pixel will overwrite the existing pixel in
*   the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - <Unused in this function>
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpRGB565RowFilter
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowFilter
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwWarpRGB565RowGradient
*   EwWarpRGB565RowSolid
*   EwScreenWarpRGB565RowGradient
*   EwScreenWarpRGB565RowSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The new
*   pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpRGB565RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpRGB565RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpRGB565RowFilterGradient
*   EwWarpRGB565RowFilterSolid
*   EwScreenWarpRGB565RowFilterGradient
*   EwScreenWarpRGB565RowFilterSolid
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The new pixel will overwrite the existing pixel in the destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpRGB565RowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpRGB565RowFilterSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowFilterGradient
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowFilterSolid
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpRGB565RowGradientBlend
*   EwWarpRGB565RowSolidBlend
*   EwScreenWarpRGB565RowGradientBlend
*   EwScreenWarpRGB565RowSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. The
*   resulting pixel will be alpha-blended with the pixel in the destination
*   memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpRGB565RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpRGB565RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

/*******************************************************************************
* FUNCTION:
*   EwWarpRGB565RowFilterGradientBlend
*   EwWarpRGB565RowFilterSolidBlend
*   EwScreenWarpRGB565RowFilterGradientBlend
*   EwScreenWarpRGB565RowFilterSolidBlend
*
* DESCRIPTION:
*   The following function defines the low-level, pixel-wise drawing operation
*   to fill a horizontal pixel row with a transformed content of the source
*   surface and modulate the pixel with opacity values from a gradient. For
*   better output quality the pixel are interpolated by a bi-linear filter.
*   The resulting pixel will be alpha-blended with the existing pixel in the
*   destination memory.
*
* ARGUMENTS:
*   aDst       - Pointer to the first pixel of the destination row.
*   aSrc       - Pointer to the first pixel of the source surface.
*   aWidth     - Width of the row.
*   aS, aT     - Start values for the S and T texture mapping coordinates.
*   aSS, aTS   - Per pixel increment for the interpolation of S and T.
*   aSrcWidth,
*   aSrcHeight - Size of the source surface to warp.
*   aGradient  - Color gradient information.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwWarpRGB565RowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwWarpRGB565RowFilterSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowFilterGradientBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);

void EwScreenWarpRGB565RowFilterSolidBlend
(
  XSurfaceMemory*   aDst,
  XSurfaceMemory*   aSrc,
  int               aWidth,
  int               aS,
  int               aT,
  int               aSS,
  int               aTS,
  int               aSrcWidth,
  int               aSrcHeight,
  XGradient*        aGradient
);


/*******************************************************************************
* FUNCTION:
*   EwAllocVideo
*
* DESCRIPTION:
*   The function EwAllocVideo() has the job to reserve a block of memory which
*   is exclusively intended for pure software surfaces.
*
*   The large size and the persistent character of surfaces may expect special
*   heap management and algorithms. The intension of this function is to handle
*   surface memory allocations in a separate way of any other allocations.
*
* ARGUMENTS:
*   aSize - Desired size of the new memory block in bytes.
*
* RETURN VALUE:
*   If successful, returns the pointer to the reserved memory block, otherwise
*   null is returned.
*
*******************************************************************************/
void* EwAllocVideo
(
  int               aSize
);


/*******************************************************************************
* FUNCTION:
*   EwFreeVideo
*
* DESCRIPTION:
*   The function EwFreeVideo() is the counterpart of EwAllocVideo() function.
*   Its job is to release the given memory block.
*
*   The large size and the persistent character of surfaces may expect special
*   heap management and algorithms. The intension of this function is to handle
*   surface memory allocations in a separate way of any other allocations.
*
* ARGUMENTS:
*   aMemory - Address of the memory block to release.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeVideo
(
  void*             aMemory
);


/*******************************************************************************
* MACRO:
*   EW_REDIRECT_WARP_FUNC
*
* DESCRIPTION:
*   The macro EW_REDIRECT_WARP_FUNC defines a new function as wrapper to an
*   already existing warp operations implemented in another function.
*
*   This macro is used in Software Pixel Driver modules implementing limited
*   set of possible warp operations. The wrapper function created by the macro
*   takes care of the right redirection of the operation to the corresponding
*   worker function.
*
* ARGUMENTS:
*   aFunc1 - Function to serve as the entry point for the operation.
*   aFunc2 - Function, which should be called by the wrapper.
*
*******************************************************************************/
#define EW_REDIRECT_WARP_FUNC( aFunc1, aFunc2 )                                \
  void aFunc1( XSurfaceMemory* aDst, XSurfaceMemory* aSrc, int aWidth,         \
    int aS, int aT, int aSS, int aTS, int aSrcWidth, int aSrcHeight,           \
    XGradient* aGradient )                                                     \
  {                                                                            \
    aFunc2( aDst, aSrc, aWidth, aS, aT, aSS, aTS, aSrcWidth, aSrcHeight,       \
            aGradient );                                                       \
  }


#ifdef __cplusplus
  }
#endif

#endif /* EWGFXDRIVER_H */

/* pba */
