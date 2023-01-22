include_guard()
message("middleware_voice_seeker_cm7 component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ARM_CortexM7/include
)

include(middleware_voice_seeker_rdsp_utilities_public_MIMXRT1176_cm7)

