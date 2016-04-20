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
enum JBufferIndex
{
	JBufferIndex_vertex_position,
	JBufferIndex_vertex_normal,
	JBufferIndex_vertex_color,
    JBufferIndex_vertex_uv,
	JBufferIndex_uniform,
	JBufferIndex_number,
};

enum JVertexAttribute
{
	JVertexAttribute_position,
	JVertexAttribute_normal,
	JVertexAttribute_color,
    JVertexAttribute_uv,
	JVertexAttribute_number,
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

enum JRenderState
{
	JRenderState_ui,
	JRenderState_light,
	JRenderState_info,
	JRenderState_number
};
#endif /* jdefinitions_h */
