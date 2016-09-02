//
//  PushIOManager.h
//  PushIOManager
//
//  Copyright (c) 2009-2015 Oracle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "PIOPreference.h"

/**
 `PushIOManager` is the primary class used for interfacing with the PushIO notification server. It is a singleton that you call to forward any system related push events, which are processed and transmitted to the PushIO server as needed.
 
 ## Requirements
 IMPORTANT: To use the Push IO service, you are required to have a PushIO API key. You can obtain this by setting up an account and adding an app at https://manage.push.io

 ## Setup
 After you add an app, set up your iOS platform via the "Set Up" section. You will then see a link to download a "pushio_config.json" file, which contains your API Key.

 Place this file in your project alongside the AppDelegate (so that it is included in your application bundle).
*/


// If you subscribe to these notification you'll be notified when a push is processed by this library, or the system has registered.
// You can also get the same information by setting a delegate for PushIOManager, described below.

/**
   Name for the notification broadcast when a push notification is processed.
   Includes dictionary of lastPushMeta in userInfo.
 */
extern NSString * const kPushProcessedNotification;

/**
 Name for the notification broadcast when push registration succeeds (a push token has been obtained and transmitted to the PushIO server).
 */
extern NSString * const kPushRegistrationSuccess;

/**
 Name for the notification broadcast when push registration fails.
 Includes dictionary with error and statusCode entries in userInfo.
 */
extern NSString * const kPushRegistrationFailure;


/**
 You may implement the optional `PushIOManagerDelegate` to be informed of push registration and incoming push notifications.
 */
@protocol PushIOManagerDelegate <NSObject>
@optional

/** 
 `PushIOManager` has a valid token and is ready to send a registration to the PushIO servers.
 */
- (void)readyForRegistration;

/** 
   Registration is called after a token is received, or when categories/trackers change.
 */
- (void)registrationSucceeded;

/**
 Called when registration fails for any reason (which can include user disabling push, or system failures).
 */
- (void)registrationFailedWithError:(NSError *)error statusCode:(int)statusCode;

/**
 In processing a push we found that new Newsstand content is ready.
 */
- (void)newNewsstandContentAvailable;

/**
 `PushIOManager` has extracted a push dictionary, the metadata is ready to be retrieved.
 */
- (void)pushProcessed;

@end


/**
The `PushIODebugLevel` enum is to be used when setting the logging level for PushIOManager - the default is "ERRORS_ONLY"
 */
typedef enum  {
    PUSHIO_DEBUG_NONE = 0,
    PUSHIO_DEBUG_ERRORS_ONLY = 1,
    PUSHIO_DEBUG_ACTIVITY = 2,
    PUSHIO_DEBUG_VERBOSE = 3
} PushIODebugLevel;

/**
 The `PushIOEngagementMetrics` enum is used when programmatically triggering an engagement with the PushIO server.
 
 ## Usage
 Applications should stick to one of the following enum values:
 
    `PUSHIO_ENGAGEMENT_METRIC_INAPP_PURCHASE`
    `PUSHIO_ENGAGEMENT_METRIC_PREMIUM_CONTENT`
    `PUSHIO_ENGAGEMENT_METRIC_SOCIAL`
 */
typedef enum  {
    PUSHIO_ENGAGEMENT_METRIC_LAUNCH = 0,  // Push IO internal use
    PUSHIO_ENGAGEMENT_METRIC_ACTIVE_SESSION = 1, // Push IO internal use
    PUSHIO_ENGAGEMENT_METRIC_INAPP_PURCHASE = 2,
    PUSHIO_ENGAGEMENT_METRIC_PREMIUM_CONTENT = 3,
    PUSHIO_ENGAGEMENT_METRIC_SOCIAL = 4,
    PUSHIO_ENGAGEMENT_METRIC_ACTION = 5, // Push IO internal use
    PUSHIO_ENGAGEMENT_METRIC_OTHER = 6, // Push IO internal use
} PushIOEngagementMetrics;

