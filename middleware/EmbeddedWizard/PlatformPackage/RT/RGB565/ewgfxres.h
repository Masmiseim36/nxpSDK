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
*   This is an internal header of the Embedded Wizard Graphics Engine EWGFX. It
*   declares the platform independent interface to an external bitmap and font
*   resource provider. In Embedded Wizard the access to resources is handled
*   separately by platform specific provider functionality. Depending on the
*   manner to store bitmap and font resources, the provider has the job to find,
*   open, decompress, load, etc. the desired bitmap or font.
*
*   The provider consists of two interfaces:
*
*   1. Bitmap loader. Its intention is to access and load bitmaps, images, etc.
*      Depending on the provider implementation, the bitmaps can be loaded from
*      the code memory or from an external storage medium.
*
*   2. Font loader. Its intention is to access and load fonts and glyphs.
*      Depending on the provider implementation, true type fonts can be opened
*      or pre-rasterized glyphs can be decompressed from the code memory.
*
*   Usually, a loader interface will consist of a header and an implementation
*   files, whose names are predetermined in order to allow the Graphics Engine
*   to access them independent of the particular target system:
*
*   > ewextbmp.h + ewextbmp.c -> for bitmap resource loader.
*
*   > ewextfnt.h + ewextfnt.c -> for font resource loader.
*
*******************************************************************************/

#ifndef EWGFXRES_H
#define EWGFXRES_H


