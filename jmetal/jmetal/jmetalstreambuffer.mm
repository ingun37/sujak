//
//  jmetalstreambuffer.m
//  jmetal
//
//  Created by ingun on 9/27/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//


#import "jmetalstreambuffer.hpp"
#import "jrenderobject.hpp"
@implementation jmetalstreambuffer

-(id)initWithDevcie:(id<MTLDevice>)device
{
	index = [[jmetalindexbuffer alloc]initWithDevice:device];
	vertex = [[jmetalvertexbuffer alloc]initWithDevice:device];
	return [super init];
}

-(void)loadObject:(sujak::jrenderobject *)obj
{
	[index loadObjectIndex:obj->getIndexDataForCopy() cnt:obj->getIndexCnt()];
	[vertex loadObjectVertex:obj->getData2DForCopy() cnt:obj->getVertexCnt()];
}

-(void)resetOffsets
{
	[index resetOffset];
	[vertex resetOffsets];
}

@end
