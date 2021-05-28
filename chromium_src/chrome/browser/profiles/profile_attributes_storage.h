/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_ATTRIBUTES_STORAGE_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_ATTRIBUTES_STORAGE_H_

#include "presearch/browser/profiles/presearch_profile_avatar_downloader.h"

#define ProfileAvatarDownloader PresearchProfileAvatarDownloader
#include "../../../../../chrome/browser/profiles/profile_attributes_storage.h"
#undef ProfileAvatarDownloader

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_ATTRIBUTES_STORAGE_H_
