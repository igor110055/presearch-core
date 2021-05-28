/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/net/presearch_httpse_network_delegate_helper.h"

#include <algorithm>
#include <memory>
#include <string>

#include "base/task/post_task.h"
#include "base/threading/scoped_blocking_call.h"
#include "presearch/browser/presearch_browser_process.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_util.h"
#include "presearch/components/presearch_shields/browser/https_everywhere_service.h"
#include "presearch/components/presearch_shields/common/presearch_shield_constants.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/net_errors.h"

using content::BrowserThread;

namespace presearch {

void OnBeforeURLRequest_HttpseFileWork(
    std::shared_ptr<PresearchRequestInfo> ctx) {
  base::ScopedBlockingCall scoped_blocking_call(FROM_HERE,
                                                base::BlockingType::WILL_BLOCK);
  DCHECK_NE(ctx->request_identifier, 0U);
  g_presearch_browser_process->https_everywhere_service()->
    GetHTTPSURL(&ctx->request_url, ctx->request_identifier, &ctx->new_url_spec);
}

void OnBeforeURLRequest_HttpsePostFileWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<PresearchRequestInfo> ctx) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (!ctx->new_url_spec.empty() &&
    ctx->new_url_spec != ctx->request_url.spec()) {
    presearch_shields::DispatchBlockedEvent(ctx->request_url,
        ctx->render_frame_id, ctx->render_process_id, ctx->frame_tree_node_id,
        presearch_shields::kHTTPUpgradableResources);
  }

  next_callback.Run();
}

int OnBeforeURLRequest_HttpsePreFileWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<PresearchRequestInfo> ctx) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  // Don't try to overwrite an already set URL by another delegate (adblock/tp)
  if (!ctx->new_url_spec.empty()) {
    return net::OK;
  }

  if (ctx->tab_origin.is_empty() || ctx->allow_http_upgradable_resource ||
      !ctx->allow_presearch_shields) {
    return net::OK;
  }

  bool is_valid_url = true;
  is_valid_url = ctx->request_url.is_valid();
  std::string scheme = ctx->request_url.scheme();
  if (scheme.length()) {
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), ::tolower);
    if ("http" != scheme && "https" != scheme) {
      is_valid_url = false;
    }
  }

  if (is_valid_url) {
    if (!g_presearch_browser_process->https_everywhere_service()->
        GetHTTPSURLFromCacheOnly(&ctx->request_url,
                                 ctx->request_identifier,
                                 &ctx->new_url_spec)) {
      g_presearch_browser_process->https_everywhere_service()->
        GetTaskRunner()->PostTaskAndReply(FROM_HERE,
          base::Bind(OnBeforeURLRequest_HttpseFileWork, ctx),
          base::Bind(base::IgnoreResult(
              &OnBeforeURLRequest_HttpsePostFileWork),
              next_callback, ctx));
      return net::ERR_IO_PENDING;
    } else {
      if (!ctx->new_url_spec.empty()) {
        presearch_shields::DispatchBlockedEvent(ctx->request_url,
            ctx->render_frame_id, ctx->render_process_id,
            ctx->frame_tree_node_id,
            presearch_shields::kHTTPUpgradableResources);
      }
    }
  }

  return net::OK;
}

}  // namespace presearch
