/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/search_engines/template_url_prepopulate_data.h"

#include <map>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/no_destructor.h"
#include "base/stl_util.h"
#include "base/strings/utf_string_conversions.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/search_engines/presearch_prepopulated_engines.h"
#include "components/country_codes/country_codes.h"

#define GetDataVersion GetDataVersion_ChromiumImpl
#if defined(OS_ANDROID)
#define GetLocalPrepopulatedEngines GetLocalPrepopulatedEngines_Unused
#endif
#define GetPrepopulatedDefaultSearch GetPrepopulatedDefaultSearch_Unused
#define GetPrepopulatedEngine GetPrepopulatedEngine_Unused
#define GetPrepopulatedEngines GetPrepopulatedEngines_Unused
#include "../../../../components/search_engines/template_url_prepopulate_data.cc"
#undef GetDataVersion
#if defined(OS_ANDROID)
#undef GetLocalPrepopulatedEngines
#endif
#undef GetPrepopulatedDefaultSearch
#undef GetPrepopulatedEngine
#undef GetPrepopulatedEngines

namespace TemplateURLPrepopulateData {

namespace {

// Default order in which engines will appear in the UI.
const std::vector<PresearchPrepopulatedEngineID> presearch_engines_default = {
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// Variations of the order / default options by country.
const std::vector<PresearchPrepopulatedEngineID> presearch_engines_with_ecosia = {
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_YANDEX,
    PREPOPULATED_ENGINE_ID_ECOSIA,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

const std::vector<PresearchPrepopulatedEngineID> presearch_engines_with_yandex = {
    PREPOPULATED_ENGINE_ID_YANDEX,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

const std::vector<PresearchPrepopulatedEngineID> presearch_engines_DE = {
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

const std::vector<PresearchPrepopulatedEngineID> presearch_engines_FR = {
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

const std::vector<PresearchPrepopulatedEngineID> presearch_engines_AU_IE = {
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

const std::vector<PresearchPrepopulatedEngineID> presearch_engines_NZ = {
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// A map to keep track of a full list of default engines for countries
// that don't use the default list.
const std::map<int, const std::vector<PresearchPrepopulatedEngineID>*>
    default_engines_by_country_id_map = {
        {country_codes::CountryCharsToCountryID('A', 'M'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('A', 'T'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('A', 'U'),
         &presearch_engines_AU_IE},
        {country_codes::CountryCharsToCountryID('A', 'Z'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('B', 'E'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('B', 'Y'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('C', 'A'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('C', 'H'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('D', 'E'), &presearch_engines_DE},
        {country_codes::CountryCharsToCountryID('D', 'K'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('E', 'S'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('F', 'I'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('F', 'R'), &presearch_engines_FR},
        {country_codes::CountryCharsToCountryID('G', 'B'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('G', 'R'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('H', 'U'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('I', 'E'),
         &presearch_engines_AU_IE},
        {country_codes::CountryCharsToCountryID('I', 'T'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('K', 'G'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('K', 'Z'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('L', 'U'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('M', 'D'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('N', 'L'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('N', 'O'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('N', 'Z'), &presearch_engines_NZ},
        {country_codes::CountryCharsToCountryID('P', 'T'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('R', 'U'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('S', 'E'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('T', 'J'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('T', 'M'),
         &presearch_engines_with_yandex},
        {country_codes::CountryCharsToCountryID('U', 'S'),
         &presearch_engines_with_ecosia},
        {country_codes::CountryCharsToCountryID('U', 'Z'),
         &presearch_engines_with_yandex}};

// A versioned map tracking the singular default search engine per-country.
PresearchPrepopulatedEngineID GetDefaultSearchEngine(int country_id, int version) {
  const PresearchPrepopulatedEngineID default_v6 =
    PREPOPULATED_ENGINE_ID_GOOGLE;
  static const base::NoDestructor<base::flat_map<int,
        PresearchPrepopulatedEngineID>>
      content_v6({
        {country_codes::CountryCharsToCountryID('A', 'U'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('F', 'R'),
         PREPOPULATED_ENGINE_ID_QWANT},
        {country_codes::CountryCharsToCountryID('D', 'E'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE},
        {country_codes::CountryCharsToCountryID('I', 'E'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('N', 'Z'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
      });
  static const base::NoDestructor<base::flat_map<int,
        PresearchPrepopulatedEngineID>>
      content_v8({
        {country_codes::CountryCharsToCountryID('A', 'M'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('A', 'Z'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('A', 'U'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('B', 'Y'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('D', 'E'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE},
        {country_codes::CountryCharsToCountryID('F', 'R'),
         PREPOPULATED_ENGINE_ID_QWANT},
        {country_codes::CountryCharsToCountryID('I', 'E'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('K', 'G'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('K', 'Z'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('M', 'D'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('N', 'Z'),
         PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('R', 'U'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('T', 'J'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('T', 'M'),
         PREPOPULATED_ENGINE_ID_YANDEX},
        {country_codes::CountryCharsToCountryID('U', 'Z'),
         PREPOPULATED_ENGINE_ID_YANDEX},
      });
  if (version > 7) {
    auto it = content_v8->find(country_id);
    if (it == content_v8->end()) {
        return default_v6;
    }
    return it->second;
  } else {
    auto it = content_v6->find(country_id);
    if (it == content_v6->end()) {
        return default_v6;
    }
    return it->second;
  }
}

// Builds a vector of PrepulatedEngine objects from the given array of
// |engine_ids|. Fills in the default engine index for the given |country_id|,
// if asked.
std::vector<const PrepopulatedEngine*> GetEnginesFromEngineIDs(
    const std::vector<PresearchPrepopulatedEngineID>& engine_ids,
    int country_id,
    PresearchPrepopulatedEngineID default_engine_id,
    size_t* default_search_provider_index = nullptr) {
  std::vector<const PrepopulatedEngine*> engines;
  const auto& presearch_engines_map =
      TemplateURLPrepopulateData::GetPresearchEnginesMap();
  for (size_t i = 0; i < engine_ids.size(); ++i) {
    const PrepopulatedEngine* engine = presearch_engines_map.at(engine_ids.at(i));
    DCHECK(engine);
    if (engine) {
      engines.push_back(engine);
      if (default_search_provider_index &&
          default_engine_id == engine_ids.at(i))
        *default_search_provider_index = i;
    }
  }
  return engines;
}

void UpdateTemplateURLDataKeyword(
    const std::unique_ptr<TemplateURLData>& t_urld) {
  DCHECK(t_urld.get());
  switch (t_urld->prepopulate_id) {
    case PREPOPULATED_ENGINE_ID_GOOGLE:
      t_urld->SetKeyword(base::ASCIIToUTF16(":g"));
      break;
    case PREPOPULATED_ENGINE_ID_BING:
      t_urld->SetKeyword(base::ASCIIToUTF16(":b"));
      break;
  }
}

// Uses presearch_engines_XX localized arrays of engine IDs instead of Chromium's
// localized arrays of PrepopulatedEngines to construct the vector of
// TemplateURLData. Also, fills in the default engine index for the given
// |country_id|.
std::vector<std::unique_ptr<TemplateURLData>>
GetPresearchPrepopulatedEnginesForCountryID(
    int country_id,
    size_t* default_search_provider_index = nullptr,
    int version = kPresearchCurrentDataVersion) {
  std::vector<PresearchPrepopulatedEngineID> presearch_engine_ids =
      presearch_engines_default;

  // Check for a per-country override of this list
  const auto& it_country = default_engines_by_country_id_map.find(country_id);
  if (it_country != default_engines_by_country_id_map.end()) {
    presearch_engine_ids = *it_country->second;
  }
  DCHECK_GT(presearch_engine_ids.size(), 0ul);

  // Get the default engine (overridable by country) for this version
  PresearchPrepopulatedEngineID default_id =
      GetDefaultSearchEngine(country_id, version);

  // Build a vector PrepopulatedEngines from PresearchPrepopulatedEngineIDs and
  // also get the default engine index
  std::vector<const PrepopulatedEngine*> engines =
      GetEnginesFromEngineIDs(presearch_engine_ids, country_id,
                              default_id, default_search_provider_index);
  DCHECK(engines.size() == presearch_engine_ids.size());

  std::vector<std::unique_ptr<TemplateURLData>> t_urls;
  for (const PrepopulatedEngine* engine : engines) {
    std::unique_ptr<TemplateURLData> t_urld =
        TemplateURLDataFromPrepopulatedEngine(*engine);
    UpdateTemplateURLDataKeyword(t_urld);
    t_urls.push_back(std::move(t_urld));
  }

  return t_urls;
}

}  // namespace

// Redefines function with the same name in Chromium. We need to account for
// the version of Presearch engines as well: kCurrentDataVersion is defined in
// prepopulated_engines.json and is bumped every time the json file is
// modified. Since we add our own engines we need to keep track of our
// version as well and combine it with Chromium's version.
int GetDataVersion(PrefService* prefs) {
  int dataVersion = GetDataVersion_ChromiumImpl(prefs);
  // Check if returned version was from preferences override and if so return
  // that version.
  if (prefs && prefs->HasPrefPath(prefs::kSearchProviderOverridesVersion))
    return dataVersion;
  return (dataVersion + kPresearchCurrentDataVersion);
}

// Redefines function with the same name in Chromium. Modifies the function to
// get search engines defined by Presearch.
std::vector<std::unique_ptr<TemplateURLData>> GetPrepopulatedEngines(
    PrefService* prefs,
    size_t* default_search_provider_index) {
  // If there is a set of search engines in the preferences file, it overrides
  // the built-in set.
  if (default_search_provider_index)
    *default_search_provider_index = 0;
  std::vector<std::unique_ptr<TemplateURLData>> t_urls =
      GetPrepopulatedTemplateURLData(prefs);
  if (!t_urls.empty())
    return t_urls;

  int version = kPresearchFirstTrackedDataVersion;
  if (prefs && prefs->HasPrefPath(kPresearchDefaultSearchVersion)) {
    version = prefs->GetInteger(kPresearchDefaultSearchVersion);
  }

  return GetPresearchPrepopulatedEnginesForCountryID(
      country_codes::GetCountryIDFromPrefs(prefs),
      default_search_provider_index,
      version);
}

// Redefines function with the same name in Chromium. Modifies the function to
// get search engines defined by Presearch.
#if defined(OS_ANDROID)

std::vector<std::unique_ptr<TemplateURLData>> GetLocalPrepopulatedEngines(
    const std::string& locale) {
  int country_id = country_codes::CountryStringToCountryID(locale);
  if (country_id == country_codes::kCountryIDUnknown) {
    LOG(ERROR) << "Unknown country code specified: " << locale;
    return std::vector<std::unique_ptr<TemplateURLData>>();
  }

  return GetPresearchPrepopulatedEnginesForCountryID(country_id);
}

#endif

// Functions below are copied verbatim from
// components\search_engines\template_url_prepopulate_data.cc because they
// need to call our versions of redefined Chromium's functions.

std::unique_ptr<TemplateURLData> GetPrepopulatedEngine(PrefService* prefs,
                                                       int prepopulated_id) {
  size_t default_index;
  auto engines =
      TemplateURLPrepopulateData::GetPrepopulatedEngines(prefs, &default_index);
  for (auto& engine : engines) {
    if (engine->prepopulate_id == prepopulated_id)
      return std::move(engine);
  }
  return nullptr;
}

std::unique_ptr<TemplateURLData> GetPrepopulatedDefaultSearch(
    PrefService* prefs) {
  size_t default_search_index;
  // This could be more efficient.  We are loading all the URLs to only keep
  // the first one.
  std::vector<std::unique_ptr<TemplateURLData>> loaded_urls =
      GetPrepopulatedEngines(prefs, &default_search_index);

  return (default_search_index < loaded_urls.size())
             ? std::move(loaded_urls[default_search_index])
             : nullptr;
}

}  // namespace TemplateURLPrepopulateData
