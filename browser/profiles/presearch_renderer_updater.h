/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PROFILES_PRESEARCH_RENDERER_UPDATER_H_
#define PRESEARCH_BROWSER_PROFILES_PRESEARCH_RENDERER_UPDATER_H_

#include <vector>

#include "base/scoped_observer.h"
#include "presearch/common/presearch_renderer_configuration.mojom-forward.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_member.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/remote.h"

class Profile;

namespace content {
class RenderProcessHost;
}

class PresearchRendererUpdater : public KeyedService {
 public:
  explicit PresearchRendererUpdater(Profile* profile);
  PresearchRendererUpdater(const PresearchRendererUpdater&) = delete;
  PresearchRendererUpdater& operator=(const PresearchRendererUpdater&) = delete;
  ~PresearchRendererUpdater() override;

  // Initialize a newly-started renderer process.
  void InitializeRenderer(content::RenderProcessHost* render_process_host);

 private:
  std::vector<mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>>
  GetRendererConfigurations();

  mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>
  GetRendererConfiguration(content::RenderProcessHost* render_process_host);

  // Update all renderers due to a configuration change.
  void UpdateAllRenderers();

  // Update the given renderer due to a configuration change.
  void UpdateRenderer(
      mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>*
          renderer_configuration);

  Profile* profile_;
  PrefChangeRegistrar pref_change_registrar_;

  // Prefs that we sync to the renderers.
  IntegerPrefMember presearch_wallet_web3_provider_;
};

#endif  // PRESEARCH_BROWSER_PROFILES_PRESEARCH_RENDERER_UPDATER_H_
