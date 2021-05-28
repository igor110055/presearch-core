/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"
#include "presearch/components/presearch_ads/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_rewards/browser/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"

#if !defined(OS_ANDROID)
#include "presearch/utility/importer/presearch_profile_import_impl.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/components/services/ipfs/ipfs_service_impl.h"
#include "presearch/components/services/ipfs/public/mojom/ipfs_service.mojom.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/components/services/tor/public/interfaces/tor.mojom.h"
#include "presearch/components/services/tor/tor_launcher_impl.h"
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
#include "presearch/components/services/bat_ledger/bat_ledger_service_impl.h"
#include "presearch/components/services/bat_ledger/public/interfaces/bat_ledger.mojom.h"
#endif

#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
#include "presearch/components/services/bat_ads/bat_ads_service_impl.h"
#include "presearch/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#endif

namespace {

#if !defined(OS_ANDROID)
auto RunPresearchProfileImporter(
    mojo::PendingReceiver<presearch::mojom::ProfileImport> receiver) {
  return std::make_unique<PresearchProfileImportImpl>(std::move(receiver));
}
#endif

#if BUILDFLAG(IPFS_ENABLED)
auto RunIpfsService(mojo::PendingReceiver<ipfs::mojom::IpfsService> receiver) {
  return std::make_unique<ipfs::IpfsServiceImpl>(std::move(receiver));
}
#endif

#if BUILDFLAG(ENABLE_TOR)
auto RunTorLauncher(mojo::PendingReceiver<tor::mojom::TorLauncher> receiver) {
  return std::make_unique<tor::TorLauncherImpl>(std::move(receiver));
}
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
auto RunBatLedgerService(
    mojo::PendingReceiver<bat_ledger::mojom::BatLedgerService> receiver) {
  return std::make_unique<bat_ledger::BatLedgerServiceImpl>(
      std::move(receiver));
}
#endif

#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
auto RunBatAdsService(
    mojo::PendingReceiver<bat_ads::mojom::BatAdsService> receiver) {
  return std::make_unique<bat_ads::BatAdsServiceImpl>(
      std::move(receiver));
}
#endif

}  // namespace

#if defined(OS_ANDROID)
#define PRESEARCH_PROFILE_IMPORTER
#else
#define PRESEARCH_PROFILE_IMPORTER services.Add(RunPresearchProfileImporter);
#endif

#if BUILDFLAG(IPFS_ENABLED)
#define PRESEARCH_IPFS_SERVICE services.Add(RunIpfsService);
#else
#define PRESEARCH_IPFS_SERVICE
#endif

#if BUILDFLAG(ENABLE_TOR)
#define PRESEARCH_TOR_LAUNCHER services.Add(RunTorLauncher);
#else
#define PRESEARCH_TOR_LAUNCHER
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
#define PRESEARCH_BAT_LEDGER_SERVICE services.Add(RunBatLedgerService);
#else
#define PRESEARCH_BAT_LEDGER_SERVICE
#endif

#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
#define PRESEARCH_BAT_ADS_SERVICE services.Add(RunBatAdsService);
#else
#define PRESEARCH_BAT_ADS_SERVICE
#endif

#define PRESEARCH_GET_MAIN_THREAD_SERVICE_FACTORY \
    PRESEARCH_PROFILE_IMPORTER \
    PRESEARCH_IPFS_SERVICE \
    PRESEARCH_TOR_LAUNCHER \
    PRESEARCH_BAT_LEDGER_SERVICE \
    PRESEARCH_BAT_ADS_SERVICE

#include "../../../../chrome/utility/services.cc"

#undef PRESEARCH_GET_MAIN_THREAD_SERVICE_FACTORY
#undef PRESEARCH_PROFILE_IMPORTER
#undef PRESEARCH_IPFS_SERVICE
#undef PRESEARCH_TOR_LAUNCHER
#undef PRESEARCH_BAT_LEDGER_SERVICE
#undef PRESEARCH_BAT_ADS_SERVICE
