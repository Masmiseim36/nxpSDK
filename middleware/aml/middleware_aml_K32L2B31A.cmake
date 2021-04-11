if(NOT MIDDLEWARE_AML_K32L2B31A_INCLUDED)
    
    set(MIDDLEWARE_AML_K32L2B31A_INCLUDED true CACHE BOOL "middleware_aml component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/spi_aml/spi_aml.c
        ${CMAKE_CURRENT_LIST_DIR}/tmr_aml/ftm_aml.c
        ${CMAKE_CURRENT_LIST_DIR}/tmr_aml/tpm_aml.c
        ${CMAKE_CURRENT_LIST_DIR}/wait_aml/wait_aml.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
        ${CMAKE_CURRENT_LIST_DIR}/spi_aml
        ${CMAKE_CURRENT_LIST_DIR}/tmr_aml
        ${CMAKE_CURRENT_LIST_DIR}/wait_aml
    )


    #OR Logic component
    if(CONFIG_USE_driver_spi_K32L2B31A)
         include(driver_spi_K32L2B31A)
    endif()
    if(CONFIG_USE_driver_tpm_K32L2B31A)
         include(driver_tpm_K32L2B31A)
    endif()
    if(NOT (CONFIG_USE_driver_spi_K32L2B31A OR CONFIG_USE_driver_tpm_K32L2B31A))
        message(WARNING "Since driver_spi_K32L2B31A/driver_tpm_K32L2B31A is not included at first or config in config.cmake file, use driver_spi_K32L2B31A/driver_tpm_K32L2B31A by default.")
        include(driver_spi_K32L2B31A)
        include(driver_tpm_K32L2B31A)
    endif()

    include(driver_common_K32L2B31A)

    include(utility_assert_K32L2B31A)

    include(driver_port_K32L2B31A)

    include(driver_gpio_K32L2B31A)

    include(driver_clock_K32L2B31A)

endif()
