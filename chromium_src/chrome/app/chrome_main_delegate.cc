/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// This chormium_src override allows us to skip a lot of patching to
// chrome/BUILD.gn
#include "presearch/app/presearch_command_line_helper.cc"  // NOLINT
#include "presearch/app/presearch_main_delegate.cc"  // NOLINT
#include "../../../../chrome/app/chrome_main_delegate.cc"  // NOLINT
