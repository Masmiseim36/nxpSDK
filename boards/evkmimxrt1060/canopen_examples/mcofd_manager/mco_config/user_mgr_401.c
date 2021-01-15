/**************************************************************************
MODULE:    USER_MGR_401, replaces USER_MGR
CONTAINS:  Default functions for manager application, handle CiA401 devices
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

// Include all MicroCANopen Plus Manager relevant files
#include "mcop_mgr_inc.h"


#if (MAX_NR_OF_NODES != NR_OF_SDO_CLIENTS)
#error This example requires one SDO channel for each node
#endif

/**************************************************************************
MODUL VARIABLES, here handling of OD entries that get automatically scanned
***************************************************************************/

#define DEFAULT_SDODELAY         50 // Default SDO delay between transfers in ms
#define DEFAULT_SLAVE_HEARTBEAT 500 // Default slave heartbeat in ms

// Internal state of node
#define NODE_OP_MSK          3  // mask for bits for operational mode
#define NODE_NON             0  // not on network,
#define NODE_ON_INIT         1  // on network, not yet usable,
#define NODE_ON_PREOP        2  // on network, pre-op or stopped
#define NODE_ON_OP           3  // on network, operational
#define NODE_SDO_SCAN     0x08  // auto scan ongoing
#define NODE_SDO_MSK      0x30  // mask for bits for SDO channel state
#define NODE_SDO_INUSE    0x10  // SDO channel in use
#define NODE_SDO_ABORT    0x20  // SDO channel in had abort

#define SDO_CLNT_BUF_SIZE   64  // (U)SDO client buffer size

// auto scan list, which object dictionary entries to read/write from/to each node
static MEM_FAR uint8_t mScanList[] = {
                           SL_ENTRY(0x1017,0,SL_WRITE|2),  // 0
                           SL_ENTRY(0x1016,1,SL_WRITE|4),  // 1
#if USE_CANOPEN_FD
                           SL_ENTRY(0x1000,1,SL_READ |4),  // 2
#else
                           SL_ENTRY(0x1000,0,SL_READ |4),  // 2
#endif
                           SL_ENTRY(0x1018,1,SL_READ |4),  // 3
                           SL_ENTRY(0x1018,2,SL_READ |4),  // 4
                           SL_END
                          };

// mScanList entry numbers
typedef enum {
  SL_ENTRY_WRITE_1017_0 = 0,  // Entry number to write Heartbeat producer
  SL_ENTRY_WRITE_1016_1,      // Entry number to write Heartbeat consumer
  SL_ENTRY_READ_1000_x,       // Entry number to read Device Type
  SL_ENTRY_READ_1018_1,       // Entry number to read Vendor ID
  SL_ENTRY_READ_1018_2,       // Entry number to read Product Code
} ScanListEntryNumbers;

// Length of auto scan list
#define SCAN_NROFENTRIES (sizeof(mScanList)/SL_ENTRY_SIZE - 1)

#if MGR_MONITOR_ALL_NODES

#if USE_CANOPEN_FD
static MEM_FAR USDOCLIENT * pSC[MAX_NR_OF_NODES];
#else
static MEM_FAR SDOCLIENT * pSC[MAX_NR_OF_NODES];
#endif // USE_CANOPEN_FD

static uint8_t nodelp = 0; // working on nodes found
static uint16_t abort_cnt = 0; // counting the number of SDO aborts
static uint8_t n401found = 0; // set, when first 401 node found

#endif // MGR_MONITOR_ALL_NODES

// Collected data for node management
typedef struct {
  uint8_t node_state;
  uint16_t delay; // delay in between SDO cycles
  uint8_t buffer[SDO_CLNT_BUF_SIZE]; // buffer for SDO client transfers
  uint8_t scan_data[SCAN_NROFENTRIES*SD_ENTRY_SIZE]; //  auto scan read/write data
} NodeManageType;

static MEM_FAR NodeManageType mNodeManage[MAX_NR_OF_NODES];


