/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define InitializePageLoadMetricsForWebContents \
  InitializePageLoadMetricsForWebContents_Chromium
#include "../../../../../chrome/browser/page_load_metrics/page_load_metrics_initialize.cc"
#undef InitializePageLoadMetricsForWebContents

#include "presearch/components/presearch_perf_predictor/browser/buildflags.h"

#if BUILDFLAG(ENABLE_PRESEARCH_PERF_PREDICTOR)
#include "presearch/components/presearch_perf_predictor/browser/perf_predictor_page_metrics_observer.h"
#endif

namespace chrome {

namespace {

class PresearchPageLoadMetricsEmbedder : public chrome::PageLoadMetricsEmbedder {
 public:
  explicit PresearchPageLoadMetricsEmbedder(content::WebContents* web_contents);
  ~PresearchPageLoadMetricsEmbedder() override;

 protected:
  // page_load_metrics::PageLoadMetricsEmbedderBase:
  void RegisterEmbedderObservers(
      ::page_load_metrics::PageLoadTracker* tracker) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchPageLoadMetricsEmbedder);
};

PresearchPageLoadMetricsEmbedder::PresearchPageLoadMetricsEmbedder(
    content::WebContents* web_contents)
    : chrome::PageLoadMetricsEmbedder(web_contents) {}

PresearchPageLoadMetricsEmbedder::~PresearchPageLoadMetricsEmbedder() = default;

void PresearchPageLoadMetricsEmbedder::RegisterEmbedderObservers(
    page_load_metrics::PageLoadTracker* tracker) {
  PageLoadMetricsEmbedder::RegisterEmbedderObservers(tracker);

#if BUILDFLAG(ENABLE_PRESEARCH_PERF_PREDICTOR)
  tracker->AddObserver(
      std::make_unique<
          presearch_perf_predictor::PerfPredictorPageMetricsObserver>());
#endif
}

}  // namespace

void InitializePageLoadMetricsForWebContents(
    content::WebContents* web_contents) {
  // TODO(bug https://github.com/presearch/presearch-browser/issues/7784)
  // change
  // android_webview/browser/page_load_metrics/page_load_metrics_initialize.cc
  // as well to register Page Load Metrics Observers
  page_load_metrics::MetricsWebContentsObserver::CreateForWebContents(
      web_contents,
      std::make_unique<PresearchPageLoadMetricsEmbedder>(web_contents));
}

}  // namespace chrome
