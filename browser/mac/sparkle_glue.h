/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_MAC_SPARKLE_GLUE_H_
#define PRESEARCH_BROWSER_MAC_SPARKLE_GLUE_H_

#include "base/strings/string16.h"
#import "chrome/browser/mac/keystone_glue.h"

#if defined(__OBJC__)

#import <Foundation/Foundation.h>

@interface SparkleGlue : NSObject

+ (instancetype)sharedSparkleGlue;

- (instancetype)init;

- (void)registerWithSparkle;

- (void)checkForUpdates;

- (void)relaunch;

- (AutoupdateStatus)recentStatus;

- (NSNotification*)recentNotification;  // NOLINT - not sure how to fix this

// Returns YES if an asynchronous operation is pending: if an update check or
// installation attempt is currently in progress.
- (BOOL)asyncOperationPending;

// Returns YES if the application is running from a read-only filesystem,
// such as a disk image.
- (BOOL)isOnReadOnlyFilesystem;

@end  // @interface SparkleGlue

#endif  // __OBJC__

namespace sparkle_glue {

bool SparkleEnabled();

base::string16 CurrentlyInstalledVersion();

}  // namespace sparkle_glue

#endif  // PRESEARCH_BROWSER_MAC_SPARKLE_GLUE_H_
