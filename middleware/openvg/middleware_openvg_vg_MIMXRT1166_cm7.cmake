if(NOT MIDDLEWARE_OPENVG_VG_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_OPENVG_VG_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_openvg_vg component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/inc
    )

endif()
