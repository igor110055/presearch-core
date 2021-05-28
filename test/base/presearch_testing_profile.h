/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_TEST_BASE_PRESEARCH_TESTING_PROFILE_H_
#define PRESEARCH_TEST_BASE_PRESEARCH_TESTING_PROFILE_H_

#include "chrome/test/base/testing_profile.h"

class PresearchTestingProfile : public TestingProfile {
 public:
  PresearchTestingProfile();
  PresearchTestingProfile(const base::FilePath& path, Delegate* delegate);
  ~PresearchTestingProfile() override = default;
};

#endif  // PRESEARCH_TEST_BASE_PRESEARCH_TESTING_PROFILE_H_
