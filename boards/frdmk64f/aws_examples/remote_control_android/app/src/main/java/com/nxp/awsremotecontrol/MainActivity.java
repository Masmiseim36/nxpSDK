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
package com.nxp.awsremotecontrol;

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
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.CardView;
import android.util.Log;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Switch;
import android.widget.TextView;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;
import com.google.gson.Gson;
import com.nxp.awsremotecontrol.iot.AwsConstants;
import com.nxp.awsremotecontrol.iot.AwsShadow;
import com.nxp.awsremotecontrol.iot.IPreferencesListener;
import com.nxp.awsremotecontrol.iot.RgbLed;
import com.nxp.awsremotecontrol.mqtt.Connection;
import java.io.UnsupportedEncodingException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;
import android.os.Handler;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    /** MQTT connection */
    Connection mqttConnection;

    /** AWS related constants (endpoint, cognito, etc. */
    AwsConstants awsConstants;

    /** GUI items */
    Switch swLedRed; // Red
    Switch swLedGreen; // Green
    Switch swLedBlue; // Blue
    ImageButton bAccelRefresh;
    ImageView iLedImage;
    TextView tvConnectionStatus;
    TextView tvLedTitle;
    TextView tvAccelTimestamp;
    TextView tvRgbLedTimestamp;
    TextView tvAccelValues;
    ProgressBar progressBarConnection;
    ProgressBar progressBarAccel;
    ProgressBar progressBarRgbLed;
    CardView cardViewLed;
    CardView cardViewAccel;

    /** variables */
    int rgbLedBinState;
    Handler timeoutHandler;
    Runnable displayTimeoutToast;
    Runnable mqttTryToConnect;
    boolean doubleBackToExit;
    Boolean isRgbLed;
    String[] ledColours;
    int numberOfLEDs;
    final String[] rgbLEDSwitchNames = { "Red", "Green", "Blue" };
    Switch[] switches;
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // initialize toolbar
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        // GUI items instances
        swLedRed = (Switch) findViewById(R.id.swLedRed);
        swLedGreen = (Switch) findViewById(R.id.swLedGreen);
        swLedBlue = (Switch) findViewById(R.id.swLedBlue);
        bAccelRefresh = (ImageButton) findViewById(R.id.bAccelRefresh);
        iLedImage = (ImageView) findViewById(R.id.ledImage);
        tvLedTitle = (TextView) findViewById(R.id.ledTitle);
        tvConnectionStatus = (TextView) findViewById(R.id.tvConnectionStatus);
        tvAccelTimestamp = (TextView) findViewById(R.id.tvAccelTimestamp);
        tvRgbLedTimestamp = (TextView) findViewById(R.id.tvRgbLedTimestamp);
        tvAccelValues = (TextView) findViewById(R.id.tvAccelValues);
        progressBarConnection = (ProgressBar) findViewById(R.id.progressBarConnection);
        progressBarAccel = (ProgressBar) findViewById(R.id.progressBarAccel);
        progressBarRgbLed = (ProgressBar) findViewById(R.id.progressBarRgbLed);
        cardViewLed = (CardView) findViewById(R.id.cardViewLed);
        cardViewAccel = (CardView) findViewById(R.id.card_view_accel);


        // default variable values
        rgbLedBinState = 0b0;
        isRgbLed = true;
        mqttConnected = false;
        ledColours = new String[] {"red", "green", "blue"};
        switches = new Switch[] { swLedRed, swLedGreen, swLedBlue };
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
                        progressBarAccel.setVisibility(View.INVISIBLE);
                        progressBarRgbLed.setVisibility(View.INVISIBLE);
                    }
                });

                enableClickableGUIItems(mqttConnected);
            }
        };
        mqttTryToConnect = new Runnable() {
            public void run() {
                Log.d(Connection.LOG_TAG, "Trying to connect to MQTT");

                if (!isNetworkAvailable()) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
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
        enableClickableGUIItems(false);

        // Update LED value in device shadow by setting which led was turned on as shadow's desired state.
        CompoundButton.OnCheckedChangeListener rgbLedSwitchListener = new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                AwsShadow shadow = new AwsShadow();

                // update RGB LED binary state
                rgbLedBinState = 0b0;
                if (swLedRed.isChecked()) {
                    rgbLedBinState |= RgbLed.RED.getValue();
                }
                if (swLedGreen.isChecked()) {
                    rgbLedBinState |= RgbLed.GREEN.getValue();
                }
                if (swLedBlue.isChecked()) {
                    rgbLedBinState |= RgbLed.BLUE.getValue();
                }
                shadow.state.desired.LEDstate = rgbLedBinState;

                // reported and metadata must not be sent along with desired shadow state
                shadow.state.desired.accelUpdate = null;
                shadow.state.reported = null;
                shadow.metadata = null;

                // disable buttons
                enableClickableGUIItems(false);

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
            }
        };
        swLedRed.setOnCheckedChangeListener(rgbLedSwitchListener);
        swLedGreen.setOnCheckedChangeListener(rgbLedSwitchListener);
        swLedBlue.setOnCheckedChangeListener(rgbLedSwitchListener);

        bAccelRefresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AwsShadow shadow = new AwsShadow();
                shadow.state.desired.LEDstate = null;
                shadow.state.desired.accelUpdate = 1;

                // reported and metadata must not be sent along with desired shadow state
                shadow.state.reported = null;
                shadow.metadata = null;

                // disable buttons
                enableClickableGUIItems(false);

                // create message json in format of device shadow
                final String message = new Gson().toJson(shadow, AwsShadow.class);

                // publish message
                mqttConnection.publish(awsConstants.SHADOW_UPDATE, message);

                // show progress bar and wait for certain time
                // if no message from device has been received, display toast message
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        progressBarAccel.setVisibility(View.VISIBLE);
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
                        enableClickableGUIItems(mqttConnected);

                        // display connection status on dashboard
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                switch (status) {
                                    case Connecting:
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
                                            tvConnectionStatus.setText("Reconnecting");
                                            progressBarConnection.setVisibility(View.VISIBLE);
                                        } else {
                                            tvConnectionStatus.setText("Network not available");
                                            progressBarConnection.setVisibility(View.INVISIBLE);
                                        }
                                        break;

                                    case ConnectionLost:
                                        if (throwable != null) {
                                            Log.e(Connection.LOG_TAG, "Connection error.", throwable);
                                            throwable.printStackTrace();
                                        }
                                        tvConnectionStatus.setText("Connection error");
                                        progressBarConnection.setVisibility(View.INVISIBLE);
                                        break;

                                    default:
                                        tvConnectionStatus.setText("Disconnected");
                                        progressBarConnection.setVisibility(View.INVISIBLE);
                                        break;
                                }
                            }
                        });

                        // when MQTT manager is connected to AWS server
                        // subscribe for accepted updates from AWS remote control example shadow
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
                                            // enable GUI items
                                            enableClickableGUIItems(true);

                                            // update LED info (e.g. colours and count)
                                            if (shadow.state.reported.LEDinfo != null) {
                                                if (shadow.state.reported.LEDinfo.isRgbLed != null) {
                                                    isRgbLed = shadow.state.reported.LEDinfo.isRgbLed;
                                                }
                                                if (shadow.state.reported.LEDinfo.colors != null) {
                                                    ledColours = shadow.state.reported.LEDinfo.colors;
                                                    numberOfLEDs = isRgbLed ? 1 : ledColours.length;
                                                }
                                                Log.d(Connection.LOG_TAG, String.format("Device has %d %s", ledColours.length, isRgbLed ? "RGB LED" : "LEDs"));
                                            }

                                            if (numberOfLEDs > 0) {
                                                // display switch text and colour according to device with RGB or regular LEDs
                                                runOnUiThread(new Runnable() {
                                                    @Override
                                                    public void run() {
                                                        cardViewLed.setVisibility(View.VISIBLE);
                                                        tvLedTitle.setText(isRgbLed ? "RGB LED" : "LEDs");

                                                        // hide by default all buttons
                                                        for (int i = 0; i < 3; i++) {
                                                            switches[i].setVisibility(View.INVISIBLE);
                                                        }

                                                        for (int i = 0; i < (isRgbLed ? 3 : numberOfLEDs) && i < 3; i++) {
                                                            switches[i].setText(isRgbLed ? rgbLEDSwitchNames[i] : ("LED #" + (i+1)));
                                                            switches[i].setEnabled(true);
                                                            switches[i].setVisibility(View.VISIBLE);

                                                            switch (ledColours[i]) {
                                                                default:
                                                                    // LED not available
                                                                    switches[i].setTextColor(blackColour);
                                                                    switches[i].setEnabled(false);
                                                                    switches[i].setChecked(false);
                                                                    break;
                                                                case "red":
                                                                    switches[i].setTextColor(redColour);
                                                                    break;
                                                                case "green":
                                                                    switches[i].setTextColor(greenColour);
                                                                    break;
                                                                case "blue":
                                                                    switches[i].setTextColor(blueColour);
                                                                    break;
                                                            }
                                                        }
                                                    }
                                                });

                                                // RGB LED
                                                updateLedColourAfterShadowUpdate(shadow);
                                            } else {
                                                // no LED
                                                runOnUiThread(new Runnable() {
                                                    @Override
                                                    public void run() {
                                                        cardViewLed.setVisibility(View.INVISIBLE);
                                                    }
                                                });
                                            }

                                            // accelerometer
                                            updateAccelValuesAfterShadowUpdate(shadow);

                                            // remove timeout callback
                                            timeoutHandler.removeCallbacks(displayTimeoutToast);

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

                                                        // update LED switches
                                                        Integer state = 0;
                                                        if (shadow.state.desired != null && shadow.state.desired.LEDstate != null) {
                                                            state = shadow.state.desired.LEDstate;
                                                        }
                                                        swLedRed.setChecked((state & RgbLed.RED.getValue()) > 0);
                                                        swLedGreen.setChecked((state & RgbLed.GREEN.getValue()) > 0);
                                                        swLedBlue.setChecked((state & RgbLed.BLUE.getValue()) > 0);

                                                        // hide accelerometer, if no data has been received
                                                        cardViewAccel.setVisibility(shadow.state.reported.accel == null ? View.GONE : View.VISIBLE);
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

    /**
     * Enable {or disable} clickable GUI items (e.g. switches, buttons, atc.)
     * @param enable
     */
    private void enableClickableGUIItems(final boolean enable) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                swLedRed.setEnabled(enable);
                swLedGreen.setEnabled(enable);
                swLedBlue.setEnabled(enable);
                bAccelRefresh.setEnabled(enable);
            }
        });
    }

    /**
     * Change colour of RGB LED after receiving shadow update from subscription.
     * @param shadow AWS shadow
     */
    private void updateLedColourAfterShadowUpdate(final AwsShadow shadow) {
        if (shadow.state.reported != null && shadow.state.reported.LEDstate != null) {
            // save RGB LED shadow state
            rgbLedBinState = shadow.state.reported.LEDstate;

            // update UI items
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    // update timestamp
                    tvRgbLedTimestamp.setText(formatUnixTimeStamp(shadow.metadata.reported.LEDstate.timestamp));

                    // hide progress bar
                    progressBarRgbLed.setVisibility(View.INVISIBLE);

                    // update colour of LED image according to state of device
                    final Bitmap LEDBitmap = changeRGBLedColour(
                            (rgbLedBinState & 0b001) > 0, // LED #1
                            (rgbLedBinState & 0b010) > 0, // LED #2
                            (rgbLedBinState & 0b100) > 0  // LED #3
                    );
                    iLedImage.setImageBitmap(LEDBitmap);
                }
            });
        }
    }

    /**
     * Either change colour of RGB LED according to reported colour or turn on/off LEDs.
     * Currently maximum of three LEDs is possible to control.
     */
    private Bitmap changeRGBLedColour(boolean red, boolean green, boolean blue) {
        // load drawables as bitmaps
        Bitmap led = BitmapFactory.decodeResource(getResources(), R.drawable.ic_led);
        Bitmap ledFilled = BitmapFactory.decodeResource(getResources(), R.drawable.ic_led_filled);

        // create result bitmap
        Bitmap result = Bitmap.createBitmap(isRgbLed ? led.getWidth() : led.getWidth() * numberOfLEDs, led.getHeight(), led.getConfig());
        Canvas canvas = new Canvas(result);

        // draw filled LED bitmap
        for (int i=0; i<numberOfLEDs && i<3; i++) {
            canvas.drawBitmap(ledFilled, led.getWidth() * i, 0f, null);
        }

        // change colour of LED according to reported colour
        if (isRgbLed) {
            final int mixedColor = Color.rgb(red ? 255 : 0, green ? 255 : 0, blue ? 255 : 0);
            canvas.drawColor(mixedColor, PorterDuff.Mode.SRC_ATOP);
        } else {
            final boolean[] states = { red, green, blue };
            for (int i=0; i<numberOfLEDs; i++) {
                if (!states[i]) {
                    continue;
                }
                // select LED area
                canvas.clipRect(led.getWidth() * i, 0, led.getWidth() * (i + 1), led.getHeight(), Region.Op.REPLACE);

                // determine colour
                int colour;
                switch (ledColours[i]) {
                    default: colour = blackColour; break;
                    case "red": colour = redColour; break;
                    case "blue": colour = blueColour; break;
                    case "green": colour = greenColour; break;
                }
                canvas.drawColor(colour, PorterDuff.Mode.SRC_ATOP);
            }
        }

        // set clip rectangle to whole area
        canvas.clipRect(0, 0, led.getWidth() * numberOfLEDs, led.getHeight(), Region.Op.UNION);

        // draw LED border
        Paint blackPaint = new Paint();
        blackPaint.setColorFilter(new PorterDuffColorFilter(blackColour, PorterDuff.Mode.SRC_ATOP));
        for (int i=0; i<numberOfLEDs; i++) {
            canvas.drawBitmap(led, led.getWidth() * i, 0f, blackPaint);
        }

        return result;
    }

    /**
     * Update accelerometer values after receiving shadow update from subscription.
     * @param shadow AWS shadow
     */
    private void updateAccelValuesAfterShadowUpdate(final AwsShadow shadow) {
        if (shadow.state.reported != null){
            if (shadow.state.reported.accel != null) {
                final AwsShadow.State.Reported.Accel accel = shadow.state.reported.accel;

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        cardViewAccel.setVisibility(View.VISIBLE);

                        // update accelerometer values
                        tvAccelValues.setText(String.format("x: %d  y: %d  z: %d", accel.x, accel.y, accel.z));

                        // update timestamp
                        tvAccelTimestamp.setText(formatUnixTimeStamp(shadow.metadata.reported.accel.x.timestamp));

                        // hide progress bar
                        progressBarAccel.setVisibility(View.INVISIBLE);
                    }
                });
            }
        }
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
