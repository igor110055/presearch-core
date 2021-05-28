/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_shields/browser/presearch_shields_web_contents_observer.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/strings/utf_string_conversions.h"
#include "presearch/common/pref_names.h"
#include "presearch/common/render_messages.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_util.h"
#include "presearch/components/presearch_shields/common/presearch_shield_constants.h"
#include "presearch/content/common/frame_messages.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/renderer_configuration.mojom.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_utils.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "content/browser/renderer_host/frame_tree_node.h"
#include "content/browser/renderer_host/navigator.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"
#include "extensions/buildflags/buildflags.h"
#include "ipc/ipc_message_macros.h"
#include "mojo/public/cpp/bindings/associated_remote.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "presearch/common/extensions/api/presearch_shields.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "extensions/browser/event_router.h"
#include "extensions/browser/extension_api_frame_id_map.h"

using extensions::Event;
using extensions::EventRouter;
#endif

using content::Referrer;
using content::RenderFrameHost;
using content::WebContents;

namespace {

// Content Settings are only sent to the main frame currently.
// Chrome may fix this at some point, but for now we do this as a work-around.
// You can verify if this is fixed by running the following test:
// npm run test -- presearch_browser_tests --filter=PresearchContentSettingsAgentImplBrowserTest.*  // NOLINT
// Chrome seems to also have a bug with RenderFrameHostChanged not updating
// the content settings so this is fixed here too. That case is covered in
// tests by:
// npm run test -- presearch_browser_tests --filter=PresearchContentSettingsAgentImplBrowserTest.*  // NOLINT
void UpdateContentSettingsToRendererFrames(content::WebContents* web_contents) {
  for (content::RenderFrameHost* frame : web_contents->GetAllFrames()) {
    Profile* profile =
        Profile::FromBrowserContext(web_contents->GetBrowserContext());
    const HostContentSettingsMap* map =
        HostContentSettingsMapFactory::GetForProfile(profile);
    RendererContentSettingRules rules;
    GetRendererContentSettingRules(map, &rules);
    IPC::ChannelProxy* channel =
        frame->GetProcess()->GetChannel();
    // channel might be NULL in tests.
    if (channel) {
      mojo::AssociatedRemote<chrome::mojom::RendererConfiguration> rc_interface;
      channel->GetRemoteAssociatedInterface(&rc_interface);
      rc_interface->SetContentSettingRules(rules);
    }
  }
}

WebContents* GetWebContents(
    int render_process_id,
    int render_frame_id,
    int frame_tree_node_id) {
  WebContents* web_contents =
      WebContents::FromFrameTreeNodeId(frame_tree_node_id);
  if (!web_contents) {
    RenderFrameHost* rfh =
        RenderFrameHost::FromID(render_process_id, render_frame_id);
    if (!rfh) {
      return nullptr;
    }
    web_contents =
        WebContents::FromRenderFrameHost(rfh);
  }
  return web_contents;
}

}  // namespace

