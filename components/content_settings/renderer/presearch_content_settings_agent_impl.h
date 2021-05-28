/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_CONTENT_SETTINGS_RENDERER_PRESEARCH_CONTENT_SETTINGS_AGENT_IMPL_H_
#define PRESEARCH_COMPONENTS_CONTENT_SETTINGS_RENDERER_PRESEARCH_CONTENT_SETTINGS_AGENT_IMPL_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/containers/flat_set.h"
#include "base/strings/string16.h"
#include "presearch/third_party/blink/renderer/presearch_farbling_constants.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/content_settings/renderer/content_settings_agent_impl.h"
#include "url/gurl.h"

namespace blink {
class WebLocalFrame;
}

namespace content_settings {

// Handles blocking content per content settings for each RenderFrame.
class PresearchContentSettingsAgentImpl : public ContentSettingsAgentImpl {
 public:
  PresearchContentSettingsAgentImpl(content::RenderFrame* render_frame,
                                bool should_whitelist,
                                std::unique_ptr<Delegate> delegate);
  ~PresearchContentSettingsAgentImpl() override;

 protected:
  bool AllowScript(bool enabled_per_settings) override;
  bool AllowScriptFromSource(bool enabled_per_settings,
                             const blink::WebURL& script_url) override;
  void DidNotAllowScript() override;

  bool UseEphemeralStorageSync(StorageType storage_type) override;
  bool AllowStorageAccessSync(StorageType storage_type) override;

  void PresearchSpecificDidBlockJavaScript(const base::string16& details);

  bool AllowAutoplay(bool play_requested) override;

  bool AllowFingerprinting(bool enabled_per_settings) override;
  void DidBlockFingerprinting(const base::string16& details);

  PresearchFarblingLevel GetPresearchFarblingLevel() override;

 private:
  FRIEND_TEST_ALL_PREFIXES(PresearchContentSettingsAgentImplAutoplayBrowserTest,
                           AutoplayBlockedByDefault);
  FRIEND_TEST_ALL_PREFIXES(PresearchContentSettingsAgentImplAutoplayBrowserTest,
                           AutoplayAllowedByDefault);

  bool IsPresearchShieldsDown(
      const blink::WebFrame* frame,
      const GURL& secondary_url);

  // RenderFrameObserver
  bool OnMessageReceived(const IPC::Message& message) override;
  void OnAllowScriptsOnce(const std::vector<std::string>& origins);
  void DidCommitProvisionalLoad(ui::PageTransition transition) override;

  bool IsScriptTemporilyAllowed(const GURL& script_url);
  bool AllowStorageAccessForMainFrameSync(StorageType storage_type);

  // Origins of scripts which are temporary allowed for this frame in the
  // current load
  base::flat_set<std::string> temporarily_allowed_scripts_;

  // cache blocked script url which will later be used in `DidNotAllowScript()`
  GURL blocked_script_url_;

  // temporary allowed script origins we preloaded for the next load
  base::flat_set<std::string> preloaded_temporarily_allowed_scripts_;

  using StoragePermissionsKey = std::pair<url::Origin, StorageType>;
  base::flat_map<StoragePermissionsKey, bool> cached_storage_permissions_;

  DISALLOW_COPY_AND_ASSIGN(PresearchContentSettingsAgentImpl);
};

}  // namespace content_settings

#endif  // PRESEARCH_COMPONENTS_CONTENT_SETTINGS_RENDERER_PRESEARCH_CONTENT_SETTINGS_AGENT_IMPL_H_
