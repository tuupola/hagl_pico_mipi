# MIPI DCS HAL for HAGL Graphics Library

HAL for HAGL graphics library for display drivers supporting the [MIPI Display Command Set](https://www.mipi.org/specifications/display-command-set) standard. This covers most displays currently used by hobbyists. Tested with ST7735S, ST7789, ST7789V ST7789VW, ILI9341 and ILI9163. There is ready made [config files](https://github.com/tuupola/hagl_pico_mipi/tree/master/cmake) for popular displays.

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

## Usage

To use with a Raspberry Pi Pico SDK project you include this HAL and the [HAGL graphics library](https://github.com/tuupola/hagl) itself. For example applications see [Old School Demo Effects](https://github.com/tuupola/pico_effects) and [Graphics Speed Tests](https://github.com/tuupola/pico_effects). If in a hurry check a [video instead](https://vimeo.com/510236080).

```
$ mkdir external
$ cd external
$ git submodule add git@github.com:tuupola/hagl_pico_mipi.git hagl_hal
$ git submodule add git@github.com:tuupola/hagl.git
```

Then in your `CMakeLists.txt` include both libraries in the build.

```
add_subdirectory(external/hagl)
add_subdirectory(external/hagl_hal)
target_link_libraries(firmware hagl hagl_hal)

```

### Buffering

By default the HAL uses single buffering. The buffer is the GRAM of the display driver chip. You can enable double buffering with the following.

```
target_compile_definitions(firmware PRIVATE
  HAGL_HAL_USE_DOUBLE_BUFFER
)
```

By default flushing from back buffer to front buffer is a locking operation. You can avoid that by enabling DMA. This is faster but will cause screen tearing unless you handle vertical syncing in the software.

```
target_compile_definitions(firmware PRIVATE
  HAGL_HAL_USE_DOUBLE_BUFFER
  HAGL_HAL_USE_DMA
)
```

Alternatively you can also use triple buffering. This is the fastest and will not have screen tearing with DMA. Downside is that it uses lot of memory.


```
target_compile_definitions(firmware PRIVATE
  HAGL_HAL_USE_TRIPLE_BUFFER
  HAGL_HAL_USE_DMA
)
```

### Pixel size

If you run out of memory you could try using bigger pixel size. For example if you have 240x240 pixel display and you want to try triple buffering you could do the following. In practice it will change your usable resolution to 120x120 pixels.

Note! At the time of writing `HAGL_HAL_PIXEL_SIZE=2` will not work together with the `HAGL_HAL_USE_DMA` setting.

```
target_compile_definitions(firmware PRIVATE
  HAGL_HAL_USE_TRIPLE_BUFFER
  HAGL_HAL_PIXEL_SIZE=2
)
```

### Power and Backlight

Some boards require power and / or backlight pins. Out of these the backlight pin is more usual.

```
target_compile_definitions(firmware PRIVATE
  MIPI_DISPLAY_PIN_BL=20
  MIPI_DISPLAY_PIN_POWER=22
)
```

### Vertical Sync

While quite rare, some devices such as [Pimoroni PicoSystem](https://shop.pimoroni.com/products/picosystem) (awesome device btw) provide vsync signal on a pin. This can be used to reduce tearing when using double or triple buffering.

```
target_compile_definitions(firmware PRIVATE
    MIPI_DISPLAY_PIN_TE=8
)
```

## Configuration

You can override any of the default settings setting in `CMakeLists.txt`. You only need to override a value if default is not ok. Below example shows all default values. Defaults are ok for [Waveshare RP2040-LCD-0.96](https://www.waveshare.com/wiki/RP2040-LCD-0.96) in vertical mode.

```
target_compile_definitions(firmware PRIVATE
    MIPI_DISPLAY_PIN_CS=9
    MIPI_DISPLAY_PIN_DC=8
    MIPI_DISPLAY_PIN_RST=12
    MIPI_DISPLAY_PIN_BL=13
    MIPI_DISPLAY_PIN_CLK=10
    MIPI_DISPLAY_PIN_MOSI=11
    MIPI_DISPLAY_PIN_MISO=-1
    MIPI_DISPLAY_PIN_POWER=-1
    MIPI_DISPLAY_PIN_TE=-1

    MIPI_DISPLAY_SPI_PORT=spi1
    MIPI_DISPLAY_SPI_CLOCK_SPEED_HZ=62500000

    MIPI_DISPLAY_PIXEL_FORMAT=MIPI_DCS_PIXEL_FORMAT_16BIT
    MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_BGR
    MIPI_DISPLAY_WIDTH=80
    MIPI_DISPLAY_HEIGHT=160
    MIPI_DISPLAY_OFFSET_X=26
    MIPI_DISPLAY_OFFSET_Y=1
    MIPI_DISPLAY_INVERT=1
)
```

`MIPI_DISPLAY_ADDRESS_MODE` controls the orientation and the RGB order of the display. The value is a bit field which can consist of the following flags defined in `mipi_dcs.h`.

```
#define MIPI_DCS_ADDRESS_MODE_MIRROR_Y      0x80
#define MIPI_DCS_ADDRESS_MODE_MIRROR_X      0x40
#define MIPI_DCS_ADDRESS_MODE_SWAP_XY       0x20
#define MIPI_DCS_ADDRESS_MODE_BGR           0x08
#define MIPI_DCS_ADDRESS_MODE_RGB           0x00
#define MIPI_DCS_ADDRESS_MODE_FLIP_X        0x02
#define MIPI_DCS_ADDRESS_MODE_FLIP_Y        0x01
```

You should `OR` together the flags you want to use. For example if you have a 135x240 pixel display on vertical mode.

```
target_compile_definitions(firmware PRIVATE
  MIPI_DISPLAY_WIDTH=135
  MIPI_DISPLAY_HEIGHT=240
)
```

You can change it to 240x135 pixel vertical mode by swapping the X and Y coordinates. You will also need to mirror Y axis.

```
target_compile_definitions(firmware PRIVATE
  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_SWAP_XY|MIPI_DCS_ADDRESS_MODE_MIRROR_Y
  MIPI_DISPLAY_WIDTH=240
  MIPI_DISPLAY_HEIGHT=135
)
```

You can `OR` together as many flags as you want. Not all combinations make sense but any display orientation can be achieved with correct combination of the flags. When in doubt just try different combinations.

## Common problems

If red and blue are mixed but green is ok change to BGR mode.

```
target_compile_definitions(firmware PRIVATE
  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_BGR
)
```

If display seems inverted turn of the inversion.

```
target_compile_definitions(firmware PRIVATE
  MIPI_DISPLAY_INVERT=0
)
```

If image is not center on screen adjust X and Y offsets as needed.

```
target_compile_definitions(firmware PRIVATE
  MIPI_DISPLAY_OFFSET_X=25
  MIPI_DISPLAY_OFFSET_Y=30
)
```

## Speed

Below testing was done with Waveshare [RP2040-LCD-0.96](https://www.waveshare.com/wiki/RP2040-LCD-0.96). Buffered refresh rate was set to 30 frames per second. Number represents operations per seconds ie. bigger number is better.

|                               | Single | Double    | Double DMA | Triple DMA |
|-------------------------------|--------|-----------|------------|------------|
| hagl_put_pixel()              | 156119 |    319931 |     364294 |     365326 |
| hagl_draw_line()              |   3068 |     15655 |      17723 |      17794 |
| hagl_draw_vline()             |  54779 |     72712 |      82412 |      82509 |
| hagl_draw_hline()             |  68244 |     72637 |      82305 |      82497 |
| hagl_draw_circle()            |   2345 |     13519 |      15345 |      15371 |
| hagl_fill_circle()            |   1539 |      8571 |       9701 |       9744 |
| hagl_draw_ellipse()           |   1563 |      7565 |       8543 |       8563 |
| hagl_fill_ellipse()           |    668 |      3666 |       4153 |       4167 |
| hagl_draw_triangle()          |   1025 |      5348 |       6045 |       6080 |
| hagl_fill_triangle()          |    533 |       699 |        806 |        806 |
| hagl_draw_rectangle()         |  14131 |     22244 |      25103 |      25244 |
| hagl_fill_rectangle()         |   1503 |      9406 |      10699 |      10712 |
| hagl_draw_rounded_rectangle() |   5778 |     16984 |      19214 |      19288 |
| hagl_fill_rounded_rectangle() |   1361 |      8223 |       9307 |       9347 |
| hagl_draw_polygon()           |    619 |      3235 |       3661 |       3678 |
| hagl_fill_polygon()           |    323 |       421 |        476 |        486 |
| hagl_put_char()               |   5296 |     25170 |      28534 |      28443 |
| hagl_put_text()               |    392 |           |            |            |

## License

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.
