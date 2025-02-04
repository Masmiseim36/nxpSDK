# Add set(CONFIG_USE_middleware_azure_rtos_ux_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/core/usbx_core_source.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_device_classes/usbx_device_classes_source.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_classes/usbx_host_classes_source.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_pictbridge/usbx_pictbridge_source.c
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_network/src/ux_network_driver.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common/inc
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/common/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/crypto_libraries/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/nx_secure/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/nx_secure/ports
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/core/inc
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_device_classes/inc
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_host_classes/inc
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_network/inc
  ${CMAKE_CURRENT_LIST_DIR}/usbx/common/usbx_pictbridge/inc
)

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/ports/cortex_m7/iar/inc
  ${CMAKE_CURRENT_LIST_DIR}/usbx/ports/cortex_m7/iar/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/ports/cortex_m7/gnu/inc
  ${CMAKE_CURRENT_LIST_DIR}/usbx/ports/cortex_m7/gnu/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/ports/cortex_m7/ac6/inc
  ${CMAKE_CURRENT_LIST_DIR}/usbx/ports/cortex_m7/ac6/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/filex/ports/cortex_m7/gnu/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL mdk OR CONFIG_TOOLCHAIN STREQUAL iar) AND (CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33))
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/filex/ports/generic/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/filex/ports/cortex_m33/gnu/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/ports/cortex_m33/iar/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/ports/cortex_m33/gnu/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/ports/cortex_m33/ac6/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL mdk OR CONFIG_TOOLCHAIN STREQUAL iar) AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/usbx/ports/cortex_m33/inc
)
endif()

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DUX_OTG_SUPPORT
    -DTX_INCLUDE_USER_DEFINE_FILE
    -DFX_INCLUDE_USER_DEFINE_FILE
    -DNX_INCLUDE_USER_DEFINE_FILE
    -DUX_INCLUDE_USER_DEFINE_FILE
  )

endif()

