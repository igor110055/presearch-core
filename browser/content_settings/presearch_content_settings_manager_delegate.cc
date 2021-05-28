/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/content_settings/presearch_content_settings_manager_delegate.h"

#include <memory>
#include <utility>

#include "presearch/browser/presearch_browser_process.h"
#include "presearch/components/presearch_shields/browser/tracking_protection_service.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/content_settings/common/content_settings_manager.mojom.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"

namespace chrome {

namespace {

bool ShouldStoreState(int render_process_id,
                      int render_frame_id,
                      const GURL& url) {
  content::RenderFrameHost* frame =
      content::RenderFrameHost::FromID(render_process_id, render_frame_id);
  content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(frame);
  if (!web_contents)
    return true;

  HostContentSettingsMap* host_content_settings_map =
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromBrowserContext(web_contents->GetBrowserContext()));

  return g_presearch_browser_process->tracking_protection_service()
      ->ShouldStoreState(host_content_settings_map, render_process_id,
                         render_frame_id, url);
}

}  // namespace

PresearchContentSettingsManagerDelegate::~PresearchContentSettingsManagerDelegate() =
    default;

// content_settings::ContentSettingsManagerImpl::Delegate:
bool PresearchContentSettingsManagerDelegate::AllowStorageAccess(
    int render_process_id,
    int render_frame_id,
    content_settings::mojom::ContentSettingsManager::StorageType storage_type,
    const GURL& url,
    bool allowed,
    base::OnceCallback<void(bool)>* callback) {
  using StorageType =
      content_settings::mojom::ContentSettingsManager::StorageType;
  // LOCAL_STORAGE | SESSION_STORAGE == DOMStorage
  switch (storage_type) {
    case StorageType::DATABASE:
    case StorageType::LOCAL_STORAGE:
    case StorageType::SESSION_STORAGE:
    case StorageType::INDEXED_DB:
      if (!ShouldStoreState(render_process_id, render_frame_id, url)) {
        std::move(*callback).Run(false);
        return true;
      }
      break;
    default:
      // FILE_SYSTEM, CACHE, WEB_LOCKS
      break;
  }

  return ContentSettingsManagerDelegate::AllowStorageAccess(
      render_process_id, render_frame_id, storage_type, url, allowed, callback);
}

std::unique_ptr<content_settings::ContentSettingsManagerImpl::Delegate>
PresearchContentSettingsManagerDelegate::Clone() {
  return std::make_unique<PresearchContentSettingsManagerDelegate>();
}

}  // namespace chrome
