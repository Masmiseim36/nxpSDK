include_guard()
message("middleware_eiq_mpp component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_camera_mipi_ov5640.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_display_lcdifv2_rk055ahd091.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_camera_csi_mt9m114.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_display_lcdif_rk043fn.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_draw.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_evkmimxrt1170.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_freertos.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_graphics_pxp.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_static_image.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_vision_algo_tflite.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_vision_algo_glow.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_vision_algo_deep_view_rt.c
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/tflite/model.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/tflite/model_all_ops_micro.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/glow/model.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/deep_view_rt/model.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/mpp/include
    ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/include
)


include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1176_cm7)

include(middleware_eiq_deepviewrt_MIMXRT1176_cm7)

