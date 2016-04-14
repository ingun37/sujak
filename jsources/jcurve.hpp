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

enum JCURVEINTERPOLATION { JCURVEINTERPOLATION_CONSTANT, JCURVEINTERPOLATION_LINEAR, JCURVEINTERPOLATION_CUBIC };
class jcurve
{
public:
    int cnt;
    
    float* times;
    float* values;
    JCURVEINTERPOLATION *interpolations;
    simd::float2 *tangents_r;
    simd::float2 *tangents_l;
    
    float evaluate(float at);
    float getTimeInterval(float ratio);
};
#endif /* jcurve_hpp */
