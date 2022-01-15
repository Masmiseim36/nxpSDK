package software.amazon.freertos.demo;

import android.app.Activity;
import android.app.AutomaticZenRule;
import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.CardView;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import software.amazon.freertos.amazonfreertossdk.AmazonFreeRTOSDevice;
import software.amazon.freertos.amazonfreertossdk.AmazonFreeRTOSManager;
import software.amazon.freertos.amazonfreertossdk.NetworkConfigCallback;
import software.amazon.freertos.amazonfreertossdk.networkconfig.*;

import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;
import com.google.gson.Gson;
import com.nxp.awsremotecontrol.iot.AwsShadow;
import com.nxp.awsremotecontrol.iot.RgbLed;
import com.nxp.awsremotecontrol.mqtt.Connection;
import com.nxp.awsremotecontrol.iot.AwsConstants;

import static android.support.v7.widget.helper.ItemTouchHelper.ACTION_STATE_SWIPE;
import static android.support.v7.widget.helper.ItemTouchHelper.DOWN;
import static android.support.v7.widget.helper.ItemTouchHelper.LEFT;
import static android.support.v7.widget.helper.ItemTouchHelper.UP;

public class ViewAwsShadowFragment extends Fragment {
    private static final String TAG = "ViewAwsShadowFragment";
    private static final String DIALOG_TAG = "ViewAwsShadowDialogTag";
    private static final int SAVED_NETWORK_RSSI = -100;
    private static final int REQUEST_CODE = 0;
    private RecyclerView mWifiInfoRecyclerView;
    private WifiInfoAdapter mWifiInfoAdapter;
    private List<WifiInfo> mWifiInfoList = new ArrayList<>();
    private HashMap<String, WifiInfo> mBssid2WifiInfoMap = new HashMap<>();
    private Handler mHandler = new Handler(Looper.getMainLooper());
    private String preferencesUri;

    /** MQTT connection */
    private Connection mqttConnection = null;

    /** AWS related constants (endpoint, cognito, etc. */
    private AwsConstants awsConstants;

    Runnable mqttTryToConnect;
    Runnable displayTimeoutToast;

    TextView tvConnectionStatus;
    ProgressBar progressBarConnection;

    Handler timeoutHandler;

    AWSIotMqttClientStatusCallback mqttClientStatusCallback;
    AWSIotMqttNewMessageCallback subscriptionCallback;

    boolean mqttConnected = false;
    boolean subscribed = false;

    private class WifiInfoHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        private TextView mSsidTextView;
        private TextView mTimeTextView;
        private TextView mBssidTextView;
        private TextView mNetworkTypeTextView;
        private WifiInfo mWifiInfo;
        private Fragment mHostingFragment;

        public WifiInfoHolder(LayoutInflater inflater, ViewGroup parent, Fragment fragment) {
            super(inflater.inflate(R.layout.list_aws_wifi_info, parent, false));
            mHostingFragment = fragment;
            mSsidTextView = (TextView) itemView.findViewById(R.id.ssid_name);
            mTimeTextView = (TextView) itemView.findViewById(R.id.timestamp);
            mBssidTextView = (TextView) itemView.findViewById(R.id.bssid_name);
            mNetworkTypeTextView = (TextView) itemView.findViewById(R.id.network_type);
            itemView.setOnClickListener(this);
        }

