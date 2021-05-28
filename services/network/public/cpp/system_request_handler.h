/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_SERVICES_NETWORK_PUBLIC_CPP_SYSTEM_REQUEST_HANDLER_H_
#define PRESEARCH_SERVICES_NETWORK_PUBLIC_CPP_SYSTEM_REQUEST_HANDLER_H_

#include "base/callback.h"
#include "base/component_export.h"
#include "base/memory/singleton.h"

namespace network {
struct ResourceRequest;
}  // namespace network

namespace network {

class COMPONENT_EXPORT(NETWORK_CPP) SystemRequestHandler {
 public:
  typedef base::Callback<network::ResourceRequest(
      const network::ResourceRequest&)>
      OnBeforeSystemRequestCallback;

  // Returns the instance of SystemRequestHandler.
  static SystemRequestHandler* GetInstance();

  // Registers callback to be invoked in different layers.
  void RegisterOnBeforeSystemRequestCallback(
      const OnBeforeSystemRequestCallback& cb);

  network::ResourceRequest OnBeforeSystemRequest(
      const network::ResourceRequest& url_request);

 private:
  friend struct base::DefaultSingletonTraits<SystemRequestHandler>;

  SystemRequestHandler();
  ~SystemRequestHandler();

  OnBeforeSystemRequestCallback on_before_system_request_callback_;

  DISALLOW_COPY_AND_ASSIGN(SystemRequestHandler);
};

}  // namespace network

#endif  // PRESEARCH_SERVICES_NETWORK_PUBLIC_CPP_SYSTEM_REQUEST_HANDLER_H_
