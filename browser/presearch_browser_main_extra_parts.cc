/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_browser_main_extra_parts.h"

#include "base/metrics/histogram_macros.h"
#include "presearch/browser/presearch_browser_process_impl.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_p3a.h"
#include "presearch/components/p3a/presearch_p3a_service.h"
#include "presearch/components/p3a/buildflags.h"
#include "components/metrics/metrics_pref_names.h"
#include "components/prefs/pref_service.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

#if !defined(OS_ANDROID)
#include "presearch/browser/importer/presearch_importer_p3a.h"
#include "presearch/browser/p3a/p3a_core_metrics.h"
#include "presearch/browser/ui/webui/new_tab_page/presearch_new_tab_message_handler.h"
#include "chrome/browser/first_run/first_run.h"
#endif  // !defined(OS_ANDROID)

namespace {

// Records default values for some histograms because we want these stats to be
// uploaded anyways. Corresponding components will write new values according
// to their usage scenarios.
//
// For profile specific values, see browser/profiles/profile_util.cc
void RecordInitialP3AValues() {
#if !defined(OS_ANDROID)
  if (first_run::IsChromeFirstRun()) {
    RecordImporterP3A(importer::ImporterType::TYPE_UNKNOWN);
  }

  PresearchNewTabMessageHandler::RecordInitialP3AValues(
      g_browser_process->local_state());
#endif  // !defined(OS_ANDROID)

  presearch_shields::MaybeRecordShieldsUsageP3A(presearch_shields::kNeverClicked,
                                            g_browser_process->local_state());

  // Record crash reporting status stats.
  const bool crash_reports_enabled = g_browser_process->local_state()->
      GetBoolean(metrics::prefs::kMetricsReportingEnabled);
  UMA_HISTOGRAM_BOOLEAN("Presearch.Core.CrashReportsEnabled",
                        crash_reports_enabled);
}

}  // namespace

PresearchBrowserMainExtraParts::PresearchBrowserMainExtraParts() {
}

PresearchBrowserMainExtraParts::~PresearchBrowserMainExtraParts() {
}

void PresearchBrowserMainExtraParts::PostBrowserStart() {
  g_presearch_browser_process->StartPresearchServices();
}

void PresearchBrowserMainExtraParts::PreMainMessageLoopRun() {
  // Disabled on mobile platforms, see for instance issues/6176
#if BUILDFLAG(PRESEARCH_P3A_ENABLED)
  // TODO(iefremov): Maybe find a better place for this initialization.
  g_presearch_browser_process->presearch_p3a_service()->Init(
      g_browser_process->shared_url_loader_factory());
#endif  // BUILDFLAG(PRESEARCH_P3A_ENABLED)

  RecordInitialP3AValues();

  // The code below is not supported on android.
#if !defined(OS_ANDROID)
  presearch::PresearchWindowTracker::CreateInstance(g_browser_process->local_state());
  presearch::PresearchUptimeTracker::CreateInstance(g_browser_process->local_state());
#endif  // !defined(OS_ANDROID)
}
