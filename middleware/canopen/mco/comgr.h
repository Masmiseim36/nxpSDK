/**************************************************************************
MODULE:    COMGR
CONTAINS:  MicroCANopen Plus implementation, CANopen Manager
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS MANAGER
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#ifndef _MGR_H
#define _MGR_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
AUTOMATIC INCLUDES
**************************************************************************/

#include "mco.h"

/**************************************************************************
SELECT DEFAULTS FOR DEFINES, IF NOT YET SET
**************************************************************************/
#ifndef MGR_MONITOR_ALL_NODES
// Enables main Manager functionailty: monitoring bootup, heartbeats and
// emergencies of all nodes
#define MGR_MONITOR_ALL_NODES 1
#endif
#ifndef USE_LSS_MANAGER
// Enables LSS Manager, when set to 1
#define USE_LSS_MANAGER 0
#endif
#ifndef USE_MLSS_MANAGER
// Enables Micro LSS manager functionality, when set to 1
#define USE_MLSS_MANAGER 0
#endif

/**************************************************************************
Further "#define" controls used in comgr.c

__SIMULATION__
Must be defined, if code is compiled for CANopen Magic Ultimate simulation

CHECK_PARAMETERS
Additional parameter check, if parameters passed are in legal range

NR_OF_TPDOS
Number of TPDOs used

USE_EVENT_TIME
Do TPDOs use event time

USE_EMCY
Does node produce emergency messages

USE_LEDS
Are CANopen LEDs used

NR_OF_SDO_CLIENTS
Number of SDO clients provided
**************************************************************************/

/**************************************************************************
GLOBAL DEFINITIONS
**************************************************************************/

// Defines for NMT States Monitored
/* already defined in mco.h
NMTSTATE_BOOT    0x00 // Boot-up
NMTSTATE_STOP    0x04 // Stopped
NMTSTATE_OP      0x05 // Operational
NMTSTATE_PREOP   0x7F // Pre-operational
*/

// Additional internal states for heartbeat consumption
#define NMTSTATE_WAIT   0xF0 // Heartbeat consumption timeout
#define NMTSTATE_LOST   0xFF // Heartbeat consumption timeout
#define NMTSTATE_KEEPOP 0xE7 // Node dropped out of operational

// Return values for MGRCB_NodeStatusChanged
#define NODESTATUS_BOOT         0x00
#define NODESTATUS_STOPPED      0x04
#define NODESTATUS_OPERATIONAL  0x05
#define NODESTATUS_PREOP        0x7F
#define NODESTATUS_EMCY_NEW     0x80
#define NODESTATUS_EMCY_OVER    0x81
#define NODESTATUS_HBACTIVE     0x90
#define NODESTATUS_HBLOST       0x91
#define NODESTATUS_SCANSTARTED  0x9F
#define NODESTATUS_SCANCOMPLETE 0xA0
#define NODESTATUS_SCANABORTED  0xA8
#define NODESTATUS_RESETAPP     0xB0
#define NODESTATUS_RESETCOM     0xB1
#define NODESTATUS_SLEEP        0xF0
#define NODESTATUS_BOOTLOADER   0xF1

// Definitions for auto-scan read/write list
#define SL_READ        0x10 // if set, the entry in the scanlist is read
#define SL_WRITE       0x20 // if set, the entry in the scanlist is written
#define SL_LENGTH_MASK 0x07 // length is in lowest three bits
#define SL_ENTRY_SIZE  4    // size in bytes of a single scanlist entry
#define SL_ENTRY(index, subindex, flags_length) \
    (uint8_t)((index)&0x00FF), (uint8_t)((index) >> 8), (uint8_t)(subindex), (uint8_t)(flags_length)
#define SL_END        SL_ENTRY(0xFFFF, 0xFF, 0xFF)
#define SD_ENTRY_SIZE 4 // size in bytes of a single scan data entry

/**************************************************************************
GLOBAL TYPES AND STRUCTURES
**************************************************************************/

