## PushIOManager for iOS

* [Integration Guide](http://docs.push.io)

## iOS 10 Support
* Ensure that 'Push Notifications' is enabled in your application settings 'Capabilities' tab ![alt text](https://raw.githubusercontent.com/pushio/PushIOManager_iOS/master/NotificationCapabilities.png "Application Capabilities")


* [UserNotifications](https://developer.apple.com/reference/usernotifications) framework is not supported in current version of PushIO SDK. For iOS 10 notification support application must override following didReceiveRemoteNotification methods as:

```objective-c

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
