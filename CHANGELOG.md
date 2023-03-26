# Changelog

All notable changes to this project will be documented in this file, in reverse chronological order by release.

## [0.5.0-dev](https://github.com/tuupola/hagl_pico_mipi/compare/0.4.0...master) - unreleased

### Fixed
- Removed stack usage from hline and vline functions ([#41](https://github.com/tuupola/hagl_pico_mipi/pull/41)).

### Changed

- SPI data write functions are now inlined to improve performance ([#43](https://github.com/tuupola/hagl_pico_mipi/pull/43)).
- Rename `mipi_display_write()` to `mipi_display_write_xywh()` ([#51](https://github.com/tuupola/hagl_pico_mipi/pull/51)).
- Rename `mipi_display_fill()` to `mipi_display_fill_xywh()` ([#52](https://github.com/tuupola/hagl_pico_mipi/pull/52)).

### Added


- Support for `MIPI_DISPLAY_PIN_TE` setting ([#45](https://github.com/tuupola/hagl_pico_mipi/pull/45), [#49](https://github.com/tuupola/hagl_pico_mipi/pull/49)). Used for boards such as Pimoroni PicoSystem which have the Tearing Effect (TE) pin connected.

## [0.4.0](https://github.com/tuupola/hagl_pico_mipi/compare/0.3.0...0.4.0) - 2023-03-11

### Added
- Config file for Pimoroni PicoSystem ([3aced13](https://github.com/tuupola/hagl_pico_mipi/commit/3aced138b409bd26b135f5d18d08f349a39f54fe)).

- Config files for generic displays ([#23](https://github.com/tuupola/hagl_pico_mipi/pull/23)).

- Config file for LilyGO T-Display RP2040 ([#31](https://github.com/tuupola/hagl_pico_mipi/pull/31)).

- Support for `HAGL_HAL_PIXEL_SIZE` setting ([#25](https://github.com/tuupola/hagl_pico_mipi/pull/25)).

    `HAGL_MIPI_DISPLAY_WIDTH` and `HAGL_MIPI_DISPLAY_HEIGHT` are the hardware
    dimensions. `MIPI_DISPLAY_WIDTH` and `MIPI_DISPLAY_HEIGHT` are the backend dimensions. If hardware dimension is 320 and pixel size is 2 the backend dimension will be 160.

    Does not work with DMA at the moment.

- Support for `MIPI_DISPLAY_PIN_POWER` setting ([#32](https://github.com/tuupola/hagl_pico_mipi/pull/32), [#33](https://github.com/tuupola/hagl_pico_mipi/pull/33)).

    Used for boards such as LilyGO T-Display RP2040 which require powering the logic or panel from a separate pin.

### Fixed

- Compiling failed with Pico SDK 1.5.0 ([#33](https://github.com/tuupola/hagl_pico_mipi/pull/33)).

### Changed

- Default SPI clock is now 62500000 ([#24](https://github.com/tuupola/hagl_pico_mipi/pull/24)).

- Default config is now for Waveshare RP2040-LCD-0.96 ([#38](https://github.com/tuupola/hagl_pico_mipi/pull/38)).


## [0.3.0](https://github.com/tuupola/hagl_pico_mipi/compare/0.2.1...0.3.0) - 2022-09-17

This version adds support for HAGL 0.6.x.

### Changed

- This HAL is now a backend as required by HAGL 0.6.0 ([#11](https://github.com/tuupola/hagl_pico_mipi/pull/11), [#13](https://github.com/tuupola/hagl_pico_mipi/pull/13), [#15](https://github.com/tuupola/hagl_pico_mipi/pull/15), [#20](https://github.com/tuupola/hagl_pico_mipi/pull/20)).

### Added

- Support to manually allocate the back buffers ([#14](https://github.com/tuupola/hagl_pico_mipi/pull/14)).

- Support for `MIPI_DISPLAY_SPI_PORT` setting ([#16](https://github.com/tuupola/hagl_pico_mipi/pull/16), [#19](https://github.com/tuupola/hagl_pico_mipi/pull/19)).

- Config files for Pimoroni Pico Display pack, Waveshare Pico-LCD-0.96, Waveshare Pico-LCD-1.44, Waveshare Pico-LCD-1.3 and Waveshare RP2040-LCD-0.96  ([#18](https://github.com/tuupola/hagl_pico_mipi/pull/18)).

## [0.2.1](https://github.com/tuupola/hagl_pico_mipi/compare/0.2.0...0.2.1) - 2021-04-30

### Fixed
- Both `fps()` and `aps()` were broken due to missing `_times()` function ([#33](https://github.com/tuupola/hagl_pico_mipi/pull/33)).

## 0.2.0 - 2021-03-21

Initial release.
