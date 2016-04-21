//
//  jrenderobject.cpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 3..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#include "jrenderobject.hpp"
#include "jallocator.hpp"
#include <string.h>
#include <stdlib.h>

typedef jallocator<simd::float4, 4000> jallocf4;
typedef jallocator<simd::float2, 4000> jallocf2;

jrenderobject::jrenderobject()
{
	dataset = false;
	layedout = false;
}

void jrenderobject::setData(simd::float4 *p, simd::float4 *n, simd::float4 *c, simd::float2 *u, int vc, int *i, int ic)
{
    dataset = true;
 
    if(p==NULL)
    {
        puts("no positions");
        exit(1);
    }
    attributeDatas[JVertexAttribute_position] = p;
    if(n==NULL)
    {
        n = jallocf4::getAvailable(vc);
    }
    attributeDatas[JVertexAttribute_normal] = n;
    if(c==NULL)
    {
        c = jallocf4::getAvailable(vc);
    }
    attributeDatas[JVertexAttribute_color] = c;
    
    if(u==NULL)
    {
        u = jallocf2::getAvailable(vc);
    }
    attributeDatas[JVertexAttribute_uv] = u;
    
    if(i==NULL)
    {
        puts("no indices");
        exit(1);
    }
    
    indices = i;
    
    if(vc == 0 || ic == 0)
    {
        puts("wrong data data ddd");
        
    }
    vertexCnt = vc;
    indexCnt = ic;
}

void jrenderobject::clone(jrenderobject &robj)
{
    robj = *this;
    robj.layedout = false;
}