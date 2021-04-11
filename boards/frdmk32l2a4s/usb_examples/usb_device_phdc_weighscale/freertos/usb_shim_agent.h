/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USB_SHIM_AGENT_H_
#define _USB_SHIM_AGENT_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief QoS bin elements */
#define SHIM_AGENT_MAX_QUEUE_NUMBER (0x04U)

/*! @brief Shim event definition */
#define SHIM_AGENT_EVENT_RECV_MESSAGE_PREAMBLE (0x00U)
#define SHIM_AGENT_EVENT_RECV_OPAQUE_DATA (0x01U)
#define SHIM_AGENT_EVENT_RECV_COMPLETE (0x02U)
#define SHIM_AGENT_EVENT_SENT_COMPLETE (0x03U)

#if META_DATA_MESSAGE_PREAMBLE_IMPLEMENTED
#define METADATA_PREAMBLE_SIGNATURE (16U)
#endif
/*! @brief the application data structure */
typedef struct _usb_shim_application_data_struct
{
    uint32_t transferSize; /*!< Transfer size */
    uint8_t *buffer;       /*!< Data buffer */
} usb_shim_application_data_struct_t;

/*! @brief the TX data structure */
typedef struct _usb_shim_tx_data_struct
{
    uint8_t epNumber;                                                         /*!< Endpoint number */
    uint32_t seller;                                                          /*!< Number of queued transfers */
    uint32_t buyer;                                                           /*!< Number of dequeued transfers */
    usb_shim_application_data_struct_t sendData[SHIM_AGENT_MAX_QUEUE_NUMBER]; /*!< Data to send */
} usb_shim_tx_data_struct_t;

/*! @brief the RX data structure */
typedef struct _usb_shim_rx_data_struct
{
    uint8_t epNumber;                            /*!< Endpoint number */
    uint16_t epMaxPacketSize;                    /*!< Endpoint max packet size */
    uint32_t transferCount;                      /*!< Size of transferred data */
    usb_shim_application_data_struct_t recvData; /*!< Data to receive */
} usb_shim_rx_data_struct_t;

#if META_DATA_MESSAGE_PREAMBLE_IMPLEMENTED
/*! @brief meta-data message preamble structure */
typedef struct _usb_shim_metadata_preamble
{
    uint8_t aSignature[METADATA_PREAMBLE_SIGNATURE]; /*!< constant used to give preamble verifiability */
    uint8_t bNumberTransfers;     /*!< count of following transfer to which the QoS setting applies */
    uint8_t bQosEncodingVersion;  /*!< version of QoS information encoding */
    uint8_t bmLatencyReliability; /*!< refer to latency/reliability bin for the QoS data */
    uint8_t bOpaqueDataSize;      /*!< opaque QoS data or meta-data size */
    uint8_t *bOpaqueData;         /*!< opaque meta-data */
} usb_shim_metadata_preamble_t;
#endif

/*! @brief USB Shim structure */
typedef struct _usb_shim_agent_struct
{
    usb_device_handle deviceHandle;               /*!< The device handle */
    void *classHandle;                            /*!< The class handle */
    uint8_t speed;                                /*!< Used to store the device speed */
    uint8_t attach;                               /*!< Used to store the attach event */
    uint8_t currentConfig;                        /*!< Current configuration */
    uint8_t currentInterfaceAlternateSetting[1U]; /*!< Current alternate setting of the interface*/
#if META_DATA_MESSAGE_PREAMBLE_IMPLEMENTED
    uint8_t isMetaDataMessagePreambleEnabled; /*!< Used to store whether meta-data feature is active or not */
    uint8_t numberTransferBulkOut;            /*!< the number of transfer that follow Meta-data Message Preamble */
    uint8_t numberTransferBulkIn;             /*!< the number of transfer that follow Meta-data Message Preamble */
#endif
    uint16_t endpointsHaveData;                /*!< Which endpoints on the device have data */
    usb_shim_rx_data_struct_t bulkOutData;     /*!< Receive data information */
    usb_shim_tx_data_struct_t bulkInData;      /*!< Send data information */
    usb_shim_tx_data_struct_t interruptInData; /*!< Send data information */
    uint8_t *recvDataBuffer;                   /*!< Receive data buffer */
    uint8_t *classBuffer;                      /*!< class specific transfer buffer */
} usb_shim_agent_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief USB shim agent receive data callback function.
 *
 * This function implements a queue to support receiving the PHDC data with length is more than
 * maximum of endpoint packet size. Then notify to upper layer when receiving is all completed.
 *
 * @param handle                The device handle.
 * @param param                 The param of receive callback function.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_ShimAgentRecvComplete(void *handle, void *param);

/*!
 * @brief USB shim agent send data callback function.
 *
 * This function is the callback function of USB shim agent send function, it is implemented
 * to continue sending the data in sending queue if the queue is not empty.
 *
 * @param handle                The device handle.
 * @param event                 The event code.
 * @param param                 The param of receive callback function.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_ShimAgentSendComplete(void *handle, uint32_t event, void *param);

/*!
 * @brief USB shim agent send data function.
 *
 * This function implements a queue to support sending multiple transfer request.
 *
 * @param handle                The device handle.
 * @param qos                   The current QoS of the transfer.
 * @param appBuffer             The data buffer.
 * @param size                  The length of the transfer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_ShimAgentSendData(void *handle, uint8_t qos, uint8_t *appBuffer, uint32_t size);

/*!
 * @brief agent callback.
 *
 * This function is the callback function called by transport layer and then it
 * will call to proper agent functions.
 *
 * @param handle        the agent handle.
 * @param request       the callback request.
 * @param data          the callback data.
 * @param size          the callback data size.
 */
extern void AGENT_Callback(void *handle, uint8_t request, uint8_t *data, uint32_t size);

#if defined(__cplusplus)
}
#endif
#endif /* _USB_SHIM_AGENT_H_ */
