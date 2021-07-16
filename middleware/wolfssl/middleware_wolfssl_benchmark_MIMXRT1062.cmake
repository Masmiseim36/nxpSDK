include_guard(GLOBAL)
message("middleware_wolfssl_benchmark component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/benchmark/benchmark.c
)


include(middleware_wolfssl_MIMXRT1062)

