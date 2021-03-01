# MIPI DCS HAL for HAGL Graphics Library

HAL for HAGL graphics library for display drivers supporting the [MIPI Display Command Set](https://www.mipi.org/specifications/display-command-set). This covers most displays currently used by hobbyists. Tested with ST7789 (Pimoroni Pico Display Pack).

[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square)](LICENSE.md)

## Usage

To use with a Raspberry Pi Pico SDK project you include this HAL and the [HAGL graphics library](https://github.com/tuupola/hagl) itself.

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
  NO_MENUCONFIG=1
  HAGL_HAL_USE_DOUBLE_BUFFER=1
)
```

By default flushing from back buffer to front buffer is a locking operation. You can avoid that by enabling DMA. This is faster but will cause screen tearing unless you handle vertical syncing somehow in the software.

```
target_compile_definitions(firmware PRIVATE
  NO_MENUCONFIG=1
  HAGL_HAL_USE_DOUBLE_BUFFER=1
  HAGL_HAL_USE_DMA=1
)
```

The default config can be found in `hagl_hal.h`. Defaults are ok for Pimoroni Pico Display Pack in vertical mode.

## License

The MIT License (MIT). Please see [LICENSE](LICENSE) for more information.
