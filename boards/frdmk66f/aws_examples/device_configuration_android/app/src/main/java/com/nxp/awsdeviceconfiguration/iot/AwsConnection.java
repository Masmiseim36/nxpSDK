/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;


import android.content.Context;

import com.amazonaws.auth.CognitoCachingCredentialsProvider;

abstract class AwsConnection {
    CognitoCachingCredentialsProvider credentialsProvider;

    AwsConstants awsConstants;

    AwsConnection(Context appContext, AwsConstants awsConstants) {
        this.awsConstants = awsConstants;

        // Initialize the AWS Cognito credentials provider
        credentialsProvider = new CognitoCachingCredentialsProvider(
                appContext,
                awsConstants.getCognitoPoolId(),
                awsConstants.getMyRegion()
        );
    }
}
