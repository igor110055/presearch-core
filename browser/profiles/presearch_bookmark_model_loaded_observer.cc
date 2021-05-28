/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/profiles/presearch_bookmark_model_loaded_observer.h"

#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_sync/presearch_sync_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/prefs/pref_service.h"

using bookmarks::BookmarkModel;

PresearchBookmarkModelLoadedObserver::PresearchBookmarkModelLoadedObserver(
    Profile* profile)
    : BookmarkModelLoadedObserver(profile) {}

void PresearchBookmarkModelLoadedObserver::BookmarkModelLoaded(
    BookmarkModel* model,
    bool ids_reassigned) {
  if (!profile_->GetPrefs()->GetBoolean(kOtherBookmarksMigrated)) {
    PresearchMigrateOtherNodeFolder(model);
    profile_->GetPrefs()->SetBoolean(kOtherBookmarksMigrated, true);
  }

  presearch_sync::Prefs presearch_sync_prefs(profile_->GetPrefs());
  if (!presearch_sync_prefs.IsSyncV1MetaInfoCleared()) {
    PresearchClearSyncV1MetaInfo(model);
    presearch_sync_prefs.SetSyncV1MetaInfoCleared(true);
  }

  BookmarkModelLoadedObserver::BookmarkModelLoaded(model, ids_reassigned);
}
