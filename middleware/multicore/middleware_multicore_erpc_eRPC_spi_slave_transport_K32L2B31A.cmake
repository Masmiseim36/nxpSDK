if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_SPI_SLAVE_TRANSPORT_K32L2B31A_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_SPI_SLAVE_TRANSPORT_K32L2B31A_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_spi_slave_transport component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/transports/erpc_spi_slave_transport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_framed_transport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_mbf_dynamic.cpp
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/transports
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra
    )


    include(middleware_multicore_erpc_common_K32L2B31A)

endif()