// Because 64-bit support added to iOS requires a iOS6.0 deployment target or higher, this version of the framework
// also requires a deployment target of 6.0 or higher.
// If you need iOS5 support, a non 64-bit version of the framework is available in the PushIOManager-IOS repository,
// under the directory "iOS5CompatibleFramework".
#if !defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) || defined(IOS_50_COMPAT)
#else
#if __IPHONE_OS_VERSION_MIN_REQUIRED < 60000
#error The 64-bit compatible version of the PushIOManager framework requires a deployment target of 6.0 or higher.
#error Use the "iOS5CompatibleFramework" PushIOManager.framework from the public repository for a 5.0 compatible version.
#endif
#endif

@class PIOPreference;

/**
 The primary class for interacting with PushIO.
 */
@interface PushIOManager : NSObject

/**
 Optional delegate to get messages about push registration and processing status.  Also obtainable via `NSNotificationCenter`.
 */
@property (nonatomic, weak) id <PushIOManagerDelegate> delegate;

/**
 See the `PushIODebugLevel` enum for valid debug levels.  The higher the value, the more activity will be logged.
 */
@property (nonatomic, assign) PushIODebugLevel debugLevel;

/**
 By default `PushIOManger` reports the local time zone to the server to use for push determination.
 `overrideTimeZone` allows you to set a custom timezone around which outbound pushes can be filtered on the server.
 */
@property (nonatomic, strong) NSTimeZone *overrideTimeZone;

// We store away the last updates to badge values, alert text and sound files stored in the alert so you do not have to do your own parsing.

/**
 `lastAlertText` holds any alert text sent in the last processed push notification.  May be nil.
 */
@property (nonatomic, strong) NSString *lastAlertText;
/**
 `lastAlertText` holds any sound text sent in the last processed push notification.  May be nil.
 */
@property (nonatomic, strong) NSString *lastSoundFileName;
/**
 `lastAlertText` holds any badge text sent in the last processed push notification.  May be nil.  
 
 @warning The system automatically applies badge updates when a push notification is received while the application is closed, so you may want to always set the app badge correctly when an app is re-opened if you are using this feature of push notifications.
 */
@property (nonatomic, strong) NSString *lastBadgeUpdate;

/**
 `lastPushMeta` gives you the entire push dictionary last handed to the library above the APS level, along with other meta values that may have been sent in the push.
 */
@property (nonatomic, strong) NSDictionary *lastPushMeta;

/**
 Each push received includes a unique engagementID that relates to that specific push. `lastEngagementID` gives you the last ID received.
 */
@property (nonatomic, copy, readonly) NSString *lastEngagementID;


//
// Setup
//

// Methods to be integrated into your app lifecycle to help us configure your app for Push IO
/**
 `didFinishLaunchingWithOptions:` is a forwarding method to be invoked from the application delegate's `didFinishLaunchingWithOptions:` method.
 */
- (void) didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
/**
 `didReceiveRemoteNotification:` is a forwarding method to be invoked from the application delegate's `didReceiveRemoteNotification:` method.
 */
- (void) didReceiveRemoteNotification:(NSDictionary *)userInfo;
/**
 `didRegisterForRemoteNotificationsWithDeviceToken:` is a forwarding method to be invoked from the application delegate's `didRegisterForRemoteNotificationsWithDeviceToken:` method.
 */
- (void) didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
/**
 `didFailToRegisterForRemoteNotificationsWithError:` is a forwarding method to be invoked from the application delegate's `didFailToRegisterForRemoteNotificationsWithError:` method.
 */
- (void) didFailToRegisterForRemoteNotificationsWithError:(NSError *)error;
/**
 `openURL:sourceApplication:annotation:` is a forwarding method meant to be invoked from the application delegate's `application:openURL:sourceApplication:annotation:` method.
 */
