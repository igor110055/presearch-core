/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/net/presearch_request_handler.h"

#include <algorithm>
#include <utility>

#include "base/feature_list.h"
#include "base/metrics/histogram_macros.h"
#include "base/task/post_task.h"
#include "presearch/browser/net/presearch_ad_block_csp_network_delegate_helper.h"
#include "presearch/browser/net/presearch_ad_block_tp_network_delegate_helper.h"
#include "presearch/browser/net/presearch_common_static_redirect_network_delegate_helper.h"
#include "presearch/browser/net/presearch_httpse_network_delegate_helper.h"
#include "presearch/browser/net/presearch_site_hacks_network_delegate_helper.h"
#include "presearch/browser/net/presearch_stp_util.h"
#include "presearch/browser/net/global_privacy_control_network_delegate_helper.h"
#include "presearch/browser/translate/buildflags/buildflags.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_referrals/buildflags/buildflags.h"
#include "presearch/components/presearch_rewards/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_shields/common/features.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/presearch_webtorrent/browser/buildflags/buildflags.h"
#include "presearch/components/decentralized_dns/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/browser_process.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/common/url_constants.h"
#include "extensions/common/constants.h"
#include "net/base/net_errors.h"

#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
#include "presearch/browser/net/presearch_referrals_network_delegate_helper.h"
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
#include "presearch/components/presearch_rewards/browser/net/network_delegate_helper.h"
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_WEBTORRENT)
#include "presearch/browser/net/presearch_torrent_redirect_network_delegate_helper.h"
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_TRANSLATE_GO)
#include "presearch/browser/net/presearch_translate_redirect_network_delegate_helper.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/browser/net/ipfs_redirect_network_delegate_helper.h"
#include "presearch/components/ipfs/features.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "presearch/browser/net/decentralized_dns_network_delegate_helper.h"
#endif

static bool IsInternalScheme(std::shared_ptr<presearch::PresearchRequestInfo> ctx) {
  DCHECK(ctx);
  return ctx->request_url.SchemeIs(extensions::kExtensionScheme) ||
         ctx->request_url.SchemeIs(content::kChromeUIScheme);
}

PresearchRequestHandler::PresearchRequestHandler() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  SetupCallbacks();
  // Initialize the preference change registrar.
  InitPrefChangeRegistrar();
}

PresearchRequestHandler::~PresearchRequestHandler() = default;

void PresearchRequestHandler::SetupCallbacks() {
  presearch::OnBeforeURLRequestCallback callback =
      base::Bind(presearch::OnBeforeURLRequest_SiteHacksWork);
  before_url_request_callbacks_.push_back(callback);

  callback = base::Bind(presearch::OnBeforeURLRequest_AdBlockTPPreWork);
  before_url_request_callbacks_.push_back(callback);

  callback = base::Bind(presearch::OnBeforeURLRequest_HttpsePreFileWork);
  before_url_request_callbacks_.push_back(callback);

  callback = base::Bind(presearch::OnBeforeURLRequest_CommonStaticRedirectWork);
  before_url_request_callbacks_.push_back(callback);

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED) && BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  callback = base::Bind(
      decentralized_dns::OnBeforeURLRequest_DecentralizedDnsPreRedirectWork);
  before_url_request_callbacks_.push_back(callback);
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
  callback = base::Bind(presearch_rewards::OnBeforeURLRequest);
  before_url_request_callbacks_.push_back(callback);
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_TRANSLATE_GO)
  callback =
      base::BindRepeating(presearch::OnBeforeURLRequest_TranslateRedirectWork);
  before_url_request_callbacks_.push_back(callback);
#endif

#if BUILDFLAG(IPFS_ENABLED)
  if (base::FeatureList::IsEnabled(ipfs::features::kIpfsFeature)) {
    callback = base::BindRepeating(ipfs::OnBeforeURLRequest_IPFSRedirectWork);
    before_url_request_callbacks_.push_back(callback);
    presearch::OnHeadersReceivedCallback ipfs_headers_received_callback =
        base::Bind(ipfs::OnHeadersReceived_IPFSRedirectWork);
    headers_received_callbacks_.push_back(ipfs_headers_received_callback);
  }
