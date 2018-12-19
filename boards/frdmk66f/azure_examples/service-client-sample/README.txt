# Prerequisites:
  - Setup your IoT hub
    [https://github.com/Azure/azure-iot-device-ecosystem/blob/master/setup_iothub.md]
  - Java SE 8 and Maven
    [https://github.com/Azure/azure-iot-sdk-java/blob/master/doc/java-devbox-setup.md]
  - After creating the IoT Hub, retreive the iothubowner connection string (mentioned in instructions to create the hub).

# Preparing the Service Client Java application:
 1) Navigate to the main sample file for Service Client. It can be found at:
    service-client-sample\src\main\java\samples\com\microsoft\azure\sdk\iot\service\sdk\ServiceClientSample.java
 2) Locate the following code in the file:
    private static final String connectionString = "[Connection string goes here]";
    private static final String deviceId = "[Device name goes here]";
 3) Replace "[Connection string goes here]" with the connection information for iothubowner user (see Prerequisites).
    Connection string required in java application is located in Shared_access_policies->iothubowner->connection_string.
    Replace "[Device name goes here]" with the name of the device you are using.

# Build:
    To build the application using Maven, at a command prompt navigate to the 'service-client-sample' folder.
    Then execute the following commands and check for build errors:
    mvn clean package
    mvn clean install

# Execute:
    To execute the application using Maven, at a command prompt navigate to the 'service-client-sample'
    where we builded our sample earlier and then execute the following command:
    mvn exec:java -D exec.mainClass="samples.com.microsoft.azure.sdk.iot.ServiceClientSample"
