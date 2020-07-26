
PROJECT(NxpRdLib_ceT4T)

FILE(GLOB NxpRdLib_ceT4T_Sources
    ./src/phceT4T.c
    ./src/Sw/phceT4T_Sw.c
    ./src/Sw/phceT4T_Sw.h
    ./src/Sw/phceT4T_Sw_Int.c
    ./src/Sw/phceT4T_Sw_Int.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palI14443p4mC
)
