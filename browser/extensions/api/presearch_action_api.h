// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_ACTION_API_H_
#define PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_ACTION_API_H_

#include <memory>
#include <string>

#include "components/keyed_service/core/keyed_service.h"
#include "extensions/browser/extension_function.h"
#include "extensions/common/extension.h"

class Browser;

namespace extensions {
class PresearchActionAPI : public KeyedService {
 public:
  class Observer {
   public:
    Observer();
    virtual void OnPresearchActionShouldTrigger(
      const std::string& extension_id,
      std::unique_ptr<std::string> ui_relative_path) = 0;

   protected:
    virtual ~Observer();
  };

  static PresearchActionAPI* Get(Browser* context);
  static bool ShowActionUI(
        ExtensionFunction* extension_function,
        const std::string& extension_id,
        std::unique_ptr<int> window_id,
        std::unique_ptr<std::string> ui_relative_path,
        std::string* error);
  static bool ShowActionUI(
        Browser* browser,
        const std::string& extension_id,
        std::unique_ptr<std::string> ui_relative_path,
        std::string* error);
  PresearchActionAPI();
  ~PresearchActionAPI() override;

  // Add or remove observers.
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 protected:
  bool NotifyObservers(const std::string& extension_id,
      std::unique_ptr<std::string> ui_relative_path_param);

 private:
  base::ObserverList<Observer>::Unchecked observers_;

  DISALLOW_COPY_AND_ASSIGN(PresearchActionAPI);
};
}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_ACTION_API_H_
