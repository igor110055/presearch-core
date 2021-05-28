// Copyright (c) 2019 The Presearch Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch/browser/ui/webui/navigation_bar_data_provider.h"

namespace {

void PresearchCustomizeDownloadsDataSource(content::WebUIDataSource* source) {
  NavigationBarDataProvider::Initialize(source);
}

}  // namespace

#include "../../../../../../../chrome/browser/ui/webui/downloads/downloads_ui.cc"

