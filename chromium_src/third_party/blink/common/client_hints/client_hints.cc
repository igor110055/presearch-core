/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define PRESEARCH_CLIENT_HINTS_IS_CLIENT_HINT_SENT_BY_DEFAULT return false;

#include "../../../../../../third_party/blink/common/client_hints/client_hints.cc"
#undef PRESEARCH_CLIENT_HINTS_IS_CLIENT_HINT_SENT_BY_DEFAULT
