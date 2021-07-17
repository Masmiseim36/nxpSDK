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
*   This header file provides declarations of data structures of drawing tasks
*   and few functions needed to perform the optimization and elimination steps
*   of these tasks and to dispatch the tasks to underlying graphics subsystem
*   or to the emulated software pixel driver.
*
*******************************************************************************/

#ifndef EWGFXTASKS_H
#define EWGFXTASKS_H


#ifdef __cplusplus
  extern "C"
  {
#endif


/*******************************************************************************
* TYPE:
*   XFillRectangle
*
* DESCRIPTION:
*   The XFillRectangle structure defines the parameters set for a FILL_RECTANGLE
*   task.
*
*   When executed, the task should fill the destination area with a solid color.
*   If the color values differ, the area should be filled with a color gradient
*   by interpolation of the colors for all drawn pixel.
*
* ELEMENTS:
*   DstX1, DstY1,
*   DstX2, DstY2 - Destination area to fill within the surface.
*   Colors       - Colors to fill the area in the RGBA8888 format. The four
*     values correspond to the four corners of the destination area (top-left,
*     top-right, bottom-right, botom-left).
*
*******************************************************************************/
typedef struct
{
  short             DstX1;
  short             DstY1;
  short             DstX2;
  short             DstY2;
  unsigned int      Colors[4];
} XFillRectangle;


/*******************************************************************************
* TYPE:
*   XCopySurface
*
* DESCRIPTION:
*   The XCopySurface structure defines the parameters set for a COPY_SURFACE
*   task.
*
*   When executed, the task should copy the given source area from the source
*   surface and place it within the destination area of the destination surface.
*
*   The additional four color values control the fading effects while drawing
*   the pixel. If these values differ, the pixel are faded with a gradient by
*   interpolation of the four values for all copied pixel.
*
* ELEMENTS:
*   DstX1, DstY1,
*   DstX2, DstY2 - Destination area to fill within the destination surface.
*   SrcX,  SrcY  - Origin position for the source area to copy.
*   Surface      - Source surface.
*   Colors       - Color values to use while copying the source surface. The
*     four values correspond to the four corners of the destination area.
*     (top-left, top-right, bottom-right, botom-left)
*
*******************************************************************************/
typedef struct
{
  short             DstX1;
  short             DstY1;
  short             DstX2;
  short             DstY2;
  short             SrcX;
  short             SrcY;
  XSurface*         Surface;
  unsigned int      Colors[4];
} XCopySurface;


/*******************************************************************************
* TYPE:
*   XTileSurface
*
* DESCRIPTION:
*   The XTileSurface structure defines the parameters set for a TILE_SURFACE
*   task.
*
*   When executed, the task should copy the given source area from the source
*   surface and place it within the destination area of the destination surface.
*   If the both areas differ in the size, the source area is repeated in the x
*   and y direction until the entire destination area was completely filled.
*
*   The additional four color values control the fading effects while drawing
*   the pixel. If these values differ, the pixel are faded with a gradient by
*   interpolation of the four values for all copied pixel.
*
* ELEMENTS:
*   DstX1, DstY1,
*   DstX2, DstY2 - Destination area to fill within the destination surface.
*   SrcX1, SrcY1,
*   SrcX2, SrcY2 - Source area to copy from the source surface.
*   OfsX, OfsY   - Position within the source area to start the operation. This
*     is an offset, which allows the scrolling of the drawn content within the
*     destination area. The source position has to lie within the source area.
*   Surface      - Source surface.
*   Colors       - Color values to use while copying the source surface. The
*     four values correspond to the four corners of the destination area.
*     (top-left, top-right, bottom-right, bottom-left)
*
*******************************************************************************/
typedef struct
{
  short             DstX1;
  short             DstY1;
  short             DstX2;
  short             DstY2;
  short             SrcX1;
  short             SrcY1;
  short             SrcX2;
  short             SrcY2;
  short             OfsX;
  short             OfsY;
  XSurface*         Surface;
  unsigned int      Colors[4];
} XTileSurface;


/*******************************************************************************
* TYPE:
*   XWarpSurface
*
* DESCRIPTION:
*   The XWarpSurface structure defines the parameters set for a WARP_SURFACE
*   task.
*
*   When executed, the task should copy the given source area from the source
*   surface and place it within the destination area of the destination surface.
*   Unlike the COPY_SURFACE task, WARP_SURFACE supports non rectangular areas.
*   Depending on the shape of the area and the W coordinates, different kinds of
*   transformations are possible, even the 3D perspective projection.
*   
*   The additional four opacity values control the fading effects while drawing
*   the pixel. If these values differ, the pixel are faded with a gradient by
*   interpolation of the four values for all copied pixel.
*
* ELEMENTS:
*   DstX1, DstY1, DstW1,
*   ...
*   DstX4, DstY4, DstW4 - Destination polygon to fill within the destination
*     surface.
*   SrcX1, SrcY1,
*   SrcX2, SrcY2        - Source area to copy from the source surface.
*   Surface             - Source surface.
*   Colors              - Color values to modulate the copied pixel. The four
*     values correspond to the four corners of the destination polygon.
*
*******************************************************************************/
typedef struct
{
  float             DstX1;
  float             DstY1;
  float             DstW1;
  float             DstX2;
  float             DstY2;
  float             DstW2;
  float             DstX3;
  float             DstY3;
  float             DstW3;
  float             DstX4;
  float             DstY4;
  float             DstW4;
  short             SrcX1;
  short             SrcY1;
  short             SrcX2;
  short             SrcY2;
  XSurface*         Surface;
  unsigned int      Colors[4];
} XWarpSurface;


/*******************************************************************************
* TYPE:
*   XDrawLine
*
* DESCRIPTION:
*   The XDrawLine structure defines the parameters set for a DRAW_LINE task.
*
*   When executed, the task should draw a line with the given colors. If both
*   color values differ, the line should be drawn with a color gradient.
*
* ELEMENTS:
*   DstX1, DstY1,
*   DstX2, DstY2 - The start and the end point of the line. The end point does
*     not belong to the line - it is invisible. In this manner polylines can
*     be drawn.
*   Colors       - Colors to fill the area in the RGBA8888 format. The both
*     values correspond to the start and the end point of the drawn line.
*
*******************************************************************************/
typedef struct
{
  short             DstX1;
  short             DstY1;
  short             DstX2;
  short             DstY2;
  unsigned int      Colors[2];
} XDrawLine;


/*******************************************************************************
* TYPE:
*   XDrawText
*
* DESCRIPTION:
*   The XDrawText structure defines the parameters set for a DRAW_TEXT task.
*
*   When executed, the task should draw text into the destination surface by
*   composing it from an array of glyphs. The references to these glyphs and
*   the associated kerning values are stored in the memory area at the end of
*   this parameters structure. Kerning values are represented by 32-bit signed
*   integers. Glyphs are refereces to the corresponding XGlyph struct as shown
*   below:
*
*   +-----------+------+-------+------+-------+     +------+-------+
*   | XDrawText | Kern | Glyph | Kern | Glyph | ... | Kern | Glyph |
*   +-----------+------+-------+------+-------+     +------+-------+
*
*   The text composition should start at the location OfsX, OfsY relative to
*   the origin of the destination area. This represents the drawing position.
*   For each glyph the position should move accordingly to the glyph's advance
*   metrics.
*
*   The additional four color values determine the color of the drawn glyphs.
*   If these values differ, the glyphs should be drawn with a color gradient
*   by interpolation of the four values for all copied pixel.
*
* ELEMENTS:
*   DstX1, DstY1,
*   DstX2, DstY2 - Destination area to fill within the surface.
*   OfsX, OfsY   - Position to start the text drawing. Relative to the corner
*     of the destination area, which by taking in account the specified text
*     orientation serves as the origin for the drawing operation.
*   StretchFac,
*   StretchAcc   - White space stretch factor in 16.16 fixed point format.
*   Glyphs       - Number of glyph/kerning entries within the following array
*     as described above. This is the number of glyphs to output.
*   Orientation  - Rotation of the text as value 0, 90, 180 or 270.
*   Colors       - Colors to fill the area in the RGBA8888 format. The four
*     values correspond to the four corners of the destination area (top-left,
*     top-right, bottom-right, bottom-left).
*
*******************************************************************************/
typedef struct
{
  short             DstX1;
  short             DstY1;
  short             DstX2;
  short             DstY2;
  short             OfsX;
  short             OfsY;
  int               StretchFac;
  int               StretchAcc;
  short             Glyphs;
  short             Orientation;
  unsigned int      Colors[4];
} XDrawText;


/*******************************************************************************
* TYPE:
*   XFillPolygon
*
* DESCRIPTION:
*   The XFillPolygon structure defines the parameters set for a FILL_POLYGON
*   task.
*
*   When executed, the task should fill the raster a polygon resulting from the
*   given path information and store it within the destination area of the
*   destination surface. The path information is stored in the memory at the 
*   end of this parameters structure. It is an array of int values starting
*   with the number of edges a path is composed of. Then follow the coordinates
*   of all path corners as X,Y pairs. After the last coordinate pair next path
*   can follow starting again with the number of edges. The end of the path
*   data is signed with 0. The X,Y coordinates are stored as signed integer
*   with 4-bit fixpoint precision:
*
*   +-------+------+------+------+------+------+------+-------+     +-----+
*   | edges |  X0  |  Y0  |  X1  |  Y1  |  X2  |  Y2  | edges | ... |  0  |
*   +-------+------+------+------+------+------+------+-------+     +-----+
*
*   The additional four color values control the fading effects while drawing
*   the pixel. If these values differ, the pixel are faded with a gradient by
*   interpolation of the four values for all copied pixel.
*
* ELEMENTS:
*   DstX1, DstY1,
*   DstX2, DstY2    - Destination area to fill within the destination surface.
*   NonZeroWinding  - Controls the fill rule to be used by the algorithm. If
*    this parameter is == 0, the even-odd fill rule is used. If this parameter
*    is != 0, the non-zero winding rule is used.
*   Orientation     - Stores the orientation of the paths data relative to the
*     physical origin of destination surface. This can be 0, 90, 180 or 270. If
*     the specified orientation doesn't correspond to the of the destination
*     surface, all path coordinates need to be converted just before performing
*     the operation.
*   Colors          - Color values to use while rasterizing the polygon. The
*     four values correspond to the four corners of the destination area.
*     (top-left, top-right, bottom-right, botom-left)
*
*******************************************************************************/
typedef struct
{
  short             DstX1;
  short             DstY1;
  short             DstX2;
  short             DstY2;
  short             NonZeroWinding;
  short             Orientation;
  unsigned int      Colors[4];
} XFillPolygon;


/*******************************************************************************
* FUNCTION:
*   EwEliminateTasks
*
* DESCRIPTION:
*  The function EwEliminateTasks() has the job to find and eliminate superfluous
*  drawing operations in the given issue.
*
* ARGUMENTS:
*   aIssue - Issue containing the tasks to eliminate.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwEliminateTasks
(
  XIssue*           aIssue
);


/*******************************************************************************
* FUNCTION:
*   EwReorderTasks
*
* DESCRIPTION:
*  The function EwReorderTasks() has the job to change the order of the drawing 
*  tasks with the objective to put together all the software emulated tasks.
*
* ARGUMENTS:
*   aIssue - Issue containing the tasks to change the order.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwReorderTasks
(
  XIssue*           aIssue
);


/*******************************************************************************
* FUNCTION:
*   EwReverseTasks
*
* DESCRIPTION:
*  The function EwReverseTasks() has the job to reverse the order of all tasks
*  within the issue.
*
* ARGUMENTS:
*   aIssue - Issue containing the tasks to change the order.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwReverseTasks
(
  XIssue*           aIssue
);


/*******************************************************************************
* FUNCTION:
*   EwExecuteTasks
*
* DESCRIPTION:
*  The function EwExecuteTasks() has the job to traverse the list of drawing
*  task and to invoke the appropriate driver functions for their execution.
*
* ARGUMENTS:
*   aIssue - Issue containing the tasks to execute.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwExecuteTasks
(
  XIssue*           aIssue
);


/*******************************************************************************
* FUNCTION:
*   EwCompleteTasks
*
* DESCRIPTION:
*  The function EwCompleteTasks() has the job to release resources used by the
*  tasks contained within the given issue.
*
* ARGUMENTS:
*   aIssue - Issue containing the tasks to complete.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCompleteTasks
(
  XIssue*           aIssue
);


/*******************************************************************************
* FUNCTION:
*   EwCountTasks
*
* DESCRIPTION:
*  The function EwCountTasks() exists for statistic purpose only. The function
*  evaluates the tasks, counts them and calculates the amount of pixel to copy
*  and to blend.
*
* ARGUMENTS:
*   aIssue     - Issue containing the tasks to cout.
*   aNoOfTasks - Pointer to variable, where the resulting number of tasks is
*     returned.
*   aCopyArea  - Pointer to variable, where the resulting pixel copy area is
*     returned.
*   aBlendArea - Pointer to variable, where the resulting pixel blend area is
*     returned.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCountTasks
(
  XIssue*           aIssue,
  int*              aNoOfTasks,
  int*              aCopyArea,
  int*              aBlendArea
);


/*******************************************************************************
* TYPE:
*   XTraceTasksProc
*
* DESCRIPTION:
*  The following type declares a prototype of a user defined function, which
*  can be registered in order to monitor task execution. To register the user
*  function EwTraceTasks() is intended.
*
*  The user defined callback function is allways called after the given task
*  has been executed.
*
*  Note the special behaviour of hardware accelerated target systems where all
*  drawing operations may remain still pending until the entire drawing cycle
*  has been finished. For this reason, it is not guaranteed that the operation
*  results are stored in the destination surface memory when the user callback
*  function is called.
*
* ARGUMENTS:
*   aDst      - Destination surface affected by the operation.
*   aTask     - Task beeing executed. Note, the corresponding drawing operation
*     may still remain pending in the graphics hardware.
*   aUserArg1 - User argument passed in the EwTraceTasks() function.
*   aUserArg2 - User argument passed in the EwTraceTasks() function.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XTraceTasksProc)
(
  XSurface*         aDst,
  XTask*            aTask,
  void*             aUserArg1,
  void*             aUserArg2
);


/*******************************************************************************
* FUNCTION:
*   EwTraceTasks
*
* DESCRIPTION:
*  The function EwTraceTasks() provides a mechanism to monitor task execution.
*  EwTraceTasks() registers a user defined callback function aProc to call when
*  a task is executed.
*
*  The registered function is called for each executed task until EwTraceTasks()
*  is called again with 0 (zero) as aProc paramater.
*
* ARGUMENTS:
*   aProc     - User defined callback function to call for each executed task.
*   aUserArg1 - User argument to pass additionally to the called user function.
*   aUserArg2 - User argument to pass additionally to the called user function.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwTraceTasks
(
  XTraceTasksProc   aProc,
  void*             aUserArg1,
  void*             aUserArg2
);


#ifdef __cplusplus
  }
#endif

#endif /* EWGFXTASKS_H */

/* pba */
