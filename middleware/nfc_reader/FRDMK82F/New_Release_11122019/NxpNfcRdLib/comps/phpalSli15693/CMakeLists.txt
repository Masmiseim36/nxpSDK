

PROJECT(NxpRdLib_palSli15693)

FILE(GLOB NxpRdLib_palSli15693_Sources
    ./src/phpalSli15693.c
    ./src/DUT/phpalSli15693_DUT.c
    ./src/DUT/phpalSli15693_DUT.h
    ./src/STUB/phpalSli15693_STUB.c
    ./src/STUB/phpalSli15693_STUB.h
    ./src/Sw/phpalSli15693_Sw.c
    ./src/Sw/phpalSli15693_Sw.h
    ./src/Sw/phpalSli15693_Sw_Int.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)
