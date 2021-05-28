/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#import <Foundation/Foundation.h>
#import "BATPresearchLedgerObserver.h"
#import "BATPresearchLedger.h"

@interface BATPresearchLedgerObserver ()
@property (nonatomic, weak) BATPresearchLedger *ledger;
@end

@implementation BATPresearchLedgerObserver

- (instancetype)initWithLedger:(BATPresearchLedger *)ledger {
  if ((self = [super init])) {
    self.ledger = ledger;
  }
  return self;
}

@end
