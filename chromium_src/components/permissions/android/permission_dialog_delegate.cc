/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/android/permission_dialog_delegate.h"

#include "base/android/jni_array.h"
#include "base/android/jni_string.h"
#include "presearch/components/permissions/android/jni_headers/PresearchPermissionDialogDelegate_jni.h"
#include "presearch/components/permissions/permission_lifetime_utils.h"
#include "components/grit/presearch_components_strings.h"
#include "components/permissions/android/jni_headers/PermissionDialogController_jni.h"
#include "components/permissions/features.h"
#include "components/strings/grit/components_strings.h"
#include "ui/base/l10n/l10n_util.h"

namespace permissions {
namespace {

void SetLifetimeOptions(const base::android::JavaRef<jobject>& j_delegate) {
  if (!base::FeatureList::IsEnabled(features::kPermissionLifetime)) {
    return;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchPermissionDialogDelegate_setLifetimeOptionsText(
      env, j_delegate,
      base::android::ConvertUTF16ToJavaString(
          env, l10n_util::GetStringUTF16(
                   IDS_PERMISSIONS_BUBBLE_LIFETIME_COMBOBOX_LABEL)));

  std::vector<PermissionLifetimeOption> lifetime_options =
      CreatePermissionLifetimeOptions();
  std::vector<base::string16> lifetime_labels;
  for (const auto& lifetime_option : lifetime_options) {
    lifetime_labels.push_back(lifetime_option.label);
  }

  Java_PresearchPermissionDialogDelegate_setLifetimeOptions(
      env, j_delegate,
      base::android::ToJavaArrayOfStrings(env, lifetime_labels));
}

void ApplyLifetimeToPermissionRequests(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj,
    PermissionPromptAndroid* permission_prompt) {
  if (!base::FeatureList::IsEnabled(features::kPermissionLifetime)) {
    return;
  }
  const int selected_lifetime_option =
      Java_PresearchPermissionDialogDelegate_getSelectedLifetimeOption(env, obj);
  DCHECK(!ShouldShowLifetimeOptions(permission_prompt->delegate()) ||
         selected_lifetime_option != -1);
  if (selected_lifetime_option != -1) {
    std::vector<PermissionLifetimeOption> lifetime_options =
        CreatePermissionLifetimeOptions();
    SetRequestsLifetime(lifetime_options, selected_lifetime_option,
                        permission_prompt->delegate());
  }
}

void Java_PermissionDialogController_createDialog_PresearchImpl(
    JNIEnv* env,
    const base::android::JavaRef<jobject>& delegate) {
  SetLifetimeOptions(delegate);
  Java_PermissionDialogController_createDialog(env, delegate);
}

}  // namespace
}  // namespace permissions

#define PRESEARCH_PERMISSION_DIALOG_DELEGATE_ACCEPT \
  ApplyLifetimeToPermissionRequests(env, obj, permission_prompt_);
#define PRESEARCH_PERMISSION_DIALOG_DELEGATE_CANCEL \
  ApplyLifetimeToPermissionRequests(env, obj, permission_prompt_);
#define Java_PermissionDialogController_createDialog \
  Java_PermissionDialogController_createDialog_PresearchImpl

#include "../../../../../components/permissions/android/permission_dialog_delegate.cc"

#undef Java_PermissionDialogController_createDialog
#undef PRESEARCH_PERMISSION_DIALOG_DELEGATE_CANCEL
#undef PRESEARCH_PERMISSION_DIALOG_DELEGATE_ACCEPT
