/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/settings/presearch_privacy_handler.h"

#include <string>

#include "base/bind.h"
#include "base/values.h"
#include "presearch/common/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "third_party/blink/public/common/peerconnection/webrtc_ip_handling_policy.h"

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "presearch/browser/gcm_driver/presearch_gcm_channel_status.h"
#endif

#if BUILDFLAG(PRESEARCH_P3A_ENABLED)
#include "presearch/components/p3a/pref_names.h"
#endif

PresearchPrivacyHandler::PresearchPrivacyHandler() {
  local_state_change_registrar_.Init(g_browser_process->local_state());
#if BUILDFLAG(PRESEARCH_P3A_ENABLED)
  local_state_change_registrar_.Add(
      presearch::kP3AEnabled,
      base::Bind(&PresearchPrivacyHandler::OnP3AEnabledChanged,
                 base::Unretained(this)));
#endif
}

PresearchPrivacyHandler::~PresearchPrivacyHandler() {
  local_state_change_registrar_.RemoveAll();
}

void PresearchPrivacyHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());

#if BUILDFLAG(PRESEARCH_P3A_ENABLED)
  web_ui()->RegisterMessageCallback(
      "setP3AEnabled", base::BindRepeating(&PresearchPrivacyHandler::SetP3AEnabled,
                                           base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getP3AEnabled", base::BindRepeating(&PresearchPrivacyHandler::GetP3AEnabled,
                                           base::Unretained(this)));
#endif
}

// static
void PresearchPrivacyHandler::AddLoadTimeData(content::WebUIDataSource* data_source,
                                          Profile* profile) {
#if BUILDFLAG(USE_GCM_FROM_PLATFORM)
  data_source->AddBoolean("pushMessagingEnabledAtStartup",
                          true);
#else
  gcm::PresearchGCMChannelStatus* gcm_channel_status =
      gcm::PresearchGCMChannelStatus::GetForProfile(profile);

  DCHECK(gcm_channel_status);
  data_source->AddBoolean("pushMessagingEnabledAtStartup",
                          gcm_channel_status->IsGCMEnabled());
#endif
}

#if BUILDFLAG(PRESEARCH_P3A_ENABLED)
void PresearchPrivacyHandler::SetP3AEnabled(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);

  bool enabled;
  args->GetBoolean(0, &enabled);

  PrefService* local_state = g_browser_process->local_state();
  local_state->SetBoolean(presearch::kP3AEnabled, enabled);
}

void PresearchPrivacyHandler::GetP3AEnabled(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);

  PrefService* local_state = g_browser_process->local_state();
  bool enabled = local_state->GetBoolean(presearch::kP3AEnabled);

  AllowJavascript();
  ResolveJavascriptCallback(args->GetList()[0].Clone(), base::Value(enabled));
}

void PresearchPrivacyHandler::OnP3AEnabledChanged() {
  if (IsJavascriptAllowed()) {
    PrefService* local_state = g_browser_process->local_state();
    bool enabled = local_state->GetBoolean(presearch::kP3AEnabled);

    FireWebUIListener("p3a-enabled-changed", base::Value(enabled));
  }
}
#endif
