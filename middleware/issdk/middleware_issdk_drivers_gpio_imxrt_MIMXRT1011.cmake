if(NOT MIDDLEWARE_ISSDK_DRIVERS_GPIO_IMXRT_MIMXRT1011_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_DRIVERS_GPIO_IMXRT_MIMXRT1011_INCLUDED true CACHE BOOL "middleware_issdk_drivers_gpio_imxrt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx/gpio_driver.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio
    )


endif()
