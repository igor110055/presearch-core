/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define PRESEARCH_REALTIMEANALYSER_CONVERTFLOATTODB                       \
  if (audio_farbling_callback_) {                                     \
    destination[i] = audio_farbling_callback_.Run(destination[i], i); \
  }

#define PRESEARCH_REALTIMEANALYSER_CONVERTTOBYTEDATA                  \
  if (audio_farbling_callback_) {                                 \
    scaled_value = audio_farbling_callback_.Run(scaled_value, i); \
  }

#define PRESEARCH_REALTIMEANALYSER_GETFLOATTIMEDOMAINDATA        \
  if (audio_farbling_callback_) {                            \
    destination[i] = audio_farbling_callback_.Run(value, i); \
  }

#define PRESEARCH_REALTIMEANALYSER_GETBYTETIMEDOMAINDATA \
  if (audio_farbling_callback_) {                    \
    value = audio_farbling_callback_.Run(value, i);  \
  }

#include "../../../../../../../third_party/blink/renderer/modules/webaudio/realtime_analyser.cc"

#undef PRESEARCH_REALTIMEANALYSER_CONVERTFLOATTODB
#undef PRESEARCH_REALTIMEANALYSER_CONVERTTOBYTEDATA
#undef PRESEARCH_REALTIMEANALYSER_GETFLOATTIMEDOMAINDATA
#undef PRESEARCH_REALTIMEANALYSER_GETBYTETIMEDOMAINDATA
