include_guard()
message("middleware_tfm_s_platform component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/auto_generated/intermedia_tfm_platform.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/auto_generated/load_info_tfm_platform.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/platform_sp.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/tfm_platform_secure_api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform
)


