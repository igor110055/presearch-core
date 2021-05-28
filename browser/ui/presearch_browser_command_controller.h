/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_PRESEARCH_BROWSER_COMMAND_CONTROLLER_H_
#define PRESEARCH_BROWSER_UI_PRESEARCH_BROWSER_COMMAND_CONTROLLER_H_

#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_command_controller.h"

// This namespace is needed for a chromium_src override
namespace chrome {

class PresearchBrowserCommandController : public chrome::BrowserCommandController {
 public:
  explicit PresearchBrowserCommandController(Browser* browser);

#if BUILDFLAG(ENABLE_TOR)
  void UpdateCommandForTor();
#endif

 private:
  // Overriden from CommandUpdater:
  bool SupportsCommand(int id) const override;
  bool IsCommandEnabled(int id) const override;
  bool ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition disposition,
      base::TimeTicks time_stamp = base::TimeTicks::Now()) override;
  void AddCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(CommandObserver* observer) override;
  bool UpdateCommandEnabled(int id, bool state) override;

  void InitPresearchCommandState();
  void UpdateCommandForPresearchRewards();
  void UpdateCommandForPresearchAdblock();
  void UpdateCommandForWebcompatReporter();
  void UpdateCommandForPresearchSync();
  void UpdateCommandForPresearchWallet();
  void UpdateCommandForSidebar();
#if BUILDFLAG(IPFS_ENABLED)
  void UpdateCommandForIpfs();
#endif
  bool ExecutePresearchCommandWithDisposition(int id,
                                          WindowOpenDisposition disposition,
                                          base::TimeTicks time_stamp);

  Browser* const browser_;

  CommandUpdaterImpl presearch_command_updater_;

  DISALLOW_COPY_AND_ASSIGN(PresearchBrowserCommandController);
};

}   // namespace chrome

#endif  // PRESEARCH_BROWSER_UI_PRESEARCH_BROWSER_COMMAND_CONTROLLER_H_