#if USE_CANOPEN_FD
/**************************************************************************
DOES:    Called when an USDO client transfer is completed
RETURNS: nothing
**************************************************************************/
void USDOCLNTCB_USDOComplete (
  uint8_t channel, // USDO channel number in range of 1 to NR_OF_SDO_CLIENTS
  uint32_t abort_code // abort code
  )
{
  // need to know if transfer aborted or not?
  mNodeManage[channel-1].node_state &= ~NODE_SDO_MSK;  // USDO channel is idle
  if (abort_code != USDO_ABORT_NONE)
  {
    mNodeManage[channel-1].node_state |= NODE_SDO_ABORT;
  }
  if (n401found == 0xFE)
  { // marker for 1008 access
	PRINTF("\r\nCANopen FD Manager USDO Client Complete: node %d\r\n [1008,00]:%s",channel,mNodeManage[channel-1].buffer);
	n401found = 0xFF;
  }
}
#else
/**************************************************************************
DOES:    Called when an SDO client transfer is completed
RETURNS: nothing
**************************************************************************/
void SDOCLNTCB_SDOComplete (
  uint8_t channel, // SDO channel number in range of 1 to NR_OF_SDO_CLIENTS
  uint32_t abort_code // status, error, abort code
  )
{
  // need to know if transfer aborted or not?
  mNodeManage[channel-1].node_state &= ~NODE_SDO_MSK;  // SDO channel is idle
  if (abort_code != SDOERR_OK)
  {
    mNodeManage[channel-1].node_state |= NODE_SDO_ABORT;
  }
  if (n401found == 0xFE)
  { // marker for 1008 access
	PRINTF("\r\nCANopen Manager Client SDO Complete: node %d\r\n [1008,00]:%s",channel,mNodeManage[channel-1].buffer);
	n401found = 0xFF;
  }
}
#endif // USE_CANOPEN_FD


#if MGR_MONITOR_ALL_NODES

