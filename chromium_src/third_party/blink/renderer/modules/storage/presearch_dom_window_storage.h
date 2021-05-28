/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_STORAGE_PRESEARCH_DOM_WINDOW_STORAGE_H_
#define PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_STORAGE_PRESEARCH_DOM_WINDOW_STORAGE_H_

#include "third_party/blink/renderer/platform/supplementable.h"

namespace blink {

class ExceptionState;
class LocalDOMWindow;
class StorageArea;

class PresearchDOMWindowStorage final
    : public GarbageCollected<PresearchDOMWindowStorage>,
      public Supplement<LocalDOMWindow> {
 public:
  static const char kSupplementName[];

  static PresearchDOMWindowStorage& From(LocalDOMWindow&);
  static StorageArea* sessionStorage(LocalDOMWindow&, ExceptionState&);
  static StorageArea* localStorage(LocalDOMWindow&, ExceptionState&);

  StorageArea* sessionStorage(ExceptionState&);
  StorageArea* localStorage(ExceptionState&);

  explicit PresearchDOMWindowStorage(LocalDOMWindow&);

  void Trace(Visitor*) const override;

 private:
  StorageArea* ephemeralSessionStorage();
  StorageArea* ephemeralLocalStorage();

  mutable Member<StorageArea> ephemeral_session_storage_;
  mutable Member<StorageArea> ephemeral_local_storage_;
};

}  // namespace blink

#endif  // PRESEARCH_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_STORAGE_PRESEARCH_DOM_WINDOW_STORAGE_H_
