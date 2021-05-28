/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_INCLUDE_BAT_ADS_MOJOM_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_INCLUDE_BAT_ADS_MOJOM_H_

#include "bat/ads/public/interfaces/ads.mojom.h"
#include "bat/ads/public/interfaces/ads_database.mojom.h"

namespace ads {

using Environment = mojom::PresearchAdsEnvironment;

using SysInfo = mojom::PresearchAdsSysInfo;
using SysInfoPtr = mojom::PresearchAdsSysInfoPtr;

using BuildChannel = mojom::PresearchAdsBuildChannel;
using BuildChannelPtr = mojom::PresearchAdsBuildChannelPtr;

using AdNotificationEventType = mojom::PresearchAdsAdNotificationEventType;
using NewTabPageAdEventType = mojom::PresearchAdsNewTabPageAdEventType;
using PromotedContentAdEventType = mojom::PresearchAdsPromotedContentAdEventType;

using UrlRequest = mojom::PresearchAdsUrlRequest;
using UrlRequestPtr = mojom::PresearchAdsUrlRequestPtr;
using UrlRequestMethod = mojom::PresearchAdsUrlRequestMethod;

using P2AEventType = mojom::PresearchAdsP2AEventType;

using UrlResponse = mojom::PresearchAdsUrlResponse;
using UrlResponsePtr = mojom::PresearchAdsUrlResponsePtr;

using DBCommand = ads_database::mojom::DBCommand;
using DBCommandPtr = ads_database::mojom::DBCommandPtr;
using DBCommandBinding = ads_database::mojom::DBCommandBinding;
using DBCommandBindingPtr = ads_database::mojom::DBCommandBindingPtr;
using DBCommandResult = ads_database::mojom::DBCommandResult;
using DBCommandResultPtr = ads_database::mojom::DBCommandResultPtr;
using DBCommandResponse = ads_database::mojom::DBCommandResponse;
using DBCommandResponsePtr = ads_database::mojom::DBCommandResponsePtr;
using DBRecord = ads_database::mojom::DBRecord;
using DBRecordPtr = ads_database::mojom::DBRecordPtr;
using DBTransaction = ads_database::mojom::DBTransaction;
using DBTransactionPtr = ads_database::mojom::DBTransactionPtr;
using DBValue = ads_database::mojom::DBValue;
using DBValuePtr = ads_database::mojom::DBValuePtr;

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_INCLUDE_BAT_ADS_MOJOM_H_
