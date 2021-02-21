if(NOT MIDDLEWARE_EIQ_WORKER_VIDEO_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_EIQ_WORKER_VIDEO_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_eiq_worker_video component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/common/video/camera/RM68191_RM68200/eiq_camera_conf.c
        ${CMAKE_CURRENT_LIST_DIR}/common/video/display/RK055AHD091_RK055IQH091/eiq_display_conf.c
        ${CMAKE_CURRENT_LIST_DIR}/common/video/eiq_display.c
        ${CMAKE_CURRENT_LIST_DIR}/common/video/eiq_video_worker.c
        ${CMAKE_CURRENT_LIST_DIR}/common/video/eiq_pxp.c
        ${CMAKE_CURRENT_LIST_DIR}/common/video/eiq_camera.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/common/video
    )


    include(middleware_eiq_worker_MIMXRT1176_cm7)

endif()
