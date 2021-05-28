/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/presearch_rewards_source.h"

#include <utility>

#include "base/memory/ref_counted_memory.h"
#include "chrome/browser/bitmap_fetcher/bitmap_fetcher_service.h"
#include "chrome/browser/bitmap_fetcher/bitmap_fetcher_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/codec/png_codec.h"
#include "url/gurl.h"

namespace {

scoped_refptr<base::RefCountedMemory> BitmapToMemory(const SkBitmap* image) {
  base::RefCountedBytes* image_bytes = new base::RefCountedBytes;
  gfx::PNGCodec::EncodeBGRASkBitmap(*image, false, &image_bytes->data());
  return image_bytes;
}

}  // namespace

PresearchRewardsSource::PresearchRewardsSource(Profile* profile)
    : profile_(profile->GetOriginalProfile()) {}

PresearchRewardsSource::~PresearchRewardsSource() {
}

std::string PresearchRewardsSource::GetSource() {
  return "rewards-image";
}

void PresearchRewardsSource::StartDataRequest(
    const GURL& url,
    const content::WebContents::Getter& wc_getter,
    content::URLDataSource::GotDataCallback got_data_callback) {
  // URL here comes in the form of
  // chrome://rewards-image/https://rewards.presearch.org/...
  // We need to take the path and make it into a URL.
  GURL actual_url(URLDataSource::URLToRequestPath(url));
  if (!actual_url.is_valid()) {
    std::move(got_data_callback).Run(nullptr);
    return;
  }

  auto it =
      find(resource_fetchers_.begin(), resource_fetchers_.end(), actual_url);
  if (it != resource_fetchers_.end()) {
    LOG(WARNING) << "Already fetching specified Presearch Rewards resource, url: "
                 << actual_url;
    return;
  }

  BitmapFetcherService* image_service =
      BitmapFetcherServiceFactory::GetForBrowserContext(profile_);
  if (image_service) {
    net::NetworkTrafficAnnotationTag traffic_annotation =
        net::DefineNetworkTrafficAnnotation("presearch_rewards_resource_fetcher", R"(
        semantics {
          sender:
            "Presearch Rewards resource fetcher"
          description:
            "Fetches resources related to Presearch Rewards."
          trigger:
            "User visits a media publisher's site."
          data: "Presearch Rewards related resources."
          destination: WEBSITE
        }
        policy {
          cookies_allowed: NO
          setting:
            "This feature cannot be disabled by settings."
          policy_exception_justification:
            "Not implemented."
        })");
    resource_fetchers_.emplace_back(actual_url);
    image_service->RequestImage(
        actual_url,
        base::BindOnce(&PresearchRewardsSource::OnBitmapFetched,
                       weak_factory_.GetWeakPtr(), std::move(got_data_callback),
                       actual_url),
        traffic_annotation);
  }
}

std::string PresearchRewardsSource::GetMimeType(const std::string&) {
  // We need to explicitly return a mime type, otherwise if the user tries to
  // drag the image they get no extension.
  return "image/png";
}

bool PresearchRewardsSource::AllowCaching() {
  return false;
}

bool PresearchRewardsSource::ShouldReplaceExistingSource() {
  // Leave the existing DataSource in place, otherwise we'll drop any pending
  // requests on the floor.
  return false;
}

bool PresearchRewardsSource::ShouldServiceRequest(
    const GURL& url,
    content::BrowserContext* browser_context,
    int render_process_id) {
  return URLDataSource::ShouldServiceRequest(url, browser_context,
                                             render_process_id);
}

void PresearchRewardsSource::OnBitmapFetched(
    content::URLDataSource::GotDataCallback got_data_callback,
    const GURL& url,
    const SkBitmap& bitmap) {
  if (bitmap.isNull()) {
    LOG(ERROR) << "Failed to retrieve Presearch Rewards resource, url: " << url;
    std::move(got_data_callback).Run(nullptr);
    return;
  }

  std::move(got_data_callback).Run(BitmapToMemory(&bitmap).get());

  auto it_url =
      find(resource_fetchers_.begin(), resource_fetchers_.end(), url);
  if (it_url != resource_fetchers_.end()) {
    resource_fetchers_.erase(it_url);
  }
}
