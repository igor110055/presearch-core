/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMMON_PRESEARCH_CHANNEL_INFO_POSIX_H_
#define PRESEARCH_COMMON_PRESEARCH_CHANNEL_INFO_POSIX_H_

#include "components/version_info/version_info.h"

namespace presearch {

version_info::Channel GetChannelImpl(std::string* modifier_out,
                                     std::string* data_dir_suffix_out);

}

#endif  // PRESEARCH_COMMON_PRESEARCH_CHANNEL_INFO_POSIX_H_

