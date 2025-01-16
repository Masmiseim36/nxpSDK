/**
 * Copyright 2018,2022,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "usb_host_config.h"

#if (!USB_HOST_CONFIG_KHCI) && (!USB_HOST_CONFIG_EHCI) && (!USB_HOST_CONFIG_OHCI) && (!USB_HOST_CONFIG_IP3516HS)
#error Please enable USB_HOST_CONFIG_KHCI, USB_HOST_CONFIG_EHCI, USB_HOST_CONFIG_OHCI, or USB_HOST_CONFIG_IP3516HS in file usb_host_config.
#endif

#include "usb_host.h"
#include "usb_host_cdc.h"

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
#include "usb_host_cdc_ecm.h"
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define IFNAME0 'N'
#define IFNAME1 'X'

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
#define CDC_ECM_DATA_BUFFER_LEN                     (CDC_ECM_FRAME_MAX_FRAMELEN)
#define CDC_ECM_NOTIFY_NETWORK_CONNECTION_LEN       (8U)
#define CDC_ECM_NOTIFY_NETWORK_SPEED_CHANGE_LEN     (16U)
#define CDC_ECM_NOTIFY_BUFFER_LEN                   (MAX(CDC_ECM_NOTIFY_NETWORK_CONNECTION_LEN, CDC_ECM_NOTIFY_NETWORK_SPEED_CHANGE_LEN))
#define CDC_ECM_STRING_MAC_BUFFER_LEN               (NETIF_MAX_HWADDR_LEN * 4 + 2U)
#define CDC_ECM_POWER_MANAGEMENT_PATTERN_FILTER_LEN (74U)
#define CDC_ECM_REQUEST_BUFFER_LEN                  (32U)
#define CDC_ECM_MAX_SUPPORT_MULTICAST_FILTERS       (2U)

#define CDC_ECM_STATE_DEVICE_ATTACH     (1U << 0)
#define CDC_ECM_STATE_DEVICE_DETACH     (1U << 1)
#define CDC_ECM_STATE_XFER_CONTROL      (1U << 2)
#define CDC_ECM_STATE_XFER_INTERRUPT    (1U << 3)
#define CDC_ECM_STATE_XFER_DATA_IN      (1U << 4)
#define CDC_ECM_STATE_XFER_DATA_OUT     (1U << 5)
#define CDC_ECM_STATE_UPDATE            (1U << 6)
#define CDC_ECM_STATE_LINK_UP           (1U << 7)
#define CDC_ECM_STATE_LINK_DOWN         (1U << 8)
#define CDC_ECM_STATE_LINK_RX           (1U << 9)
#define CDC_ECM_STATE_LINK_SET_MCFILTER (1U << 10)
#define CDC_ECM_STATE_MASK              (0x7FFU)
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
#define RNDIS_DATA_MESSAGE       (RNDIS_FRAME_MAX_FRAMELEN + RNDIS_DAT_MSG_HEADER_SIZE + 1)
#define RNDIS_CONTROL_MESSAGE    (1024)
#define RNDIS_RESPONSE_AVAILABLE (8)

#define RNDIS_LWIP_OUTPUT       (0x01)
#define RNDIS_DEVICE_ATTACH     (0x02)
#define RNDIS_CONTROL_MSG       (0x04)
#define RNDIS_INTERRUPT_MSG     (0x08)
#define RNDIS_LWIP_INPUT        (0x10)
#define RNDIS_DEVICE_INIT_READY (0x20)
#define RNDIS_EVENT_MASK        (0xff)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*! @brief USB host application device state map */
typedef enum _usb_device_state_enum
{
    USB_DeviceStateIdle = 0,
    USB_DeviceStateAttached,
    USB_DeviceStateDetached
} USB_DeviceState_t;

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
/*! @brief USB Host CDC-ECM run state map */
typedef enum _usb_host_cdc_ecm_run_state_enum
{
    USB_HostCdcEcmRunIdle = 0,
    USB_HostCdcEcmRunSetControlInterface,
    USB_HostCdcEcmRunSetDataInterface,
    USB_HostCdcEcmRunPariseFunctionalDescriptor,
    USB_HostCdcEcmRunSetupTraffic,
    USB_HostCdcEcmRunTransfer
} USB_HostCdcEcmRunState_t;

