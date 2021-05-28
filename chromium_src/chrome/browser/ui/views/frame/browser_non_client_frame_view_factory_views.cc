/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"

#if defined(OS_WIN)
#include "presearch/browser/ui/views/frame/presearch_glass_browser_frame_view.h"
#define GlassBrowserFrameView PresearchGlassBrowserFrameView
#endif

#include "presearch/browser/ui/views/frame/presearch_opaque_browser_frame_view.h"
#define OpaqueBrowserFrameView PresearchOpaqueBrowserFrameView

#include "../../../../../../../chrome/browser/ui/views/frame/browser_non_client_frame_view_factory_views.cc"

#undef OpaqueBrowserFrameView

#if defined(OS_WIN)
#undef GlassBrowserFrameView
#endif
