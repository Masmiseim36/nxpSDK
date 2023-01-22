include_guard()
message("middleware_eiq_deepviewrt_deps_stb component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/stb_image.c
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/stb_image_resize.c
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/stb_image_write.c
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/stb_sprintf.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/stb/include
)


