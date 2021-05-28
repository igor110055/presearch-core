/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_INFOBARS_PRESEARCH_WAYBACK_MACHINE_DELEGATE_IMPL_H_
#define PRESEARCH_BROWSER_INFOBARS_PRESEARCH_WAYBACK_MACHINE_DELEGATE_IMPL_H_

#include <memory>

#include "presearch/components/presearch_wayback_machine/presearch_wayback_machine_delegate.h"

namespace content {
class WebContents;
}  // namespace content

namespace infobars {
class InfoBar;
}  // namespace infobars

class PresearchWaybackMachineDelegateImpl : public PresearchWaybackMachineDelegate {
 public:
  static void AttachTabHelperIfNeeded(content::WebContents* web_contents);

  PresearchWaybackMachineDelegateImpl();
  ~PresearchWaybackMachineDelegateImpl() override;

  PresearchWaybackMachineDelegateImpl(
      const PresearchWaybackMachineDelegateImpl&) = delete;
  PresearchWaybackMachineDelegateImpl& operator=(
      const PresearchWaybackMachineDelegateImpl&) = delete;

 private:
  // PresearchWaybackMachineDelegate overrides:
  void CreateInfoBar(content::WebContents* web_contents) override;

  std::unique_ptr<infobars::InfoBar> CreateInfoBarView(
      std::unique_ptr<PresearchWaybackMachineInfoBarDelegate> delegate,
      content::WebContents* contents);
};

#endif  // PRESEARCH_BROWSER_INFOBARS_PRESEARCH_WAYBACK_MACHINE_DELEGATE_IMPL_H_
