/**************************************************************************
MODULE:    MCOHW
CONTAINS:  Hardware driver specification for MicroCANopen
           The specific implementations are named mcohwXXX.c, where
           XXX represents the CAN hardware used.
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

Implementation recommendations:

1.) CAN interrupt
The CAN interrupt should check all the possible error flags and set the
global variable gMCOConfig.HWstatus accordingly. Fatal errors must result
in a call to MCOUSER_FatalError with an error code in the range of 0x8000
to 0x87FF.

If a transmit queue is implemented, the transmit interrupt should be used
to trigger transmission of the next message in the transmit queue.

On "Basic CAN" controllers the receive interrupt copies the incoming message
into a receive FIFO. CAN controllers with "Full CAN" style capabilities
or internal receive queue might not need to maintain a software queue.
In case a hardware queue or buffers are used, the interrupt should still
check for a potential overrun and set bit RXOR in gMCOConfig.HWstatus in
case of an overrun.

2.) Timer interrupt
A 1ms timer interrupt needs to implement a local 1ms uint16_t timer tick.
The timer tick is only accessable via the functions MCOHW_GetTime and
MCOHW_IsTimeExpired to avoid data inconsistency.

In case only a multiple of 1ms is available on a system, the timer tick
would need to be incremented in each interrupt in a way that the timer
tick is still accurate (for example increment by 4 all 4ms).
***************************************************************************/

#ifndef _MCOHW_H
#define _MCOHW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mco.h"

/**************************************************************************
DOES:    This function returns the global status variable.
CHANGES: The status can be changed anytime by this module, for example from
         within an interrupt service routine or by any of the other
         functions in this module.
BITS:    0: INIT - set to 1 after a completed initialization
                   left 0 if not yet inited or init failed
         1: CERR - set to 1 if a CAN bit or frame error occurred
         2: ERPA - set to 1 if a CAN "error passive" occurred
         3: RXOR - set to 1 if a receive queue overrun occurred
         4: TXOR - set to 1 if a transmit queue overrun occurred
         5: Reserved
         6: TXBSY - set to 1 if Transmit queue is not empty
         7: BOFF - set to 1 if a CAN "bus off" error occurred
**************************************************************************/
uint8_t MCOHW_GetStatus(void);

/**************************************************************************
DOES:    This function implements the initialization of the CAN interface.
RETURNS: 1 if init is completed
         0 if init failed, bit INIT of MCOHW_GetStatus stays 0
**************************************************************************/
uint8_t MCOHW_Init(uint16_t BaudRate // Allowed values: 1000, 800, 500, 250, 125, 50, 25, 10
#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
                   ,
                   uint16_t BRSBaudRate
#endif
);

/**************************************************************************
DOES:    This function implements the initialization of a CAN ID hardware
         filter as supported by many CAN controllers.
         If hardware filters are not supported, a system can use software
         filters as provided by module canfifo.c or no filters at all
         (in that case ALL CAN messages are received and evaluated by
         MicroCANopen)
RETURNS: 1 if filter was set
         2 if this HW does not support filters
           (in this case HW will receive EVERY CAN message)
         0 if no more filter is available
**************************************************************************/
uint8_t MCOHW_SetCANFilter(COBID_TYPE CANID // CAN-ID to be received by filter
);

/**************************************************************************
DOES:    This function implements the deletion of a previously set CAN ID
         hardware filter as supported by many CAN controllers.
         This function is only required if a CANopen node uses dynmaic
         PDO linking extensively (frequent changes of CAN IDs used by PDOs)
RETURNS: 1 if filter was deleted
         0 if filter could not be deleted
**************************************************************************/
uint8_t MCOHW_ClearCANFilter(COBID_TYPE CANID);

#if defined(USE_CANOPEN_FD) && (USE_CANOPEN_FD == 1)
/**************************************************************************
DOES:    This function implements the initialization of a CAN ID hardware
         filter as supported by many CAN controllers.
         Allows a range of identifiers to be received
RETURNS: 1 if filter was set
         2 if this HW does not support filters
           (in this case HW will receive EVERY CAN message)
         0 if no more filter is available
**************************************************************************/
uint8_t MCOHW_SetCANFilterRange(COBID_TYPE CANID_Start, // start of identifier range
                                COBID_TYPE CANID_End    // end of identifier range
);
#endif // USE_CANOPEN_FD

