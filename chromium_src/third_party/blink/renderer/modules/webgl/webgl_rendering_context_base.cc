/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/public/platform/web_content_settings_client.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/html/canvas/canvas_rendering_context_host.h"
#include "third_party/blink/renderer/core/workers/worker_global_scope.h"

namespace {

bool AllowFingerprintingForHost(blink::CanvasRenderingContextHost* host) {
  if (!host)
    return true;
  blink::ExecutionContext* context = host->GetTopExecutionContext();
  blink::WebContentSettingsClient* settings =
      presearch::GetContentSettingsClientFor(context);
  return !settings || settings->AllowFingerprinting(true);
}

}  // namespace

#define PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_RETURN \
  if (!AllowFingerprintingForHost(Host()))        \
    return;

#define PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_NULLPTR \
  if (!AllowFingerprintingForHost(Host()))         \
    return nullptr;

#define PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_NULLOPT \
  if (!AllowFingerprintingForHost(Host()))         \
    return base::nullopt;

#define PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_ZERO \
  if (!AllowFingerprintingForHost(Host()))      \
    return 0;

#define PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_MINUS_ONE \
  if (!AllowFingerprintingForHost(Host()))           \
    return -1;

#define PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_SCRIPT_VALUE \
  if (!AllowFingerprintingForHost(Host()))              \
    return ScriptValue::CreateNull(script_state->GetIsolate());

#define PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_STRING \
  if (!AllowFingerprintingForHost(Host()))        \
    return String();

#define PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_GETSHADERINFOLOG \
  if (!AllowFingerprintingForHost(Host())) {                \
    range[0] = 0;                                           \
    range[1] = 0;                                           \
    precision = 0;                                          \
  }

#define PRESEARCH_WEBGL_GET_PARAMETER_UNMASKED_RENDERER     \
  if (ExtensionEnabled(kWebGLDebugRendererInfoName) &&  \
      !AllowFingerprintingForHost(Host()))              \
    return WebGLAny(                                    \
        script_state,                                   \
        String(presearch::PresearchSessionCache::From(          \
                   *(Host()->GetTopExecutionContext())) \
                   .GenerateRandomString("UNMASKED_RENDERER_WEBGL", 8)));

#define PRESEARCH_WEBGL_GET_PARAMETER_UNMASKED_VENDOR       \
  if (ExtensionEnabled(kWebGLDebugRendererInfoName) &&  \
      !AllowFingerprintingForHost(Host()))              \
    return WebGLAny(                                    \
        script_state,                                   \
        String(presearch::PresearchSessionCache::From(          \
                   *(Host()->GetTopExecutionContext())) \
                   .GenerateRandomString("UNMASKED_VENDOR_WEBGL", 8)));

#include "../../../../../../../third_party/blink/renderer/modules/webgl/webgl_rendering_context_base.cc"
#undef PRESEARCH_WEBGL_GET_PARAMETER_UNMASKED_RENDERER
#undef PRESEARCH_WEBGL_GET_PARAMETER_UNMASKED_VENDOR
#undef PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_STRING
#undef PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_SCRIPT_VALUE
#undef PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_MINUS_ONE
#undef PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_ZERO
#undef PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_NULLOPT
#undef PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_NULLPTR
#undef PRESEARCH_WEBGL_RENDERING_CONTEXT_BASE_RETURN
