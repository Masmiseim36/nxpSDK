

PROJECT(NxpRdLib_lnLlcp)

FILE(GLOB NxpRdLib_lnLlcp_Sources
    ./src/phlnLlcp.c
    ./src/Sw/phlnLlcp_Sw.c
    ./src/Sw/phlnLlcp_Sw.h
    ./src/Sw/phlnLlcp_Sw_Int.c
    ./src/Sw/phlnLlcp_Sw_Int.h
    ./src/Sw/phlnLlcp_Sw_Mac.c
    ./src/Sw/phlnLlcp_Sw_Mac.h
    ./src/Sw/phlnLlcp_Sw_Transport.c
    ./src/Sw/phlnLlcp_Sw_Transport_Connection.c
    ./src/Sw/phlnLlcp_Sw_Transport_Connectionless.c
    ./src/Sw/phlnLlcp_Timers.c
    ./src/Sw/phlnLlcp_Timers.h
)
ADD_LIBRARY(NxpRdLib_lnLlcp
    ${NxpRdLib_lnLlcp_Sources}
)


TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_phTools
)
