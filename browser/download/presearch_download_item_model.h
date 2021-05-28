/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_DOWNLOAD_PRESEARCH_DOWNLOAD_ITEM_MODEL_H_
#define PRESEARCH_BROWSER_DOWNLOAD_PRESEARCH_DOWNLOAD_ITEM_MODEL_H_

#include "base/macros.h"
#include "base/strings/string16.h"

// The purpose of this class is to extend DonwloadItemModel's class
// functionality by adding a method for the origin URL text and
// a method that returns tool tip text that includes origin URL.
// This class, however, doesn't inherit from DownloadItemModel because
// DownloadItemView has a member of DownloadItemModel type. To use this
// extended functionality, PresearchDownloadItemView will have this model as
// a member.
class DownloadUIModel;

class PresearchDownloadItemModel {
 public:
  // Constructs a PresearchDownloadItemModel that encapsulates DownloadItemModel.
  explicit PresearchDownloadItemModel(DownloadUIModel* model);
  ~PresearchDownloadItemModel();

  // Method that returns a string suitable for use as a tooltip. For
  // a regular download, the tooltip is the filename and the origin URL with an
  // indicator if the URL is secure. For an interrupted download, falls back on
  // the base class behavior.
  base::string16 GetTooltipText();

  // Returns a string suitable for use as the origin URL. |is_secure| is set to
  // true if the url is considered secure.
  base::string16 GetOriginURLText(bool* is_secure);

  // Encapsulated model.
  DownloadUIModel* model_;

  DISALLOW_COPY_AND_ASSIGN(PresearchDownloadItemModel);
};

#endif  // PRESEARCH_BROWSER_DOWNLOAD_PRESEARCH_DOWNLOAD_ITEM_MODEL_H_
