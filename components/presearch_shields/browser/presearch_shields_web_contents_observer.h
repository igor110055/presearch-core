/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_PRESEARCH_SHIELDS_WEB_CONTENTS_OBSERVER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_PRESEARCH_SHIELDS_WEB_CONTENTS_OBSERVER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/macros.h"
#include "base/synchronization/lock.h"
#include "base/strings/string16.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class WebContents;
}

class PrefRegistrySimple;

namespace presearch_shields {

class PresearchShieldsWebContentsObserver : public content::WebContentsObserver,
    public content::WebContentsUserData<PresearchShieldsWebContentsObserver> {
 public:
  explicit PresearchShieldsWebContentsObserver(content::WebContents*);
  ~PresearchShieldsWebContentsObserver() override;

  static void RegisterProfilePrefs(PrefRegistrySimple* registry);
  static void DispatchBlockedEventForWebContents(
      const std::string& block_type,
      const std::string& subresource,
      content::WebContents* web_contents);
  static void DispatchBlockedEvent(
      std::string block_type,
      std::string subresource,
      int render_process_id,
      int render_frame_id, int frame_tree_node_id);
  static GURL GetTabURLFromRenderFrameInfo(int render_process_id,
                                           int render_frame_id,
                                           int render_frame_tree_node_id);
  void AllowScriptsOnce(const std::vector<std::string>& origins,
                        content::WebContents* web_contents);
  bool IsBlockedSubresource(const std::string& subresource);
  void AddBlockedSubresource(const std::string& subresource);

 protected:
    // A set of identifiers that uniquely identifies a RenderFrame.
  struct RenderFrameIdKey {
    RenderFrameIdKey();
    RenderFrameIdKey(int render_process_id, int frame_routing_id);

    // The process ID of the renderer that contains the RenderFrame.
    int render_process_id;

    // The routing ID of the RenderFrame.
    int frame_routing_id;

    bool operator<(const RenderFrameIdKey& other) const;
    bool operator==(const RenderFrameIdKey& other) const;
  };

  // content::WebContentsObserver overrides.
  void RenderFrameCreated(content::RenderFrameHost* host) override;
  void RenderFrameDeleted(content::RenderFrameHost* render_frame_host) override;
  void RenderFrameHostChanged(content::RenderFrameHost* old_host,
                              content::RenderFrameHost* new_host) override;
  void ReadyToCommitNavigation(
      content::NavigationHandle* navigation_handle) override;
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;

  // Invoked if an IPC message is coming from a specific RenderFrameHost.
  bool OnMessageReceived(const IPC::Message& message,
      content::RenderFrameHost* render_frame_host) override;
  void OnJavaScriptBlockedWithDetail(
      content::RenderFrameHost* render_frame_host,
      const base::string16& details);
  void OnFingerprintingBlockedWithDetail(
      content::RenderFrameHost* render_frame_host,
      const base::string16& details);

  // TODO(iefremov): Refactor this away or at least put into base::NoDestructor.
  // Protects global maps below from being concurrently written on the UI thread
  // and read on the IO thread.
  static base::Lock frame_data_map_lock_;
  static std::map<RenderFrameIdKey, GURL> frame_key_to_tab_url_;
  static std::map<int, GURL> frame_tree_node_id_to_tab_url_;

 private:
  friend class content::WebContentsUserData<PresearchShieldsWebContentsObserver>;
  std::vector<std::string> allowed_script_origins_;
  // We keep a set of the current page's blocked URLs in case the page
  // continually tries to load the same blocked URLs.
  std::set<std::string> blocked_url_paths_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();
  DISALLOW_COPY_AND_ASSIGN(PresearchShieldsWebContentsObserver);
};

}  // namespace presearch_shields

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_PRESEARCH_SHIELDS_WEB_CONTENTS_OBSERVER_H_
