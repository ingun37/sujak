/*
 *  jlinear.cpp
 *  jlinear
 *
 *  Created by Ingun Jon on 7/17/16.
 *  Copyright © 2016 Ingun Jon. All rights reserved.
 *
 */

#include <iomanip>
#include "jlinear.hpp"
#include "jlinearsystem.h"

using namespace std;
using namespace simd;
using namespace jlinear;


void jlinear::getReducedEchelonMatrix34(float m[3][4], float4 v1, float4 v2, float4 v3)
{
    jlinearsystem<3, 4> system;
    
    system.setrow(0, v1[0], v1[1], v1[2], v1[3]);
    system.setrow(1, v2[0], v2[1], v2[2], v2[3]);
    system.setrow(2, v3[0], v3[1], v3[2], v3[3]);
    
    system.reducedEchelonize();
    for(int i=0;i<3;i++)
        for(int j=0;j<4;j++)
            m[i][j] = system.getAt(i, j);
}

matrix_double4x4 jlinear::makePlaneDistanceK(double3 p1, double3 p2, double3 p3)
{
    jlinearsystem<3, 5> system;
    
    system.setrow(0, p1[0], p1[1], p1[2], 1, 0);
    system.setrow(1, p2[0], p2[1], p2[2], 1, 0);
    system.setrow(2, p3[0], p3[1], p3[2], 1, 0);
    
    system.reducedEchelonize();
    //ax+by+cz+d = 0
    double fvcs[4];
    
    
    for(int i=0;i<4;i++)
    {
        if(system.isElementFreeVariable(i))
        {
            fvcs[i] = 1;
            continue;
        }
        
        if(system.getNumberFreeVariableDependencyOfElement(i)>1)
            throw "impossible freevarialbe dependency number";
        
        fvcs[i] = -system.getSumOfFreeVariableCoefficientOfElement(i);
    }
    
    double3 fvcs3{fvcs[0], fvcs[1], fvcs[2]};
    double mag = dot(fvcs3, fvcs3);
    
    double sqrfv;
    
    if(abs(mag) < 0.0001)
        throw "magnitude a,b,c cant be 0";
    
    sqrfv = ((double)1)/mag;
    
    double4 fvcs4{fvcs[0], fvcs[1], fvcs[2], fvcs[3]};
    double4 zero4{0,0,0,0};
    matrix_double4x4 m1 = matrix_from_columns(fvcs4, zero4, zero4, zero4);
    matrix_double4x4 m2 = matrix_from_rows(fvcs4, zero4, zero4, zero4);
    
    matrix_double4x4 K = (m1 * m2) * sqrfv;
    return K;
}
