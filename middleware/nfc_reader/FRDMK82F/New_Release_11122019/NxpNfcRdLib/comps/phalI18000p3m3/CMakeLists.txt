

PROJECT(NxpRdLib_alI18000p3m3)

FILE(GLOB NxpRdLib_alI18000p3m3_Sources
    ./src/phalI18000p3m3.c
    ./src/Sw/phalI18000p3m3_Sw.c
    ./src/Sw/phalI18000p3m3_Sw.h
    ./src/Sw/phalI18000p3m3_Sw_Int.c
    ./src/Sw/phalI18000p3m3_Sw_Int.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palI18000p3m3
)
