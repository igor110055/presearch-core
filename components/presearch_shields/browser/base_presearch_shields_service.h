/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_BASE_PRESEARCH_SHIELDS_SERVICE_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_BASE_PRESEARCH_SHIELDS_SERVICE_H_

#include <stdint.h>

#include <memory>
#include <string>
// TODO(presearch): <mutex> is an unapproved C++11 header
#include <mutex>  // NOLINT

#include "base/files/file_path.h"
#include "base/sequenced_task_runner.h"
#include "presearch/components/presearch_component_updater/browser/presearch_component.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"

using presearch_component_updater::PresearchComponent;

namespace presearch_shields {

// The presearch shields service in charge of checking presearch shields like ad-block,
// tracking protection, etc.
class BasePresearchShieldsService : public PresearchComponent {
 public:
  explicit BasePresearchShieldsService(PresearchComponent::Delegate* delegate);
  ~BasePresearchShieldsService() override;
  bool Start();
  bool IsInitialized() const;
  virtual void ShouldStartRequest(const GURL& url,
                                  blink::mojom::ResourceType resource_type,
                                  const std::string& tab_host,
                                  bool* did_match_rule,
                                  bool* did_match_exception,
                                  bool* did_match_important,
                                  std::string* mock_data_url);

 protected:
  virtual bool Init() = 0;

 private:
  void InitShields();

  bool initialized_;
  std::mutex initialized_mutex_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
};

}  // namespace presearch_shields

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_BASE_PRESEARCH_SHIELDS_SERVICE_H_
