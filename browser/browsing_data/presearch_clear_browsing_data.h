/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_BROWSING_DATA_PRESEARCH_CLEAR_BROWSING_DATA_H_
#define PRESEARCH_BROWSER_BROWSING_DATA_PRESEARCH_CLEAR_BROWSING_DATA_H_

#include "base/macros.h"

namespace content {
class BrowsingDataRemover;
}
class Profile;
class PresearchClearDataOnExitTest;

namespace content {

class PresearchClearBrowsingData {
 public:
  // Clears browsing data for all loaded non-off-the-record profiles.
  // Profile's *OnExit preferences determine what gets cleared.
  // Note: this method will wait until browsing data has been cleared.
  static void ClearOnExit();

  // Used for testing only.
  struct OnExitTestingCallback {
    // Called from ClearOnExit right before the call to BrowsingDataRemover
    // to remove data.
    virtual void BeforeClearOnExitRemoveData(
        content::BrowsingDataRemover* remover,
        int remove_mask,
        int origin_mask) = 0;
  };

 protected:
  friend class ::PresearchClearDataOnExitTest;

  // Used for testing only.
  static void SetOnExitTestingCallback(OnExitTestingCallback* callback);

 private:
  static OnExitTestingCallback* on_exit_testing_callback_;

  DISALLOW_COPY_AND_ASSIGN(PresearchClearBrowsingData);
};

}  // namespace content

#endif  // PRESEARCH_BROWSER_BROWSING_DATA_PRESEARCH_CLEAR_BROWSING_DATA_H_
