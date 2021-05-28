/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_IOS_APP_PRESEARCH_CORE_MAIN_H_
#define PRESEARCH_IOS_APP_PRESEARCH_CORE_MAIN_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface PresearchCoreMain : NSObject

- (instancetype)init;

- (instancetype)initWithSyncServiceURL:(NSString*)syncServiceURL;

- (void)scheduleLowPriorityStartupTasks;

- (void)setUserAgent:(NSString*)userAgent;

@end

NS_ASSUME_NONNULL_END

#endif  // PRESEARCH_IOS_APP_PRESEARCH_CORE_MAIN_H_
