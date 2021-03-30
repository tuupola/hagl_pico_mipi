# MIPI DCS HAL for HAGL Graphics Library

HAL for HAGL graphics library for display drivers supporting the [MIPI Display Command Set](https://www.mipi.org/specifications/display-command-set) standard. This covers most displays currently used by hobbyists. Tested with ST7789 (Pimoroni Pico Display Pack), ST7735S, ILI9341 and ILI9163.

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

The default config can be found in `hagl_hal.h`. Defaults are ok for [Pimoroni Pico Display Pack](https://shop.pimoroni.com/products/pico-display-pack) in vertical mode.

## Configuration

You can override any of the default settings setting in `CMakeLists.txt`. You only need to override a value if default is not ok. Below example shows all the possible overridable values. These are also the default values.

```
target_compile_definitions(firmware PRIVATE
  MIPI_DISPLAY_SPI_CLOCK_SPEED_HZ=64000000
  MIPI_DISPLAY_PIN_CS=17
  MIPI_DISPLAY_PIN_DC=16
  MIPI_DISPLAY_PIN_RST=-1
  MIPI_DISPLAY_PIN_BL=20
  MIPI_DISPLAY_PIN_CLK=18
  MIPI_DISPLAY_PIN_MOSI=19
  MIPI_DISPLAY_PIN_MISO=-1
  MIPI_DISPLAY_PIXEL_FORMAT=MIPI_DCS_PIXEL_FORMAT_16BIT
  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_RGB
  MIPI_DISPLAY_WIDTH=135
  MIPI_DISPLAY_HEIGHT=240
  MIPI_DISPLAY_OFFSET_X=52
  MIPI_DISPLAY_OFFSET_Y=40
  MIPI_DISPLAY_INVERT=0
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

Below testing was done with Pimoroni Pico Display Pack. Double buffering refresh rate was set to 30 frames per second. Number represents operations per seconsd ie. bigger number is better.

|                               | Single | Double    | Double DMA | Triple DMA |
|-------------------------------|--------|-----------|------------|------------|
| hagl_put_pixel()              | 104919 |    229668 |     328069 |     328080 |
| hagl_draw_line()              |   1602 |     10688 |      15311 |      15313 |
| hagl_draw_vline()             |  36079 |     37557 |      53787 |      53877 |
| hagl_draw_hline()             |  36070 |     37622 |      54001 |      53951 |
| hagl_draw_circle()            |   1610 |     14925 |      21412 |      21403 |
| hagl_fill_circle()            |   1267 |      7201 |      10281 |      10320 |
| hagl_draw_ellipse()           |    965 |      8179 |      11696 |      11715 |
| hagl_fill_ellipse()           |    492 |      2790 |       4021 |       2005 |
| hagl_draw_triangle()          |    535 |      3661 |       5230 |       5263 |
| hagl_fill_triangle()          |    346 |       499 |        711 |        716 |
| hagl_draw_rectangle()         |   8989 |     12168 |      17425 |      17420 |
| hagl_fill_rectangle()         |    611 |      3979 |       5752 |       5739 |
| hagl_draw_rounded_rectangle() |   3552 |     10802 |      15417 |      15468 |
| hagl_fill_rounded_rectangle() |    569 |      3664 |       5271 |       5259 |
| hagl_draw_polygon()           |    322 |      2211 |       3170 |       3170 |
| hagl_fill_polygon()           |    178 |       221 |        315 |        319 |
| hagl_put_char()               |  19664 |     19364 |      27753 |      27988 |

## License

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.
