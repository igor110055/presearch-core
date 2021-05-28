/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_APP_CHROME_MAIN_DELEGATE_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_APP_CHROME_MAIN_DELEGATE_H_

#include "presearch/common/presearch_content_client.h"
#include "chrome/common/chrome_content_client.h"

#define ChromeContentClient PresearchContentClient
#include "../../../../chrome/app/chrome_main_delegate.h"
#undef ChromeContentClient

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_APP_CHROME_MAIN_DELEGATE_H_
