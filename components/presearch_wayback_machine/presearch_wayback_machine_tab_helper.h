/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WAYBACK_MACHINE_PRESEARCH_WAYBACK_MACHINE_TAB_HELPER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WAYBACK_MACHINE_PRESEARCH_WAYBACK_MACHINE_TAB_HELPER_H_

#include <memory>

#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class PresearchWaybackMachineDelegate;
class PrefService;

class PresearchWaybackMachineTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<PresearchWaybackMachineTabHelper> {
 public:
  explicit PresearchWaybackMachineTabHelper(content::WebContents* contents);
  ~PresearchWaybackMachineTabHelper() override;

  PresearchWaybackMachineTabHelper(const PresearchWaybackMachineTabHelper&) = delete;
  PresearchWaybackMachineTabHelper& operator=(
      const PresearchWaybackMachineTabHelper&) = delete;

  void set_delegate(std::unique_ptr<PresearchWaybackMachineDelegate> delegate);

  WEB_CONTENTS_USER_DATA_KEY_DECL();
 private:
  FRIEND_TEST_ALL_PREFIXES(PresearchWaybackMachineTest, InfobarAddTest);

  // content::WebContentsObserver overrides:
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;

  void CreateInfoBar();
  bool IsWaybackMachineEnabled() const;

  // virtual for test.
  virtual bool ShouldAttachWaybackMachineInfoBar(int response_code) const;

  PrefService* pref_service_ = nullptr;
  std::unique_ptr<PresearchWaybackMachineDelegate> delegate_;

  base::WeakPtrFactory<PresearchWaybackMachineTabHelper> weak_factory_;
};

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WAYBACK_MACHINE_PRESEARCH_WAYBACK_MACHINE_TAB_HELPER_H_
