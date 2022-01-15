/* For MCU Integration */


#define BOARD_InitHardware() \
    BOARD_ConfigMPU(); \
    BOARD_InitBootPins();    \
    BOARD_BootClockRUN(); \
    BOARD_InitDebugConsole()
