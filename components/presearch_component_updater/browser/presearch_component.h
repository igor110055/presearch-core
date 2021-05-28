/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_COMPONENT_UPDATER_BROWSER_PRESEARCH_COMPONENT_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_COMPONENT_UPDATER_BROWSER_PRESEARCH_COMPONENT_H_

#include <string>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner.h"
#include "components/component_updater/component_updater_service.h"

namespace presearch_component_updater {

class PresearchComponent {
 public:
  using ReadyCallback = base::RepeatingCallback<void(const base::FilePath&,
                                                const std::string& manifest)>;
  using ComponentObserver = update_client::UpdateClient::Observer;

  class Delegate {
   public:
    virtual ~Delegate() = default;
    virtual void Register(const std::string& component_name,
                          const std::string& component_base64_public_key,
                          base::OnceClosure registered_callback,
                          ReadyCallback ready_callback) = 0;
    virtual bool Unregister(const std::string& component_id) = 0;
    virtual void OnDemandUpdate(const std::string& component_id) = 0;
    // An observer should not be added more than once.
    // The caller retains the ownership of the observer object.
    virtual void AddObserver(ComponentObserver* observer) = 0;

    // It is safe for an observer to be removed while
    // the observers are being notified.
    virtual void RemoveObserver(ComponentObserver* observer) = 0;
    virtual scoped_refptr<base::SequencedTaskRunner> GetTaskRunner() = 0;
  };

  explicit PresearchComponent(Delegate* delegate);
  virtual ~PresearchComponent();
  void Register(const std::string& component_name,
                const std::string& component_id,
                const std::string& component_base64_public_key);

  bool Unregister();
  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner();

  // Adds an observer for ComponentObserver. An observer should not be added
  // more than once. The caller retains the ownership of the observer object.
  void AddObserver(ComponentObserver* observer);

  // Removes an observer. It is safe for an observer to be removed while
  // the observers are being notified.
  void RemoveObserver(ComponentObserver* observer);

 protected:
  virtual void OnComponentReady(const std::string& component_id,
                                const base::FilePath& install_dir,
                                const std::string& manifest);

 private:
  static void OnComponentRegistered(Delegate* delegate,
                                    const std::string& component_id);
  void OnComponentReadyInternal(const std::string& component_id,
                                const base::FilePath& install_dir,
                                const std::string& manifest);

  std::string component_name_;
  std::string component_id_;
  std::string component_base64_public_key_;
  Delegate* delegate_;  // NOT OWNED
  base::WeakPtrFactory<PresearchComponent> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PresearchComponent);
};

}  // namespace presearch_component_updater

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_COMPONENT_UPDATER_BROWSER_PRESEARCH_COMPONENT_H_