- (BOOL) openURL:(NSURL *)URL sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 70000
/**
 IOS7+ defined a more advanced mechanism for handling notifications, where a block can be invoked when a push notification is received.
 
 When engagement of a push notification is complete, call the fetch completion handler with the indicated fetch result
 An engagement call to PushIO will occur within five seconds of this call being invoked.

 @warning This call replaces the call to the system `fetchCompletionHandler` method that would normally be invoked for action notifications.
 */
- (void) didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionResult:(UIBackgroundFetchResult)fetchResult fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))handler;
#endif

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
/**
 This call is built to work with Action notifications, which require a completion handler to be called when you have completed handing the action chosen by the user.
 
 Note that PushIOManager will call the completion handler, so make sure you do any additional processing before this call. Engagement call will occur within five seconds of this call being activated.

 @warning This call replaces the call to the system `completionHandler` method that would normally be invoked for action notifications.
 */
- (void) handleActionWithIdentifier:(NSString *)identifier forRemoteNotification:(NSDictionary *)userInfo completionHandler:(void (^)(void))completionHandler;
#endif

//
//
// Simple (Broadcast Push) Registration
//

/**
 Register a device with Push IO so you can broadcast to a user, without them opting into specific categories.
 */
- (void) registerWithPushIO;

/**
 Delete a device from Push IO.
 */
- (void) unregisterFromPushIO;

/**
 Tells you if application has successfully registered with PushIO to receive notifications.
 */
- (BOOL) readyToReceivePushNotifications;

//
// Targeted (Segmentation/Category Push) Registration 
/**
 Register a list of categories for this device with Push IO. Categories let you opt your user into specific groups such as "Birdwatchers" or "BroncosFans"

 Any categories not present in the array will be deregistered if already registered with the server.

 @warning An empty array is treated as an Unregister call.
 */
- (void) registerCategories:(NSArray *)categories;

/**
 Registers a single category, leaving any other categories still registered.
 */
- (void) registerCategory:(NSString *)category;

/** 
 Unregisters a single category, leaving all other categories in place.
 */
- (void) unregisterCategory:(NSString *)category;

/**
 Unregisters a group of categories, leaving any categories not in the passed in array still registered.
 */
- (void) unregisterCategories:(NSArray *)categories;

/**
 Unregister all categories for this device from Push IO.
 */
- (void) unregisterAllCategories;

/** 
 Returns information on the categories for which this app has requested registration.
 */
- (BOOL) isRegisteredForCategory:(NSString *)category;

/**
 Returns all categories the user is currently registered to.
 */
- (NSArray *) allRegisteredCategories;

/**
 An optional UserID can be set to target individual users for push notifications, sent along with push registration.
 */
- (void) registerUserID:(NSString *)userID;

/**
 Forgets any registered userID and re-registers with PushIO.
 */
- (void) unregisterUserID;

/**
 Checks to see if a given userID has been registered.
 */
- (BOOL) isRegisteredForUserID:(NSString *)userID;

/**
 The current registered UserID.
 */
- (NSString *) registeredUserID;

//
// Engagement Metric Tracking
//

/**
 You can call this from anywhere to tell PushIO the last push led to a conversion.
 */
- (void) trackEngagementMetric:(PushIOEngagementMetrics)metric;

/**
 You can call this from anywhere to tell PushIO the last push led to a conversion, using a custom metric with an arbitrary string value.
 */
- (void) trackEngagementCustomMetric:(NSString *)customMetric;

/**
 Generally you can use one of the simpler engagement methods, which calls this method in turn.
 */
- (BOOL) trackEngagementOptions:(NSDictionary *)optionsDict withMetric:(PushIOEngagementMetrics)metric;

/**
 Tells you if the application was last launched (or opened from background) as the result of a notification.
 */
- (BOOL) isCurrentSessionAnEngagement;

//
// Event Tracking
//

/**
 Trigger an event.
 */
