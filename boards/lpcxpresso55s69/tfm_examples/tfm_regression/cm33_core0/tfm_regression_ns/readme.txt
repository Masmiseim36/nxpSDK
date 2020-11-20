Overview
========
The Trusted Firmware M (TF-M) regression test application provides testing of the TF-M core and its RoT services:
 - Secure Storage Service
 - Attestation Service
 - Crypto Service
 - TF-M Audit Log Service
 - Platform Service

Toolchain supported
===================
- GCC ARM Embedded  9.2.1
- Keil MDK  5.31
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55s69 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (P6) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Use secure project to download the program to target board
    For IAR environment go to project options/Debugger/Images. Open file dialog for first extra image. Select file <application_name>_ns.out and click on open button. 
    Now you have absolute path in the path box. The flash loader requires absolute path to this file.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the TFM regression tests in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[Sec Thread] Secure image initializing!
Booting TFM v1.0

#### Execute test suites for the Secure area ####
Running Test Suite PSA protected storage S interface tests (TFM_SST_TEST_2XXX)...
> Executing 'TFM_SST_TEST_2001'
  Description: 'Set interface'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2002'
  Description: 'Set interface with create flags'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2003'
  Description: 'Set interface with NULL data pointer'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2004'
  Description: 'Set interface with invalid data length'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2005'
  Description: 'Set interface with write once UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2006'
  Description: 'Get interface with valid data'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2007'
  Description: 'Get interface with zero data length'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2008'
  Description: 'Get interface with invalid UIDs'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2009'
  Description: 'Get interface with invalid data lengths and offsets'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2010'
  Description: 'Get interface with NULL data pointer'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2011'
  Description: 'Get info interface with write once UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2012'
  Description: 'Get info interface with valid UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2013'
  Description: 'Get info interface with invalid UIDs'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2014'
  Description: 'Get info interface with NULL info pointer'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2015'
  Description: 'Remove interface with valid UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2016'
  Description: 'Remove interface with write once UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2017'
  Description: 'Remove interface with invalid UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2018'
  Description: 'Block compaction after remove'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2019'
  Description: 'Multiple partial gets'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2020'
  Description: 'Multiple sets to same UID from same thread'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2021'
  Description: 'Get support interface'
  TEST PASSED!
> Executing 'TFM_SST_TEST_2022'
  Description: 'Set, get and remove interface with different asset sizes'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite SST reliability tests (TFM_SST_TEST_3XXX)...
> Executing 'TFM_SST_TEST_3001'
  Description: 'repetitive sets and gets in/from an asset'
  > Iteration 15 of 15
  TEST PASSED!
> Executing 'TFM_SST_TEST_3002'
  Description: 'repetitive sets, gets and removes'
  > Iteration 15 of 15
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite SST rollback protection tests (TFM_SST_TEST_4XXX)...
> Executing 'TFM_SST_TEST_4001'
  Description: 'Check SST area version when NV counters 1/2/3 have the same value'
  TEST PASSED!
> Executing 'TFM_SST_TEST_4002'
  Description: 'Check SST area version when it is different from NV counters 1/2/3'
  TEST PASSED!
> Executing 'TFM_SST_TEST_4003'
  Description: 'Check SST area version when NV counters 1 and 2 are equals, 3 is different, and SST area version match NV counters 1 and 2'
  TEST PASSED!
> Executing 'TFM_SST_TEST_4004'
  Description: 'Check SST area version when NV counters 2 and 3 are equals, 1 is different and SST area version match NV counter 2 and 3'
  TEST PASSED!
> Executing 'TFM_SST_TEST_4005'
  Description: 'Check SST area version when NV counters 2 and 3 are equals, 1 is different and SST area version match NV counter 1'
  TEST PASSED!
> Executing 'TFM_SST_TEST_4006'
  Description: 'Check SST area version when NV counters 1, 2 and 3 have different values and SST area version match NV counter 1 value'
  TEST PASSED!
> Executing 'TFM_SST_TEST_4007'
  Description: 'Check SST area version when NV counters 1, 2 and 3 have different values and SST area version match NV counter 2 value'
  TEST PASSED!
> Executing 'TFM_SST_TEST_4008'
  Description: 'Check SST area version when NV counters 1, 2 and 3 have different values and SST area version match NV counter 3 value'
  TEST PASSED!
