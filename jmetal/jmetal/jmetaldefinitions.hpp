//
//  jmetaldefinitions.h
//  jmetal
//
//  Created by ingun on 9/30/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmetaldefinitions_h
#define jmetaldefinitions_h
#import "jconstants.hpp"

unsigned int jmetalconstant_vertex_datatype_size(sujak::JDataTypeVertex t)
{
	switch(t)
	{
		case sujak::JDataTypeVertex_f2: return sizeof(simd::float2);
		case sujak::JDataTypeVertex_f4: return sizeof(simd::float4);
		case sujak::JDataTypeVertex_f44: return sizeof(matrix_float4x4);
		default: throw "unknown type";
	}
}

unsigned int jmetalconstant_indextype_size(sujak::JDataTypeIndex t)
{
	switch (t)
	{
		case sujak::JDataTypeIndex_16: return sizeof(unsigned short);
		case sujak::JDataTypeIndex_32: return sizeof(unsigned int);
		default: throw "unknown index type";
	}
}

MTLPixelFormat jmetalconstant_pixelformat(sujak::JPixelFormat f)
{
	switch(f)
	{
		case sujak::JPixelFormat_8: return MTLPixelFormatStencil8;
		case sujak::JPixelFormat_F32: return MTLPixelFormatDepth32Float;
		case sujak::JPixelFormat_rgba8: return MTLPixelFormatBGRA8Unorm;
		case sujak::JPixelFormat_rgbaf32: return MTLPixelFormatRGBA32Float;
		default: throw "unknown pixlformat";
	}
}

MTLVertexFormat jmetalconstant_vertexformat(sujak::JDataTypeVertex t)
{
    switch(t)
    {
        case sujak::JDataTypeVertex_f2: return MTLVertexFormatFloat2;
        case sujak::JDataTypeVertex_f4: return MTLVertexFormatFloat4;
        default:
            throw "unknown data type fo sss";
    }
}

MTLIndexType jmetalconstant_indextype(sujak::JDataTypeIndex t)
{
    switch(t)
    {
        case sujak::JDataTypeIndex_16: return MTLIndexTypeUInt16;
        case sujak::JDataTypeIndex_32: return MTLIndexTypeUInt32;
        default:
            throw "9292t2493t2490t904";
    }
}

NSString* jmetalconstant_shader_vertex(sujak::JShaderVert s)
{
    switch(s)
    {
        case sujak::JShaderVert_color: return @"vert_color";
        default: throw "unknonw shadwseifjsiefsef";
    }
}

NSString* jmetalconstant_shader_fragment(sujak::JShaderFrag s)
{
    switch(s)
    {
        case sujak::JShaderFrag_color: return @"frag_color";
        default: throw "unknown shei89999999999";
    }
}

#endif /* jmetaldefinitions_h */
