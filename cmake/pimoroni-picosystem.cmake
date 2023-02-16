#
# Pimoroni PicoSystem (240x240 ST7789)
# https://shop.pimoroni.com/products/picosystem
#
target_compile_definitions(firmware PRIVATE
    MIPI_DISPLAY_PIN_CS=5
    MIPI_DISPLAY_PIN_DC=9
    MIPI_DISPLAY_PIN_RST=4
    MIPI_DISPLAY_PIN_BL=12
    MIPI_DISPLAY_PIN_CLK=6
    MIPI_DISPLAY_PIN_MOSI=7
    MIPI_DISPLAY_PIN_MISO=-1
    MIPI_DISPLAY_PIN_POWER=-1

    MIPI_DISPLAY_SPI_PORT=spi0
    MIPI_DISPLAY_SPI_CLOCK_SPEED_HZ=62500000

    MIPI_DISPLAY_PIXEL_FORMAT=MIPI_DCS_PIXEL_FORMAT_16BIT
    MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_RGB
    MIPI_DISPLAY_WIDTH=240
    MIPI_DISPLAY_HEIGHT=240
    MIPI_DISPLAY_OFFSET_X=0
    MIPI_DISPLAY_OFFSET_Y=0
    MIPI_DISPLAY_INVERT=1
)
