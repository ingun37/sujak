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
    enum JDataType
    {
        JDataType_None,
        JDataType_f4,
        JDataType_f2,
        JDataType_f44,
		JDataType_uint16,
		JDataType_uint32,
        JDataType_number,
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
	
	enum JInstanceAttribute
	{
		JInstanceAttribute_trans,
		JInstanceAttribute_number,
	};
	
    enum JBuffer
    {
        JBuffer_vertex_position,
        JBuffer_vertex_normal,
        JBuffer_vertex_color,
        JBuffer_vertex_uv,
		JBuffer_instance,
        JBuffer_index,
        JBuffer_uniform,
        JBuffer_number,
    };
	
    enum JFunctionVertex
    {
		JFunctionVertex_color,
        JFunctionVertex_light,
        JFunctionVertex_ui,
        JFunctionVertex_line,
        JFunctionVertex_number,
    };
    
    enum JFunctionFragment
    {
		JFunctionFragment_color,
        JFunctionFragment_light,
        JFunctionFragment_ui,
        JFunctionFragment_line,
        JFunctionFragment_number,
    };
	
	enum JShader
	{
		JShader_color,
		JShader_number
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
	
	enum JAttributeGroupInstance
	{
		JAttributeGroupInstance_trans,
		JAttributeGroupInstance_number,
	};
	
	enum JAttributeGroupVertex
	{
		JAttributeGroupVertex_position_color,
		JAttributeGroupVertex_number,
	};
    
	typedef struct _JUniform
	{
		matrix_float4x4 proj;
		matrix_float4x4 ortho;
		matrix_float4x4 view;
	}JUniform;
    
    enum JMemoryOption
    {
        JMemoryOption_managed_cpuwrite,
        JMemoryOption_number,
    };
}
#endif /* jdefinitions_h */