> Executing 'TFM_SST_TEST_4009'
  Description: 'Check SST area version when NV counter 1 cannot be incremented'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite PSA internal trusted storage S interface tests (TFM_ITS_TEST_2XXX)...
> Executing 'TFM_ITS_TEST_2001'
  Description: 'Set interface'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2002'
  Description: 'Set interface with create flags'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2003'
  Description: 'Set interface with NULL data pointer'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2004'
  Description: 'Set interface with write once UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2005'
  Description: 'Get interface with valid data'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2006'
  Description: 'Get interface with zero data length'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2007'
  Description: 'Get interface with invalid UIDs'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2008'
  Description: 'Get interface with data lengths and offsets greater than UID length'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2009'
  Description: 'Get interface with NULL data pointer'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2010'
  Description: 'Get info interface with write once UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2011'
  Description: 'Get info interface with valid UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2012'
  Description: 'Get info interface with invalid UIDs'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2013'
  Description: 'Remove interface with valid UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2014'
  Description: 'Remove interface with write once UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2015'
  Description: 'Remove interface with invalid UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2016'
  Description: 'Block compaction after remove'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2017'
  Description: 'Multiple partial gets'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2018'
  Description: 'Multiple sets to same UID from same thread'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2019'
  Description: 'Set, get and remove interface with different asset sizes'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2020'
  Description: 'Set interface with invalid data length'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2021'
  Description: 'Get interface with invalid data lengths and offsets'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2022'
  Description: 'Get info interface with NULL info pointer'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_2023'
  Description: 'Attempt to get a UID set by a different partition'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite ITS reliability tests (TFM_ITS_TEST_3XXX)...
> Executing 'TFM_ITS_TEST_3001'
  Description: 'repetitive sets and gets in/from an asset'
  > Iteration 15 of 15
  TEST PASSED!
> Executing 'TFM_ITS_TEST_3002'
  Description: 'repetitive sets, gets and removes'
  > Iteration 15 of 15
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite Crypto secure interface tests (TFM_CRYPTO_TEST_5XXX)...
> Executing 'TFM_CRYPTO_TEST_5001'
  Description: 'Secure Key management interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5002'
  Description: 'Secure Symmetric encryption (AES-128-CBC) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5003'
  Description: 'Secure Symmetric encryption (AES-128-CFB) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5005'
  Description: 'Secure Symmetric encryption (AES-128-CTR) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5007'
  Description: 'Secure Symmetric encryption invalid cipher (AES-128-GCM)'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5008'
  Description: 'Secure Symmetric encryption invalid cipher (AES-152-CBC)'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5009'
  Description: 'Secure Symmetric encryption invalid cipher (HMAC-128-CFB)'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5010'
  Description: 'Secure Unsupported Hash (SHA-1) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5011'
  Description: 'Secure Hash (SHA-224) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5012'
  Description: 'Secure Hash (SHA-256) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5013'
  Description: 'Secure Hash (SHA-384) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5014'
  Description: 'Secure Hash (SHA-512) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5019'
  Description: 'Secure Unsupported HMAC (SHA-1) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5020'
  Description: 'Secure HMAC (SHA-256) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5021'
  Description: 'Secure HMAC (SHA-384) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5022'
  Description: 'Secure HMAC (SHA-512) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5024'
  Description: 'Secure HMAC with long key (SHA-224) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5030'
  Description: 'Secure AEAD (AES-128-CCM) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5031'
  Description: 'Secure AEAD (AES-128-GCM) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5032'
  Description: 'Secure key policy interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5033'
  Description: 'Secure key policy check permissions'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_5034'
  Description: 'Key access control'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite Initial Attestation Service secure interface tests(TFM_ATTEST_TEST_1XXX)...
> Executing 'TFM_ATTEST_TEST_1004'
  Description: 'ECDSA signature test of attest token'
  TEST PASSED!
> Executing 'TFM_ATTEST_TEST_1005'
  Description: 'Negative test cases for initial attestation service'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite Platform Service Secure interface tests(TFM_PLATFORM_TEST_1XXX)...
> Executing 'TFM_PLATFORM_TEST_1001'
  Description: 'Minimal platform service test'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite IPC secure interface test (TFM_IPC_TEST_1XXX)...
> Executing 'TFM_IPC_TEST_1001'
  Description: 'Secure functional'
  TEST PASSED!
TESTSUITE PASSED!