/*! @brief USB Host CDC-ECM data state map */
typedef enum _usb_host_cdc_ecm_data_state_enum
{
    USB_HostCdcEcmDataIdle = 0,
    USB_HostCdcEcmDataXfering,
    USB_HostCdcEcmDataXferred
} USB_HostCdcEcmDataState_t;

/*! @brief USB Host CDC-ECM instance information */
typedef struct _usb_host_cdc_ecm_instance_struct
{
    /* USB Host CDC-ECM handle information */
    usb_device_handle deviceHandle;
    usb_host_handle hostHandle;
    usb_host_class_handle classHandle;
    usb_host_interface_handle controlInterfaceHandle;
    usb_host_interface_handle dataInterfaceHandle;

    /* USB Host CDC-ECM buffer information */
    uint8_t *dataRecvBuffer;
    uint8_t *dataSendBuffer;
    uint8_t *notifyBuffer;
    void *requestParamBuffer;

    /* USB Host CDC-ECM device properites information */
    uint8_t deviceMacAddress[NETIF_MAX_HWADDR_LEN];
    uint32_t deviceEthernetStatisticsBitmap;
    uint16_t deviceMaxSegmentSize;
    uint8_t devicePerfectMCFiltersSupport;
    uint16_t devicePktFilerBitmap;
    uint16_t deviceMCFilters;
    uint8_t devicePowerFilters;
    uint8_t deviceNetworkConnection;
    uint32_t deviceNetworkDownLinkSpeed;
    uint32_t deviceNetworkUpLinkSpeed;

    /* USB Host CDC-ECM instance state information */
    volatile USB_DeviceState_t devCurState;
    volatile USB_DeviceState_t devWaitState;
    volatile USB_HostCdcEcmRunState_t runPrevState;
    volatile USB_HostCdcEcmRunState_t runCurState;
    volatile USB_HostCdcEcmRunState_t runWaitState;
    volatile USB_HostCdcEcmDataState_t dataState;

    /* LwIP interface instance information */
    void *netif;
#if defined(SDK_OS_FREE_RTOS)
    EventGroupHandle_t netifUsbStateEvent;
    EventGroupHandle_t netifUsbDataOutEvent;
    EventGroupHandle_t netifUsbIgmpFilterEvent;
#else
    uint32_t netifUsbStateEvent;
    uint32_t netifUsbDataOutEvent;
    uint32_t netifUsbIgmpFilterEvent;
#endif
} USB_HostCdcEcmInstance_t;

/**
 * @brief USB Host CDC-ECM event process function
 * @param deviceHandle device handle
 * @param configurationHandle attached device's configuration descriptor information
 * @param event_code callback event code
 * @return USB status code
 */
usb_status_t USB_HostCdcEcmEvent(usb_device_handle deviceHandle,
                                 usb_host_configuration_handle configurationHandle,
                                 usb_host_event_t event_code);

/**
 * This function implements the host class action, it is used to create task.
 * @brief USB Host CDC-ECM class process function
 * @param param USB Host CDC-ECM instance pointer
 * @param task_event Host CDC-ECM class task event
 */
void USB_HostCdcEcmTask(void *param, uint32_t *task_event);
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
typedef struct _usb_host_rndis_instance_struct
{
    usb_device_handle deviceHandle;
    usb_host_class_handle classHandle;
    usb_host_interface_handle controlInterfaceHandle;
    usb_host_interface_handle dataInterfaceHandle;
    usb_host_handle hostHandle;
#if defined(SDK_OS_FREE_RTOS)
    EventGroupHandle_t event_group;
    EventGroupHandle_t lwipoutput;
#endif
    uint8_t *sendMessage;
    uint8_t *getMessage;
    uint8_t *outPutBuffer;
    uint8_t *inPutBuffer;
    void *netif;
    usb_host_cdc_acm_state_struct_t state;
    uint8_t deviceState;
    uint8_t previousState;
    uint8_t runState;
    uint8_t previousRunState;
    volatile uint8_t interruptRunState;
    uint8_t runWaitState;
    uint8_t dataSend;
    uint8_t responseAvailable;
    volatile uint8_t attach;
    uint8_t pollingInSending;
} usb_host_rndis_instance_struct_t;

