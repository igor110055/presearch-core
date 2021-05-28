/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_MAC_H_
#define PRESEARCH_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_MAC_H_

#include <string>

#include "presearch/components/l10n/browser/locale_helper.h"

namespace presearch_l10n {

class LocaleHelperMac : public LocaleHelper {
 public:
  LocaleHelperMac(const LocaleHelperMac&) = delete;
  LocaleHelperMac& operator=(const LocaleHelperMac&) = delete;

  static LocaleHelperMac* GetInstanceImpl();

 private:
  friend struct base::DefaultSingletonTraits<LocaleHelperMac>;

  LocaleHelperMac();
  ~LocaleHelperMac() override;

  // LocaleHelper impl
  std::string GetLocale() const override;
};

}  // namespace presearch_l10n

#endif  // PRESEARCH_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_MAC_H_
