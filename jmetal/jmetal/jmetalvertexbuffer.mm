//
//  jmetalvertexbuffer.m
//  jmetal
//
//  Created by ingun on 9/21/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jmetalvertexbuffer.hpp"
using namespace sujak;
@implementation jmetalvertexbuffer

-(id)initWithDevice:(id<MTLDevice>)device
{
	for(int i=0;i<JVertexAttribute_number;i++)
	{
		buffers[i] = [[jmetalbuffer alloc] initWithDevice:device initialSize:1024 option:MTLResourceCPUCacheModeWriteCombined];
	}
	return [super init];
}

-(jmetalbuffer *)getBufferOf:(sujak::JVertexAttribute)att
{
	return buffers[att];
}

-(void)resetOffsets
{
	for(int i=0;i<JVertexAttribute_number;i++)
		[buffers[i] resetOffset];
}

-(void)loadObjectVertex:(const void * const *)data cnt:(unsigned int)cnt
{
	for(int i=0;i<JVertexAttribute_number;i++)
	{
		[buffers[i] append:data[i] len:(jmetalconstantvertextypesize[i] * cnt)];
	}
}
@end
