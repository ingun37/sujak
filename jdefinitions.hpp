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
    enum JVertexFetchStep
	{
		JVertexFecthStep_vertex,
		JVertexFecthStep_instance,
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
        JShaderVert_light,
        JShaderVert_ui,
        JShaderVert_line,
        JShaderVert_number,
    };
    
    enum JShaderFrag
    {
        JShaderFrag_light,
        JShaderFrag_ui,
        JShaderFrag_line,
        JShaderFrag_number,
    };
    
    enum JDepthCompare
    {
        JDepthCompare_less,
        JDepthCompare_always,
        JDepthCompare_number,
    };
    
    enum JRenderStateColor
    {
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
    
    enum JRenderContext
    {
        JRenderContext_light,
        JRenderContext_ui,
        JRenderContext_line,
        JRenderContext_number
    };
    
    typedef struct _JUniformBlock
    {
        simd::float4x4 projview;
        simd::float4x4 orthoview;
    } JUniformBlock;
    
    enum JRenderPrimitive
    {
        JRenderPrimitive_triangle,
        JRenderPrimitive_line,
        JRenderPrimitive_number
    };
    
    typedef struct _jconstantvertexatt
    {
        JDataTypeVertex type;
        JBuffer bufferIdx;
		JVertexFetchStep step;
    } jconstantvertexatt;
    
    typedef struct _jconstantbuffer
    {
        unsigned long reserveSize;
    } jconstantbuffer;
    
    typedef struct _jconstantrenderstatecolor
    {
        JShaderFrag shaderFrag;
        JShaderVert shaderVert;
    } jconstantrenderstatecolor;
    
    typedef struct _jconstantrenderstatedepthstencil
    {
        JDepthCompare depthCompare;
        bool depthwriteEnabled;
    } jconstantrenderstatedepthstencil;
    
    typedef struct _jconstantrendercontext
    {
        JRenderStateColor color;
        JRenderStateDepthStencil depthStencil;
    } jconstantrendercontext;
}
#endif /* jdefinitions_h */