namespace presearch_shields {

base::Lock PresearchShieldsWebContentsObserver::frame_data_map_lock_;
std::map<PresearchShieldsWebContentsObserver::RenderFrameIdKey, GURL>
    PresearchShieldsWebContentsObserver::frame_key_to_tab_url_;
std::map<int, GURL>
    PresearchShieldsWebContentsObserver::frame_tree_node_id_to_tab_url_;

PresearchShieldsWebContentsObserver::RenderFrameIdKey::RenderFrameIdKey()
    : render_process_id(content::ChildProcessHost::kInvalidUniqueID),
      frame_routing_id(MSG_ROUTING_NONE) {}

PresearchShieldsWebContentsObserver::RenderFrameIdKey::RenderFrameIdKey(
    int render_process_id,
    int frame_routing_id)
    : render_process_id(render_process_id),
      frame_routing_id(frame_routing_id) {}

bool PresearchShieldsWebContentsObserver::RenderFrameIdKey::operator<(
    const RenderFrameIdKey& other) const {
  return std::tie(render_process_id, frame_routing_id) <
         std::tie(other.render_process_id, other.frame_routing_id);
}

bool PresearchShieldsWebContentsObserver::RenderFrameIdKey::operator==(
    const RenderFrameIdKey& other) const {
  return render_process_id == other.render_process_id &&
         frame_routing_id == other.frame_routing_id;
}

PresearchShieldsWebContentsObserver::~PresearchShieldsWebContentsObserver() {
}

PresearchShieldsWebContentsObserver::PresearchShieldsWebContentsObserver(
    WebContents* web_contents)
    : WebContentsObserver(web_contents) {
}

void PresearchShieldsWebContentsObserver::RenderFrameCreated(
    RenderFrameHost* rfh) {
  if (rfh && allowed_script_origins_.size()) {
    rfh->Send(new PresearchFrameMsg_AllowScriptsOnce(
          rfh->GetRoutingID(), allowed_script_origins_));
  }

  WebContents* web_contents = WebContents::FromRenderFrameHost(rfh);
  if (web_contents) {
    UpdateContentSettingsToRendererFrames(web_contents);

    base::AutoLock lock(frame_data_map_lock_);
    const RenderFrameIdKey key(rfh->GetProcess()->GetID(), rfh->GetRoutingID());
    frame_key_to_tab_url_[key] = web_contents->GetURL();
    frame_tree_node_id_to_tab_url_[rfh->GetFrameTreeNodeId()] =
        web_contents->GetURL();
  }
}

void PresearchShieldsWebContentsObserver::RenderFrameDeleted(
    RenderFrameHost* rfh) {
  base::AutoLock lock(frame_data_map_lock_);
  const RenderFrameIdKey key(rfh->GetProcess()->GetID(), rfh->GetRoutingID());
  frame_key_to_tab_url_.erase(key);
  frame_tree_node_id_to_tab_url_.erase(rfh->GetFrameTreeNodeId());
}

void PresearchShieldsWebContentsObserver::RenderFrameHostChanged(
    RenderFrameHost* old_host, RenderFrameHost* new_host) {
  if (old_host) {
    RenderFrameDeleted(old_host);
  }
  if (new_host) {
    RenderFrameCreated(new_host);
  }
}

void PresearchShieldsWebContentsObserver::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  RenderFrameHost* main_frame = web_contents()->GetMainFrame();
  if (!web_contents() || !main_frame) {
    return;
  }
  int process_id = main_frame->GetProcess()->GetID();
  int routing_id = main_frame->GetRoutingID();
  int tree_node_id = main_frame->GetFrameTreeNodeId();

  base::AutoLock lock(frame_data_map_lock_);
  frame_key_to_tab_url_[{process_id, routing_id}] = web_contents()->GetURL();
  frame_tree_node_id_to_tab_url_[tree_node_id] = web_contents()->GetURL();
}

// static
GURL PresearchShieldsWebContentsObserver::GetTabURLFromRenderFrameInfo(
    int render_process_id, int render_frame_id, int render_frame_tree_node_id) {
  base::AutoLock lock(frame_data_map_lock_);
  if (-1 != render_process_id && -1 != render_frame_id) {
    auto iter = frame_key_to_tab_url_.find({render_process_id,
                                            render_frame_id});
    if (iter != frame_key_to_tab_url_.end()) {
      return iter->second;
    }
  }
  if (-1 != render_frame_tree_node_id) {
    auto iter2 = frame_tree_node_id_to_tab_url_.find(render_frame_tree_node_id);
    if (iter2 != frame_tree_node_id_to_tab_url_.end()) {
      return iter2->second;
    }
  }
  return GURL();
}

bool PresearchShieldsWebContentsObserver::IsBlockedSubresource(
    const std::string& subresource) {
  return blocked_url_paths_.find(subresource) != blocked_url_paths_.end();
}

void PresearchShieldsWebContentsObserver::AddBlockedSubresource(
    const std::string& subresource) {
  blocked_url_paths_.insert(subresource);
}

// static
void PresearchShieldsWebContentsObserver::DispatchBlockedEvent(
    std::string block_type,
    std::string subresource,
    int render_process_id,
    int render_frame_id,
    int frame_tree_node_id) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  WebContents* web_contents = GetWebContents(render_process_id,
    render_frame_id, frame_tree_node_id);
  DispatchBlockedEventForWebContents(block_type, subresource, web_contents);

  if (web_contents) {
    PresearchShieldsWebContentsObserver* observer =
        PresearchShieldsWebContentsObserver::FromWebContents(web_contents);
    if (observer &&
        !observer->IsBlockedSubresource(subresource)) {
      observer->AddBlockedSubresource(subresource);
      PrefService* prefs = Profile::FromBrowserContext(
          web_contents->GetBrowserContext())->
          GetOriginalProfile()->
          GetPrefs();

      if (block_type == kAds) {
        prefs->SetUint64(kAdsBlocked, prefs->GetUint64(kAdsBlocked) + 1);
      } else if (block_type == kHTTPUpgradableResources) {
        prefs->SetUint64(kHttpsUpgrades, prefs->GetUint64(kHttpsUpgrades) + 1);
      } else if (block_type == kJavaScript) {
        prefs->SetUint64(kJavascriptBlocked,
            prefs->GetUint64(kJavascriptBlocked) + 1);
      } else if (block_type == kFingerprintingV2) {
        prefs->SetUint64(kFingerprintingBlocked,
            prefs->GetUint64(kFingerprintingBlocked) + 1);
      }
    }
  }
}

