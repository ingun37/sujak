//
//  jlinear.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 7/11/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//
#include <iostream>
#include <iomanip>
#include "jlinear.hpp"
#include "jlinearsystem.hpp"

using namespace std;
using namespace simd;
using namespace jlinear;

template <unsigned int C, unsigned int R>
void printSystem(const jlinearsystem<C, R> system)
{
    cout << fixed << setprecision(2);
    for(int i=0;i<C;i++)
    {
        for(int j=0;j<R;j++)
        {
            cout << system.getAt(i, j) << " ";
        }
        cout << endl;
    }
}

void jlinear::jlineartest(float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float a3, float b3, float c3, float d3)
{
    jlinearsystem<3, 4> system;
    
    system.setrow(0, a1, b1, c1, d1);
    system.setrow(1, a2, b2, c2, d2);
    system.setrow(2, a3, b3, c3, d3);
    
    system.reducedEchelonize();
    printSystem(system);
}

matrix_float4x4 jlinear::makePlaneDistanceK(float3 p1, float3 p2, float3 p3)
{
    jlinearsystem<3, 5> system;
    
    system.setrow(0, p1[0], p1[1], p1[2], 1, 0);
    system.setrow(1, p2[0], p2[1], p2[2], 1, 0);
    system.setrow(2, p3[0], p3[1], p3[2], 1, 0);
    
    system.reducedEchelonize();
    //ax+by+cz+d = 0
    float fvcs[4];
    
    
    for(int i=0;i<4;i++)
    {
        if(system.isElementFreeVariable(i))
        {
            fvcs[i] = 1;
            continue;
        }
        
        if(system.getNumberFreeVariableDependencyOfElement(i)>1)
            throw "impossible freevarialbe dependency number";
        
        fvcs[i] = system.getSumOfFreeVariableCoefficientOfElement(i);
    }
    
    float3 fvcs3{fvcs[0], fvcs[1], fvcs[2]};
    float mag = dot(fvcs3, fvcs3);
    
    float sqrfv;
    
    if(abs(mag) < 0.0001)
        sqrfv = 1;
    else
        sqrfv = 1.f/mag;
    
    float4 fvcs4{fvcs[0], fvcs[1], fvcs[2], fvcs[3]};
    float4 zero4{0,0,0,0};
    matrix_float4x4 m1 = matrix_from_columns(fvcs4, zero4, zero4, zero4);
    matrix_float4x4 m2 = matrix_from_rows(fvcs4, zero4, zero4, zero4);
    
    matrix_float4x4 K = (m1 * m2) * sqrfv;
    return K;
}
