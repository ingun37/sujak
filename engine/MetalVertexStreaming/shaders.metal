/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 Metal basic pass through shader
 */

#include <metal_stdlib>
#include "jdefinitions.h"
using namespace metal;
using namespace sujak;
struct vertexinput
{
	float4 position [[attribute(0)]];
	float4 normal [[attribute(1)]];
	float4 color   [[attribute(2)]];
    float2 uv   [[attribute(3)]];
};

struct VertexInOut
{
    float4  position [[position]];
	float3  normal;
    float4  color;
    float2  uv;
};

vertex VertexInOut vertLight(vertexinput in [[stage_in]], constant JUniformBlock& uniformblock[[buffer(JBufferIndex_uniform)]])
{
    VertexInOut outVertex;
    
    outVertex.position = (uniformblock.projview) * in.position;
	outVertex.normal = in.normal.xyz;
    outVertex.color    = in.color;
    outVertex.uv = in.uv;
    
    return outVertex;
};

fragment half4 fragLight(VertexInOut inFrag [[stage_in]], texture2d<half> tex2D [[ texture(0) ]])
{
	float3 lightdir = normalize(float3(0.5,-1,0.3));
	
	float el = max((float)dot(lightdir, inFrag.normal),0.f);
	
    constexpr sampler quad_sampler;
    
    half4 color = tex2D.sample(quad_sampler, inFrag.uv);
    
    color.xyz = color.xyz * el;
    
    return color;
};

vertex VertexInOut vertLine(vertexinput in [[stage_in]], constant JUniformBlock& uniformblock[[buffer(JBufferIndex_uniform)]])
{
	VertexInOut outVertex;
	
	outVertex.position = (uniformblock.projview) * in.position;
	outVertex.normal = in.normal.xyz;
	outVertex.color    = in.color;
	
	
	return outVertex;
};

fragment float4 fragLine(VertexInOut inFrag [[stage_in]])
{
	return inFrag.color;
};

vertex VertexInOut vertUI(vertexinput in [[stage_in]], constant JUniformBlock& uniformblock[[buffer(JBufferIndex_uniform)]])
{
	VertexInOut outVertex;
	outVertex.position = (uniformblock.orthoview) * in.position;
	return outVertex;
}

fragment float4 fragUI(VertexInOut inFrag [[stage_in]])
{
	return float4(0.3f,0.3f,0.1f,1);
}