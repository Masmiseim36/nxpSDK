# Add set(CONFIG_USE_middleware_pngdec true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/adler32.c
  ${CMAKE_CURRENT_LIST_DIR}/src/crc32.c
  ${CMAKE_CURRENT_LIST_DIR}/src/infback.c
  ${CMAKE_CURRENT_LIST_DIR}/src/inffast.c
  ${CMAKE_CURRENT_LIST_DIR}/src/inflate.c
  ${CMAKE_CURRENT_LIST_DIR}/src/inftrees.c
  ${CMAKE_CURRENT_LIST_DIR}/src/PNGdec.c
  ${CMAKE_CURRENT_LIST_DIR}/src/zutil.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src
)

