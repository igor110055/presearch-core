/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/renderer/presearch_content_renderer_client.h"

#include "base/feature_list.h"
#include "presearch/components/presearch_shields/common/features.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/cosmetic_filters/renderer/cosmetic_filters_js_render_frame_observer.h"
#include "presearch/renderer/presearch_render_thread_observer.h"
#include "chrome/common/chrome_isolated_world_ids.h"
#include "content/public/renderer/render_thread.h"
#include "third_party/blink/public/platform/web_runtime_features.h"
#include "third_party/blink/public/web/modules/service_worker/web_service_worker_context_proxy.h"
#include "url/gurl.h"

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/components/presearch_wallet/common/features.h"
#include "presearch/renderer/presearch_wallet/presearch_wallet_render_frame_observer.h"
#endif

PresearchContentRendererClient::PresearchContentRendererClient()
    : ChromeContentRendererClient() {}

void PresearchContentRendererClient::
    SetRuntimeFeaturesDefaultsBeforeBlinkInitialization() {
  ChromeContentRendererClient::
      SetRuntimeFeaturesDefaultsBeforeBlinkInitialization();

  blink::WebRuntimeFeatures::EnableWebNfc(false);

  // These features don't have dedicated WebRuntimeFeatures wrappers.
  blink::WebRuntimeFeatures::EnableFeatureFromString("DigitalGoods", false);
  blink::WebRuntimeFeatures::EnableFeatureFromString("FileSystemAccess", false);
  blink::WebRuntimeFeatures::EnableFeatureFromString(
      "FileSystemAccessAPIExperimental", false);
  blink::WebRuntimeFeatures::EnableFeatureFromString("Serial", false);
}

PresearchContentRendererClient::~PresearchContentRendererClient() = default;

void PresearchContentRendererClient::RenderThreadStarted() {
  ChromeContentRendererClient::RenderThreadStarted();

  presearch_observer_ = std::make_unique<PresearchRenderThreadObserver>();
  content::RenderThread::Get()->AddObserver(presearch_observer_.get());
  presearch_search_service_worker_holder_.SetBrowserInterfaceBrokerProxy(
      browser_interface_broker_.get());
}

void PresearchContentRendererClient::RenderFrameCreated(
    content::RenderFrame* render_frame) {
  ChromeContentRendererClient::RenderFrameCreated(render_frame);

#if !defined(OS_ANDROID) && !defined(CHROME_OS)
  if (base::FeatureList::IsEnabled(
          presearch_shields::features::kPresearchAdblockCosmeticFilteringNative))
#endif
    new cosmetic_filters::CosmeticFiltersJsRenderFrameObserver(
        render_frame, ISOLATED_WORLD_ID_PRESEARCH_INTERNAL);

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  if (base::FeatureList::IsEnabled(
          presearch_wallet::features::kNativePresearchWalletFeature)) {
    new presearch_wallet::PresearchWalletRenderFrameObserver(
        render_frame, PresearchRenderThreadObserver::GetDynamicParams());
  }
#endif
}

void PresearchContentRendererClient::RunScriptsAtDocumentStart(
    content::RenderFrame* render_frame) {
  auto* observer =
      cosmetic_filters::CosmeticFiltersJsRenderFrameObserver::Get(render_frame);
  // Run this before any extensions
  if (observer)
    observer->RunScriptsAtDocumentStart();

  ChromeContentRendererClient::RunScriptsAtDocumentStart(render_frame);
}

void PresearchContentRendererClient::WillEvaluateServiceWorkerOnWorkerThread(
    blink::WebServiceWorkerContextProxy* context_proxy,
    v8::Local<v8::Context> v8_context,
    int64_t service_worker_version_id,
    const GURL& service_worker_scope,
    const GURL& script_url) {
  presearch_search_service_worker_holder_.WillEvaluateServiceWorkerOnWorkerThread(
      context_proxy, v8_context, service_worker_version_id,
      service_worker_scope, script_url);
  ChromeContentRendererClient::WillEvaluateServiceWorkerOnWorkerThread(
      context_proxy, v8_context, service_worker_version_id,
      service_worker_scope, script_url);
}

void PresearchContentRendererClient::WillDestroyServiceWorkerContextOnWorkerThread(
    v8::Local<v8::Context> v8_context,
    int64_t service_worker_version_id,
    const GURL& service_worker_scope,
    const GURL& script_url) {
  presearch_search_service_worker_holder_
      .WillDestroyServiceWorkerContextOnWorkerThread(
          v8_context, service_worker_version_id, service_worker_scope,
          script_url);
  ChromeContentRendererClient::WillDestroyServiceWorkerContextOnWorkerThread(
      v8_context, service_worker_version_id, service_worker_scope, script_url);
}
