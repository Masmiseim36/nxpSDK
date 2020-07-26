

PROJECT(NxpRdLib_halHw)

FILE(GLOB NxpRdLib_halHw_Sources
    ./src/phhalHw.c
    ./src/DUT/phhalHw_DUT.c
    ./src/DUT/phhalHw_DUT.h
    ./src/DUT/phhalHw_DUT_CL.c
    ./src/DUT/phhalHw_DUT_Int.c
    ./src/DUT/phhalHw_DUT_Int.h
    ./src/Pn5180/phhalHw_Pn5180.c
    ./src/Pn5180/phhalHw_Pn5180.h
    ./src/Pn5180/phhalHw_Pn5180_Instr.c
    ./src/Pn5180/phhalHw_Pn5180_Int.c
    ./src/Pn5180/phhalHw_Pn5180_Int.h
    ./src/Pn5180/phhalHw_Pn5180_Wait.c
    ./src/Pn5180/phhalHw_Pn5180_Wait.h
    ./src/Pn5190/phhalHw_Pn5190.c
    ./src/Pn5190/phhalHw_Pn5190.h
    ./src/Pn5190/phhalHw_Pn5190_InstMngr.c
    ./src/Pn5190/phhalHw_Pn5190_Instr.c
    ./src/Pn5190/phhalHw_Pn5190_InstrMngr.h
    ./src/Pn5190/phhalHw_Pn5190_Int.c
    ./src/Pn5190/phhalHw_Pn5190_Int.h
    ./src/Pn5190/phhalHw_Pn5190_Wait.c
    ./src/Pn5190/phhalHw_Pn5190_Wait.h
    ./src/PN7462AU/phhalHw_PN7462AU.c
    ./src/PN7462AU/phhalHw_PN7462AU.h
    ./src/PN7462AU/phhalHw_PN7462AU_Int.c
    ./src/PN7462AU/phhalHw_PN7462AU_Int.h
    ./src/Rc663/phhalHw_Rc663.c
    ./src/Rc663/phhalHw_Rc663.h
    ./src/Rc663/phhalHw_Rc663_Cmd.c
    ./src/Rc663/phhalHw_Rc663_Config.h
    ./src/Rc663/phhalHw_Rc663_Int.c
    ./src/Rc663/phhalHw_Rc663_Int.h
    ./src/Rc663/phhalHw_Rc663_Wait.c
)

ADD_LIBRARY(NxpRdLib_halHw
    ${NxpRdLib_halHw_Sources}
)


TARGET_LINK_LIBRARIES(
    NxpRdLib_halHw
    NxpRdLib_phTools
    phOsal
    NxpRdLib_KeyStore
#    NxpRdLib_balReg
    DAL
)
