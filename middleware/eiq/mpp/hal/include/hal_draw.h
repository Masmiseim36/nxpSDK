/*
 * Copyright 2019-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef HAL_DRAW_H
#define HAL_DRAW_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Local text buffer size. */
#define GUI_PRINTF_BUF_SIZE 64

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Draws pixel with RGB565 color to defined point.
 *
 * @param pDst image data address of destination buffer
 * @param x drawing position on X axe
 * @param y drawing position on Y axe
 * @param color RGB565 encoded value
 * @param lcd_w lcd width
 */
void hal_draw_pixel(uint16_t *pDst, uint32_t x, uint32_t y, uint16_t color, uint32_t lcd_w);

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
void hal_draw_text(uint16_t *lcd_buf, uint16_t fcolor, uint16_t bcolor, uint32_t width,
                   int x, int y, const char *label);

/*!
 * @brief Draws rectangle
 *
 * @param[in] lcd_buf LCD buffer address destination for drawing rectangle
 * @param[in] color: background color in rgb565 format
 * @param[in] x drawing position on X axe
 * @param[in] y drawing position on Y axe
 * @param[in] xsize rectangle width
 * @param[in] ysize rectangle height
 * @param[in] r 0-255 red color value
 * @param[in] g 0-255 green color value
 * @param[in] b 0-255 blue color value
 * @param[in] width: LCD width
 * @return N/A

 */
void hal_draw_rect(uint16_t *lcd_buf, uint32_t x, uint32_t y, uint32_t xsize, uint32_t ysize,
                  uint32_t r, uint32_t g, uint32_t b, uint32_t width);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* HAL_DRAW_H */
