if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_MSC_DISK_VFS_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_MSC_DISK_VFS_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_msc_disk_vfs component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/disk.c
        ${CMAKE_CURRENT_LIST_DIR}/sln_msc_vfs.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(middleware_sln_iot_common_platform_fica_driver_MIMXRT117H)

endif()
