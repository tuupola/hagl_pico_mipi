/*

MIT License

Copyright (c) 2019-2023 Mika Tuupola

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

This is the HAL used when buffering is disabled. I call this single buffered
since I consider the GRAM of the display driver chip to be the framebuffer.

Note that all coordinates are already clipped in the main library itself.
HAL does not need to validate the coordinates, they can alway be assumed
valid.

*/

#include "hagl_hal.h"

#ifdef HAGL_HAL_USE_SINGLE_BUFFER

#include <hagl/bitmap.h>
#include <hagl/backend.h>
#include <hagl.h>
#include <string.h>

#include "mipi_display.h"

static void
put_pixel(void *self, int16_t x0, int16_t y0, hagl_color_t color)
{
    mipi_display_write_xy(x0, y0, (uint8_t *) &color);
}

static void
blit(void *self, int16_t x0, int16_t y0, hagl_bitmap_t *src)
{
    mipi_display_write_xywh(x0, y0, src->width, src->height, (uint8_t *) src->buffer);
}

static void
hline(void *self, int16_t x0, int16_t y0, uint16_t width, hagl_color_t color)
{
    mipi_display_fill_xywh(x0, y0, width, 1, &color);
}

static void
vline(void *self, int16_t x0, int16_t y0, uint16_t height, hagl_color_t color)
{
    mipi_display_fill_xywh(x0, y0, 1, height, &color);
}

void
hagl_hal_init(hagl_backend_t *backend)
{
    mipi_display_init();

    backend->width = MIPI_DISPLAY_WIDTH;
    backend->height = MIPI_DISPLAY_HEIGHT;
    backend->depth = MIPI_DISPLAY_DEPTH;
    backend->put_pixel = put_pixel;
    backend->hline = hline;
    backend->vline = vline;
}

#endif /* HAGL_HAL_USE_SINGLE_BUFFER */
