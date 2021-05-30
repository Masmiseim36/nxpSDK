if(NOT DRIVER_PDB_MK22F51212_INCLUDED)
    
    set(DRIVER_PDB_MK22F51212_INCLUDED true CACHE BOOL "driver_pdb component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_pdb.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK22F51212)

endif()