/**************************************************************************
DOES:    This function implements a CAN transmit queue. With each
         function call is added to the queue.
RETURNS: 1 Message was added to the transmit queue
         0 If queue is full, message was not added,
           bit TXOR in MCOHW_GetStatus set
NOTES:   The MicroCANopen stack will not try to add messages to the queue
         "back-to-back". With each call to MCO_ProcessStack, a maximum
         of one message is added to the queue. For many applications
         a queue with length "1" will be sufficient. Only applications
         with a high busload or very slow bus speed might need a queue
         of length "3" or more.
**************************************************************************/
uint8_t MCOHW_PushMessage(CAN_MSG MEM_FAR *pTransmitBuf // Data structure with message to be send
);

/**************************************************************************
DOES:    This function implements a CAN receive queue. With each
         function call a message is pulled from the queue.
RETURNS: 1 Message was pulled from receive queue
         0 Queue empty, no message received
NOTES:   Implementation of this function greatly varies with CAN
         controller used. In an SJA1000 style controller, the hardware
         queue inside the controller can be used as the queue. Controllers
         with just one receive buffer need a bigger software queue.
         "Full CAN" style controllers might just implement multiple
         message objects, one each for each ID received (using function
         MCOHW_SetCANFilter).
**************************************************************************/
uint8_t MCOHW_PullMessage(CAN_MSG MEM_FAR *pTransmitBuf // Data structure with message received
);

/**************************************************************************
DOES:    This function reads a 1 millisecond timer tick. The timer tick
         must be a uint16_t and must be incremented once per millisecond.
RETURNS: 1 millisecond timer tick
NOTES:   Data consistency must be insured by this implementation.
         (On 8-bit systems, disable the timer interrupt incrementing
         the timer tick while executing this function)
         Systems that cannot provide a 1ms tick may consider incrementing
         the timer tick only once every "x" ms, if the increment is by "x".
**************************************************************************/
uint16_t MCOHW_GetTime(void);

/**************************************************************************
DOES:    This function compares a uint16_t timestamp to the internal timer tick
         and returns 1 if the timestamp expired/passed.
RETURNS: 1 if timestamp expired/passed
         0 if timestamp is not yet reached
NOTES:   The maximum timer runtime measurable is 0x8000 (about 32 seconds).
         For the usage in MicroCANopen that is sufficient.
**************************************************************************/
uint8_t MCOHW_IsTimeExpired(uint16_t timestamp // Timestamp to be checked for expiration
);

/**************************************************************************
// Recommended implementation for this function (8051 version):
{
uint16_t time_now;

  EA = 0; // Disable Interrupts
  time_now = gTimCnt;
  EA = 1; // Enable Interrupts
  timestamp++; // To ensure the minimum runtime
  if (time_now > timestamp)
  {
    if ((time_now - timestamp) < 0x8000)
      return 1;
    else
      return 0;
  }
  else
  {
    if ((timestamp - time_now) >= 0x8000)
      return 1;
    else
      return 0;
  }
}
**************************************************************************/

#if USE_SYNC_PRODUCER
/**************************************************************************
DOES:    Activates the SYNC producer.
NOTE:    Sync signal is only produced while MY_NMT_STATE is NMTSTATE_OP.
RETURNS: Notrhing.
**************************************************************************/
void MCOHW_SetSyncProducer(COBID_TYPE canid, // CAN ID used for SYNC production
                           uint32_t cycle,   // SYNC cycle time in microseconds
                           uint8_t cntovr    // SYNC overflow counter, zero for none used
);
#endif

#if MGR_MONITOR_ALL_NODES
/**************************************************************************
DOES:    Sets all CAN receive filters for the Manager
RETURNS: TRUE or FALSE, if filter is not available
**************************************************************************/
uint8_t MCOHWMGR_SetCANFilter(void);

/**************************************************************************
DOES:    Examines if a new message arrived in the receive queue
         for the Manager
RETURNS: TRUE if a message was copied from the receive queue
**************************************************************************/
uint8_t MCOHWMGR_PullMessage(CAN_MSG MEM_FAR *pReceiveBuf);
#endif // MGR_MONITOR_ALL_NODES

#ifdef __cplusplus
}
#endif

#endif // _MCOHW_H
/**************************************************************************
END OF FILE
**************************************************************************/
