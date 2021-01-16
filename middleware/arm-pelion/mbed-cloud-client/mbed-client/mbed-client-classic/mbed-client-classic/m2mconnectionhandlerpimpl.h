/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef M2M_CONNECTION_HANDLER_PIMPL_H__
#define M2M_CONNECTION_HANDLER_PIMPL_H__

#include "ns_types.h"
#include "ns_list.h"
#include "eventOS_event.h"
#include "mbed-client/m2mconfig.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mconnectionobserver.h"
#include "mbed-client/m2mconnectionsecurity.h"
#include "nsdl-c/sn_nsdl.h"
#include "pal.h"


class M2MConnectionSecurity;
class M2MConnectionHandler;
class M2MSecurity;

/**
 * @brief M2MConnectionHandlerPimpl.
 * This class handles the socket connection for LWM2M Client
 */


class M2MConnectionHandlerPimpl {
public:

    enum SocketEvent {
        ESocketIdle          = 0x00,
        ESocketCallback      = 0x02,
        ESocketConnect       = 0x04,
        ESocketSend          = 0x08,
        ESocketDnsResolved   = 0x10,
        ESocketDnsError      = 0x20,
        ESocketClose         = 0x40
    };

    // NOTE! Check that these values does not overlap with the SocketEvent values
    enum InterfaceStatusEvent {
        EInterfaceConnected     = 0x41,
        EInterfaceDisconnected  = 0x42
    };

    /**
    * @brief Constructor
    */
    M2MConnectionHandlerPimpl(M2MConnectionHandler* base, M2MConnectionObserver &observer,
                              M2MConnectionSecurity* sec,
                              M2MInterface::BindingMode mode,
                              M2MInterface::NetworkStack /*stack*/);

    /**
    * @brief Destructor
    */
    ~M2MConnectionHandlerPimpl();

    void start_timer(void);

    /**
    * @brief This binds the socket connection.
    * @param listen_port Port to listen for incoming connection.
    * @return true if successful else false.
    */
    bool bind_connection(const uint16_t listen_port);

    /**
    * @brief This resolves the server address. Output is
    * returned through callback
    * @param String server address.
    * @param uint16_t Server port.
    * @param ServerType, Server Type to be resolved.
    * @return true if address is valid else false.
    */
    bool resolve_server_address(const String& server_address,
                                const uint16_t server_port,
                                M2MConnectionObserver::ServerType server_type,
                                const M2MSecurity* security);

    /**
    * @brief Sends data, to the connected sent to server.
    * @param data, Data to be sent.
    */
    bool send_data(uint8_t *data_ptr,
                   uint16_t data_len,
                   sn_nsdl_addr_s *address_ptr);

    /**
    * @brief Listens for incoming data from remote server
    * @return true if successful else false.
    */
    bool start_listening_for_data();

    /**
    * @brief Stops listening for incoming data
    */
    void stop_listening();

    /**
    * @brief Error handling for DTLS connectivity.
    * @param error, Error code from TLS library
    */
    void handle_connection_error(int error);

    /**
     * \brief Sets the network interface handler that is used by client to connect
     * to a network over IP..
     * \param handler A network interface handler that is used by client to connect.
     *  This API is optional but provides a mechanism for different platforms to
     * manage usage of underlying network interface by client.
     */
    void set_platform_network_handler(void *handler = NULL);

    /**
    * \brief Claims mutex to prevent thread clashes
    * in multithreaded environment.
    */
    void claim_mutex();

    /**
    * \brief Releases mutex to prevent thread clashes
    * in multithreaded environment.
    */
    void release_mutex();

    /**
    * @brief Callback handler for receiving data over socket.
    */
    void receive_handler();

    /**
    * @brief Returns true if DTLS handshake is still ongoing.
    */
    bool is_handshake_ongoing() const;

    /**
    * @brief Sends data to socket through event loop.
    */
    void send_socket_data();

    /**
    * @brief Does DNS resolving. Return true if DNS has been resolved
    * or triggered though DNS thread.
    */
    bool address_resolver(void);

    /**
     * @brief handler for eventloop events. Note, this needs to be public as it is called
     * from C wrapper.
     */
    void event_handler(arm_event_s *event);

    /**
    * \brief Closes the open connection.
    * \note This must be called from the same event loop context!
    */
    void force_close();

    /**
     * \brief Unregisters the network interface handler that is set in 'set_platform_network_handler'.
     */
    void unregister_network_handler();

private:

