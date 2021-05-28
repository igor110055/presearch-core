/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/net/resource_context_data.h"

#include <string>
#include <utility>

#include "presearch/browser/net/presearch_proxying_url_loader_factory.h"
#include "presearch/browser/net/presearch_proxying_web_socket.h"
#include "presearch/browser/net/presearch_request_handler.h"
#include "content/public/browser/browser_context.h"
#include "net/cookies/site_for_cookies.h"

// User data key for ResourceContextData.
const void* const kResourceContextUserDataKey = &kResourceContextUserDataKey;

ResourceContextData::ResourceContextData()
    : request_id_generator_(base::MakeRefCounted<RequestIDGenerator>()),
      weak_factory_(this) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
}

ResourceContextData::~ResourceContextData() = default;

// static
void ResourceContextData::StartProxying(
    content::BrowserContext* browser_context,
    int render_process_id,
    int frame_tree_node_id,
    network::mojom::URLLoaderFactoryRequest request,
    network::mojom::URLLoaderFactoryPtrInfo target_factory) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  auto* self = static_cast<ResourceContextData*>(
      browser_context->GetUserData(kResourceContextUserDataKey));
  if (!self) {
    self = new ResourceContextData();
    browser_context->SetUserData(kResourceContextUserDataKey,
                                  base::WrapUnique(self));
  }

  if (!self->request_handler_) {
    self->request_handler_.reset(new PresearchRequestHandler);
  }

  auto proxy = std::make_unique<PresearchProxyingURLLoaderFactory>(
      self->request_handler_.get(), browser_context, render_process_id,
      frame_tree_node_id, std::move(request), std::move(target_factory),
      self->request_id_generator_,
      base::BindOnce(&ResourceContextData::RemoveProxy,
                     self->weak_factory_.GetWeakPtr()));

  self->proxies_.emplace(std::move(proxy));
}

// static
PresearchProxyingWebSocket* ResourceContextData::StartProxyingWebSocket(
    content::ContentBrowserClient::WebSocketFactory factory,
    const GURL& url,
    const net::SiteForCookies& site_for_cookies,
    const base::Optional<std::string>& user_agent,
    mojo::PendingRemote<network::mojom::WebSocketHandshakeClient>
        handshake_client,
    content::BrowserContext* browser_context,
    int render_process_id,
    int frame_id,
    int frame_tree_node_id,
    const url::Origin& origin) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  auto* self = static_cast<ResourceContextData*>(
      browser_context->GetUserData(kResourceContextUserDataKey));
  if (!self) {
    self = new ResourceContextData();
    browser_context->SetUserData(kResourceContextUserDataKey,
                                  base::WrapUnique(self));
  }

  if (!self->request_handler_) {
    self->request_handler_.reset(new PresearchRequestHandler);
  }

  network::ResourceRequest request;
  request.url = url;
  // TODO(iefremov): site_for_cookies is not enough, we should find a way
  // to initialize NetworkIsolationKey.
  request.site_for_cookies = site_for_cookies;
  if (user_agent) {
    request.headers.SetHeader(net::HttpRequestHeaders::kUserAgent, *user_agent);
  }
  request.request_initiator = origin;
  request.render_frame_id = frame_id;

  auto proxy = std::make_unique<PresearchProxyingWebSocket>(
      std::move(factory), request, std::move(handshake_client),
      render_process_id, frame_tree_node_id, browser_context,
      self->request_id_generator_, self->request_handler_.get(),
      base::BindOnce(&ResourceContextData::RemoveProxyWebSocket,
                     self->weak_factory_.GetWeakPtr()));

  auto* raw_proxy = proxy.get();
  self->websocket_proxies_.emplace(std::move(proxy));
  return raw_proxy;
}


void ResourceContextData::RemoveProxy(PresearchProxyingURLLoaderFactory* proxy) {
  auto it = proxies_.find(proxy);
  DCHECK(it != proxies_.end());
  proxies_.erase(it);
}

void ResourceContextData::RemoveProxyWebSocket(PresearchProxyingWebSocket* proxy) {
  auto it = websocket_proxies_.find(proxy);
  DCHECK(it != websocket_proxies_.end());
  websocket_proxies_.erase(it);
}

