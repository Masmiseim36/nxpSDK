

PROJECT(NxpRdLib_alICode)

FILE(GLOB NxpRdLib_alICode_Sources
    ./src/phalICode.c
    ./src/phalICode_Int.c
    ./src/phalICode_Int.h
    ./src/Sw/phalICode_Sw.c
    ./src/Sw/phalICode_Sw.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palSli15693
)
