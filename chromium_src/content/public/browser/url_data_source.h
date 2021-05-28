/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_URL_DATA_SOURCE_H_
#define PRESEARCH_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_URL_DATA_SOURCE_H_

#define GetContentSecurityPolicy                                        \
  GetContentSecurityPolicy(network::mojom::CSPDirectiveName directive); \
  std::string GetContentSecurityPolicy_ChromiumImpl

#include "../../../../../content/public/browser/url_data_source.h"
#undef GetContentSecurityPolicy

#endif  // PRESEARCH_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_URL_DATA_SOURCE_H_
