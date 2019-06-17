/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

import com.amazonaws.regions.Region;
import com.amazonaws.services.iot.AWSIotClient;
import com.amazonaws.services.iot.model.AttachPolicyRequest;
import com.amazonaws.services.iot.model.AttachThingPrincipalRequest;
import com.amazonaws.services.iot.model.CreateKeysAndCertificateRequest;
import com.amazonaws.services.iot.model.CreateKeysAndCertificateResult;
import com.amazonaws.services.iot.model.CreatePolicyRequest;
import com.amazonaws.services.iot.model.CreatePolicyResult;
import com.amazonaws.services.iot.model.CreateThingRequest;
import com.amazonaws.services.iot.model.CreateThingResult;
import com.amazonaws.services.iot.model.KeyPair;
import com.amazonaws.services.iot.model.ListPoliciesRequest;
import com.amazonaws.services.iot.model.ListPoliciesResult;
import com.amazonaws.services.iot.model.ListThingsRequest;
import com.amazonaws.services.iot.model.ListThingsResult;
import com.amazonaws.services.iot.model.Policy;
import com.amazonaws.services.iot.model.ThingAttribute;
import com.nxp.awsdeviceconfiguration.iot.listeners.IAWSIoTClientListener;

import java.util.ArrayList;
import java.util.List;

public class AwsIoTConnection extends AwsConnection {
    private static final String TAG = "AWS: IoT Connection";

    private AWSIotClient iotClient;

    public AwsIoTConnection(Context appContext, AwsConstants awsConstants) {
        super(appContext, awsConstants);

        iotClient = new AWSIotClient(credentialsProvider);
        iotClient.setRegion(Region.getRegion(awsConstants.getMyRegion()));
    }

    /**
     * List all AWS thing names.
     * @param listener Callback
     */
    public void listAWSThingNames(IAWSIoTClientListener listener) {
        new ListAWSThingNamesTask(iotClient, listener).execute();
    }

    /** Result of AWS  */
    public static class ListAWSThingNamesResult extends AWSResult{
        private List<String> thingNames;

        ListAWSThingNamesResult(List<ThingAttribute> things, String exception) {
            super(exception);
            if (things != null) {
                thingNames = new ArrayList<>(things.size());
                for (ThingAttribute thing : things) {
                    thingNames.add(thing.getThingName());
                }
            }
        }

        public List<String> getThingNames() {
            return thingNames;
        }
    }

    /** AWS list thing names async task */
    private static class ListAWSThingNamesTask extends AsyncTask<Void, Void, ListAWSThingNamesResult> {
        private IAWSIoTClientListener listener;
        private AWSIotClient iotClient;

        ListAWSThingNamesTask(AWSIotClient iotClient, IAWSIoTClientListener listener) {
            this.iotClient = iotClient;
            this.listener = listener;
        }

        @Override
        protected ListAWSThingNamesResult doInBackground(Void... voids) {
            try {
                // list all thing names
                ListThingsRequest request = new ListThingsRequest();
                ListThingsResult result = iotClient.listThings(request);


                return new ListAWSThingNamesResult(result.getThings(), null);
            } catch (Exception e) {
                Log.e(TAG, "Exception during getting of AWS thing names.", e);
                return new ListAWSThingNamesResult(null, e.getMessage());
            }
        }

        @Override
        protected void onPostExecute(ListAWSThingNamesResult result) {
            if (result.getException() != null) {
                listener.onError(String.format("An error occurred while getting list of AWS thing names: %s", result.getException()));
            } else {
                listener.onSuccess(result);
            }
        }
    }

    /**
     * Create new AWS thing
     * @param thingName Thing name
     * @param listener Callback
     */
    public void createNewAWSThing(String thingName, IAWSIoTClientListener listener) {
        new CreateNewAWSThingTask(iotClient, thingName, listener).execute();
    }

    /** Result of AWS configuration task */
    static class CreateNewAWSThingResult extends AWSResult{
        boolean status;

        CreateNewAWSThingResult(boolean status, String exception) {
            super(exception);
            this.status = status;
        }

        boolean getStatus() {
            return status;
        }
    }

    /** AWS  async task */
    private static class CreateNewAWSThingTask extends AsyncTask<Void, Void, CreateNewAWSThingResult> {
        private IAWSIoTClientListener listener;
        private AWSIotClient iotClient;
        private String thingName;

        CreateNewAWSThingTask(AWSIotClient iotClient, String thingName, IAWSIoTClientListener listener) {
            this.iotClient = iotClient;
            this.thingName = thingName;
            this.listener = listener;
        }

        @Override
        protected CreateNewAWSThingResult doInBackground(Void... params) {
            try {
                CreateThingRequest createThingRequest = new CreateThingRequest();
                createThingRequest.setThingName(thingName);

                CreateThingResult createThingResult = iotClient.createThing(createThingRequest);
                final boolean thingNameCreated = (createThingResult.getThingName().equals(thingName));

                Log.d(TAG, "AWS thing name created: " + createThingResult.getThingName());

                return new CreateNewAWSThingResult(thingNameCreated, null);
            } catch (Exception e) {
                Log.e(TAG, "Exception during AWS configuration.", e);
                return new CreateNewAWSThingResult(false, e.getMessage());
            }
        }

