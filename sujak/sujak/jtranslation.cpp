//
//  jtranslation.cpp
//  MetalVertexStreaming
//
//  Created by Build on 12/9/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#include "jtranslation.hpp"

sujak::jtranslation::jtranslation()
{
    pos = {0,0,0,1};
}

sujak::jtranslation::jtranslation(float x, float y, float z)
{
    pos = {x, y, z , 1};
}

sujak::jtranslation::jtranslation(simd::float4 _pos)
{
    pos = _pos;
}