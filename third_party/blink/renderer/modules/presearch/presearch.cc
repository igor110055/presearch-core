/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/third_party/blink/renderer/modules/presearch/presearch.h"

#include "third_party/blink/renderer/bindings/core/v8/script_promise.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise_resolver.h"

namespace blink {

ScriptPromise Presearch::isPresearch(ScriptState* script_state) {
  auto* resolver = MakeGarbageCollected<ScriptPromiseResolver>(script_state);
  ScriptPromise promise = resolver->Promise();
  resolver->Resolve(true);
  return promise;
}

}  // namespace blink
