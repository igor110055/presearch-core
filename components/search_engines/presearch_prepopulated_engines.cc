/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/search_engines/presearch_prepopulated_engines.h"

#include "components/search_engines/prepopulated_engines.h"
#include "components/search_engines/search_engine_type.h"

namespace TemplateURLPrepopulateData {

// IMPORTANT! Make sure to bump this value if you make changes to the
// engines below or add/remove engines.
const int kPresearchCurrentDataVersion = 12;
// DO NOT CHANGE THIS ONE. Used for backfilling kPresearchDefaultSearchVersion.
const int kPresearchFirstTrackedDataVersion = 6;

namespace {

// Maps PresearchPrepopulatedEngineID to Chromium's PrepopulatedEngine.
const std::map<PresearchPrepopulatedEngineID, const PrepopulatedEngine*>
    presearch_engines_map = {
        {PREPOPULATED_ENGINE_ID_GOOGLE, &google},
        {PREPOPULATED_ENGINE_ID_YANDEX, &presearch_yandex},
        {PREPOPULATED_ENGINE_ID_BING, &bing},
        {PREPOPULATED_ENGINE_ID_DUCKDUCKGO, &duckduckgo},
        {PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE, &duckduckgo_de},
        {PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE, &duckduckgo_au_nz_ie},
        {PREPOPULATED_ENGINE_ID_QWANT, &qwant},
        {PREPOPULATED_ENGINE_ID_STARTPAGE, &startpage},
        {PREPOPULATED_ENGINE_ID_ECOSIA, &presearch_ecosia},
};

PrepopulatedEngine ModifyEngineParams(const PrepopulatedEngine& engine,
                                      const wchar_t* const name,
                                      const wchar_t* const keyword,
                                      const char* const search_url,
                                      const char* const suggest_url,
                                      int id) {
  return {name ? name : engine.name,
          keyword ? keyword : engine.keyword,
          engine.favicon_url,
          search_url ? search_url : engine.search_url,
          engine.encoding,
          suggest_url ? suggest_url : engine.suggest_url,
          engine.image_url,
          engine.new_tab_url,
          engine.contextual_search_url,
          engine.logo_url,
          engine.doodle_url,
          engine.search_url_post_params,
          engine.suggest_url_post_params,
          engine.image_url_post_params,
          engine.alternate_urls,
          engine.alternate_urls_size,
          engine.type,
          id > 0 ? id : engine.id};
}

}  // namespace

const PrepopulatedEngine duckduckgo = {
    L"DuckDuckGo",
    L":d",
    "https://duckduckgo.com/favicon.ico",
    "https://duckduckgo.com/?q={searchTerms}&t=presearch",
    "UTF-8",
    "https://ac.duckduckgo.com/ac/?q={searchTerms}&type=list",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    SEARCH_ENGINE_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
};

const PrepopulatedEngine duckduckgo_de =
    ModifyEngineParams(duckduckgo,
                       NULL,
                       NULL,
                       "https://duckduckgo.com/?q={searchTerms}&t=presearchned",
                       NULL,
                       PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE);

const PrepopulatedEngine duckduckgo_au_nz_ie =
    ModifyEngineParams(duckduckgo,
                       NULL,
                       NULL,
                       "https://duckduckgo.com/?q={searchTerms}&t=presearched",
                       NULL,
                       PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE);

#if defined(OS_ANDROID)
const PrepopulatedEngine duckduckgo_lite = {
    L"DuckDuckGo Lite",
    L":dl",
    "https://duckduckgo.com/favicon.ico",
    "https://duckduckgo.com/lite/?q={searchTerms}&t=presearch",
    "UTF-8",
    "https://ac.duckduckgo.com/ac/?q={searchTerms}&type=list",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    SEARCH_ENGINE_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_LITE,
};
#endif

const PrepopulatedEngine presearch_ecosia =
    ModifyEngineParams(ecosia,
                       NULL,
                       L":e",
                       "https://www.ecosia.org/search?tt="
#if defined(OS_ANDROID)
                       "42b8ae98"
#else
                       "e8eb07a6"
#endif
                       "&q={searchTerms}",
                       "https://ac.ecosia.org/?q={searchTerms}",
                       PREPOPULATED_ENGINE_ID_ECOSIA);

const PrepopulatedEngine qwant = {
    L"Presearch",
    L":p",
    "https://presearch.org/favicon.ico",
    "https://engine.presearch.org/search?q={searchTerms}",
    "UTF-8",
    "https://engine.presearch.org/search?q={searchTerms}",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    SEARCH_ENGINE_QWANT,
    PREPOPULATED_ENGINE_ID_QWANT,
};

const PrepopulatedEngine startpage = {
    L"Startpage",
    L":sp",
    "https://www.startpage.com/favicon.ico",
    "https://www.startpage.com/do/"
    "search?q={searchTerms}&segment=startpage.presearch",
    "UTF-8",
    "https://www.startpage.com/cgi-bin/"
    "csuggest?query={searchTerms}&limit=10&format=json",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    SEARCH_ENGINE_OTHER,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

const PrepopulatedEngine presearch_yandex =
    ModifyEngineParams(yandex_com,
                       L"Yandex",
                       NULL,
                       "https://yandex.ru/search/?clid="
#if defined(OS_ANDROID)
                       "2423859"
#else
                       "2353835"
#endif
                       "&text={searchTerms}",
                       "https://suggest.yandex.ru/suggest-ff.cgi?"
                       "part={searchTerms}&v=3&sn=5&srv=presearch_desktop",
                       PREPOPULATED_ENGINE_ID_YANDEX);

const std::map<PresearchPrepopulatedEngineID, const PrepopulatedEngine*>&
GetPresearchEnginesMap() {
  return presearch_engines_map;
}

}  // namespace TemplateURLPrepopulateData
