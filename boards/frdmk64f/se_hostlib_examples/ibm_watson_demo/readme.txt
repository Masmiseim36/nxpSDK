Overview
========

This project connects to IBM Watson IoT cloud Platform by using keys and certificates inside secure
module and subscibes to device push notifications.

THIS DEMO EXPECTS ROOT CA FOR THE DEVICE CERTIFICATE IS ALREADY UPLOADED TO THE
Watson Cloud ACCOUNT.  AND REQUIRED STEPS AS LISTED IN https://console.bluemix.net/docs/services/IoT/reference/security/set_up_certificates.html#set_up_certificates
ARE FOLLOWED.

This project uses the intermediate CA certificate and the
device certificate pre-provisioned inside the secure module with Watson IoT 
following which a secure communication channel is established.  Following this,
messages are subscribed from the WATSON IOT and these messages are interpreted to
control a Tri-Color LED on the FRDM-K64F board.

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K64F board (Rev D or Later)
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Prepare the Demo
================
1. Create an IBM ID which enables you to create an service instance for watson IoT platform (https://idaas.iam.ibm.com/idaas/mtfim/sps/authsvc?PolicyId=urn:ibm:security:authentication:asf:basicldapuser)
2. Create an instance of Internet of Things Platform in the Watson IoT after logging in.
3. Register the device intermediate CA and root certificate of the intermediate CA by following the below link 
 https://console.bluemix.net/docs/services/IoT/reference/security/set_up_certificates.html#set_up_certificates
    a. Click on ‘Launch’ button to access to the IoT dashboard
    b. Click on ‘Settings’ tab
    c. Click on ‘CA Certificates’
    d. Click on ‘Add Certificate’
    e. A new pop-up appears. Click on ‘Select a file’ option and select the certificate 
    f. click on ‘Save’
4. configure the security policies of the service by following steps
    a. Click on ‘Security’ tab
    b. Click on the pencil of ‘Connection Security’ to edit the preferences
    c. A new window is loaded, ‘Connection Security’. In the Security Level field, select ‘TLS with Client Certificate Authentication’.
    e. click on ‘Save’
5. Register Device type 
    a. In the service, Click on ‘Devices’ tab and ‘Device Types’
    b. Click on ‘Add device type. 
    c. Select the ‘Device’ type and write a name. Device type shall be registered as 'NXP-A71CH-D' and Optionally, add a description. 
    d. Click ‘Next’
    e. Optionally, add information to the rest of the fields. In this guide all the fields have been intentionally left empty. Finally, click ‘Done’.
    f. For more information, refer to https://console.bluemix.net/docs/services/IoT/iotplatform_task.html#iotplatform_task
6. Register device 
    a. Click on ‘Devices’ tab.
    b. Click on ‘Add Device’
    c. In the ‘Identity’ tab, select as ‘Device Type’ the one created in 'Register Device type' and the ‘Device ID’ the one retrieved using the A71CH config tool (UID of the device)
    d. Click on ‘Next’ button
    e. Click Done button to create the device 
7. Configure the application. 
    a. in the 'watson_iot_config.h' file, update the org details in the macro "WATSONIOT_MQTT_BROKER_ENDPOINT" which we get from the URL of the dashboard https://org_id.internetofthings.ibmcloud.com/dashboard/#/overview
    b. update the org details and UID of the device in the macro 'WatsonechoCLIENT_ID' which is available in 'watson_iot_config.h' 
8. Configuring the publish application
    a. Create API key and authentication pair token 
        i. In the Watson IoT Platform dashboard, go to Apps > API Keys.
        ii. Click Generate API Key.
            Important: Make a note of the API key and token pair. Authentication tokens are non-recoverable. If you lose or forget this token, you will need to re-register the API key to generate a new authentication token.
            
            An example of an API key is a-organization_id-a84ps90Ajs
            An example of a token is MP$08VKz!8rXwnR-Q*

        iii. Add a comment to identify the API key in the dashboard, for example: Key to connect my application.
        iv. Click Finish
    b. In OrgDetails.cfg file, update the Org details which we got from step 7.a in to the 'org' section
    c. In OrgDetails.cfg file, update the auth-key which we got from 8.a.ii in to the 'auth-key' section  
    d. Upate the UID of the device in the application test.py (at Line 40)	
9. Build the demo
10. Connect a USB cable between the PC host and the OpenSDA USB port on the target board.
11. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - change Setup->Terminal->New-line->Receive->AUTO
12. Connect the boards's RJ45 to network with Internet access (IP address to the
   board is assigned by the DHCP server). Make sure the connection on port 8883
   is not blocked.
13. Download the program to the target board.
14. Either press the reset button on your board or launch the debugger in your
   IDE to begin running the demo.
15. to see the event coming in to device and event going out of the device, login to the Watson IoT platform and launch the service.
    a. Click 'Devices’
    b. Click the registered device id
    c. Click 'Recent Events'
    d. Events will be displayed in portal


Running the demo
================

1. Persistent RED LED ON indicates error
2. All lights off along with the following message indicates readiness to
   subscribe messages from AWS Subscribing...  -->sleep -->sleep Publish done
3. Run the Publish application to publish events to the device 
    > python test.py OrgDetails.cfg GREEN ON
        The above command ensures that the green LED is turned ON. Similarly RED and BLUE LED can be turned ON and OFF
4. Events that are published shall be verified in the Watson Platform Dashboard(Refer to section 15)

References 

1. GitHub portal which explans about registering device and  certificates
    a. https://github.com/ibm-watson-iot/iot-nxpimxa71ch-c
2. Registering devices, CA certificates 
    b. https://console.bluemix.net/docs/services/IoT/iotplatform_task.html#iotplatform_task
3. For installing the IBM watson library in PIP refer to link https://github.com/ibm-watson-iot/iot-python

End of project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Board settings
==============
- Connect OM3710/A71CHARD to the board.
- The A71CH on board should have pre-installed Keys & certificates.
  (Please consult A71CH-Kineits-AWS Application notes / user manual for
  further details.)


Toolchain supported
===================
- MCUXpresso 10.3.0
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1

