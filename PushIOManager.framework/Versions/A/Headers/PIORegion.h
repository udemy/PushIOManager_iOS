//
//  PIORegion.h
//  PushIOManager
//
//  Copyright (c) 2009-2015 Oracle. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

/**
 @discussion This class wraps a CLRegion, adding extra metadata used by PushIO.
 */

@interface PIORegion : NSObject

/**
 Holds radius, lat/long, and identifier
 */
@property (nonatomic, strong) CLRegion *systemRegion;

/**
Repeat location for quicker sorting.
*/
 @property (nonatomic, strong) CLLocation *centerLocation;

/**
Repeat ID for quick access and easier sorting.
*/
@property (nonatomic, strong) NSString *identifier;

/**
 The system will maintain a count of times a region has been entered.
 */
@property (nonatomic, assign) NSInteger regionEntryCount;

/**
 Optional - present only if regions are loaded from remote layers (defined on PushIO servers)
 */
@property (nonatomic, strong) NSString *layerID;

@end
