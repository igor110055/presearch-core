/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_IOS_BROWSER_API_BOOKMARKS_IMPORTER_PRESEARCH_BOOKMARKS_IMPORTER_H_
#define PRESEARCH_IOS_BROWSER_API_BOOKMARKS_IMPORTER_PRESEARCH_BOOKMARKS_IMPORTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, PresearchBookmarksImporterState) {
  PresearchBookmarksImporterStateCompleted,
  PresearchBookmarksImporterStateAutoCompleted,
  PresearchBookmarksImporterStateStarted,
  PresearchBookmarksImporterStateCancelled
};

OBJC_EXPORT
@interface PresearchImportedBookmark : NSObject
@property(nonatomic, readonly) bool inToolbar;
@property(nonatomic, readonly) bool isFolder;
@property(nullable, nonatomic, readonly, copy) NSURL* url;
@property(nullable, nonatomic, readonly, copy) NSArray<NSString*>* path;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly, copy) NSDate* creationTime;
@end

OBJC_EXPORT
@interface PresearchBookmarksImporter : NSObject
- (instancetype)init;

- (void)cancel;

- (void)importFromFile:(NSString*)filePath
    topLevelFolderName:(NSString*)folderName
       automaticImport:(bool)automaticImport
          withListener:
              (void (^)(PresearchBookmarksImporterState,
                        NSArray<PresearchImportedBookmark*>* _Nullable))listener;

- (void)importFromArray:(NSArray<PresearchImportedBookmark*>*)bookmarks
     topLevelFolderName:(NSString*)folderName
           withListener:(void (^)(PresearchBookmarksImporterState))listener;
@end

NS_ASSUME_NONNULL_END

#endif  // PRESEARCH_IOS_BROWSER_API_BOOKMARKS_IMPORTER_PRESEARCH_BOOKMARKS_IMPORTER_H_
