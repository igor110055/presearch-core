/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_NET_DECENTRALIZED_DNS_CONSTANTS_H_
#define PRESEARCH_NET_DECENTRALIZED_DNS_CONSTANTS_H_

namespace decentralized_dns {

constexpr char kCryptoDomain[] = ".crypto";
constexpr char kUnstoppableDomainsDoHResolver[] =
    "https://resolver.unstoppable.io/dns-query{?presearch_UD}";

constexpr char kEthDomain[] = ".eth";
constexpr char kENSDoHResolver[] =
    "https://resolver.cloudflare-eth.com/dns-query{?presearch_ENS}";

}  // namespace decentralized_dns

#endif  // PRESEARCH_NET_DECENTRALIZED_DNS_CONSTANTS_H_
