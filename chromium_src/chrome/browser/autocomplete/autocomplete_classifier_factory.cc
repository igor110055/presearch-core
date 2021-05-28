/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/autocomplete/presearch_autocomplete_scheme_classifier.h"
#include "components/omnibox/browser/autocomplete_classifier.h"
#include "components/omnibox/browser/autocomplete_controller.h"

#define ChromeAutocompleteSchemeClassifier PresearchAutocompleteSchemeClassifier
#include "../../../../../chrome/browser/autocomplete/autocomplete_classifier_factory.cc"
#undef ChromeAutocompleteSchemeClassifier
