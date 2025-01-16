# Add set(CONFIG_USE_middleware_tfm_ns_board true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_BOARD STREQUAL frdmmcxn947)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxn947/Device/Source/startup_frdmmcxn947.c
        )
    endif()

        if(CONFIG_BOARD STREQUAL lpcxpresso55s69)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s69/Device/Source/startup_lpcxpresso55s69.c
        )
    endif()

        if(CONFIG_BOARD STREQUAL frdmrw612)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmrw612/Device/Source/startup_frdmrw612.c
        )
    endif()

        if(CONFIG_BOARD STREQUAL mcxn9xxevk)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn9xxevk/Device/Source/startup_mcxn9xxevk.c
        )
    endif()

        if(CONFIG_BOARD STREQUAL rdrw612bga)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rdrw612bga/Device/Source/startup_rdrw612bga.c
        )
    endif()

        if(CONFIG_BOARD STREQUAL mcxn5xxevk)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn5xxevk/Device/Source/startup_mcxn5xxevk.c
        )
    endif()

  
      if(CONFIG_BOARD STREQUAL frdmmcxn947)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxn947
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxn947/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxn947/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL lpcxpresso55s69)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s69
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s69/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s69/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL frdmrw612)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmrw612
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmrw612/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmrw612/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL mcxn9xxevk)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn9xxevk
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn9xxevk/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn9xxevk/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL rdrw612bga)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rdrw612bga
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rdrw612bga/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rdrw612bga/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL mcxn5xxevk)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn5xxevk
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn5xxevk/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn5xxevk/Device/Include
        )
    endif()

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DTARGET_CONFIG_HEADER_FILE="config_tfm_target.h"
              )
  
  
  endif()

