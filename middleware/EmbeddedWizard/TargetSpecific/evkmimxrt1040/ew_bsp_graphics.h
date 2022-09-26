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
*   Please note: The implementation of this module is partially based on
*   examples that are provided within the NXP MCUXpresso software. In case you
*   want to adapt this module to your custom specific hardware, please adapt the
*   hardware initialization code according your needs.
*   This file contains an interface to the PXP hardware of the NXP target
*   in order to provide graphics acceleration for the Embedded Wizard generated
*   UI applications.
*   This interface is intended to be used only by the Graphics Engine of
*   Embedded Wizard.
*
*******************************************************************************/

#ifndef EW_BSP_GRAPHICS_H
#define EW_BSP_GRAPHICS_H


#include "fsl_pxp.h"


#ifdef __cplusplus
  extern "C"
  {
#endif


/*
  EW_USE_PXP_INTERRUPT_MODE - Flag to switch on/off the usage of the interrupt
  mode for the PXP (graphics accelerator).
  Per default, the usage of the interrupt mode is enabled. If the interrupt mode
  is swtched off, polling mode is used instead. To switch off the usage of the
  PXP interrupt mode, please set the macro EW_USE_PXP_INTERRUPT_MODE to 0
  within your makefile. This can be achieved by using the compiler flag
  -DEW_USE_PXP_INTERRUPT_MODE=0
*/
#ifndef EW_USE_PXP_INTERRUPT_MODE
  #define EW_USE_PXP_INTERRUPT_MODE 1
#endif

#if EW_USE_PXP_INTERRUPT_MODE == 0
  #undef EW_USE_PXP_INTERRUPT_MODE
#endif


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsInit
*
* DESCRIPTION:
*   Initialize the PXP graphics accelerator.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int EwBspGraphicsInit( void );


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsDone
*
* DESCRIPTION:
*   Deinitialize the PXP graphics accelerator.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsDone( void );


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsWaitForCompletion
*
* DESCRIPTION:
*   The function EwBspGraphicsWaitForCompletion returns as soon as the PXP has
*   completed a pending graphics instruction. If the PXP is in idle mode,
*   the function returns immediately.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsWaitForCompletion( void );


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsConcurrentOperation
*
* DESCRIPTION:
*   The function EwBspGraphicsConcurrentOperation configures the operation mode
*   of PXP and CPU. If concurrent operation is enabled, the CPU will work in
*   parallel while the PXP is transferring data. If concurrent operation is
*   disabled, the CPU will wait everytime the PXP is active.
*   This feature is intended to limit the memory bandwidth, e.g. during display
*   update or other bandwidth consuming activities.
*
* ARGUMENTS:
*   aEnable - flag to switch on/off concurrent operation mode.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsConcurrentOperation
(
  int               aEnable
);


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsFill
*
* DESCRIPTION:
*   The function EwBspGraphicsFill is used by the Graphics Engine, when a
*   rectangular area should be filled with a constant color by using the PXP
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aDstPitch     - Number of bytes per line within the destination bitmap.
*   aWidth,
*   aHeight       - Size of the area to fill.
*   aDstColorMode - Colorformat of the destination.
*   aSrcColor     - Color to fill the rectangular area.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsFill
(
  uint32_t          aDstAddr,
  uint32_t          aDstPitch,
  uint32_t          aWidth,
  uint32_t          aHeight,
  uint32_t          aDstColorMode,
  uint32_t          aSrcColor
);


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsCopy
*
* DESCRIPTION:
*   The function EwBspGraphicsCopy is used by the Graphics Engine, when a source
*   bitmap should be copied into a destination bitmap by using the PXP
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aSrcAddr      - Source address of the transfer.
*   aDstPitch     - Number of bytes per line within the destination bitmap.
*   aSrcPitch     - Number of bytes per line within the source bitmap.
*   aWidth,
*   aHeight       - Size of the area to transfer the bitmap.
*   aDstColorMode - Colorformat of the destination bitmap
*   aSrcColorMode - Colorformat of the source bitmap.
*   aSrcColor     - Color/alpha value to be used for the transfer.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsCopy
(
  uint32_t          aDstAddr,
  uint32_t          aSrcAddr,
  uint32_t          aDstPitch,
  uint32_t          aSrcPitch,
  uint32_t          aWidth,
  uint32_t          aHeight,
  uint32_t          aDstColorMode,
  uint32_t          aSrcColorMode,
  uint32_t          aSrcColor
);


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsCopyBlend
*
* DESCRIPTION:
*   The function EwBspGraphicsCopyBlend is used by the Graphics Engine, when a
*   source bitmap should be blended over a destination bitmap by using the PXP
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aSrcAddr      - Source address of the transfer.
*   aDstPitch     - Number of bytes per line within the destination bitmap.
*   aSrcPitch     - Number of bytes per line within the source bitmap.
*   aWidth,
*   aHeight       - Size of the area to transfer the bitmap.
*   aDstColorMode - Colorformat of the destination bitmap
*   aSrcColorMode - Colorformat of the source bitmap.
*   aSrcColor     - Color/alpha value to be used for the transfer.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsCopyBlend
(
  uint32_t          aDstAddr,
  uint32_t          aSrcAddr,
  uint32_t          aDstPitch,
  uint32_t          aSrcPitch,
  uint32_t          aWidth,
  uint32_t          aHeight,
  uint32_t          aDstColorMode,
  uint32_t          aSrcColorMode,
  uint32_t          aSrcColor
);


#ifdef __cplusplus
  }
#endif

#endif /* EW_BSP_GRAPHICS_H */


/* msy */
