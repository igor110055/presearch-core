// Copyright (c) 2019 The Presearch Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "ui/base/webui/web_ui_util.h"

#include "presearch/ui/webui/resources/grit/presearch_webui_resources.h"
#include "ui/resources/grit/webui_generated_resources.h"

// Replace text_defaults_md.css with presearch's text_defaults_md.css
// which is defined in presearch_webui_resources.grd.
#undef IDR_WEBUI_CSS_TEXT_DEFAULTS_MD_CSS
#define IDR_WEBUI_CSS_TEXT_DEFAULTS_MD_CSS IDR_PRESEARCH_WEBUI_CSS_TEXT_DEFAULTS_MD

#include "../../../../../ui/base/webui/web_ui_util.cc"
#undef IDR_WEBUI_CSS_TEXT_DEFAULTS_MD_CSS
