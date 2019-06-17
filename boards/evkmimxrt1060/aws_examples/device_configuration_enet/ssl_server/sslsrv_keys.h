/*
 * Copyright 2018, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

static const char ssl_srv_cert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDMDCCAhgCCQCLlbXycU7CHDANBgkqhkiG9w0BAQsFADBRMQswCQYDVQQGEwJj\n"
    "ejEVMBMGA1UECAwMWmxpbnNreSBrcmFqMR0wGwYDVQQHDBRSb3pub3YgcG9kIFJh\n"
    "ZGhvc3RlbTEMMAoGA1UECgwDTlhQMB4XDTE4MDYyNjA4NTY0MFoXDTIxMDQxNTA4\n"
    "NTY0MFowYzELMAkGA1UEBhMCQ1oxFTATBgNVBAgMDFpsaW5za3kga3JhajEdMBsG\n"
    "A1UEBwwUUm96bm92IHBvZCBSYWRob3N0ZW0xDDAKBgNVBAoMA05YUDEQMA4GA1UE\n"
    "AwwHKi5sb2NhbDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAPGxyBTT\n"
    "QOklJ1c/u7uZUxpH9FE8VT0E3OabE67c0KD5RDUbC+L/KMVozv6n5xkzMpuvwoxw\n"
    "PI937J1128ghSj4WacAoUOnRvSX8n6btKW2idpJOeN7Pi00PyPw2NJFAOK7hrGPG\n"
    "23oyx04EP+q9cPfyShkq4dO/CngvjTv2DcCzNTJdV9RucFzkBT54+kItCCSYkvwJ\n"
    "gPFByclBkQU/Y79UH27JsEOK77abEeo0nKVmitCSfjEabNhBIaglktvSqTdi9fWv\n"
    "bdM6vOWsWCV6sZ1MBhvVoaKNj87u6HpiEv2KL9toU84yRj4kIxlq29bfAfmmWRQE\n"
    "c0gmp1DPjWs2d5kCAwEAATANBgkqhkiG9w0BAQsFAAOCAQEAg+/bZiLxwCkvZH61\n"
    "l2ZHR/efb+ku6fHrEYKJuSsMfRCe1M9tO/RAPDnIpDb57jgVcSwhAnveqvhHcoL5\n"
    "dU3Vlo/s1ro3w7xL/ep/fTFXaA0s3jBcJauyGNH0SNPb71r9wmfKItGg/9aXbCFx\n"
    "c59KFRl8jadNyFFCMaE2Bt1uedvAavPu7h5o6pRa1JDKDym4DXH+MIxnTb9kYium\n"
    "19A2ec150cNWPxWTyLFhMm0eF0HkA+lxGvcieIau+Zfbur/iARpOG7t3P5Th9U4Z\n"
    "ypYsye/M8f7ca+wSpcUiP+DoCwHyzGeUMKIbHjQf7NrZrJoF8NfdTr71bjP/XnQk\n"
    "YueCdw==\n"
    "-----END CERTIFICATE-----\n";

static const char ssl_srv_pkey[] =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEpAIBAAKCAQEA8bHIFNNA6SUnVz+7u5lTGkf0UTxVPQTc5psTrtzQoPlENRsL\n"
    "4v8oxWjO/qfnGTMym6/CjHA8j3fsnXXbyCFKPhZpwChQ6dG9Jfyfpu0pbaJ2kk54\n"
    "3s+LTQ/I/DY0kUA4ruGsY8bbejLHTgQ/6r1w9/JKGSrh078KeC+NO/YNwLM1Ml1X\n"
    "1G5wXOQFPnj6Qi0IJJiS/AmA8UHJyUGRBT9jv1QfbsmwQ4rvtpsR6jScpWaK0JJ+\n"
    "MRps2EEhqCWS29KpN2L19a9t0zq85axYJXqxnUwGG9Whoo2Pzu7oemIS/Yov22hT\n"
    "zjJGPiQjGWrb1t8B+aZZFARzSCanUM+NazZ3mQIDAQABAoIBAF7xBzenBIiRH7MW\n"
    "RhibUhmwyjhfCty9XEj6AfFerFWPlUuHJFQ3Bz0EvssqCY3MnsSpdjLUo/V3lmsE\n"
    "MSGcch2RkFj7fU1cJoEVi2F3KB75ArrWOITN4lqTKNBqOpwULjAEVkObifumlsP+\n"
    "x1zerdG4S4M3ZR5YnTQ4+bHkJeLbv0xFckvnRxj/y+23I3CmhO+3Y9otRuYL9Qvv\n"
    "1Z2sqL7Gb17CFpjhZz1a6op7StMB9EX/NnPTHtFedaerYkmQELjosjGty7FVXGIy\n"
    "Lrn48txsDexMUockxjXOIx+mF46ScuDv9aL2YwpV6zBv1LSEMOyDcdrEGuoqEzI4\n"
    "7y51dUECgYEA+jTJNX1shLM+myB504UAdzeLirGojpd+QMGH9RdzFrYXCrmEQEaB\n"
    "hAlKr7qVbGQRdg3NtQCxCYJvFSl7P3z0LjcmamDIHLai0n7xe1/I9UhdYFgacYJd\n"
    "7u7ZqsjZ1kgZ0dFDs9HdqB5olmz/2zGmSibY0a5d9x7jZSYzN9snYLcCgYEA90qJ\n"
    "znpkiSyFYqn6hpoSatZ+mQ9/pKyhDusJn5zzN8LsHWg/+YFVJ0YxR4/ShtNDgdPq\n"
    "2OkJlJz3r9WveiSc/GxVkswc9hy99uU7kbaAw0drSHIq5DK6ja1xwFc6XPXvow66\n"
    "mHqKVeIR43tN1c2Lt3+MJygEdaeznOSkl/6/+i8CgYBCNEdtROO69SbCq0QxCMdH\n"
    "xEXDDCE6obFBdzP/URYd006jgtUQoNdSVZcSin9RL+VC5IIiu60chAvagKBg+zIt\n"
    "LGCY00T/+/93TSmeaIjnBh/XB1jv4fvy5fZobz7cqTVTxFhyszxAaNNpwiutFyUW\n"
    "B9NlIAszAVRQBlyVEgMIXwKBgQCPJ8xdBod6zZYuBr6r+zdcN+2zx8AZ4YGfNDsu\n"
    "9lnqVuH/KCGY2+EiykSu67hxKo3y824JweY07SWJ2gcqi1WB32kugRDm/L6P1iZE\n"
    "9UTXR1n453ajAiby64xlT1nPBOSEHXux14aLr0Kx0Oi/M62FPcYxyL3EKqAU3FvY\n"
    "TbZvKwKBgQDZdYDmJ/WXJhhaUpxBvau9ov9ZH/ocAFoAxMp9c+1vqbYaJcQ2ZMCt\n"
    "lRRGypFgsEMlf0Id7cZYLURaBmCIPDfKn4NfYHmi+Qigqgp/gUYdrFJZ1Lhv8n8W\n"
    "D8tEIZVlE/bBC4v9Iui5NYMc5SBiclORZjOumnJ1lJLUVgi29Jrrtg==\n"
    "-----END RSA PRIVATE KEY-----\n";

static const uint32_t ssl_srv_cert_len = sizeof(ssl_srv_cert);

static const uint32_t ssl_srv_pkey_len = sizeof(ssl_srv_pkey);