/**************************************************************************
DOES:    Call-back function, status change of a node on the network
         NODESTATUS_BOOT          node booted
         NODESTATUS_STOPPED       node changed into stopped mode
         NODESTATUS_OPERATIONAL   node changed into operational mode
         NODESTATUS_PREOP         node changed into pre-operational mode
         NODESTATUS_EMCY_OVER     Emergency clear
         NODESTATUS_EMCY_NEW      New emergency occurred
         NODESTATUS_HBACTIVE      HB monitoring is now active
         NODESTATUS_HBLOST        HB was lost
         NODESTATUS_SCANCOMPLETE  Auto-scan of node completed (in nodelist)
         NODESTATUS_SCANABORTED   Auto-scan of node completed (in nodelist)
RETURNS: Nothing
**************************************************************************/
void MGRCB_NodeStatusChanged (
  uint8_t node_id, // Node ID of the node with a new status
  uint8_t status,  // NODESTATUS_xxx
  uint8_t wait     // if TRUE, blocking allowed
  )
{

#if USE_CANOPEN_FD
  PRINTF("\r\nCANopen FD Manager Event - Node Status Change: %d, 0x%02X ",node_id,status);
#else
  PRINTF("\r\nCANopen Manager Event - Node Status Change: %d, 0x%02X ",node_id,status);
#endif

  if ((node_id > 0) && (node_id <= MAX_NR_OF_NODES))
  {
    if (status == NODESTATUS_BOOT)
    {
      PRINTF("booted ");
      // Make sure node is in pre-operational mode for configuration
      MGR_TransmitNMT(NMTMSG_PREOP,node_id);
      // All write entries' data has to be initialized at this point, using little-endian byte ordering.
      // Initialize the default heartbeat for slave nodes.
      GEN_WR16(&mNodeManage[node_id-1].scan_data[SL_ENTRY_WRITE_1017_0 * SD_ENTRY_SIZE],DEFAULT_SLAVE_HEARTBEAT);
      GEN_WR32(&mNodeManage[node_id-1].scan_data[SL_ENTRY_WRITE_1016_1 * SD_ENTRY_SIZE],(1 << 16) + 1000); // node id 1, 1000ms
      // trigger scan write/read to ensure we get a heartbeat early on
      MGRSCAN_Init(node_id,node_id,mScanList,mNodeManage[node_id-1].scan_data,DEFAULT_SDODELAY);
      mNodeManage[node_id-1].node_state |= NODE_SDO_SCAN;
      // At the same time, set up the heartbeat consumer for the node, using three times the heartbeat rate
      // to be on the safe side.
      MGR_InitHBConsumer(node_id,DEFAULT_SLAVE_HEARTBEAT*3);
    }
    else if (status == NODESTATUS_SCANCOMPLETE)
    { // Node was auto scanned, SDO is idle now
#if USE_CANOPEN_FD
      PRINTF("scan complete\r\n [1000,01]:%08X ",GEN_RD32(&mNodeManage[node_id-1].scan_data[SL_ENTRY_READ_1000_x*SD_ENTRY_SIZE]));
#else
      PRINTF("scan complete\r\n [1000,00]:%08X ",GEN_RD32(&mNodeManage[node_id-1].scan_data[SL_ENTRY_READ_1000_x*SD_ENTRY_SIZE]));
#endif
      PRINTF("[1018,01]:%08X ",GEN_RD32(&mNodeManage[node_id-1].scan_data[SL_ENTRY_READ_1018_1*SD_ENTRY_SIZE]));
      PRINTF("[1018,02]:%08X ",GEN_RD32(&mNodeManage[node_id-1].scan_data[SL_ENTRY_READ_1018_2*SD_ENTRY_SIZE]));
      mNodeManage[node_id-1].node_state &= ~NODE_SDO_SCAN;
      // timeout to start operation in USER_ProcessMgr(), can be reduced for better performance
      mNodeManage[node_id-1].delay = MCOHW_GetTime() + DEFAULT_SDODELAY;
      // start node
      MGR_TransmitNMT(NMTMSG_OP,node_id);
    }
    else if (status == NODESTATUS_HBACTIVE)
    {
        PRINTF("heartbeat monitoring active ");
    }
    else if (status == NODESTATUS_EMCY_NEW)
    {
        PRINTF("emergency ");
    }
    else if (status == NODESTATUS_EMCY_OVER)
    {
        PRINTF("emergency over / reset ");
    }
    else if (status == NODESTATUS_HBLOST)
    { // Node was lost, send node a reset
      PRINTF("heartbeat lost ");
      MGR_TransmitNMT(NMTMSG_RESETAPP,node_id);
      mNodeManage[node_id-1].node_state = NODE_NON;
      mNodeManage[node_id-1].scan_data[(int)SL_ENTRY_READ_1000_x * SD_ENTRY_SIZE] = 0;
    }
    else if ((status == NODESTATUS_STOPPED) || (status == NODESTATUS_PREOP))
    {
      PRINTF("not operational ");
      mNodeManage[node_id-1].node_state &= ~NODE_OP_MSK;
      mNodeManage[node_id-1].node_state |= NODE_ON_PREOP;
      // if not in operational mode, outside of scan, try to start start node
      if (!(mNodeManage[node_id-1].node_state & NODE_SDO_SCAN))
      {
        MGR_TransmitNMT(NMTMSG_OP,node_id);
      }
    }
    else if (status == NODESTATUS_OPERATIONAL)
    {
      PRINTF("operational ");
      mNodeManage[node_id-1].node_state &= ~NODE_OP_MSK;
      mNodeManage[node_id-1].node_state |= NODE_ON_OP;
    }
  }
}


#if USE_LSS_MANAGER
void MGRCB_LSSIdentified (
  MEM_FAR uint8_t *pDat // Pointer to 8 data bytes received
  )
{
  pDat = pDat;
}
#endif // USE_LSS_MANAGER


