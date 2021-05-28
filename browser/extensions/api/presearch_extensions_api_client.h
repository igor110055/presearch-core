/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_EXTENSIONS_API_CLIENT_H_
#define PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_EXTENSIONS_API_CLIENT_H_

#include "chrome/browser/extensions/api/chrome_extensions_api_client.h"
#include "extensions/browser/api/web_request/web_request_info.h"

namespace extensions {

class PresearchExtensionsAPIClient : public ChromeExtensionsAPIClient {
  bool ShouldHideBrowserNetworkRequest(
      content::BrowserContext* context,
      const WebRequestInfo& request) const override;

  friend class PresearchExtensionsAPIClientTests;
};

}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_EXTENSIONS_API_CLIENT_H_
