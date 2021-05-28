/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/profiles/presearch_renderer_updater.h"

#include <utility>

#include "base/bind.h"
#include "presearch/common/presearch_renderer_configuration.mojom.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_process_host.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/components/presearch_wallet/browser/presearch_wallet_constants.h"
#include "presearch/browser/presearch_wallet/presearch_wallet_context_utils.h"
#include "presearch/components/presearch_wallet/browser/pref_names.h"
#endif

PresearchRendererUpdater::PresearchRendererUpdater(Profile* profile)
    : profile_(profile) {
  PrefService* pref_service = profile->GetPrefs();
  presearch_wallet_web3_provider_.Init(kPresearchWalletWeb3Provider, pref_service);

  pref_change_registrar_.Init(pref_service);
#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  pref_change_registrar_.Add(
      kPresearchWalletWeb3Provider,
      base::BindRepeating(&PresearchRendererUpdater::UpdateAllRenderers,
                          base::Unretained(this)));
#endif
}

PresearchRendererUpdater::~PresearchRendererUpdater() {}

void PresearchRendererUpdater::InitializeRenderer(
    content::RenderProcessHost* render_process_host) {
  auto renderer_configuration = GetRendererConfiguration(render_process_host);
  UpdateRenderer(&renderer_configuration);
}

std::vector<mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>>
PresearchRendererUpdater::GetRendererConfigurations() {
  std::vector<mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>>
      rv;
  for (content::RenderProcessHost::iterator it(
           content::RenderProcessHost::AllHostsIterator());
       !it.IsAtEnd(); it.Advance()) {
    Profile* renderer_profile =
        static_cast<Profile*>(it.GetCurrentValue()->GetBrowserContext());
    if (renderer_profile == profile_ ||
        renderer_profile->GetOriginalProfile() == profile_) {
      auto renderer_configuration =
          GetRendererConfiguration(it.GetCurrentValue());
      if (renderer_configuration)
        rv.push_back(std::move(renderer_configuration));
    }
  }
  return rv;
}

mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>
PresearchRendererUpdater::GetRendererConfiguration(
    content::RenderProcessHost* render_process_host) {
  IPC::ChannelProxy* channel = render_process_host->GetChannel();
  if (!channel)
    return mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>();

  mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>
      renderer_configuration;
  channel->GetRemoteAssociatedInterface(&renderer_configuration);

  return renderer_configuration;
}

void PresearchRendererUpdater::UpdateAllRenderers() {
  auto renderer_configurations = GetRendererConfigurations();
  for (auto& renderer_configuration : renderer_configurations)
    UpdateRenderer(&renderer_configuration);
}

void PresearchRendererUpdater::UpdateRenderer(
    mojo::AssociatedRemote<presearch::mojom::PresearchRendererConfiguration>*
        renderer_configuration) {
#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  bool use_presearch_web3_provider = (static_cast<PresearchWalletWeb3ProviderTypes>(
                                      presearch_wallet_web3_provider_.GetValue()) ==
                                  PresearchWalletWeb3ProviderTypes::PRESEARCH_WALLET) &&
                                 presearch_wallet::IsAllowedForContext(profile_);

  (*renderer_configuration)
      ->SetConfiguration(
          presearch::mojom::DynamicParams::New(use_presearch_web3_provider));
#endif
}
