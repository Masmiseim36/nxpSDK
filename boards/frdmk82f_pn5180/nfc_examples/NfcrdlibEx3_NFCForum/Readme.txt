            ______________________________________________________

                  NXPNFCRDLIB EXAMPLE : NFCRDLIBEX3_NFCFORUM
                                  (V06.01.00)
            ______________________________________________________


Table of Contents
_________________

1 Document Purpose
2 Description of the NfcrdlibEx3_NFCForum
3 Restrictions on NfcrdlibEx3_NFCForum
4 Configurations of NfcrdlibEx3_NFCForum
5 Package Contents
6 Mandatory materials (not included)
7 Hardware Configuration
8 Software Configuration
9 Steps to build NfcrdlibEx3_NFCForum for LPC1769 with PN5180 using MCUXpresso
10 Steps to build NfcrdlibEx3_NFCForum for PN7462AU using MCUXpresso
11 Steps to build NfcrdlibEx3_NFCForum for FRDM-K82F using MCUXpresso
12 Selection of Board / OSAL / Frontend (MCUXpresso)
13 Steps to follow for PI / Linux / CMake
14 Running NfcrdlibEx3_NFCForum
15 List of supported NFC Reader Boards/ICs
16 Reference Documents





1 Document Purpose
==================

  This document describes the steps to be followed to execute
  NfcrdlibEx3_NFCForum example as well as it's known problems and restrictions.


2 Description of the NfcrdlibEx3_NFCForum
=========================================

  - NfcrdlibEx3_NFCForum can be run with PN5180 and PN7462AU.  (See Section-15
    for supported versions).
  - This example will demonstrate both Peer to Peer (P2P) and Reader functions
    to detect type A, type B, type F tags, type V tags and 18000p3m3 tags.
  - If any P2P device is brought in proximity then this example will go into
    P2P Mode.
  - When SNEP Client is enabled then this example performs SNEP PUT onto the
    Peer device.
  - By default the configured SNEP PUT message is "www.nxp.com".
  - To demonstrate SNEP Server, the SNEP Client needs to be disabled as no
    control can be provided with boards as incase of mobiles (Just like the
    Touch-to-beam feature on Mobiles).
  - The maximum length of SNEP PUT from peer device that can be received by
    SNEP Server with this example is 1KB (1024 Bytes).
  - If any NFC cards/devices (Type A, Type B or Type F) are brought in
    proximity then this example will behave as a reader and will detect and
    print the NFC technology type detected along with NDEF message from the tag
    if present.


3 Restrictions on NfcrdlibEx3_NFCForum
======================================

  - NfcrdlibEx3_NFCForum is restricted to be run on NFC Reader Boards/ICs
    mentioned in Section-15.
  - For PN7462AU (Section-15 Bullet-2) either of SNEP Client or SNEP Sever can
    be enabled at a time (See Section-8).
  - For PN7462AU (Section-15 Bullet-2) the project should be build only with
    Release configuration or Debug Configuration with semi hosting disabled.
  - The APIs are intended for NXP contact-less reader ICs only, as such the
    APIs are not to be ported to any technology from any other vendor.
  - NXP will not support porting to any other vendor platform.
  - This software project requires NxpNfcRdLib v06.01.00 or later.
  - **IMPORTANT** The default used MCU Type is LPC1769.  When executing example
    on FRDMK82 or PN7462AU, the MCU Type has to be changed in the MCUXpresso
    IDE.
  - For switching to MCU Type to PN7462AU, see Section-10
  - For switching to MCU Type to FRDMK82, see Section-11


4 Configurations of NfcrdlibEx3_NFCForum
========================================

  - The example application is written to detect and activate one tag of
    supported technologies.
  - If switching between supported NFC Reader boards mention in 15, user is
    adviced to include the appropriate define in file mentioned in Section-,
    Bullet-, Bullet-, Bullet-.


5 Package Contents
==================

  - Readme.txt
    + This readme file
  - NfcrdlibEx3_NFCForum.c
    + Main example file.
  - intfs/NfcrdlibEx3_NFCForum.h
    + Interfaces/Defines specific to the example
  - intfs/ph_NxpBuild_App.h
    + Reader library build configuration file
  - src/phApp_Init.c and intfs/phApp_Init.h
    + Common utility functions (common across all examples)
  - src/cards.c and intfs/cards.h
    + Common utility functions and APIs for cards
  - mcux/.cproject and mcux/.project
    + MCUXpresso project configuration file
  - cr_startup_lpc175x_6x.c
    + Startup required to compile example for LPC1769 Micro Controller.


