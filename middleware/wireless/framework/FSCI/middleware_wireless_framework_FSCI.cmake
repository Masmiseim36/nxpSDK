# Add set(CONFIG_USE_middleware_wireless_framework_FSCI true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Source/FsciCommands.c
  ${CMAKE_CURRENT_LIST_DIR}/Source/FsciCommunication.c
  ${CMAKE_CURRENT_LIST_DIR}/Source/FsciLogging.c
  ${CMAKE_CURRENT_LIST_DIR}/Source/FsciMain.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Interface
  ${CMAKE_CURRENT_LIST_DIR}/Source
)

