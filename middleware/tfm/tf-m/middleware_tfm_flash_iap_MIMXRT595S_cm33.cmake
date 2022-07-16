include_guard()
message("middleware_tfm_flash_iap component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap.c
)


include(driver_iap_MIMXRT595S_cm33)

