/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_REWARDS_TIP_DIALOG_H_
#define PRESEARCH_BROWSER_PRESEARCH_REWARDS_TIP_DIALOG_H_

#include <memory>
#include <string>

namespace base {
class DictionaryValue;
}

namespace content {
class WebContents;
}

namespace presearch_rewards {

void OpenTipDialog(content::WebContents* initiator,
                   std::unique_ptr<base::DictionaryValue> params);

}

#endif  // PRESEARCH_BROWSER_PRESEARCH_REWARDS_TIP_DIALOG_H_
