/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_BROWSER_MAIN_LOOP_H_
#define PRESEARCH_BROWSER_PRESEARCH_BROWSER_MAIN_LOOP_H_

#include "base/macros.h"
#include "content/browser/browser_main_loop.h"

namespace presearch {

class PresearchBrowserMainLoop : public content::BrowserMainLoop {
 public:
  using BrowserMainLoop::BrowserMainLoop;
  ~PresearchBrowserMainLoop() override = default;

  void PreShutdown() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchBrowserMainLoop);
};

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_PRESEARCH_BROWSER_MAIN_LOOP_H_
