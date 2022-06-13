/*

MIT License

Copyright (c) 2019-2022 Mika Tuupola

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

This file is part of the Raspberry Pi Pico MIPI DCS backend for the HAGL
graphics library: https://github.com/tuupola/hagl_pico_mipi

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

static bitmap_t bb = {
    .width = DISPLAY_WIDTH,
    .height = DISPLAY_HEIGHT,
    .depth = DISPLAY_DEPTH,
};

static size_t
flush(void *_backend)
{
    const hagl_backend_t *backend = _backend;

    uint8_t *buffer = bb.buffer;

    /* Flip the buffers. */
    if (bb.buffer == backend->buffer) {
        bb.buffer = backend->buffer2;
    } else {
        bb.buffer = backend->buffer;
    }
    /* Flush the current back buffer. */
    return mipi_display_write(0, 0, bb.width, bb.height, (uint8_t *) buffer);
}

static void
put_pixel(int16_t x0, int16_t y0, color_t color)
{
    bitmap_put_pixel(&bb, x0, y0, color);
}

static color_t
get_pixel(int16_t x0, int16_t y0)
{
    return bitmap_get_pixel(&bb, x0, y0);
}

static void
blit(uint16_t x0, uint16_t y0, bitmap_t *src)
{
    bitmap_blit(x0, y0, src, &bb);
}

static void
scale_blit(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, bitmap_t *src)
{
    bitmap_scale_blit(x0, y0, w, h, src, &bb);
}

static void
hline(int16_t x0, int16_t y0, uint16_t width, color_t color)
{
    bitmap_hline(&bb, x0, y0, width, color);
}

static void
vline(int16_t x0, int16_t y0, uint16_t height, color_t color)
{
    bitmap_vline(&bb, x0, y0, height, color);
}

void
hagl_hal_init(hagl_backend_t *backend)
{
    mipi_display_init();

    if (!backend->buffer) {
        backend->buffer = calloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_DEPTH / 8), sizeof(uint8_t));
        hagl_hal_debug("Allocated first back buffer to address %p.\n", (void *) backend->buffer);
    } else {
        hagl_hal_debug("Using provided first back buffer at address %p.\n", (void *) backend->buffer);
    }

    if (!backend->buffer2) {
        backend->buffer2 = calloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_DEPTH / 8), sizeof(uint8_t));
        hagl_hal_debug("Allocated second back buffer to address %p.\n", (void *) backend->buffer2);
    } else {
        hagl_hal_debug("Using provided second back buffer at address %p.\n", (void *) backend->buffer2);
    }

    /* Initially use the first buffer. */
    bitmap_init(&bb, backend->buffer);

    backend->width = MIPI_DISPLAY_WIDTH;
    backend->height = MIPI_DISPLAY_HEIGHT;
    backend->depth = MIPI_DISPLAY_DEPTH;
    backend->put_pixel = put_pixel;
    backend->hline = hline;
    backend->vline = vline;

    backend->flush = flush;
}

#endif /* HAGL_HAL_USE_TRIPLE_BUFFER */