        public void bind(WifiInfo wifiInfo) {
            mWifiInfo = wifiInfo;
            mSsidTextView.setText(mWifiInfo.getSsid());
            // mRssiTextView.setText(getResources().getString(R.string.rssi_value, mWifiInfo.getRssi()));
            mTimeTextView.setText(getResources().getString(R.string.timestamp) + " " + mWifiInfo.getTime(null));
            mBssidTextView.setText(bssidToString(mWifiInfo.getBssid()));
            mNetworkTypeTextView.setText(mWifiInfo.getNetworkTypeName());

            int colorPrimary = getResources().getColor(R.color.colorPrimary, null);
            mSsidTextView.setTextColor(colorPrimary);
            mBssidTextView.setTextColor(colorPrimary);
            mTimeTextView.setTextColor(colorPrimary);
            mNetworkTypeTextView.setTextColor(colorPrimary);

//            mNetworkTypeTextView.setText(mWifiInfo.getNetworkTypeName());
//            if (SAVED_NETWORK_RSSI == mWifiInfo.getRssi()) {
//                int colorAccent = getResources().getColor(R.color.colorAccent, null);
//                mSsidTextView.setTextColor(colorAccent);
//                mBssidTextView.setTextColor(colorAccent);
//                mRssiTextView.setTextColor(colorAccent);
//                mNetworkTypeTextView.setTextColor(colorAccent);
//            }
//            if (mWifiInfo.isConnected()) {
//                int colorPrimary = getResources().getColor(R.color.colorPrimary, null);
//                mSsidTextView.setTextColor(colorPrimary);
//                mBssidTextView.setTextColor(colorPrimary);
//                mRssiTextView.setTextColor(colorPrimary);
//                mNetworkTypeTextView.setTextColor(colorPrimary);
//            }
        }

