/*
 * Copyright 2019-2024 NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *  */

#ifndef HAL_CHGUI_H
#define HAL_CHGUI_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! \addtogroup HAL_TYPES
*  @{
*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Local text buffer size. */
#define GUI_PRINTF_BUF_SIZE 64

/** @} */

/*******************************************************************************
 * API
 ******************************************************************************/

/*! \addtogroup HAL_OPERATIONS
*  @{
*/

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
/** @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* HAL_CHGUI_H */
