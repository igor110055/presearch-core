/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_H_
#define PRESEARCH_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_H_

#include <string>

#include "base/macros.h"
#include "base/memory/singleton.h"
#include "build/build_config.h"

namespace presearch_l10n {

const char kDefaultLocale[] = "en-US";

class LocaleHelper {
 public:
  LocaleHelper(const LocaleHelper&) = delete;
  LocaleHelper& operator=(const LocaleHelper&) = delete;

  static LocaleHelper* GetInstance();

  void set_for_testing(
      LocaleHelper* locale_helper);

  // Should return the locale based upon the tagging conventions of RFC 4646
  virtual std::string GetLocale() const;

 protected:
  friend struct base::DefaultSingletonTraits<LocaleHelper>;

  LocaleHelper();
  virtual ~LocaleHelper();

  static LocaleHelper* GetInstanceImpl();
};

}  // namespace presearch_l10n

#endif  // PRESEARCH_COMPONENTS_L10N_BROWSER_LOCALE_HELPER_H_
