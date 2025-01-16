# lwip_https_client_freertos

## Overview

The lwip_https_client_freertos demo application demonstrates downloading of a web page over SSL connection using HTTP GET method.

Upon start, the example obtains IP address and DNS information from DHCP, resolves IP address of a server and establishes a secure connection with it.
Then it downloads and displays the web resource and closes the secure connection.

Four major third party components are used to achieve the functionality:
  - lwIP TCP/IP stack for TCP/IP connectivity,
  - mbedTLS for securing the connection,
  - coreHTTP library for HTTP protocol,
  - FreeRTOS for operating system.

The resource to download is defined by EXAMPLE_HOST, EXAMPLE_PORT and EXAMPLE_PATH macros.
EXAMPLE_MAX_REQUEST_LENGTH and EXAMPLE_MAX_RESPONSE_LENGTH might need to be increased if downloading other resource.

List of certification authority certificates used for validation of the server's certificate are stored in app_ca_bundle.c in g_app_ca_bundle.

On some hardware platforms, this example overrides the list of SSL cipher suites offered by the client to the server. It is done by defining
the list in MBEDTLS_SSL_CIPHERSUITES macro. This list needs to have common items with the list of cipher suites offered by the server to the client,
otherwise the two will not be able to communicate. The list of server's ciphersuites can be examined for example by nmap utility:

        $ nmap --script ssl-enum-ciphers -p 443 httpbin.org
        Starting Nmap 7.80 ( https://nmap.org ) at 2024-12-04 18:20 CET
        Nmap scan report for httpbin.org (3.223.200.11)
        Host is up (0.11s latency).
        Other addresses for httpbin.org (not scanned): 34.224.200.202
        rDNS record for 3.223.200.11: ec2-3-223-200-11.compute-1.amazonaws.com

        PORT    STATE SERVICE
        443/tcp open  https
        | ssl-enum-ciphers:
        |   TLSv1.0:
        |     ciphers:
        |       TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA (secp256r1) - A
        |       TLS_RSA_WITH_AES_128_CBC_SHA (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_CBC_SHA (rsa 2048) - A
        |       TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384 (secp256r1) - A
        |       TLS_RSA_WITH_AES_128_GCM_SHA256 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_128_CBC_SHA256 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_GCM_SHA384 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_CBC_SHA256 (rsa 2048) - A
        |     compressors:
        |       NULL
        |     cipher preference: server
        |   TLSv1.1:
        |     ciphers:
        |       TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA (secp256r1) - A
        |       TLS_RSA_WITH_AES_128_CBC_SHA (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_CBC_SHA (rsa 2048) - A
        |       TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384 (secp256r1) - A
        |       TLS_RSA_WITH_AES_128_GCM_SHA256 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_128_CBC_SHA256 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_GCM_SHA384 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_CBC_SHA256 (rsa 2048) - A
        |     compressors:
        |       NULL
        |     cipher preference: server
        |   TLSv1.2:
        |     ciphers:
        |       TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384 (secp256r1) - A
        |       TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA (secp256r1) - A
        |       TLS_RSA_WITH_AES_128_GCM_SHA256 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_128_CBC_SHA256 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_128_CBC_SHA (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_GCM_SHA384 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_CBC_SHA256 (rsa 2048) - A
        |       TLS_RSA_WITH_AES_256_CBC_SHA (rsa 2048) - A
        |     compressors:
        |       NULL
        |     cipher preference: server
        |_  least strength: A

        Nmap done: 1 IP address (1 host up) scanned in 8.88 seconds

If the server is public on internet, the online service such as https://www.ssllabs.com/ssltest/index.html
can be used to get the information like supported ciphersuites, certification paths and protocol details.

The output of mbedTLS debug messages to the console can be enabled by the following:
    - define MBEDTLS_DEBUG_C macro in mbedTLS configuration file or on a project level,
    - redefine MBEDTLS_DEBUG_THRESHOLD macro to a value between 0 (no output) to 4 (verbose output),
    - optinally define MBEDTLS_SSL_DEBUG_ALL macro in mbedTLS configuration file or on a project level,
    - and rebuild the project.

## Prepare the Demo
1.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to a router (which must be connect to the internet).
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo
1.  When the demo runs successfully, the terminal will display similar information:

        ***********************************************************
        lwIP HTTPS client example
        ***********************************************************
        Initializing PHY...
        Getting IPv4 address from DHCP...
         IPv4 Address     : 192.168.10.2
         IPv4 Subnet mask : 255.255.255.0
         IPv4 Gateway     : 192.168.10.1
         IPv6 Address0    : -
         IPv6 Address1    : -
         IPv6 Address2    : -
         DNS Server0      : 192.168.10.1
         DNS Server1      : 192.168.10.1
        ***********************************************************

        Resolving host httpbin.org...
        Connecting to 3.223.200.11:443...
        Performing SSL/TLS handshake...
        SSL: Protocol: TLSv1.2, cipher suite: TLS-ECDHE-RSA-WITH-AES-128-GCM-SHA256
        SSL: Record expansion: 29
        SSL: Peer certificate information:
        >>> cert. version     : 3
        >>> serial number     : 0B:D1:35:8F:80:E2:54:A2:DD:CF:A2:09:08:77:03:05
        >>> issuer name       : C=US, O=Amazon, CN=Amazon RSA 2048 M02
        >>> subject name      : CN=httpbin.org
        >>> issued  on        : 2024-08-20 00:00:00
        >>> expires on        : 2025-09-17 23:59:59
        >>> signed using      : RSA with SHA-256
        >>> RSA key size      : 2048 bits
        >>> basic constraints : CA=false
        >>> subject alt name  :
        >>>     dNSName : httpbin.org
        >>>     dNSName : *.httpbin.org
        >>> key usage         : Digital Signature, Key Encipherment
        >>> ext key usage     : TLS Web Server Authentication, TLS Web Client Authentication
        >>> certificate policies : ???

        NetworkCtx: Connection established.
        GET /get

        IPv6 address update, valid addresses:
         IPv6 Address0    : FE80::5627:8DFF:FEED:B45E
         IPv6 Address1    : -
         IPv6 Address2    : -

        Response code: 200
        Response body:
        {
          "args": {},
          "headers": {
            "Host": "httpbin.org",
            "User-Agent": "lwip_https_client_example",
            "X-Amzn-Trace-Id": "Root=1-674de416-2c41247879c1582c304aef20"
          },
          "origin": "46.135.36.251",
          "url": "https://httpbin.org/get"
        }

        NetworkCtx: Connection closed.

## Supported Boards
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/lwip_examples/lwip_https_client/freertos/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/lwip_examples/lwip_https_client/freertos/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/lwip_examples/lwip_https_client/freertos/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/lwip_examples/lwip_https_client/freertos/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/lwip_examples/lwip_https_client/freertos/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/lwip_examples/lwip_https_client/freertos/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/lwip_examples/lwip_https_client/freertos/example_board_readme.md)
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/lwip_examples/lwip_https_client/freertos/example_board_readme.md)