#endif

  presearch::OnBeforeStartTransactionCallback start_transaction_callback =
      base::Bind(presearch::OnBeforeStartTransaction_SiteHacksWork);
  before_start_transaction_callbacks_.push_back(start_transaction_callback);

  start_transaction_callback =
      base::Bind(presearch::OnBeforeStartTransaction_GlobalPrivacyControlWork);
  before_start_transaction_callbacks_.push_back(start_transaction_callback);

#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
  start_transaction_callback =
      base::Bind(presearch::OnBeforeStartTransaction_ReferralsWork);
  before_start_transaction_callbacks_.push_back(start_transaction_callback);
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_WEBTORRENT)
  presearch::OnHeadersReceivedCallback headers_received_callback =
      base::Bind(webtorrent::OnHeadersReceived_TorrentRedirectWork);
  headers_received_callbacks_.push_back(headers_received_callback);
#endif

  if (base::FeatureList::IsEnabled(
          ::presearch_shields::features::kPresearchAdblockCspRules)) {
    presearch::OnHeadersReceivedCallback headers_received_callback2 =
        base::Bind(presearch::OnHeadersReceived_AdBlockCspWork);
    headers_received_callbacks_.push_back(headers_received_callback2);
  }
}

void PresearchRequestHandler::InitPrefChangeRegistrar() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
  PrefService* prefs = g_browser_process->local_state();
  pref_change_registrar_.reset(new PrefChangeRegistrar());
  pref_change_registrar_->Init(prefs);
  pref_change_registrar_->Add(
      kReferralHeaders,
      base::Bind(&PresearchRequestHandler::OnReferralHeadersChanged,
                 base::Unretained(this)));
  // Retrieve current referral headers, if any.
  OnReferralHeadersChanged();
#endif
}

void PresearchRequestHandler::OnReferralHeadersChanged() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  if (const base::ListValue* referral_headers =
          g_browser_process->local_state()->GetList(kReferralHeaders)) {
    referral_headers_list_.reset(referral_headers->DeepCopy());
  }
}

bool PresearchRequestHandler::IsRequestIdentifierValid(
    uint64_t request_identifier) {
  return base::Contains(callbacks_, request_identifier);
}

int PresearchRequestHandler::OnBeforeURLRequest(
    std::shared_ptr<presearch::PresearchRequestInfo> ctx,
    net::CompletionOnceCallback callback,
    GURL* new_url) {
  if (before_url_request_callbacks_.empty() || IsInternalScheme(ctx)) {
    return net::OK;
  }
  SCOPED_UMA_HISTOGRAM_TIMER("Presearch.OnBeforeURLRequest_Handler");
  ctx->new_url = new_url;
  ctx->event_type = presearch::kOnBeforeRequest;
  callbacks_[ctx->request_identifier] = std::move(callback);
  RunNextCallback(ctx);
  return net::ERR_IO_PENDING;
}

int PresearchRequestHandler::OnBeforeStartTransaction(
    std::shared_ptr<presearch::PresearchRequestInfo> ctx,
    net::CompletionOnceCallback callback,
    net::HttpRequestHeaders* headers) {
  if (before_start_transaction_callbacks_.empty() || IsInternalScheme(ctx)) {
    return net::OK;
  }
  ctx->event_type = presearch::kOnBeforeStartTransaction;
  ctx->headers = headers;
  ctx->referral_headers_list = referral_headers_list_.get();
  callbacks_[ctx->request_identifier] = std::move(callback);
  RunNextCallback(ctx);
  return net::ERR_IO_PENDING;
}

int PresearchRequestHandler::OnHeadersReceived(
    std::shared_ptr<presearch::PresearchRequestInfo> ctx,
    net::CompletionOnceCallback callback,
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    GURL* allowed_unsafe_redirect_url) {
  if (!ctx->tab_origin.is_empty()) {
    presearch::RemoveTrackableSecurityHeadersForThirdParty(
        ctx->request_url, url::Origin::Create(ctx->tab_origin),
        original_response_headers, override_response_headers);
  }

  if (headers_received_callbacks_.empty() &&
      !ctx->request_url.SchemeIs(content::kChromeUIScheme)) {
    // Extension scheme not excluded since presearch_webtorrent needs it.
    return net::OK;
  }

  callbacks_[ctx->request_identifier] = std::move(callback);
  ctx->event_type = presearch::kOnHeadersReceived;
  ctx->original_response_headers = original_response_headers;
  ctx->override_response_headers = override_response_headers;
  ctx->allowed_unsafe_redirect_url = allowed_unsafe_redirect_url;

  RunNextCallback(ctx);
  return net::ERR_IO_PENDING;
}

