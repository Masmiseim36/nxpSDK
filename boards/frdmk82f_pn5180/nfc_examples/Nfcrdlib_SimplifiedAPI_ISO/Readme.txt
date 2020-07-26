            ______________________________________________________

               NXPNFCRDLIB EXAMPLE : NFCRDLIB_SIMPLIFIEDAPI_ISO
                                  (V06.01.00)
            ______________________________________________________


Table of Contents
_________________

1 Document Purpose
2 Description of the Nfcrdlib_SimplifiedAPI_ISO
3 Restrictions on Nfcrdlib_SimplifiedAPI_ISO
4 Package Contents
5 Mandatory materials (not included)
6 Hardware Configuration
7 Software Configuration
8 Steps to build Nfcrdlib_SimplifiedAPI_ISO for LPC1769 with PN5180 using MCUXpresso
9 Steps to build Nfcrdlib_SimplifiedAPI_ISO for LPC1769 with RC663 using MCUXpresso
10 Steps to build Nfcrdlib_SimplifiedAPI_ISO for PN7462AU using MCUXpresso
11 Steps to build Nfcrdlib_SimplifiedAPI_ISO for FRDM-K82F using MCUXpresso
12 Selection of Board / OSAL / Frontend (MCUXpresso)
13 Steps to follow for PI / Linux / CMake
14 Running Nfcrdlib_SimplifiedAPI_ISO
15 List of supported NFC Reader Boards/ICs
16 Reference Documents





1 Document Purpose
==================

  This document describes the steps to be followed to execute
  Nfcrdlib_SimplifiedAPI_ISO example as well as it's known problems and
  restrictions.


2 Description of the Nfcrdlib_SimplifiedAPI_ISO
===============================================

  This example is a Reference Application to demonstrate the usage of
  Simplified API with ISO profile. Application contains example of Type A Layer
  4, Type B Layer 4, Mifare UltraLight, Mifare Classic, ISO 15693 and ISO
  18000p3m3.


3 Restrictions on Nfcrdlib_SimplifiedAPI_ISO
============================================

  - Nfcrdlib_SimplifiedAPI_ISO is restricted to be run on NFC Reader Boards/ICs
    mentioned in Section-15.
  - The APIs are intended for NXP contact-less reader ICs only, as such the
    APIs are not to be ported to any technology from any other vendor.
  - NXP will not support porting to any other vendor platform.
  - This software project requires NxpNfcRdLib v06.01.00 or later.
  - **IMPORTANT** The default used MCU Type is LPC1769.  When executing example
    on FRDMK82 or PN7462AU, the MCU Type has to be changed in the MCUXpresso
    IDE.
  - For switching to MCU Type to PN7462AU, see Section-10
  - For switching to MCU Type to FRDMK82, see Section-11


4 Package Contents
==================

  - Readme.txt
    + This readme file
  - Nfcrdlib_SimplifiedAPI_ISO.c
    + Main example file.
  - intfs/Nfcrdlib_SimplifiedAPI_ISO.h
    + Interfaces/Defines specific to the example
  - intfs/ph_NxpBuild_App.h
    + Reader library build configuration file
  - src/Nfcrdlib_SimplifiedApi_ISO_15693.c
  - src/Nfcrdlib_SimplifiedApi_ISO_18000.c
  - src/Nfcrdlib_SimplifiedApi_ISO_Layer4TypeA.c
  - src/Nfcrdlib_SimplifiedApi_ISO_MFC.c
  - src/Nfcrdlib_SimplifiedApi_ISO_MFUL.c
  - src/Nfcrdlib_SimplifiedApi_ISO_TypeB.c
    + Reference Functionality for the component of corresponding protocol
  - intfs/Nfcrdlib_SimplifiedApi_ISO.h
  - intfs/Nfcrdlib_SimplifiedApi_ISO_15693.h
  - intfs/Nfcrdlib_SimplifiedApi_ISO_18000.h
  - intfs/Nfcrdlib_SimplifiedApi_ISO_MFC.h
  - intfs/Nfcrdlib_SimplifiedApi_ISO_MFUL.h
    + Function Macros to create the command for the component of corresponding
      protocol
  - mcux/.cproject and mcux/.project
    + MCUXpresso project configuration file
  - cr_startup_lpc175x_6x.c
    + Startup required to compile example for LPC1769 Micro Controller.


5 Mandatory materials (not included)
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
    Section-15 (Note: PN7462AU (Section-15 Bullet-3) does not need any other
    other microcontroller).


6 Hardware Configuration
========================

  Before starting this application, HW Changes may be required for the used
  board.  Refer to the following User Manuals / Application notes before
  starting with this example.

  - AN11211: Quick Start Up Guide RC663 Blueboard
  - AN11744: PN5180 Evaluation board quick start guide
  - AN11802: NFC Reader Library for Linux Installation Guidelines


7 Software Configuration
========================

  - The Software can be compiled for Cortex M3 LPC1769 micro-controller, Cortex
    M4 Freedom K82 and Cortex M0 based PN7462AU from NXP.
  - Since this example can be configured to run on various MCU ICs and various
    NFC ICs, appropriate changes are required as mentioned in Section-8,
    Section-9, Section-10 and Section-11.


8 Steps to build Nfcrdlib_SimplifiedAPI_ISO for LPC1769 with PN5180 using MCUXpresso
====================================================================================

  See "Importing provided SW example projects" in AN11908 (See Section-16,
  Bullet-2 below)


9 Steps to build Nfcrdlib_SimplifiedAPI_ISO for LPC1769 with RC663 using MCUXpresso
===================================================================================

  See "Importing provided SW example projects" in AN11022 (See Section-16,
  Bullet-3 below)


10 Steps to build Nfcrdlib_SimplifiedAPI_ISO for PN7462AU using MCUXpresso
==========================================================================

  - For MCUXpressio versions before MCUXpressoIDE_10.0.2, See "Adding PN7462AU
    Plugin" in UM10883. (See Section-16, Bullet-4 below)
  - See "Importing provided SW example projects" in UM10883.


11 Steps to build Nfcrdlib_SimplifiedAPI_ISO for FRDM-K82F using MCUXpresso
===========================================================================

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

  The steps are described in AN11802. (See Section-16, Bullet-5 below)


14 Running Nfcrdlib_SimplifiedAPI_ISO
=====================================

  The running application can now be used for EMVCo Test case validation.


15 List of supported NFC Reader Boards/ICs
==========================================

  1) CLEV6630B 2.0 Customer Evaluation Board
  2) PNEV5180B v2.0 Customer Evaluation Board
  3) PN7462AU v2.1 Customer Evaluation Board


16 Reference Documents
======================

  1) UM10954 : PN5180 SW Quick start guide
     [http://www.nxp.com/docs/en/user-guide/UM10954.pdf]

  2) AN11908 : NFC Reader Library for FRDM-K82F Board Installation guidelines
     [http://www.nxp.com/docs/en/application-note/AN11908.pdf]

  3) AN11022 : CLRC663 Evaluation board quick start guide
     [http://www.nxp.com/docs/en/application-note/AN11022.pdf]

  4) UM10883 : PN7462AU Quick Start Guide - Development Kit
     [http://www.nxp.com/docs/en/user-guide/UM10883.pdf]

  5) AN11802 : NFC Reader Library for Linux Installation Guidelines
     [http://www.nxp.com/docs/en/application-note/AN11802.pdf]

  -----------------------------------------------------------------------------

  For updates of this example, see
  [http://www.nxp.com/products/:NFC-READER-LIBRARY]
