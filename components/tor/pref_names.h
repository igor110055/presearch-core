/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_TOR_PREF_NAMES_H_
#define PRESEARCH_COMPONENTS_TOR_PREF_NAMES_H_

namespace tor {
namespace prefs {

extern const char kTorDisabled[];

// Automatically open onion available site or .onion domain in Tor window
extern const char kAutoOnionRedirect[];

}  // namespace prefs
}  // namespace tor

#endif  // PRESEARCH_COMPONENTS_TOR_PREF_NAMES_H_
