'''
Created on Feb 1, 2017

@author: B35385
'''

import re
import sys
import serial
import paho.mqtt.client as mqtt

# Serial Communication Parameters.
BAUDRATE                  = 115200
UART_MSG_BUFFER_SIZE      = 64
STATE_GETTINGPACKETDATA   = 0
STATE_WAITFORPACKETMARKER = 1
STREAMING_PKT_ID          = 0x40
EVENT_STREAM_ID           = 0xFF
HDLC_PKT_MARKER           = "7E"
STOP_STREAMING_CMD        = "\x7E\x21\x00\x00\x02\x02\x01\x7E"
START_STREAMING_CMD       = "\x7E\x21\x00\x00\x02\x01\x01\x7E"

# Cloud Communication Parameters.
exitCommand       = "Exit"
topicSample       = "iot-2/evt/Acceleration3D/fmt/json"
sampleClientID    = "d:quickstart:iotqs-sensor:F8B156CA590B"
cloudServerURL    = "quickstart.messaging.internetofthings.ibmcloud.com"
cloudServerPort   = 1883
keepAliveDuration = 10
jsonFmtResponse   = "{\"d\":{\"Type\":\"Response\",\"Data\":%s}}"
jsonFmtSample     = "{\"d\":{\"Type\":\"Sample\",\"Data\":%s}}"
jsonFmtEvent      = "{\"d\":{\"Type\":\"Event\",\"Data\":%s}}"

# Global Variables.
comPort      = None
sampleCount  = 0
prevChar     = 0
packetIndex  = 0
uartPacket   = [None] * UART_MSG_BUFFER_SIZE
packetState  = STATE_WAITFORPACKETMARKER

# Function to handle incomming HDLC encoded bytes form the Board over UART.
def process_hdlc_byte(cByte):
    global packetState
    global packetIndex
    global uartPacket
    global prevChar
    escapeChar     = False
    packetError    = False
    packetReceived = False

    byte = ord(cByte)
    # Start of a packet.
    if (packetState == STATE_WAITFORPACKETMARKER):
        if (byte == 0x7E):
            packetIndex = 0
            packetState = STATE_GETTINGPACKETDATA
    # Packet payload.
    elif (packetState == STATE_GETTINGPACKETDATA):
        # Process actual payload bytes.
        if (byte != 0x7E):
            # Process escape sequences.
            if (byte == 0x7D):
                prevChar = byte;
                escapeChar = True
            elif (prevChar == 0x7D):
                if (byte == 0x5D):
                    byte = 0x7D
                elif (byte == 0x5E):
                    byte = 0x7E
                else:
                    packetError = True
                prevChar = 0
                escapeChar = False
            if (escapeChar == False):
                # Populate packet.
                uartPacket[packetIndex] = byte
                packetIndex += 1
        else:
            # Packet end, receive complete.
            if (packetIndex > 0):
                packetState = STATE_WAITFORPACKETMARKER
                packetReceived = True
            # Blank packet received, restart.
            else:
                packetState = STATE_GETTINGPACKETDATA
                packetIndex = 0
    else:
        packetError = True

    # Incase of error, restart.
    if (packetError == True):
        packetIndex = 0
        packetState = STATE_WAITFORPACKETMARKER

    return packetReceived

def sendCommandToBoard(handle, hexCmd):
    strCmd = hexCmd.encode("hex")
    lenCmd = len(strCmd)/2
    
    printCmd = "Sending Command   ["
    for byte in range(lenCmd):
        printCmd += "'0x"
        printCmd += strCmd[byte*2].upper()
        printCmd += strCmd[byte*2+1].upper()
        printCmd += "', "
    printCmd = printCmd[:-2] + "] to Board."
    
    print printCmd
    handle.write(hexCmd)

# Function to convert sample bytes to Values and then create JSON format payload.
def get_json_formatted_sample():
    global uartPacket
    strSample = []
    strResponse = []

    for byteOffset in range(packetIndex):
        strSample.append(uartPacket[byteOffset])
        strResponse.append("0x%02X" % uartPacket[byteOffset]) 
    
    if (uartPacket[0] == STREAMING_PKT_ID):
        if (uartPacket[3] == EVENT_STREAM_ID):
            payload = jsonFmtEvent % strSample
            print("Received Event %s from Board." % strResponse)
        else:
            payload = jsonFmtSample % strSample
    else:
        payload = jsonFmtResponse % strSample
        print("Received Response %s from Board." % strResponse)
    
    return payload

# The callback for when the client receives a CONNACK response from the server.
def connect_Sample_Handler(client, userData, rC):
    if (rC != mqtt.MQTT_ERR_SUCCESS):
        print("Connection with Cloud Service for Samples Failed ! [Error Code = %d]" % rC)
    else:
        print("Connected to Cloud Service for sending Samples...")
        sendCommandToBoard(userData, START_STREAMING_CMD)      
       
# Check CLI args for COM port number.
if (len(sys.argv) != 2):
    print("Usage : python %s <COM_PORT>" % (sys.argv[0].split('\\')[-1]))
    sys.exit(1)
else:
    comPort = sys.argv[1]

# Open COM Port for reading samples over UART from Board.
comHandle = serial.Serial(comPort, BAUDRATE)
if (comHandle.isOpen()):
    comHandle.flushInput()
    comHandle.flushOutput()
    print("Connected to Board @ %s..." % comPort)
else:
    print("Connection to Board @ %s Failed..." % comPort)
    sys.exit(1)

# Client settings for Sending Samples.
sampleClient            = mqtt.Client()
sampleClient.on_connect = connect_Sample_Handler
sampleClient._client_id = sampleClientID
sampleClient.user_data_set(comHandle)

# Open Cloud connections.
sampleClient.connect(cloudServerURL, cloudServerPort, keepAliveDuration)

# Start listening for Events from Cloud.
sampleClient.loop_start()

print "Press CTRL+C to exit...\n"
try:
    while(True):
        if process_hdlc_byte(comHandle.read(1)):
            jsonPayload = get_json_formatted_sample()
            (retCode, _) = sampleClient.publish(topicSample, jsonPayload, 0, False)
            if (retCode != mqtt.MQTT_ERR_SUCCESS):
                print("Publishing Sample to Topic for sending Samples Failed! [Error Code = %d]" % retCode)
            else:
                sampleCount += 1
except:
    sendCommandToBoard(comHandle, STOP_STREAMING_CMD)
    while(False == process_hdlc_byte(comHandle.read(1))):
        continue
    get_json_formatted_sample()
            
# Release resources on exit.
     
comHandle.close()
sampleClient.loop_stop()
sampleClient.disconnect()

print ("Finishing with %d samples sent.\n" % sampleCount)
sys.exit(0)
