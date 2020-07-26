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
*   declares the core functionality of the engine:
*
*   > Surface and font glyph management. Creation and destroying of surfaces
*     and glyphs. Cache management.
*
*   > Drawing packages - so called 'issues' and the enclosed drawing 'tasks'.
*     An issue provides storage for drawing operations. This builds the base 
*     for the optimization of the drawing process.
*
*   > State machines to control the execution of pending issues.
*
*******************************************************************************/

#ifndef EWGFXCORE_H
#define EWGFXCORE_H


#ifdef __cplusplus
  extern "C"
  {
#endif


/*******************************************************************************
* MACRO:
*   EW_LOCK_XXX
*
* DESCRIPTION:
*   The following enumeration defines the possible modes used when obtaining
*   a direct access to a surface memory. These modes serve as hints only. They
*   determine the intention of the user only and help to avoid any unnecessary
*   operation when locking surfaces.
*
* ELEMENTS:
*   EW_LOCK_PIXEL_READ       - Allow read access to the surface pixel memory.
*   EW_LOCK_PIXEL_WRITE      - Allow write access to the pixel surface memory.
*   EW_LOCK_PIXEL_READ_WRITE - Allow both the read and the write access.
*   EW_LOCK_CLUT_READ        - Allow read access to the Index8 surface CLUT.
*   EW_LOCK_CLUT_WRITE       - Allow write access to the Index8 surface CLUT.
*   EW_LOCK_CLUT_READ_WRITE  - Allow both the read and the write access.
*   EW_LOCK_DONT_WAIT        - Don't wait for the surface if it is involved in
*     other pending drawing operations. This mode can cause unexpected results
*     when CPU tries to read or write the surface pixel memory and at the same
*     time the surface is modified by the graphics subsystem.
*
*******************************************************************************/
#define EW_LOCK_PIXEL_READ        0x01
#define EW_LOCK_PIXEL_WRITE       0x02
#define EW_LOCK_PIXEL_READ_WRITE  0x03
#define EW_LOCK_CLUT_READ         0x04
#define EW_LOCK_CLUT_WRITE        0x08
#define EW_LOCK_CLUT_READ_WRITE   0x0C
#define EW_LOCK_DONT_WAIT         0x80


/*******************************************************************************
* MACRO:
*   EW_TASK_XXX
*
* DESCRIPTION:
*   The following enumeration defines flags to control the execution of a task.
*   Beside this, the flags also should simplify the evaluation and optimization
*   of pending tasks.
*
* ELEMENTS:
*   EW_TASK_ALPHABLEND      - If set, determines, that the drawing operation
*     should apply the alpha blending algorithm for all drawn pixel. If not set,
*     the drawn pixel simply overwrite the previous content of the surface.
*   EW_TASK_SOLID           - If set, determines, that the drawn pixel should
*     be modulated by a solid color or opacity value.
*   EW_TASK_GRADIENT        - If set, determines, that the drawn pixel should
*     be modulated by a color or opacity complex gradient.
*   EW_TASK_LINEAR_GRADIENT - If set, determines, that the drawn pixel should
*     be modulated by a color or opacity linear (vertical or horizontal) 
*     gradient.
*   EW_TASK_HIGH_QUALITY    - If set, the drawing operation should be performed
*     with the best possible quality. This flag is just a hint. Its meaning 
*     depends on the capability of the underlying graphics subsystem.
*   EW_TASK_ENTIRE_AREA     - If set, determines that the task will affect the
*     entire area of the surface specified in the task variables X1,Y1 .. X2,Y2.
*     If not set, some of the pixel within the area may remain unchanged (e.g.
*     when drawing a text).
*   EW_TASK_OVERLAP         - If set, determines an optional task, which can be
*     simply ignored if no elimination took place for the underlying sibling
*     tasks. This flag is used exclusively for bitmaps composed of a full opaque
*     and a semi-transparent area. When drawing such a bitmap, the opaque area
*     can be handled separately in order to optimize the operation, otherwise
*     the entire bitmap needs to be handled as a semi-transparent bitmap.
*   EW_TASK_EMULATION       - If set, the drawing operation should be performed
*     by the software emulation. If not set, the underlying graphics subsystem
*     should perform the operation.
*
*******************************************************************************/
#define EW_TASK_ALPHABLEND          0x0001
#define EW_TASK_SOLID               0x0002
#define EW_TASK_GRADIENT            0x0004
#define EW_TASK_LINEAR_GRADIENT     0x0006
#define EW_TASK_HIGH_QUALITY        0x0008
#define EW_TASK_ENTIRE_AREA         0x0100
#define EW_TASK_OVERLAP             0x0200
#define EW_TASK_EMULATION           0x8000
#define EW_TASK_MODE_MASK           0x000F


/*******************************************************************************
* MACRO:
*   EW_TASKID_XXX
*
* DESCRIPTION:
*   The following enumeration defines command tokens for the drawing tasks.
*
* ELEMENTS:
*   EW_TASKID_FILL_RECTANGLE      - Fill a rectangular area with a solid color
*     or with a color gradient.
*   EW_TASKID_COPY_SURFACE        - Copy an area from a source surface into an
*     area within a destination surface.
*   EW_TASKID_TILE_SURFACE        - Fill an area within the destination surface
*     with repetitions of an area from a source surface.
*   EW_TASKID_WARP_PERSP_SURFACE  - Project a rectangular area from the source
*     surface on a four point polygon within a destination surface. Perspective
*     projection will take place.
*   EW_TASKID_WARP_AFFINE_SURFACE - Project a rectangular area from the source
*     surface on a four point polygon within a destination surface. Affine only
*     projection will take place.
*   EW_TASKID_WARP_SCALE_SURFACE  - Project a rectangular area from the source
*     surface on a four point polygon within a destination surface. Scale only
*     projection will take place.
*   EW_TASKID_DRAW_LINE           - Draw a line with a solid color or with a
*     color gradient.
*   EW_TASKID_DRAW_TEXT           - Draw text fragment with a solid color or
*     with a color gradient.
*   EW_TASKID_FILL_POLYGON        - Fill a polygon with a solid color or with
*     a color gradient.
*
*******************************************************************************/
#define EW_TASKID_FILL_RECTANGLE       1
#define EW_TASKID_COPY_SURFACE         2
#define EW_TASKID_TILE_SURFACE         3
#define EW_TASKID_WARP_PERSP_SURFACE   4
#define EW_TASKID_WARP_AFFINE_SURFACE  5
#define EW_TASKID_WARP_SCALE_SURFACE   6
#define EW_TASKID_DRAW_LINE            7
#define EW_TASKID_DRAW_TEXT            8
#define EW_TASKID_FILL_POLYGON         9


/*******************************************************************************
* TYPE:
*   XSurface
*
* DESCRIPTION:
*   The type XSurface defines the structure to cover attributes of the generic,
*   platform independent surface. It allows the Graphics Engine to handle with
*   surfaces without taking in account the real representation of the surface
*   in the underlying graphics subsystem.
*
*   Beside this, the XSurface structure complements the sophisticated concepts
*   of the Graphics Engine to perform operations in a very efficient way:
*
*   - The surface cache. All existing surfaces are stored within a global cache
*     and can be reused very quickly - as long as there is still video memory
*     available. The repeated surface allocation, loading and deallocation can
*     be omitted in this manner. 
*
*   - Lazy drawing tasks. The drawing operations affecting a destination surface
*     are performed in blocks - as so-called issues. As long as the composition
*     of a surface is still not completed, the prepared graphics tasks remain
*     waiting within the issue. As soon as the issue becomes flushed, the tasks
*     are evaluated and passed to the underlying graphics subsystem. During the
*     evaluation pass, the Graphics Engine can detect and eliminate superfluous
*     operations, the order of operations can be changed, or other optimization
*     steps can be executed on the waiting drawing tasks.
*
* ELEMENTS:
*   Newer,
*   Older   - References to the sibling surfaces within the surface cache.
*   Owned   - Usage counter to determine, whether the surface has an owner.
*     As soon as the counter == 0, the surface is considered as ready for a
*     new owner. Note, not owned surfaces may still be involved in pending
*     drawing operations.
*   Used    - Usage counter to determine, whether the surface is involved in
*     tasks pending for other surfaces or if it is helt by its owner. As long 
*     as the counter is > 0, the surface may not be released.
*   Pending - Counter for completely composed but still waiting issues for this
*     surface. As long as the counter > 0, there are incomplete drawing tasks
*     with this surface as the destination.
*   Owner   - The owner in context of which the surface is stored. Usually this
*     is a XBitmap.
*   Tag1,
*   Tag2    - Unique identification for the surface. Useful to find surfaces
*     within the cache with the objective to reuse them.
*   Format  - Pixel format of the surface. (See EW_PIXEL_FORMAT_XXX).
*   Width,
*   Height  - Size of the surface in pixel.
*   MemSize - Estimated size of memory in bytes used to cover the surface.
*   DoCache - Determines what to do when the surface is freed again. If this
*     value is != 0, the surface will remain in the surface cache (if there
*     is enough space in the cache). If this value is == 0, the surface is
*     discarded automatically at the moment when it is freed.
*   Stamp   - Identifies the screen update, this surface has been used last 
*     time.
*   Tasks   - Issue currently in the drawing composition. The issue stores the
*     drawing operations as a list of tasks.
*   Handle  - Reference to the real surface or to its video memory. This value
*    depends on the underlying graphics subsystem.
*
*******************************************************************************/
typedef struct XSurface
{
  struct XSurface*  Newer;
  struct XSurface*  Older;
  int               Owned;
  int               Used;
  int               Pending;
  void*             Owner;
  unsigned long     Tag1;
  unsigned long     Tag2;
  short             Format;
  short             Width;
  short             Height;
  char              Reserved[2];
  int               MemSize;
  int               DoCache;
  int               Stamp;
  struct XIssue*    Tasks;
  unsigned long     Handle;
} XSurface;


/*******************************************************************************
* TYPE:
*   XGlyph
*
* DESCRIPTION:
*   The type XGlyph defines the structure to cover attributes and metrics of a
*   glyph after it has been loaded into the global glyph cache surface. In this
*   manner the glyphs can be handled by the Graphics Engine as areas of a simple
*   surface - the text composition is performed by copying of these areas from
*   the glyph cache surface into a destination surface. This is a convenient way
*   to handle glyphs by the underlying graphics subsystem.
*
*   The area allocation within the glyph cache surface is performed by a special
*   kind of 2D memory manager. It ensures a fair surface occupation with as few
*   as possible waste.
*
* ELEMENTS:
*   Newer,
*   Older    - References to the sibling glyphs within the global glyph cache.
*   Next,
*   Prev     - References to the sibling glyphs within the hashed glyph list.
*   Used     - Usage counter to determine, whether the glyph is involved in any
*     tasks pending for other surfaces or if it is used by its owner. As long
*     as the counter is > 0, the glyph may not be released.
*   Font,
*   CharCode - Unique identification for the glyph. Useful to find glyphs within
*     the cache with the objective to reuse them.
*   Width,
*   Height   - Size of the glyph in pixel (means the size of the ink-box).
*   OriginX,
*   OriginY  - Upper left corner of the glyph, relative to the current printing
*     position on the baseline.
*   Advance  - Advance in horizontal direction added to the printing position.
*   CacheX,
*   CacheY   - The storage location of the glyph within the cache surface.
*   Pixel    - Option copy of the glyph pixel data in case of target systems
*     where no read access to glyph cache surface is possible.
*
*******************************************************************************/
typedef struct XGlyph
{
  struct XGlyph*    Newer;
  struct XGlyph*    Older;
  struct XGlyph*    Next;
  struct XGlyph*    Prev;
  int               Used;
  unsigned long     Font;
  unsigned long     CharCode;
  short             Width;
  short             Height;
  short             OriginX;
  short             OriginY;
  short             Advance;
  short             CacheX;
  short             CacheY;
  char              Reserved[2];
  unsigned char*    Pixel;
} XGlyph;


/*******************************************************************************
* TYPE:
*   XSurfaceLock
*
* DESCRIPTION:
*   The XSurfaceLock structure provides information needed for the direct access
*   to the memory of a previously locked surface. It includes the pointer to the
*   pixel memory, the distance between two pixel rows, etc. In case of an Index8
*   surface, also the CLUT can be accessed directly.
*
* ELEMENTS:
*   Memory - Descriptor containing pointers and distance information for the
*     surface pixel and color table.
*   X, Y,
*   Width,
*   Height - Surface area guaranteed by this lock. The access outside this area
*     may cause an access violation. The area also serves as a hint to optimize
*     and limit memory transfers to/from the video memory.
*     Please note: The area is already expressed in coordinates valid within the
*     surface by taking in account all particular configuration aspects like the
*     default rotation of the surface content.
*   Index,
*   Count  - Range of CLUT entries guaranteed by this lock. The access outside
*     this range may cause an access violation. The range also serves as a hint
*     to optimize and limit memory transfers to/from the video memory.
*   Mode   - Kind of operations, the user can perform on the locked memory. Mode
*     also serves as a hint to optimize and limit memory transfers to/from the
*     video memory. (See EW_LOCK_XXX).
*   Handle - Reference to the real surface lock or to the temporarily allocated
*     memory area. This value depends on the underlying graphics subsystem.
*
*******************************************************************************/
typedef struct
{
  XSurfaceMemory    Memory;
  short             X;
  short             Y;
  short             Width;
  short             Height;
  short             Index;
  short             Count;
  unsigned short    Mode;
  char              Reserved[2];
  unsigned long     Handle;
} XSurfaceLock;


/*******************************************************************************
* TYPE:
*   XIssue
*
* DESCRIPTION:
*   The XIssue structure incarnates the idea of a single drawing transaction,
*   which in turn is composed of several different drawing tasks. The intention
*   behind a transaction is to handle the enclosed tasks as a single entity.
*
*   Beside this, the XIssue structure provides an own memory manager for the
*   waiting drawing tasks. This management is realized as two separate heaps,
*   which do share the same memory area. Whereas the lower heap occupies the
*   lower address space of the memory area, the upper heap resides in the upper
*   address space. The idea is to let grow the one heap against the other. The
*   remaining gap between th both heaps is the empty still unused memory.
*
* ELEMENTS:
*   List      - Reference to a list, where this issue currently does belong to.
*     The membership to a particular list determines the state of the issue.
*     (issue is pending, running, finished, etc.)
*   Next,
*   Prev      - References to the next/prev sibling issues within the same list.
*   Surface   - Destination surface affected by this issue. This is the surface,
*     where the drawing operations take place.
*   Tasks     - Origin of a list of pending drawing operations prepared for this
*     issue. The tasks will be reserved dynamically on the private upper heap of
*     this issue.
*   LowerHeap,
*   UpperHeap - Pointers to the respective heap. The area between these both
*     pointers is still free.
*   UpperEnd  - Upper end of the issue heap.
*   Usage     - Memory usage of the issue. The value is used for profiling only.
*   Peak      - Tracks the maxim. memory usage for the biggest task. The value
*     is used for profiling only.
*
*******************************************************************************/
typedef struct XIssue
{
  struct XIssues*   List;
  struct XIssue*    Next;
  struct XIssue*    Prev;
  XSurface*         Surface;
  struct XTask*     Tasks;
  char*             LowerHeap;
  char*             UpperHeap;
  char*             UpperEnd;
  int               Usage;
  int               Peak;
} XIssue;


/*******************************************************************************
* TYPE:
*   XIssues
*
* DESCRIPTION:
*   The XIssues structure provides a mechanism to maintain of issues in a list.
*   The order of the issues within the list reflects the order in which these
*   issues will be evaluated, processed and finally freed.
*
* ELEMENTS:
*   Head, 
*   Tail  - References to the first and last issue within the list. The issues
*     are stored in a chained list.
*
*******************************************************************************/
typedef struct XIssues
{
  XIssue*           Head;
  XIssue*           Tail;
} XIssues;


/*******************************************************************************
* TYPE:
*   XTask
*
* DESCRIPTION:
*   The XTask structure covers a single pending drawing task. The task describes
*   the area affected by the drawing operation, the mode how to perform it and a
*   set of additional operation dependent parameters for this task, like colors,
*   source surfaces, glyphs, etc.
*   
*   Several tasks will be stored and managed together within so called issues.
*   As soon as an issue becomes flushed, the enclosed tasks will be optimized
*   and then executed.
*
* ELEMENTS:
*   Next    - Reference to the next pending task within the same issue.
*   Token   - Unique token to identify the kind of the drawing operation.
*     (See EW_TASKID_XXX).
*   Flags   - Bitfield of options how to perform and optimize the drawing task.
*     (See EW_TASK_XXX).
*   X1, Y1,
*   X2, Y2  - Area of the destination surface affected by the drawing operation.
*   Data    - Optional parameters for the drawing task.
*
*******************************************************************************/
typedef struct XTask
{
  struct XTask*     Next;
  short             Token;
  short             Flags;
  short             X1;
  short             Y1;
  short             X2;
  short             Y2;
  void*             Data;
} XTask;


/*******************************************************************************
* FUNCTION:
*   EwInitGfxCore
*
* DESCRIPTION:
*   The function EwInitGfxCore() initializes the Graphics Engine core module.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int EwInitGfxCore
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwDoneGfxCore
*
* DESCRIPTION:
*   The function EwDoneGfxCore() deinitializes the Graphics Engine core module.
*   If necessary, the function discards all existing surfaces and frees the
*   used resources.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDoneGfxCore
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwCreateSurface
*
* DESCRIPTION:
*   The function EwCreateSurface() creates a new surface with the given size and
*   pixel format. The additional tag values serve for identification purpose and
*   simplifies the reuse of previously freed surfaces -> see the EwFindSurface()
*   function.
*
*   In order to create a new surface, the function forwards the request to the
*   underlying graphics subsystem. If the request could not be handled by the
*   subsystem due to a resource deficit, the function tries to flush the caches
*   and forces the processing of pending drawing operations until the subsystem
*   can handle the request sucessfully. If all the adopted measures didn't help
*   to handle the request, the function fails and returns null.
*
*   The newly created surface passes into the ownership of the caller of this
*   function. In order to release the surface, EwFreeSurface() should be used.
*
* ARGUMENTS:
*   aFormat - Pixel format of the desired surface. (See EW_PIXEL_FORMAT_XXX).
*   aWidth,
*   aHeight - Size of the surface in pixel.
*   aOwner  - The owner in context of which the surface will be stored.
*   aTag1,
*   aTag2   - Unique identification for the surface.
*
* RETURN VALUE:
*   If successful, EwCreateSurface() returns the new surface otherwise null is
*   returned.
*
*******************************************************************************/
XSurface* EwCreateSurface
(
  int               aFormat,
  int               aWidth,
  int               aHeight,
  void*             aOwner,
  unsigned long     aTag1,
  unsigned long     aTag2
);


/*******************************************************************************
* FUNCTION:
*   EwCreateConstSurface
*
* DESCRIPTION:
*   The function EwCreateConstSurface() creates a new surface with the given 
*   size, pixel format and pixel information. Important here is the fact that
*   the function doesn't reserve any memory to store the pixel information.
*   Instead it, the surface associates the pixel data passed in the parameter
*   aMemory.
*
*   The additional tag values serve for identification purpose and simplifies
*   the reuse of previously freed surfaces -> see the EwFindSurface() function.
*
*   In order to create a new surface, the function forwards the request to the
*   underlying graphics subsystem. If the request could not be handled by the
*   subsystem due to a resource deficit, the function tries to flush the caches
*   and forces the processing of pending drawing operations until the subsystem
*   can handle the request sucessfully. If all the adopted measures didn't help
*   to handle the request, the function fails and returns null.
*
*   The newly created surface passes into the ownership of the caller of this
*   function. In order to release the surface, EwFreeSurface() should be used.
*
* ARGUMENTS:
*   aFormat - Pixel format of the desired surface. (See EW_PIXEL_FORMAT_XXX).
*   aWidth,
*   aHeight - Size of the surface in pixel.
*   aOwner  - The owner in context of which the surface will be stored.
*   aTag1,
*   aTag2   - Unique identification for the surface.
*   aMemory - Structure to pass the ROM address information.
*
* RETURN VALUE:
*   If successful, EwCreateSurface() returns the new surface otherwise null is
*   returned.
*
*******************************************************************************/
XSurface* EwCreateConstSurface
(
  int               aFormat,
  int               aWidth,
  int               aHeight,
  void*             aOwner,
  unsigned long     aTag1,
  unsigned long     aTag2,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwFindSurface
*
* DESCRIPTION:
*   The function EwFindSurface() searches the cache for an unused surface with
*   the given parameters and if successfull, reactivates it. If no surface
*   matches these parameters, the function fails and returns null.
*   
*   The found surface passes to the ownership of the caller of this function.
*   In order to release the surface, EwFreeSurface() should be used.
*
* ARGUMENTS:
*   aFormat - Pixel format of the desired surface. (See EW_PIXEL_FORMAT_XXX).
*   aWidth,
*   aHeight - Size of the surface in pixel.
*   aOwner  - The owner in context of which the surface will be stored after
*     it has been found.
*   aTag1,
*   aTag2   - Unique identification for the surface.
*
* RETURN VALUE:
*   If successful, EwFindSurface() returns the found surface otherwise null is
*   returned.
*
*******************************************************************************/
XSurface* EwFindSurface
(
  int               aFormat,
  int               aWidth,
  int               aHeight,
  void*             aOwner,
  unsigned long     aTag1,
  unsigned long     aTag2
);


/*******************************************************************************
* FUNCTION:
*   EwFreeSurface
*
* DESCRIPTION:
*   The function EwFreeSurface() decrements the usage counter of the surface. If
*   not involved in any other drawing operations the surface is considered as an
*   unused surface.
*
*   Freed surfaces do remain in the cache until they are reactivated or due to a
*   resource deficit displaced from the video memory.
*
* ARGUMENTS:
*   aSurface - Surface to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeSurface
(
  XSurface*         aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwLockSurface
*
* DESCRIPTION:
*   The function EwLockSurface() has the job to lock an area of the surface and
*   to provide the caller with a pointer to the surface memory. In this manner
*   the surface memory can be read and modified. After finishing the memory
*   access, the surface should be unlocked again by calling EwUnlockSurface().
*
*   Depending on the specified mode, the function may force the Graphics Engine
*   to flush pending tasks and to wait for the surface if there are any pending
*   tasks affecting the surface. To avoid this kind of synchronization, the flag
*   EW_LOCK_DONT_WAIT should be set in the aMode parameter. Regardless of this
*   additional flag, the underlying gryphics subsystem will always perform the
*   necessary synchronisation steps in order to avoid any interferences with
*   hardware executed drawing operations.
*   
*   The function fills the passed lock structure with the lock information. Here
*   the caller will find a pointer to the pixel memory and the distance between
*   two pixel rows. The memory returned by the function is not guaranteed to be
*   the real video memory of the surface. If necessary, the function will handle
*   the access by using a shadow memory area. Therefore to limit the effort of
*   memory copy operations, the desired surface area and the access mode should
*   be specified.
*
*   If there was not possible to lock the surface, the function fails and 
*   returns zero.
*
* ARGUMENTS:
*   aSurface - Surface to obtain the direct memory access.
*   aX, aY,
*   aWidth,
*   aHeight  - Limitation for the memory access to an area.
*   aIndex,
*   aCount   - Range of CLUT entries to lock. This is for Index8 surfaces only.
*   aMode    - Desired access mode to the surface memory (see EW_LOCK_XXX).
*   aLock    - Pointer to a structure where the lock information will be stored.
*
* RETURN VALUE:
*   If successful, the function returns != 0, otherwise 0 ist returned.
*
*******************************************************************************/
int EwLockSurface
(
  XSurface*         aSurface,
  int               aX,
  int               aY,
  int               aWidth,
  int               aHeight,
  int               aIndex,
  int               aCount,
  unsigned short    aMode,
  XSurfaceLock*     aLock
);


/*******************************************************************************
* FUNCTION:
*   EwUnlockSurface
*
* DESCRIPTION:
*   The function EwUnlockSurface() provides a counterpart of EwLockSurface().
*   When called, the function releases the given lock and updates the affected
*   surface. If necessary, the made modifications are transfered to the video
*   memory of the surface.
*
* ARGUMENTS:
*   aSurface - Surface to terminate the direct memory access.
*   aLock    - Lock descriptor initialized by the previous EwLockSurface() call.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwUnlockSurface
(
  XSurface*         aSurface,
  XSurfaceLock*     aLock
);


/*******************************************************************************
* FUNCTION:
*   EwCreateGlyph
*
* DESCRIPTION:
*   The function EwCreateGlyph() creates a new glyph with the given size. The
*   both additional font and charcode values serve for identification purpose
*   and simplify the reuse of already existing glyphs.
*
*   In order to create a new glyph, EwCreateGlyph() reserves an area within the
*   global glyph cache surface. If there is no space available in this surface,
*   the function forces the processing of still pending drawing operations and
*   flushes the glyph cache. If all the adopted measures didn't help to attend
*   the allocation, the function fails and returns null.
*
*   The newly created glyph passes into the ownership of the caller. In order
*   to release the glyph, EwFreeGlyph() should be used.
*
* ARGUMENTS:
*   aWidth,
*   aHeight   - Size of the glyph in pixel.
*   aFont,
*   aCharCode - Unique identification for the glyph.
*
* RETURN VALUE:
*   If successful, EwCreateGlyph() returns the new glyph otherwise null is
*   returned.
*
*******************************************************************************/
XGlyph* EwCreateGlyph
(
  int               aWidth,
  int               aHeight,
  unsigned long     aFont,
  unsigned long     aCharCode
);


/*******************************************************************************
* FUNCTION:
*   EwFindGlyph
*
* DESCRIPTION:
*   The function EwFindGlyph() searches the glyph cache for a glyph matching
*   the given font and charcode values. If no glyph could be found, the function
*   fails and returns null.
*   
*   The found glyph passes to the ownership of the caller of this function. In
*   order to release the glyph, EwFreeGlyph() should be used.
*
* ARGUMENTS:
*   aFont,
*   aCharCode - Unique identification for the glyph.
*
* RETURN VALUE:
*   If successful, EwFindGlyph() returns the found glyph otherwise null is
*   returned.
*
*******************************************************************************/
XGlyph* EwFindGlyph
(
  unsigned long     aFont,
  unsigned long     aCharCode
);


/*******************************************************************************
* FUNCTION:
*   EwFreeGlyph
*
* DESCRIPTION:
*   The function EwFreeGlyph() decrements the usage counter of the glyph. If not
*   involved in any other drawing operations the glyph is considered as unused.
*
*   Freed glyphs do remain in the cache until they are used again or due to a
*   resource deficit displaced from the video memory.
*
* ARGUMENTS:
*   aGlyph - Glyph to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeGlyph
(
  XGlyph*           aGlyph
);


/*******************************************************************************
* FUNCTION:
*   EwDiscardGlyph
*
* DESCRIPTION:
*   The function EwDiscardGlyph() forces the affected glyph to be removed from
*   the glyph cache if the glyph is not iun use actually. Should the glyph be
*   used, nothing happens.
*
* ARGUMENTS:
*   aGlyph - Glyph to eventually discard.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDiscardGlyph
(
  XGlyph*           aGlyph
);


/*******************************************************************************
* FUNCTION:
*   EwLockGlyphCache
*
* DESCRIPTION:
*   The function EwLockGlyphCache() provides a direct access to the pixel memory
*   of the given glyph. In this manner the memory can be read and modified. When
*   finishing the memory access, the glyph should be unlocked again by using the
*   function EwUnlockGlyphCache().
*
*   Because all glyphs are stored within the glyph cache surface, the request 
*   will be passed to the EwLockSurface() function, which in turn will lock the
*   glyph area within the surface.
*
*   Depending on the specified mode, the function may force the Graphics Engine
*   to flush pending tasks and to wait for the glyph cache surface if there are
*   pending tasks affecting the surface. To avoid this kind of synchronization,
*   the flag EW_LOCK_DONT_WAIT should be set in the aMode parameter. Regardless
*   of this additional flag, the underlying gryphics subsystem always performs
*   the necessary synchronisation steps in order to avoid any interferences with
*   hardware executed drawing operations.
*   
*   The function fills the passed lock structure with the lock information. Here
*   the caller will find a pointer to the pixel memory and the distance between
*   two pixel rows. The memory returned by the function is not guaranteed to be
*   the real video memory of the surface. If necessary, the function will handle
*   the access by using a shadow memory area. Therefore to limit the effort of
*   memory copy operations, the desired access mode should be specified.
*
*   If there was not possible to lock the glyph, the function fails and returns
*   zero.
*
* ARGUMENTS:
*   aGlyph - Glyph to obtain the direct memory access.
*   aMode  - Desired access mode to the glyph pixel memory (see EW_LOCK_XXX).
*   aLock  - Pointer to a structure where the lock information will be stored.
*
* RETURN VALUE:
*   If successful, the function returns != 0, otherwise 0 is returned.
*
*******************************************************************************/
int EwLockGlyphCache
(
  XGlyph*           aGlyph,
  unsigned short    aMode,
  XSurfaceLock*     aLock
);


/*******************************************************************************
* FUNCTION:
*   EwUnlockGlyphCache
*
* DESCRIPTION:
*   The function EwUnlockGlyphCache() provides a counterpart to the function
*   EwLockGlyphCache(). When called, the function releases the given lock and
*   updates the global glyph cache surface. If necessary, the made modifications
*   are transfered to the video memory of the surface.
*
* ARGUMENTS:
*   aGlyph - Glyph to terminate the direct memory access.
*   aLock  - Lock descriptor initialized by the previous call to the function
*     EwLockGlyphCache().
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwUnlockGlyphCache
(
  XGlyph*           aGlyph,
  XSurfaceLock*     aLock
);


/*******************************************************************************
* FUNCTION:
*   EwInitIssue
*
* DESCRIPTION:
*  The function EwInitIssue() introduces the preparation of a new drawing task
*  for the specified surface. The surface will remain in this state until the
*  function EwDoneIssue() has been invoked.
*
*  EwInitIssue() ensures, that there is a valid issue attached to the surface.
*  If necessary, new empty issue will be created. If no memory is available for
*  new issues, the function forces the processing of pending drawing operations
*  and flushes the caches.
*
* ARGUMENTS:
*   aSurface - Surface to introduce the preparation of drawing tasks.
*
* RETURN VALUE:
*   If successful, EwInitIssue() returns the desired issue otherwise null is
*   returned.
*
*******************************************************************************/
XIssue* EwInitIssue
(
  XSurface*         aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwDoneIssue
*
* DESCRIPTION:
*  The function EwDoneIssue() completes the preparation of drawing tasks for the
*  specified surface.
*
*  EwDoneIssue() can cause the issue to be flushed if the issue memory has been
*  exhausted by the previously prepared tasks.
*
* ARGUMENTS:
*   aSurface - Surface to complete the issue preparation.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDoneIssue
(
  XSurface*         aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwAllocTask
*
* DESCRIPTION:
*  The function EwAllocTask() has the job to reserve memory for a new drawing
*  task and to incorporate it into the list of pending tasks of the specified
*  issue. The memory reservation will take place on the upper heap area of the
*  issue.
*
*  If the parameter aCopyOf refers to an other task, the function initializes
*  the new task with a copy of it and enqueues the task immediately after the
*  original task. If there is no task to make a copy of it, the new task will
*  remain uninitialized at the end of the list with pending tasks.
*
*  If there is no memory to attend the task allocation, the function fails and
*  returns null. 
*
* ARGUMENTS:
*   aIssue  - Issue to attend the memory allocation.
*   aCopyOf - Optional task to copy into the new task.
*
* RETURN VALUE:
*   If succesful, EwAllocTask() returns a pointer to the new task otherwise
*   null is returned.
*
*******************************************************************************/
XTask* EwAllocTask
(
  XIssue*           aIssue,
  XTask*            aCopyOf
);


/*******************************************************************************
* FUNCTION:
*   EwAllocTasks
*
* DESCRIPTION:
*  The function EwAllocTasks() has the job to reserve memory for a set of new
*  drawing tasks and to incorporate them into the list of pending tasks of the
*  specified issue. The memory reservation will take place on the upper heap
*  area of the issue.
*
*  If the parameter aCopyOf refers to an other task, the function initializes
*  the new tasks with a copy of it and enqueues the tasks immediately after the
*  original task. If there is no task to make a copy of it, the new tasks will
*  remain uninitialized at the end of the list with pending tasks.
*
*  If there is no memory to attend the task allocation, the function fails and
*  returns null. 
*
* ARGUMENTS:
*   aIssue     - Issue to attend the memory allocation.
*   aNoOfTasks - Number to tasks to allocate.
*   aCopyOf    - Optional task to copy into the new task.
*
* RETURN VALUE:
*   If succesful, EwAllocTask() returns a pointer to the first task otherwise
*   null is returned.
*
*******************************************************************************/
XTask* EwAllocTasks
(
  XIssue*           aIssue,
  int               aNoOfTasks,
  XTask*            aCopyOf
);


/*******************************************************************************
* FUNCTION:
*   EwAllocTaskData
*
* DESCRIPTION:
*  The function EwAllocTaskData() has the job to reserve a memory area for the
*  parameters of a drawing task. The memory reservation will take place on the
*  lower heap area of the specified issue.
*
*  If there is no memory available to attend the allocation, the function fails
*  and returns null.
*
* ARGUMENTS:
*   aIssue - Issue to attend the memory allocation.
*   aSize  - Size of the memory to reserve.
*
* RETURN VALUE:
*   If succesful, EwAllocTaskData() returns a pointer to the reserved memory
*   area otherwise null is returned.
*
*******************************************************************************/
void* EwAllocTaskData
(
  XIssue*           aIssue,
  int               aSize
);


/*******************************************************************************
* FUNCTION:
*   EwFreeTaskData
*
* DESCRIPTION:
*  The function EwFreeTaskData() has the job to shrink the memory area reserved
*  by the preceding invocation of the EwAllocTaskData() call.
*
* ARGUMENTS:
*   aIssue - Issue to attend the memory allocation.
*   aPtr   - Address specifying the new end of actually occupied memory area.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeTaskData
(
  XIssue*           aIssue,
  void*             aPtr
);


/*******************************************************************************
* FUNCTION:
*   EwFlushTasks
*
* DESCRIPTION:
*  The function EwFlushTasks() has the job to complete and flush the currently
*  existing issue of the specified surface. If there is no open issue, nothing
*  is done and the function returns to the caller.
*
*  If 0 is passed for aSurface, the next available issue is flushed.
*
* ARGUMENTS:
*   aSurface - Surface to flush the pending tasks.
*
* RETURN VALUE:
*   Returns != 0, if an issue could be flushed.
*
*******************************************************************************/
int EwFlushTasks
(
  XSurface*         aSurface
);


/*******************************************************************************
* FUNCTION:
*   EwWaitForSurface
*
* DESCRIPTION:
*  The function EwWaitForSurface() ensures, that the surface is not involved in
*  any pending or running drawing operations. If necessary the function flushes
*  issues and waits until they are terminated. The last argument describes the
*  impact of the wait operation.
*
* ARGUMENTS:
*   aSurface     - Surface to flush and finish its pending drawing operations.
*   aUntilUnused - If != 0, the function also waits until other operations are
*     done, which involve the surface as a source. If aUntilUnused == 0, only
*     the drawing operations affecting the surface as a destination are flushed
*     and finished.
*
* RETURN VALUE:
*   Returns != 0, if issues could be flushed.
*
*******************************************************************************/
int EwWaitForSurface
(
  XSurface*         aSurface,
  int               aUntilUnused
);


#ifdef __cplusplus
  }
#endif

#endif /* EWGFXCORE_H */

/* pba */
