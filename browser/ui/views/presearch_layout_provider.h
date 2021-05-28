/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_LAYOUT_PROVIDER_H_
#define PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_LAYOUT_PROVIDER_H_

#include "base/macros.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"

class PresearchLayoutProvider : public ChromeLayoutProvider {
 public:
  PresearchLayoutProvider() = default;
  ~PresearchLayoutProvider() override = default;

  int GetCornerRadiusMetric(views::EmphasisMetric emphasis_metric,
                            const gfx::Size& size = gfx::Size()) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchLayoutProvider);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_LAYOUT_PROVIDER_H_
