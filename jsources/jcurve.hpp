//
//  jcurve.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/8/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jcurve_hpp
#define jcurve_hpp

#include <stdio.h>
#include <simd/simd.h>

class jcurve
{
    unsigned long cnt;
    float* times;
    float* values;
    simd::float2 *tangents_r;
    simd::float2 *tangents_l;
    
public:
    float evaluate(float at);
};
#endif /* jcurve_hpp */
