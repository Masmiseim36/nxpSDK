Overview
========

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
        $ nmap --script ssl-enum-ciphers -p 443 http.minori.work
        Starting Nmap 7.80 ( https://nmap.org ) at 2024-05-17 15:38 CEST
        Nmap scan report for http.minori.work (54.178.197.123)
        Host is up (0.27s latency).
        Other addresses for http.minori.work (not scanned): 2406:da14:457:c100:2afe:a322:3abb:97d0
        rDNS record for 54.178.197.123: ec2-54-178-197-123.ap-northeast-1.compute.amazonaws.com

        PORT    STATE SERVICE
        443/tcp open  https
        | ssl-enum-ciphers:
        |   TLSv1.2:
        |     ciphers:
        |       TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 (secp256r1) - A
        |       TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 (secp256r1) - A
        |       TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 (secp256r1) - A
        |     compressors:
        |       NULL
        |     cipher preference: client
        |_  least strength: A

        Nmap done: 1 IP address (1 host up) scanned in 8.01 seconds

The output of mbedTLS debug messages to the console can be enabled by the following:
    - define MBEDTLS_DEBUG_C macro in mbedTLS configuration file or on a project level,
    - redefine MBEDTLS_DEBUG_THRESHOLD macro to a value between 0 (no output) to 4 (verbose output),
    - optinally define MBEDTLS_SSL_DEBUG_ALL macro in mbedTLS configuration file or on a project level,
    - and rebuild the project.


SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- IAR embedded Workbench  9.60.1
- GCC ARM Embedded  13.2.1
- Keil MDK  5.39.0
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- MIMXRT1180-EVK board
- Router or other device with DHCP server capability and internet connectivity.
- Personal Computer

Board settings
==============
This example uses Ethernet RJ45 port J32.
Prepare the Demo
================
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

Running the demo
================
1.  When the demo runs successfully, the terminal will display similar information:
        ***********************************************************
        lwIP HTTPS client example
        ***********************************************************
        Initializing PHY...
        Getting IPv4 address from DHCP...
         IPv4 Address     : 192.168.10.3
         IPv4 Subnet mask : 255.255.255.0
         IPv4 Gateway     : 192.168.10.1
         IPv6 Address0    : -
         IPv6 Address1    : -
         IPv6 Address2    : -
         DNS Server0      : 192.168.10.1
         DNS Server1      : -
        ***********************************************************

        Resolving host http.minori.work...
        Connecting to 54.178.197.123:443...
        Performing SSL/TLS handshake...

        IPv6 address update, valid addresses:
         IPv6 Address0    : FE80::5627:8DFF:FE38:B002
         IPv6 Address1    : -
         IPv6 Address2    : -

        SSL: Protocol: TLSv1.2, cipher suite: TLS-ECDHE-ECDSA-WITH-AES-256-GCM-SHA384
        SSL: Record expansion: 29
        SSL: Peer certificate information:
        >>> cert. version     : 3
        >>> serial number     : 03:20:66:38:93:98:D2:C3:63:CC:94:20:D1:9C:76:1E:C3:2C
        >>> issuer name       : C=US, O=Let's Encrypt, CN=R3
        >>> subject name      : CN=http.minori.work
        >>> issued  on        : 2024-03-18 06:27:51
        >>> expires on        : 2024-06-16 06:27:50
        >>> signed using      : RSA with SHA-256
        >>> EC key size       : 256 bits
        >>> basic constraints : CA=false
        >>> subject alt name  :
        >>>     dNSName : http.minori.work
        >>> key usage         : Digital Signature
        >>> ext key usage     : TLS Web Server Authentication, TLS Web Client Authentication
        >>> certificate policies : ???

        NetworkCtx: Connection established.
        GET /get
        Response code: 200
        Response body:
        {
          "args": {},
          "headers": {
            "Connection": "Upgrade",
            "Host": "http.minori.work",
            "User-Agent": "lwip_https_client_example"
          },
          "origin": "46.135.105.141",
          "url": "https://http.minori.work/get"
        }

        NetworkCtx: Connection closed.
