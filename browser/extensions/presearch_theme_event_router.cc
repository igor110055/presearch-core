/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/presearch_theme_event_router.h"

#include <memory>
#include <string>
#include <utility>

#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/common/extensions/api/presearch_theme.h"
#include "chrome/browser/profiles/profile.h"
#include "extensions/browser/event_router.h"
#include "ui/native_theme/native_theme.h"

namespace extensions {

PresearchThemeEventRouter::PresearchThemeEventRouter(Profile* profile)
    : profile_(profile),
      observer_(this) {
  observer_.Add(ui::NativeTheme::GetInstanceForNativeUi());
}

PresearchThemeEventRouter::~PresearchThemeEventRouter() {}

void PresearchThemeEventRouter::OnNativeThemeUpdated(
    ui::NativeTheme* observed_theme) {
  DCHECK(observer_.IsObserving(observed_theme));
  Notify();
}

void PresearchThemeEventRouter::Notify() {
  const std::string theme_type =
      dark_mode::GetStringFromPresearchDarkModeType(
          dark_mode::GetActivePresearchDarkModeType());

  auto event = std::make_unique<extensions::Event>(
      extensions::events::PRESEARCH_ON_PRESEARCH_THEME_TYPE_CHANGED,
      api::presearch_theme::OnPresearchThemeTypeChanged::kEventName,
      api::presearch_theme::OnPresearchThemeTypeChanged::Create(theme_type),
      profile_);

  if (EventRouter* event_router = EventRouter::Get(profile_))
    event_router->BroadcastEvent(std::move(event));
}

}  // namespace extensions
