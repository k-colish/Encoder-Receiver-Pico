if(TARGET tinyusb_device)
    add_executable(main
        main.c)

    target_link_libraries(main pico_stdlib hardware_spi)

    pico_add_extra_outputs(main)

    pico_enable_stdio_usb(main 1)
    pico_enable_stdio_uart(main 1)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building")
endif()