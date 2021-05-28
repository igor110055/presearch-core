/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/presearch_actions/presearch_actions_container.h"
#include "presearch/browser/ui/views/location_bar/presearch_location_bar_view.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_rewards/browser/buildflags/buildflags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_window.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/search_test_utils.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/notification_service.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/test_utils.h"
#include "extensions/common/constants.h"

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
#include "presearch/components/presearch_rewards/common/pref_names.h"
#endif

class PresearchActionsContainerTest : public InProcessBrowserTest {
 public:
  PresearchActionsContainerTest() = default;
  ~PresearchActionsContainerTest() override = default;

  void SetUpOnMainThread() override {
    Init(browser());
  }

  void Init(Browser* browser) {
    BrowserView* browser_view =
        BrowserView::GetBrowserViewForBrowser(browser);
    ASSERT_NE(browser_view, nullptr);
    PresearchLocationBarView* presearch_location_bar_view =
        static_cast<PresearchLocationBarView*>(browser_view->GetLocationBarView());
    ASSERT_NE(presearch_location_bar_view, nullptr);
    presearch_actions_ = presearch_location_bar_view->presearch_actions_;
    ASSERT_NE(presearch_actions_, nullptr);
    prefs_ = browser->profile()->GetPrefs();
  }

  void CheckPresearchRewardsActionShown(bool expected_shown) {
    const bool shown =
        presearch_actions_->IsActionShown(presearch_rewards_extension_id);
    ASSERT_EQ(shown, expected_shown);
  }

 protected:
  PresearchActionsContainer* presearch_actions_;
  PrefService* prefs_;
  DISALLOW_COPY_AND_ASSIGN(PresearchActionsContainerTest);
};

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
IN_PROC_BROWSER_TEST_F(PresearchActionsContainerTest, HidePresearchRewardsAction) {
  // By default the action should be shown.
  EXPECT_FALSE(
      prefs_->GetBoolean(presearch_rewards::prefs::kHideButton));
  CheckPresearchRewardsActionShown(true);

  // Set to hide.
  prefs_->SetBoolean(presearch_rewards::prefs::kHideButton, true);
  CheckPresearchRewardsActionShown(false);

  // Set to show.
  prefs_->SetBoolean(presearch_rewards::prefs::kHideButton, false);
  CheckPresearchRewardsActionShown(true);
}

IN_PROC_BROWSER_TEST_F(PresearchActionsContainerTest,
                       PresearchRewardsActionHiddenInGuestSession) {
    // By default the action should be shown.
  EXPECT_FALSE(
      prefs_->GetBoolean(presearch_rewards::prefs::kHideButton));
  CheckPresearchRewardsActionShown(true);

  // Open a Guest window.
  EXPECT_EQ(1U, BrowserList::GetInstance()->size());
  content::WindowedNotificationObserver browser_creation_observer(
      chrome::NOTIFICATION_BROWSER_OPENED,
      content::NotificationService::AllSources());
  profiles::SwitchToGuestProfile(ProfileManager::CreateCallback());
  base::RunLoop().RunUntilIdle();
  browser_creation_observer.Wait();
  EXPECT_EQ(2U, BrowserList::GetInstance()->size());

  // Retrieve the new Guest profile.
  Profile* guest = g_browser_process->profile_manager()->GetProfileByPath(
      ProfileManager::GetGuestProfilePath());
  // The BrowsingDataRemover needs a loaded TemplateUrlService or else it hangs
  // on to a CallbackList::Subscription forever.
  search_test_utils::WaitForTemplateURLServiceToLoad(
        TemplateURLServiceFactory::GetForProfile(guest));

  // Access the browser with the Guest profile and re-init test for it.
  Browser* browser = chrome::FindAnyBrowser(guest, true);
  EXPECT_TRUE(browser);
  Init(browser);
  CheckPresearchRewardsActionShown(false);
}
#endif
