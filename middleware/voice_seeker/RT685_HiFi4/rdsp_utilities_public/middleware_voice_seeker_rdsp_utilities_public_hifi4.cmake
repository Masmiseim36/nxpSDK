# Add set(CONFIG_USE_middleware_voice_seeker_rdsp_utilities_public_hifi4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL dsp) AND (CONFIG_CORE_ID STREQUAL hifi4) AND (CONFIG_BOARD STREQUAL evkmimxrt685 OR CONFIG_BOARD STREQUAL mimxrt685audevk))

if(CONFIG_TOOLCHAIN STREQUAL xcc)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/rdsp_memory_utils_public/RdspMemoryUtilsPublic.c
      ${CMAKE_CURRENT_LIST_DIR}/rdsp_memory_utils_public/memcheck.c
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL xcc)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/rdsp_memory_utils_public
)
endif()

else()

message(SEND_ERROR "middleware_voice_seeker_rdsp_utilities_public_hifi4 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
