//
//  jmetalshader.metal
//  jmetalshader
//
//  Created by ingun on 10/5/16.
//  Copyright (c) 2016 loadcomplete. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;


#include "../../jdefinitions.hpp"

using namespace sujak;


struct VertexInput {
	float4 position [[attribute(0)]];
	float4 color [[attribute(1)]];
};

struct VertexOutput {
	float4 pos [[position]];
	float4 color;
};

vertex VertexOutput vert_color(VertexInput in [[stage_in]], constant JUniform& uni [[buffer(JBuffer_uniform)]])
{
	VertexOutput out;
	out.pos = uni.ortho * uni.view * in.position;
	out.color = in.color;
	return out;
}

fragment float4 frag_color(VertexOutput in [[stage_in]])
{
	return float4(1,0,0,1);
	return in.color;
}
