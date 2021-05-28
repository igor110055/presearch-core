/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_EXECUTION_CONTEXT_NAVIGATOR_BASE_H_
#define PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_EXECUTION_CONTEXT_NAVIGATOR_BASE_H_

#include "third_party/blink/public/platform/web_content_settings_client.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/navigator_id.h"
#include "third_party/blink/renderer/core/loader/frame_loader.h"

#define PRESEARCH_NAVIGATOR_BASE_USER_AGENT                                \
  if (!GetExecutionContext())                                          \
    return String();                                                   \
  if (blink::WebContentSettingsClient* settings =                      \
          presearch::GetContentSettingsClientFor(GetExecutionContext())) { \
    if (!settings->AllowFingerprinting(true)) {                        \
      return presearch::PresearchSessionCache::From(*(GetExecutionContext()))  \
          .FarbledUserAgent(GetExecutionContext()->UserAgent());       \
    }                                                                  \
  }

#include "../../../../../../../third_party/blink/renderer/core/execution_context/navigator_base.h"
#undef PRESEARCH_NAVIGATOR_BASE_USER_AGENT

#endif  // PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_EXECUTION_CONTEXT_NAVIGATOR_BASE_H_
