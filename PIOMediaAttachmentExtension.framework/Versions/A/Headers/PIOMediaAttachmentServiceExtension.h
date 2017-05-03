//
//  PIOMediaAttachmentServiceExtension.h
//  PIOMediaAttachmentExtension
//
//  Copyright © 2009-2017 Oracle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UserNotifications/UserNotifications.h>


@interface PIOMediaAttachmentServiceExtension : UNNotificationServiceExtension

-(void)enableLogging;
-(void)disableLogging;
-(BOOL)isLoggingEnabled;

@end
