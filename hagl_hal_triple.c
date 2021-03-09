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

This is the HAL used when triple buffering is enabled. The GRAM of the
display driver chip is the framebuffer. The two memory blocks allocated
by this HAL are the two back buffers. Total three buffers.

Note that all coordinates are already clipped in the main library itself.
HAL does not need to validate the coordinates, they can alway be assumed
valid.


*/

#include "hagl_hal.h"

#ifdef HAGL_HAL_USE_TRIPLE_BUFFER

#include <string.h>
#include <mipi_display.h>
#include <mipi_dcs.h>

#include <bitmap.h>
#include <hagl.h>

#include <stdio.h>
#include <stdlib.h>

static uint8_t buffer1[BITMAP_SIZE(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_DEPTH)];
static uint8_t buffer2[BITMAP_SIZE(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_DEPTH)];

static bitmap_t bb = {
    .width = DISPLAY_WIDTH,
    .height = DISPLAY_HEIGHT,
    .depth = DISPLAY_DEPTH,
};

bitmap_t *hagl_hal_init(void)
{
    mipi_display_init();
    bitmap_init(&bb, buffer2);
    bitmap_init(&bb, buffer1);

    hagl_hal_debug("Back buffer 1 address is %p\n", (void *) buffer1);
    hagl_hal_debug("Back buffer 2 address is %p\n", (void *) buffer2);

    return &bb;
}

size_t hagl_hal_flush()
{
    uint8_t *buffer = bb.buffer;
    if (bb.buffer == buffer1) {
        bb.buffer = buffer2;
    } else {
        bb.buffer = buffer1;
    }
    /* Flush the current back buffer. */
    return mipi_display_write(0, 0, bb.width, bb.height, (uint8_t *) buffer);
}

void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color)
{
    color_t *ptr = (color_t *) (bb.buffer + bb.pitch * y0 + (bb.depth / 8) * x0);
    *ptr = color;
}

color_t hagl_hal_get_pixel(int16_t x0, int16_t y0)
{
    return *(color_t *) (bb.buffer + bb.pitch * y0 + (bb.depth / 8) * x0);
}

void hagl_hal_blit(uint16_t x0, uint16_t y0, bitmap_t *src)
{
    bitmap_blit(x0, y0, src, &bb);
}

void hagl_hal_scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *src)
{
    bitmap_scale_blit(x0, y0, w, h, src, &bb);
}

void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t width, color_t color)
{
    color_t *ptr = (color_t *) (bb.buffer + bb.pitch * y0 + (bb.depth / 8) * x0);
    for (uint16_t x = 0; x < width; x++) {
        *ptr++ = color;
    }
}

void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t height, color_t color)
{
    color_t *ptr = (color_t *) (bb.buffer + bb.pitch * y0 + (bb.depth / 8) * x0);
    for (uint16_t y = 0; y < height; y++) {
        *ptr = color;
        ptr += bb.pitch / (bb.depth / 8);
    }
}

#endif /* HAGL_HAL_USE_TRIPLE_BUFFER */
