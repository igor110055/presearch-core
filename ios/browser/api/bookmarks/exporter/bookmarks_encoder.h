/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_IOS_BROWSER_API_BOOKMARKS_EXPORTER_BOOKMARKS_ENCODER_H_
#define PRESEARCH_IOS_BROWSER_API_BOOKMARKS_EXPORTER_BOOKMARKS_ENCODER_H_

#include <memory>
#include <vector>

namespace bookmarks {
class BookmarkNode;
}  // namespace bookmarks

namespace base {
class Value;
}  // namespace base

namespace ios {
namespace bookmarks_encoder {
std::unique_ptr<base::Value> Encode(
    const bookmarks::BookmarkNode* bookmark_bar_node,
    const bookmarks::BookmarkNode* other_folder_node,
    const bookmarks::BookmarkNode* mobile_folder_node);
}  // namespace bookmarks_encoder
}  // namespace ios

#endif  // PRESEARCH_IOS_BROWSER_API_BOOKMARKS_EXPORTER_BOOKMARKS_ENCODER_H_
