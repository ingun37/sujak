//
//  definitions.h
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 2..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#ifndef jdefinitions_h
#define jdefinitions_h
#include <simd/simd.h>

namespace sujak
{
    enum JDataTypeVertex
    {
        JDataTypeVertex_None,
        JDataTypeVertex_f4,
        JDataTypeVertex_f2,
        JDataTypeVertex_f44,
        JDataTypeVertex_number,
    };
	
	enum JDataTypeIndex
	{
		JDataTypeIndex_16,
		JDataTypeIndex_32,
		JDataTypeIndex_number,
	};
	
	enum JPixelFormat
	{
		JPixelFormat_F32,
		JPixelFormat_8,
		JPixelFormat_rgba8,
		JPixelFormat_rgbaf32,
		JPixelFormat_number,
	};
    enum JVertexAttribute
    {
        JVertexAttribute_position,
        JVertexAttribute_normal,
        JVertexAttribute_color,
        JVertexAttribute_uv,
        JVertexAttribute_number,
    };
	
	
	
	
    enum JBuffer
    {
        JBuffer_vertex_position,
        JBuffer_vertex_normal,
        JBuffer_vertex_color,
        JBuffer_vertex_uv,
        JBuffer_index,
        JBuffer_uniform,
        JBuffer_number,
    };
	
    enum JShaderVert
    {
		JShaderVert_color,
        JShaderVert_light,
        JShaderVert_ui,
        JShaderVert_line,
        JShaderVert_number,
    };
    
    enum JShaderFrag
    {
		JShaderFrag_color,
        JShaderFrag_light,
        JShaderFrag_ui,
        JShaderFrag_line,
        JShaderFrag_number,
    };
	
	enum JPipeline
	{
		JPipeline_color,
		JPipeline_number
	};
	
    enum JDepthCompare
    {
        JDepthCompare_less,
        JDepthCompare_always,
        JDepthCompare_number,
    };
    
    enum JRenderStateColor
    {
		JRenderStateColor_color,
        JRenderStateColor_light,
        JRenderStateColor_ui,
        JRenderStateColor_line,
        JRenderStateColor_number,
    };
    
    enum JRenderStateDepthStencil
    {
        JRenderStateDepthStencil_write,
        JRenderStateDepthStencil_ignore,
        JRenderStateDepthStencil_number,
    };
    
	
	
    enum JRenderPrimitive
    {
        JRenderPrimitive_triangle,
        JRenderPrimitive_line,
        JRenderPrimitive_number
    };
	
	typedef struct _JUniform
	{
		matrix_float4x4 proj;
		matrix_float4x4 ortho;
		matrix_float4x4 view;
	}JUniform;
}
#endif /* jdefinitions_h */