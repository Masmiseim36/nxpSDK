include_guard(GLOBAL)
message("middleware_tfm_memory_utils component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
)

