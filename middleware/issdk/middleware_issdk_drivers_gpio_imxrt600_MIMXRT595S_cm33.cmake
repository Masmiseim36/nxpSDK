if(NOT MIDDLEWARE_ISSDK_DRIVERS_GPIO_IMXRT600_MIMXRT595S_cm33_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_DRIVERS_GPIO_IMXRT600_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "middleware_issdk_drivers_gpio_imxrt600 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx/i.mxrt600/gpio_driver.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx/i.mxrt600
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio
    )


endif()
