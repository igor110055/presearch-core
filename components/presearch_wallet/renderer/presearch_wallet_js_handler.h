/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_RENDERER_PRESEARCH_WALLET_JS_HANDLER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_RENDERER_PRESEARCH_WALLET_JS_HANDLER_H_

#include <memory>
#include <string>

#include "presearch/components/presearch_wallet/common/presearch_wallet.mojom.h"
#include "content/public/renderer/render_frame.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "v8/include/v8.h"

namespace presearch_wallet {

class PresearchWalletJSHandler {
 public:
  explicit PresearchWalletJSHandler(content::RenderFrame* render_frame);
  ~PresearchWalletJSHandler();

  void AddJavaScriptObjectToFrame(v8::Local<v8::Context> context);

 private:
  void BindFunctionsToObject(v8::Isolate* isolate,
                             v8::Local<v8::Context> context,
                             v8::Local<v8::Object> javascript_object);

  // Adds a function to the provided object.
  template <typename Sig>
  void BindFunctionToObject(v8::Isolate* isolate,
                            v8::Local<v8::Object> javascript_object,
                            const std::string& name,
                            const base::RepeatingCallback<Sig>& callback);
  void CreateEthereumObject(v8::Isolate* isolate,
                            v8::Local<v8::Context> context);
  bool EnsureConnected();

  // A function to be called from JS
  v8::Local<v8::Promise> Request(v8::Isolate* isolate,
                                 const std::string& input);
  void OnRequest(v8::Global<v8::Promise::Resolver> promise_resolver,
                 v8::Isolate* isolate,
                 v8::Global<v8::Context> context_old,
                 const int http_code,
                 const std::string& response);

  content::RenderFrame* render_frame_;
  mojo::Remote<presearch_wallet::mojom::PresearchWalletProvider> presearch_wallet_provider_;
};

}  // namespace presearch_wallet

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_RENDERER_PRESEARCH_WALLET_JS_HANDLER_H_
