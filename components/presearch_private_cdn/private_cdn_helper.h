/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_

#include <string>

#include "base/memory/singleton.h"
#include "base/strings/string_piece.h"

namespace presearch {

class PrivateCdnHelper final {
 public:
  PrivateCdnHelper(const PrivateCdnHelper&) = delete;
  PrivateCdnHelper& operator=(const PrivateCdnHelper&) = delete;

  static PrivateCdnHelper* GetInstance() {
    return base::Singleton<PrivateCdnHelper>::get();
  }

  bool RemovePadding(base::StringPiece* padded_string) const;

 private:
  friend struct base::DefaultSingletonTraits<PrivateCdnHelper>;

  PrivateCdnHelper();
  ~PrivateCdnHelper();
};

}  // namespace presearch

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_
