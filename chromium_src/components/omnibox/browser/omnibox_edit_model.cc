/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/omnibox/browser/presearch_omnibox_client.h"
#include "components/omnibox/browser/omnibox_client.h"
#include "components/omnibox/browser/omnibox_controller.h"
#include "url/gurl.h"

#if !defined(OS_IOS)
#include "content/public/common/url_constants.h"
#endif

class PresearchOmniboxController : public OmniboxController {
 public:
  PresearchOmniboxController(OmniboxEditModel* omnibox_edit_model,
                         OmniboxClient* client)
      : OmniboxController(omnibox_edit_model, client),
        client_(static_cast<PresearchOmniboxClient*>(client)) {}
  ~PresearchOmniboxController() override = default;

  // OmniboxController overrides:
  void StartAutocomplete(const AutocompleteInput& input) const override {
    if (!client_->IsAutocompleteEnabled())
      return;

    OmniboxController::StartAutocomplete(input);
  }

 private:
  PresearchOmniboxClient* client_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(PresearchOmniboxController);
};

namespace {
void PresearchAdjustTextForCopy(GURL* url) {
#if !defined(OS_IOS)
  if (url->scheme() == content::kChromeUIScheme) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kPresearchUIScheme);
    *url = url->ReplaceComponents(replacements);
  }
#endif
}

}  // namespace

#define PRESEARCH_ADJUST_TEXT_FOR_COPY \
  PresearchAdjustTextForCopy(url_from_text);

#define OmniboxController PresearchOmniboxController
#include "../../../../../components/omnibox/browser/omnibox_edit_model.cc"  // NOLINT
#undef OmniboxController
#undef PRESEARCH_ADJUST_TEXT_FOR_COPY
