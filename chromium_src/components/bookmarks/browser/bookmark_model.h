/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_BOOKMARKS_BROWSER_BOOKMARK_MODEL_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_BOOKMARKS_BROWSER_BOOKMARK_MODEL_H_

class PresearchSyncServiceTestDelayedLoadModel;

#define PRESEARCH_BOOKMARK_MODEL_H \
 private: \
  friend class ::PresearchSyncServiceTestDelayedLoadModel;

#include "../../../../../components/bookmarks/browser/bookmark_model.h"

namespace bookmarks {
void PresearchMigrateOtherNodeFolder(BookmarkModel* model);
void PresearchClearSyncV1MetaInfo(BookmarkModel* model);
}  // namespace bookmarks

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_BOOKMARKS_BROWSER_BOOKMARK_MODEL_H_
