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

This is the backend when double buffering is enabled. The GRAM of the
display driver chip is the framebuffer. The memory allocated by the
backend is the back buffer. Total two buffers.

Note that all coordinates are already clipped in the main library itself.
Backend does not need to validate the coordinates, they can always be
assumed to be valid.

*/

#include "hagl_hal.h"

#ifdef HAGL_HAL_USE_DOUBLE_BUFFER

#include <string.h>
#include <mipi_display.h>
#include <mipi_dcs.h>

#include <hagl/backend.h>
#include <hagl/bitmap.h>
#include <hagl.h>

#include <stdio.h>
#include <stdlib.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

static hagl_bitmap_t bb;
static hagl_window_t dirty = {
    .x0 = DISPLAY_WIDTH - 1,
    .y0 = DISPLAY_HEIGHT - 1,
    .x1 = 0,
    .y1 = 0,
};

static size_t
flush(void *self)
{
    uint8_t *buffer = bb.buffer;
    size_t sent = 0;

    /* Forward to first dirty scanline. */
    buffer += bb.pitch * dirty.y0;

    // hagl_hal_debug("%d,%d %d\n", dirty.y0, dirty.y1, dirty.y1 - dirty.y0 + 1);

#if HAGL_HAL_PIXEL_SIZE==1
    sent =  mipi_display_write(0, dirty.y0, bb.width, dirty.y1 - dirty.y0 + 1, (uint8_t *) buffer);
#endif /* HAGL_HAL_PIXEL_SIZE==1 */

#if HAGL_HAL_PIXEL_SIZE==2
    static color_t line[MIPI_DISPLAY_WIDTH];
    color_t *ptr = (color_t *) buffer;

    for (uint16_t y = dirty.y0; y <= dirty.y1; y++) {
        for (uint16_t x = 0; x < DISPLAY_WIDTH; x++) {
            line[x * 2] = *(ptr);
            line[x * 2 + 1] = *(ptr++);
        }
        sent += mipi_display_write(0, y * 2, MIPI_DISPLAY_WIDTH, 1, (uint8_t *) line);
        sent += mipi_display_write(0, y * 2 + 1, MIPI_DISPLAY_WIDTH, 1, (uint8_t *) line);
    }
#endif /* HAGL_HAL_PIXEL_SIZE==2 */

    // dirty.x0 = DISPLAY_WIDTH - 1;
    dirty.y0 = DISPLAY_HEIGHT - 1;
    // dirty.x1 = 0;
    dirty.y1 = 0;

    return sent;
}

static void
put_pixel(void *self, int16_t x0, int16_t y0, color_t color)
{
    // dirty.x0 = min(dirty.x0, x0);
    dirty.y0 = min(dirty.y0, y0);
    // dirty.x1 = max(dirty.x1, x0);
    dirty.y1 = max(dirty.y1, y0);

    bb.put_pixel(&bb, x0, y0, color);
}

static color_t
get_pixel(void *self, int16_t x0, int16_t y0)
{
    return bb.get_pixel(&bb, x0, y0);
}

static void
blit(void *self, int16_t x0, int16_t y0, hagl_bitmap_t *src)
{
    // dirty.x0 = min(dirty.x0, x0);
    dirty.y0 = min(dirty.y0, y0);
    // dirty.x1 = max(dirty.x1, x0 + src->width - 1);
    dirty.y1 = max(dirty.y1, (y0 + src->height - 1));

    bb.blit(&bb, x0, y0, src);
}

static void
scale_blit(void *self, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, hagl_bitmap_t *src)
{
    // dirty.x0 = min(dirty.x0, x0);
    dirty.y0 = min(dirty.y0, y0);
    // dirty.x1 = max(dirty.x1, x0 + w - 1);
    dirty.y1 = max(dirty.y1, (y0 + h - 1));
    bb.scale_blit(&bb, x0, y0, w, h, src);
}

static void
hline(void *self, int16_t x0, int16_t y0, uint16_t w, color_t color)
{
    // dirty.x0 = min(dirty.x0, x0);
    dirty.y0 = min(dirty.y0, y0);
    // dirty.x1 = max(dirty.x1, x0 + w - 1);
    dirty.y1 = max(dirty.y1, y0);

    bb.hline(&bb, x0, y0, w, color);
}

static void
vline(void *self, int16_t x0, int16_t y0, uint16_t h, color_t color)
{
    // dirty.x0 = min(dirty.x0, x0);
    dirty.y0 = min(dirty.y0, y0);
    // dirty.x1 = max(dirty.x1, x0);
    dirty.y1 = max(dirty.y1, y0 + h - 1);

    bb.vline(&bb, x0, y0, h, color);
}

void
hagl_hal_init(hagl_backend_t *backend)
{
    mipi_display_init();

    if (!backend->buffer) {
        backend->buffer = calloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_DEPTH / 8), sizeof(uint8_t));
        hagl_hal_debug("Allocated back buffer to address %p.\n", (void *) backend->buffer);
    } else {
        hagl_hal_debug("Using provided back buffer at address %p.\n", (void *) backend->buffer);
    }

    memset(&bb, 0, sizeof(hagl_bitmap_t));
    bb.width = DISPLAY_WIDTH;
    bb.height = DISPLAY_HEIGHT;
    bb.depth = DISPLAY_DEPTH;

    bitmap_init(&bb, backend->buffer);

    backend->width = MIPI_DISPLAY_WIDTH;
    backend->height = MIPI_DISPLAY_HEIGHT;
    backend->depth = MIPI_DISPLAY_DEPTH;
    backend->put_pixel = put_pixel;
    backend->get_pixel = get_pixel;
    backend->hline = hline;
    backend->vline = vline;
    backend->blit = blit;
    backend->scale_blit = scale_blit;

    backend->flush = flush;
}

#endif /* HAGL_HAL_USE_DOUBLE_BUFFER */
