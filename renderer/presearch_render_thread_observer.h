/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_RENDERER_PRESEARCH_RENDER_THREAD_OBSERVER_H_
#define PRESEARCH_RENDERER_PRESEARCH_RENDER_THREAD_OBSERVER_H_

#include "presearch/common/presearch_renderer_configuration.mojom.h"
#include "content/public/renderer/render_thread_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace blink {
class AssociatedInterfaceRegistry;
}

class PresearchRenderThreadObserver
    : public content::RenderThreadObserver,
      public presearch::mojom::PresearchRendererConfiguration {
 public:
  PresearchRenderThreadObserver(const PresearchRenderThreadObserver&) = delete;
  PresearchRenderThreadObserver& operator=(const PresearchRenderThreadObserver&) =
      delete;
  PresearchRenderThreadObserver();
  ~PresearchRenderThreadObserver() override;

  // Return the dynamic parameters - those that may change while the
  // render process is running.
  static const presearch::mojom::DynamicParams& GetDynamicParams();

 private:
  // content::RenderThreadObserver:
  void RegisterMojoInterfaces(
      blink::AssociatedInterfaceRegistry* associated_interfaces) override;
  void UnregisterMojoInterfaces(
      blink::AssociatedInterfaceRegistry* associated_interfaces) override;

  // presearch::mojom::PresearchRendererConfiguration:
  void SetConfiguration(presearch::mojom::DynamicParamsPtr params) override;

  void OnRendererConfigurationAssociatedRequest(
      mojo::PendingAssociatedReceiver<presearch::mojom::PresearchRendererConfiguration>
          receiver);

  mojo::AssociatedReceiverSet<presearch::mojom::PresearchRendererConfiguration>
      renderer_configuration_receivers_;
};

#endif  // PRESEARCH_RENDERER_PRESEARCH_RENDER_THREAD_OBSERVER_H_
