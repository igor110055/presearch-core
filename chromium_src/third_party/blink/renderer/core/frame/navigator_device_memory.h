/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_DEVICE_MEMORY_H_
#define PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_DEVICE_MEMORY_H_

#include "third_party/blink/renderer/core/core_export.h"

namespace blink {

class CORE_EXPORT NavigatorDeviceMemory {
 public:
  float deviceMemory(ScriptState*) const;
};

}  // namespace blink

#endif  // PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_DEVICE_MEMORY_H_
