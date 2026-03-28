add_library(hagl_hal INTERFACE)

if(HAGL_HAL_USE_TRIPLE_BUFFER)
    target_sources(hagl_hal INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/hagl_hal_buffered.c
    )
    target_compile_definitions(hagl_hal INTERFACE HAGL_HAL_BUFFER_COUNT=3)
elseif(HAGL_HAL_USE_DOUBLE_BUFFER)
    target_sources(hagl_hal INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/hagl_hal_buffered.c
    )
    target_compile_definitions(hagl_hal INTERFACE HAGL_HAL_BUFFER_COUNT=2)
else()
    target_sources(hagl_hal INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/hagl_hal_single.c
    )
    target_compile_definitions(hagl_hal INTERFACE HAGL_HAL_BUFFER_COUNT=1)
endif()

target_sources(hagl_hal INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/mipi_display.c
    ${CMAKE_CURRENT_LIST_DIR}/times.c
)

target_include_directories(hagl_hal INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(hagl_hal INTERFACE pico_stdlib hardware_spi hardware_gpio hardware_dma hagl)