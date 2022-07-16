include_guard()
message("middleware_eiq_mpp component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_camera_mipi_ov5640.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_display_lcdifv2_rk055ahd091.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_draw.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_evkmimxrt1170.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_freertos.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_graphics_pxp.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_static_image.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_vision_algo_tflite.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/tflite/model.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/tflite/model_all_ops_micro.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mpp/include
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/include
)


include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1176_cm7)

