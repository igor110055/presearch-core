/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_TOR_PRESEARCH_TOR_CLIENT_UPDATER_H_
#define PRESEARCH_COMPONENTS_TOR_PRESEARCH_TOR_CLIENT_UPDATER_H_

#include <memory>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/sequenced_task_runner.h"
#include "presearch/components/presearch_component_updater/browser/presearch_component.h"

class PresearchTorClientUpdaterTest;
class PrefService;

using presearch_component_updater::PresearchComponent;

namespace tor {

#if defined(OS_WIN)
extern const char kTorClientComponentName[];
extern const char kTorClientComponentId[];
#elif defined(OS_MAC)
extern const char kTorClientComponentName[];
extern const char kTorClientComponentId[];
extern const char kTorClientComponentBase64PublicKey[];
#elif defined(OS_LINUX)
extern const char kTorClientComponentName[];
extern const char kTorClientComponentId[];
extern const char kTorClientComponentBase64PublicKey[];
#endif

class PresearchTorClientUpdater : public PresearchComponent {
 public:
  class Observer : public base::CheckedObserver {
   public:
    virtual void OnExecutableReady(const base::FilePath& path) = 0;

   protected:
    ~Observer() override = default;
  };

  PresearchTorClientUpdater(PresearchComponent::Delegate* component_delegate,
                        PrefService* local_state,
                        const base::FilePath& user_data_dir);
  ~PresearchTorClientUpdater() override;

  void Register();
  void Unregister();
  void Cleanup();
  base::FilePath GetExecutablePath() const;
  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner() {
    return task_runner_;
  }

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 protected:
  void OnComponentReady(const std::string& component_id,
      const base::FilePath& install_dir,
      const std::string& manifest) override;
  bool IsTorDisabled();

 private:
  friend class ::PresearchTorClientUpdaterTest;

  static std::string g_tor_client_component_name_;
  static std::string g_tor_client_component_id_;
  static std::string g_tor_client_component_base64_public_key_;
  static void SetComponentIdAndBase64PublicKeyForTest(
      const std::string& component_id,
      const std::string& component_base64_public_key);
  void SetExecutablePath(const base::FilePath& path);

  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  bool registered_;
  base::FilePath executable_path_;
  base::ObserverList<Observer> observers_;
  PrefService* local_state_;
  base::FilePath user_data_dir_;

  base::WeakPtrFactory<PresearchTorClientUpdater> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PresearchTorClientUpdater);
};

}  // namespace tor

#endif  // PRESEARCH_COMPONENTS_TOR_PRESEARCH_TOR_CLIENT_UPDATER_H_
