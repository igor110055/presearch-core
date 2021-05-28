/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/presearch_browser_command_controller.h"

#include <vector>

#include "presearch/app/presearch_command_ids.h"
#include "presearch/browser/profiles/profile_util.h"
#include "presearch/browser/ui/presearch_pages.h"
#include "presearch/browser/ui/browser_commands.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_rewards/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_sync/buildflags/buildflags.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"

#if BUILDFLAG(ENABLE_PRESEARCH_SYNC)
#include "components/sync/driver/sync_driver_switches.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "presearch/browser/ui/sidebar/sidebar_utils.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/components/ipfs/ipfs_constants.h"
#include "presearch/components/ipfs/ipfs_utils.h"
#endif

namespace {

bool IsPresearchCommands(int id) {
  return id >= IDC_PRESEARCH_COMMANDS_START && id <= IDC_PRESEARCH_COMMANDS_LAST;
}

bool IsPresearchOverrideCommands(int id) {
  static std::vector<int> override_commands({
      IDC_NEW_WINDOW,
      IDC_NEW_INCOGNITO_WINDOW,
  });
  return std::find(override_commands.begin(), override_commands.end(), id) !=
         override_commands.end();
}

}  // namespace

namespace chrome {

PresearchBrowserCommandController::PresearchBrowserCommandController(Browser* browser)
    : BrowserCommandController(browser),
      browser_(browser),
      presearch_command_updater_(nullptr) {
  InitPresearchCommandState();
}

bool PresearchBrowserCommandController::SupportsCommand(int id) const {
  return IsPresearchCommands(id)
      ? presearch_command_updater_.SupportsCommand(id)
      : BrowserCommandController::SupportsCommand(id);
}

bool PresearchBrowserCommandController::IsCommandEnabled(int id) const {
  return IsPresearchCommands(id)
      ? presearch_command_updater_.IsCommandEnabled(id)
      : BrowserCommandController::IsCommandEnabled(id);
}

bool PresearchBrowserCommandController::ExecuteCommandWithDisposition(
    int id,
    WindowOpenDisposition disposition,
    base::TimeTicks time_stamp) {
  return IsPresearchCommands(id) || IsPresearchOverrideCommands(id)
             ? ExecutePresearchCommandWithDisposition(id, disposition, time_stamp)
             : BrowserCommandController::ExecuteCommandWithDisposition(
                   id, disposition, time_stamp);
}

void PresearchBrowserCommandController::AddCommandObserver(
    int id, CommandObserver* observer) {
  IsPresearchCommands(id)
      ? presearch_command_updater_.AddCommandObserver(id, observer)
      : BrowserCommandController::AddCommandObserver(id, observer);
}

void PresearchBrowserCommandController::RemoveCommandObserver(
    int id, CommandObserver* observer) {
  IsPresearchCommands(id)
      ? presearch_command_updater_.RemoveCommandObserver(id, observer)
      : BrowserCommandController::RemoveCommandObserver(id, observer);
}

void PresearchBrowserCommandController::RemoveCommandObserver(
    CommandObserver* observer) {
  presearch_command_updater_.RemoveCommandObserver(observer);
  BrowserCommandController::RemoveCommandObserver(observer);
}

bool PresearchBrowserCommandController::UpdateCommandEnabled(int id, bool state) {
  return IsPresearchCommands(id)
      ? presearch_command_updater_.UpdateCommandEnabled(id, state)
      : BrowserCommandController::UpdateCommandEnabled(id, state);
}

void PresearchBrowserCommandController::InitPresearchCommandState() {
  // Sync & Rewards pages doesn't work on tor(guest) session.
  // They also doesn't work on private window but they are redirected
  // to normal window in this case.
  const bool is_guest_session = browser_->profile()->IsGuestSession();
  if (!is_guest_session) {
#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
    UpdateCommandForPresearchRewards();
#endif
#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
    UpdateCommandForPresearchWallet();
#endif
#if BUILDFLAG(ENABLE_PRESEARCH_SYNC)
    if (switches::IsSyncAllowedByFlag())
      UpdateCommandForPresearchSync();
#endif
  }
  UpdateCommandForPresearchAdblock();
  UpdateCommandForWebcompatReporter();
#if BUILDFLAG(ENABLE_TOR)
  UpdateCommandForTor();
#endif
  UpdateCommandForSidebar();
  bool add_new_profile_enabled = !is_guest_session;
  bool open_guest_profile_enabled = !is_guest_session;
  if (!is_guest_session) {
    if (PrefService* local_state = g_browser_process->local_state()) {
      add_new_profile_enabled =
          local_state->GetBoolean(prefs::kBrowserAddPersonEnabled);
      open_guest_profile_enabled =
          local_state->GetBoolean(prefs::kBrowserGuestModeEnabled);
    }
  }
  UpdateCommandEnabled(IDC_ADD_NEW_PROFILE, add_new_profile_enabled);
  UpdateCommandEnabled(IDC_OPEN_GUEST_PROFILE, open_guest_profile_enabled);
  UpdateCommandEnabled(IDC_TOGGLE_SPEEDREADER, true);
#if BUILDFLAG(IPFS_ENABLED)
  UpdateCommandForIpfs();
#endif
}

void PresearchBrowserCommandController::UpdateCommandForPresearchRewards() {
  UpdateCommandEnabled(IDC_SHOW_PRESEARCH_REWARDS, true);
}

void PresearchBrowserCommandController::UpdateCommandForPresearchAdblock() {
  UpdateCommandEnabled(IDC_SHOW_PRESEARCH_ADBLOCK, true);
}

void PresearchBrowserCommandController::UpdateCommandForWebcompatReporter() {
  UpdateCommandEnabled(IDC_SHOW_PRESEARCH_WEBCOMPAT_REPORTER, true);
}

#if BUILDFLAG(ENABLE_TOR)
void PresearchBrowserCommandController::UpdateCommandForTor() {
  // Enable new tor connection only for tor profile.
  UpdateCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE,
                       browser_->profile()->IsTor());
  UpdateCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR,
                       !presearch::IsTorDisabledForProfile(browser_->profile()));
}
#endif

