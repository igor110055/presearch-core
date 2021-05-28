/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/settings/default_presearch_shields_handler.h"

#include <string>

#include "base/bind.h"
#include "base/values.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_util.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/web_ui.h"
#include "url/gurl.h"

using presearch_shields::ControlType;
using presearch_shields::ControlTypeFromString;
using presearch_shields::ControlTypeToString;

void DefaultPresearchShieldsHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  web_ui()->RegisterMessageCallback(
      "isAdControlEnabled",
      base::BindRepeating(&DefaultPresearchShieldsHandler::IsAdControlEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setAdControlType",
      base::BindRepeating(&DefaultPresearchShieldsHandler::SetAdControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isFirstPartyCosmeticFilteringEnabled",
      base::BindRepeating(
          &DefaultPresearchShieldsHandler::IsFirstPartyCosmeticFilteringEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setCosmeticFilteringControlType",
      base::BindRepeating(
          &DefaultPresearchShieldsHandler::SetCosmeticFilteringControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getCookieControlType",
      base::BindRepeating(&DefaultPresearchShieldsHandler::GetCookieControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setCookieControlType",
      base::BindRepeating(&DefaultPresearchShieldsHandler::SetCookieControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getFingerprintingControlType",
      base::BindRepeating(
          &DefaultPresearchShieldsHandler::GetFingerprintingControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setFingerprintingControlType",
      base::BindRepeating(
          &DefaultPresearchShieldsHandler::SetFingerprintingControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setHTTPSEverywhereEnabled",
      base::BindRepeating(
          &DefaultPresearchShieldsHandler::SetHTTPSEverywhereEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setNoScriptControlType",
      base::BindRepeating(&DefaultPresearchShieldsHandler::SetNoScriptControlType,
                          base::Unretained(this)));
}

void DefaultPresearchShieldsHandler::IsAdControlEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);

  ControlType setting = presearch_shields::GetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_), GURL());

  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0].Clone(),
      base::Value(setting == ControlType::BLOCK));
}

void DefaultPresearchShieldsHandler::SetAdControlType(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool value;
  args->GetBoolean(0, &value);

  presearch_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value ? ControlType::BLOCK
            : ControlType::ALLOW,
      GURL(),
      g_browser_process->local_state());
}

void DefaultPresearchShieldsHandler::IsFirstPartyCosmeticFilteringEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);

  bool enabled = presearch_shields::IsFirstPartyCosmeticFilteringEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      GURL());

  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0].Clone(),
      base::Value(enabled));
}

void DefaultPresearchShieldsHandler::SetCosmeticFilteringControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  std::string value;
  args->GetString(0, &value);

  presearch_shields::SetCosmeticFilteringControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value),
      GURL(),
      g_browser_process->local_state());
}

void DefaultPresearchShieldsHandler::GetCookieControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);

  ControlType setting = presearch_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      GURL());

  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0].Clone(),
      base::Value(ControlTypeToString(setting)));
}

void DefaultPresearchShieldsHandler::SetCookieControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  std::string value;
  args->GetString(0, &value);

  presearch_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value),
      GURL(),
      g_browser_process->local_state());
}

void DefaultPresearchShieldsHandler::GetFingerprintingControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);

  ControlType setting = presearch_shields::GetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      GURL());

  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0].Clone(),
      base::Value(ControlTypeToString(setting)));
}

void DefaultPresearchShieldsHandler::SetFingerprintingControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  std::string value;
  args->GetString(0, &value);

  presearch_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value),
      GURL(),
      g_browser_process->local_state());
}

void DefaultPresearchShieldsHandler::SetHTTPSEverywhereEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool value;
  args->GetBoolean(0, &value);

  presearch_shields::SetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value,
      GURL(),
      g_browser_process->local_state());
}

void DefaultPresearchShieldsHandler::SetNoScriptControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool value;
  args->GetBoolean(0, &value);

  presearch_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value ? ControlType::BLOCK
            : ControlType::ALLOW,
      GURL(),
      g_browser_process->local_state());
}
