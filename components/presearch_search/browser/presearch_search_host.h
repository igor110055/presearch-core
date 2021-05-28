/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SEARCH_BROWSER_PRESEARCH_SEARCH_HOST_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SEARCH_BROWSER_PRESEARCH_SEARCH_HOST_H_

#include <list>
#include <map>
#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "presearch/components/presearch_search/common/presearch_search.mojom.h"
#include "url/gurl.h"

namespace network {
class SharedURLLoaderFactory;
class SimpleURLLoader;
}  // namespace network

namespace presearch_search {

class PresearchSearchHost final : public presearch_search::mojom::PresearchSearchFallback {
 public:
  PresearchSearchHost(const PresearchSearchHost&) = delete;
  PresearchSearchHost& operator=(const PresearchSearchHost&) = delete;
  explicit PresearchSearchHost(
      scoped_refptr<network::SharedURLLoaderFactory> factory);
  ~PresearchSearchHost() override;

  void FetchBackupResults(const std::string& query_string,
                          const std::string& lang,
                          const std::string& country,
                          const std::string& geo,
                          bool filter_explicit_results,
                          FetchBackupResultsCallback callback) override;

  static GURL GetBackupResultURL(const GURL& baseURL,
                                 const std::string& query,
                                 const std::string& lang,
                                 const std::string& country,
                                 const std::string& geo,
                                 bool filter_explicit_results);
  static void SetBackupProviderForTest(const GURL&);

 private:
  using SimpleURLLoaderList =
      std::list<std::unique_ptr<network::SimpleURLLoader>>;
  using URLRequestCallback =
      base::OnceCallback<void(const int,
                              const std::string&,
                              const std::map<std::string, std::string>&)>;

  void OnURLLoaderComplete(SimpleURLLoaderList::iterator iter,
                           PresearchSearchHost::FetchBackupResultsCallback callback,
                           const std::unique_ptr<std::string> response_body);
  SimpleURLLoaderList url_loaders_;
  scoped_refptr<network::SharedURLLoaderFactory> shared_url_loader_factory_;
  base::WeakPtrFactory<PresearchSearchHost> weak_factory_;
};

}  // namespace presearch_search

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SEARCH_BROWSER_PRESEARCH_SEARCH_HOST_H_
