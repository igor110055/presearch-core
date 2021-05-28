/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "presearch/components/tor/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/browser/tor/tor_profile_service_factory.h"
#include "presearch/components/tor/tor_profile_service.h"
#include "net/proxy_resolution/proxy_config_service.h"
#endif

namespace {

#if BUILDFLAG(ENABLE_TOR)
std::unique_ptr<net::ProxyConfigService> CreateProxyConfigServiceTor(
    Profile* profile) {
  auto* tor_service = TorProfileServiceFactory::GetForContext(profile);
  DCHECK(tor_service);
  return tor_service->CreateProxyConfigService();
}
#endif  // BUILDFLAG(ENABLE_TOR)

}  // namespace

#if BUILDFLAG(ENABLE_TOR)
#define PRESEARCH_PROXY_CONFIG_MONITOR \
  if (profile && profile->IsTor()) \
    proxy_config_service_ = CreateProxyConfigServiceTor(profile); \
  else
#else
#define PRESEARCH_PROXY_CONFIG_MONITOR
#endif

#include "../../../../../chrome/browser/net/proxy_config_monitor.cc"  // NOLINT
