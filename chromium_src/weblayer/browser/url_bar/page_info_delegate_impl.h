/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_WEBLAYER_BROWSER_URL_BAR_PAGE_INFO_DELEGATE_IMPL_H_
#define PRESEARCH_CHROMIUM_SRC_WEBLAYER_BROWSER_URL_BAR_PAGE_INFO_DELEGATE_IMPL_H_

#include "components/page_info/page_info_delegate.h"

#define IsContentDisplayedInVrHeadset                           \
  PresearchShouldShowPermission(ContentSettingsType type) override; \
  virtual bool IsContentDisplayedInVrHeadset

#include "../../../../../weblayer/browser/url_bar/page_info_delegate_impl.h"
#undef IsContentDisplayedInVrHeadset

#endif  // PRESEARCH_CHROMIUM_SRC_WEBLAYER_BROWSER_URL_BAR_PAGE_INFO_DELEGATE_IMPL_H_