        @Override
        protected void onPostExecute(CreateNewAWSThingResult result) {
            if (result.getException() != null) {
                listener.onError(String.format("An error occurred while creating new AWS Thing: %s", result.getException()));
            } else if (!result.getStatus()) {
                listener.onError("An error occurred while creating new AWS Thing");
            } else {
                listener.onSuccess(result);
            }
        }
    }


    /**
     * Configure AWS IoT for device. Create and assign new certificate and create new thing name or use selected one.
     *
     * @param listener Callback
     */
    public void configureAWSCredentials(String thingName, IAWSIoTClientListener listener) {
        new ConfigureAWSCredentialsTask(iotClient, thingName, listener).execute();
    }

    /** Result of AWS configuration task */
    public static class ConfigureAWSCredentialsResult extends AWSResult{
        String pemCertificate;
        KeyPair keyPair;

        ConfigureAWSCredentialsResult(String pemCertificate, KeyPair keyPair, String exception) {
            super(exception);
            this.pemCertificate = pemCertificate;
            this.keyPair = keyPair;
        }

        public KeyPair getKeyPair() {
            return keyPair;
        }

        public String getPemCertificate() {
            return pemCertificate;
        }
    }

    /** AWS configuration async task */
    private static class ConfigureAWSCredentialsTask extends AsyncTask<Void, Void, ConfigureAWSCredentialsResult> {
        private IAWSIoTClientListener listener;
        private AWSIotClient iotClient;
        private String thingName;

        ConfigureAWSCredentialsTask(AWSIotClient iotClient, String thingName, IAWSIoTClientListener listener) {
            this.iotClient = iotClient;
            this.thingName = thingName;
            this.listener = listener;
        }

        @Override
        protected ConfigureAWSCredentialsResult doInBackground(Void... params) {
            try {
                // create certificates
                CreateKeysAndCertificateRequest certificateRequest = new CreateKeysAndCertificateRequest();
                certificateRequest.setSetAsActive(true);

                CreateKeysAndCertificateResult certificateResult = iotClient.createKeysAndCertificate(certificateRequest);
                Log.d(TAG, "Certificate created: "+certificateResult.getCertificateId());

                String pemCertificate = certificateResult.getCertificatePem();
                KeyPair keyPair = certificateResult.getKeyPair();

                // create default policy
                ListPoliciesRequest req = new ListPoliciesRequest();
                req.setPageSize(250);
                ListPoliciesResult listPoliciesResult = iotClient.listPolicies(req);
                boolean defaultPolicyExist = false;
                for (Policy policy : listPoliciesResult.getPolicies()) {
                    if (policy.getPolicyName().equals(AwsConfig.DEFAULT_POLICY_NAME)) {
                        defaultPolicyExist = true;
                        break;
                    }
                }
                if (!defaultPolicyExist) {
                    CreatePolicyRequest createPolicyRequest = new CreatePolicyRequest();
                    createPolicyRequest.setPolicyName(AwsConfig.DEFAULT_POLICY_NAME);
                    createPolicyRequest.setPolicyDocument(AwsConfig.DEFAULT_POLICY_JSON);

                    CreatePolicyResult createPolicyResult = iotClient.createPolicy(createPolicyRequest);
                    if (!createPolicyResult.getPolicyName().equals(AwsConfig.DEFAULT_POLICY_NAME)) {
                        return new ConfigureAWSCredentialsResult(null, null, "Error during creating policy document!");
                    }
                    Log.d(TAG, "Default policy created.");
                }

                // attach policy to certificate
                AttachPolicyRequest attachPolicyRequest = new AttachPolicyRequest();
                attachPolicyRequest.setTarget(certificateResult.getCertificateArn());
                attachPolicyRequest.setPolicyName(AwsConfig.DEFAULT_POLICY_NAME);

                iotClient.attachPolicy(attachPolicyRequest);
                Log.d(TAG, "Default policy attached to created certificate.");

                // attach certificate to thing
                AttachThingPrincipalRequest attachThingPrincipalRequest = new AttachThingPrincipalRequest();
                attachThingPrincipalRequest.setThingName(thingName);
                attachThingPrincipalRequest.setPrincipal(certificateResult.getCertificateArn());

                iotClient.attachThingPrincipal(attachThingPrincipalRequest);
                Log.d(TAG, String.format("Certificate \"%s\" attached to thing \"%s\".", certificateResult.getCertificateId(), thingName));

                return new ConfigureAWSCredentialsResult(pemCertificate, keyPair, null);
            } catch (Exception e) {
                Log.e(TAG, "Exception during AWS configuration.", e);
                return new ConfigureAWSCredentialsResult(null, null, e.getMessage());
            }
        }

        @Override
        protected void onPostExecute(ConfigureAWSCredentialsResult result) {
            if (result.getException() != null) {
                listener.onError(String.format("An error occurred while configuring AWS: %s", result.getException()));
            } else {
                listener.onSuccess(result);
            }
        }
    }
}
