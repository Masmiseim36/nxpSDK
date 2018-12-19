/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.activities.device;

import android.app.Activity;
import android.content.Context;
import android.support.annotation.NonNull;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.nxp.awsdeviceconfiguration.R;
import com.nxp.awsdeviceconfiguration.nsd.DeviceInfo;

import java.util.List;

public class DevicesListAdapter extends ArrayAdapter<DeviceInfo> {

    private Activity ctx;
    private List<DeviceInfo> list;
    private int resourceId;

    DevicesListAdapter(Context context, int textViewResourceId, List<DeviceInfo> devices) {
        super(context, textViewResourceId, devices);

        list = devices;
        resourceId = textViewResourceId;
        ctx = (Activity) context;
    }

    @NonNull
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            LayoutInflater inflater = ctx.getLayoutInflater();
            convertView = inflater.inflate(resourceId, null);
        }

        View rowView = convertView;

        if (list != null && list.size() > 0) {
            DeviceInfo item = list.get(position);

            final TextView itemName = convertView.findViewById(R.id.itemName);
            itemName.setText(item.getServiceName());

            final TextView itemIpAddress = convertView.findViewById(R.id.itemIpAddress);
            itemIpAddress.setText(item.getIp().getHostAddress());
        }

        return rowView;
    }
}





