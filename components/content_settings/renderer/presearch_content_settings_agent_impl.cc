/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/content_settings/renderer/presearch_content_settings_agent_impl.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/callback_helpers.h"
#include "base/feature_list.h"
#include "base/stl_util.h"
#include "base/strings/utf_string_conversions.h"
#include "presearch/common/render_messages.h"
#include "presearch/components/presearch_shields/common/presearch_shield_utils.h"
#include "presearch/components/presearch_shields/common/features.h"
#include "presearch/content/common/frame_messages.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/content_settings/core/common/content_settings_utils.h"
#include "content/public/renderer/render_frame.h"
#include "net/base/features.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "third_party/blink/public/platform/web_url.h"
#include "third_party/blink/public/web/web_document.h"
#include "third_party/blink/public/web/web_frame.h"
#include "third_party/blink/public/web/web_local_frame.h"
#include "url/url_constants.h"

namespace content_settings {
namespace {

bool IsFrameWithOpaqueOrigin(blink::WebFrame* frame) {
  // Storage access is keyed off the top origin and the frame's origin.
  // It will be denied any opaque origins so have this method to return early
  // instead of making a Sync IPC call.
  return frame->GetSecurityOrigin().IsOpaque() ||
         frame->Top()->GetSecurityOrigin().IsOpaque();
}

GURL GetOriginOrURL(const blink::WebFrame* frame) {
  url::Origin top_origin = url::Origin(frame->Top()->GetSecurityOrigin());
  // The |top_origin| is unique ("null") e.g., for file:// URLs. Use the
  // document URL as the primary URL in those cases.
  // TODO(alexmos): This is broken for --site-per-process, since top() can be a
  // WebRemoteFrame which does not have a document(), and the WebRemoteFrame's
  // URL is not replicated.  See https://crbug.com/628759.
  if (top_origin.opaque() && frame->Top()->IsWebLocalFrame())
    return frame->Top()->ToWebLocalFrame()->GetDocument().Url();
  return top_origin.GetURL();
}

bool IsPresearchShieldsDown(const blink::WebFrame* frame,
                        const GURL& secondary_url,
                        const ContentSettingsForOneType& rules) {
  ContentSetting setting = CONTENT_SETTING_DEFAULT;
  const GURL& primary_url = GetOriginOrURL(frame);

  for (const auto& rule : rules) {
    if (rule.primary_pattern.Matches(primary_url) &&
        rule.secondary_pattern.Matches(secondary_url)) {
      setting = rule.GetContentSetting();
      break;
    }
  }

  return setting == CONTENT_SETTING_BLOCK;
}

}  // namespace

PresearchContentSettingsAgentImpl::PresearchContentSettingsAgentImpl(
    content::RenderFrame* render_frame,
    bool should_whitelist,
    std::unique_ptr<Delegate> delegate)
    : ContentSettingsAgentImpl(render_frame,
                               should_whitelist,
                               std::move(delegate)) {}

PresearchContentSettingsAgentImpl::~PresearchContentSettingsAgentImpl() {}

bool PresearchContentSettingsAgentImpl::OnMessageReceived(
    const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(PresearchContentSettingsAgentImpl, message)
    IPC_MESSAGE_HANDLER(PresearchFrameMsg_AllowScriptsOnce, OnAllowScriptsOnce)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  if (handled)
    return true;
  return ContentSettingsAgentImpl::OnMessageReceived(message);
}

void PresearchContentSettingsAgentImpl::OnAllowScriptsOnce(
    const std::vector<std::string>& origins) {
  preloaded_temporarily_allowed_scripts_ = std::move(origins);
}

void PresearchContentSettingsAgentImpl::DidCommitProvisionalLoad(
    ui::PageTransition transition) {
  temporarily_allowed_scripts_ =
      std::move(preloaded_temporarily_allowed_scripts_);
  ContentSettingsAgentImpl::DidCommitProvisionalLoad(transition);
}

bool PresearchContentSettingsAgentImpl::IsScriptTemporilyAllowed(
    const GURL& script_url) {
  // Check if scripts from this origin are temporily allowed or not.
  // Also matches the full script URL to support data URL cases which we use
  // the full URL to allow it.
  return base::Contains(temporarily_allowed_scripts_,
                        script_url.GetOrigin().spec()) ||
         base::Contains(temporarily_allowed_scripts_, script_url.spec());
}

void PresearchContentSettingsAgentImpl::PresearchSpecificDidBlockJavaScript(
    const base::string16& details) {
  Send(new PresearchViewHostMsg_JavaScriptBlocked(routing_id(), details));
}

bool PresearchContentSettingsAgentImpl::AllowScript(bool enabled_per_settings) {
  // clear cached url for other flow like directly calling `DidNotAllowScript`
  // without calling `AllowScriptFromSource` first
  blocked_script_url_ = GURL::EmptyGURL();

  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();
  const GURL secondary_url(url::Origin(frame->GetSecurityOrigin()).GetURL());

  bool allow = ContentSettingsAgentImpl::AllowScript(enabled_per_settings);
  allow = allow || IsPresearchShieldsDown(frame, secondary_url) ||
          IsScriptTemporilyAllowed(secondary_url);

  return allow;
}

void PresearchContentSettingsAgentImpl::DidNotAllowScript() {
  if (!blocked_script_url_.is_empty()) {
    PresearchSpecificDidBlockJavaScript(
        base::UTF8ToUTF16(blocked_script_url_.spec()));
    blocked_script_url_ = GURL::EmptyGURL();
  }
  ContentSettingsAgentImpl::DidNotAllowScript();
}

bool PresearchContentSettingsAgentImpl::UseEphemeralStorageSync(
    StorageType storage_type) {
  if (!base::FeatureList::IsEnabled(net::features::kPresearchEphemeralStorage))
    return false;

  if (storage_type != StorageType::kLocalStorage &&
      storage_type != StorageType::kSessionStorage)
    return false;

  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();

  if (!frame || IsFrameWithOpaqueOrigin(frame))
    return false;

  auto top_origin = url::Origin(frame->Top()->GetSecurityOrigin());
  if (net::registry_controlled_domains::SameDomainOrHost(
          top_origin, url::Origin(frame->GetSecurityOrigin()),
          net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES))
    return false;

  return  // block 3p
      !ContentSettingsAgentImpl::AllowStorageAccessSync(storage_type) &&
      // allow 1p
      AllowStorageAccessForMainFrameSync(storage_type);
}

bool PresearchContentSettingsAgentImpl::AllowStorageAccessSync(
    StorageType storage_type) {
  bool result = ContentSettingsAgentImpl::AllowStorageAccessSync(storage_type);

  if (result || UseEphemeralStorageSync(storage_type))
    return true;

  return false;
}

bool PresearchContentSettingsAgentImpl::AllowStorageAccessForMainFrameSync(
    StorageType storage_type) {
  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();

  if (IsFrameWithOpaqueOrigin(frame))
    return false;

  bool result = false;

  StoragePermissionsKey key(url::Origin(frame->GetSecurityOrigin()),
                            storage_type);
  const auto permissions = cached_storage_permissions_.find(key);
  if (permissions != cached_storage_permissions_.end())
    return permissions->second;

  // check for block all by looking at top domain only
  GetContentSettingsManager().AllowStorageAccess(
      routing_id(), ConvertToMojoStorageType(storage_type),
      frame->GetDocument().TopFrameOrigin(),
      url::Origin(frame->GetDocument().TopFrameOrigin()).GetURL(),
      frame->GetDocument().TopFrameOrigin(), &result);
  cached_storage_permissions_[key] = result;

  return result;
}

bool PresearchContentSettingsAgentImpl::AllowScriptFromSource(
    bool enabled_per_settings,
    const blink::WebURL& script_url) {
  const GURL secondary_url(script_url);

  bool allow = ContentSettingsAgentImpl::AllowScriptFromSource(
      enabled_per_settings, script_url);

  // scripts with whitelisted protocols, such as chrome://extensions should
  // be allowed
  bool should_white_list = IsAllowlistedForContentSettings(
      blink::WebSecurityOrigin::Create(script_url),
      render_frame()->GetWebFrame()->GetDocument().Url());

  allow = allow || should_white_list ||
          IsPresearchShieldsDown(render_frame()->GetWebFrame(), secondary_url) ||
          IsScriptTemporilyAllowed(secondary_url);

  if (!allow) {
    blocked_script_url_ = secondary_url;
  }

  return allow;
}

void PresearchContentSettingsAgentImpl::DidBlockFingerprinting(
    const base::string16& details) {
  Send(new PresearchViewHostMsg_FingerprintingBlocked(routing_id(), details));
}

bool PresearchContentSettingsAgentImpl::IsPresearchShieldsDown(
    const blink::WebFrame* frame,
    const GURL& secondary_url) {
  return !content_setting_rules_ ||
         ::content_settings::IsPresearchShieldsDown(
             frame, secondary_url, content_setting_rules_->presearch_shields_rules);
}

bool PresearchContentSettingsAgentImpl::AllowFingerprinting(
    bool enabled_per_settings) {
  if (!enabled_per_settings)
    return false;
  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();
  const GURL secondary_url(url::Origin(frame->GetSecurityOrigin()).GetURL());
  if (IsPresearchShieldsDown(frame, secondary_url)) {
    return true;
  }

  return GetPresearchFarblingLevel() != PresearchFarblingLevel::MAXIMUM;
}

PresearchFarblingLevel PresearchContentSettingsAgentImpl::GetPresearchFarblingLevel() {
  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();

  ContentSetting setting = CONTENT_SETTING_DEFAULT;
  if (content_setting_rules_) {
    if (IsPresearchShieldsDown(frame,
                           url::Origin(frame->GetSecurityOrigin()).GetURL())) {
      setting = CONTENT_SETTING_ALLOW;
    } else {
      setting = GetPresearchFPContentSettingFromRules(
          content_setting_rules_->fingerprinting_rules, GetOriginOrURL(frame));
    }
  }

  if (setting == CONTENT_SETTING_BLOCK) {
    VLOG(1) << "farbling level MAXIMUM";
    return PresearchFarblingLevel::MAXIMUM;
  } else if (setting == CONTENT_SETTING_ALLOW) {
    VLOG(1) << "farbling level OFF";
    return PresearchFarblingLevel::OFF;
  } else {
    VLOG(1) << "farbling level BALANCED";
    return PresearchFarblingLevel::BALANCED;
  }
}

bool PresearchContentSettingsAgentImpl::AllowAutoplay(bool play_requested) {
  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();
  auto origin = frame->GetSecurityOrigin();
  // default allow local files
  if (origin.IsNull() || origin.Protocol().Ascii() == url::kFileScheme) {
    VLOG(1) << "AllowAutoplay=true because no origin or file scheme";
    return true;
  }

  // respect user's site blocklist, if any
  if (content_setting_rules_) {
    ContentSetting setting =
        GetContentSettingFromRules(content_setting_rules_->autoplay_rules,
                                   frame, url::Origin(origin).GetURL());
    if (setting == CONTENT_SETTING_BLOCK) {
      VLOG(1) << "AllowAutoplay=false because rule=CONTENT_SETTING_BLOCK";
      if (play_requested)
        DidBlockContentType(ContentSettingsType::AUTOPLAY);
      return false;
    } else if (setting == CONTENT_SETTING_ALLOW) {
      VLOG(1) << "AllowAutoplay=true because rule=CONTENT_SETTING_ALLOW";
      return true;
    }
  }

  bool allow = ContentSettingsAgentImpl::AllowAutoplay(play_requested);
  if (allow) {
    VLOG(1) << "AllowAutoplay=true because "
               "ContentSettingsAgentImpl::AllowAutoplay says so";
  } else {
    if (play_requested)
      DidBlockContentType(ContentSettingsType::AUTOPLAY);
    VLOG(1) << "AllowAutoplay=false because "
               "ContentSettingsAgentImpl::AllowAutoplay says so";
  }
  return allow;
}

}  // namespace content_settings
