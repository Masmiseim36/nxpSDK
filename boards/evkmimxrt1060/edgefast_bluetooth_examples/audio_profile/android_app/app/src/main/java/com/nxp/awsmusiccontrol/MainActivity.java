/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.nxp.awsmusiccontrol;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffColorFilter;
import android.graphics.Region;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Switch;
import android.widget.TextView;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;
import com.google.gson.Gson;
import com.nxp.awsmusiccontrol.iot.AwsConstants;
import com.nxp.awsmusiccontrol.iot.AwsShadow;
import com.nxp.awsmusiccontrol.iot.IPreferencesListener;
import com.nxp.awsmusiccontrol.iot.RgbLed;
import com.nxp.awsmusiccontrol.mqtt.Connection;
import java.io.UnsupportedEncodingException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.TimeZone;
import android.os.Handler;
import android.widget.Toast;

import java.util.Objects;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import static android.support.v7.widget.helper.ItemTouchHelper.ACTION_STATE_SWIPE;
import static android.support.v7.widget.helper.ItemTouchHelper.DOWN;
import static android.support.v7.widget.helper.ItemTouchHelper.LEFT;
import static android.support.v7.widget.helper.ItemTouchHelper.UP;

public class MainActivity extends AppCompatActivity {

    /** MQTT connection */
    Connection mqttConnection;

    /** AWS related constants (endpoint, cognito, etc. */
    AwsConstants awsConstants;

    /** GUI items */
    ImageButton imageButtonPlayPause;
    ImageButton imageButtonPlayPrevious;
    ImageButton imageButtonPlayNext;
    TextView textViewPlayerState;

    TextView tvConnectionStatus;
    ProgressBar progressBarConnection;
    ProgressBar progressBarRgbLed;

    /** variables */
    Boolean playStateLabel;
    Boolean playerReadyLabel;
    Integer playIndexLabel;
    String[] musicListLabel;

    Handler timeoutHandler;
    Runnable displayTimeoutToast;
    Runnable mqttTryToConnect;
    boolean doubleBackToExit;
    boolean mqttConnected;
    IPreferencesListener preferencesListener;
    AWSIotMqttClientStatusCallback mqttClientStatusCallback;

    /** Colours */
    int redColour;
    int greenColour;
    int blueColour;
    int blackColour;

    /** Preferences file dialog ID */
    private static final int FILE_DIALOG_ID = 5;

    private List<MusicInfo> mMusicInfoList = new ArrayList<>();
    private RecyclerView mMusicInfoRecyclerView;
    private MusicInfoAdapter mMusicInfoAdapter;

    private class MusicInfo {
        private String musicName;

        public MusicInfo(String musicNameSet) {
            this.musicName = musicNameSet;
        }

        public String GetMusicName(){
            return this.musicName;
        }
    }

    private class MusicInfoHolder extends RecyclerView.ViewHolder{
        private TextView mMusicNameTextView;
        private MusicInfo mMusicInfo;

        public MusicInfoHolder(LayoutInflater inflater, ViewGroup parent) {
            super(inflater.inflate(R.layout.music_info, parent, false));

            mMusicNameTextView = (TextView) itemView.findViewById(R.id.music_name);
        }

        public void bind(MusicInfo musicInfo, boolean selected) {
            mMusicInfo = musicInfo;
            if (selected) {
                mMusicNameTextView.setBackgroundColor(blueColour);
            }
            mMusicNameTextView.setText(musicInfo.GetMusicName());
        }
    }

    private class MusicInfoAdapter extends RecyclerView.Adapter<MusicInfoHolder> {
        private List<MusicInfo> mMusicInfoList;
        int mSelectItem = 0;

        public MusicInfoAdapter(List<MusicInfo> musicInfos, int selectItem) {
            mMusicInfoList = musicInfos;
            mSelectItem = selectItem;
        }

        @Override
        public MusicInfoHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            LayoutInflater layoutInflater = LayoutInflater.from(MainActivity.this);
            return new MusicInfoHolder(layoutInflater, parent);
        }

        @Override
        public void onBindViewHolder(MusicInfoHolder holder, int position) {
            MusicInfo musicInfo = mMusicInfoList.get(position);
            if (position == mSelectItem)
            {
                holder.bind(musicInfo, true);
            }
            else
            {
                holder.bind(musicInfo, false);
            }
        }

