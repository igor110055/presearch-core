/** Copyright (c) 2020 The Presearch Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */
package org.chromium.chrome.browser;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.net.Uri;
import android.text.TextUtils;

import org.json.JSONException;
import org.json.JSONObject;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.PresearchRewardsBalance;
import org.chromium.chrome.browser.PresearchRewardsHelper;
import org.chromium.chrome.browser.PresearchRewardsNativeWorker;
import org.chromium.chrome.browser.PresearchRewardsObserver;
import org.chromium.chrome.browser.app.PresearchActivity;
import org.chromium.chrome.browser.externalnav.PresearchExternalNavigationHandler;
import org.chromium.components.external_intents.ExternalNavigationParams;

import java.util.Locale;

//used from org.chromium.chrome.browser.externalnav
public class PresearchUphold implements PresearchRewardsObserver {
    public static final String UPHOLD_REDIRECT_URL = "rewards://uphold";
    public static final String UPHOLD_REDIRECT_URL_KEY = "redirect_url";
    public static final String ACTION_VALUE = "authorization";
    public static final String UPHOLD_SUPPORT_URL = "http://uphold.com/en/presearch/support";
    public static final String UPHOLD_ORIGIN_URL = "http://uphold.com";

    // Wallet types
    public static final String UPHOLD = "uphold";

    private static int UNKNOWN_ERROR_CODE = -1;

    private ExternalNavigationParams mExternalNavigationParams;
    private PresearchExternalNavigationHandler mPresearchExternalNavigationHandler;
    private PresearchRewardsNativeWorker rewardsNativeProxy;

    public void CompleteUpholdVerification(ExternalNavigationParams params,
            PresearchExternalNavigationHandler handler) {
        mExternalNavigationParams = params;
        mPresearchExternalNavigationHandler = handler;
        rewardsNativeProxy = PresearchRewardsNativeWorker.getInstance();

        Uri uri = Uri.parse(params.getUrl());
        rewardsNativeProxy.AddObserver(this);
        String path = uri.getPath();
        String query = uri.getQuery();

        if (TextUtils.isEmpty(path) || TextUtils.isEmpty(query)) {
            rewardsNativeProxy.RemoveObserver(this);
            ReleaseDependencies();
            ShowErrorMessageBox(UNKNOWN_ERROR_CODE);
            return;
        }

        // Ledger expects:
        // path: "/uphold/path"
        // query: "?query"
        path = String.format(Locale.US,"/%s/%s",
                PresearchRewardsBalance.WALLET_UPHOLD, path);
        query = String.format(Locale.US,"?%s", query);
        rewardsNativeProxy.ProcessRewardsPageUrl(path, query);
    }

    @Override
    public void OnProcessRewardsPageUrl(int error_code,
            String wallet_type, String action,
            String json_args ) {

        //remove observer
        if (rewardsNativeProxy != null) {
            rewardsNativeProxy.RemoveObserver(this);
        }

        String redirect_url = parseJsonArgs (json_args);
        if (PresearchRewardsNativeWorker.LEDGER_OK == error_code &&
                TextUtils.equals(action, ACTION_VALUE)) {

                    //wallet is verified: redirect to chrome://rewards for now
                    if (TextUtils.isEmpty(redirect_url)) {
                        redirect_url = PresearchActivity.REWARDS_SETTINGS_URL;
                    }
                    mPresearchExternalNavigationHandler.
                            clobberCurrentTabWithFallbackUrl (redirect_url,
                            mExternalNavigationParams);

                    // temporary: open Rewards Panel to show wallet transition state.
                    // remove this step once settings activity has this info
                    PresearchActivity.getPresearchActivity().openRewardsPanel();
                    ReleaseDependencies();
        }
        else {
            ShowErrorMessageBox(error_code);
        }
    }

    private String parseJsonArgs(String json_args) {
        String redirect_url = "";
        try {
            JSONObject jsonObj = new JSONObject(json_args);
            if (jsonObj.has(UPHOLD_REDIRECT_URL_KEY)) {
                redirect_url = jsonObj.getString(UPHOLD_REDIRECT_URL_KEY);
            }
        }
        catch(JSONException e){ }
        return redirect_url;
    }

    private void ReleaseDependencies() {
        mExternalNavigationParams = null;
        mPresearchExternalNavigationHandler = null;
    }

    private void ShowErrorMessageBox (int error_code) {
        String msg = "";
        String msg_title = "";
        Context context = ContextUtils.getApplicationContext();
        AlertDialog.Builder builder = new AlertDialog.Builder(
                PresearchRewardsHelper.getChromeTabbedActivity(),
                R.style.Theme_Chromium_AlertDialog);

        if (PresearchRewardsNativeWorker.BAT_NOT_ALLOWED == error_code) {
            msg = context.getResources().getString(R.string.bat_not_allowed_in_region);
            msg_title = context.getResources().getString(R.string.bat_not_allowed_in_region_title);
        }
        else {
            msg = context.getResources().getString(R.string.wallet_verification_generic_error);
            msg_title = context.getResources().getString(R.string.wallet_verification_generic_error_title);
        }
        builder.setMessage(msg)
                .setTitle (msg_title)
                .setPositiveButton(R.string.ok, (DialogInterface dialog, int which)-> {
                        mPresearchExternalNavigationHandler.
                            clobberCurrentTabWithFallbackUrl (UPHOLD_SUPPORT_URL,
                            mExternalNavigationParams);
                })
                .setOnDismissListener((DialogInterface dialog)-> {
                    ReleaseDependencies();
                });

        AlertDialog dlg = builder.create();
        dlg.show();
    }
}