// This structure holds all node specific configuration
typedef struct
{
    uint8_t last_emcy[EMCY_DATA_SIZE]; // Last emergency msg received
    MEM_FAR uint8_t *p_scanlist;       // Pointer to autoscan list (what to read)
    MEM_FAR uint8_t *p_scandata;       // Pointer to autoscan data (data read)
    uint16_t scandelay;                // Delay in ms between read requests
    uint16_t delaytimer;               // Timer value used to implement
    uint16_t hb_time;                  // Heartbeat Consumer time in ms
    uint16_t hb_timestamp;             // Expiration time stamp
    uint8_t hb_nmtstate;               // Last HB NMT State received
    HBCONS_STATE hb_consstat;          // Consumer status: off, init, active, lost
    uint8_t scanclnt;                  // SDO client to use for scan
    uint8_t id_scanstat;               // set to IDSCAN_DONE when autoscan is completed
} NODELIST;

/**************************************************************************
PUBLIC VARIABLES
**************************************************************************/
#if MAX_NR_OF_NODES > 0
extern MEM_FAR NODELIST gNodeList[MAX_NR_OF_NODES];
#endif

/**************************************************************************
PUBLIC FUNCTIONS
**************************************************************************/

/**************************************************************************
DOES:    This function transmits the NMT Master Message.
         Commands: NMTMSG_OP, NMTMSG_STOP, NMTMSG_PREOP,
                   NMTMSG_RNODE, NMTMSG_RAPP
RETURNS: TRUE, if message was queued
         FALSE, if message could not be queued (transmit queue full)
**************************************************************************/
uint8_t MGR_TransmitNMT(uint8_t nmt_cmd, // NMT Command to be send
                        uint8_t node_id  // Node addressed with the command or zero for all nodes
);

/**************************************************************************
DOES:    Initializes Heartbeat Consumer
**************************************************************************/
void MGR_InitHBConsumer(uint8_t node_id, // Node ID to monitor
                        uint16_t hb_time // Timeout ot use (in ms)
);

/**************************************************************************
DOES:    Checks if a heartbeat consumer timeout occured
RETURNS: HB consumer status, see mco.h
**************************************************************************/
HBCONS_STATE MGR_ProcessHBCheck(uint8_t node_id);

/**************************************************************************
DOES:    This function (re-)initializes the node list maintained by the
         Manager
RETURNS: nothing
**************************************************************************/
void MGR_ResetNodeList(void);

/**************************************************************************
DOES:    Retrieves a pointer to an element in the master's node list
RETURNS: Pointer to a record in the the node list, or zero if an invalid
         node ID was used.
         Values of all bits set indicate that data was not yet retrieved
**************************************************************************/
NODELIST *MGR_GetNodeInfoPtr(uint8_t node_id);

/**************************************************************************
DOES:    Call-back function, status change of a node on the network
         NODESTATUS_BOOT          node booted
         NODESTATUS_STOPPED       node changed into operational mode
         NODESTATUS_OPERATIONAL   node changed into operational mode
         NODESTATUS_PREOP         node changed into pre-operational mode
         NODESTATUS_EMCY_OVER     Emergency clear
         NODESTATUS_EMCY_NEW      New emergency occured
         NODESTATUS_HBACTIVE      HB monitoring is now active
         NODESTATUS_HBLOST        HB was lost
         NODESTATUS_SCANSTARTED   An automated scan cycle started
         NODESTATUS_SCANCOMPLETE  Last auto-scan of node completed
         NODESTATUS_SCANABORTED   Last auto-scan of node aborted
RETURNS: Nothing
**************************************************************************/
void MGRCB_NodeStatusChanged(uint8_t node_id, // Node ID of the node transmitting the emergency
                             uint8_t status,  // NODESTATUS_xxx
                             uint8_t wait     // if TRUE blocks until node status has been fully processed
);

/**************************************************************************
DOES:    Works on Manager functionality.
         Checks if a message arrived and processes it as required.
         Checks if timeouts or states changed and require action
RETURNS: TRUE, if this process had something to do, either received a CAN
         message or produced one or issued a call-back.
         FALSE, if this process had nothing to do
***************************************************************************/
uint8_t MGR_ProcessMgr(void);
// The process are divided into:
uint8_t MGR_ProcessMgrRx(void);
uint8_t MGR_ProcessMgrTick(void);

/**************************************************************************
DOES:    This USER function works on the application specific
         manager implementation
RETURNS: nothing
**************************************************************************/
void USER_ProcessMgr(void);

#ifdef __cplusplus
}
#endif

#endif // _MGR_H
/**************************************************************************
END OF FILE
**************************************************************************/
