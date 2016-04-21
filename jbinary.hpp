//
//  jbinary.hpp
//  MetalVertexStreaming
//
//  Created by Build on 12/28/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef jbinary_hpp
#define jbinary_hpp

#include <stdio.h>
#include <simd/simd.h>
class jbinary_jmesh
{
public:
    static void getInfo(char* data, int& vcnt, int& icnt, simd::float4* &poolPos, simd::float4* &poolNormal, simd::float2* &pooluv, int* &poolIdx );
	static void saveInfo(int& vcnt, int& icnt, simd::float4* poolPos, simd::float4* poolNormal, int* poolIdx );
};

class jbinary_jskinner
{
public:
	static void getInfo(char* data, int& clustercnt, matrix_float4x4* &inverses, matrix_float4x4* &bindmeshes, int* &jointidxs, int* &cpcounts, int* &cpIdxs, float* &weights, int* &accuTable);
};

class jcurvenode;
class jbinary_janim
{
public:
    static void getInfo(char* data, jcurvenode* &curvenodes, const int skelcnt);
};

#endif /* jbinary_hpp */
