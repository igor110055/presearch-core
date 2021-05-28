/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_RENDERER_CONTENT_SETTINGS_AGENT_IMPL_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_RENDERER_CONTENT_SETTINGS_AGENT_IMPL_H_

#define PRESEARCH_CONTENT_SETTINGS_AGENT_IMPL_H_ \
friend class PresearchContentSettingsAgentImpl;

#define IsAllowlistedForContentSettings                                     \
  IsAllowlistedForContentSettings(const blink::WebSecurityOrigin& origin,   \
                                  const blink::WebURL& document_url) const; \
  bool IsAllowlistedForContentSettings

#include "../../../../../components/content_settings/renderer/content_settings_agent_impl.h"
#undef IsAllowlistedForContentSettings

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_RENDERER_CONTENT_SETTINGS_AGENT_IMPL_H_
