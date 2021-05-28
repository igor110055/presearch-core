/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_OMNIBOX_BROWSER_PRESEARCH_OMNIBOX_CLIENT_H_
#define PRESEARCH_COMPONENTS_OMNIBOX_BROWSER_PRESEARCH_OMNIBOX_CLIENT_H_

#include "components/omnibox/browser/omnibox_client.h"

class PresearchOmniboxClient : public OmniboxClient {
 public:
  virtual bool IsAutocompleteEnabled() const;

 protected:
  ~PresearchOmniboxClient() override {}
};

#endif  // PRESEARCH_COMPONENTS_OMNIBOX_BROWSER_PRESEARCH_OMNIBOX_CLIENT_H_