    /**
    * @brief Terminate the DNS thread, if any is used. The #ifdef magic is inside this
    * method, so it can be used without ifdef guards.
     */
    void terminate_dns_thread();

    /**
    * @brief Handles socket initialization and connect phase.
    */
    void socket_connect_handler();

    /**
    * @brief Callback handler for receiving data for secured connection.
    */
    void receive_handshake_handler();

    /**
    * @brief Callback handler for socket events.
    */
    void socket_event();

    /**
    * @brief Handles the result set by address_resolver.
    */
    void handle_dns_result(bool success);


    /**
    * @brief Initialize socket
    */
    bool init_socket();

    /**
    * @brief Check socket type
    * @return True if TCP connection otherwise false
    */
    bool is_tcp_connection() const;

    /**
    * @brief Close and delete socket
    */
    void close_socket();

    /**
    * @brief Init event structure.
    */
    void initialize_event(arm_event_storage_t *event);

public:

    /**
     * @brief Internal helper for sending an event.
     */
    void send_event(SocketEvent event_type);

    void interface_event(palNetworkStatus_t status);

private:
    typedef struct send_data_queue {
        uint8_t *data;
        uint16_t offset;
        uint16_t data_len;
        ns_list_link_t link;
    } send_data_queue_s;

    /**
     * @brief Get first item from the queue list.
     */
    send_data_queue_s* get_item_from_list();

    /**
     * @brief Add queue data back to list.
     */
    void add_item_to_list(send_data_queue_s* data);

private:
    enum SocketState {

        /* NOTE: Order of these values does matter, do not make unplanned changes to it. */

        /** Socket has not been initialized/connected yet. */
        ESocketStateDisconnected,

        /** pal_close() is in progress. */
        ESocketStateCloseBeingCalled,

        /** DNS resolving in progress. */
        ESocketStateDNSResolving,

        /** DNS resolving done, connection needs to be initialized.  */
        EsocketStateInitializeConnection,

        /** pal_connect() is in progress. */
        ESocketStateConnectBeingCalled,

        /** pal_connect() has been called and we are waiting for asynchronous response. */
        ESocketStateConnecting,

        /** pal_connect is complete and the (D)TLS handshake is to be done if in secure mode. */
        ESocketStateConnected,

        /** (D)TLS Handshaking in progress */
        ESocketStateHandshaking,

        /** Unsecure Connection to the server has been established */
        ESocketStateUnsecureConnection,

        /** Secure Connection to the server has been established */
        ESocketStateSecureConnection
    };

    typedef NS_LIST_HEAD(send_data_queue_s, link) send_data_list_t;

    M2MConnectionHandler                        *_base;
    M2MConnectionObserver                       &_observer;
    M2MConnectionSecurity                       *_security_impl; //owned
    const M2MSecurity                           *_security; //non-owned
    M2MInterface::BindingMode                   _binding_mode;
    M2MConnectionObserver::SocketAddress        _address;

    // _address._address will point to one of these two
    palIpV4Addr_t                               _ipV4Addr;
    palIpV6Addr_t                               _ipV6Addr;

    palSocket_t                                 _socket;
    M2MConnectionObserver::ServerType           _server_type;
    uint16_t                                    _server_port;
    uint16_t                                    _listen_port;
    uint32_t                                    _net_iface;
#if (PAL_DNS_API_VERSION == 0) || (PAL_DNS_API_VERSION == 1)
    palSocketLength_t                           _socket_address_len;
#else
    palDNSQuery_t                               _handler_async_DNS;
#endif
    volatile palSocketAddress_t                 _socket_address;
    static int8_t                               _tasklet_id;
    String                                      _server_address;

    // A state variable for the socket itself, which is needed to handle the
    // asynchronous events and callbacks. Note: the state may be accessed from
    // event sender and receiver threads.
    SocketState                                 _socket_state;

    send_data_list_t                            _linked_list_send_data;

    bool                                        _secure_connection;

    arm_event_storage_t                         _event;

    arm_event_storage_t                         _socket_callback_event;

friend class Test_M2MConnectionHandlerPimpl;
friend class Test_M2MConnectionHandlerPimpl_mbed;
friend class Test_M2MConnectionHandlerPimpl_classic;
friend class M2MConnection_TestObserver;
};

#endif //M2M_CONNECTION_HANDLER_PIMPL_H__