/**************************************************************************
DOES:    Working on user specific management functions
RETURNS: nothing
**************************************************************************/
void USER_ProcessMgr (void)
{
static uint16_t tim_val; // received timer value analog one

  // ensure that nodelp runs from 1 to MAX_NR_OF_NODES
  nodelp++;
  if (nodelp > MAX_NR_OF_NODES)
  {
    nodelp = 1;
  }

  if ( ((mNodeManage[nodelp-1].node_state & NODE_OP_MSK) >= NODE_ON_PREOP) &&
       (!(mNodeManage[nodelp-1].node_state & NODE_SDO_SCAN)) &&
       (MCOHW_IsTimeExpired(mNodeManage[nodelp-1].delay))
     )
  { // node has SDO channel free, initial delay is over
    // set new delay, in this example, max of one transfer per 1000ms
    mNodeManage[nodelp-1].delay = MCOHW_GetTime() + 1000;
    // now work on nodes
    //  check if Device Profile is CiA 401
    if ((GEN_RD32(&mNodeManage[nodelp-1].scan_data[(int)SL_ENTRY_READ_1000_x * SD_ENTRY_SIZE]) & 0x0000FFFF) == 401)
    { // this is a CiA 401 device
      if (n401found == 0)
      { // first CiA 401 device found, for this one adopt PDOs to match 401 device
        // Expected length is 4 bytes, 16 bytes for RPDO2 in CANopen FD
        MCO_ChangeRPDOID(1,0x180+nodelp);
        MCO_ChangeRPDOID(2,0x280+nodelp);
        MCO_ChangeTPDOID(1,0x200+nodelp);
        MCO_ChangeTPDOID(2,0x300+nodelp);
      }
      // read 1008h
      // SDO client example using non-blocking call and call-back SDOCLNT_SDOComplete
#if USE_CANOPEN_FD
      pSC[nodelp-1] = USDOCLNT_Init(nodelp,nodelp,&(mNodeManage[nodelp-1].buffer[0]),SDO_CLNT_BUF_SIZE,FALSE);
      if (pSC[nodelp-1] != NULL)
      {
        // read [0x1008,0x00] of node
        USDOCLNT_Read(pSC[nodelp-1],0x1008,0x00);
      }
#else
      pSC[nodelp-1] = SDOCLNT_Init(nodelp,0x600+nodelp,0x580+nodelp,&(mNodeManage[nodelp-1].buffer[0]),SDO_CLNT_BUF_SIZE);
      if (pSC[nodelp-1] != NULL)
      {
        // read [0x1008,0x00] of node
        SDOCLNT_Read(pSC[nodelp-1],0x1008,0x00);
      }
#endif // USE_CANOPEN_FD
      // mark for processing in SDO complete
      n401found = 0xFE;
      // now mark channel used
      mNodeManage[nodelp-1].node_state &= ~NODE_SDO_MSK;
      mNodeManage[nodelp-1].node_state |= NODE_SDO_INUSE;
      // set marker in scan_data for further processing
      mNodeManage[nodelp-1].scan_data[(int)SL_ENTRY_READ_1000_x * SD_ENTRY_SIZE] = 0xFF;
    }
    else if (mNodeManage[nodelp-1].scan_data[(int)SL_ENTRY_READ_1000_x * SD_ENTRY_SIZE] == 0xFF)
    { // marker found, read analog value [6411,01], blocking version
      if (pSC[nodelp-1] != NULL)
      {
#if USE_CANOPEN_FD
        USDOCLNT_ReadCycle(nodelp,nodelp,0x6411,0x01,(MEM_FAR uint8_t *)&tim_val,2,0);
#else
        SDOCLNT_ReadCycle(nodelp,nodelp,0x6411,0x01,(MEM_FAR uint8_t *)&tim_val,2,0);
#endif // USE_CANOPEN_FD
#if DEBUG_CONSOLE
#warning "Extended DEBUG_CONSOLE output enabled, delays execution"
        PRINTF("\r\n [6411,01]:0x%04X ",tim_val);
#endif
      }
    }
  }
  else if  (mNodeManage[nodelp-1].node_state & NODE_SDO_ABORT)
  { // node had an abort
    abort_cnt++;
    mNodeManage[nodelp-1].delay = MCOHW_GetTime() + 1000; // 1s timeout after abort
    mNodeManage[nodelp-1].node_state &= ~NODE_SDO_MSK;  // mark channel free again to start over
  }

  // Check for CAN Err, auto-recover
  if (MCOHW_GetStatus() & HW_BOFF)
  {
    MCOUSER_FatalError(0xF6F6);
  }

}

#endif // MGR_MONITOR_ALL_NODES

/**************************************************************************
END-OF-FILE
***************************************************************************/
