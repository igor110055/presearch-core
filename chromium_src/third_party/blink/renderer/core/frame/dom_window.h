/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_DOM_WINDOW_H_
#define PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_DOM_WINDOW_H_

#define PRESEARCH_DOM_WINDOW_H \
  LocalFrame* GetDisconnectedFrame() const;

#include "../../../../../../../third_party/blink/renderer/core/frame/dom_window.h"

#undef PRESEARCH_DOM_WINDOW_H

#endif  // PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_DOM_WINDOW_H_
