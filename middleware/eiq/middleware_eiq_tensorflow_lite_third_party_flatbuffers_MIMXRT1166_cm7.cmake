if(NOT MIDDLEWARE_EIQ_TENSORFLOW_LITE_THIRD_PARTY_FLATBUFFERS_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_EIQ_TENSORFLOW_LITE_THIRD_PARTY_FLATBUFFERS_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_eiq_tensorflow_lite_third_party_flatbuffers component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/flatbuffers/include
    )

endif()
