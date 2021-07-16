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
*   This file is part of the interface (glue layer) between an Embedded Wizard
*   generated UI application and the board support package (BSP) of a dedicated
*   target.
*   This template is responsible to initialize the display hardware of the board
*   and to provide the necessary access to update the display content.
*
*******************************************************************************/

#ifndef EW_BSP_DISPLAY_H
#define EW_BSP_DISPLAY_H


#ifdef __cplusplus
  extern "C"
  {
#endif


/* flag to indicate normal display update with full access to frame buffer */
#define EW_BSP_DISPLAY_UPDATE_NORMAL         0x00000000

/* flag to indicate partial frame buffer update in case of a synchroneous single
   buffer - update is divided in stripes (fields) defined by the display driver */
#define EW_BSP_DISPLAY_UPDATE_PARTIAL        0x00000001

/* flag to indicate display update by using a scratch-pad buffer - update is done
   in subareas that fit into the scratch-pad buffer */
#define EW_BSP_DISPLAY_UPDATE_SCRATCHPAD     0x00000002


/******************************************************************************
* TYPE:
*   XDisplayInfo
*
* DESCRIPTION:
*   The structure XDisplayInfo describes the attributes and current configuration
*   of the display. The interpretation and usage of the members may depend on the
*   underlying system and the selected framebuffer integration scenario.
*
* ELEMENTS:
*   FrameBuffer    - Pointer to the framebuffer memory. In case of double-buffering
*     it refers to the first framebuffer (not the currently active front-buffer).
*     If the display is updated from a scrach-pad buffer, the pointer refers to
*     the scratch-pad buffer memory.
*   DoubleBuffer   - Pointer to the second framebuffer or scratch-pad buffer in
*     case of double-buffering.
*   BufferWidth    - Width of the framebuffer(s) / scratch-pad buffer(s) in pixel.
*   BufferHeight   - Height of the framebuffer(s) / scratch-pad buffer(s) in pixel.
*   DisplayWidth   - Width of the display in pixel.
*   DisplayHeight  - Height of the display in pixel.
*   UpdateMode     - The display update mode (normal, partial, scratch-pad).
*   Context        - Optional pointer to a target specific struct.
*
******************************************************************************/
typedef struct
{
  void* FrameBuffer;
  void* DoubleBuffer;
  int   BufferWidth;
  int   BufferHeight;
  int   DisplayWidth;
  int   DisplayHeight;
  int   UpdateMode;
  void* Context;
} XDisplayInfo;


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayInit
*
* DESCRIPTION:
*   The function EwBspDisplayInit initializes the display hardware and returns
*   the display parameter.
*
* ARGUMENTS:
*   aDisplayInfo - Display info data structure.
*
* RETURN VALUE:
*   Returns 1 if successful, 0 otherwise.
*
*******************************************************************************/
int EwBspDisplayInit
(
  XDisplayInfo*               aDisplayInfo
);


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayDone
*
* DESCRIPTION:
*   The function EwBspDisplayDone deinitializes the display hardware.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayDone
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayGetUpdateArea
*
* DESCRIPTION:
*   The function EwBspDisplayGetUpdateArea returns the next update area
*   depending on the selected display mode:
*   In case of a synchroneous single-buffer, the function has to return the
*   the rectangular areas that correspond to the horizontal stripes (fields)
*   of the framebuffer.
*   In case of a scratch-pad buffer, the function has to return the subareas
*   that fit into the provided update rectangle.
*   During each display update, this function is called until it returns 0.
*
* ARGUMENTS:
*   aUpdateRect - Rectangular area which should be updated (redrawn).
*
* RETURN VALUE:
*   Returns 1 if a further update area can be provided, 0 otherwise.
*
*******************************************************************************/
int EwBspDisplayGetUpdateArea
(
  XRect* aUpdateRect
);


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayWaitForCompletion
*
* DESCRIPTION:
*   The function EwBspDisplayWaitForCompletion is called from the Graphics Engine
*   to ensure that all pending activities of the display system are completed, so
*   that the rendering of the next frame can start.
*   In case of a double-buffering system, the function has to wait until the
*   V-sync has occured and the pending buffer is used by the display controller.
*   In case of an external display controller, the function has to wait until
*   the transfer (update) of the graphics data has been completed and there are
*   no pending buffers.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayWaitForCompletion
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayCommitBuffer
*
* DESCRIPTION:
*   The function EwBspDisplayCommitBuffer is called from the Graphics Engine
*   when the rendering of a certain buffer has been completed.
*   The type of buffer depends on the selected framebuffer concept.
*   If the display is running in a double-buffering mode, the function is called
*   after each buffer update in order to change the currently active framebuffer
*   address. Changing the framebuffer address should be synchronized with V-sync.
*   If the system is using an external graphics controller, this function is
*   responsible to start the transfer of the framebuffer content.
*
* ARGUMENTS:
*   aAddress - Address of the framebuffer to be shown on the display.
*   aX,
*   aY       - Origin of the area which has been updated by the Graphics Engine.
*   aWidth,
*   aHeight  - Size of the area which has been updated by the Graphics Engine.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayCommitBuffer
(
  void*                       aAddress,
  int                         aX,
  int                         aY,
  int                         aWidth,
  int                         aHeight
);


/*******************************************************************************
* FUNCTION:
*   EwBspDisplaySetClut
*
* DESCRIPTION:
*   The function EwBspDisplaySetClut is called from the Graphics Engine
*   in order to update the hardware CLUT of the current framebuffer.
*   The function is only called when the color format of the framebuffer is
*   Index8 or LumA44.
*
* ARGUMENTS:
*   aClut - Pointer to a color lookup table with 256 entries.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplaySetClut
(
  unsigned long*              aClut
);


#ifdef __cplusplus
  }
#endif

#endif /* EW_BSP_DISPLAY_H */


/* msy */
