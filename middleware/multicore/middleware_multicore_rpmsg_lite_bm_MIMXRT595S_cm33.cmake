include_guard()
message("middleware_multicore_rpmsg_lite_bm component is included.")

if(CONFIG_USE_middleware_baremetal_MIMXRT595S_cm33)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_bm.c
)
else()
    message(WARNING "please config middleware.baremetal_MIMXRT595S_cm33 first.")
endif()


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/include/environment/bm
)


