if(NOT MIDDLEWARE_MULTICORE_RPMSG_LITE_IMXRT1160_BM_MIMXRT1166_cm4_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_RPMSG_LITE_IMXRT1160_BM_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "middleware_multicore_rpmsg_lite_imxrt1160_bm component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/porting/platform/imxrt1160/rpmsg_platform.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/include/platform/imxrt1160
    )


    include(middleware_multicore_rpmsg_lite_MIMXRT1166_cm4)

endif()
