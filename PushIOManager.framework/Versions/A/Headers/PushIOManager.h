//
//  PushIOManager.h
//  PushIOManager
//
//  Copyright (c) 2009-2012 Push IO LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

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
    PUSHIO_ENGAGEMENT_METRIC_LAUNCH = 0,
    PUSHIO_ENGAGEMENT_METRIC_ACTIVE_SESSION = 1,
    PUSHIO_ENGAGEMENT_METRIC_INAPP_PURCHASE = 2,
    PUSHIO_ENGAGEMENT_METRIC_PREMIUM_CONTENT = 3,
    PUSHIO_ENGAGEMENT_METRIC_SOCIAL = 4,
    PUSHIO_ENGAGEMENT_METRIC_OTHER = 5,
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

// Every time an engagementID is parsed from a push, we store it until cleared by the app being killed or
// going into the background.   You can use this either to make use of the enagementID sent in a push, or
// simply to verify an engagementID was present after parsing the push results.
- (NSString *)lastEngagementID;

//
// Registration
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


// Register a device with Push IO so you can broadcast to a user, without them opting into specific categories.
- (void) registerWithPushIO;

// Delete this device from Push IO
- (void) unregisterFromPushIO;

// Tells you if application has successfully registered with PushIO to receive notifications.
- (BOOL) readyToReceivePushNotifications;

//
// Engagement Metric Tracking
//
// Call on resume where you have some kind of push dictionary
- (BOOL) trackEngagementOptions:(NSDictionary *)optionsDict withMetric:(PushIOEngagementMetrics)metric;

// You can call this from anywhere with a simple metric.
- (void) trackEngagementMetric:(PushIOEngagementMetrics)metric;

// Call this from your AppDelegate applicationDidEnterBackground to ensure proper metric tracking
- (void) resetLastEngagement;

// 
// Helpers
//
// Methods to help with push token registration.
// Note you can either simply register for a token and give it to Push IO (using pushTokenFound:)
// Or use the macro defined at the end of this file to add methods that will handle the callbacks for you.
- (void) pushTokenFound:(NSData *)token;
- (void) recordPushTokenError:(NSError *)pushTokenRegisterError;

// Currently in-use Push IO api settings
- (NSString *) pushIOAPIHost;
- (NSString *) pushIOAPIKey;

// A unique ID used by Push IO. You can use this for registering testing devices.
// You can also find this in the NSUserDefaults via the key @"PUSHIO_UUID" once it has been created.  
// This call will always return a non-null value.
- (NSString *) pushIOUUID;


// Sets the delegate of the shared instance, shortcut method
+ (void) setDelegate:(id <PushIOManagerDelegate>)delegate;

// 
// 
// Singleton instance
//
+(PushIOManager *) sharedInstance;

@end


// In your app delegate if you simply call:
//     [[UIApplication sharedApplication] registerForRemoteNotificationTypes:UIRemoteNotificationTypeBadge];

// then add the following macro in your application delegate @implmentation block, all of the push 
// related registration callbacks will be handled for you.

// You still need to implement the methods that handle incoming push requests.

/*
 
#define PUSHIO_HANDLE_PUSH_REGISTRATION \
\
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken\
{\
    [[PushIOManager sharedInstance] pushTokenFound:deviceToken];\
    [[PushIOManager sharedInstance] registerCategories:nil];\
}\
\
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error\
{\
    [[PushIOManager sharedInstance] recordPushTokenError:error];\
}\

#define PUSHIO_HANDLE_ENGAGEMENT_METRIC_ACTIVE_SESSION \
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo\
{\
    [[PushIOManager sharedInstance] trackEngagementOptions:userInfo withMetric:PUSHIO_ENGAGEMENT_METRIC_ACTIVE_SESSION];\
}\

*/
