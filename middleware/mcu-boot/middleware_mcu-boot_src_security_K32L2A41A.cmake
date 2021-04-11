if(NOT MIDDLEWARE_MCU-BOOT_SRC_SECURITY_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_SRC_SECURITY_K32L2A41A_INCLUDED true CACHE BOOL "middleware_mcu-boot_src_security component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/security/src/aes_security.c
        ${CMAKE_CURRENT_LIST_DIR}/src/security/src/aes128_key_wrap_unwrap.c
        ${CMAKE_CURRENT_LIST_DIR}/src/security/src/cbc_mac.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/security
    )


endif()
