/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_LOCATION_BAR_MODEL_DELEGATE_H_
#define PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_LOCATION_BAR_MODEL_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "chrome/browser/ui/browser_location_bar_model_delegate.h"

class Browser;

class PresearchLocationBarModelDelegate : public BrowserLocationBarModelDelegate {
 public:
  explicit PresearchLocationBarModelDelegate(Browser* browser);
  ~PresearchLocationBarModelDelegate() override;
  static void FormattedStringFromURL(const GURL& url,
      base::string16* new_formatted_url);
 private:
  base::string16 FormattedStringWithEquivalentMeaning(
      const GURL& url,
      const base::string16& formatted_url) const override;

  DISALLOW_COPY_AND_ASSIGN(PresearchLocationBarModelDelegate);
};

#endif  // PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_LOCATION_BAR_MODEL_DELEGATE_H_