void PresearchRequestHandler::OnURLRequestDestroyed(
    std::shared_ptr<presearch::PresearchRequestInfo> ctx) {
  if (base::Contains(callbacks_, ctx->request_identifier)) {
    callbacks_.erase(ctx->request_identifier);
  }
}

void PresearchRequestHandler::RunCallbackForRequestIdentifier(
    uint64_t request_identifier,
    int rv) {
  std::map<uint64_t, net::CompletionOnceCallback>::iterator it =
      callbacks_.find(request_identifier);
  // We intentionally do the async call to maintain the proper flow
  // of URLLoader callbacks.
  base::PostTask(FROM_HERE, {content::BrowserThread::UI},
                 base::BindOnce(std::move(it->second), rv));
}

// TODO(iefremov): Merge all callback containers into one and run only one loop
// instead of many (issues/5574).
void PresearchRequestHandler::RunNextCallback(
    std::shared_ptr<presearch::PresearchRequestInfo> ctx) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  if (!base::Contains(callbacks_, ctx->request_identifier)) {
    return;
  }

  // Continue processing callbacks until we hit one that returns PENDING
  int rv = net::OK;

  if (ctx->event_type == presearch::kOnBeforeRequest) {
    while (before_url_request_callbacks_.size() !=
           ctx->next_url_request_index) {
      presearch::OnBeforeURLRequestCallback callback =
          before_url_request_callbacks_[ctx->next_url_request_index++];
      presearch::ResponseCallback next_callback =
          base::Bind(&PresearchRequestHandler::RunNextCallback,
                     weak_factory_.GetWeakPtr(), ctx);
      rv = callback.Run(next_callback, ctx);
      if (rv == net::ERR_IO_PENDING) {
        return;
      }
      if (rv != net::OK) {
        break;
      }
    }
  } else if (ctx->event_type == presearch::kOnBeforeStartTransaction) {
    while (before_start_transaction_callbacks_.size() !=
           ctx->next_url_request_index) {
      presearch::OnBeforeStartTransactionCallback callback =
          before_start_transaction_callbacks_[ctx->next_url_request_index++];
      presearch::ResponseCallback next_callback =
          base::Bind(&PresearchRequestHandler::RunNextCallback,
                     weak_factory_.GetWeakPtr(), ctx);
      rv = callback.Run(ctx->headers, next_callback, ctx);
      if (rv == net::ERR_IO_PENDING) {
        return;
      }
      if (rv != net::OK) {
        break;
      }
    }
  } else if (ctx->event_type == presearch::kOnHeadersReceived) {
    while (headers_received_callbacks_.size() != ctx->next_url_request_index) {
      presearch::OnHeadersReceivedCallback callback =
          headers_received_callbacks_[ctx->next_url_request_index++];
      presearch::ResponseCallback next_callback =
          base::Bind(&PresearchRequestHandler::RunNextCallback,
                     weak_factory_.GetWeakPtr(), ctx);
      rv = callback.Run(ctx->original_response_headers,
                        ctx->override_response_headers,
                        ctx->allowed_unsafe_redirect_url, next_callback, ctx);
      if (rv == net::ERR_IO_PENDING) {
        return;
      }
      if (rv != net::OK) {
        break;
      }
    }
  }

  if (rv != net::OK) {
    RunCallbackForRequestIdentifier(ctx->request_identifier, rv);
    return;
  }

  if (ctx->event_type == presearch::kOnBeforeRequest) {
    if (!ctx->new_url_spec.empty() &&
        (ctx->new_url_spec != ctx->request_url.spec()) &&
        IsRequestIdentifierValid(ctx->request_identifier)) {
      *ctx->new_url = GURL(ctx->new_url_spec);
    }
    if (ctx->blocked_by == presearch::kAdBlocked ||
        ctx->blocked_by == presearch::kOtherBlocked) {
      if (!ctx->ShouldMockRequest()) {
        RunCallbackForRequestIdentifier(ctx->request_identifier,
                                        net::ERR_BLOCKED_BY_CLIENT);
        return;
      }
    }
  }
  RunCallbackForRequestIdentifier(ctx->request_identifier, rv);
}
