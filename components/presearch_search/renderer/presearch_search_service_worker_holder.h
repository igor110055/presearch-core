/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SEARCH_RENDERER_PRESEARCH_SEARCH_SERVICE_WORKER_HOLDER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SEARCH_RENDERER_PRESEARCH_SEARCH_SERVICE_WORKER_HOLDER_H_

#include <memory>
#include <vector>

#include "base/threading/thread_local.h"
#include "content/public/renderer/worker_thread.h"
#include "v8/include/v8.h"

class GURL;

namespace blink {
class WebServiceWorkerContextProxy;
class ThreadSafeBrowserInterfaceBrokerProxy;
}  // namespace blink

namespace presearch_search {

class PresearchSearchJSHandler;

class PresearchSearchServiceWorkerHolder : public content::WorkerThread::Observer {
 public:
  PresearchSearchServiceWorkerHolder();
  PresearchSearchServiceWorkerHolder(const PresearchSearchServiceWorkerHolder&) =
      delete;
  PresearchSearchServiceWorkerHolder& operator=(
      const PresearchSearchServiceWorkerHolder&) = delete;
  ~PresearchSearchServiceWorkerHolder() override;

  void SetBrowserInterfaceBrokerProxy(
      blink::ThreadSafeBrowserInterfaceBrokerProxy* broker);
  void WillEvaluateServiceWorkerOnWorkerThread(
      blink::WebServiceWorkerContextProxy* context_proxy,
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url);
  void WillDestroyServiceWorkerContextOnWorkerThread(
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url);

 private:
  // WorkerThread::Observer:
  void WillStopCurrentWorkerThread() override;

  // Implement thread safety by storing each PresearchSearchJSHandler in TLS. The
  // vector is called from worker threads.
  base::ThreadLocalPointer<std::vector<std::unique_ptr<PresearchSearchJSHandler>>>
      js_handlers_tls_;
  blink::ThreadSafeBrowserInterfaceBrokerProxy* broker_;  // not owned
};

}  // namespace presearch_search

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SEARCH_RENDERER_PRESEARCH_SEARCH_SERVICE_WORKER_HOLDER_H_
