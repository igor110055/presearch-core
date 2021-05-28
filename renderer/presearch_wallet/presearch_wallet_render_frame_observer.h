/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_RENDERER_PRESEARCH_WALLET_PRESEARCH_WALLET_RENDER_FRAME_OBSERVER_H_
#define PRESEARCH_RENDERER_PRESEARCH_WALLET_PRESEARCH_WALLET_RENDER_FRAME_OBSERVER_H_

#include <memory>

#include "presearch/common/presearch_renderer_configuration.mojom.h"
#include "presearch/components/presearch_wallet/renderer/presearch_wallet_js_handler.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "third_party/blink/public/web/web_navigation_type.h"
#include "url/gurl.h"
#include "v8/include/v8.h"

namespace presearch_wallet {

class PresearchWalletRenderFrameObserver : public content::RenderFrameObserver {
 public:
  explicit PresearchWalletRenderFrameObserver(
      content::RenderFrame* render_frame,
      presearch::mojom::DynamicParams dynamic_params);
  ~PresearchWalletRenderFrameObserver() override;

  // RenderFrameObserver implementation.
  void DidStartNavigation(
      const GURL& url,
      base::Optional<blink::WebNavigationType> navigation_type) override;
  void DidCreateScriptContext(v8::Local<v8::Context> context,
                              int32_t world_id) override;

 private:
  // RenderFrameObserver implementation.
  void OnDestruct() override;

  // Handle to "handler" JavaScript object functionality.
  std::unique_ptr<PresearchWalletJSHandler> native_javascript_handle_;

  GURL url_;
  const presearch::mojom::DynamicParams dynamic_params_;
};

}  // namespace presearch_wallet

#endif  // PRESEARCH_RENDERER_PRESEARCH_WALLET_PRESEARCH_WALLET_RENDER_FRAME_OBSERVER_H_
