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

float jcurve::getTimeInterval(float ratio)
{
    return (ratio*times[cnt-1]) + ((1-ratio)*times[0]);
}
float jcurve::evaluate(float at)
{
    //todo optimize
    
    if (at > times[cnt-1])
    {
        return values[cnt-1];
    }
    else if(at < times[0])
    {
        return values[0];
    }
    
    //todo : make it to O(1)
    int start=0;
    for(;start<cnt;start++)
    {
        if(times[start] > at)
        {
            break;
        }
    }
    
    --start;
    
    simd::float2 eval;
    float t1 = times[start];
    float t2 = times[start+1];
    
    float t;
    
    simd::float4 T;
    
    float v1;
    float v2;
    
    matrix_float4x2 G;
    
    switch(interpolations[start])
    {
        case JCURVEINTERPOLATION_CUBIC:
            t1 = times[start];
            t2 = times[start+1];
            
            //todo : no float division. (use bresenham algorithm instead)
            t = ((float)(at-t1))/(t2-t1);
            
            T = {1,t,t*t,t*t*t};
            
            v1 = values[start];
            v2 = values[start+1];
            
            G = {
                (simd::float2){static_cast<float>(t1), v1},
                (simd::float2){static_cast<float>(t2), v2},
                tangents_r[start],
                tangents_l[start + 1],
            };
            
            eval = matrix_multiply(G, matrix_multiply(hermite, T));
            break;
        case JCURVEINTERPOLATION_CONSTANT:
            eval[1] = values[start];
            break;
        case JCURVEINTERPOLATION_LINEAR:
            t = ((float)(at-t1))/(t2-t1);
            v1 = values[start];
            v2 = values[start+1];
            eval[1] = (v2*t) + (v1*(1-t));
            break;
    }
    
    
    return eval[1];
}