/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_STARTUP_DEFAULT_PRESEARCH_BROWSER_PROMPT_H_
#define PRESEARCH_BROWSER_UI_STARTUP_DEFAULT_PRESEARCH_BROWSER_PROMPT_H_

class PrefRegistrySimple;
class Profile;

void RegisterDefaultPresearchBrowserPromptPrefs(PrefRegistrySimple* registry);

void ShowDefaultPresearchBrowserPrompt(Profile* profile);

void ResetDefaultPresearchBrowserPrompt(Profile* profile);

#endif  // PRESEARCH_BROWSER_UI_STARTUP_DEFAULT_PRESEARCH_BROWSER_PROMPT_H_