#if !defined(OS_ANDROID)
// static
void PresearchShieldsWebContentsObserver::DispatchBlockedEventForWebContents(
    const std::string& block_type, const std::string& subresource,
    WebContents* web_contents) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (!web_contents) {
    return;
  }
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  EventRouter* event_router = EventRouter::Get(profile);
  if (profile && event_router) {
    extensions::api::presearch_shields::OnBlocked::Details details;
    details.tab_id = extensions::ExtensionTabUtil::GetTabId(web_contents);
    details.block_type = block_type;
    details.subresource = subresource;
    std::unique_ptr<base::ListValue> args(
        extensions::api::presearch_shields::OnBlocked::Create(details)
          .release());
    std::unique_ptr<Event> event(
        new Event(extensions::events::PRESEARCH_AD_BLOCKED,
          extensions::api::presearch_shields::OnBlocked::kEventName,
          std::move(args)));
    event_router->BroadcastEvent(std::move(event));
  }
#endif
}
#endif

bool PresearchShieldsWebContentsObserver::OnMessageReceived(
    const IPC::Message& message, RenderFrameHost* render_frame_host) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_WITH_PARAM(PresearchShieldsWebContentsObserver,
        message, render_frame_host)
    IPC_MESSAGE_HANDLER(PresearchViewHostMsg_JavaScriptBlocked,
        OnJavaScriptBlockedWithDetail)
    IPC_MESSAGE_HANDLER(PresearchViewHostMsg_FingerprintingBlocked,
        OnFingerprintingBlockedWithDetail)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}

void PresearchShieldsWebContentsObserver::OnJavaScriptBlockedWithDetail(
    RenderFrameHost* render_frame_host,
    const base::string16& details) {
  WebContents* web_contents =
      WebContents::FromRenderFrameHost(render_frame_host);
  if (!web_contents) {
    return;
  }
  DispatchBlockedEventForWebContents(presearch_shields::kJavaScript,
      base::UTF16ToUTF8(details), web_contents);
}

void PresearchShieldsWebContentsObserver::OnFingerprintingBlockedWithDetail(
    RenderFrameHost* render_frame_host,
    const base::string16& details) {
  WebContents* web_contents =
      WebContents::FromRenderFrameHost(render_frame_host);
  if (!web_contents) {
    return;
  }
  DispatchBlockedEventForWebContents(presearch_shields::kFingerprintingV2,
      base::UTF16ToUTF8(details), web_contents);
}

// static
void PresearchShieldsWebContentsObserver::RegisterProfilePrefs(
    PrefRegistrySimple* registry) {
  registry->RegisterUint64Pref(kAdsBlocked, 0);
  registry->RegisterUint64Pref(kTrackersBlocked, 0);
  registry->RegisterUint64Pref(kJavascriptBlocked, 0);
  registry->RegisterUint64Pref(kHttpsUpgrades, 0);
  registry->RegisterUint64Pref(kFingerprintingBlocked, 0);
}

void PresearchShieldsWebContentsObserver::ReadyToCommitNavigation(
    content::NavigationHandle* navigation_handle) {
  // when the main frame navigate away
  content::ReloadType reload_type = navigation_handle->GetReloadType();
  if (navigation_handle->IsInMainFrame() &&
      !navigation_handle->IsSameDocument()) {
    if (reload_type == content::ReloadType::NONE) {
      // For new loads, we reset the counters for both blocked scripts and URLs.
      allowed_script_origins_.clear();
      blocked_url_paths_.clear();
    } else if (reload_type == content::ReloadType::NORMAL) {
      // For normal reloads (or loads to the current URL, internally converted
      // into reloads i.e see NavigationControllerImpl::NavigateWithoutEntry),
      // we only reset the counter for blocked URLs, not the one for scripts.
      blocked_url_paths_.clear();
    }
  }

  navigation_handle->GetWebContents()->SendToAllFrames(
      new PresearchFrameMsg_AllowScriptsOnce(
        MSG_ROUTING_NONE, allowed_script_origins_));
}

void PresearchShieldsWebContentsObserver::AllowScriptsOnce(
    const std::vector<std::string>& origins, WebContents* contents) {
  allowed_script_origins_ = std::move(origins);
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(PresearchShieldsWebContentsObserver)

}  // namespace presearch_shields
