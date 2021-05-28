/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_STATS_PRESEARCH_STATS_TAB_HELPER_H_
#define PRESEARCH_BROWSER_PRESEARCH_STATS_PRESEARCH_STATS_TAB_HELPER_H_

#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class NavigationHandle;
class WebContents;
}  // namespace content

namespace presearch_stats {

class PresearchStatsUpdater;

class PresearchStatsTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<PresearchStatsTabHelper> {
 public:
  explicit PresearchStatsTabHelper(content::WebContents*);
  ~PresearchStatsTabHelper() override;
  PresearchStatsTabHelper(const PresearchStatsTabHelper&) = delete;
  PresearchStatsTabHelper& operator=(const PresearchStatsTabHelper&) = delete;

  void NotifyStatsUpdater();

 private:
  void DidStartNavigation(content::NavigationHandle*) override;

  friend class content::WebContentsUserData<PresearchStatsTabHelper>;
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace presearch_stats
#endif  // PRESEARCH_BROWSER_PRESEARCH_STATS_PRESEARCH_STATS_TAB_HELPER_H_
