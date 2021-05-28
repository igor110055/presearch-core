/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/sync/driver/presearch_sync_auth_manager.h"

#include "base/base64.h"
#include "base/strings/string_number_conversions.h"
#include "presearch/common/network_constants.h"
#include "presearch/components/presearch_sync/crypto/crypto.h"
#include "presearch/components/presearch_sync/network_time_helper.h"

namespace syncer {

namespace {
std::string AppendPresearchServiceKeyHeaderString() {
  return std::string("\r\n") + kPresearchServicesKeyHeader + ": " +
         PRESEARCH_SERVICES_KEY;
}
}  // namespace

PresearchSyncAuthManager::PresearchSyncAuthManager(
    signin::IdentityManager* identity_manager,
    const AccountStateChangedCallback& account_state_changed,
    const CredentialsChangedCallback& credentials_changed)
    : SyncAuthManager(identity_manager,
                      account_state_changed,
                      credentials_changed) {}

PresearchSyncAuthManager::~PresearchSyncAuthManager() {}

void PresearchSyncAuthManager::DeriveSigningKeys(const std::string& seed) {
  VLOG(1) << __func__;
  if (seed.empty())
    return;
  const std::vector<uint8_t> HKDF_SALT = {
      72,  203, 156, 43,  64,  229, 225, 127, 214, 158, 50,  29,  130,
      186, 182, 207, 6,   108, 47,  254, 245, 71,  198, 109, 44,  108,
      32,  193, 221, 126, 119, 143, 112, 113, 87,  184, 239, 231, 230,
      234, 28,  135, 54,  42,  9,   243, 39,  30,  179, 147, 194, 211,
      212, 239, 225, 52,  192, 219, 145, 40,  95,  19,  142, 98};
  std::vector<uint8_t> seed_bytes;
  if (!presearch_sync::crypto::PassphraseToBytes32(seed, &seed_bytes))
    return;
  const std::string info_str = "sync-auth-key";
  std::vector<uint8_t> info(info_str.begin(), info_str.end());
  presearch_sync::crypto::DeriveSigningKeysFromSeed(seed_bytes, &HKDF_SALT, &info,
                                                &public_key_, &private_key_);
  if (registered_for_auth_notifications_)
    UpdateSyncAccountIfNecessary();
}

void PresearchSyncAuthManager::ResetKeys() {
  VLOG(1) << __func__;
  public_key_.clear();
  private_key_.clear();
  if (registered_for_auth_notifications_)
    UpdateSyncAccountIfNecessary();
}

void PresearchSyncAuthManager::RequestAccessToken() {
  VLOG(1) << __func__;
  presearch_sync::NetworkTimeHelper::GetInstance()->GetNetworkTime(
      base::BindOnce(&PresearchSyncAuthManager::OnNetworkTimeFetched,
                     weak_ptr_factory_.GetWeakPtr()));
}

SyncAccountInfo PresearchSyncAuthManager::DetermineAccountToUse() const {
  if (!public_key_.empty()) {
    const std::string client_id =
        base::HexEncode(public_key_.data(), public_key_.size());
    AccountInfo account_info;
    account_info.account_id = CoreAccountId::FromString(client_id);
    account_info.gaia = client_id;
    // about:sync-internals needs space separator in order to confine table
    // data within specific width. (ex. client_version and encrypted_types)
    account_info.email =
        std::string(client_id).insert(client_id.length() / 2, 1, ' ') +
        " @presearch.org";
    VLOG(1) << "presearch client id=" << client_id;
    return SyncAccountInfo(account_info, true);
  } else {
    return SyncAccountInfo();
  }
}

std::string PresearchSyncAuthManager::GenerateAccessToken(
    const std::string& timestamp) {
  VLOG(1) << "timestamp=" << timestamp;

  DCHECK(!timestamp.empty() && !public_key_.empty() && !private_key_.empty());
  const std::string public_key_hex =
      base::HexEncode(public_key_.data(), public_key_.size());

  const std::string timestamp_hex =
      base::HexEncode(timestamp.data(), timestamp.size());

  std::vector<uint8_t> timestamp_bytes;
  base::HexStringToBytes(timestamp_hex, &timestamp_bytes);
  std::vector<uint8_t> signature;
  presearch_sync::crypto::Sign(timestamp_bytes, private_key_, &signature);
  DCHECK(presearch_sync::crypto::Verify(timestamp_bytes, signature, public_key_));

  const std::string signed_timestamp_hex =
      base::HexEncode(signature.data(), signature.size());

  // base64(timestamp_hex|signed_timestamp_hex|public_key_hex)
  const std::string access_token =
      timestamp_hex + "|" + signed_timestamp_hex + "|" + public_key_hex;
  std::string encoded_access_token;
  base::Base64Encode(access_token, &encoded_access_token);
  DCHECK(!encoded_access_token.empty());

  return encoded_access_token + AppendPresearchServiceKeyHeaderString();
}

void PresearchSyncAuthManager::OnNetworkTimeFetched(const base::Time& time) {
  std::string timestamp = std::to_string(int64_t(time.ToJsTime()));
  if (public_key_.empty() || private_key_.empty())
    return;
  access_token_ = GenerateAccessToken(timestamp);
  if (registered_for_auth_notifications_)
    credentials_changed_callback_.Run();
}

}  // namespace syncer
