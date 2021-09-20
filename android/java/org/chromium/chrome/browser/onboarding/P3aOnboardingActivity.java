/**
 * Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.onboarding;

import android.os.Bundle;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.ForegroundColorSpan;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.PresearchRewardsHelper;
import org.chromium.chrome.browser.app.PresearchActivity;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.night_mode.GlobalNightModeStateProviderHolder;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.preferences.PresearchPrefServiceBridge;
import org.chromium.chrome.browser.util.PackageUtils;

public class P3aOnboardingActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_p3a_onboarding);

        boolean isFirstInstall = PackageUtils.isFirstInstall(this);

        TextView p3aOnboardingTitle = findViewById(R.id.p3a_onboarding_title);
        if (isFirstInstall){
          p3aOnboardingTitle.setText(getResources().getString(R.string.p3a_onboarding_title_text_1));
        }

        ImageView p3aOnboardingImg = findViewById(R.id.p3a_onboarding_img);
        if(isFirstInstall){
          p3aOnboardingImg.setImageResource(R.drawable.ic_presearch_logo);
        }

        Button btnContinue = findViewById(R.id.btn_continue);
        btnContinue.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                PresearchPrefServiceBridge.getInstance().setP3AEnabled(false);
                PresearchPrefServiceBridge.getInstance().setP3ANoticeAcknowledged(true);
                OnboardingPrefManager.getInstance().setP3aOnboardingShown(true);
                finish();
            }
        });
    }

    @Override
    public void onBackPressed() {}
}
