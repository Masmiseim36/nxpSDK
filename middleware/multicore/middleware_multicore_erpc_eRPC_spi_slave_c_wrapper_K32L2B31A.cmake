if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_SPI_SLAVE_C_WRAPPER_K32L2B31A_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_SPI_SLAVE_C_WRAPPER_K32L2B31A_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_spi_slave_c_wrapper component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_spi_slave.cpp
    )


    include(middleware_multicore_erpc_common_K32L2B31A)

endif()
