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

float4 jlinear::makePlaneContaining3Points(float3 p1, float3 p2, float3 p3)
{
    jlinearsystem<3, 4> system;
    
    system.setrow(0, p1[0], p1[1], p1[2], 1);
    system.setrow(1, p2[0], p2[1], p2[2], 1);
    system.setrow(2, p3[0], p3[1], p3[2], 1);
    
    system.echelonize();
    float4 aa;
    return aa;
}
