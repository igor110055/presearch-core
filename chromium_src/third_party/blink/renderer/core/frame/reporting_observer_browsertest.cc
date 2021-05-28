/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "presearch/browser/presearch_content_browser_client.h"
#include "presearch/common/presearch_paths.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/chrome_content_client.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"

namespace {

const char kReportingObserver[] = "/reporting_observer.html";

}  // namespace

class ReportingObserverTest : public InProcessBrowserTest {
 public:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();
    content::SetupCrossSiteRedirector(embedded_test_server());

    presearch::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(presearch::DIR_TEST_DATA, &test_data_dir);
    embedded_test_server()->ServeFilesFromDirectory(test_data_dir);

    ASSERT_TRUE(embedded_test_server()->Start());
  }
};

IN_PROC_BROWSER_TEST_F(ReportingObserverTest, IsDisabled) {
  GURL url = embedded_test_server()->GetURL(kReportingObserver);
  ui_test_utils::NavigateToURL(browser(), url);
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();

  EXPECT_EQ(true, EvalJs(contents, "isReportingObserverDisabled();"));
}
