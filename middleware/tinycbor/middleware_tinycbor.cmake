# Add set(CONFIG_USE_middleware_tinycbor true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/cborencoder.c
  ${CMAKE_CURRENT_LIST_DIR}/src/cborencoder_close_container_checked.c
  ${CMAKE_CURRENT_LIST_DIR}/src/cborerrorstrings.c
  ${CMAKE_CURRENT_LIST_DIR}/src/cborparser.c
  ${CMAKE_CURRENT_LIST_DIR}/src/cborparser_dup_string.c
  ${CMAKE_CURRENT_LIST_DIR}/src/cborpretty.c
  ${CMAKE_CURRENT_LIST_DIR}/src/cborvalidation.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src
)

