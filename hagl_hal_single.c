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

#include "hagl_hal.h"

#ifdef HAGL_HAL_USE_SINGLE_BUFFER

#include <bitmap.h>
#include <hagl.h>

#include "mipi_display.h"

bitmap_t *hagl_hal_init(void)
{
    mipi_display_init();
    return NULL;
}

void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color)
{
    mipi_display_write(x0, y0, 1, 1, (uint8_t *) &color);
}

void hagl_hal_blit(uint16_t x0, uint16_t y0, bitmap_t *src)
{
    mipi_display_write(x0, y0, src->width, src->height, (uint8_t *) src->buffer);
}

void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t width, color_t color)
{
    static color_t line[DISPLAY_WIDTH];
    const uint16_t height = 1;

    for (uint16_t x = 0; x < width; x++) {
        line[x] = color;

    }

    mipi_display_write(x0, y0, width, height, (uint8_t *) line);
}

void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t height, color_t color)
{
    static color_t line[DISPLAY_HEIGHT];
    const uint16_t width = 1;

    for (uint16_t y = 0; y < height; y++) {
        line[y] = color;
    }

    mipi_display_write(x0, y0, width, height, (uint8_t *) line);
}

#endif /* HAGL_HAL_USE_SINGLE_BUFFER */
