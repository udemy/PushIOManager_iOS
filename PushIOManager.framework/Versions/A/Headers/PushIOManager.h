//
//  PushIOManager.h
//  PushIOManager
//
//  Copyright (c) 2009-2012 Push IO LLC. All rights reserved.
//

// This version of the PushIOManager library is 2.0.1

#import <Foundation/Foundation.h>

// IMPORTANT: To use the Push IO service, you are required to have a PushIO API key. You can
// obtain this by setting up an account and adding an app at https://manage.push.io
//
// After you add an app, set up your iOS platform via the "Set Up" section. You will then see a link
// to download a "pushio_config.json" file, which contains your API Key.  
//
// You need to place it inside of a file called "pushio_config.json", which you then
// place inside your project alongside the AppDelegate (so that it is included in your application bundle).



@protocol PushIOManagerDelegate <NSObject>
- (void)readyForRegistration;
- (void)registrationSucceeded;
- (void)registrationFailedWithError:(NSError *)error statusCode:(int)statusCode;
@end


typedef enum  {
    PUSHIO_DEBUG_NONE = 0,
    PUSHIO_DEBUG_ERRORS_ONLY = 1,
    PUSHIO_DEBUG_ACTIVITY = 2,
    PUSHIO_DEBUG_VERBOSE = 3
} PushIODebugLevel;

typedef enum  {
    PUSHIO_ENGAGEMENT_METRIC_LAUNCH = 0,  // Push IO internal use
    PUSHIO_ENGAGEMENT_METRIC_ACTIVE_SESSION = 1, // Push IO internal use
    PUSHIO_ENGAGEMENT_METRIC_INAPP_PURCHASE = 2,
    PUSHIO_ENGAGEMENT_METRIC_PREMIUM_CONTENT = 3,
    PUSHIO_ENGAGEMENT_METRIC_SOCIAL = 4,
    PUSHIO_ENGAGEMENT_METRIC_OTHER = 5, // Push IO internal use
} PushIOEngagementMetrics;


@interface PushIOManager : NSObject

// Set a delegate to get messages about push status, or to make calls that need to call back.
@property (nonatomic, assign) id <PushIOManagerDelegate> delegate;


// See the PushIODebugLevel enum for valid debug levels.
@property (nonatomic, assign) PushIODebugLevel debugLevel;

// We store away the last updates to badge values, alert text and sound files stored in the alert so you
// do not have to do your own parsing.
@property (nonatomic, strong) NSString *lastAlertText;
@property (nonatomic, strong) NSString *lastSoundFileName;
@property (nonatomic, strong) NSString *lastBadgeUpdate;


//
// Setup
//
// Methods to be integrated into your app lifecycle to help us configure your app for Push IO
- (void) didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
- (void) didReceiveRemoteNotification:(NSDictionary *)userInfo;
- (void) didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
- (void) didFailToRegisterForRemoteNotificationsWithError:(NSError *)error;
- (void) applicationDidEnterBackground;
- (void) applicationWillEnterForeground;
- (void) applicationDidBecomeActive;

//
//
// Simple (Broadcast Push) Registration
//
// Register a device with Push IO so you can broadcast to a user, without them opting into specific categories.
- (void) registerWithPushIO;

// Delete a device from Push IO
- (void) unregisterFromPushIO;

// Tells you if application has successfully registered with PushIO to receive notifications.
- (BOOL) readyToReceivePushNotifications;

//
// Targeted (Segmentation/Category Push) Registration 
//
// Register a list of categories for this device with Push IO. Categories let you opt your user into specific groups
// such as "Birdwatchers" or "BroncosFans"
//
// Any categories not present in the array will be degregistered if already registered with the server.
// An empty array is treated as an Unregister call. 
- (void) registerCategories:(NSArray *)categories;

// Registers a single category, leaving any other categories still registered.
- (void) registerCategory:(NSString *)category;

// Unregisters a single category, leaving all other categories in place.
- (void) unregisterCategory:(NSString *)category;

// Unregister all categories for this device from Push IO
- (void) unregisterAllCategories;


//
// Engagement Metric Tracking
//
//
// You can call this from anywhere to track whether your push led to a conversion
- (void) trackEngagementMetric:(PushIOEngagementMetrics)metric;

// You can call this to track a custom metric, and whether it led to a conversion
- (void) trackEngagementCustomMetric:(NSString *)customMetric;

// You don't need to call this method directly, we use it for you.
- (BOOL) trackEngagementOptions:(NSDictionary *)optionsDict withMetric:(PushIOEngagementMetrics)metric;

//
// Other Helpers
//
// Currently in-use Push IO api settings
- (NSString *) pushIOAPIHost;
- (NSString *) pushIOAPIKey;

// A unique ID used by Push IO. You can use this for adding test devices at https://manage.push.io
// You can also find this in the NSUserDefaults via the key @"PUSHIO_UUID" once it has been created.  
// This call will always return a non-null value.
- (NSString *) pushIOUUID;

//
// Singleton instance
//
+ (PushIOManager *) sharedInstance;

// Sets the delegate of the shared instance, shortcut method
+ (void) setDelegate:(id <PushIOManagerDelegate>)delegate;


@end


