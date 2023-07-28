..
    Copyright 2022 NXP


.. highlight:: bat

.. _el2go_usage_examples:

=======================================================================
 SE05X EdgeLock 2GO Agent example
=======================================================================

This demo demonstrates how to use the EdgeLock 2GO service for provisioning keys and certificates into the SE05x secure element.
Those keys and certificates can then be used to establish mutual-authenticated TLS connections to cloud services such as AWS or Azure.

Prerequisites
=======================================================================
- Active EdgeLock 2GO account (https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO)
- Any Serial communicator


Setup of the EdgeLock 2GO platform
===========================================================================
The documentation which explains how to setup the EdgeLock 2GO Account to
- Add devices to the group
- Create Secure Object
- Assign Secure Objects to device
can be found under the EdgeLock 2GO account under the Documentation tab.


Building the Demo
=======================================================================
Before you start building the demo you must configure the EdgeLock 2GO URL for your account:
- From the EdgeLock 2GO account copy the account specific URL (in Admin Settings section)
- Open file middleware/se_hostlib/nxp_iot_agent/inc/nxp_iot_agent_config.h
- Fill the definition EDGELOCK2GO_HOSTNAME with the account specific URL.

To build for your platform follow your board specific readme file.

In order to enable MQTT test to cloud services or configuring the device with specific device features, 
refer to the documentation present in the Plug & Trust MW package and which can be downloaded 
from: https://www.nxp.com/products/security-and-authentication/authentication/:SE050?tab=Design_Tools_Tab.


Running the Demo
=======================================================================
If you have built a binary, flash the binary on to the board and reset the board.