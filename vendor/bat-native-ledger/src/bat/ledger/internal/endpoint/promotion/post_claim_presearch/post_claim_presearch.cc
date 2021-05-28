/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "bat/ledger/internal/endpoint/promotion/post_claim_presearch/post_claim_presearch.h"

#include <utility>

#include "base/json/json_writer.h"
#include "base/strings/stringprintf.h"
#include "bat/ledger/internal/common/request_util.h"
#include "bat/ledger/internal/endpoint/promotion/promotions_util.h"
#include "bat/ledger/internal/ledger_impl.h"
#include "net/http/http_status_code.h"

using std::placeholders::_1;

namespace {

std::string GetPath(const std::string& payment_id) {
  return base::StringPrintf(
      "/v3/wallet/presearch/%s/claim",
      payment_id.c_str());
}

}  // namespace

namespace ledger {
namespace endpoint {
namespace promotion {

PostClaimPresearch::PostClaimPresearch(LedgerImpl* ledger) : ledger_(ledger) {
  DCHECK(ledger_);
}

PostClaimPresearch::~PostClaimPresearch() = default;

std::string PostClaimPresearch::GetUrl() {
  const auto wallet = ledger_->wallet()->GetWallet();
  if (!wallet) {
    BLOG(0, "Wallet is null");
    return "";
  }

  const std::string& path = GetPath(wallet->payment_id);

  return GetServerUrl(path);
}

std::string PostClaimPresearch::GeneratePayload(
    const std::string& destination_payment_id) {
  base::Value payload(base::Value::Type::DICTIONARY);
  payload.SetStringKey("depositDestination", destination_payment_id);
  std::string json;
  base::JSONWriter::Write(payload, &json);

  return json;
}

type::Result PostClaimPresearch::CheckStatusCode(const int status_code) {
  if (status_code == net::HTTP_BAD_REQUEST) {
    BLOG(0, "Invalid request");
    return type::Result::LEDGER_ERROR;
  }

  if (status_code == net::HTTP_NOT_FOUND) {
    BLOG(0, "Not found");
    return type::Result::NOT_FOUND;
  }

  if (status_code == net::HTTP_CONFLICT) {
    BLOG(0, "Conflict");
    return type::Result::ALREADY_EXISTS;
  }

  if (status_code == net::HTTP_INTERNAL_SERVER_ERROR) {
    BLOG(0, "Internal server error");
    return type::Result::LEDGER_ERROR;
  }

  if (status_code != net::HTTP_OK) {
    return type::Result::LEDGER_ERROR;
  }

  return type::Result::LEDGER_OK;
}

void PostClaimPresearch::Request(
    const std::string& destination_payment_id,
    PostClaimPresearchCallback callback) {
  auto url_callback = std::bind(&PostClaimPresearch::OnRequest,
      this,
      _1,
      callback);
  const std::string payload = GeneratePayload(destination_payment_id);

  const auto wallet = ledger_->wallet()->GetWallet();
  if (!wallet) {
    BLOG(0, "Wallet is null");
    callback(type::Result::LEDGER_ERROR);
    return;
  }

  const auto sign_url =  base::StringPrintf(
      "post %s",
      GetPath(wallet->payment_id).c_str());
  auto headers = util::BuildSignHeaders(
      sign_url,
      payload,
      wallet->payment_id,
      wallet->recovery_seed);

  auto request = type::UrlRequest::New();
  request->url = GetUrl();
  request->content = payload;
  request->headers = headers;
  request->content_type = "application/json; charset=utf-8";
  request->method = type::UrlMethod::POST;
  ledger_->LoadURL(std::move(request), url_callback);
}

void PostClaimPresearch::OnRequest(
    const type::UrlResponse& response,
    PostClaimPresearchCallback callback) {
  ledger::LogUrlResponse(__func__, response);
  callback(CheckStatusCode(response.status_code));
}

}  // namespace promotion
}  // namespace endpoint
}  // namespace ledger
