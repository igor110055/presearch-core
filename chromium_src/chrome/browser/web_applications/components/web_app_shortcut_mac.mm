/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace base {
class FilePath;
}  // namespace base

namespace {
base::FilePath GetLocalizablePresearchAppShortcutsSubdirName();
}

#include "../../../../../../chrome/browser/web_applications/components/web_app_shortcut_mac.mm"  // NOLINT

namespace {
base::FilePath GetLocalizablePresearchAppShortcutsSubdirName() {
  static const char kPresearchBrowserDevelopmentAppDirName[] =
      "Presearch Browser Development Apps.localized";
  static const char kPresearchBrowserAppDirName[] =
      "Presearch Browser Apps.localized";
  static const char kPresearchBrowserBetaAppDirName[] =
      "Presearch Browser Beta Apps.localized";
  static const char kPresearchBrowserDevAppDirName[] =
      "Presearch Browser Dev Apps.localized";
  static const char kPresearchBrowserNightlyAppDirName[] =
      "Presearch Browser Nightly Apps.localized";

  switch (chrome::GetChannel()) {
    case version_info::Channel::STABLE:
      return base::FilePath(kPresearchBrowserAppDirName);
    case version_info::Channel::BETA:
      return base::FilePath(kPresearchBrowserBetaAppDirName);
    case version_info::Channel::DEV:
      return base::FilePath(kPresearchBrowserDevAppDirName);
    case version_info::Channel::CANARY:
      return base::FilePath(kPresearchBrowserNightlyAppDirName);
    case version_info::Channel::UNKNOWN:
      return base::FilePath(kPresearchBrowserDevelopmentAppDirName);
    default:
      NOTREACHED();
      return base::FilePath(kPresearchBrowserDevelopmentAppDirName);
  }
}
}  // namespace
