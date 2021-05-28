/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/renderer/presearch_wallet/presearch_wallet_render_frame_observer.h"

#include "content/public/renderer/render_frame.h"
#include "third_party/blink/public/web/web_local_frame.h"

namespace presearch_wallet {

PresearchWalletRenderFrameObserver::PresearchWalletRenderFrameObserver(
    content::RenderFrame* render_frame,
    presearch::mojom::DynamicParams dynamic_params)
    : RenderFrameObserver(render_frame), dynamic_params_(dynamic_params) {
  native_javascript_handle_.reset(new PresearchWalletJSHandler(render_frame));
}

PresearchWalletRenderFrameObserver::~PresearchWalletRenderFrameObserver() {}

void PresearchWalletRenderFrameObserver::DidStartNavigation(
    const GURL& url,
    base::Optional<blink::WebNavigationType> navigation_type) {
  url_ = url;
}

void PresearchWalletRenderFrameObserver::DidCreateScriptContext(
    v8::Local<v8::Context> context,
    int32_t world_id) {
  // There could be empty, invalid and "about:blank" URLs,
  // they should fallback to the main frame rules
  if (url_.is_empty() || !url_.is_valid() || url_.spec() == "about:blank")
    url_ = url::Origin(render_frame()->GetWebFrame()->GetSecurityOrigin())
               .GetURL();

  if (!dynamic_params_.presearch_use_native_wallet || !native_javascript_handle_ ||
      !url_.SchemeIsHTTPOrHTTPS())
    return;

  native_javascript_handle_->AddJavaScriptObjectToFrame(context);
}

void PresearchWalletRenderFrameObserver::OnDestruct() {
  delete this;
}

}  // namespace presearch_wallet
