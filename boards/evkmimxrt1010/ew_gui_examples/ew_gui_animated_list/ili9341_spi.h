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
 *
 *******************************************************************************/

#ifndef ILI9341_SPI_H
#define ILI9341_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#define ILI9341_ID 0x9341

/*******************************************************************************
 * FUNCTION:
 *   ILI9341_Init
 *
 * DESCRIPTION:
 *   The function initializes the FSMC hardware and all necessary GPIO in order
 *   to reset and intialize the connected display hardware.
 *
 * ARGUMENTS:
 *   None
 *
 * RETURN VALUE:
 *   None
 *
 *******************************************************************************/
void ILI9341_Init(void);

/*******************************************************************************
 * FUNCTION:
 *   ILI9341_ReadID
 *
 * DESCRIPTION:
 *   The function assumes a connected and intialized ILI9341 display and tries
 *   to read its ID.
 *
 * ARGUMENTS:
 *   None
 *
 * RETURN VALUE:
 *   If successful, the function returns the ID of the display - 0 otherwise.
 *
 *******************************************************************************/
uint16_t ILI9341_ReadID(void);

/*******************************************************************************
 * FUNCTION:
 *   ILI9341_WaitForCompletion
 *
 * DESCRIPTION:
 *   The function ensures that the previous SPI data transfer is completed..
 *
 * ARGUMENTS:
 *   None
 *
 * RETURN VALUE:
 *   None
 *
 *******************************************************************************/
void ILI9341_WaitForCompletion(void);

/*******************************************************************************
 * FUNCTION:
 *   ILI9341_WriteDataWindow
 *
 * DESCRIPTION:
 *   The function sets the destination position and size within the framebuffer
 *   of the display according the given rectangle and starts the transfer of
 *   the given data. The function does NOT wait for the completion of the transfer.
 *
 * ARGUMENTS:
 *   aX       - Horizontal position for next write access.
 *   aY       - Vertical position for next write access.
 *   aW       - Width of the data window in pixel.
 *   aH       - Height of the data window in pixel.
 *
 * RETURN VALUE:
 *   None
 *
 *******************************************************************************/
void ILI9341_WriteDataWindow(uint16_t aXPos, uint16_t aYPos, uint16_t aWidth, uint16_t aHeight, uint8_t *aData);

#ifdef __cplusplus
}
#endif

#endif /* ILI9341_SPI_H */

/* msy */