*** Secure test suites summary ***
Test suite 'PSA protected storage S interface tests (TFM_SST_TEST_2XXX)' has  PASSED
Test suite 'SST reliability tests (TFM_SST_TEST_3XXX)' has  PASSED
Test suite 'SST rollback protection tests (TFM_SST_TEST_4XXX)' has  PASSED
Test suite 'PSA internal trusted storage S interface tests (TFM_ITS_TEST_2XXX)' has  PASSED
Test suite 'ITS reliability tests (TFM_ITS_TEST_3XXX)' has  PASSED
Test suite 'Crypto secure interface tests (TFM_CRYPTO_TEST_5XXX)' has  PASSED
Test suite 'Initial Attestation Service secure interface tests(TFM_ATTEST_TEST_1XXX)' has  PASSED
Test suite 'Platform Service Secure interface tests(TFM_PLATFORM_TEST_1XXX)' has  PASSED
Test suite 'IPC secure interface test (TFM_IPC_TEST_1XXX)' has  PASSED

*** End of Secure test suites ***

#### Execute test suites for the Non-secure area ####
Running Test Suite PSA protected storage NS interface tests (TFM_SST_TEST_1XXX)...
> Executing 'TFM_SST_TEST_1001'
  Description: 'Set interface'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1002'
  Description: 'Set interface with create flags'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1003'
  Description: 'Set interface with NULL data pointer'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1004'
  Description: 'Set interface with write once UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1005'
  Description: 'Get interface with valid data'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1006'
  Description: 'Get interface with zero data length'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1007'
  Description: 'Get interface with invalid UIDs'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1008'
  Description: 'Get interface with invalid data lengths and offsets'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1009'
  Description: 'Get interface with NULL data pointer'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1010'
  Description: 'Get info interface with write once UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1011'
  Description: 'Get info interface with valid UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1012'
  Description: 'Get info interface with invalid UIDs'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1013'
  Description: 'Remove interface with valid UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1014'
  Description: 'Remove interface with write once UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1015'
  Description: 'Remove interface with invalid UID'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1021'
  Description: 'Block compaction after remove'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1022'
  Description: 'Multiple partial gets'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1023'
  Description: 'Multiple sets to same UID from same thread'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1024'
  Description: 'Get support interface'
  TEST PASSED!
> Executing 'TFM_SST_TEST_1025'
  Description: 'Set, get and remove interface with different asset sizes'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite PSA internal trusted storage NS interface tests (TFM_ITS_TEST_1XXX)...
> Executing 'TFM_ITS_TEST_1001'
  Description: 'Set interface'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1002'
  Description: 'Set interface with create flags'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1003'
  Description: 'Set interface with NULL data pointer'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1004'
  Description: 'Set interface with write once UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1005'
  Description: 'Get interface with valid data'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1006'
  Description: 'Get interface with zero data length'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1007'
  Description: 'Get interface with invalid UIDs'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1008'
  Description: 'Get interface with invalid data lengths and offsets'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1009'
  Description: 'Get interface with NULL data pointer'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1010'
  Description: 'Get info interface with write once UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1011'
  Description: 'Get info interface with valid UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1012'
  Description: 'Get info interface with invalid UIDs'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1013'
  Description: 'Remove interface with valid UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1014'
  Description: 'Remove interface with write once UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1015'
  Description: 'Remove interface with invalid UID'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1016'
  Description: 'Block compaction after remove'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1017'
  Description: 'Multiple partial gets'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1018'
  Description: 'Multiple sets to same UID from same thread'
  TEST PASSED!
> Executing 'TFM_ITS_TEST_1019'
  Description: 'Set, get and remove interface with different asset sizes'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite Crypto non-secure interface test (TFM_CRYPTO_TEST_6XXX)...