#ifdef __cplusplus
  extern "C" 
  {
#endif


/* Predeklaration of platform dependent resource structures */
struct XBmpRes;
struct XFntRes;


/*******************************************************************************
* FUNCTION:
*   EwBmpInit
*
* DESCRIPTION:
*   The function EwBmpInit() is called during the initialization of the Graphics
*   Engine. Its intention is to give the external loader a chance to initialize
*   its private variables and to start the bitmap decoder functionality (if any)
*
* ARGUMENTS:
*   aArgs - User argument passed in the EwInitGraphicsEngine().
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int EwBmpInit
(
  void*             aArgs
);


/*******************************************************************************
* FUNCTION:
*   EwBmpDone
*
* DESCRIPTION:
*   The function EwBmpDone() will be called during the shutdown of the Graphics
*   Engine. It gives the loader a chance to deinitialize itself just before the
*   Graphics Engine terminates.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBmpDone
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBmpOpen
*
* DESCRIPTION:
*   The function EwBmpOpen() will be called by the Graphics Engine in order to
*   initiate the access to the given bitmap resource. Depending on the manner,
*   the resources are handled in the particular platform system, the function
*   can open an image file or it can access bitmap information available in the
*   code memory.
*
*   The function should return a handle to the opened resource. By using this
*   handle, the bitmap content and its metrics can be requested by the Graphics
*   Engine without taking in account any platform specific aspects.
*
* ARGUMENTS:
*   aResource - Descriptor of the bitmap resource to open. The content of this
*     descriptor depends on the particular platform system.
*
* RETURN VALUE:
*   If sucessful, the function returns the handle to the opened bitmap resource.
*   If failed, the function returns null.
*
*******************************************************************************/
unsigned long EwBmpOpen
(
  const struct XBmpRes* aResource
);


/*******************************************************************************
* FUNCTION:
*   EwBmpClose
*
* DESCRIPTION:
*   The function EwBmpClose() will be called by the Graphics Engine to finish
*   the processing of a bitmap resource. Depending on the manner, the resources
*   are handled in the particular platform system, the function can close the
*   previously opened file and free any temporarily reserved resources.
*
* ARGUMENTS:
*   aHandle - Handle to the bitmap resource to terminate the access.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBmpClose
(
  unsigned long     aHandle
);


/*******************************************************************************
* FUNCTION:
*   EwBmpGetMetrics
*
* DESCRIPTION:
*   The function EwBmpGetMetrics() will be called by the Graphics Engine to get
*   the general attributes of the bitmap resource. By using of these attributes,
*   Graphics Engine can create and initialize video surfaces.
*
* ARGUMENTS:
*   aHandle         - Handle to the bitmap resource to determine its attributes.
*   aFormat         - Pointer to a variable, where the pixel format of the bitmap
*     resource will returned (See EW_PIXEL_FORMAT_XXX).
*   aNoOfFrames     - Pointer to a variable, where the number of frames included
*     within this bitmap resource will returned.
*   aNoOfVirtFrames - Pointer to a variable, where the number of all frames incl.
*     duplicates will be returned. This value can be greater than aNoOfFrames if
*     some of the frames do repeat in the bitmap.
*   aFrameWidth,
*   aFrameHeight    - Pointers to variables, where the size in pixel of a single
*     frame will be returned.
*   aFrameDelay     - Pointer to a variable, where the delay in milliseconds for
*     animated bitmap resources will be returned.
*
* RETURN VALUE:
*   If sucessful, the function returns != 0.
*
*******************************************************************************/
int EwBmpGetMetrics
(
  unsigned long     aHandle,
  int*              aFormat,
  int*              aNoOfFrames,
  int*              aNoOfVirtFrames,
  int*              aFrameWidth,
  int*              aFrameHeight,
  int*              aFrameDelay
);


/*******************************************************************************
* FUNCTION:
*   EwBmpGetFrameMetrics
*
* DESCRIPTION:
*   The function EwBmpGetFrameMetrics() will be called by the Graphics Engine in
*   order to obtain attributes for a single bitmap resource frame.
*
* ARGUMENTS:
*   aHandle      - Handle to the bitmap resource to determine its attributes.
*   aFrameNo     - Number of the desired frame to get its attributes.
*   aOpqX, aOpqY,
*   aOpqWidth,
*   aOpqHeight   - Pointers to variables, where an optional full-opaque area of
*     the frame will be returned. By using this area, drawing operation can be
*     optimized. The returned origin lies relative to the top-left corner of the
*     frame.
*
* RETURN VALUE:
*   If sucessful, the function returns != 0.
*
*******************************************************************************/
int EwBmpGetFrameMetrics
(
  unsigned long     aHandle,
  int               aFrameNo,
  int*              aOpqX,
  int*              aOpqY,
  int*              aOpqWidth,
  int*              aOpqHeight
);


/*******************************************************************************
* FUNCTION:
*   EwBmpGetFrameMemory
*
* DESCRIPTION:
*   The function EwBmpGetFrameMemory() will be called by the Graphics Engine in
*   order to verify whether it is possible to directly access the frame pixel
*   data of the bitmap resource represented by aHandle and in the positive case
*   to obtain the affected memory address pointers.
*
*   This function belongs to the interface separating the Graphics Engine from
*   the platform specific bitmap resource loader functionality. In the case the
*   target system stores the bitmap resources already in a usable format, the
*   function allows the Graphics Engine to use it directly without to need any
*   additional surface memory to be allocated and pixel contents to be copied.
*
*   This approach will not work when the bitmap resources are stored compressed
*   or loaded dynamically from e.g. file system. To work it is essential, that
*   the delivered pixel memory address is persistently valid (e.g. resides in
*   ROM code) and the pixel in the memory are stored in the format used by the
*   surfaces in the target system.
*
*   The function should return 0 if the direct access is not supported.
*
* ARGUMENTS:
*   aHandle  - Handle to the bitmap resource to obtain direct pixel memory
*     access.
*   aFrameNo - Number of the desired frame to get its pixel data.
*   aMemory  - Structure to receive the desired address information.
*
* RETURN VALUE:
*   If successful, the function fills the given aMemory structure with the
*   address information and returns != 0. Otherwise 0 is returned.
*
*******************************************************************************/
int EwBmpGetFrameMemory
(
  unsigned long     aHandle,
  int               aFrameNo,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwBmpLoadFrame
*
* DESCRIPTION:
*   The function EwBmpLoadFrame() will be called by the Graphics Engine in order
*   to load the frame pixel data from the openend bitmap resource aHandle into
*   the given memory area aMemory. Depending on the manner, the resources are
*   handled in the particular platform, the pixel data may be loaded from a file
*   or they can be decompressed from the code memory.
*
* ARGUMENTS:
*   aHandle  - Handle to the bitmap resource to load the pixel data.
*   aFrameNo - Number of the desired frame to get its pixel data.
*   aMemory  - Memory descriptor containing pointers and pitch information of
*     the destination to write the pixel data.
*
* RETURN VALUE:
*   If sucessful, the function returns != 0.
*
*******************************************************************************/
int EwBmpLoadFrame
(
  unsigned long     aHandle,
  int               aFrameNo,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwBmpLoadClut
*
* DESCRIPTION:
*   The function EwBmpLoadClut() will be called by the Graphics Engine in order
*   to load the Clut data from the openend bitmap resource aHandle into the 
*   given memory area aMemory.
*
* ARGUMENTS:
*   aHandle  - Handle to the bitmap resource to load the CLUT data.
*   aMemory  - Memory descriptor containing pointers to the CLUT where to copy
*     the CLUT data.
*
* RETURN VALUE:
*   If sucessful, the function returns != 0.
*
*******************************************************************************/
int EwBmpLoadClut
(
  unsigned long     aHandle,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwBmpLoadMappingTable
*
* DESCRIPTION:
*   The function EwBmpLoadMappingTable() will be called by the Graphics Engine
*   in order to load a table used to map between virtual and real frame numbers.
*   If a bitmap contains repetitions of a frame, the corresponding frames are
*   stored only once (as real frame). The numbers of original frames need to be
*   mapped to the numbers of the real (existing) frames.
*
* ARGUMENTS:
*   aHandle  - Handle to the bitmap resource to load the table.
*   aMapping - Pointer to memory where to copy the table. The memory area has
*     to be large enough.
*
* RETURN VALUE:
*   If sucessful, the function returns the number of virtual frames (the number
*   of copied entries).
*
*******************************************************************************/
int EwBmpLoadMappingTable
(
  unsigned long     aHandle,
  unsigned short*   aMapping
);


/*******************************************************************************
* FUNCTION:
*   EwFntInit
*
* DESCRIPTION:
*   The function EwFntInit() is called during the initialization of the Graphics
*   Engine. Its intention is to give the external loader a chance to initialize
*   its private variables and to start the TrueType font engine functionality
*   (if any).
*
* ARGUMENTS:
*   aArgs - User argument passed in the EwInitGraphicsEngine().
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int EwFntInit
(
  void*             aArgs
);


/*******************************************************************************
* FUNCTION:
*   EwFntDone
*
* DESCRIPTION:
*   The function EwFntDone() will be called during the shutdown of the Graphics
*   Engine. It gives the loader a chance to deinitialize itself just before the
*   Graphics Engine terminates.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFntDone
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwFntOpen
*
* DESCRIPTION:
*   The function EwFntOpen() will be called by the Graphics Engine in order to
*   initiate the access to the given font resource. Depending on the manner,
*   the resources are handled in the particular platform system, the function
*   can open a TrueType file or it can access font information available in the
*   code memory.
*
*   The function should return a handle to the opened resource. By using this
*   handle, the font content and its metrics can be requested by the Graphics
*   Engine without taking in account any platform specific aspects.
*
* ARGUMENTS:
*   aResource - Descriptor of the font resource to open. The content of this
*     descriptor depends on the particular platform system.
*
* RETURN VALUE:
*   If sucessful, the function returns the handle to the opened font resource.
*   If failed, the function returns null.
*
*******************************************************************************/
unsigned long EwFntOpen
(
  const struct XFntRes*  aResource
);


/*******************************************************************************
* FUNCTION:
*   EwFntClose
*
* DESCRIPTION:
*   The function EwFntClose() will be called by the Graphics Engine to finish
*   the processing of a font resource. Depending on the manner, the resources
*   are handled in the particular platform system, the function can close the
*   previously opened file and free any temporarily reserved resources.
*
* ARGUMENTS:
*   aHandle - Handle to the font resource to terminate the access.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFntClose
(
  unsigned long     aHandle
);


/*******************************************************************************
* FUNCTION:
*   EwFntGetMetrics
*
* DESCRIPTION:
*   The function EwFntGetMetrics() will be called by the Graphics Engine to get
*   the general attributes of the font resource. By using of these attributes,
*   Graphics Engine can create and initialize new font instances.
*
* ARGUMENTS:
*   aHandle   - Handle to the font resource to determine its attributes.
*   aAscent,
*   aDescent,
*   aLeading  - Pointers to variables, where the font metrics will be returned.
*   aDefChar  - Pointers to variable, where the default character code will be
*     returned.
*
* RETURN VALUE:
*   If sucessful, the function returns != 0.
*
*******************************************************************************/
int EwFntGetMetrics
(
  unsigned long     aHandle,
  int*              aAscent,
  int*              aDescent,
  int*              aLeading,
  XChar*            aDefChar
);


/*******************************************************************************
* FUNCTION:
*   EwFntGetGlyphMetrics
*
* DESCRIPTION:
*   The function EwFntGetGlyphMetrics() will be called by the Graphics Engine in
*   order to obtain the metrics of a single glyph from the given font.
*
* ARGUMENTS:
*   aHandle   - Handle to the font resource to determine its attributes.
*   aCharCode - Character code of the glyph to be inspected.
*   aOriginX,
*   aOriginY  - Pointers to variables, where the origin offset in pixel of the
*     desired glyph will be returned.
*   aWidth,
*   aHeight   - Pointers to variables, where the size in pixel of the glyph will
*     be returned.
*   aAdvance  - Pointer to a variable, where the horizontal advance in pixel for
*     the glyph will be returned.
*
* RETURN VALUE:
*   If sucessful, the function returns != 0.
*
*******************************************************************************/
int EwFntGetGlyphMetrics
(
  unsigned long     aHandle,
  unsigned short    aCharCode,
  int*              aOriginX,
  int*              aOriginY,
  int*              aWidth,
  int*              aHeight,
  int*              aAdvance
);


/*******************************************************************************
* FUNCTION:
*   EwFntGetKerning
*
* DESCRIPTION:
*   The function EwFntGetKerning() will be called by the Graphics Engine in
*   order to obtain the kerning value for the pair of two glyphs from the given
*   font.
*
* ARGUMENTS:
*   aHandle    - Handle to the font resource to determine the kerning.
*   aCharCode1 - Character code of the first glyph.
*   aCharCode2 - Character code of the second glyph.
*
* RETURN VALUE:
*   The function returns the kerning value for the given pair of glyphs or 0
*   if the glyphs are not affected by kerning or at least one of the glyphs is
*   not existing in the font.
*
*******************************************************************************/
int EwFntGetKerning
(
  unsigned long     aHandle,
  unsigned short    aCharCode1,
  unsigned short    aCharCode2
);


/*******************************************************************************
* FUNCTION:
*   EwFntIsGlyphAvailable
*
* DESCRIPTION:
*   The function EwFntIsGlyphAvailable() will be called by the Graphics Engine
*   in order to verify whether the given font contains a glyph for the character
*   aCharCode.
*
* ARGUMENTS:
*   aHandle   - Handle to the font resource to determine the glyph existence.
*   aCharCode - Character code of the glyph.
*
* RETURN VALUE:
*   The function returns != 0 if the glyph exists. Otherwise 0 is returned.
*
*******************************************************************************/
int EwFntIsGlyphAvailable
(
  unsigned long     aHandle,
  unsigned short    aCharCode
);


/*******************************************************************************
* FUNCTION:
*   EwFntLoadGlyph
*
* DESCRIPTION:
*   The function EwFntLoadGlyph() will be called by the Graphics Engine in order
*   to load the glyph pixel data from the openend font resource aHandle into the
*   given memory area aMemory. Depending on the manner, a resource is handled in
*   the particular platform, the pixel data may be loaded from a TrueType file
*   or they can be decompressed from the code memory.
*
* ARGUMENTS:
*   aHandle   - Handle to the font resource to load its glyph pixel data.
*   aCharCode - Character code of the glyph to be loaded.
*   aMemory   - Memory descriptor containing pointers and pitch information of
*     the destination to write the glyph pixel data.
*
* RETURN VALUE:
*   If sucessful, the function returns != 0.
*
*******************************************************************************/
int EwFntLoadGlyph
(
  unsigned long     aHandle,
  unsigned short    aCharCode,
  XSurfaceMemory*   aMemory
);


#ifdef __cplusplus
  }
#endif

#endif /* EWGFXRES_H */


/* msy, pba */
