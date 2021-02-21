if(NOT MIDDLEWARE_DHARA_DHARA_ECC_HAMMING_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_DHARA_DHARA_ECC_HAMMING_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_dhara_dhara_ecc_hamming component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ecc/hamming.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ecc
    )


    include(middleware_dhara_MIMXRT1176_cm7)

endif()