> Executing 'TFM_CRYPTO_TEST_6001'
  Description: 'Non Secure Key management interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6002'
  Description: 'Non Secure Symmetric encryption (AES-128-CBC) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6003'
  Description: 'Non Secure Symmetric encryption (AES-128-CFB) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6005'
  Description: 'Non Secure Symmetric encryption (AES-128-CTR) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6007'
  Description: 'Non Secure Symmetric encryption invalid cipher (AES-128-GCM)'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6008'
  Description: 'Non Secure Symmetric encryption invalid cipher (AES-152-CBC)'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6009'
  Description: 'Non Secure Symmetric encryption invalid cipher (HMAC-128-CFB)'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6010'
  Description: 'Non Secure Unsupported Hash (SHA-1) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6011'
  Description: 'Non Secure Hash (SHA-224) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6012'
  Description: 'Non Secure Hash (SHA-256) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6013'
  Description: 'Non Secure Hash (SHA-384) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6014'
  Description: 'Non Secure Hash (SHA-512) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6019'
  Description: 'Non Secure Unsupported HMAC (SHA-1) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6020'
  Description: 'Non Secure HMAC (SHA-256) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6021'
  Description: 'Non Secure HMAC (SHA-384) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6022'
  Description: 'Non Secure HMAC (SHA-512) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6024'
  Description: 'Non Secure HMAC with long key (SHA-224) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6030'
  Description: 'Non Secure AEAD (AES-128-CCM) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6031'
  Description: 'Non Secure AEAD (AES-128-GCM) interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6032'
  Description: 'Non Secure key policy interface'
  TEST PASSED!
> Executing 'TFM_CRYPTO_TEST_6033'
  Description: 'Non Secure key policy check permissions'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite Initial Attestation Service non-secure interface tests(TFM_ATTEST_TEST_2XXX)...
> Executing 'TFM_ATTEST_TEST_2004'
  Description: 'ECDSA signature test of attest token'
  TEST PASSED!
> Executing 'TFM_ATTEST_TEST_2005'
  Description: 'Negative test cases for initial attestation service'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite Platform Service Non-Secure interface tests(TFM_PLATFORM_TEST_2XXX)...
> Executing 'TFM_PLATFORM_TEST_2001'
  Description: 'Minimal platform service test'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite QCBOR regression test(TFM_QCBOR_TEST_7XXX)...
> Executing 'TFM_QCBOR_TEST_7001'
  Description: 'Regression test of QCBOR library'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite T_COSE regression test(TFM_T_COSE_TEST_8XXX)...
> Executing 'TFM_T_COSE_TEST_8001'
  Description: 'Regression test of t_cose library'
  TEST PASSED!
TESTSUITE PASSED!
Running Test Suite IPC non-secure interface test (TFM_IPC_TEST_1XXX)...
> Executing 'TFM_IPC_TEST_1001'
  Description: 'Get PSA framework version'
The version of the PSA Framework API is 256.
  TEST PASSED!
> Executing 'TFM_IPC_TEST_1002'
  Description: 'Get version of an RoT Service'
The service version is 1.
  TEST PASSED!
> Executing 'TFM_IPC_TEST_1003'
  Description: 'Connect to an RoT Service'
Connect success!
  TEST PASSED!
> Executing 'TFM_IPC_TEST_1004'
  Description: 'Call an RoT Service'
TFM service support version is 1.
psa_call is successful!
outvec1 is: It is just for IPC call test.
outvec2 is: It is just for IPC call test.
  TEST PASSED!
> Executing 'TFM_IPC_TEST_1005'
  Description: 'Call IPC_INIT_BASIC_TEST service'
Connect success!Call success!  TEST PASSED!
> Executing 'TFM_IPC_TEST_1006'
  Description: 'Call PSA RoT access APP RoT memory test service'
Connect success!Call success!  TEST PASSED!
> Executing 'TFM_IPC_TEST_1010'
  Description: 'Test psa_call with the status of PSA_ERROR_PROGRAMMER_ERROR'
Connect success!
The first time call success!
The second time call success!
  TEST PASSED!
TESTSUITE PASSED!

*** Non-secure test suites summary ***
Test suite 'PSA protected storage NS interface tests (TFM_SST_TEST_1XXX)' has  PASSED
Test suite 'PSA internal trusted storage NS interface tests (TFM_ITS_TEST_1XXX)' has  PASSED
Test suite 'Crypto non-secure interface test (TFM_CRYPTO_TEST_6XXX)' has  PASSED
Test suite 'Initial Attestation Service non-secure interface tests(TFM_ATTEST_TEST_2XXX)' has  PASSED
Test suite 'Platform Service Non-Secure interface tests(TFM_PLATFORM_TEST_2XXX)' has  PASSED
Test suite 'QCBOR regression test(TFM_QCBOR_TEST_7XXX)' has  PASSED
Test suite 'T_COSE regression test(TFM_T_COSE_TEST_8XXX)' has  PASSED
Test suite 'IPC non-secure interface test (TFM_IPC_TEST_1XXX)' has  PASSED

*** End of Non-secure test suites ***

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

