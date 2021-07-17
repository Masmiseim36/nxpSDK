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

#ifndef _GraphicsPath_H
#define _GraphicsPath_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

/* Forward declaration of the class Graphics::Path */
#ifndef _GraphicsPath_
  EW_DECLARE_CLASS( GraphicsPath )
#define _GraphicsPath_
#endif


/* The class Graphics::Path provides functionality to create and store curve data. 
   Every path object can contain one or more sub-paths. In turn, every sub-path 
   can be individually composed of line segments, Bézier curves, elliptical arcs 
   or even circles. In this manner complex polygons and paths can be constructed 
   easily.
   With the path information the curves can be displayed on the screen. The view 
   Views::FillPath displays the path as filled polygon. The view Views::StrokePath 
   displays the path as a line with specified width.
   The methods of the class Graphics::Paths can be divided in following groups:
   - To specify how many sub-paths the path should be able to manage use the method 
   @SetMaxNoOfSubPaths(). To reserve memory for a sub-path use the method @InitSubPath().
   - With the method @Begin() the start position for a sub-path can be determined 
   or existing sub-path data can be cleared again. In turn, with the method @Close() 
   a sub-path can be closed so its end and start positions appear connected together.
   - Alternatively path data can be loaded from an SVG string by using the method 
   @CreateFromSVGString().
   - With the methods @AddLine(), @AddBezier2(), @AddBezier3() and @AddArc() new 
   line segments or even complex curves are added to the sub-path. With the method 
   @AddCopy() a duplicate of another existing sub-path can be added.
   - With the method @SetNode() the previously prepared sub-path data can be modified. 
   Similarly by using the methods @GetNodeX() and @GetNodeY() the X, Y coordinates 
   stored in the path can be evaluated. The method @ShiftNodes() is convenient to 
   delete old nodes before new nodes are appended.
   - The methods @InitMatrix(), @Translate(), @Scale(), @Rotate(), @PushMatrix() 
   and @PopMatrix() manage a 2D transformation matrix used to convert the original 
   coordinates of the lines and curves when adding them to the path.
   - The methods @GetSubPathBounds() and @GetPathBounds() return the area enlosing 
   all edges of a particular sub-path or of all sub-paths. */
EW_DEFINE_FIELDS( GraphicsPath, XObject )
  EW_VARIABLE( path,            XHandle )
EW_END_OF_FIELDS( GraphicsPath )

/* Virtual Method Table (VMT) for the class : 'Graphics::Path' */
EW_DEFINE_METHODS( GraphicsPath, XObject )
EW_END_OF_METHODS( GraphicsPath )

/* 'C' function for method : 'Graphics::Path.onUpdate()' */
void GraphicsPath_onUpdate( GraphicsPath _this, XObject sender );

/* 'C' function for method : 'Graphics::Path.Done()' */
void GraphicsPath_Done( GraphicsPath _this );

/* The method AddCopy() appends to the sub-path with the given number aSubPathNo 
   a copy data from another sub-path. In this manner contents from different paths 
   can be combined together without the necessity to re-calculate them again. The 
   source sub-path as well as the range of data to copy are determined by the parameters 
   aSrcPath, aSrcSubPathNo, aSrcNodeNo and aNoOfEdges. Accordingly, depending on 
   the paramerters aSrcPath and aSrcSubPathNo contents between different paths can 
   be copied. 
   Does aSrcPath and aSrcSubPathNo refer to the same sub-path as the destination 
   sub-path, the sub-path can append a duplicate of its own contents. Specifying 
   -1 in the parameter aNoOfEdges causes all edges till the end of the source sub-path 
   being copied. Accordingly, with the parameters aSrcNodeNo = 0 and aNoOfEdges 
   = -1 a complete copy of the source sub-path is appended.
   If the destination sub-path contains already curve information, the end of the 
   last existing segment and the start position of the first copied edge are connected 
   with an additional straight line segment. Similarly, if start position for the 
   destination sub-path has been specified by using @Begin(), an additional line 
   segment is added to connect the appended segments with the specified sub-path 
   start position. After the operation is done the actual position in the destination 
   sub-path is moved to refer to the end position of the copied sub-path. If the 
   destination path own matrix is not an identity matrix, the corresponding transformations 
   are applied on all copied positions before storing them in the destination sub-path. 
   See methods @Translate(), @Scale() and @Rotate().
   The method returns the index of the node within the destination sub-path where 
   the start position of the first copied segment has bee stored. Knowing this index 
   and the number of copied segments (aNoOfEdges), the position of every segment 
   can be modified by using the method @SetNode().
   If the sub-paths have not been initialized previously by using the method @InitSubPath(), 
   the destination sub-path has been closed by @Close() method or the memory reserved 
   for the destination sub-path (see parameter aMaxNoOfEdges in the method @InitSubPath()) 
   is occupied by other edges so no sufficient space is available to accommodate 
   aNoOfEdges new segments, the method fails and returns 0.
   Within the path all existing sub-paths are numbered starting with the value 0 
   for the first sub-path, 1 for the second and so far. The maximum number of sub-paths 
   the path can manage is determined by the method @SetMaxNoOfSubPaths(). */
