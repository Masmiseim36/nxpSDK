/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

public class AwsConfig {

    /** mDNS service type of AWS Device Configuration example project */
    public static final String MDNS_SERVICE_TYPE = "_sslserver._tcp.";

    /** AWS preference key */
    public static final String PREFERENCES_KEY = "com.nxp.awsdeviceconfiguration.preferences";

    /** AWS server certificate */
    public static final String AWS_SERVER_CERTIFICATE = "-----BEGIN CERTIFICATE-----\n" +
            "MIIDHjCCAgYCCQDFp49MIClhoTANBgkqhkiG9w0BAQsFADBRMQswCQYDVQQGEwJj\n" +
            "ejEVMBMGA1UECAwMWmxpbnNreSBrcmFqMR0wGwYDVQQHDBRSb3pub3YgcG9kIFJh\n" +
            "ZGhvc3RlbTEMMAoGA1UECgwDTlhQMB4XDTE4MDYyNjA4MjQxNVoXDTI4MDYyMzA4\n" +
            "MjQxNVowUTELMAkGA1UEBhMCY3oxFTATBgNVBAgMDFpsaW5za3kga3JhajEdMBsG\n" +
            "A1UEBwwUUm96bm92IHBvZCBSYWRob3N0ZW0xDDAKBgNVBAoMA05YUDCCASIwDQYJ\n" +
            "KoZIhvcNAQEBBQADggEPADCCAQoCggEBAKQzXqC8lToJMojgVyGCrQV0GmjlGg6R\n" +
            "5BCLg8u5yG6F0vHTkLaKH9o/hE1HEgmc8Oi8SW7lJnksPZtajJIMRfePyxzWcMhG\n" +
            "Ltc7h9bYuKxMoBxQXca8uJknJO3X8yY5kbwit1Xlnn4SlwsdW3BTjFc0eR26IG5d\n" +
            "vSKaF4UrOjagHw9m1dg09STDVc3Tsj6z4ugNljScj+H21g6Zgpseju5UqeW4t8er\n" +
            "oT/gkXyMFUY3ruEUU5RgAJ0k0qVDRqIDSyAGdJUWd+lbHamiYrlmhlHkHw9ABC6i\n" +
            "cVTHQ6X14ujvqjDumMLtiRIkb58kDcOK3xZKMY3e0RCNhI6WM0A6raMCAwEAATAN\n" +
            "BgkqhkiG9w0BAQsFAAOCAQEAo9tjGRwMFAHtFbPGbByy/9aQPKVap0pUUMVwqJ8/\n" +
            "iQPTyxL9xuJ9MXEA0N4SELCKJvxWWbKH09FEiFbRmnRwxMF3/MJwYe1Irsw4vyCn\n" +
            "WslewmJ6SW5zHG19X8lQcJmHY9IUd1b8xri71MoohDooCVeS/SM2eOtUmwICEV3C\n" +
            "faBYT5hj1q2+h3IQWvOsu32eQnhA0eU7WRYW0iqrPu/qjbaTaup8p6u+k4aAVppr\n" +
            "kKu0AyUjtjcN64ro6ndtnqEGNyL58zw7TNGMEYIB98FXFRb8IVwxsZszbILujNkh\n" +
            "aPTeaDNQrG7pjOpnakjHq30cRla8K2u5ceZxphxniCtoHA==\n" +
            "-----END CERTIFICATE-----\n";

    /** Default AWS policy name */
    static final String DEFAULT_POLICY_NAME = "awsDeviceConfigurationPolicy";

    /** Default AWS policy to be attached to created certificate */
    static final String DEFAULT_POLICY_JSON = "{\n" +
            "  \"Version\": \"2012-10-17\",\n" +
            "  \"Statement\": [\n" +
            "    {\n" +
            "      \"Effect\": \"Allow\",\n" +
            "      \"Action\": \"iot:*\",\n" +
            "      \"Resource\": \"*\"\n" +
            "    }\n" +
            "  ]\n" +
            "}";
}
