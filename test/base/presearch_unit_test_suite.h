/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_TEST_BASE_PRESEARCH_UNIT_TEST_SUITE_H_
#define PRESEARCH_TEST_BASE_PRESEARCH_UNIT_TEST_SUITE_H_

#include <memory>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/test/test_discardable_memory_allocator.h"
#include "chrome/test/base/chrome_unit_test_suite.h"

class PresearchUnitTestSuite : public ChromeUnitTestSuite {
 public:
  PresearchUnitTestSuite(int argc, char** argv);

 protected:
  // base::TestSuite overrides:
  void Initialize() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchUnitTestSuite);
};

#endif  // PRESEARCH_TEST_BASE_PRESEARCH_UNIT_TEST_SUITE_H_