XInt32 GraphicsPath_AddCopy( GraphicsPath _this, XInt32 aSubPathNo, GraphicsPath 
  aSrcPath, XInt32 aSrcSubPathNo, XInt32 aSrcNodeNo, XInt32 aNoOfEdges );

/* The method AddArc() adds to the sub-path a new elliptical arc composed of exact 
   aNoOfEdges straight line segments. The more edges the more smooth the resulting 
   curve. The center of the ellipse is determined in the parameters aCenterX, aCenterY. 
   The parameters aRadiusX and aRadiusY determine the horizontal and vertical radius 
   of the ellipse. The start and the end angle of the arc is determined in the parameters 
   aStartAngle and aEndAngle, both expressed in degree and measured clockwise relative 
   to the positive X-axis of the path coordinate system.
   If the absolute difference between aStartAngle and aEndAngle is equal to or greater 
   than 360.0, the method calculates with a closed ellipse instead of an opened 
   arc. If the parameters aRadiusX and aRadiusY are equal, the method calculates 
   with a circle instead of an ellipse.
   If the affected sub-path contains already curve information, the end of the last 
   existing segment and the start of the new arc (resulting from the parameter aStartAngle) 
   are connected with an additional straight line segment. Similarly, if start position 
   for the sub-path has been specified by using the @Begin() method, an additional 
   line segment is added to connect the arc with the given sub-path start position. 
   After the elliptical arc curve is added, the sub-path actual position is adjusted 
   to refer to the end of the last line segment of the curve (resulting from the 
   parameter aEndAngle).
   The specified position aX, aY is relative to the origin of the path coordinate 
   system. If the path own matrix is not an identity matrix, the corresponding transformations 
   are applied on the resulting arc segments before storing them in the sub-path. 
   See methods @Translate(), @Scale() and @Rotate().
   The method returns the index of the node within the sub-path where the start 
   position of the first arc line segment has been stored (parameter aStartAngle). 
   Knowing this index and the number of segments the arc curve is composed of (aNoOfEdges), 
   the position of every arc line segment can be modified later by using the method 
   @SetNode().
   If the sub-path has not been initialized previously by using the method @InitSubPath(), 
   the sub-path has been closed by @Close() method or the memory reserved for the 
   sub-path (see parameter aMaxNoOfEdges in the method @InitSubPath()) is occupied 
   by other edges so no sufficient space is available to accommodate aNoOfEdges 
   new segments, the method fails and returns 0.
   The affected sub-path is determined in the parameter aSubPathNo. Within the path 
   all existing sub-paths are numbered starting with the value 0 for the first sub-path, 
   1 for the second and so far. The maximum number of sub-paths the path can manage 
   is determined by the method @SetMaxNoOfSubPaths(). */
XInt32 GraphicsPath_AddArc( GraphicsPath _this, XInt32 aSubPathNo, XFloat aCenterX, 
  XFloat aCenterY, XFloat aRadiusX, XFloat aRadiusY, XFloat aStartAngle, XFloat 
  aEndAngle, XInt32 aNoOfEdges );

