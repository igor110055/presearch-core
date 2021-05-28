/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_stats/presearch_stats_tab_helper.h"

#include "presearch/browser/presearch_browser_process.h"
#include "presearch/browser/presearch_stats/presearch_stats_updater.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/base/page_transition_types.h"

namespace presearch_stats {

PresearchStatsTabHelper::PresearchStatsTabHelper(content::WebContents *web_contents)
    : content::WebContentsObserver(web_contents) {}

PresearchStatsTabHelper::~PresearchStatsTabHelper() = default;

void PresearchStatsTabHelper::DidStartNavigation(
        content::NavigationHandle* handle) {
  if (!handle || !handle->IsInMainFrame() || handle->IsDownload())
    return;

  auto transition = handle->GetPageTransition();

  switch (ui::PageTransitionStripQualifier(transition)) {
    case ui::PAGE_TRANSITION_TYPED:
    case ui::PAGE_TRANSITION_AUTO_BOOKMARK:
    case ui::PAGE_TRANSITION_GENERATED: {
      auto url = handle->GetURL();
      if (!(url.SchemeIs("chrome") || url.SchemeIs("presearch")))
        NotifyStatsUpdater();
      break;
    }
    default:
      break;
  }
}

void PresearchStatsTabHelper::NotifyStatsUpdater() {
  if (g_presearch_browser_process->presearch_stats_updater()->MaybeDoThresholdPing(1))
    web_contents()->RemoveUserData(UserDataKey());
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(PresearchStatsTabHelper)

}  //  namespace presearch_stats
