//
//  jmetalnontransients.m
//  jmetal
//
//  Created by ingun on 9/30/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include <vector>
#import "jmetalnontransients.hpp"
#import "jmetaltransients.hpp"
#import "jmetaldefinitions.hpp"


using namespace sujak;



@implementation jmetalnontransients

id<MTLFunction> frags[JShaderFrag_number] = {nil, };
+(id<MTLFunction>)functionWithLibrary:(id<MTLLibrary>)library frag:(JShaderFrag) s
{
	if(frags[s] == nil)
		frags[s] = ;
	return frags[s];
}

id<MTLFunction> verts[JShaderVert_number] = {nil, };
+(id<MTLFunction>)functionWithLibrary:(id<MTLLibrary>)library vert:(JShaderVert) s
{
	NSLog(@"vert names : %@", [library functionNames]);
	if(verts[s] == nil)
		verts[s] = ;
    return verts[s];
}

id<MTLRenderPipelineState> pipelines[JPipeline_number] = {nil, };
+(id<MTLRenderPipelineState>)pipelineWithDevice:(id<MTLDevice>)device library:(id<MTLLibrary>)library of:(JPipeline)p
{
	if(pipelines[p] != nil)
		return pipelines[p];
    
}

id<MTLTexture> texd = nil;
+(id<MTLTexture>)defaultDepthTexWithDevice:(id<MTLDevice>)device width:(NSUInteger)w height:(NSUInteger)h format:(MTLPixelFormat)f
{
	if(texd  == nil)
	{
		MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:f width:w height:h mipmapped:NO];
		desc.textureType = MTLTextureType2D;
		desc.sampleCount = 1;
		
		texd = [device newTextureWithDescriptor:desc];
	}
	return texd;
}
id<MTLTexture> texs = nil;
+(id<MTLTexture>)defaultStencilTexWithDevice:(id<MTLDevice>)device width:(NSUInteger)w height:(NSUInteger)h  format:(MTLPixelFormat)f
{
	if(texs  == nil)
	{
		MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:f width:w height:h mipmapped:NO];
		desc.textureType = MTLTextureType2D;
		desc.sampleCount = 1;
		texs = [device newTextureWithDescriptor:desc];
	}
	return texs;
}
@end
