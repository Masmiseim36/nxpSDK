# Add set(CONFIG_USE_middleware_freertos-kernel_cm33_trustzone_secure true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL mdk) AND CONFIG_CORE STREQUAL cm33 AND (CONFIG_FPU STREQUAL NO_FPU OR CONFIG_FPU STREQUAL SP_FPU))
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure/secure_context.c
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure/secure_context_port.c
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure/secure_heap.c
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure/secure_init.c
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33/secure/secure_context.c
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33/secure/secure_context_port_asm.s
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33/secure/secure_heap.c
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33/secure/secure_init.c
        )
    endif()

  
      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL mdk))
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/secure
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33/non_secure
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33/secure
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33/non_secure
        )
    endif()

  
