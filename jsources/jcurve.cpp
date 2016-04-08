//
//  jcurve.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/8/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jcurve.hpp"

simd::float4x4 hermite(
                       (simd::float4){1, 0,  0,  0},
                       (simd::float4){0, 0,  1,  0},
                       (simd::float4){-3,3,  -2, -1},
                       (simd::float4){2, -2, 1,  1}
);
float jcurve::evaluate(float at)
{
    //todo optimize
    
    int start=0;
    for(;start<cnt;start++)
    {
        if(times[start] > at)
        {
            break;
        }
    }
    
    if (start == cnt-1)
    {
        return values[start];
    }
    
    --start;
    
    float t1 = times[start];
    float t2 = times[start+1];
    
    float t = (t2-t1)/(at-t1);
    
    simd::float4 T = {1,t,t*t,t*t*t};
    
    float v1 = values[start];
    float v2 = values[start+1];
    
    matrix_float4x2 G = {
        (simd::float2){t1, v1},
        (simd::float2){t2, v2},
        tangents_r[start],
        tangents_l[start + 1],
    };
    
    simd::float2 eval = matrix_multiply(G, matrix_multiply(hermite, T));
    
    return eval[1];
}