# Copyright (c) 2019 The Presearch Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/. */

import os

PRESEARCH_THIRD_PARTY_DIRS = [
    'vendor',
]

ANDROID_ONLY_PATHS = []

DESKTOP_ONLY_PATHS = []


def AddPresearchCredits(prune_paths, special_cases, prune_dirs, additional_paths):
    # Exclude these specific paths from needing a README.chromium file.
    prune_paths.update([
        # Formerly external Presearch code which has moved to presearch-core
        # (i.e these are already covered by the Presearch Browser license notice).
        os.path.join('presearch', 'vendor', 'bat-native-ads'),
        os.path.join('presearch', 'vendor', 'bat-native-ledger'),
        os.path.join('presearch', 'vendor', 'presearch-ios'),
        os.path.join('presearch', 'vendor', 'presearch_base'),

        # Presearch overrides to third-party code, also covered by main notice.
        os.path.join('presearch', 'third_party', 'android_deps'),
        os.path.join('presearch', 'third_party', 'blink'),
        os.path.join('presearch', 'third_party', 'libaddressinput'),

        # Build dependencies which don't end up in the binaries.
        os.path.join('presearch', 'vendor', 'depot_tools'),
        os.path.join('presearch', 'vendor', 'gn-project-generators')
    ])

    # Add the licensing info that would normally be in a README.chromium file.
    # This is for when we pull in external repos directly.
    special_cases.update({
        os.path.join('presearch', 'vendor', 'bat-native-anonize'): {
            "Name": "bat-native-anonize",
            "URL": "https://github.com/brave-intl/bat-native-anonize",
            "License": "Apache-2.0",
            "License File": "/presearch/vendor/bat-native-anonize/LICENSE.txt",
        },
        os.path.join('presearch', 'vendor', 'bat-native-rapidjson'): {
            "Name": "RapidJSON",
            "URL": "https://github.com/brave-intl/bat-native-rapidjson",
            "License": "MIT",
            "License File": "/presearch/vendor/bat-native-rapidjson/license.txt",
        },
        os.path.join('presearch', 'vendor', 'bat-native-tweetnacl'): {
            "Name": "TweetNaCl",
            "URL": "https://github.com/brave-intl/bat-native-tweetnacl",
            "License": "MPL-2.0",
        },
        os.path.join('presearch', 'vendor', 'bip39wally-core-native'): {
            "Name": "libwally-core",
            "URL": "https://github.com/brave-intl/bat-native-bip39wally-core",
            "License": "MIT",
        },
        os.path.join('presearch', 'vendor', 'boto'): {
            "Name": "boto",
            "URL": "https://github.com/boto/boto",
            "License": "MIT",
        },
        os.path.join('presearch', 'vendor', 'presearch-extension'): {
            "Name": "Presearch Only Extension",
            "URL": "https://github.com/presearch/presearch-extension",
            "License": "MPL-2.0",
        },
        os.path.join('presearch', 'vendor', 'challenge_bypass_ristretto_ffi'): {
            "Name": "challenge-bypass-ristretto-ffi",
            "URL": "https://github.com/brave-intl/challenge-bypass-ristretto-ffi",
            "License": "MPL-2.0",
        },
        os.path.join('presearch', 'vendor', 'extension-whitelist'): {
            "Name": "extension-whitelist",
            "URL": "https://github.com/presearch/extension-whitelist",
            "License": "MPL-2.0",
        },
        os.path.join('presearch', 'vendor', 'hashset-cpp'): {
            "Name": "Hash Set",
            "URL": "https://github.com/presearch/hashset-cpp",
            "License": "MPL-2.0",
        },
        os.path.join('presearch', 'vendor', 'omaha'): {
            "Name": "Omaha",
            "URL": "https://github.com/presearch/omaha",
            "License": "Apache-2.0",
            "License File": "/presearch/vendor/omaha/LICENSE.txt",
        },
        os.path.join('presearch', 'vendor', 'python-patch'): {
            "Name": "Python Patch",
            "URL": "https://github.com/presearch/python-patch",
            "License": "MIT",
            "License File": "/presearch/vendor/python-patch/doc/LICENSE",
        },
        os.path.join('presearch', 'vendor', 'requests'): {
            "Name": "Requests",
            "URL": "https://github.com/psf/requests",
            "License": "Apache-2.0",
        },
        os.path.join('presearch', 'vendor', 'sparkle'): {
            "Name": "Sparkle",
            "URL": "https://github.com/presearch/Sparkle",
            "License": "MIT",
        },
    })

    # Don't recurse into these directories looking for third-party code.
    prune_list = list(prune_dirs)
    prune_list += [
        'chromium_src',  # Presearch's overrides, covered by main notice.
        'node_modules',  # See presearch/third_party/npm-* instead.
        '.vscode',       # Automatically added by Visual Studio.
    ]
    prune_dirs = tuple(prune_list)

    # Look for a README.chromium file directly inside these directories.
    # This is for directories which include third-party code that isn't
    # contained under a "third_party" or "vendor" directory.
    additional_list = list(additional_paths)
    additional_list += [
        os.path.join('presearch', 'components', 'presearch_prochlo'),
        os.path.join('presearch', 'components', 'presearch_new_tab_ui', 'data'),
    ]
    additional_paths = tuple(additional_list)

    return (prune_dirs, additional_paths)


def CheckPresearchMissingLicense(target_os, path, error):
    if path.startswith('presearch'):
        if (target_os == 'android'):
            if path in DESKTOP_ONLY_PATHS:
                return  # Desktop failures are not relevant on Android.
        else:
            if path in ANDROID_ONLY_PATHS:
                return  # Android failures are not relevant on desktop.
        raise error
