/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PROFILES_PRESEARCH_PROFILE_AVATAR_DOWNLOADER_H_
#define PRESEARCH_BROWSER_PROFILES_PRESEARCH_PROFILE_AVATAR_DOWNLOADER_H_

#include "chrome/browser/profiles/profile_avatar_downloader.h"

class PresearchProfileAvatarDownloader : public ProfileAvatarDownloader {
 public:
  PresearchProfileAvatarDownloader(size_t icon_index,
                               FetchCompleteCallback callback);
  ~PresearchProfileAvatarDownloader() override;

  void Start();

 private:
  FetchCompleteCallback callback_;
};

#endif  // PRESEARCH_BROWSER_PROFILES_PRESEARCH_PROFILE_AVATAR_DOWNLOADER_H_
