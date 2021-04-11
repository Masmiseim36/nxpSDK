if(NOT MIDDLEWARE_ISSDK_DRIVERS_GPIO_KINETIS_K32L3A60_cm4_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_DRIVERS_GPIO_KINETIS_K32L3A60_cm4_INCLUDED true CACHE BOOL "middleware_issdk_drivers_gpio_kinetis component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/kinetis/gpio_driver_irq.c
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/kinetis/gpio_driver.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/kinetis
        ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio
    )


endif()
