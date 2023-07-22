include_guard()
message("middleware_voice_seeker_rdsp_utilities_public component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/rdsp_memory_utils_public/RdspMemoryUtilsPublic.c
    ${CMAKE_CURRENT_LIST_DIR}/rdsp_memory_utils_public/memcheck.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/rdsp_memory_utils_public
)


