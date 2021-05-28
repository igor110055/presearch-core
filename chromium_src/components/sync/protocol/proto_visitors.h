/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_

#define PRESEARCH_VISIT_DEVICE_INFO_SPECIFICS_PRESEARCH_FIELDS \
VISIT(presearch_fields);

#define PRESEARCH_VISIT_PROTO_FIELDS_PRESEARCH_SPECIFIC_FIELD                  \
VISIT_PROTO_FIELDS(const sync_pb::PresearchSpecificFields& proto) {        \
  VISIT(is_self_delete_supported);                                     \
}

#include "../../../../../components/sync/protocol/proto_visitors.h"

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_
