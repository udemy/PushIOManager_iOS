//
//  PushIOManager+PushTrackers.h
//  PushIOManager
//
//  Copyright (c) 2012 Push IO LLC. All rights reserved.
//

#import "PushIOManager.h"

typedef enum PushIOTrackerTypes {
    PUSHIO_TRACKER_UNKNOWN = 0,
    PUSHIO_TRACKER_LOCATION = 1
    } PushIOTrackerTypes;

// Defined at the bottom of this file.
@class PushIOTracker;

@interface PushIOManager (PushTrackers)

// Returns all trackers currently stored.
- (NSArray *) allTrackers;

// Returns only trackers of a specific type.
- (NSArray *) trackersForTrackerType:(PushIOTrackerTypes)type;

// Add a new tracker, will be associated with the publisher matching the tracker type.
- (void) addTracker:(PushIOTracker *)tracker;

// Remove an existing tracker from the system.
- (void) removeTrackerWithTrackerID:(NSString *)trackerID;

// Clear out all trackers across all publishers
- (void) removeAllTrackers;

// Removes only trackers with the given type
- (void) removeAllTrackersWithType:(PushIOTrackerTypes) type;

// Returns the matching publisher ID for a type.
- (NSString *) publisherIDForType:(PushIOTrackerTypes)type;

@end

@interface PushIOTracker : NSObject <NSCoding>

// The type of tracker object this is.
@property (nonatomic, assign) PushIOTrackerTypes trackerType;

// The location of this tracker.
@property (nonatomic, copy, readonly) NSString *trackerID;

@end