6 Mandatory materials (not included)
====================================

  - MCUXpresso IDE. It can be downloaded from
    [http://www.nxp.com/products/:MCUXpresso-IDE]
  - SDK for Freedom K82 Board ([http://www.nxp.com/products/:FRDM-K82F]).  The
    pre-build SDK can be downloaded from
    [https://mcuxpresso.nxp.com/en/license?hash=9897a8c19a6bc569c3fade7141f0f405&hash_download=true&to_vault=true]
    See MCUXpresso User Manual for steps needed to install an SDK.
  - Plugin to extend MCUXpresso to support PN7462AU. (Required for MCUXpress
    versions before MCUXpressoIDE_10.0.2)

  - LPCXpresso LPC1769 / Freedom K82 development boards For NFC ICs listed in
    Section-15 (Note: PN7462AU (Section-15 Bullet-2) does not need any other
    other microcontroller).


7 Hardware Configuration
========================

  Before starting this application, HW Changes may be required for the used
  board.  Refer to the following User Manuals / Application notes before
  starting with this example.

  - AN11744: PN5180 Evaluation board quick start guide
  - AN11802: NFC Reader Library for Linux Installation Guidelines


8 Software Configuration
========================

  - The Software can be compiled for Cortex M3 LPC1769 micro-controller, Cortex
    M4 Freedom K82 and Cortex M0 based PN7462AU from NXP.
  - Since this example can be configured to run on various MCU ICs and various
    NFC ICs, appropriate changes are required as mentioned in Section-9,
    Section-10 and Section-11.
  - Only reader library layers mentioned in the header file ph_NxpBuild_App.h
    are enabled in build for this project.
  - NfcrdlibEx3_NFCForum itself can be configured for the following items:

    Listen Mode  : In discovery loop listen mode, active listen or passive
                   listen can be selectively enabled/disabled by defining
                   "ACTIVE_MODE" and "PASSIVE_MODE" respectively.
    Poll Mode    : In discovery loop poll mode, active poll or passive poll can
                   be selectively enabled/disabled by defining "ACTIVE_MODE"
                   and "PASSIVE_MODE" respectively.
    Active Mode  : To enable Active Mode. If Disabled both Active Poll and
                   Active Listen discovery loop configuration will be disabled.
    Passive Mode : To enable Passive Mode. If Disabled both Passive Poll and
                   Passive Listen discovery loop configuration will be
                   disabled.
    SNEP Client  : To enable SNEP Client functionality i.e. to perform SNEP PUT
                   onto remote peer device (Mobile/Board).
    SNEP Server  : To enable SNEP Server functionality i.e. to send SNEP PUT
                   Message from remote peer device (Mobile/Board).


9 Steps to build NfcrdlibEx3_NFCForum for LPC1769 with PN5180 using MCUXpresso
==============================================================================

  See "Importing provided SW example projects" in AN11908 (See Section-16,
  Bullet-2 below)


10 Steps to build NfcrdlibEx3_NFCForum for PN7462AU using MCUXpresso
====================================================================

  - For MCUXpressio versions before MCUXpressoIDE_10.0.2, See "Adding PN7462AU
    Plugin" in UM10883. (See Section-16, Bullet-3 below)
  - See "Importing provided SW example projects" in UM10883.


11 Steps to build NfcrdlibEx3_NFCForum for FRDM-K82F using MCUXpresso
=====================================================================

  See "Import projects to the MCUXpresso" in AN11908. (See Section-16, Bullet-2
  below)


12 Selection of Board / OSAL / Frontend (MCUXpresso)
====================================================

  For MCUXpresso, the selection of Board / OSAL / Frontend has to be done via
  -D (Preprocessor defines).

  1) To select the board go to "Project Properties" --> "C/C++ Build" -->
     "Settings" --> "Preprocessor" --> "Defined symbols (-D)", and define the
     relevant PHDRIVER_<BoardNFCCombination>_BOARD macro.

     e.g. For using LPC1769 with Pn5180 use PHDRIVER_LPC1769PN5180_BOARD.  For
     list of supported boards refer to Platform\DAL\cfg\BoardSelection.h.

  2) To select the osal/os type, go to "Project Properties" --> "C/C++ Build"
     --> "Settings" --> "Preprocessor" --> "Defined symbols (-D)".

     e.g. For using FreeRTOS use PH_OSAL_FREERTOS. For other options refer to
     RTOS\phOsal\inc\phOsal_Config.h.


13 Steps to follow for PI / Linux / CMake
=========================================

  The steps are described in AN11802. (See Section-16, Bullet-4 below)


14 Running NfcrdlibEx3_NFCForum
===============================

  When the application is running:

  1) If any NFC cards/devices (Type A, Type B or Type F) are brought in
     proximity then this example will behave as a reader and will detect and
     report the NFC technology types detected along with NDEF message from the
     tag if present.
  2) If any P2P device is brought in proximity then this example will
     dynamically switch between Initiator or Target mode and performs SNEP PUT
     if both Poll and Listen Modes are enabled. If SNEP Client is disabled then
     Peer P2P device can perform SNEP PUT operation which will be received by
     this example.


15 List of supported NFC Reader Boards/ICs
==========================================

  1) PNEV5180B v2.0 Customer Evaluation Board
  2) PN7462AU v2.1 Customer Evaluation Board


16 Reference Documents
======================

  1) UM10954 : PN5180 SW Quick start guide
     [http://www.nxp.com/docs/en/user-guide/UM10954.pdf]

  2) AN11908 : NFC Reader Library for FRDM-K82F Board Installation guidelines
     [http://www.nxp.com/docs/en/application-note/AN11908.pdf]

  3) UM10883 : PN7462AU Quick Start Guide - Development Kit
     [http://www.nxp.com/docs/en/user-guide/UM10883.pdf]

  4) AN11802 : NFC Reader Library for Linux Installation Guidelines
     [http://www.nxp.com/docs/en/application-note/AN11802.pdf]


  -----------------------------------------------------------------------------

  For updates of this example, see
  [http://www.nxp.com/products/:NFC-READER-LIBRARY]
