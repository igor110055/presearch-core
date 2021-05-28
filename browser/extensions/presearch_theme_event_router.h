/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_THEME_EVENT_ROUTER_H_
#define PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_THEME_EVENT_ROUTER_H_

#include "base/scoped_observer.h"
#include "ui/native_theme/native_theme.h"
#include "ui/native_theme/native_theme_observer.h"

class Profile;

namespace extensions {

class PresearchThemeEventRouter : public ui::NativeThemeObserver {
 public:
  explicit PresearchThemeEventRouter(Profile* profile);
  ~PresearchThemeEventRouter() override;

 private:
  friend class MockPresearchThemeEventRouter;

  // ui::NativeThemeObserver overrides:
  void OnNativeThemeUpdated(ui::NativeTheme* observed_theme) override;

  // Make virtual for testing.
  virtual void Notify();

  ui::NativeTheme* current_native_theme_for_testing_ = nullptr;
  Profile* profile_;
  ScopedObserver<ui::NativeTheme, ui::NativeThemeObserver> observer_;

  DISALLOW_COPY_AND_ASSIGN(PresearchThemeEventRouter);
};

}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_THEME_EVENT_ROUTER_H_
