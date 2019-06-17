Overview
========

The lwip_https_client_mbedTLS demo application demonstrates an HTTPS client set up on lwIP TCP/IP and the MbedTLS stack with
FreeRTOS. The board acts as an HTTP client and sends a request to the HTTPServer.

Demo requires DHCP and DNS servers. The address of the DNS server is obtained from DHCP.


Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to a computer network which has a DHCP server and access to the internet.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the terminal will display the following:

Initializing PHY...

Getting IP address from DHCP ...


 IPv4 Address     : 10.171.65.94

DHCP OK
  . Seeding the random number generator...  . Loading the CA root certificate ... ok (0 skipped)
  . Loading the client cert. and key... ok
  . C
onnecting to os.mbed.org/443...  . Setting up the SSL/TLS structure...

SSL state connect : 0  ok


SSL state connect : 0   . Performing the SSL/TLS
 handshake...
Verify requested for (Depth 2):
cert. version     : 3
serial number     : 04:00:00:00:00:01:15:4B:5A:C3:94
issuer name       : C=BE, O=Globa
lSign nv-sa, OU=Root CA, CN=GlobalSign Root CA
subject name      : C=BE, O=GlobalSign nv-sa, OU=Root CA, CN=GlobalSign Root CA
issued  on        : 1998-
09-01 12:00:00
expires on        : 2028-01-28 12:00:00
signed using      : RSA with SHA1
RSA key size      : 2048 bits
basic constraints : CA=true
key usag
e         : Key Cert Sign, CRL Sign
cert. version     : 3
serial number     : 04:00:00:00:00:01:15:4B:5A:C3:94
issuer name       : C=BE, O=GlobalSign nv-
sa, OU=Root CA, CN=GlobalSign Root CA
subject name      : C=BE, O=GlobalSign nv-sa, OU=Root CA, CN=GlobalSign Root CA
issued  on        : 1998-09-01 12:
00:00
expires on        : 2028-01-28 12:00:00
signed using      : RSA with SHA1
RSA key size      : 2048 bits
basic constraints : CA=true
key usage
 : Key Cert Sign, CRL Sign
cert. version     : 3
serial number     : 04:00:00:00:00:01:15:4B:5A:C3:94
issuer name       : C=BE, O=GlobalSign nv-sa, OU=Ro
ot CA, CN=GlobalSign Root CA
subject name      : C=BE, O=GlobalSign nv-sa, OU=Root CA, CN=GlobalSign Root CA
issued  on        : 1998-09-01 12:00:00
expi
res on        : 2028-01-28 12:00:00
signed using      : RSA with SHA1
RSA key size      : 2048 bits
basic constraints : CA=true
key usage         : Key Ce
rt Sign, CRL Sign


Verify requested for (Depth 1):
cert. version     : 3
serial number     : 04:00:00:00:00:01:44:4E:F0:42:47
issuer name       : C=BE, O=G
lobalSign nv-sa, OU=Root CA, CN=GlobalSign Root CA
subject name      : C=BE, O=GlobalSign nv-sa, CN=GlobalSign Organization Validation CA - SHA256 - G2

issued  on        : 2014-02-20 10:00:00
expires on        : 2024-02-20 10:00:00
signed using      : RSA with SHA-256
RSA key size      : 2048 bits
basic c
onstraints : CA=true, max_pathlen=0
key usage         : Key Cert Sign, CRL Sign
  This certificate has no flags

Verify requested for (Depth 0):
cert. vers
ion     : 3
serial number     : 65:7B:6D:8D:15:A5:B6:86:87:6B:5E:BC
issuer name       : C=BE, O=GlobalSign nv-sa, CN=GlobalSign Organization Validation
CA - SHA256 - G2
subject name      : C=GB, ST=Cambridgeshire, L=Cambridge, O=ARM Ltd, CN=*.mbed.com
issued  on        : 2017-04-03 13:54:02
expires on
     : 2018-05-06 10:31:02
signed using      : RSA with SHA-256
RSA key size      : 2048 bits
basic constraints : CA=false
subject alt name  : *.mbed.com,
 mbed.org, *.mbed.org, mbed.com
key usage         : Digital Signature, Key Encipherment
ext key usage     : TLS Web Server Authentication, TLS Web Client Authentication
  This certificate has no flags
 ok
    [ Protocol is TLSv1.2 ]
    [ Ciphersuite is TLS-ECDHE-RSA-WITH-AES-128-GCM-SHA256 ]
    [ Record
expansion is 29 ]
  . Verifying peer X.509 certificate... Server Verification skipped
  > Write to server: 55 bytes written

GET /media/uploads/mbed_official/hello.txt HTTP/1.0



  < Read from server: 433 bytes read

HTTP/1.1 200 OK

Server: nginx/1.11.12

Date: Thu, 07 Sep 2017 13:53:39 GMT

Content-Type: text/plain

Content-Length: 14

Connection: close

Last-Modified: Fri, 27 Jul 2012 13:30:34 GMT

Accept-Ranges: bytes

Cache-Control: max-age=36000

Expires: Thu, 07 Sep 2017 23:53:39 GMT

X-Upstream-L3: 172.17.0.4:80

X-Upstream-L2: developer-sjc-cyan-2-nginx

Strict-Transport-Security: max-age=31536000; includeSubdomains



Hello world!

Customization options
=====================

