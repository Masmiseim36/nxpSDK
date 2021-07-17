include_guard(GLOBAL)
message("middleware_dhara_dhara_ecc_hamming component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ecc/hamming.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ecc
)


include(middleware_dhara_MIMXRT1176_cm7)

