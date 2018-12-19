/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot.ssl;

import com.nxp.awsdeviceconfiguration.iot.AwsConfig;
import com.nxp.awsdeviceconfiguration.iot.AwsConstants;
import com.nxp.awsdeviceconfiguration.iot.DeviceRequestCommand;
import com.nxp.awsdeviceconfiguration.iot.DeviceRequestResponse;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;
import java.security.KeyStore;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.util.Locale;

import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLHandshakeException;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.TrustManagerFactory;

public class SSLConnection  {

    private static final int RESPONSE_OFFSET = 0;
    private static final int PAYLOAD_LENGTH_OFFSET = 1; // length is 2 Bytes
    private static final int PAYLOAD_OFFSET = 3;

    private SSLContext sslContext;
    private SSLSocket socket;
    private OutputStream writer;
    private InputStream reader;
    private ISSLConnectionStatus listener;

    public SSLConnection(ISSLConnectionStatus listener) {
        this.listener = listener;

        // prepare AWS server certificate (defined in AWS config)
        Certificate certificate;
        try {
            final CertificateFactory factory = CertificateFactory.getInstance("X.509");
            final InputStream stream = new ByteArrayInputStream(AwsConfig.AWS_SERVER_CERTIFICATE.getBytes(StandardCharsets.UTF_8));
            certificate = factory.generateCertificate(stream);
            stream.close();
        } catch (Exception e) {
            listener.onError("Could not load AWS server certificate.");
            return;
        }

        try {
            // set up key store
            final KeyStore keyStore = KeyStore.getInstance(KeyStore.getDefaultType());
            keyStore.load(null, null);
            keyStore.setCertificateEntry("ca", certificate);

            // set up trust manager factory
            final TrustManagerFactory tmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
            tmf.init(keyStore);

            // create SSL context
            sslContext = SSLContext.getInstance("TLS");
            sslContext.init(null, tmf.getTrustManagers(), null);
        } catch (Exception e) {
            listener.onError("Could not set up SSL context.");
        }

        // create socket
        socket = createSocket();
    }

    private SSLSocket createSocket() {
        try {
            return (SSLSocket) sslContext.getSocketFactory().createSocket();
        } catch (IOException e) {
            listener.onError("Could not create SSL socket!");
            return null;
        }
    }

    public boolean connect(InetAddress ipAddress, int port, int timeout) {
        if (socket == null && (socket = createSocket()) == null) {
            return false;
        }

        try {
            socket.connect(new InetSocketAddress(ipAddress, port), timeout);
            return true;
        } catch(IOException e) {
            return false;
        }
    }

    public void closeSocket() {
        try {
            socket.close();
        } catch (IOException e) {
            listener.onError("Could not close SSL socket.");
        }
    }

    /**
     * Send byte data through SSL connection and receive an answer.
     * Blocking operation! Has to be run in async task.
     *
     * @param cmd Device request command
     * @param data Byte array with data to be send
     * @return Device request response
     */
    public DeviceRequestResponse sendBlocking(DeviceRequestCommand cmd, byte[] data) {
        if (writer == null || reader == null) {
            // create socket again if it is closed
            if (socket.isClosed() && (socket = createSocket()) == null) {
                return DeviceRequestResponse.FAIL.setReason("Could not create SSL socket!");
            }

            try {
                // get input and output socket streams
                writer = socket.getOutputStream();
                reader = socket.getInputStream();
            } catch (SSLHandshakeException e) {
                return DeviceRequestResponse.FAIL.setReason("Handshake with device failed!");
            } catch (IOException e) {
                return DeviceRequestResponse.FAIL.setReason("Could not get socket's input and output stream!");
            }
        }

        // send data
        try {
            writer.write(data);
            writer.flush();
        } catch (IOException e) {
            return DeviceRequestResponse.FAIL.setReason("Could not write data to socket's output stream!");
        }

        if (cmd.isNoReply()) {
            return DeviceRequestResponse.OK;
        }

        // receive data
        byte[] buf = new byte[AwsConstants.READER_BUFFER_SIZE];
        int bytesRead;
        try {
            if ((bytesRead = reader.read(buf)) == -1 || bytesRead < 1) {
                return DeviceRequestResponse.FAIL.setReason("No data has been read!");
            }

            // return response
            if (buf[RESPONSE_OFFSET] < DeviceRequestResponse.values().length) {
                final DeviceRequestResponse response = DeviceRequestResponse.values()[buf[RESPONSE_OFFSET]];

                // response has payload
                if (cmd.isGet() && response == DeviceRequestResponse.OK) {
                    final int length = (buf[PAYLOAD_LENGTH_OFFSET] << 8) | buf[PAYLOAD_LENGTH_OFFSET + 1];
                    if (buf.length > length) {
                        final String payload = new String(buf, PAYLOAD_OFFSET, length);
                        if (payload.length() > 0) {
                            response.setPayload(payload);
                        } else {
                            return DeviceRequestResponse.FAIL.setReason("Payload has zero data length!");
                        }
                    } else {
                        return DeviceRequestResponse.FAIL.setReason("Payload data length is higher than buffer size!");
                    }
                }

                // login failed
                if (response == DeviceRequestResponse.FAIL && cmd == DeviceRequestCommand.LOGIN) {
                    return response.setReason("Wrong password?");
                }

                return response.setReason("Device answer");
            } else {
                return DeviceRequestResponse.FAIL.setReason(String.format(Locale.US, "Unknown response: %d", buf[RESPONSE_OFFSET]));
            }
        } catch (IOException e) {
            return DeviceRequestResponse.FAIL.setReason("Could not read data from socket's input stream!");
        }
    }
}
