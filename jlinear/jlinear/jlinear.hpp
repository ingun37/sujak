/*
 *  jlinear.hpp
 *  jlinear
 *
 *  Created by Ingun Jon on 7/17/16.
 *  Copyright © 2016 Ingun Jon. All rights reserved.
 *
 */

#ifndef jlinear_
#define jlinear_

/* The classes below are exported */
#pragma GCC visibility push(default)
#include <simd/simd.h>

namespace jlinear
{
    extern void getReducedEchelonMatrix34(float m[3][4], simd::float4 v1, simd::float4 v2, simd::float4 v3);
    extern matrix_double4x4 makePlaneDistanceK(simd::double3 p1, simd::double3 p2, simd::double3 p3);
}

#pragma GCC visibility pop
#endif
