/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <random>

#include "presearch/third_party/blink/renderer/presearch_farbling_constants.h"
#include "third_party/blink/public/platform/web_content_settings_client.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/workers/worker_global_scope.h"
#include "third_party/blink/renderer/modules/mediastream/media_device_info.h"

using blink::DynamicTo;
using blink::ExecutionContext;
using blink::LocalDOMWindow;
using blink::MediaDeviceInfoVector;
using blink::To;
using blink::WebContentSettingsClient;
using blink::WorkerGlobalScope;

namespace presearch {

void FarbleMediaDevices(ExecutionContext* context,
                        MediaDeviceInfoVector* media_devices) {
  WebContentSettingsClient* settings = GetContentSettingsClientFor(context);
  if (!settings)
    return;
  if (settings->GetPresearchFarblingLevel() != PresearchFarblingLevel::OFF) {
    // Shuffle the list of plugins pseudo-randomly, based on the
    // domain+session key.
    std::mt19937_64 prng =
        PresearchSessionCache::From(*context).MakePseudoRandomGenerator();
    std::shuffle(media_devices->begin(), media_devices->end(), prng);
  }
}

}  // namespace presearch

#define PRESEARCH_MEDIA_DEVICES_DEVICES_ENUMERATED                       \
  if (ExecutionContext* context = resolver->GetExecutionContext()) { \
    presearch::FarbleMediaDevices(context, &media_devices);              \
  }

#include "../../../../../../../third_party/blink/renderer/modules/mediastream/media_devices.cc"
#undef PRESEARCH_MEDIA_DEVICES_DEVICES_ENUMERATED