- (void) trackEvent:(NSString *)eventType;

//
// Other Helpers
//

// Convenience methods for asking iOS to register for push

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
/**
Convenience method : Combines the new iOS8 calls for registering settings and asking the system to register for remote notifications.
 */
- (void) registerForRemoteNotificationWithSettings:(UIUserNotificationSettings *)settings;
#endif
                                                                                       
/** 
 Calls either of the registration calls required for iOS8 or earlier iOS versions (the methods changed and you must call the appropriate method for your iOS version), registering for Sound/Badge/Alert types.
 @warning Does not register for newsstand.
 */
- (void) registerForAllRemoteNotificationTypes;


/** 
 Override for the PushIO API host.  Leave alone unless you have a specific PushIO API host to target.
 */
- (NSString *) pushIOAPIHost;

/**
 The current Account token in use.
 */
- (NSString *) pushIOAccountToken;

/**
 The current API key in use.
 */
- (NSString *) pushIOAPIKey;

/**
 Allows the application to specify an Device Tracking ID programatically.
 */
@property (nonatomic, strong) NSString *externalDeviceTrackingID;

/**
 Allows the application to specify an IDFA programatically.
 */
@property (nonatomic, strong) NSString *advertisingIdentifier;

/**
 Override for your Account Token.  You can set the Account Token here instead of the pushio_config.json if you prefer, though the json file adds extra safeguards.
 @warning This overrides account token read from PushIO configuration files. Preferably set the account token before overriding APIKey.
 */
@property (nonatomic, strong) NSString *overrideAccountToken;

/**
 Override for your PushIO API Key.  You can set the API key here instead of the pushio_config.json if you prefer, though the json file adds extra safeguards.
 @warning This overrides any values in the PushIO configuration files.
 */
@property (nonatomic, strong) NSString *overridePushIOAPIKey;


/**
 A unique ID used by Push IO to register a device. You can use this for adding test devices at https://manage.push.io
 This call will always return a non-null value.
 @warning Uses CFUUID internally.
 */
- (NSString *) pushIOUUID;

/** 
 Returns the version of this framework in use.
 */
- (NSString *) frameworkVersion;

//
// Singleton instance
//

/**
 Returns the singleton instance of the `PushIOManager`.
 */
+ (PushIOManager *) sharedInstance;

/**
 Sets the delegate of the shared instance, convenience for the instance method.
 */
+ (void) setDelegate:(id <PushIOManagerDelegate>)delegate;

//
// Notification Preferences
//

/**
 Declares a preference.
 
 A preference is not stored until a value has been assigned by a setter method.
 
 @see setBoolPreference:forKey:
 @see setNumberPreference:forKey:
 @see setStringPreference:forKey:
 */
- (void) declarePreference:(NSString *)key label:(NSString *)label type:(PIOPreferenceType)type error:(NSError **)error;

/**
 Assigns a boolean value to a previously declared preference.
 
 @return Whether the value assignment was successful.
 */
- (BOOL) setBoolPreference:(BOOL)value forKey:(NSString *)key;

/**
 Assigns a numeric value to a previously declared preference.
 
 @return Whether the value assignment was successful.
 */
- (BOOL) setNumberPreference:(NSNumber *)value forKey:(NSString *)key;

/**
 Assigns a string value to a previously declared preference.
 
 @return Whether the value assignment was successful.
 */
- (BOOL) setStringPreference:(NSString *)value forKey:(NSString *)key;

/**
 Retrieves a preference.
 */
- (PIOPreference *) getPreference:(NSString *)key;

/**
 Returns declared preferences.
 */
- (NSArray *) getPreferences;

/**
 Remove a preference. Preference must be redeclared if set again.
 */
- (void) removePreference:(NSString *)key error:(NSError **)error;

/**
 Removes all preferences. Preferences must be redeclared if set again.
 */
- (void) clearAllPreferences;

@end
