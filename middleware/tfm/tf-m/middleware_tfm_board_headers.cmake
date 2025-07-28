# Add set(CONFIG_USE_middleware_tfm_board_headers true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_BOARD STREQUAL kw47evk)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/kw47evk
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/kw47evk/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/kw47evk/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL frdmmcxw72)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxw72
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxw72/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxw72/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL rdrw612bga)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rdrw612bga
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rdrw612bga/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rdrw612bga/Device/Include
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rw61x/platform/include
        )
    endif()

        if(CONFIG_BOARD STREQUAL frdmrw612)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmrw612
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmrw612/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmrw612/Device/Include
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/rw61x/platform/include
        )
    endif()

        if(CONFIG_BOARD STREQUAL mcxw72evk)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxw72evk
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxw72evk/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxw72evk/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL frdmmcxn236)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxn236
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxn236/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxn236/Device/Include
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

        if(CONFIG_BOARD STREQUAL mcxw71evk)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxw71evk
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxw71evk/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxw71evk/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL frdmmcxw71)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxw71
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxw71/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/frdmmcxw71/Device/Include
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

        if(CONFIG_BOARD STREQUAL mcxn5xxevk)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn5xxevk
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn5xxevk/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mcxn5xxevk/Device/Include
        )
    endif()

        if(CONFIG_BOARD STREQUAL kw45b41zevk)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/kw45b41zevk
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/kw45b41zevk/partition
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/kw45b41zevk/Device/Include
        )
    endif()

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DTARGET_CONFIG_HEADER_FILE="config_tfm_target.h"
              )
  
  
  endif()

