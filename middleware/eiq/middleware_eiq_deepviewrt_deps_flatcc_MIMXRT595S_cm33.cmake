include_guard()
message("middleware_eiq_deepviewrt_deps_flatcc component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/flatcc/include
)

