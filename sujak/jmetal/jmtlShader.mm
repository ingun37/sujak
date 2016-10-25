//
//  jmtlShader.cpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtlShader.hpp"
#include "jconstants.hpp"
#include "jmetaldefinitions.hpp"
using namespace sujak;
jmtlShader::jmtlShader()
{
	pipelinestate = nil;
}


void jmtlShader::init(sujak::JShader p, id<MTLDevice> device, id<MTLLibrary> library)
{
	jglShader::init(p);
	const jinfo_shader& pipe = jconstant_shaders(p);
	MTLRenderPipelineDescriptor* pipelinedesc = [MTLRenderPipelineDescriptor new];
	
	std::vector<NSUInteger> buffidxs;
	std::vector<MTLVertexFormat> vformats;
	std::vector<unsigned int> strides;
	std::vector<MTLVertexStepFunction> stepFunctions;
	
	size_t vattnum;
	JVertexAttribute vattribs[JVertexAttribute_number];
	jconstant_attributes_of_flag(pipe.vertexFlag, vattnum, vattribs);
	
	for(int i=0;i<vattnum;i++)
	{
		buffidxs.push_back( jconstant_bufferidx_of(vattribs[i]) );
		vformats.push_back( jmetalconstant_mtldataformat( jconstant_attribute_datatype(vattribs[i]) ));
		strides.push_back( jmetalconstant_datatype_size( jconstant_attribute_datatype(vattribs[i]) ));
		stepFunctions.push_back( MTLVertexStepFunctionPerVertex);
	}
	
	size_t iattnum;
	JInstanceAttribute iattribs[JVertexAttribute_number];
	jconstant_attributes_of_flag(pipe.instanceFlag, iattnum, iattribs);
	
	for(int i=0;i<iattnum;i++)
	{
		buffidxs.push_back( jconstant_bufferidx_of(iattribs[i]) );
		vformats.push_back( jmetalconstant_mtldataformat( jconstant_attribute_datatype(iattribs[i]) ));
		strides.push_back( jmetalconstant_datatype_size( jconstant_attribute_datatype(iattribs[i]) ));
		stepFunctions.push_back( MTLVertexStepFunctionPerInstance);
	}
	
	MTLVertexDescriptor *vertexdesc = [MTLVertexDescriptor new];
	
	for(int i=0;i<buffidxs.size();i++)
	{
		vertexdesc.attributes[i].bufferIndex = buffidxs[i];
		vertexdesc.attributes[i].format = vformats[i];
		vertexdesc.attributes[i].offset = 0;
		
		vertexdesc.layouts[buffidxs[i]].stride = strides[i];
		vertexdesc.layouts[buffidxs[i]].stepFunction = stepFunctions[i];
	}
	
	pipelinedesc.vertexDescriptor = vertexdesc;
	
	pipelinedesc.vertexFunction = [library newFunctionWithName: jmetalconstant_shader_vertex(pipe.v)];
	pipelinedesc.fragmentFunction = [library newFunctionWithName: jmetalconstant_shader_fragment(pipe.f)];
	
	pipelinedesc.colorAttachments[0].pixelFormat = jmetalconstant_pixelformat(pipe.pf_color);
	pipelinedesc.sampleCount = 1;
	
	pipelinedesc.depthAttachmentPixelFormat = jmetalconstant_pixelformat(pipe.pf_depth);
	pipelinedesc.stencilAttachmentPixelFormat = jmetalconstant_pixelformat(pipe.pf_stencil);
	
	NSError* err = nil;
	pipelinestate = [device newRenderPipelineStateWithDescriptor:pipelinedesc error:&err];
	if(err != nil)
	{
		[NSException raise:@"pipeline create fail" format:@"%@", err];
	}
}
