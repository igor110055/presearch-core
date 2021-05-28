/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/page_info/page_info_ui.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/vector_icons/vector_icons.h"

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/components/ipfs/ipfs_utils.h"
#include "components/grit/presearch_components_strings.h"
#endif  // BUILDFLAG(IPFS_ENABLED)

#define PRESEARCH_PAGE_INFO_UI_GET_PERMISSION_ICON \
  case ContentSettingsType::AUTOPLAY:          \
    icon = &kAutoplayStatusIcon;               \
    break;

#if BUILDFLAG(IPFS_ENABLED)
#define GetSecurityDescription GetSecurityDescription_ChromiumImpl
#endif  // BUILDFLAG(IPFS_ENABLED)

#include "../../../../components/page_info/page_info_ui.cc"

#if BUILDFLAG(IPFS_ENABLED)
#undef GetSecurityDescription

std::unique_ptr<PageInfoUI::SecurityDescription>
PageInfoUI::GetSecurityDescription(const IdentityInfo& identity_info) const {
  if (!ipfs::IsIPFSScheme(GURL(identity_info.site_identity)))
    return GetSecurityDescription_ChromiumImpl(identity_info);
  return CreateSecurityDescription(
      SecuritySummaryColor::GREEN, IDS_PAGE_INFO_IPFS_BUBBLE_TITTLE,
      IDS_PAGE_INFO_IPFS_BUBBLE_TEXT, SecurityDescriptionType::CONNECTION);
}
#endif  // BUILDFLAG(IPFS_ENABLED)

#undef PRESEARCH_PAGE_INFO_UI_GET_PERMISSION_ICON
