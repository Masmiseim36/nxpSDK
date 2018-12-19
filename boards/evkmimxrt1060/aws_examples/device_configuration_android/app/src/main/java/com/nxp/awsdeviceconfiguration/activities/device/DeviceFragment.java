/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.activities.device;

import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.TextInputLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.TextView;

import com.nxp.awsdeviceconfiguration.R;
import com.nxp.awsdeviceconfiguration.iot.AwsConstants;

public class DeviceFragment extends Fragment {
    private static final String TAG = "AWS: Device Fragment";

    protected FragmentActivity activity;
    protected AwsConstants aws;

    // GUI items
    protected AlertDialog progressDialog;
    private TextView statusMessage;

    /**
     * Edit text validator class
     */
    protected class MyTextWatcher implements TextWatcher {
        private Activity activity;
        private EditText editText;
        private TextInputLayout layout;

        MyTextWatcher(Activity activity, EditText editText, TextInputLayout layout) {
            this.activity = activity;
            this.editText = editText;
            this.layout = layout;
        }

        @Override
        public void beforeTextChanged(CharSequence s, int start, int count, int after) {}

        @Override
        public void onTextChanged(CharSequence s, int start, int before, int count) {}

        @Override
        public void afterTextChanged(Editable s) {
            if (editText == null || layout == null) {
                Log.e(TAG, "Validated edit text's cannot be null!");
                return;
            }
            validateEditText(activity, editText, layout);
        }
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setHasOptionsMenu(true);

        activity = getActivity();
        if (activity != null) {
            aws = new AwsConstants(activity);

            // progress dialog
            final View progressDialogView = activity.getLayoutInflater().inflate(R.layout.dialog_progress, null);
            statusMessage = progressDialogView.findViewById(R.id.statusMessage);
            progressDialog = new AlertDialog.Builder(activity)
                    .setTitle("Please wait")
                    .setView(progressDialogView)
                    .setCancelable(false)
                    .create();
        }
    }

    /**
     * Show or hide back button in action bar
     * @param show True if back button should be shown
     */
    protected void showBackButton(boolean show) {
        final ActionBar bar = ((AppCompatActivity) activity).getSupportActionBar();
        if (bar != null) {
            bar.setDisplayHomeAsUpEnabled(show);
            bar.setDisplayShowHomeEnabled(show);
        }
    }

    /**
     * Validate edit text to be not empty
     * @param editText Edit text to be validated
     * @param layout Edit text layout
     * @return True if edit text is valid or if it is not visible
     */
    protected boolean validateEditText(Activity activity, EditText editText, TextInputLayout layout) {
        if (editText.getVisibility() != View.VISIBLE) {
            // edit text must be visible for validation
            return true;
        }

        if (editText.getText().toString().trim().isEmpty()) {
            layout.setError("Input field cannot be empty!");

            if (editText.requestFocus()) {
                activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
            }

            return false;
        }

        layout.setErrorEnabled(false);

        return true;
    }

    /**
     * Set status message of progress dialog
     * @param msg Message to be displayed
     */
    protected void setStatusMessage(String msg) {
        Log.d(TAG, msg);

        if (!progressDialog.isShowing()) {
            progressDialog.show();
        }

        statusMessage.setText(msg);
    }

    /**
     * Show error dialog
     * @param msg Error message to be shown
     */
    protected void showErrorDialog(String msg) {
        Log.e(TAG, msg);

        if (progressDialog.isShowing()) {
            progressDialog.dismiss();
        }

        new AlertDialog.Builder(activity)
                .setTitle("Error!")
                .setMessage(msg)
                .setIcon(getResources().getDrawable(android.R.drawable.ic_dialog_alert, null))
                .setPositiveButton("OK", (dialog, which) -> dialog.cancel())
                .show();
    }

}
