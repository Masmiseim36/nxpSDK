include_guard(GLOBAL)
message("middleware_wolfssl_rt component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/port/nxp/dcp_port.c
)


include(middleware_wolfssl_MIMXRT1062)

include(driver_dcp_MIMXRT1062)

include(driver_trng_MIMXRT1062)

include(driver_cache_armv7_m7_MIMXRT1062)

