/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_

#define PRESEARCH_TRANSLATE_BUBBLE_VIEW_H_ \
  private: \
    friend class PresearchTranslateBubbleView; \
    friend class PresearchTranslateBubbleViewTest; \
    FRIEND_TEST_ALL_PREFIXES(PresearchTranslateBubbleViewTest, \
                             PresearchBeforeTranslateView); \
    FRIEND_TEST_ALL_PREFIXES(PresearchTranslateBubbleViewTest, TranslateButton); \
    FRIEND_TEST_ALL_PREFIXES(PresearchTranslateBubbleViewTest, CancelButton); \
  public:
// define PRESEARCH_TRANSLATE_BUBBLE_VIEW_H_

class PresearchTranslateBubbleView;
#include "../../../../../../../chrome/browser/ui/views/translate/translate_bubble_view.h"

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_
