include_guard()
message("middleware_eiq_deepviewrt_deps_json component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/flex.c
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/reader.c
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/safe.c
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/writer.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/include
)


