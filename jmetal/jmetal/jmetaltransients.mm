//
//  jmetaltransients.m
//  jmetal
//
//  Created by ingun on 9/30/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jmetaltransients.hpp"
#import "jmetaldefinitions.hpp"
@implementation jmetaltransients

+(MTLRenderPassDescriptor *)newRenderPass:(id<MTLTexture>)color depth:(id<MTLTexture>)depth stencil:(id<MTLTexture>)stencil
{
	if(color == nil)
		[NSException raise:@"color is null" format:@"sef"];
	
	MTLRenderPassDescriptor* pass = [MTLRenderPassDescriptor new];
	
	pass.colorAttachments[0].texture = color;
	pass.colorAttachments[0].clearColor = MTLClearColorMake(0.1, 1, 0.1, 1);
	pass.colorAttachments[0].loadAction = MTLLoadActionClear;
	if(depth != nil)
	{
		pass.depthAttachment.clearDepth = 1;
		pass.depthAttachment.texture = depth;
	}
	
	if(stencil != nil)
	{
		pass.stencilAttachment.texture = stencil;
	}
	
	return pass;
}

+(MTLVertexDescriptor*) newVertexDescWithCnt:(NSUInteger)cnt buffidxs:(NSUInteger*)buffidxs vertexformats:(MTLVertexFormat*)vformats stride:(unsigned int*)strides
{
    if(cnt == 0)
        [NSException raise:@"cnt is 0" format:@""];
    
	

	NSLog(@"vertex description : %@", desc.description);
	
	return desc;
}
@end
