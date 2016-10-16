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
		frags[s] = [library newFunctionWithName: jmetalconstant_shader_fragment(s)];
	return frags[s];
}

id<MTLFunction> verts[JShaderVert_number] = {nil, };
+(id<MTLFunction>)functionWithLibrary:(id<MTLLibrary>)library vert:(JShaderVert) s
{
	NSLog(@"vert names : %@", [library functionNames]);
	if(verts[s] == nil)
		verts[s] = [library newFunctionWithName: jmetalconstant_shader_vertex(s)];
    return verts[s];
}

id<MTLRenderPipelineState> pipelines[JPipeline_number] = {nil, };
+(id<MTLRenderPipelineState>)pipelineWithDevice:(id<MTLDevice>)device library:(id<MTLLibrary>)library of:(JPipeline)p
{
	if(pipelines[p] != nil)
		return pipelines[p];
    jtype_pipeline pipe = jconstant_programs(p);
	MTLRenderPipelineDescriptor* desc = [MTLRenderPipelineDescriptor new];
	
	std::vector<NSUInteger> buffidxs;
	std::vector<MTLVertexFormat> vformats;
	std::vector<unsigned int> strides;
	
    size_t attnum;
    JVertexAttribute attribs[JVertexAttribute_number];
    jconstant_vertextype_info(pipe.vtype, attnum, attribs);
    
    for(int i=0;i<attnum;i++)
    {
		buffidxs.push_back( jconstant_bufferidx_of(attribs[i]) );
		vformats.push_back( jmetalconstant_vertexformat( jconstant_attribute_datatype(attribs[i]) ));
		strides.push_back( jmetalconstant_datatype_size( jconstant_attribute_datatype(attribs[i]) ));
    }
    
    desc.vertexDescriptor = [jmetaltransients newVertexDescWithCnt:attnum buffidxs:&buffidxs[0] vertexformats:&vformats[0] stride:&strides[0]];
    
    desc.vertexFunction = [jmetalnontransients functionWithLibrary:library vert:pipe.v];
    desc.fragmentFunction = [jmetalnontransients functionWithLibrary:library frag:pipe.f];
    
    desc.colorAttachments[0].pixelFormat = jmetalconstant_pixelformat(pipe.pf_color);
    desc.sampleCount = 1;
    
    desc.depthAttachmentPixelFormat = jmetalconstant_pixelformat(pipe.pf_depth);
    desc.stencilAttachmentPixelFormat = jmetalconstant_pixelformat(pipe.pf_stencil);
	
	NSError* err = nil;
    pipelines[p] = [device newRenderPipelineStateWithDescriptor:desc error:&err];
	if(err != nil)
	{
		[NSException raise:@"pipeline create fail" format:@"%@", err];
	}
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
