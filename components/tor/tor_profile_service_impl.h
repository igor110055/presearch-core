/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_TOR_TOR_PROFILE_SERVICE_IMPL_H_
#define PRESEARCH_COMPONENTS_TOR_TOR_PROFILE_SERVICE_IMPL_H_

#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "base/optional.h"
#include "presearch/components/tor/presearch_tor_client_updater.h"
#include "presearch/components/tor/tor_launcher_factory.h"
#include "presearch/components/tor/tor_launcher_observer.h"
#include "presearch/components/tor/tor_profile_service.h"
#include "net/proxy_resolution/proxy_info.h"

namespace content {
class BrowserContext;
}  // namespace content

namespace net {
class ProxyConfigService;
class ProxyConfigServiceTor;
}

namespace tor {

using NewTorCircuitCallback = base::OnceCallback<void(
    const base::Optional<net::ProxyInfo>& proxy_info)>;

class TorProfileServiceImpl : public TorProfileService,
                              public PresearchTorClientUpdater::Observer,
                              public TorLauncherObserver {
 public:
  TorProfileServiceImpl(content::BrowserContext* context,
                        PresearchTorClientUpdater* tor_client_updater,
                        const base::FilePath& user_data_dir);
  ~TorProfileServiceImpl() override;

  // TorProfileService:
  void RegisterTorClientUpdater() override;
  void UnregisterTorClientUpdater() override;
  void SetNewTorCircuit(content::WebContents* web_contents) override;
  std::unique_ptr<net::ProxyConfigService> CreateProxyConfigService() override;
  bool IsTorConnected() override;
  void KillTor() override;
  void SetTorLauncherFactoryForTest(TorLauncherFactory* factory) override;

  // TorLauncherObserver:
  void OnTorNewProxyURI(const std::string& uri) override;

 private:
  void LaunchTor();

  base::FilePath GetTorExecutablePath();
  base::FilePath GetTorDataPath();
  base::FilePath GetTorWatchPath();

  // PresearchTorClientUpdater::Observer
  void OnExecutableReady(const base::FilePath& path) override;

  content::BrowserContext* context_ = nullptr;
  PresearchTorClientUpdater* tor_client_updater_ = nullptr;
  base::FilePath user_data_dir_;
  TorLauncherFactory* tor_launcher_factory_;  // Singleton
  net::ProxyConfigServiceTor* proxy_config_service_;  // NOT OWNED
  base::WeakPtrFactory<TorProfileServiceImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(TorProfileServiceImpl);
};

}  // namespace tor

#endif  // PRESEARCH_COMPONENTS_TOR_TOR_PROFILE_SERVICE_IMPL_H_
