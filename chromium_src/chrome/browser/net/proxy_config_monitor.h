/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_NET_PROXY_CONFIG_MONITOR_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_NET_PROXY_CONFIG_MONITOR_H_

#define PRESEARCH_PROXY_CONFIG_MONITOR_H \
  net::ProxyConfigService* GetProxyConfigServiceForTesting() \
    { return proxy_config_service_.get(); }
#include "../../../../../chrome/browser/net/proxy_config_monitor.h"  // NOLINT
#undef PRESEARCH_PROXY_CONFIG_MONITOR_H

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_NET_PROXY_CONFIG_MONITOR_H_
