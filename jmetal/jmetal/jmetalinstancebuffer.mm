//
//  jmetalinstancebuffer.m
//  jmetal
//
//  Created by ingun on 13/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jmetalinstancebuffer.hpp"
#import "jmetaldefinitions.hpp"
@implementation jmetalinstancebuffer
-(void)append:(void **)datas cnt:(unsigned int)cnt type:(int)type
{
	if(_type != type)
		[NSException raise:@"23434hgjhj" format:@""];
	
	_num += cnt;
	for(int i=0;i<_attnum;i++)
	{
		unsigned int buffersize = jmetalconstant_datatype_size( jconstant_attribute_datatype(attribs[i]) ) * cnt;
		if(datas[i] == NULL)
			[NSException raise:@"ery45u" format:@""];
		[_buffers[i] append:datas[i] len:buffersize];
	}
}

-(sujak::JInstanceAttribute)getAttribOfBufferAt:(int)idx
{
	return attribs[idx];
}

-(unsigned int)getUnitSizeOfBufferAt:(int)idx
{
	return jmetalconstant_datatype_size( jconstant_attribute_datatype( attribs[idx] ) );
}

-(id)initWithDevice:(id<MTLDevice>)device type:(int)type reserveCnt:(unsigned int)reserveCnt
{
	if(_buffers == nil)
		_buffers = [[NSMutableArray alloc]init];
	[_buffers removeAllObjects];
	
	_type = type;
	
	jconstant_instancetype_info(type, _attnum, attribs);
	
	for(int i=0;i<_attnum;i++)
	{
		size_t initsize = jmetalconstant_datatype_size( jconstant_attribute_datatype( attribs[i] ) ) * reserveCnt;
		[_buffers addObject:[[jmetalbuffer alloc] initWithDevice:device initialSize:initsize option:MTLResourceCPUCacheModeWriteCombined]];
	}
	return [super init];
}
@end
