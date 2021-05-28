/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/infobars/presearch_confirm_p3a_infobar_delegate.h"

#include <memory>
#include <utility>

#include "presearch/common/pref_names.h"
#include "presearch/common/url_constants.h"
#include "presearch/components/p3a/pref_names.h"
#include "presearch/grit/presearch_generated_resources.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/grit/chromium_strings.h"
#include "components/infobars/core/infobar.h"
#include "components/prefs/pref_service.h"
#include "components/strings/grit/components_strings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/views/vector_icons.h"

// static
void PresearchConfirmP3AInfoBarDelegate::Create(InfoBarService* infobar_service,
                                            PrefService* local_state) {
  // Don't show infobar if:
  // - P3A is disabled
  // - notice has already been acknowledged
  if (local_state) {
    if (!local_state->GetBoolean(presearch::kP3AEnabled) ||
        local_state->GetBoolean(presearch::kP3ANoticeAcknowledged)) {
      local_state->SetBoolean(presearch::kP3ANoticeAcknowledged, true);
      return;
    }
  }

  infobar_service->AddInfoBar(infobar_service->CreateConfirmInfoBar(
      std::unique_ptr<ConfirmInfoBarDelegate>(
          new PresearchConfirmP3AInfoBarDelegate(local_state))));
}

PresearchConfirmP3AInfoBarDelegate::PresearchConfirmP3AInfoBarDelegate(
    PrefService* local_state)
    : ConfirmInfoBarDelegate(), local_state_(local_state) {}

PresearchConfirmP3AInfoBarDelegate::~PresearchConfirmP3AInfoBarDelegate() {}

infobars::InfoBarDelegate::InfoBarIdentifier
PresearchConfirmP3AInfoBarDelegate::GetIdentifier() const {
  return PRESEARCH_CONFIRM_P3A_INFOBAR_DELEGATE;
}

const gfx::VectorIcon& PresearchConfirmP3AInfoBarDelegate::GetVectorIcon() const {
  return views::kInfoIcon;
}

bool PresearchConfirmP3AInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  return false;
}

void PresearchConfirmP3AInfoBarDelegate::InfoBarDismissed() {
  // Mark notice as acknowledged when infobar is dismissed
  if (local_state_) {
    local_state_->SetBoolean(presearch::kP3ANoticeAcknowledged, true);
  }
}

base::string16 PresearchConfirmP3AInfoBarDelegate::GetMessageText() const {
  return l10n_util::GetStringUTF16(IDS_PRESEARCH_CONFIRM_P3A_INFO_BAR);
}

int PresearchConfirmP3AInfoBarDelegate::GetButtons() const {
  return BUTTON_OK | BUTTON_CANCEL;
}

base::string16 PresearchConfirmP3AInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  if (button == BUTTON_CANCEL) {
    return l10n_util::GetStringUTF16(IDS_PRESEARCH_CONFIRM_P3A_INFO_BAR_DISABLE);
  }
  return l10n_util::GetStringUTF16(IDS_PRESEARCH_CONFIRM_P3A_INFO_BAR_ACKNOWLEDGE);
}

base::string16 PresearchConfirmP3AInfoBarDelegate::GetLinkText() const {
  return l10n_util::GetStringUTF16(IDS_LEARN_MORE);
}

GURL PresearchConfirmP3AInfoBarDelegate::GetLinkURL() const {
  return GURL(kP3ALearnMoreURL);
}

bool PresearchConfirmP3AInfoBarDelegate::Accept() {
  // Mark notice as acknowledged when infobar is dismissed
  if (local_state_) {
    local_state_->SetBoolean(presearch::kP3ANoticeAcknowledged, true);
  }
  return true;
}

bool PresearchConfirmP3AInfoBarDelegate::Cancel() {
  // OK button is "Disable"
  // Clicking should disable P3A
  if (local_state_) {
    local_state_->SetBoolean(presearch::kP3AEnabled, false);
  }
  return true;
}
