if(NOT MIDDLEWARE_EIQ_TENSORFLOW_LITE_THIRD_PARTY_FFT2D_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_EIQ_TENSORFLOW_LITE_THIRD_PARTY_FFT2D_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_eiq_tensorflow_lite_third_party_fft2d component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/fft2d/fftsg.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/fft2d
    )


endif()
