/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_REWARDS_SOURCE_H_
#define PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_REWARDS_SOURCE_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/bitmap_fetcher/bitmap_fetcher_service.h"
#include "content/public/browser/url_data_source.h"

class GURL;
class Profile;
class SkBitmap;

class PresearchRewardsSource : public content::URLDataSource {
 public:
  explicit PresearchRewardsSource(Profile* profile);
  PresearchRewardsSource(const PresearchRewardsSource&) = delete;
  PresearchRewardsSource& operator=(const PresearchRewardsSource&) = delete;

  ~PresearchRewardsSource() override;

  // content::URLDataSource implementation.
  std::string GetSource() override;
  void StartDataRequest(
      const GURL& url,
      const content::WebContents::Getter& wc_getter,
      content::URLDataSource::GotDataCallback got_data_callback)
      override;
  std::string GetMimeType(const std::string&) override;
  bool AllowCaching() override;
  bool ShouldReplaceExistingSource() override;
  bool ShouldServiceRequest(const GURL& url,
                            content::BrowserContext* browser_context,
                            int render_process_id) override;

 private:
  void OnBitmapFetched(
      content::URLDataSource::GotDataCallback got_data_callback,
      const GURL& url,
      const SkBitmap& bitmap);

  Profile* profile_;
  std::vector<GURL> resource_fetchers_;

  base::WeakPtrFactory<PresearchRewardsSource> weak_factory_{this};
};

#endif  // PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_REWARDS_SOURCE_H_
