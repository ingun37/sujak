//
//  jmetalindexbuffer.m
//  jmetal
//
//  Created by ingun on 9/22/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jmetalindexbuffer.hpp"
#import "jmetaldefinitions.hpp"
using namespace sujak;
@implementation jmetalindexbuffer

-(id)initWithDevice:(id<MTLDevice>)device reserveIndexCnt:(unsigned int)reserveIndexCnt
{
	unsigned long reserveSize = jmetalconstant_indextype_size(jconstant_index_datatype) * reserveIndexCnt;
	_buffer = [[jmetalbuffer alloc]initWithDevice:device initialSize:reserveSize option:MTLResourceCPUCacheModeWriteCombined];
	return [super init];
}

-(void)append:(void *)data cnt:(unsigned int)cnt
{
	_indexCnt += cnt;
	unsigned int size = jmetalconstant_indextype_size(jconstant_index_datatype);
	[_buffer append:data len:(size * cnt)];
}
-(unsigned int)getUnitSize
{
	return jmetalconstant_indextype_size( jconstant_index_datatype );
}
-(void)reset
{
	[_buffer reset];
	_indexCnt = 0;
}
@end
