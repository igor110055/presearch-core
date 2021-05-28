// Copyright (c) 2019 The Presearch Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_H_
#define PRESEARCH_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_H_

#define PRESEARCH_UI_NATIVE_THEME_NATIVE_THEME_H_     \
  friend void SetUseDarkColors(bool dark_mode);   \
  friend void ReCalcAndSetPreferredColorScheme(); \
  static NativeTheme* PresearchGetInstanceForNativeUi();

#define GetSystemButtonPressedColor                                   \
  GetSystemButtonPressedColor_ChromiumImpl(SkColor base_color) const; \
  virtual SkColor GetSystemButtonPressedColor

#include "../../../../ui/native_theme/native_theme.h"
#undef GetSystemButtonPressedColor
#undef PRESEARCH_UI_NATIVE_THEME_NATIVE_THEME_H_

#endif  // PRESEARCH_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_H_
