/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_BOOKMARK_PRESEARCH_BOOKMARK_TAB_HELPER_H_
#define PRESEARCH_BROWSER_UI_BOOKMARK_PRESEARCH_BOOKMARK_TAB_HELPER_H_

#include "chrome/browser/ui/bookmarks/bookmark_tab_helper.h"
#include "content/public/browser/web_contents_user_data.h"

class BookmarkTabHelperObserver;

// This proxies BookmarkTabHelper apis that used by Browser.
class PresearchBookmarkTabHelper
    : public content::WebContentsUserData<PresearchBookmarkTabHelper>{
 public:
  ~PresearchBookmarkTabHelper() override;

  bool ShouldShowBookmarkBar();
  void AddObserver(BookmarkTabHelperObserver* observer);
  void RemoveObserver(BookmarkTabHelperObserver* observer);

 private:
  friend class content::WebContentsUserData<PresearchBookmarkTabHelper>;

  explicit PresearchBookmarkTabHelper(content::WebContents* web_contents);

  content::WebContents* web_contents_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();

  DISALLOW_COPY_AND_ASSIGN(PresearchBookmarkTabHelper);
};

#endif  // PRESEARCH_BROWSER_UI_BOOKMARK_PRESEARCH_BOOKMARK_TAB_HELPER_H_
