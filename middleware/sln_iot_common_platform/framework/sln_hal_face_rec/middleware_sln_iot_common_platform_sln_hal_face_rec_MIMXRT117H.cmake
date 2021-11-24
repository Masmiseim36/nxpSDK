if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_HAL_FACE_REC_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_HAL_FACE_REC_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_sln_hal_face_rec component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/hal_vision_algo_oasis_lite3D.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_input_ble_wuart_qn9090.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_smart_lock_config.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_vision_algo_oasis_lite2D.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_ffi_ui.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_sln_facedb.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_smart_lock_ui.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
        ${CMAKE_CURRENT_LIST_DIR}/inc
    )


endif()
