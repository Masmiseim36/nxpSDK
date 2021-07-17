package software.amazon.freertos.demo;

import java.util.Objects;
import java.text.SimpleDateFormat;
import java.util.Date;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class WifiInfo {
    private static String[] NETWORK_TYPES = {"Open", "WEP", "WPA", "WPA2", "WPA2-Enterprise", "Other"};
    private String ssid;
    private byte[] bssid;
    private int rssi;
    private int networkType;
    private int index;
    private boolean connected;
    private long timeStamp;

    public String getNetworkTypeName() {
        if (networkType < NETWORK_TYPES.length) {
            return NETWORK_TYPES[networkType];
        }
        else {
            return "N/A";
        }
    }

    public String getTime(String format) {
        if ((null == format) || (true == format.isEmpty()))
        {
            format = "MM-dd-yyyy HH:mm:ss";
        }
        SimpleDateFormat dataFormat = new SimpleDateFormat(format);
        return dataFormat.format(new Date(timeStamp * 1000));
    }

    public WifiInfo(String ssid, byte[] bssid, int rssi, int networkType, int index, boolean connected) {
        this.ssid = ssid;
        this.bssid = bssid;
        this.rssi = rssi;
        this.networkType = networkType;
        this.index = index;
        this.connected = connected;
    }

    public WifiInfo(String ssid, byte[] bssid, int networkType, long timeStamp) {
        this.ssid = ssid;
        this.bssid = bssid;
        this.timeStamp = timeStamp;
        this.networkType = networkType;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        WifiInfo wifiInfo = (WifiInfo) obj;
        return Objects.equals(wifiInfo.getSsid(), ssid);
    }
}
