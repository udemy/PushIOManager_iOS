//
//  PushIOManager+PushIOLocation.h
//  PushIOManager
//
//  Copyright (c) 2012 Push IO LLC. All rights reserved.
//

#import <CoreLocation/CoreLocation.h>
#import "PushIOManager.h"
#import "PushiOManager+PushIOTrackers.h"

// If you are a delegate of PushIOManager, you will also be signed up for these callbacks also.
@protocol PushiOManagerLocationDelegate <NSObject>
@optional
// Returns you the current location as changes are detected.
// A null location means location services have been disabled.
- (void) locationDidChange:(CLLocation *)newLocation;
@end

@class PushIOLocationTracker;

// Category on PushIOManager.

@interface PushIOManager (PushIOLocation) <CLLocationManagerDelegate>

//      *********** SETUP AND MANAGEMENT OF LOCATION SERVICES FOR ENGAGEMENT TRACKING **************
// Two options are presented for location management, The first where PushIOManager maintains a CLLocationManager.
// In the second option you simply feed PushIOManager locations to record.
// In either option locations, if availaible, are set to PushIO on during any engagement tracking.
// Note that if you are providing locations directly, make sure to provide them BEFORE engagemnet calls.

// =========== Option (1) - pushIOManager can create and maintian a LocationManager.

// These calls tell PushIO to start monitoring location for push notification engagemnet tracking.
// Note that the pushIOManager library will maintain a CoreLocation LocationManager, and manage shutting down
// or starting up this location manager when the application suspends/resume.
// You need only call "startDetectingLocation" at the point in your aplication lifecycle when you feel it approprriate
// to generate the alert asking the user for permission to monitor locations.
- (void) startUpdatingLocationForPush;

// If you want to provide the user with a reason why the application would like to monitor location, add that string here.
- (void) startUpdatingLocationForPushWithReason:(NSString *)reasonForLocationMonitoring;

// You only need to call this if you wish to disable location services while the application is running.
- (void) stopUpdatingLocationForPush;

// By default PushIOManager will use the "significant change" location service for detecting location.
// If you set this property, the "standard location" service will be used with this value set.
@property (nonatomic, assign) CLLocationAccuracy desiredLocationAccuracy;

// Last location found by the internal location manager, so that you can use the location found for your own needs as well.
- (CLLocation *) lastLocationFound;


// =========== Option (2) - you can manage your own CoreLocation manager.

// Use this call to tell the pushIOManager when location changes.
// This same location will be passed back to PushIO for engagemnet tracking when set.
// The location persists for the life of the application, until changed again or the application is shut down.
// You can make a new CLLocation object from lat/long values using the CLLocation initWithLatitude:longitude: method.

- (void) assignCurrentLocation:(CLLocation *)location;
//***********


//      *********** HOME LOCATION *****************

// PushIOManager has the concept of a "home location" that can be set, this is passed up with registration calls only (not engagements which
// use the current location or a location you set dynamically)

@property (nonatomic, retain) CLLocation *homeLocation;


//      *********** LOCATION PUBLISHER **************

// In your pushIO config.json, add in a publisher entry with your location publisher GUID:
// { "publishers" : { "locationGUID" : "XYZPDQ" } }
// Then you can make use of the following calls to add locations to that publisher.

// Returns a tracker for this location you can use to delete this exact item later.
// You may also ignore the tracker, you can obtain it in the allLocationTrackers call.
// The tracker returned will have the trackerID set in case you wish to use for later extraction.
- (PushIOLocationTracker *) addLocationTrackerWithLocation:(CLLocation *)location;

// Replaces all current locations for a publisher with the contents of this array
// The array passed in shoudl hold PushIOLocationTracker objects.
- (void) addLocationTrackers:(NSArray *)locations;

// Returns an array of location tracker objects, from which you can extract location.
- (NSArray *) allLocationTrackers;

@end

@interface PushIOLocationTracker : PushIOTracker

// The location of this tracker.
@property (nonatomic, retain) CLLocation *location;

// Used to build a tracker object.
// The returned location tracker will have the location and trackerID set.
+ (PushIOLocationTracker *) locationTrackerWithLocation:(CLLocation *)location;

@end
