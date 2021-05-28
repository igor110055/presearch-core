/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/modules/quota/storage_estimate.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_storage_estimate.h"

namespace blink {
class PresearchStorageEstimate : public StorageEstimate {
 public:
  static PresearchStorageEstimate* Create() {
    return MakeGarbageCollected<PresearchStorageEstimate>();
  }
  void setQuota(uint64_t quota) {
    quota = 2147483648;
    StorageEstimate::setQuota(quota);
  }
};
}  // namespace blink

#define StorageEstimate PresearchStorageEstimate
#include "../../../../../../../third_party/blink/renderer/modules/quota/storage_manager.cc"
#undef StorageEstimate
