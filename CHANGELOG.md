# Changelog

All notable changes to this project will be documented in this file, in reverse chronological order by release.

## [0.4.0-dev](https://github.com/tuupola/hagl_pico_mipi/compare/0.3.0...master) - unreleased

### Fixed
- Compiling failed with Pico SDK 1.5.0 ([#33](https://github.com/tuupola/hagl_pico_mipi/pull/33)).
- Removed stack usage from hline and vline functions ([#41](https://github.com/tuupola/hagl_pico_mipi/pull/41)).

### Changed
- Default SPI clock is now 62500000 ([#24](https://github.com/tuupola/hagl_pico_mipi/pull/24)).
- Default config is now for Waveshare RP2040-LCD-0.96 ([#38](https://github.com/tuupola/hagl_pico_mipi/pull/38)).
- SPI data write functions are now inlined to improve performance ([#43](https://github.com/tuupola/hagl_pico_mipi/pull/43)).

### Removed
### Added
- Config file for Pimoroni PicoSystem ([3aced13](https://github.com/tuupola/hagl_pico_mipi/commit/3aced138b409bd26b135f5d18d08f349a39f54fe)).
- Config files for generic displays ([#23](https://github.com/tuupola/hagl_pico_mipi/pull/23)).
- Config file for LilyGO T-Display RP2040 ([#31](https://github.com/tuupola/hagl_pico_mipi/pull/31)).
- Support for `HAGL_HAL_PIXEL_SIZE` setting ([#25](https://github.com/tuupola/hagl_pico_mipi/pull/25)).

    `HAGL_MIPI_DISPLAY_WIDTH` and `HAGL_MIPI_DISPLAY_HEIGHT` are the hardware
    dimensions. `MIPI_DISPLAY_WIDTH` and `MIPI_DISPLAY_HEIGHT` are the backend dimensions. If hardware dimension is 320 and pixel size is 2 the backend dimension will be 160.

    Does not work with DMA at the moment.
- Support for `MIPI_DISPLAY_PIN_POWER` setting ([#32](https://github.com/tuupola/hagl_pico_mipi/pull/32), [#33](https://github.com/tuupola/hagl_pico_mipi/pull/33)). Used for boards such as LilyGO T-Display RP2040 which require powering the logic or panel from a separate pin.
- Support for `MIPI_DISPLAY_PIN_VSYNC` setting ([#45](https://github.com/tuupola/hagl_pico_mipi/pull/45)). Used for boards such as Pimoroni PicoSystem which have the Tearing Effect (TE) pin connected.

## [0.3.0](https://github.com/tuupola/hagl_pico_mipi/compare/0.2.1...0.3.0) - 2022-09-17

### Fixed
### Changed
### Removed
### Added

## [0.2.1](https://github.com/tuupola/hagl_pico_mipi/compare/0.2.0...0.2.1) - 2021-04-30

### Fixed

## 0.2.0 - 2021-03-21

Initial release.