/* The method AddLine() appends between the actual position of the sub-path and 
   the position aX, aY a new straight line segment consisting of one edge. Then 
   the sub-path actual position is adjusted to refer to the end of the just added 
   line segment (aX, aY).
   Within a sub-path the actual position is either the end position of the preceding 
   sub-path segment or it is the value specified in the invocation of the @Begin() 
   method. If the sub-path is empty and no start position has been specified in 
   the preceding @Begin() invocation, the method assumes the origin of the path 
   coordinate system (X=0, Y=0) as the start position for the new line segment.
   The specified position aX, aY is relative to the origin of the path coordinate 
   system. If the path own matrix is not an identity matrix, the corresponding transformations 
   are applied on the position before storing it in the sub-path. See methods @Translate(), 
   @Scale() and @Rotate().
   The method returns the index of the node within the sub-path where the end position 
   of the new line segment has been stored. Knowing this index, the position can 
   be modified later by using the method @SetNode().
   If the sub-path has not been initialized previously by using the method @InitSubPath(), 
   the sub-path has been closed by @Close() method or the entire memory reserved 
   for the sub-path (see parameter aMaxNoOfEdges in the method @InitSubPath()) is 
   already occupied by other edges, the method fails and returns 0.
   The affected sub-path is determined in the parameter aSubPathNo. Within the path 
   all existing sub-paths are numbered starting with the value 0 for the first sub-path, 
   1 for the second and so far. The maximum number of sub-paths the path can manage 
   is determined by the method @SetMaxNoOfSubPaths(). */
XInt32 GraphicsPath_AddLine( GraphicsPath _this, XInt32 aSubPathNo, XFloat aX, XFloat 
  aY );

/* The method Close() marks the affected sub-path as closed. The method verifies 
   whether the first and the last position of the sub-path are equal and if this 
   is not the case, adds an additional straight line segment to the sub-path in 
   order to connect them together.
   Once the method is invoked, no additional path information can be added to the 
   affected sub-path unless it is initialized or cleared again by using the method 
   @InitSubPath() or @Begin(). If the affected sub-path is empty, the method returns 
   without any effect.
   Within the path all existing sub-paths are numbered starting with the value 0 
   for the first sub-path, 1 for the second and so far. The maximum number of sub-paths 
   the path can manage is determined by the method @SetMaxNoOfSubPaths(). */
void GraphicsPath_Close( GraphicsPath _this, XInt32 aSubPathNo );

/* The method Begin() sets the parameters aX, aY as the start position for the sub-path 
   with the specified number aSubPathNo. The affected sub-path has to be initialized 
   previously by invoking the method @InitSubPath(). Beginning with the specified 
   position, the path can be filled with curve data by using methods like @AddLine(), 
   @AddBezier2(), @AddArc(), etc.
   The specified position aX, aY is relative to the origin of the path coordinate 
   system. If the path own matrix is not an identity matrix, the corresponding transformations 
   are applied on the position before storing it in the sub-path. See methods @Translate(), 
   @Scale() and @Rotate().
   Every sub-path contains exact one start position. Calling this method for a sub-path 
   being already filled with edge coordinates will clear the actual sub-path as 
   if the @InitSubPath() method has been called before.
   Within the path all existing sub-paths are numbered starting with the value 0 
   for the first sub-path, 1 for the second and so far. The maximum number of sub-paths 
   the path can manage is determined by the method @SetMaxNoOfSubPaths(). */
void GraphicsPath_Begin( GraphicsPath _this, XInt32 aSubPathNo, XFloat aX, XFloat 
  aY );

/* The method InitSubPath() prepares the sub-path with the number aSubPathNo to 
   be able to store up to aMaxNoOfEdges path edges (straight line segments). With 
   this operation memory for the sub-path data is reserved. Initially the just initialized 
   sub-path is considered as being empty. To fill the sub-path with data use the 
   methods like @AddLine(), @AddBezier2(), @AddArc(), etc.
   If the affected sub-path has been already initialized in the past, the old information 
   is discarded before initializing the new sub-path. Passing 0 (zero) in the parameter 
   aMaxNoOfEdges results in the old sub-path data being simply released without 
   allocating the memory for the new sub-path.
   If successful, the method returns 'true'. If there is no memory available for 
   the specified number of edges, the method fails and 'false' is returned leaving 
   the sub-path not initialized. Similarly, trying to initialize a sub-path not 
   existing in the path causes the method to return 'false'.
   Within the path all existing sub-paths are numbered starting with the value 0 
   for the first sub-path, 1 for the second and so far. The maximum number of sub-paths 
   the path can manage is determined by the method @SetMaxNoOfSubPaths(). */
