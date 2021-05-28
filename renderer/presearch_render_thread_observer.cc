/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/renderer/presearch_render_thread_observer.h"

#include <utility>

#include "base/logging.h"
#include "base/no_destructor.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"

namespace {

presearch::mojom::DynamicParams* GetDynamicConfigParams() {
  static base::NoDestructor<presearch::mojom::DynamicParams> dynamic_params;
  return dynamic_params.get();
}

}  // namespace

PresearchRenderThreadObserver::PresearchRenderThreadObserver() {}

PresearchRenderThreadObserver::~PresearchRenderThreadObserver() {}

// static
const presearch::mojom::DynamicParams&
PresearchRenderThreadObserver::GetDynamicParams() {
  return *GetDynamicConfigParams();
}

void PresearchRenderThreadObserver::RegisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->AddInterface(base::BindRepeating(
      &PresearchRenderThreadObserver::OnRendererConfigurationAssociatedRequest,
      base::Unretained(this)));
}

void PresearchRenderThreadObserver::UnregisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->RemoveInterface(
      presearch::mojom::PresearchRendererConfiguration::Name_);
}

void PresearchRenderThreadObserver::OnRendererConfigurationAssociatedRequest(
    mojo::PendingAssociatedReceiver<presearch::mojom::PresearchRendererConfiguration>
        receiver) {
  renderer_configuration_receivers_.Add(this, std::move(receiver));
}

void PresearchRenderThreadObserver::SetConfiguration(
    presearch::mojom::DynamicParamsPtr params) {
  *GetDynamicConfigParams() = std::move(*params);
}
