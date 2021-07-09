/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/presearch_browser_content_setting_bubble_model_delegate.h"

#include "presearch/common/url_constants.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_tabstrip.h"

const char kPresearchCommunitySupportUrl[] = "https://support.presearch.org/";

PresearchBrowserContentSettingBubbleModelDelegate::
PresearchBrowserContentSettingBubbleModelDelegate(Browser* browser) :
    BrowserContentSettingBubbleModelDelegate(browser),
    browser_(browser) {
}

PresearchBrowserContentSettingBubbleModelDelegate::
~PresearchBrowserContentSettingBubbleModelDelegate() {
}

void
PresearchBrowserContentSettingBubbleModelDelegate::ShowWidevineLearnMorePage() {
  GURL learn_more_url = GURL(kWidevineTOS);
  chrome::AddSelectedTabWithURL(browser_, learn_more_url,
                                ui::PAGE_TRANSITION_LINK);
}

void PresearchBrowserContentSettingBubbleModelDelegate::ShowLearnMorePage(
    ContentSettingsType type) {
  // TODO: Use specific support pages for each content setting type
  GURL learn_more_url(kPresearchCommunitySupportUrl);
  chrome::AddSelectedTabWithURL(browser_, learn_more_url,
                                ui::PAGE_TRANSITION_LINK);
}
