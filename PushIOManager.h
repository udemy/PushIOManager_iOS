/*
 *  PushIOManager.h
 *  Version 1.0.12
 *
 *  Copyright 2009 - 2012 Push IO LLC. All rights reserved.
 *
 */

// version history:
//  1.0.12
//      - add armv6 
//  1.0.11
//      - add unregister methods
//      - rename methods to be more understandable
//  1.0.10a
//      - added requestNotificationTypesForThisDevice

// To integrate PushIOManager:
// 1. Add libPushIOManager.a to your project and make sure it is
// 2. Include PushIOManager.h in your code files that call it


// PREREQUISITES before calling the Push IO Register method:
// 1. PushIORegistrationHost and PushIONotificationHost values must be set in your application's Info.plist
// 3. apiToken must be stored in NSUserDefaults in the APIToken key
// 4. deviceToken must be stored in NSUserDefaults in the DeviceToken key, so it must have been successfully received from the iPhoneOS call


#import <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <ifaddrs.h>
#include <net/if_dl.h>
#include <sys/socket.h>

#ifdef DEBUG
#   define DNSLog(fmt, ...) NSLog((@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#   define DNSLog(...)
#endif


@protocol PushIOManagerDelegate
- (void)registrationSucceeded;
- (void)registrationFailedWithError:(NSError *)error statusCode:(int)statusCode;
- (void)availableNotificationTypesSucceeded:(NSArray *)notificationTypes; 
- (void)availableNotificationTypesFailedWithError:(NSError *)error statusCode:(int)statusCode;
@end


@interface PushIOManager : NSObject <NSXMLParserDelegate> {

	int statusCode;
	id<PushIOManagerDelegate> delegate;
	NSError *error;
	
	NSMutableData *receivedData;
	
	int tries;
	int lastServer;
	int lastMethodNumber;
	NSString *lastParameters;
	NSString *contentType;
    
    NSMutableArray *notificationTypesArray;
	
}

@property (assign, nonatomic) id<PushIOManagerDelegate> delegate;
@property (retain, nonatomic) NSError *error;
@property (retain, nonatomic) NSString *lastParameters;
@property (retain, nonatomic) NSString *contentType;
@property (retain, nonatomic) NSMutableArray *notificationTypesArray;

// Register with Push IO with Channels (Channels are Strings)
- (void)registerChannelsWithPushIO:(NSArray *)channels;

// Register with Push IO with Channels and username
- (void)registerChannelsWithPushIO:(NSArray *)channels username:(NSString *)username;

// Unregister with Push IO with Channels (Channels are Strings)
- (void)unregisterChannelsWithPushIO:(NSArray *)channels;

// Unregister from all Push IO Channels the device is currently registered to for this app
- (void)unregisterAllChannelsWithPushIO;

// Notification Types available for your application as defined from your Push IO admin portal
- (void)requestAvailableNotificationTypes;

// Notification Types that this device has registered for
- (void)requestNotificationTypesForThisDevice;


- (void)trackEngagement:(NSDictionary *)launchOptions;
// Track an engagement.
// Call this during application:didFinishLaunchingWithOptions: with the launchOptions dictionary supplied to that method.
// Example:
/*
 - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
 // Track engagement
 PushIOManager *pushIOManager = [[PushIOManager alloc] init];
 [pushIOManager trackEngagement:launchOptions];
 [pushIOManager release];
 ...
 }
 */



// Register with PushIO with notification types (DEPRECATED)
- (void)registerWithNotificationTypes:(NSArray *)notificationTypes;

// Register with PushIO notification types and a unique service identifier (DEPRECATED)
- (void)registerWithNotificationTypes:(NSArray *)notificationTypes withServiceIdentifier:(NSString *)uniqueIdentifier;



@end