void PresearchBrowserCommandController::UpdateCommandForSidebar() {
#if BUILDFLAG(ENABLE_SIDEBAR)
  if (sidebar::CanUseSidebar(browser_->profile()))
    UpdateCommandEnabled(IDC_SIDEBAR_SHOW_OPTION_MENU, true);
#endif
}

void PresearchBrowserCommandController::UpdateCommandForPresearchSync() {
  UpdateCommandEnabled(IDC_SHOW_PRESEARCH_SYNC, true);
}

void PresearchBrowserCommandController::UpdateCommandForPresearchWallet() {
  UpdateCommandEnabled(IDC_SHOW_PRESEARCH_WALLET, true);
}

#if BUILDFLAG(IPFS_ENABLED)
void PresearchBrowserCommandController::UpdateCommandForIpfs() {
  auto enabled = ipfs::IsIpfsMenuEnabled(browser_->profile());
  UpdateCommandEnabled(IDC_APP_MENU_IPFS, enabled);
  UpdateCommandEnabled(IDC_APP_MENU_IPFS_IMPORT_LOCAL_FILE, enabled);
  UpdateCommandEnabled(IDC_APP_MENU_IPFS_IMPORT_LOCAL_FOLDER, enabled);
}
#endif

bool PresearchBrowserCommandController::ExecutePresearchCommandWithDisposition(
    int id,
    WindowOpenDisposition disposition,
    base::TimeTicks time_stamp) {
  if (!SupportsCommand(id) || !IsCommandEnabled(id))
    return false;

  if (browser_->tab_strip_model()->active_index() == TabStripModel::kNoTab)
    return true;

  DCHECK(IsCommandEnabled(id)) << "Invalid/disabled command " << id;

  switch (id) {
    case IDC_NEW_WINDOW:
      // Use chromium's action for non-Tor profiles.
      if (!browser_->profile()->IsTor())
        return BrowserCommandController::ExecuteCommandWithDisposition(
            id, disposition, time_stamp);
      NewEmptyWindow(browser_->profile()->GetOriginalProfile());
      break;
    case IDC_NEW_INCOGNITO_WINDOW:
      // Use chromium's action for non-Tor profiles.
      if (!browser_->profile()->IsTor())
        return BrowserCommandController::ExecuteCommandWithDisposition(
            id, disposition, time_stamp);
      NewIncognitoWindow(browser_->profile()->GetOriginalProfile());
      break;
    case IDC_SHOW_PRESEARCH_REWARDS:
      presearch::ShowPresearchRewards(browser_);
      break;
    case IDC_SHOW_PRESEARCH_ADBLOCK:
      presearch::ShowPresearchAdblock(browser_);
      break;
    case IDC_SHOW_PRESEARCH_WEBCOMPAT_REPORTER:
      presearch::ShowWebcompatReporter(browser_);
      break;
    case IDC_NEW_OFFTHERECORD_WINDOW_TOR:
      presearch::NewOffTheRecordWindowTor(browser_);
      break;
    case IDC_NEW_TOR_CONNECTION_FOR_SITE:
      presearch::NewTorConnectionForSite(browser_);
      break;
    case IDC_SHOW_PRESEARCH_SYNC:
      presearch::ShowSync(browser_);
      break;
    case IDC_SHOW_PRESEARCH_WALLET:
      presearch::ShowPresearchWallet(browser_);
      break;
    case IDC_ADD_NEW_PROFILE:
      presearch::AddNewProfile();
      break;
    case IDC_OPEN_GUEST_PROFILE:
      presearch::OpenGuestProfile();
      break;
#if BUILDFLAG(IPFS_ENABLED)
    case IDC_APP_MENU_IPFS_IMPORT_LOCAL_FILE:
      presearch::ShareLocalFileUsingIPFS(browser_);
      break;
    case IDC_APP_MENU_IPFS_IMPORT_LOCAL_FOLDER:
      presearch::ShareLocalFolderUsingIPFS(browser_);
      break;
#endif
    case IDC_TOGGLE_SPEEDREADER:
      presearch::ToggleSpeedreader(browser_);
      break;
    default:
      LOG(WARNING) << "Received Unimplemented Command: " << id;
      break;
  }

  return true;
}

}  // namespace chrome
