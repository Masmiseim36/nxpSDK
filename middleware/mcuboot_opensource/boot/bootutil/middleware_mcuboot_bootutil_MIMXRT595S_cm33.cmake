include_guard()
message("middleware_mcuboot_bootutil component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/boot_record.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootutil_misc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootutil_public.c
    ${CMAKE_CURRENT_LIST_DIR}/src/caps.c
    ${CMAKE_CURRENT_LIST_DIR}/src/encrypted.c
    ${CMAKE_CURRENT_LIST_DIR}/src/fault_injection_hardening.c
    ${CMAKE_CURRENT_LIST_DIR}/src/fault_injection_hardening_delay_rng_mbedtls.c
    ${CMAKE_CURRENT_LIST_DIR}/src/image_ec.c
    ${CMAKE_CURRENT_LIST_DIR}/src/image_ec256.c
    ${CMAKE_CURRENT_LIST_DIR}/src/image_ed25519.c
    ${CMAKE_CURRENT_LIST_DIR}/src/image_rsa.c
    ${CMAKE_CURRENT_LIST_DIR}/src/image_validate.c
    ${CMAKE_CURRENT_LIST_DIR}/src/loader.c
    ${CMAKE_CURRENT_LIST_DIR}/src/swap_misc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/swap_move.c
    ${CMAKE_CURRENT_LIST_DIR}/src/swap_scratch.c
    ${CMAKE_CURRENT_LIST_DIR}/src/tlv.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/include
)


include(middleware_mbedtls_MIMXRT595S_cm33)

