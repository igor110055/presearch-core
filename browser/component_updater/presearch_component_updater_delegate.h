/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_COMPONENT_UPDATER_PRESEARCH_COMPONENT_UPDATER_DELEGATE_H_
#define PRESEARCH_BROWSER_COMPONENT_UPDATER_PRESEARCH_COMPONENT_UPDATER_DELEGATE_H_

#include <string>

#include "base/macros.h"
#include "presearch/components/presearch_component_updater/browser/presearch_component.h"
#include "components/component_updater/component_updater_service.h"

using presearch_component_updater::PresearchComponent;

namespace base {
class SequencedTaskRunner;
}

namespace presearch {

class PresearchComponentUpdaterDelegate : public PresearchComponent::Delegate {
 public:
  PresearchComponentUpdaterDelegate();
  ~PresearchComponentUpdaterDelegate() override;

  using ComponentObserver = update_client::UpdateClient::Observer;
  // presearch_component_updater::PresearchComponent::Delegate implementation
  void Register(const std::string& component_name,
                const std::string& component_base64_public_key,
                base::OnceClosure registered_callback,
                PresearchComponent::ReadyCallback ready_callback) override;
  bool Unregister(const std::string& component_id) override;
  void OnDemandUpdate(const std::string& component_id) override;

  void AddObserver(ComponentObserver* observer) override;
  void RemoveObserver(ComponentObserver* observer) override;

  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner() override;

 private:
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  DISALLOW_COPY_AND_ASSIGN(PresearchComponentUpdaterDelegate);
};

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_COMPONENT_UPDATER_PRESEARCH_COMPONENT_UPDATER_DELEGATE_H_
