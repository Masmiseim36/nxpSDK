

PROJECT(NxpRdLib_alFelica)

FILE(GLOB NxpRdLib_alFelica_Sources
    ./src/phalFelica.c
    ./src/phalFelica_Int.h
    ./src/Sw/phalFelica_Sw.c
    ./src/Sw/phalFelica_Sw.h
)


ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palFelica
)
