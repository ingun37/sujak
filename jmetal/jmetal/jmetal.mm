//
//  jmetal.m
//  jmetal
//
//  Created by ingun on 7/20/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>
#import "jmetal.h"

using namespace sujak;

class jrendercontextpair
{
    bool inited;
public:
    id<MTLRenderPipelineState> color;
    id<MTLDepthStencilState> depthstencil;
    bool isInited(){ return inited; }
    void setPair(id<MTLRenderPipelineState> _color, id<MTLDepthStencilState> _depth)
    {
        if(color != nil || depthstencil != nil || inited == true)
        [NSException raise:@"render" format:@"reinitializing renderContext"];
        
        color = _color;
        depthstencil = _depth;
        inited = true;
    }
    jrendercontextpair()
    {
        inited = false;
        color = nil;
        depthstencil = nil;
    }
};

jrendercontextpair renderContextPairs[JRenderState::JRenderState_number];

jmetal::jmetal()
{
    renderEncoder = nil;
    primitiveTypeCurrent = MTLPrimitiveTypeTriangle;
    indexBuffer = nil;
}

Class jmetal::layerClass()
{
    return [CAMetalLayer class];
}

void jmetal::preRender()
{
    
}

void jmetal::renderIndexed(unsigned long offset, unsigned long cnt)
{
    if(renderEncoder == nil)
    [NSException raise:@"render" format:@"no encoder trying to draw"];
    
    [renderEncoder drawIndexedPrimitives:primitiveTypeCurrent indexCount:cnt indexType:MTLIndexTypeUInt32 indexBuffer:indexBuffer indexBufferOffset:offset * sizeof(int)];
}

void jmetal::setPrimitive(sujak::JRenderPrimitive prim)
{
    switch (prim)
    {
        case JRenderPrimitive_triangle:
        primitiveTypeCurrent = MTLPrimitiveTypeTriangle;
        break;
        case JRenderPrimitive_line:
        primitiveTypeCurrent = MTLPrimitiveTypeLine;
        break;
        default:
        [NSException raise:@"render" format:@"unexpected primitivetype"];
        break;
    }
}

void jmetal::setRenderState(sujak::JRenderState state)
{
    if(renderEncoder == nil)
    [NSException raise:@"render" format:@"no renderencoder"];
    
#ifdef DEBUG
    for(int i=0;i<JRenderState::JRenderState_number;i++)
    if(!renderContextPairs[i].isInited())
    [NSException raise:@"render" format:@"one of rendercontexst is not inited"];
    
#endif
    [renderEncoder setRenderPipelineState:renderContextPairs[state].color];
    [renderEncoder setDepthStencilState:renderContextPairs[state].depthstencil];
}