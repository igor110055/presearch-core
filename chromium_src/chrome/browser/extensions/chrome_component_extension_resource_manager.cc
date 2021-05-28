/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/extensions/chrome_component_extension_resource_manager.h"

#include "presearch/components/presearch_extension/grit/presearch_extension_generated_map.h"
#include "presearch/components/presearch_extension/grit/presearch_extension_resources_map.h"
#include "presearch/components/presearch_rewards/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_webtorrent/browser/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_PRESEARCH_WEBTORRENT)
#include "presearch/components/presearch_webtorrent/grit/presearch_webtorrent_generated_map.h"
#include "presearch/components/presearch_webtorrent/grit/presearch_webtorrent_resources_map.h"
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
#include "presearch/components/presearch_rewards/resources/extension/grit/presearch_rewards_extension_resources_map.h"
#include "presearch/components/presearch_rewards/resources/extension/grit/presearch_rewards_panel_generated_map.h"
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
#define PRESEARCH_REWARDS_EXTENSION_RESOURCES                           \
  AddComponentResourceEntries(kPresearchRewardsExtensionResources,      \
                              kPresearchRewardsExtensionResourcesSize); \
  AddComponentResourceEntries(kPresearchRewardsPanelGenerated,          \
                              kPresearchRewardsPanelGeneratedSize);
#else
#define PRESEARCH_REWARDS_EXTENSION_RESOURCES
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_WEBTORRENT)
#define PRESEARCH_WEBTORRENT_RESOURCES                            \
  AddComponentResourceEntries(kPresearchWebtorrentResources,      \
                              kPresearchWebtorrentResourcesSize); \
  AddComponentResourceEntries(kPresearchWebtorrentGenerated,      \
                              kPresearchWebtorrentGeneratedSize);
#else
#define PRESEARCH_WEBTORRENT_RESOURCES
#endif

#define PRESEARCH_CHROME_COMPONENT_EXTENSION_RESOURCE_MANAGER_DATA_DATA  \
  AddComponentResourceEntries(kPresearchExtension, kPresearchExtensionSize); \
  AddComponentResourceEntries(kPresearchExtensionGenerated,              \
                              kPresearchExtensionGeneratedSize);         \
  PRESEARCH_REWARDS_EXTENSION_RESOURCES                                  \
  PRESEARCH_WEBTORRENT_RESOURCES

#include "../../../../../chrome/browser/extensions/chrome_component_extension_resource_manager.cc"
#undef PRESEARCH_CHROME_COMPONENT_EXTENSION_RESOURCE_MANAGER_DATA_DATA
