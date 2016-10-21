//
//  jmetaluniformbuffer.m
//  jmetal
//
//  Created by ingun on 9/27/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//


#import "jmetaluniformbuffer.hpp"
#import "jmtlBuffer.hpp"
jmtlBuffer buff;
@implementation jmetaluniformbuffer
-(id)initWithDevice:(id<MTLDevice>)device
{
	self = [super init];
	buff.init(128, device, MTLResourceCPUCacheModeWriteCombined);
	return self;
}
@end
