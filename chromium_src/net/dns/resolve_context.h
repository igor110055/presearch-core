/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_NET_DNS_RESOLVE_CONTEXT_H_
#define PRESEARCH_CHROMIUM_SRC_NET_DNS_RESOLVE_CONTEXT_H_

namespace net {
class PresearchResolveContext;
}  // namespace net

#define GetDohServerAvailability virtual GetDohServerAvailability
#define NumAvailableDohServers virtual NumAvailableDohServers
#define PRESEARCH_RESOLVE_CONTEXT_H \
 private:                       \
  friend class PresearchResolveContext;

#include "../../../../net/dns/resolve_context.h"
#undef GetDohServerAvailability
#undef NumAvailableDohServers
#undef PRESEARCH_RESOLVE_CONTEXT_H

#endif  // PRESEARCH_CHROMIUM_SRC_NET_DNS_RESOLVE_CONTEXT_H_
