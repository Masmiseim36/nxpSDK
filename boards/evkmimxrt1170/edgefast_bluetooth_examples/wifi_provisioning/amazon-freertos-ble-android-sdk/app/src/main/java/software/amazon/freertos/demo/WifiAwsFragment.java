package software.amazon.freertos.demo;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.le.ScanResult;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.PopupMenu;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.amazonaws.auth.AWSCredentialsProvider;
import com.amazonaws.mobile.client.AWSMobileClient;
import com.nxp.awsremotecontrol.iot.AwsConstants;

import java.util.ArrayList;
import java.util.List;

import software.amazon.freertos.amazonfreertossdk.AmazonFreeRTOSConstants;
import software.amazon.freertos.amazonfreertossdk.AmazonFreeRTOSDevice;
import software.amazon.freertos.amazonfreertossdk.AmazonFreeRTOSManager;
import software.amazon.freertos.amazonfreertossdk.BleConnectionStatusCallback;
import software.amazon.freertos.amazonfreertossdk.BleScanResultCallback;

import static android.app.Activity.RESULT_OK;

public class WifiAwsFragment extends Fragment {
    private static final String TAG = "WifiAwsFragment";
    
    private Button WifiProvisioningButton;
    private Button viewAwsShadowButton;
    private boolean verifyAuthentication = false;
    /** Preferences file dialog ID */
    private static final int FILE_DIALOG_ID = 5;
    private Uri uri = null;
    /** AWS related constants (endpoint, cognito, etc. */
    AwsConstants awsConstants;
    private boolean mqttOn = true;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
        awsConstants = new AwsConstants(getContext().getApplicationContext(), getString(R.string.preference_file_key));
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_main, container, false);

        WifiProvisioningButton = (Button)view.findViewById(R.id.wifi_provisioning);
        WifiProvisioningButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String path;
                Log.i(TAG, "Start Wi-Fi Provisioning");
                if ((false == awsConstants.checkPreferencesAndValidate()) && (null == uri))
                {
                    Toast.makeText(
                            getContext().getApplicationContext(),
                            "Preferences is invalid, please set preferences file.",
                            Toast.LENGTH_LONG
                    ).show();
                    return;
                }
                verifyAuthentication = true;

                if (null == uri) {
                    path = null;
                }
                else
                {
                    path = uri.toString();
                }

                Intent intentDeviceScanActivityButton
                        = DeviceScanActivity.newIntent(getActivity(), path);
                startActivity(intentDeviceScanActivityButton);
            }
        });

        viewAwsShadowButton = (Button)view.findViewById(R.id.view_aws_shadow);
        viewAwsShadowButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String path;
                Log.i(TAG, "View AWS Shadow Click");
                if ((false == awsConstants.checkPreferencesAndValidate()) && (null == uri))
                {
                    Toast.makeText(
                            getContext().getApplicationContext(),
                            "Preferences is invalid, please set preferences file",
                            Toast.LENGTH_LONG
                    ).show();
                    return;
                }
                verifyAuthentication = true;

                if (null == uri) {
                    path = null;
                }
                else
                {
                    path = uri.toString();
                }

                Intent intentViewAwsShadowButton
                = ViewAwsShadowActivity.newIntent(getActivity(), path);
                startActivity(intentViewAwsShadowButton);
            }
        });

        if (mqttOn)
        {
            Intent intentToStartAuthenticator = AuthenticatorActivity.newIntent(getActivity());
            startActivity(intentToStartAuthenticator);
        }

        return view;
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode != RESULT_OK) {
            return;
        }
        // file dialog result
        if (requestCode == FILE_DIALOG_ID && resultCode == RESULT_OK && getActivity().getIntent() != null) {
            // load preferences from selected file
            uri = data.getData();
            Toast.makeText(
                    getContext().getApplicationContext(),
                    "Preferences selected successfully.",
                    Toast.LENGTH_LONG
            ).show();
        }
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);
        inflater.inflate(R.menu.wifi_provision_option, menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.logout:
                AWSMobileClient.getInstance().signOut();
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (mqttOn)
                        {
                            Intent intentToStartAuthenticator = AuthenticatorActivity.newIntent(getActivity());
                            startActivity(intentToStartAuthenticator);
                        }
                    }
                });
                return true;
            case R.id.menuItemLoadPreferences:
                // open file dialog for loading preferences
                openFileDialog();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.i(TAG,"onResume: resume the activity");
        if (true == verifyAuthentication) {
            verifyAuthentication = false;
            Log.i(TAG,"Verify the authentication of the current user");
            getActivity().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (mqttOn)
                    {
                        Intent intentToStartAuthenticator = AuthenticatorActivity.newIntent(getActivity());
                        startActivity(intentToStartAuthenticator);
                    }
                }
            });
        }
    }

    /**
     * Open file dialog
     */
    private void openFileDialog() {
        Intent intent = new Intent()
                .setType("*/*")
                .setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent, "Select a preferences file"), FILE_DIALOG_ID);
    }

}
