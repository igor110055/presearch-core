/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/presearch_wallet_util.h"

#include <memory>

#include "base/environment.h"

namespace extensions {

bool HasInfuraProjectID() {
  std::string project_id = GetInfuraProjectID();

  if (!project_id.empty()) {
    return true;
  }

  std::unique_ptr<base::Environment> env(base::Environment::Create());
  bool has_project_id = env->HasVar("PRESEARCH_INFURA_PROJECT_ID");

  return has_project_id;
}

std::string GetInfuraProjectID() {
  std::string project_id(PRESEARCH_INFURA_PROJECT_ID);
  std::unique_ptr<base::Environment> env(base::Environment::Create());

  if (env->HasVar("PRESEARCH_INFURA_PROJECT_ID")) {
    env->GetVar("PRESEARCH_INFURA_PROJECT_ID", &project_id);
  }

  return project_id;
}

std::string GetPresearchKey() {
  std::string presearch_key(PRESEARCH_SERVICES_KEY);
  std::unique_ptr<base::Environment> env(base::Environment::Create());

  if (env->HasVar("PRESEARCH_SERVICES_KEY")) {
    env->GetVar("PRESEARCH_SERVICES_KEY", &presearch_key);
  }

  return presearch_key;
}

}  // namespace extensions
