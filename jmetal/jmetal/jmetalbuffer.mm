//
//  jmetalbuffer.m
//  jmetal
//
//  Created by ingun on 9/6/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "jmetalbuffer.hpp"

@implementation jmetalbuffer


-(id)init
{
	self = [super init];
	
	_offset = 0;
	
	return self;
}

-(id)initWithDevice:(id<MTLDevice>)device initialSize:(unsigned long)initialSize option:(MTLResourceOptions)option
{
	_buffer = [device newBufferWithLength:initialSize options:option];
	return [self init];
}

-(void)append:(const void *)data len:(unsigned int)len
{
	memcpy((char*)[_buffer contents] + _offset, data, len);
	_offset += len;
}

-(void)reset
{
	_offset = 0;
}
@end
