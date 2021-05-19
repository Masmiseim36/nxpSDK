if(NOT MIDDLEWARE_EIQ_DEEPVIEWRT_DEPS_STB_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_EIQ_DEEPVIEWRT_DEPS_STB_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_eiq_deepviewrt_deps_stb component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/stb_image_resize.c
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/stb_image_write.c
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/stb_sprintf.c
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/stb_image.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/include
    )


endif()
