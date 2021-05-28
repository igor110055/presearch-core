/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/component_updater/presearch_component_updater_delegate.h"

#include <utility>

#include "base/sequenced_task_runner.h"
#include "base/task/post_task.h"
#include "presearch/browser/component_updater/presearch_component_installer.h"
#include "presearch/components/presearch_component_updater/browser/presearch_on_demand_updater.h"
#include "chrome/browser/browser_process.h"
#include "components/component_updater/component_updater_service.h"

using presearch_component_updater::PresearchComponent;
using presearch_component_updater::PresearchOnDemandUpdater;

namespace presearch {

PresearchComponentUpdaterDelegate::PresearchComponentUpdaterDelegate()
    : task_runner_(base::CreateSequencedTaskRunner(
          {base::ThreadPool(), base::MayBlock(),
           base::TaskPriority::USER_VISIBLE,
           base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN})) {}

PresearchComponentUpdaterDelegate::~PresearchComponentUpdaterDelegate() {}

void PresearchComponentUpdaterDelegate::Register(
    const std::string& component_name,
    const std::string& component_base64_public_key,
    base::OnceClosure registered_callback,
    PresearchComponent::ReadyCallback ready_callback) {
  presearch::RegisterComponent(g_browser_process->component_updater(),
                           component_name,
                           component_base64_public_key,
                           std::move(registered_callback),
                           std::move(ready_callback));
}

bool PresearchComponentUpdaterDelegate::Unregister(
    const std::string& component_id) {
  return g_browser_process->component_updater()->UnregisterComponent(
      component_id);
}

void PresearchComponentUpdaterDelegate::OnDemandUpdate(
    const std::string& component_id) {
  PresearchOnDemandUpdater::GetInstance()->OnDemandUpdate(component_id);
}

void PresearchComponentUpdaterDelegate::AddObserver(ComponentObserver* observer) {
  g_browser_process->component_updater()->AddObserver(observer);
}

void PresearchComponentUpdaterDelegate::RemoveObserver(
    ComponentObserver* observer) {
  g_browser_process->component_updater()->RemoveObserver(observer);
}

scoped_refptr<base::SequencedTaskRunner>
PresearchComponentUpdaterDelegate::GetTaskRunner() {
  return task_runner_;
}

}  // namespace presearch
