/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/themes/presearch_dark_mode_utils.h"

namespace dark_mode {

void MigratePresearchDarkModePrefs(Profile* profile) {
}

void RegisterPresearchDarkModeLocalStatePrefs(PrefRegistrySimple* registry) {
}

void RegisterPresearchDarkModePrefsForMigration(
    user_prefs::PrefRegistrySyncable* registry) {
}

bool SystemDarkModeEnabled() {
  return false;
}

void SetUseSystemDarkModeEnabledForTest(bool enabled) {
}

std::string GetStringFromPresearchDarkModeType(PresearchDarkModeType type) {
  return "Default";
}

void SetPresearchDarkModeType(const std::string& type) {
}

void SetPresearchDarkModeType(PresearchDarkModeType type) {
}

PresearchDarkModeType GetActivePresearchDarkModeType() {
  return PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_DEFAULT;
}

PresearchDarkModeType GetPresearchDarkModeType() {
  return PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_DEFAULT;
}

base::Value GetPresearchDarkModeTypeList() {
  return base::Value(base::Value::Type::LIST);
}

void SetSystemDarkMode(PresearchDarkModeType type) {
}

}  // namespace dark_mode