        @Override
        public int getItemCount() {
            return mMusicInfoList.size();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // initialize toolbar
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        // GUI items instances
        imageButtonPlayPause = (ImageButton) findViewById(R.id.imageButtonPlayPause);
        imageButtonPlayPrevious = (ImageButton) findViewById(R.id.imageButtonPlayPrevious);
        imageButtonPlayNext = (ImageButton) findViewById(R.id.imageButtonPlayNext);
        mMusicInfoRecyclerView = (RecyclerView) findViewById(R.id.recyclerViewMusicList);
        textViewPlayerState = (TextView) findViewById(R.id.textViewPlayerState);

        tvConnectionStatus = (TextView) findViewById(R.id.tvConnectionStatus);
        progressBarConnection = (ProgressBar) findViewById(R.id.progressBarConnection);
        progressBarRgbLed = (ProgressBar) findViewById(R.id.progressBarRgbLed);


        // default variable values
        mqttConnected = false;
        redColour = getResources().getColor(android.R.color.holo_red_dark);
        greenColour = getResources().getColor(android.R.color.holo_green_dark);
        blueColour = getResources().getColor(android.R.color.holo_blue_dark);
        blackColour = getResources().getColor(android.R.color.black);
        timeoutHandler = new Handler();
        final AppCompatActivity activity = this;
        displayTimeoutToast = new Runnable() {
            public void run() {
                Toast.makeText(
                    getApplicationContext(),
                    "Device is not responding.",
                    Toast.LENGTH_LONG
                ).show();

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        progressBarRgbLed.setVisibility(View.INVISIBLE);
                    }
                });
            }
        };
        mqttTryToConnect = new Runnable() {
            public void run() {
                Log.d(Connection.LOG_TAG, "Trying to connect to MQTT");

                if (!isNetworkAvailable()) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            textViewPlayerState.setVisibility(View.INVISIBLE);
                            tvConnectionStatus.setText("Network not available");
                            progressBarConnection.setVisibility(View.INVISIBLE);
                        }
                    });

                    // run timer again
                    timeoutHandler.postDelayed(this, AwsConstants.CONNECT_TIMEOUT);
                } else {
                    // network available, try to connect
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            textViewPlayerState.setVisibility(View.INVISIBLE);
                            tvConnectionStatus.setText("Connecting...");
                            progressBarConnection.setVisibility(View.VISIBLE);
                        }
                    });

                    final boolean isConnected = mqttConnection.connect(mqttClientStatusCallback);

                    if (isConnected) {
                        // cancel timeout
                        timeoutHandler.removeCallbacks(this);
                    } else {
                        new AlertDialog.Builder(activity).setTitle("Could not connect to AWS IoT")
                                .setMessage("Error during creation of AWS IoT client.\nPlease check *.properties file.")
                                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int which) {
                                        // open file choose dialog
                                        openFileDialog();
                                    }
                                })
                                .show();
                    }
                }
            }
        };

        // by default disable GUI items
        enableGUIIteams(false);

        imageButtonPlayPause.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AwsShadow shadow = new AwsShadow();
                shadow.state.desired.playState = (playStateLabel ? false : true);
                shadow.state.desired.playIndex = playIndexLabel;

                // reported and metadata must not be sent along with desired shadow state
                shadow.state.reported = null;
                shadow.metadata = null;

                //imageButtonPlayPause.setImageResource(android.drawable.ic_media_pause);

                // create message json in format of device shadow
                final String message = new Gson().toJson(shadow, AwsShadow.class);

                // publish message
                mqttConnection.publish(awsConstants.SHADOW_UPDATE, message);

                // show progress bar and wait for certain time
                // if no message from device has been received, display toast message
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        progressBarRgbLed.setVisibility(View.VISIBLE);
                    }
                });
                timeoutHandler.postDelayed(displayTimeoutToast, AwsConstants.TIMEOUT);

                Log.d(Connection.LOG_TAG, "Accelerometer update request was send.");
            }
        });

        imageButtonPlayNext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AwsShadow shadow = new AwsShadow();
                shadow.state.desired.playState = playStateLabel;
                if (playIndexLabel >= (mMusicInfoList.size() - 1))
                {
                    shadow.state.desired.playIndex = 0;
                }
                else
                {
                    shadow.state.desired.playIndex = playIndexLabel + 1;
                }
                Log.d(Connection.LOG_TAG, String.format("play Index \"%d\" from \"%d\"", shadow.state.desired.playIndex, mMusicInfoList.size()));

                // reported and metadata must not be sent along with desired shadow state
                shadow.state.reported = null;
                shadow.metadata = null;

                // create message json in format of device shadow
                final String message = new Gson().toJson(shadow, AwsShadow.class);

                // publish message
                mqttConnection.publish(awsConstants.SHADOW_UPDATE, message);

                // show progress bar and wait for certain time
                // if no message from device has been received, display toast message
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        progressBarRgbLed.setVisibility(View.VISIBLE);
                    }
                });
                timeoutHandler.postDelayed(displayTimeoutToast, AwsConstants.TIMEOUT);

                Log.d(Connection.LOG_TAG, "Accelerometer update request was send.");
            }
        });

        imageButtonPlayPrevious.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AwsShadow shadow = new AwsShadow();
                shadow.state.desired.playState = playStateLabel;
                if (playIndexLabel == 0)
                {
                    shadow.state.desired.playIndex = (mMusicInfoList.size() - 1);
                }
                else
                {
                    shadow.state.desired.playIndex = playIndexLabel - 1;
                }
                Log.d(Connection.LOG_TAG, String.format("play Index \"%d\" from \"%d\"", shadow.state.desired.playIndex, mMusicInfoList.size()));

                // reported and metadata must not be sent along with desired shadow state
                shadow.state.reported = null;
                shadow.metadata = null;

                // create message json in format of device shadow
                final String message = new Gson().toJson(shadow, AwsShadow.class);

                // publish message
                mqttConnection.publish(awsConstants.SHADOW_UPDATE, message);

                // show progress bar and wait for certain time
                // if no message from device has been received, display toast message
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        progressBarRgbLed.setVisibility(View.VISIBLE);
                    }
                });
                timeoutHandler.postDelayed(displayTimeoutToast, AwsConstants.TIMEOUT);

                Log.d(Connection.LOG_TAG, "Accelerometer update request was send.");
            }
        });

        // create preferences listener
        // on success, establish MQTT connection
        // on error, try to load preferences from file
        preferencesListener = new IPreferencesListener() {
            @Override
            public void onPreferencesLoadSuccess() {
                Toast.makeText(
                        getApplicationContext(),
                        "Preferences loaded successfully.",
                        Toast.LENGTH_LONG
                ).show();

                // create MQTT client status callback
                mqttClientStatusCallback = new AWSIotMqttClientStatusCallback() {
                    @Override
                    public void onStatusChanged(final AWSIotMqttClientStatus status,
                                                final Throwable throwable) {
                        Log.d(Connection.LOG_TAG, "MQTT connection status: " + String.valueOf(status));

                        // refreshShadowUrls GUI
                        mqttConnected = (status == AWSIotMqttClientStatus.Connected);

                        // display connection status on dashboard
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                switch (status) {
                                    case Connecting:
                                        textViewPlayerState.setVisibility(View.INVISIBLE);
                                        tvConnectionStatus.setText("Connecting...");
                                        progressBarConnection.setVisibility(View.VISIBLE);
                                        break;

                                    case Connected:
                                        // MQTT connection is ready, can do publish and subscribe
                                        tvConnectionStatus.setText(String.format("Connected to \"%s\" thing", awsConstants.getThingName()));
                                        progressBarConnection.setVisibility(View.INVISIBLE);
                                        break;

                                    case Reconnecting:
                                        if (throwable != null) {
                                            Log.e(Connection.LOG_TAG, "Connection error.", throwable);
                                        }
                                        if (isNetworkAvailable()) {
                                            textViewPlayerState.setVisibility(View.INVISIBLE);
                                            tvConnectionStatus.setText("Reconnecting");
                                            progressBarConnection.setVisibility(View.VISIBLE);
                                        } else {
                                            textViewPlayerState.setVisibility(View.INVISIBLE);
                                            tvConnectionStatus.setText("Network not available");
                                            progressBarConnection.setVisibility(View.INVISIBLE);
                                        }
                                        break;

                                    case ConnectionLost:
                                        if (throwable != null) {
                                            Log.e(Connection.LOG_TAG, "Connection error.", throwable);
                                            throwable.printStackTrace();
                                        }
                                        textViewPlayerState.setVisibility(View.INVISIBLE);
                                        tvConnectionStatus.setText("Connection error");
                                        progressBarConnection.setVisibility(View.INVISIBLE);
                                        break;

                                    default:
                                        textViewPlayerState.setVisibility(View.INVISIBLE);
                                        tvConnectionStatus.setText("Disconnected");
                                        progressBarConnection.setVisibility(View.INVISIBLE);
                                        break;
                                }
                            }
                        });

                        // when MQTT manager is connected to AWS server
                        // subscribe for accepted updates from AWS music control example shadow
                        // and download shadow after successful connection
                        if (status == AWSIotMqttClientStatus.Connected) {
                            // subscription callback for updating GUI values
                            final AWSIotMqttNewMessageCallback subscriptionCallback = new AWSIotMqttNewMessageCallback() {
                                @Override
                                public void onMessageArrived(final String topic, final byte[] data) {
                                    try {
                                        String message = new String(data, "UTF-8");

                                        // parse message from JSON into class
                                        Gson gson = new Gson();
                                        final AwsShadow shadow = gson.fromJson(message, AwsShadow.class);

                                        Log.d(Connection.LOG_TAG, String.format("Received message \"%s\" from topic \"%s\"", message, topic));

                                        if (shadow != null && shadow.state.reported != null) {
                                            playStateLabel = shadow.state.reported.playState;
                                            playIndexLabel = shadow.state.reported.playIndex;
                                            musicListLabel = shadow.state.reported.musicList;
                                            playerReadyLabel = shadow.state.reported.playerReady;

                                            if (playStateLabel)
                                            {
                                                imageButtonPlayPause.setImageResource(android.R.drawable.ic_media_pause);
                                            }
                                            else
                                            {
                                                imageButtonPlayPause.setImageResource(android.R.drawable.ic_media_play);
                                            }
                                            if ((shadow.state.reported.musicList.length > 0) && (playerReadyLabel))
                                            {
                                                runOnUiThread(new Runnable() {
                                                    @Override
                                                    public void run() {
                                                        textViewPlayerState.setVisibility(View.VISIBLE);
                                                        textViewPlayerState.setText("ready to play");
                                                    }
                                                });
                                                enableGUIIteams(true);
                                            }
                                            else
                                            {
                                                runOnUiThread(new Runnable() {
                                                    @Override
                                                    public void run() {
                                                        textViewPlayerState.setVisibility(View.VISIBLE);
                                                        if (shadow.state.reported.musicList.length <= 0) {
                                                            textViewPlayerState.setText("music list is null");
                                                        }
                                                        else {
                                                            textViewPlayerState.setText("player is not ready");
                                                        }
                                                    }
                                                });
                                                enableGUIIteams(false);
                                            }
                                            // remove timeout callback
                                            timeoutHandler.removeCallbacks(displayTimeoutToast);

                                            runOnUiThread(new Runnable() {
                                                @Override
                                                public void run() {
                                                    // hide progress bar
                                                    progressBarRgbLed.setVisibility(View.INVISIBLE);

                                                    mMusicInfoList.clear();
                                                    for (int index = 0; index < shadow.state.reported.musicList.length; ++index)
                                                    {
                                                        mMusicInfoList.add(new MusicInfo(shadow.state.reported.musicList[index]));
                                                    }
                                                    mMusicInfoRecyclerView.setLayoutManager(new LinearLayoutManager(MainActivity.this));
                                                    mMusicInfoAdapter = new MusicInfoAdapter(mMusicInfoList, playIndexLabel);
                                                    mMusicInfoRecyclerView.setAdapter(mMusicInfoAdapter);
                                                }
                                            });

                                            // last known shadow state received
                                            if (topic.equals(awsConstants.SHADOW_GET_ACCEPTED)) {
                                                runOnUiThread(new Runnable() {
                                                    @Override
                                                    public void run() {
                                                        Toast.makeText(
                                                                getApplicationContext(),
                                                                "Last known device shadow state has been received.",
                                                                Toast.LENGTH_LONG
                                                        ).show();
                                                    }
                                                });
                                            }
                                        } else {
                                            runOnUiThread(new Runnable() {
                                                @Override
                                                public void run() {
                                                    if (topic.equals(awsConstants.SHADOW_GET_ACCEPTED)) {
                                                        Toast.makeText(
                                                                getApplicationContext(),
                                                                "Device shadow has no reported state.",
                                                                Toast.LENGTH_LONG
                                                        ).show();
                                                    }
                                                }
                                            });
                                        }
                                    } catch (UnsupportedEncodingException e) {
                                        Log.e(Connection.LOG_TAG, "Message encoding error.", e);
                                    } catch (Exception e) {
                                        Log.e(Connection.LOG_TAG, "Could not parse message JSON.", e);
                                    }
                                }
                            };

                            // subscribe for all accepted changes (used for accelerometer values)
                            mqttConnection.subscribe(awsConstants.SHADOW_UPDATE_ACCEPTED, subscriptionCallback);

                            // get device shadow after successful connection
                            mqttConnection.subscribe(awsConstants.SHADOW_GET_ACCEPTED, subscriptionCallback);

                            // wait one second for subscribe to be done before sending GET request
                            timeoutHandler.postDelayed(new Runnable() {
                                @Override
                                public void run() {
                                    // make publish
                                    mqttConnection.publish(awsConstants.SHADOW_GET, AwsConstants.EMPTY_MESSAGE);

                                    // wait for certain time for device response
                                    timeoutHandler.postDelayed(displayTimeoutToast, AwsConstants.TIMEOUT);
                                }
                            }, 1000);
                        }
                    }
                };

                // Create new MQTT connection instance
                mqttConnection = new Connection(getApplicationContext(), awsConstants);

                // try to establish MQTT connection
                timeoutHandler.postDelayed(mqttTryToConnect, 1);
            }

            @Override
            public void onPreferencesLoadError(String errMsg) {
                new AlertDialog.Builder(activity).setTitle("Could not load preferences")
                    .setMessage("Could not load properties file with AWS IoT preferences.\nReason: " +
                            errMsg + "\nPlease select correct *.properties file.")
                    .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            // open file choose dialog
                            openFileDialog();
                        }
                    })
                    .show();
            }
        };

        // check saved preferences
        Context context = getApplicationContext();
        awsConstants = new AwsConstants(context, getString(R.string.preference_file_key));

        if (awsConstants.checkPreferencesAndValidate()) {
            // preferences are loaded, connect to MQTT
            awsConstants.refreshShadowUrls();
            preferencesListener.onPreferencesLoadSuccess();
        } else {
            // preferences not loaded yet, display dialog for loading preferences from file
            new AlertDialog.Builder(this).setTitle("Load preferences")
                .setMessage("Please locate properties file with AWS IoT preferences (e.g. Customer specific endpoint, Cognito pool ID, etc.).")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        // open file choose dialog
                        openFileDialog();
                    }
                })
                .show();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
        super.onActivityResult(requestCode, resultCode, intent);

        // file dialog result
        if (requestCode == FILE_DIALOG_ID && resultCode == RESULT_OK && intent != null) {
            // load preferences from selected file
            awsConstants.loadPreferencesFromFile(intent.getData(), preferencesListener);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if (mqttConnection != null && !mqttConnection.disconnect()) {
            Log.i(Connection.LOG_TAG, "Could not disconnect from AWS IoT.");
        }
    }

    private void enableGUIIteams(final boolean enable) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                imageButtonPlayNext.setEnabled(enable);
                imageButtonPlayPause.setEnabled(enable);
                imageButtonPlayPrevious.setEnabled(enable);
            }
        });
    }

    /**
     * Format Unix timestamp to human readable format
     */
    private String formatUnixTimeStamp(Long timestamp) {
        DateFormat sdf = new SimpleDateFormat("dd.MM.yyyy hh:mm:ss a", Locale.US);
        sdf.setTimeZone(TimeZone.getDefault());
        return sdf.format(new Date(timestamp * 1000L));
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

    /**
     * Check if any kind of network (WiFi or mobile) connection is available
     */
    private boolean isNetworkAvailable() {
        final ConnectivityManager connectivity = (ConnectivityManager) getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivity != null) {
            final NetworkInfo[] infos = connectivity.getAllNetworkInfo();
            if (infos != null) {
                for (NetworkInfo info : infos) {
                    if (info.getState() == NetworkInfo.State.CONNECTED) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    @Override
    public void onBackPressed() {
        if (doubleBackToExit) {
            super.onBackPressed();
            return;
        }

        this.doubleBackToExit = true;
        Toast.makeText(
                getApplicationContext(),
                "Please press back once more to exit",
                Toast.LENGTH_SHORT
        ).show();

        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                doubleBackToExit = false;
            }
        }, 2000);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        switch (id) {
            case R.id.menuItemAbout:
                // load About activity
                startActivity(new Intent(MainActivity.this, AboutActivity.class));
                return true;

            case R.id.menuItemLoadPreferences:
                // open file dialog for loading preferences
                openFileDialog();
                return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
