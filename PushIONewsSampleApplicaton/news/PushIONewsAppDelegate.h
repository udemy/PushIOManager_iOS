//
//  PushIONewsAppDelegate.h
//  news
//
//  Copyright (c) 2012 Push IO LLC. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <PushIOManager/PushIOManager.h>

@interface PushIONewsAppDelegate : UIResponder <UIApplicationDelegate, PushIOManagerDelegate>

@property (strong, nonatomic) UIWindow *window;

@end
