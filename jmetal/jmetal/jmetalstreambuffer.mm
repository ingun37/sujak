//
//  jmetalstreambuffer.m
//  jmetal
//
//  Created by ingun on 9/27/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//


#import "jmetalstreambuffer.hpp"
using namespace sujak;
@implementation jmetalstreambuffer

-(id)initWithDevcie:(id<MTLDevice>)device vertextype:(int)vertextype reserveVertexCnt:(unsigned int)reserveVertexCnt reserveIndexCnt:(unsigned int)reserveIndexCnt
{
	_index = [[jmetalindexbuffer alloc]initWithDevice:device reserveIndexCnt:reserveIndexCnt];
    _vertex = [[jmetalvertexbuffer alloc]initWithDevice:device vertextype:vertextype reserveVertexCnt:reserveVertexCnt];
	return [super init];
}

-(void)appendVertexData:(void **)vdatas vertextype:(int)vertextype vertexCnt:(unsigned int)vcnt indexData:(void *)idata indexCnt:(unsigned int)icnt
{
    
    [_vertex append:vdatas cnt:vcnt vertextype:vertextype];
	[_index append:idata cnt:icnt];
}

-(void)reset
{
	[_index reset];
	[_vertex reset];
}
@end
