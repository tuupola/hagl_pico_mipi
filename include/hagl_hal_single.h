/*

MIT License

Copyright (c) 2019-2021 Mika Tuupola

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

This file is part of the Raspberry Pi Pico HAL for the HAGL graphics library:
https://github.com/tuupola/hagl_pico_mipi

SPDX-License-Identifier: MIT

-cut-

This is the HAL used when buffering is disabled. I call this single buffered
since I consider the GRAM of the display driver chip to be the framebuffer.

Note that all coordinates are already clipped in the main library itself.
HAL does not need to validate the coordinates, they can alway be assumed
valid.

*/

#ifndef _HAGL_HAL_SINGLE_H
#define _HAGL_HAL_SINGLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <bitmap.h>

/* Define if header file included directly. */
#ifndef HAGL_HAL_USE_SINGLE_BUFFER
#define HAGL_HAL_USE_SINGLE_BUFFER
#endif /* HAGL_HAL_USE_SINGLE_BUFFER */

#include "hagl_hal.h"

#define HAGL_HAS_HAL_INIT
#define HAGL_HAS_HAL_BLIT
#define HAGL_HAS_HAL_HLINE
#define HAGL_HAS_HAL_VLINE

/**
 * Put a pixel
 *
 * @param x0 X coordinate
 * @param y0 Y coorginate
 * @param color RGB565 color
 */
void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color);

/**
 * Initialize the HAL
 *
 * This HAL returns null since it does not use buffering.
 *
 * @return NULL
 */
bitmap_t *hagl_hal_init(void);

/**
 * Blit given bitmap to the display
 *
 * @param x0 X coordinate
 * @param y0 Y coorginate
 * @param src Pointer to the source bitmap
 */
void hagl_hal_blit(uint16_t x0, uint16_t y0, bitmap_t *src);

/**
 * Draw a horizontal line
 *
 * @param x0 X coordinate
 * @param y0 Y coorginate
 * @param w width of the line
 */
void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t w, color_t color);

/**
 * Draw a vertical line
 *
 * @param x0 X coordinate
 * @param y0 Y coorginate
 * @param h height of the line
 */
void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t h, color_t color);

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_HAL_SINGLE_H */