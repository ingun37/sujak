//
//  jmetalindexbuffer.m
//  jmetal
//
//  Created by ingun on 9/22/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jmetalindexbuffer.hpp"
#import "jmetalconstants.hpp"
using namespace sujak;
@implementation jmetalindexbuffer

-(id)initWithDevice:(id<MTLDevice>)device
{
	buffer = [[jmetalbuffer alloc]initWithDevice:device initialSize:1024 option:MTLResourceCPUCacheModeWriteCombined];
	return [super init];
}

-(NSUInteger)indexCnt
{
	size_t unitsize = 0;
	switch(jmetalconstantindextype[jconstant_index_type])
	{
		case MTLIndexTypeUInt16:
			unitsize = sizeof(short);
			break;
		case MTLIndexTypeUInt32:
			unitsize = sizeof(int);
			break;
		default:
			[NSException raise:@"sefsef" format:@"sr;girsga"];
			break;
	}
	return buffer.offset/unitsize;
}

-(void)resetOffset
{
	[buffer resetOffset];
}

-(void)loadObjectIndex:(const void *)data cnt:(unsigned int)cnt
{
	[buffer append:data len:(cnt * jmetalconstantindextypesize[jconstant_index_type])];
}
@end
