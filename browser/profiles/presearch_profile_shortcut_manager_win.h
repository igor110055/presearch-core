/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PROFILES_PRESEARCH_PROFILE_SHORTCUT_MANAGER_WIN_H_
#define PRESEARCH_BROWSER_PROFILES_PRESEARCH_PROFILE_SHORTCUT_MANAGER_WIN_H_

#include "chrome/browser/profiles/profile_shortcut_manager_win.h"

class PresearchProfileShortcutManagerWin : public ProfileShortcutManagerWin {
 public:
  explicit PresearchProfileShortcutManagerWin(ProfileManager* manager);
  ~PresearchProfileShortcutManagerWin() override = default;

  void GetShortcutProperties(const base::FilePath& profile_path,
                             base::CommandLine* command_line,
                             base::string16* name,
                             base::FilePath* icon_path) override;

 private:
  ProfileManager* profile_manager_;

  DISALLOW_COPY_AND_ASSIGN(PresearchProfileShortcutManagerWin);
};

#endif  // PRESEARCH_BROWSER_PROFILES_PRESEARCH_PROFILE_SHORTCUT_MANAGER_WIN_H_