/*! @brief USB Host RNDIS run state map */
typedef enum HostCdcRndisRunState
{
    kUSB_HostCdcRndisRunIdle = 0,                    /* idle */
    kUSB_HostCdcRndisRunSetControlInterface,         /* execute set interface code */
    kUSB_HostCdcRndisRunWaitSetControlInterface,     /* wait set interface done */
    kUSB_HostCdcRndisRunSetControlInterfaceDone,     /* set interface is done, execute next step */
    kUSB_HostCdcRndisRunSetDataInterface,            /* execute set interface code */
    kUSB_HostCdcRndisRunWaitSetDataInterface,        /* wait set interface done */
    kUSB_HostCdcRndisRunSetDataInterfaceDone,        /* set interface is done, execute next step */
    kUSB_HostCdcRndisRunWaitInitMsg,                 /* wait init message done */
    kUSB_HostCdcRndisRunWaitInitMsgDone,             /* init message done, execute next step */
    kUSB_HostCdcRndisRunWaitGetMACAddress,           /* wait query message done */
    kUSB_HostCdcRndisRunWaitGetMACAddressDone,       /* init query done, execute next step */
    kUSB_HostCdcRndisRunWaitMaxmumFrame,             /* wait query message done */
    kUSB_HostCdcRndisRunWaitMaxmumFrameDone,         /* init query done, execute next step */
    kUSB_HostCdcRndisRunWaitSetMsg,                  /* wait set message done */
    kUSB_HostCdcRndisRunWaitSetMsgDone,              /* init query done, execute next step */
    kUSB_HostCdcRndisRunWaitDataSend,                /* wait data send done */
    kUSB_HostCdcRndisRunDataSendDone,                /* data send is done, execute next step */
    kUSB_HostCdcRndisRunDataReceive,                 /* wait data receive done */
    kUSB_HostCdcRndisRunWaitDataReceived,            /* wait data receive done */
    kUSB_HostCdcRndisRunDataReceivedDone,            /* data receive is done, execute next step */
    kUSB_HostCdcRndisRunWaitGetEncapsulatedCommand,  /* wait data receive done */
    kUSB_HostCdcRndisRunGetEncapsulatedResponseDone, /* data receive is done, execute next step */
    kUSB_HostCdcRndisRunInterruptRecvPrime,
    kUSB_HostCdcRndisRunInterruptRecvDone,
    kUSB_HostCdcRndisRunGetState,
    kUSB_HostCdcRndisRunGetStateDone
} host_cdc_rndis_run_state;

/**
 * This function implements the host class action, it is used to create task.
 * @brief USB Host RNDIS class process function
 * @param param USB Host RNDIS instance pointer
 */
void USB_HostCdcRndisTask(void *param);

/**
 * @brief USB Host RNDIS event process function
 * @param deviceHandle device handle
 * @param configurationHandle attached device's configuration descriptor information
 * @param event_code callback event code
 * @return USB status code
 */
usb_status_t USB_HostCdcRndisEvent(usb_device_handle deviceHandle,
                                   usb_host_configuration_handle configurationHandle,
                                   usb_host_event_t event_code);
#endif

/*! @brief ethernetif config information for Network interface initialization */
typedef struct _ethernetifConfig_struct
{
    uint8_t controllerId;
    void *privateData;
} ethernetifConfig_t;

typedef usb_status_t (*USB_HostEventFcn_t)(usb_device_handle deviceHandle,
                                           usb_host_configuration_handle configurationHandle,
                                           usb_host_event_t event_code);

/**
 * @brief Network interface initialization function
 * @param netif the LwIP network interface structure for this ethernetif
 * @return LwIP error code
 */
err_t USB_EthernetIfInIt(struct netif *netif);

#if !defined(SDK_OS_FREE_RTOS)
/**
 * @brief Network interface input process function
 * This function should be called when a packet is ready to be read from the interface. It is used by bare-metal
 * applications.
 * @param netif the LwIP network interface structure for this ethernetif
 */
void USB_EthernetIfInput(struct netif *netif);
#endif

/**
 * @brief Network interface output process function
 * @param netif the LwIP network interface structure for this ethernetif
 * @param p the LwIP network packet buffer
 * @return LwIP error code
 */
err_t USB_EthernetIfOutPut(struct netif *netif, struct pbuf *p);

/**
 * @brief add or delete an entry in the multicast filter table of the ethernet MAC
 * @param netif the LwIP network interface structure for this ethernetif
 * @param operated group multicast group
 * @param action add or delete operation code
 * @return LwIP error code
 */
err_t USB_EthernetIfIgmpMacFilter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action);
