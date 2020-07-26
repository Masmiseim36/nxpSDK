/**************************************************************************
MODULE:    USER - User System Functions
CONTAINS:  MicroCANopen hardware specific system functions
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2019
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.01, EmSA 02-APR-20
           $LastChangedDate: 2020-04-26 20:06:08 +0200 (Sun, 26 Apr 2020) $
           $LastChangedRevision: 4959 $
***************************************************************************/

#include "mcop_mgr_inc.h"

#include "stackinit.h"

/**************************************************************************
GLOBAL FUNCTIONS
***************************************************************************/

/**************************************************************************
DOES:    Call-back function for occurance of a fatal error.
         Stops operation and displays blnking error pattern on LED
**************************************************************************/
void MCOUSER_FatalError(uint16_t ErrCode)
{
    uint16_t timeout;

    PRINTF("\r\nCANopen Library Event - Fatal Error: 0x%04X ", ErrCode);

    // Remember last error for stack
    gMCOConfig.last_fatal = ErrCode;

#if USE_LEDS
    LED_ERR_ON;
    gMCOConfig.LEDErr = LED_ON;
#endif

#if USE_EMCY
    MCOP_PushEMCY(0x6100, ErrCode >> 8, ErrCode, 0, 0, 0);
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
    PRINTF("\r\nCANopen Library Event - Reset Application, ignored in CANopen Manager ");

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

    PRINTF("\r\nCANopen Library Event - Reset Communication, ");

    node_id = NODEID_DCF;
    can_bps = CAN_BITRATE_DCF;
#if USE_CANOPEN_FD
    can_data_bps = CAN_BRS_BITRATE;
#endif

    PRINTF("bitrate %dkbps, node id %d ", can_bps, node_id);

    if (MCO_Init(can_bps,
#if USE_CANOPEN_FD
                 can_data_bps,
#endif
                 node_id, DEFAULT_HEARTBEAT))
    {
#if USE_LSS_SLAVE == 0
        // Initialization of PDOs comes from EDS
        INITPDOS_CALLS

// Initialization of heartbeat consumers from EDS
#if MGR_MONITOR_ALL_NODES
// In manager, substitute slave for manager heartbeat consumer init function
#define MCOP_InitHBConsumer(chn, nid, tim) MGR_InitHBConsumer(nid, tim)
#endif
        // Initialization of heartbeat consumers from EDS
        INITHBCONSUMER_CALLS

#if USE_STORE_PARAMETERS
        MCOSP_GetStoredParameters();
#endif

#if MGR_MONITOR_ALL_NODES
        MCOHWMGR_SetCANFilter();
        MGR_ResetNodeList();
#endif

#if (NR_OF_SDO_CLIENTS > 0)
        SDOCLNT_ResetChannels();
#endif

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
    // unused
    PRINTF("\r\nCANopen Library Event - NMT Change: 0x%02X ", nmtstate);
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
    PRINTF("\r\nCANopen Library Event - Request serial nr: 0x%08X ", serial);
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
    PRINTF("\r\nCANopen Library Event - Heartbeat lost node: %d ", node_id);
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