        @Override
        public void onClick(View v) {
//            FragmentManager manager = getFragmentManager();
//            WifiCredentialFragment dialog = WifiCredentialFragment.newInstance(
//                    mWifiInfo.getSsid(), bssidToString(mWifiInfo.getBssid()));
//            dialog.setTargetFragment(mHostingFragment, REQUEST_CODE);
//            dialog.show(manager, DIALOG_TAG);
        }
    }

    private class WifiInfoAdapter extends RecyclerView.Adapter<WifiInfoHolder> {
        private List<WifiInfo> mWifiInfoList;
        private Fragment mHostingFragment;

        public WifiInfoAdapter(List<WifiInfo> wifiInfos, Fragment fragment) {
            mWifiInfoList = wifiInfos;
            mHostingFragment = fragment;
        }

        @Override
        public WifiInfoHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            LayoutInflater layoutInflater = LayoutInflater.from(getActivity());
            return new WifiInfoHolder(layoutInflater, parent, mHostingFragment);
        }

        @Override
        public void onBindViewHolder(WifiInfoHolder holder, int position) {
            WifiInfo wifiInfo = mWifiInfoList.get(position);
            holder.bind(wifiInfo);
        }

        @Override
        public int getItemCount() {
            return mWifiInfoList.size();
        }

        public void deleteItem(int position) {
            mWifiInfoList.remove(position);
            notifyItemRemoved(position);
        }

        public WifiInfo getItem(int position) {
            return mWifiInfoList.get(position);
        }

        public void moveItem(int oldPosition, int newPosition) {
            WifiInfo oldWifiInfo = mWifiInfoList.get(oldPosition);
            mWifiInfoList.remove(oldPosition);
            mWifiInfoList.add(newPosition, oldWifiInfo);
            notifyItemMoved(oldPosition, newPosition);
        }

        public void reorderItem(int fromIndex, int toIndex) {
//            if (fromIndex != toIndex) {
//                Log.d(TAG, "Reordering item: [" + fromIndex + "] -> [" + toIndex + "]");
//                editNetwork(fromIndex, toIndex);
//            }
        }
    }

    private class DragSwipeController extends ItemTouchHelper.SimpleCallback {

        private WifiInfoAdapter mAdapter;
        private Drawable mDeleteIcon;
        private final ColorDrawable mDeleteBackground;
        private boolean mMoving;
        private int mFromIndex, mToIndex;

        public DragSwipeController(WifiInfoAdapter adapter) {
            super(UP|DOWN, LEFT);
            mAdapter = adapter;
            mDeleteIcon = ContextCompat.getDrawable(getContext(), R.drawable.ic_delete);
            mDeleteBackground = new ColorDrawable(Color.RED);
            mMoving = false;
        }

        @Override
        public void onSwiped(RecyclerView.ViewHolder viewHolder, int direction) {
            int position = viewHolder.getAdapterPosition();
            mAdapter.deleteItem(position);
        }

        @Override
        public boolean onMove(RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder,
                              RecyclerView.ViewHolder target) {
            if (!mMoving) {
                mFromIndex = viewHolder.getAdapterPosition();
                mMoving = true;
            }
            mAdapter.moveItem(viewHolder.getAdapterPosition(), target.getAdapterPosition());
            return true;
        }

        @Override
        public void clearView(RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder) {
            super.clearView(recyclerView, viewHolder);
            if (mMoving) {
                mToIndex = viewHolder.getAdapterPosition();
                mMoving = false;
                mAdapter.reorderItem(mFromIndex, mToIndex);
            }
        }

        @Override
        public int getSwipeDirs(RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder) {
            /*
            int position = viewHolder.getAdapterPosition();
            WifiInfo wifiInfo = mAdapter.getItem(position);
            if (SAVED_NETWORK_RSSI != wifiInfo.getRssi()) {
                return 0;
            }
            return super.getSwipeDirs(recyclerView, viewHolder);
            */
            return 0;
        }

        @Override
        public void onChildDraw(Canvas c, RecyclerView recyclerView, RecyclerView.ViewHolder viewHolder,
                                float dX, float dY, int actionState, boolean isCurrentlyActive) {

            View itemView = viewHolder.itemView;
            if (actionState == ACTION_STATE_SWIPE) {
                float alpha = 1 - (Math.abs(dX) / recyclerView.getWidth());
                itemView.setAlpha(alpha);
            }

            int iconMargin = (itemView.getHeight() - mDeleteIcon.getIntrinsicHeight()) / 2;
            int iconTop = itemView.getTop() + (itemView.getHeight() - mDeleteIcon.getIntrinsicHeight()) / 2;
            int iconBottom = iconTop + mDeleteIcon.getIntrinsicHeight();

            int iconLeft = itemView.getRight() - iconMargin - mDeleteIcon.getIntrinsicWidth();
            int iconRight = itemView.getRight() - iconMargin;
            mDeleteIcon.setBounds(iconLeft, iconTop, iconRight, iconBottom);

            mDeleteBackground.setBounds(itemView.getRight() + ((int) dX) ,
                    itemView.getTop(), itemView.getRight(), itemView.getBottom());

            mDeleteBackground.draw(c);
            mDeleteIcon.draw(c);
            super.onChildDraw(c, recyclerView, viewHolder, dX, dY, actionState, isCurrentlyActive);
        }
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        preferencesUri = getActivity().getIntent().getStringExtra(ViewAwsShadowActivity.PROFENCES_URI);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_view_aws_device, container, false);
        mWifiInfoRecyclerView = (RecyclerView) view.findViewById(R.id.wifi_set_recycler_view);
        mWifiInfoRecyclerView.setLayoutManager(new LinearLayoutManager(getActivity(), LinearLayoutManager.VERTICAL, false));
        mWifiInfoAdapter = new WifiInfoAdapter(mWifiInfoList, this);
        mWifiInfoRecyclerView.setAdapter(mWifiInfoAdapter);

        DragSwipeController dragSwipeController = new DragSwipeController(mWifiInfoAdapter);
        ItemTouchHelper itemTouchHelper = new ItemTouchHelper(dragSwipeController);
        itemTouchHelper.attachToRecyclerView(mWifiInfoRecyclerView);

        if (null == mqttConnection)
        {
            createMqttClient(view);
        }

        return view;
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.i(TAG,"onResume: unsubscribe");

        if ((true == subscribed) && (null != mqttConnection))
        {
            // unsubscribe for all accepted changes (used for accelerometer values)
            mqttConnection.subscribe(awsConstants.SHADOW_UPDATE_ACCEPTED, subscriptionCallback);
            mqttConnection.subscribe(awsConstants.SHADOW_GET_ACCEPTED, subscriptionCallback);
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.i(TAG,"onPause: unsubscribe");

        if ((true == subscribed) && (null != mqttConnection))
        {
            // unsubscribe for all accepted changes (used for accelerometer values)
            mqttConnection.unsubscribe(awsConstants.SHADOW_UPDATE_ACCEPTED);
            mqttConnection.unsubscribe(awsConstants.SHADOW_GET_ACCEPTED);
        }
    }

    @Override
    public void onStop() {
        super.onStop();
        Log.i(TAG,"onStop: unsubscribe");

        if ((true == subscribed) && (null != mqttConnection))
        {
            // unsubscribe for all accepted changes (used for accelerometer values)
            mqttConnection.unsubscribe(awsConstants.SHADOW_UPDATE_ACCEPTED);
            mqttConnection.unsubscribe(awsConstants.SHADOW_GET_ACCEPTED);
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG,"onDestroy: disconnect from AWS");
        if (null != mqttConnection)
        {
            mqttConnection.disconnect();
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode != Activity.RESULT_OK) {
            return;
        }
        if (requestCode == REQUEST_CODE) {
            String pw = data.getStringExtra(WifiCredentialFragment.EXTRA_WIFI_PW);
            String bssid = data.getStringExtra(WifiCredentialFragment.EXTRA_WIFI_BSSID);
//            saveNetwork(bssid, pw);
        }
    }
    /**
     * Check if any kind of network (WiFi or mobile) connection is available
     */
    private boolean isNetworkAvailable() {
        final ConnectivityManager connectivity = (ConnectivityManager) getActivity().getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivity != null) {
            final NetworkInfo infos = connectivity.getActiveNetworkInfo();
            if ((infos != null) && (infos.getState() == NetworkInfo.State.CONNECTED)) {
                return true;
            }
        }
        return false;
    }

    public byte[] bssidToBytes(String bssid){

        String newBssid = bssid.replace(":", "");
        int byteLen = newBssid.length() / 2;
        byte[] ret = new byte[byteLen];
        for (int i = 0; i < byteLen; i++) {
            int intVal = Integer.decode("0x" + newBssid.substring(i * 2, i * 2 + 1) + newBssid.substring(i * 2 + 1, i * 2 + 2));
            ret[i] = Byte.valueOf((byte)intVal);
        }
        return ret;
    }

    private String bssidToString(byte[] bssid) {
        StringBuilder sb = new StringBuilder(18);
        for (byte b : bssid) {
            if (sb.length() > 0)
                sb.append(':');
            sb.append(String.format("%02x", b));
        }
        return sb.toString();
    }

    private void createMqttClient(View view)
    {
        Context context = getActivity().getApplicationContext();
        awsConstants = new AwsConstants(context, getString(R.string.preference_file_key));

        if (awsConstants.checkPreferencesAndValidate())
        {
            awsConstants.refreshShadowUrls();
        }
        else
        {
            Uri uriFile = Uri.parse(preferencesUri);
            awsConstants.loadPreferencesFromFile(uriFile, null);
        }

        mqttConnection = new Connection(getActivity().getApplicationContext(), awsConstants);

        tvConnectionStatus = (TextView) view.findViewById(R.id.tvConnectionStatus);
        progressBarConnection = (ProgressBar) view.findViewById(R.id.progressBarConnection);

        timeoutHandler = new Handler();

        mqttTryToConnect = new Runnable() {
            public void run() {
                Log.d(TAG, "Trying to connect to MQTT");

                if (!isNetworkAvailable()) {
                    getActivity().runOnUiThread(new Runnable() {
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
                    getActivity().runOnUiThread(new Runnable() {
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
                        // Invalid connect parameter
                    }
                }
            }
        };
        // create MQTT client status callback
        mqttClientStatusCallback = new AWSIotMqttClientStatusCallback() {
            @Override
            public void onStatusChanged(final AWSIotMqttClientStatus status,
                                        final Throwable throwable) {
                Log.d(TAG, "MQTT connection status: " + String.valueOf(status));

                // refreshShadowUrls GUI
                mqttConnected = (status == AWSIotMqttClientStatus.Connected);

                // display connection status on dashboard
                getActivity().runOnUiThread(new Runnable() {
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
                                    Log.e(TAG, "Connection error.", throwable);
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
                                    Log.e(TAG, "Connection error.", throwable);
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
                    subscriptionCallback = new AWSIotMqttNewMessageCallback() {
                        @Override
                        public void onMessageArrived(final String topic, final byte[] data) {
                            try {
                                String message = new String(data, "UTF-8");

                                // parse message from JSON into class
                                Gson gson = new Gson();
                                final AwsShadow shadow = gson.fromJson(message, AwsShadow.class);

                                Log.i(TAG, String.format("Received message \"%s\" from topic \"%s\"", message, topic));

                                if (shadow != null && shadow.state.reported != null) {
                                    // remove timeout callback
                                    timeoutHandler.removeCallbacks(displayTimeoutToast);

                                    // last known shadow state received
                                    if (topic.equals(awsConstants.SHADOW_GET_ACCEPTED) || topic.equals(awsConstants.SHADOW_UPDATE_ACCEPTED)) {
                                        getActivity().runOnUiThread(new Runnable() {
                                            @Override
                                            public void run() {
                                                Toast.makeText(
                                                        getContext().getApplicationContext(),
                                                        "Last known device shadow state has been received.",
                                                        Toast.LENGTH_LONG
                                                ).show();
                                            }
                                        });
                                        mHandler.post(new Runnable() {
                                            @Override
                                            public void run() {
                                                Log.i(TAG, String.format("Remove all item from the view"));
                                                while (false == mWifiInfoList.isEmpty())
                                                {
                                                    mWifiInfoList.remove(0);
                                                    mWifiInfoAdapter.notifyDataSetChanged();
                                                }

                                                try
                                                {
                                                    Log.i(TAG, String.format("Add new item to the view"));
                                                    for (int i = 0;i < shadow.state.reported.wifi_list.count;i++) {
                                                        WifiInfo wifiInfo = new WifiInfo(
                                                                shadow.state.reported.wifi_list.wifi[i].ssid,
                                                                bssidToBytes(shadow.state.reported.wifi_list.wifi[i].bssid),
                                                                shadow.state.reported.wifi_list.wifi[i].security,
                                                                shadow.metadata.reported.wifi_list.wifi[i].ssid.timestamp
                                                        );
                                                        if (!mWifiInfoList.contains(wifiInfo)) {
                                                            Log.i(TAG, String.format("Add new item %s", wifiInfo.toString()));
                                                            mWifiInfoList.add(wifiInfo);
                                                            mBssid2WifiInfoMap.put(bssidToString(wifiInfo.getBssid()), wifiInfo);
                                                            mWifiInfoAdapter.notifyDataSetChanged();
                                                        }
                                                    }
                                                }
                                                catch (Exception e)
                                                {
                                                    Log.e(TAG, "Could not parse message JSON.", e);
                                                }
                                            }
                                        });
                                    }
                                } else {
                                    getActivity().runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            if (topic.equals(awsConstants.SHADOW_GET_ACCEPTED)) {
                                                Toast.makeText(
                                                        getContext().getApplicationContext(),
                                                        "Device shadow has no reported state.",
                                                        Toast.LENGTH_LONG
                                                ).show();
                                            }
                                        }
                                    });
                                }
                            } catch (UnsupportedEncodingException e) {
                                Log.e(TAG, "Message encoding error.", e);
                            } catch (Exception e) {
                                Log.e(TAG, "Could not parse message JSON.", e);
                            }
                        }
                    };

                    // subscribe for all accepted changes (used for accelerometer values)
                    mqttConnection.subscribe(awsConstants.SHADOW_UPDATE_ACCEPTED, subscriptionCallback);

                    // get device shadow after successful connection
                    mqttConnection.subscribe(awsConstants.SHADOW_GET_ACCEPTED, subscriptionCallback);

                    subscribed = true;

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

        // try to establish MQTT connection
        timeoutHandler.postDelayed(mqttTryToConnect, 1);
    }
}
