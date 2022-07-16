/*
 * Copyright 2019-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef HAL_CHGUI_H
#define HAL_CHGUI_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @addtogroup gprintf
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Local text buffer size. */
#define GUI_PRINTF_BUF_SIZE 64

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Draws text stored in label pointer to LCD buffer.
 *
 * This function copy content of data from label text buffer
 * to the LCD.
 *
 * @param lcd_buf LCD buffer address destination for drawing text
 * @param fcolor: foreground color in rgb565 format
 * @param bcolor: background color in rgb565 format
 * @param width: LCD width
 * @param x drawing position on X axe
 * @param y drawing position on Y axe
 * @param format C string pointed by format
 * @return The return number of written chars to the buffer

 */
void GUI_DrawText(uint16_t *lcd_buf, uint16_t fcolor, uint16_t bcolor, uint32_t width,
                    int x, int y, const char *label);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* HAL_CHGUI_H */
