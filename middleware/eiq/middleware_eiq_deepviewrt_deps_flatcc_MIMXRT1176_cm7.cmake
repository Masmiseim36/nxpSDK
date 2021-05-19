if(NOT MIDDLEWARE_EIQ_DEEPVIEWRT_DEPS_FLATCC_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_EIQ_DEEPVIEWRT_DEPS_FLATCC_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_eiq_deepviewrt_deps_flatcc component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/flatcc/include
    )

endif()
