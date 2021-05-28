/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_IOS_BROWSER_API_BOOKMARKS_EXPORTER_PRESEARCH_BOOKMARKS_EXPORTER_H_
#define PRESEARCH_IOS_BROWSER_API_BOOKMARKS_EXPORTER_PRESEARCH_BOOKMARKS_EXPORTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, PresearchBookmarksExporterState) {
  PresearchBookmarksExporterStateCompleted,
  PresearchBookmarksExporterStateStarted,
  PresearchBookmarksExporterStateCancelled,
  PresearchBookmarksExporterStateErrorCreatingFile,
  PresearchBookmarksExporterStateErrorWritingHeader,
  PresearchBookmarksExporterStateErrorWritingNodes
};

@class IOSBookmarkNode;

OBJC_EXPORT
@interface PresearchBookmarksExporter : NSObject
- (instancetype)init;

- (void)exportToFile:(NSString*)filePath
        withListener:(void (^)(PresearchBookmarksExporterState))listener;

- (void)exportToFile:(NSString*)filePath
           bookmarks:(NSArray<IOSBookmarkNode*>*)bookmarks
        withListener:(void (^)(PresearchBookmarksExporterState))listener;
@end

NS_ASSUME_NONNULL_END

#endif  // PRESEARCH_IOS_BROWSER_API_BOOKMARKS_EXPORTER_PRESEARCH_BOOKMARKS_EXPORTER_H_