XBool GraphicsPath_InitSubPath( GraphicsPath _this, XInt32 aSubPathNo, XInt32 aMaxNoOfEdges );

/* The method CreateFromSVGString() parses the SVG instructions from the given string 
   aSVGString and creates corresponding path data. If the path contained already 
   some data, this data is replaced by the information loaded from the SVG string. 
   The method returns one of the following values:
   - Graphics::SVGParserStatus.Success : The SVG string was evaluated without any 
   problems.
   - Graphics::SVGParserStatus.EmptyString : The SVG string is empty. The resulting 
   path is also empty.
   - Graphics::SVGParserStatus.SyntaxError : The processing of the SVG string has 
   been aborted due to a syntax error. The position where the error has been encountered 
   can be queried by using the method @GetSVGParserErrorPos().
   - Graphics::SVGParserStatus.InvalidArgument : The processing of the SVG string 
   has been aborted due to an invalid argument (one parameter or parameter combination 
   is not valid). The position where the error has been encountered can be queried 
   by using the method @GetSVGParserErrorPos().
   - Graphics::SVGParserStatus.OutOfMemory : The processing of the SVG string has 
   been aborted due to out of memory error. The position where the error has been 
   encountered can be queried by using the method @GetSVGParserErrorPos().
   The parameters aScaleX and aScaleY specify a scaling factors to apply on each 
   coordinate read from the SVG string. Accordingly the original SVG path data can 
   be scaled or even mirrored. The both parameters aDeltaX and aDeltaY add additional 
   offset to the coordinates. The last parameter aAngle specifies an angle to perform 
   a rotation transformation on the original SVG coordinates. The angle is expressed 
   in degree and measured clockwise relative to the positive X-axis. The rotation 
   is performed around the origin position (X:0, Y:0) of the original SVG coordinates.
   For more information regarding SVG string and the syntax please visit: https://www.w3.org/TR/SVG/paths.html. */
XEnum GraphicsPath_CreateFromSVGString( GraphicsPath _this, XString aSVGString, 
  XFloat aScaleX, XFloat aScaleY, XFloat aDeltaX, XFloat aDeltaY, XFloat aAngle );

/* The method GetNoOfEdges() returns how many edges (straight line segments) the 
   sub-path with the given number aSubPathNo actually does store. If the specified 
   sub-path doesn't exist, it has not been initialized by using the method @InitSubPath() 
   or it is still empty, the method returns 0.
   Within the path the sub-paths are numbered starting with the value 0 for the 
   first sub-path, 1 for the second and so far. The maximum number of sub-paths 
   the path can manage is determined by the method @SetMaxNoOfSubPaths(). */
XInt32 GraphicsPath_GetNoOfEdges( GraphicsPath _this, XInt32 aSubPathNo );

/* The method GetMaxNoOfSubPaths() returns how many sub-paths the affected path 
   can maximally store. The returned value corresponds to the parameter passed in 
   the invocation of the @SetMaxNoOfSubPaths() method. */
XInt32 GraphicsPath_GetMaxNoOfSubPaths( GraphicsPath _this );

/* The method SetMaxNoOfSubPaths() changes the number of sub-paths the affected 
   path can maximally manage to the specified value aMaxNoOfSubPaths. After modifying 
   its size the affected path is empty. This means all contents of previously existing 
   sub-paths are discarded. Before new path data can be stored in a sub-path the 
   method @InitSubPath() should be called. Please note, if the size of the path 
   does already correspond to the parameter aMaxNoOfSubPaths, the path remains unaffected 
   and retains all existing contents.
   The method returns 'true' if the path could be initialized with the new size. 
   In case, there is no sufficient memory to store the desired number of sub-paths, 
   the method fails and returns 'false' leaving the path empty. */
XBool GraphicsPath_SetMaxNoOfSubPaths( GraphicsPath _this, XInt32 aMaxNoOfSubPaths );

#ifdef __cplusplus
  }
#endif

#endif /* _GraphicsPath_H */

/* Embedded Wizard */
