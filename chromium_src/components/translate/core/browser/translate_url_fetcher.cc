/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/translate/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_PRESEARCH_TRANSLATE_GO)
#include "../../../../../../components/translate/core/browser/translate_url_fetcher.cc"
#endif
