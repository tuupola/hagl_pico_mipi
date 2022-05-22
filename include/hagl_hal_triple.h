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

#ifndef _HAGL_HAL_TRIPLE_H
#define _HAGL_HAL_TRIPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <bitmap.h>
#include <backend.h>

/* Define if header file included directly. */
#ifndef HAGL_HAL_USE_TRIPLE_BUFFER
#define HAGL_HAL_USE_TRIPLE_BUFFER
#endif /* HAGL_HAL_USE_TRIPLE_BUFFER */

#include "hagl_hal.h"

#define HAGL_HAS_HAL_BACK_BUFFER
#define HAGL_HAS_HAL_INIT
// #define HAGL_HAS_HAL_BLIT
// #define HAGL_HAS_HAL_SCALE_BLIT
// #define HAGL_HAS_HAL_HLINE
// #define HAGL_HAS_HAL_VLINE
#define HAGL_HAS_HAL_FLUSH
#define HAGL_HAS_HAL_GET_PIXEL

/**
 * Initialize the backend
 *
 * @return pointer to he backend instance
 */
hagl_backend_t *hagl_hal_init(void);

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_HAL_TRIPLE_H */