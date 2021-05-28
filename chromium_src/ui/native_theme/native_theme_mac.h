// Copyright (c) 2019 The Presearch Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_MAC_H_
#define PRESEARCH_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_MAC_H_

#include "ui/native_theme/native_theme_base.h"

#define GetSystemButtonPressedColor                                            \
  GetSystemButtonPressedColor_ChromiumImpl(SkColor base_color) const override; \
  virtual SkColor GetSystemButtonPressedColor

#include "../../../../ui/native_theme/native_theme_mac.h"
#undef GetSystemButtonPressedColor

#endif  // PRESEARCH_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_MAC_H_
