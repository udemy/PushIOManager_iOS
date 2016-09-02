## PushIOManager for iOS

* [Integration Guide](http://docs.push.io)

## iOS 10 Support
* Ensure that 'Push Notifications' is enabled in your application settings 'Capabilities' tab ![alt text](http://i.stack.imgur.com/33Klz.png "Application Capabilities")


* [NSUserNotification](https://developer.apple.com/library/mac/documentation/Foundation/Reference/NSUserNotification_Class/) framework is not supported in current version (6.29.0) of PushIO SDK. For iOS 10 notification support application must override following didReceiveRemoteNotification methods as:

```objective-c
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler
{
    [[PushIOManager sharedInstance] didReceiveRemoteNotification:userInfo fetchCompletionResult:UIBackgroundFetchResultNewData fetchCompletionHandler:completionHandler];
}

-(void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo{
    [[PushIOManager sharedInstance] didReceiveRemoteNotification:userInfo];
}
```
## Other Resources
* [Downloads + Documenation] (http://docs.push.io)
* [Sign In / Sign Up] (https://manage.push.io)

## Contact
* Support: [My Oracle Support] (http://support.oracle.com)

Copyright © 2016, Oracle Corporation and/or its affiliates. All rights reserved. Oracle and Java are registered trademarks of Oracle and/or its affiliates. Other names may be trademarks of their respective owners.
