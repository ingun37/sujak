//
//  jmetalvertexbuffer.m
//  jmetal
//
//  Created by ingun on 9/21/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jmetalvertexbuffer.hpp"
#import "jmetaldefinitions.hpp"
using namespace sujak;
@implementation jmetalvertexbuffer

-(id)initWithDevice:(id<MTLDevice>)device vertextype:(int)vertextype reserveVertexCnt:(unsigned int)reserveVertexCnt
{
    if(_buffers == nil)
        _buffers = [[NSMutableArray alloc]init];
    [_buffers removeAllObjects];
    
    _vertextype = vertextype;
    
    jconstant_vertextype_info(vertextype, _attnum, attribs);
    
    for(int i=0;i<_attnum;i++)
    {
        size_t initsize = jmetalconstant_vertex_datatype_size( jconstant_vertex_attribute_datatype( attribs[i] ) ) * reserveVertexCnt;
        [_buffers addObject:[[jmetalbuffer alloc] initWithDevice:device initialSize:initsize option:MTLResourceCPUCacheModeWriteCombined]];
    }
	return [super init];
}
-(unsigned int)getUnitSizeOfBufferAt:(int)idx
{
	return jmetalconstant_vertex_datatype_size( jconstant_vertex_attribute_datatype( attribs[idx] ) );
}
-(sujak::JVertexAttribute)getAttribOfBufferAt:(int)idx
{
    return attribs[idx];
}
-(void)append:(void **)datas cnt:(unsigned int)cnt vertextype:(int)vertextype
{
    if(_vertextype != vertextype)
        [NSException raise:@"23434hgjhj" format:@""];
    
    _vnum += cnt;
    for(int i=0;i<_attnum;i++)
    {
        unsigned int buffersize = jmetalconstant_vertex_datatype_size( jconstant_vertex_attribute_datatype(attribs[i]) ) * cnt;
        if(datas[i] == NULL)
            [NSException raise:@"ery45u" format:@""];
        [_buffers[i] append:datas[i] len:buffersize];
    }
	
}
-(void)reset
{
	for(jmetalbuffer *b in _buffers)
	{
		[b reset];
	}
	_vnum = 0;
}
@end
