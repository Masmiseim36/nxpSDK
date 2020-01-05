
PROJECT(test_common)

FILE(GLOB ${PROJECT_NAME}_files
    *.c
    *.h
    ../inc/*.h
)

IF(WithSSS_A71CH OR WithSSS_SE050_EAR)
	FILE(GLOB ${PROJECT_NAME}_ax_files
	    ../../../hostlib/hostLib/tstUtil/app_boot.c
	)
ENDIF()

add_library(${PROJECT_NAME}
    ${${PROJECT_NAME}_files}
    ${${PROJECT_NAME}_ax_files}
)

target_link_libraries(${PROJECT_NAME}
    SSS_APIs
    unity
)

IF(WithSSS_A71CH OR WithSSS_SE050_EAR)
	target_include_directories(${PROJECT_NAME} PRIVATE
	    ../../../hostlib/hostLib/tstUtil
	)
ENDIF()

target_include_directories(${PROJECT_NAME} PUBLIC  ../inc)

