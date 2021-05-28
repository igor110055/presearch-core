/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_BROWSER_MAIN_PARTS_MAC_H_
#define PRESEARCH_BROWSER_PRESEARCH_BROWSER_MAIN_PARTS_MAC_H_

#include "chrome/browser/chrome_browser_main_mac.h"

class PresearchBrowserMainPartsMac : public ChromeBrowserMainPartsMac {
 public:
  using ChromeBrowserMainPartsMac::ChromeBrowserMainPartsMac;
  ~PresearchBrowserMainPartsMac() override = default;

 private:
  // ChromeBrowserMainPartsMac overrides:
  void PreMainMessageLoopStart() override;
};

#endif  // PRESEARCH_BROWSER_PRESEARCH_BROWSER_MAIN_PARTS_MAC_H_
