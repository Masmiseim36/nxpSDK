if(NOT MIDDLEWARE_EIQ_DEEPVIEWRT_DEPS_JSON_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_EIQ_DEEPVIEWRT_DEPS_JSON_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_eiq_deepviewrt_deps_json component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/flex.c
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/writer.c
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/safe.c
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/reader.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/include
    )


endif()
