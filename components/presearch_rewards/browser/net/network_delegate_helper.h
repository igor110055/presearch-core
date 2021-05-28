/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_NET_NETWORK_HELPER_DELEGATE_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_NET_NETWORK_HELPER_DELEGATE_H_

#include "presearch/browser/net/url_context.h"

namespace presearch_rewards {

int OnBeforeURLRequest(
    const presearch::ResponseCallback& next_callback,
    std::shared_ptr<presearch::PresearchRequestInfo> ctx);

}  // namespace presearch_rewards

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_NET_NETWORK_HELPER_DELEGATE_H_
