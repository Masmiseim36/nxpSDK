/**************************************************************************
MODULE:    USER - User System Functions
CONTAINS:  MicroCANopen hardware specific system functions
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#include "mcop_mgr_inc.h"

#include "stackinit.h"

/**************************************************************************
GLOBAL FUNCTIONS
***************************************************************************/

/**************************************************************************
DOES:    Call-back function for occurrence of a fatal error.
         Stops operation and displays blinking error pattern on LED.
**************************************************************************/
void MCOUSER_FatalError(uint16_t ErrCode)
{
    uint16_t timeout;

#if USE_CANOPEN_FD
    PRINTF("\r\nCANopen FD Library Event - Fatal Error: 0x%04X ", ErrCode);
#else
    PRINTF("\r\nCANopen Library Event - Fatal Error: 0x%04X ", ErrCode);
#endif

    // Remember last error for stack
    gMCOConfig.last_fatal = ErrCode;

#if USE_LEDS
    LED_ERR_ON;
    gMCOConfig.LEDErr = LED_ON;
#endif

#if USE_EMCY
    MCOP_PushEMCY(0x6100, ErrCode >> 8, ErrCode, 0, 0, 0
#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
                  ,
                  0,                    // dev_num - logical device number
                  COFD_BASE_SPEC >> 20, // spec_num, CiA specification number
                  ERST_STATE_OCC | 4,   // status - priority=4, recoverable, error occurred
                  0,                    // time_lo - timestamp bits 0-31, not supported
                  0                     // time_hi - timestamp bits 32-47, not supported
#endif                                  // USE_CANOPEN_FD
    );
#endif // USE_EMCY

    // Wait 10ms
    timeout = MCOHW_GetTime() + 10;
    while (!MCOHW_IsTimeExpired(timeout))
    { // Wait until timeout
    }

    if (ErrCode >= ERR_FATAL)
    { // Fatal error, should abort/reset
        MCOUSER_ResetApplication();
    }

    // Warning only, simply timeout, then continue
}

/**************************************************************************
DOES:    Call-back function for reset application.
         Starts the watchdog and waits until watchdog causes a reset.
**************************************************************************/
void MCOUSER_ResetApplication(void)
{
#if USE_CANOPEN_FD
    PRINTF("\r\nCANopen FD Library Event - Reset Application, ignored in CANopen Manager ");
#else
    PRINTF("\r\nCANopen Library Event - Reset Application, ignored in CANopen Manager ");
#endif

    // In the manager, ignore NMT reset requests
    //  __NVIC_SystemReset();  // Use CMSIS function to generate a software reset
}

/**************************************************************************
DOES:    Call-back function for reset communication.
         Re-initializes the process image and the entire MicroCANopen
         communication.
**************************************************************************/
void MCOUSER_ResetCommunication(void)
{
    uint16_t can_bps;
#if USE_CANOPEN_FD
    uint16_t can_data_bps;
#endif
    uint8_t node_id;

    node_id = NODEID_DCF;
    can_bps = CAN_BITRATE_DCF;
#if USE_CANOPEN_FD
    can_data_bps = CAN_BRS_BITRATE;
    PRINTF("\r\nCANopen FD Library Event - Reset Communication, ");
    PRINTF("nominal bitrate %dkbps, data bitrate %dkbps, node id %d", can_bps, can_data_bps, node_id);
#else
    PRINTF("\r\nCANopen Library Event - Reset Communication, ");
    PRINTF("bitrate %dkbps, node id %d", can_bps, node_id);
#endif

    if (MCO_Init(can_bps,
#if USE_CANOPEN_FD
                 can_data_bps,
#endif
                 node_id, DEFAULT_HEARTBEAT))
    {
        // Now initialize system entries from values stored in OD
        MCO_UpdateSystemFromOD();

#if USE_STORE_PARAMETERS
        MCOSP_GetStoredParameters();
#endif

#if MGR_MONITOR_ALL_NODES
        MCOHWMGR_SetCANFilter();
        MGR_ResetNodeList();
#endif

#if (NR_OF_SDO_CLIENTS > 0)
#if USE_CANOPEN_FD
        USDOCLNT_ResetChannels();
#else
        SDOCLNT_ResetChannels();
#endif // USE_CANOPEN_FD
#endif
    }
}

#if USECB_NMTCHANGE
/**************************************************************************
DOES:    Called when the NMT state of the stack changes
RETURNS: nothing
**************************************************************************/
void MCOUSER_NMTChange(uint8_t nmtstate // new nmt state of stack
)
{
#if USE_CANOPEN_FD
    PRINTF("\r\nCANopen FD Library Event - NMT Change: 0x%02X ", nmtstate);
#else
    PRINTF("\r\nCANopen Library Event - NMT Change: 0x%02X ", nmtstate);
#endif

    if (nmtstate == NMTSTATE_BOOT)
        PRINTF("boot ");
    else if (nmtstate == NMTSTATE_PREOP)
        PRINTF("pre-operational ");
    else if (nmtstate == NMTSTATE_OP)
        PRINTF("operational ");
    else if (nmtstate == NMTSTATE_STOP)
        PRINTF("stopped ");
}
#endif

#if USECB_ODSERIAL
/**************************************************************************
DOES:    This function is called upon read requests to Object Dictionary
         entry [1018h,4] - serial number
RETURNS: The 32bit serial number
**************************************************************************/
uint32_t MCOUSER_GetSerial(void)
{
    uint32_t serial = 0;
    // replace with code to read serial number, for example from
    // non-volatile memory
#if USE_CANOPEN_FD
    PRINTF("\r\nCANopen FD Library Event - Request serial nr: 0x%08X ", serial);
#else
    PRINTF("\r\nCANopen Library Event - Request serial nr: 0x%08X ", serial);
#endif
    return serial;
}
#endif // USECB_ODSERIAL

/**************************************************************************
DOES:    Call-back function, heartbeat lost, timeout occured.
         Gets called when a heartbeat timeout occured for a node.
RETURNS: Nothing
**************************************************************************/
void MCOUSER_HeartbeatLost(uint8_t node_id)
{
#if USE_CANOPEN_FD
    PRINTF("\r\nCANopen FD Library Event - Heartbeat lost node: %d ", node_id);
#else
    PRINTF("\r\nCANopen Library Event - Heartbeat lost node: %d ", node_id);
#endif
    // Add code to react on the loss of heartbeat,
    // if node is essential, switch to pre-operational mode
    MCO_HandleNMTRequest(NMTMSG_PREOP);
}

#if USECB_TIMEOFDAY
/**************************************************************************
DOES:    This function is called if the message with the time object has
         been received. This example implementation calculates the current
         time in hours, minutes, seconds.
RETURNS: nothing
**************************************************************************/
// Global variables holding clock info
uint32_t hours;
uint32_t minutes;
uint32_t seconds;

void MCOUSER_TimeOfDay(uint32_t millis, // Milliseconds since midnight
                       uint16_t days    // Number of days since January 1st, 1984
)
{
    if (millis < (1000UL * 60 * 60 * 24))
    { // less Milliseconds as one day has
        // calculate hours, minutes & seconds since midnight
        hours   = millis / (1000UL * 60 * 60);
        minutes = (millis - (hours * 1000UL * 60 * 60)) / (1000UL * 60);
        seconds = (millis - (hours * 1000UL * 60 * 60) - (minutes * 1000UL * 60)) / 1000;
    }
}
#endif // USECB_TIMEOFDAY

/**************************************************************************
END-OF-FILE
***************************************************************************/
