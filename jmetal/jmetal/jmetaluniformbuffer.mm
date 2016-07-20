//
//  jmetaluniformbuffer.m
//  jmetal
//
//  Created by ingun on 9/27/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//


#import "jmetaluniformbuffer.hpp"

@implementation jmetaluniformbuffer
-(id)initWithDevice:(id<MTLDevice>)device
{
	buffer = [[jmetalbuffer alloc]initWithDevice:device initialSize:128 option:MTLResourceCPUCacheModeWriteCombined];
	return [super init];
}
@end
