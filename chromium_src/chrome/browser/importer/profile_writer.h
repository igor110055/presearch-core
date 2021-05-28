/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_PROFILE_WRITER_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_PROFILE_WRITER_H_

#include "base/strings/string16.h"

// Extends ProfileWriter by file overriding instead of subclassing because there
// are many places that instantiate it.
#define AddAutofillFormDataEntries                           \
  AddCreditCard(const base::string16& name_on_card,          \
                const base::string16& expiration_month,      \
                const base::string16& expiration_year,       \
                const base::string16& decrypted_card_number, \
                const std::string& origin);                  \
  virtual void AddAutofillFormDataEntries

#include "../../../../../chrome/browser/importer/profile_writer.h"

#undef AddAutofillFormDataEntries

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_PROFILE_WRITER_H_
