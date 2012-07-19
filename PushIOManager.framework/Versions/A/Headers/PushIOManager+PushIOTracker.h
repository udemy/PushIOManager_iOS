//
//  PushIOManager+PushIOTracker.h
//  PushIOManager
//
//  Created on 5/16/12.
//  Copyright (c) 2012 Push IO LLC. All rights reserved.
//

// This category adds the ability to register for Trackers, pre-defined monitored data sets that provide 
// you with pushes based on changes.

#import "PushIOManager.h"

@interface PushIOManager (PushIOTracker)

- (void) addTracker:(NSString *)trackerID;

@end
