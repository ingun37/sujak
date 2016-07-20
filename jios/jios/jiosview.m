//
//  jiosview.m
//  jios
//
//  Created by ingun on 9/23/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jiosview.h"
#import <QuartzCore/CAMetalLayer.h>
@implementation jiosview

+(Class)layerClass
{
	return [CAMetalLayer class];
}

@end
