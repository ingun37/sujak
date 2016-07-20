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

using namespace sujak;

typedef jallocator<simd::float4, 4000, jrenderobject> jallocf4;
typedef jallocator<simd::float2, 4000, jrenderobject> jallocf2;

sujak::jrenderobject::jrenderobject()
{
	dataset = false;
}

void sujak::jrenderobject::setData(void* p, void* n, void* c, void* u, unsigned int vc, void* i, unsigned int ic)
{
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
	
	dataset = true;
}

void sujak::jrenderobject::clone(jrenderobject &robj)
{
    robj = *this;
}
