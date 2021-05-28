/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_TAB_HELPERS_H_
#define PRESEARCH_BROWSER_PRESEARCH_TAB_HELPERS_H_

namespace content {
class WebContents;
}

namespace presearch {

void AttachTabHelpers(content::WebContents* web_contents);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_PRESEARCH_TAB_HELPERS_H_

