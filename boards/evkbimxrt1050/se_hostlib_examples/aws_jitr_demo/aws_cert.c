/**
 * @file aws_cert.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017,2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Keys / Key Indexes
 */

#include <stddef.h>
#include <aws_iot_config.h>

/* Root CA certificate */
const unsigned char certRootCA[] = "-----BEGIN CERTIFICATE-----\r\n"
    "MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
    "yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\r\n"
    "ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"
    "U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\r\n"
    "ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\r\n"
    "aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\r\n"
    "MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\r\n"
    "ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\r\n"
    "biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\r\n"
    "U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\r\n"
    "aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\r\n"
    "nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\r\n"
    "t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\r\n"
    "SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\r\n"
    "BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\r\n"
    "rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\r\n"
    "NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\r\n"
    "BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\r\n"
    "BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\r\n"
    "aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\r\n"
    "MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\r\n"
    "p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\r\n"
    "5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\r\n"
    "WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\r\n"
    "4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\r\n"
    "hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\r\n"
    "-----END CERTIFICATE-----\r\n";
const size_t lenCertRootCA = sizeof(certRootCA);

#if AWS_IOT_USE_KEYS_FROM_SM
/*
 Reference steps to load keys to debug sample.

 >>> debug reset
 >>> set pair -x 0 -k C:\temp\aws_pg.pem
 ECCPrivateKey: hidden
 ECCPublicKey (LEN=65):
 04:61:B7:F8:1C:EE:E5:17:BF:E9:AD:50:D5:3D:DE:25:5B:E3:FF:95:9D:DD:F5:89:4C:B6:CB:5B:F7:9E:4E:4D:
 42:E2:06:A6:E3:E2:A1:7B:12:E7:F6:75:58:E4:E0:D4:B9:3C:3C:C2:F8:FF:0F:A2:A5:E6:1D:4F:2E:3A:6A:0D:
 6E
 >>> set gp -h 0000 -c C:\temp\aws_pg.crt
 Filename: C:\temp\aws_pg.crt
 Certificate Size (DER format) = 421 byte
 */
/* Device certificate */
const unsigned char certClient[] = {
    0x00, 0x00,  // Starting address in GP Storage. (Little Endian Format)
    };

const size_t lenCertClient = sizeof(certClient);


#else
/* Device certificate */
const unsigned char certClient[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBbDCCAROgAwIBAgIBATAKBggqhkjOPQQDAjAyMTAwLgYDVQQDDCdOWFAgU2Vt"
"aWNvbmR1Y3RvcnMgNzAwMjIwODkwMDI5MzQ5MTY0MjMwHhcNMTgwMTMwMDkxMTUy"
"WhcNMjgwMTI4MDkxMTUyWjAyMTAwLgYDVQQDDCdOWFAgU2VtaWNvbmR1Y3RvcnMg"
"NzAwMjIwODkwMDI5MzQ5MTY0MjMwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASl"
"w1vxaLpTNzT1L9Zqr+8zkRRBQ85RTAyecNIRTIZa06qp1hz8Ty1x5mibQk3pDkHt"
"TG0kt4MVrt+JP97OcYnOoxowGDAJBgNVHRMEAjAAMAsGA1UdDwQEAwIHgDAKBggq"
"hkjOPQQDAgNHADBEAiAVWjrUSgNrd45ffG55iLGbdt1FdzvG25m1m/7PU6rmygIg"
"QrKEQ7gydfxAecDmzDyYD+HPlD+99thFFdmgFmo3jLw="
"-----END CERTIFICATE-----\r\n"
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDYzCCAUugAwIBAgIJAPHpMqkJ5QhmMA0GCSqGSIb3DQEBCwUAMCkxJzAlBgNV"
"BAMMHk5YUCBTZW1pY29uZHVjdG9ycyBERU1PIHJvb3RDQTAeFw0xODAxMzAwOTEx"
"NTFaFw0yODAxMjgwOTExNTFaMDIxMDAuBgNVBAMMJ05YUCBTZW1pY29uZHVjdG9y"
"cyA3MDAyMjA4OTAwMjkzNDkxNjQyMzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IA"
"BEHAHd+E1R/Fs2w3EYwsmcDTSVE02otd27kvfUqC8DULDMjSVod6zk3zVKartPz1"
"tH8CapZpctSoFe4x2ZV0q3ajUDBOMB0GA1UdDgQWBBQXInxIfw0pAJgnEUbM60Lh"
"caty+TAfBgNVHSMEGDAWgBRUixPIhgT9liwuYCuXvX/xTskVTzAMBgNVHRMEBTAD"
"AQH/MA0GCSqGSIb3DQEBCwUAA4ICAQBsLtz73hKbVAGrAQW/PEQNyY4djKz062f4"
"xGuKfZfjM1hZVda6JAHCKKYCGbrlQE0OJpRyr1nkPEWiF8HSCt3nt4UY42wYN4+X"
"bwUOHQpb/BLxXk5aMkPjCPKvw4pVyHEoJ+tek+rBj2k7cyD1MCrYGa9f/3pFi+Jg"
"qySLqLSw2G4FkRk565vnsK6yF8pWyFJtk7NSXsvo0+ivLoQj/2hdlxs6e89lU31r"
"+R3jLs9Fwtx3+XGmr9dziVnUGCe7oy64myAuUpDuJXYLREllokQ87JChryE0u1Nh"
"UnktAhKfd9mAI0SeZ+gFsvCAEgSrGBwci0ILtYY5Cn8WK6Ha0l6gISKtuvwYp7QM"
"mttchmbXH0ZL3AtSK1D9ae5lEM82ATHHnV4dOJl5m5McuPaqlS7E1a0BogpsVepd"
"HVyB9YpStSVUM/PlXfinr8fNmFMAgeJ3I168Ny1JmgfrrdBvnZmLGNX2bshNeVwG"
"G94c8vR1wSI9UK6xjN3f2SJkh7X/BJP8zq72IyvfDvfFIyzn2D3qD/6fajBsTNlS"
"WaORgsyuBPJtckMgYPLSDyJWv/TyMNmT2vrPMdNx1gyibf2qI5AWPsSdVUB3Sl/r"
"49ZuTV04WZUee8cRC73dzoKvV94NK33dv3ZZzPSEvO6M2+RfvUFXqKj0kkaXdS3J"
"BiXrfwDbzA=="
"-----END CERTIFICATE-----\r\n";
const size_t lenCertClient = sizeof(certClient);

/* Private key */
const unsigned char privKey[] = "-----BEGIN EC PRIVATE KEY-----\r\n"
"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ\r\n"
"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ\r\n"
"-----END EC PRIVATE KEY-----\r\n";
const size_t lenPrivKey = sizeof(privKey);

#endif
