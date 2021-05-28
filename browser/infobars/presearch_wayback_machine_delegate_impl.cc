/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/infobars/presearch_wayback_machine_delegate_impl.h"

#include "base/command_line.h"
#include "presearch/common/presearch_switches.h"
#include "presearch/components/presearch_wayback_machine/presearch_wayback_machine_infobar_delegate.h"
#include "presearch/components/presearch_wayback_machine/presearch_wayback_machine_tab_helper.h"
#include "components/infobars/core/infobar.h"
#include "chrome/browser/infobars/infobar_service.h"

// static
void PresearchWaybackMachineDelegateImpl::AttachTabHelperIfNeeded(
    content::WebContents* web_contents) {
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisablePresearchWaybackMachineExtension)) {
    PresearchWaybackMachineTabHelper::CreateForWebContents(web_contents);
    auto* tab_helper =
        PresearchWaybackMachineTabHelper::FromWebContents(web_contents);
    tab_helper->set_delegate(
        std::make_unique<PresearchWaybackMachineDelegateImpl>());
  }
}

PresearchWaybackMachineDelegateImpl::PresearchWaybackMachineDelegateImpl() = default;
PresearchWaybackMachineDelegateImpl::~PresearchWaybackMachineDelegateImpl() = default;


void PresearchWaybackMachineDelegateImpl::CreateInfoBar(
    content::WebContents* web_contents) {
  InfoBarService::FromWebContents(web_contents)->AddInfoBar(
      CreateInfoBarView(
          std::make_unique<PresearchWaybackMachineInfoBarDelegate>(),
          web_contents),
      true);
}
