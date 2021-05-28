/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_PRESEARCH_DEFAULT_EXTENSIONS_HANDLER_H_
#define PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_PRESEARCH_DEFAULT_EXTENSIONS_HANDLER_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "chrome/common/extensions/webstore_install_result.h"
#include "components/prefs/pref_change_registrar.h"
#include "third_party/widevine/cdm/buildflags.h"

class Profile;

class PresearchDefaultExtensionsHandler : public settings::SettingsPageUIHandler {
 public:
  PresearchDefaultExtensionsHandler();
  ~PresearchDefaultExtensionsHandler() override;

 private:
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void GetRestartNeeded(const base::ListValue* args);
  void SetWebTorrentEnabled(const base::ListValue* args);
  void SetHangoutsEnabled(const base::ListValue* args);
  void SetIPFSCompanionEnabled(const base::ListValue* args);
  void SetMediaRouterEnabled(const base::ListValue* args);
  void SetIPFSStorageMax(const base::ListValue* args);
  void LaunchIPFSService(const base::ListValue* args);
  void ShutdownIPFSService(const base::ListValue* args);
#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  void SetPresearchWalletEnabled(const base::ListValue* args);
#endif
  void SetTorEnabled(const base::ListValue* args);
  void IsTorEnabled(const base::ListValue* args);
  void OnTorEnabledChanged();
  void IsTorManaged(const base::ListValue* args);
  void SetWidevineEnabled(const base::ListValue* args);
  void IsWidevineEnabled(const base::ListValue* args);
  void OnWidevineEnabledChanged();
  void IsDecentralizedDnsEnabled(const base::ListValue* args);
  void GetDecentralizedDnsResolveMethodList(const base::ListValue* args);

  void InitializePrefCallbacks();

  bool IsExtensionInstalled(const std::string& extension_id) const;
  void OnInstallResult(const std::string& pref_name,
                       bool success,
                       const std::string& error,
                       extensions::webstore_install::Result result);

  void OnRestartNeededChanged();
  void OnMediaRouterEnabledChanged();
  bool IsRestartNeeded();

#if BUILDFLAG(ENABLE_WIDEVINE)
  bool was_widevine_enabled_ = false;
#endif
  Profile* profile_ = nullptr;
  PrefChangeRegistrar pref_change_registrar_;
#if BUILDFLAG(ENABLE_TOR)
  PrefChangeRegistrar local_state_change_registrar_;
#endif
  base::WeakPtrFactory<PresearchDefaultExtensionsHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PresearchDefaultExtensionsHandler);
};

#endif  // PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_PRESEARCH_DEFAULT_EXTENSIONS_HANDLER_H_
