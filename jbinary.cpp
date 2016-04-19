//
//  jbinary.cpp
//  MetalVertexStreaming
//
//  Created by Build on 12/28/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include "jbinary.hpp"
#include "jallocator.hpp"
#include "jcurvenode.hpp"
#include "jcurve.hpp"
typedef jallocator<simd::float4, 12000> poolf4;

using namespace std;
void jbinary_jmesh::getInfo(char* data, int &vcnt, int &icnt, simd::float4* &poolPos, simd::float4* &poolNormal, int* &poolIdx)
{
	if(data == NULL)
	{
		puts("no!!");
		exit(1);
	}
	char* fp = data;
	
	vcnt = *((int*)fp);
	fp += sizeof(int);
	icnt = *((int*)fp);
	fp += sizeof(int);
	
	cout << "vnum : " << vcnt << ", inum : " << icnt << endl;

	poolPos = (simd::float4*)fp;
	fp += sizeof(simd::float4) * vcnt;
	
	poolNormal = (simd::float4*)fp;
	fp += sizeof(simd::float4) * vcnt;
	
	poolIdx = (int*)fp;
}

void jbinary_jmesh::saveInfo(int &vcnt, int &icnt, simd::float4 *poolPos, simd::float4 *poolNormal, int *poolIdx)
{
	
}
typedef jallocator<int, 5000> pooli;
void jbinary_jskinner::getInfo(char *data, int &clustercnt, matrix_float4x4* &inverses, int *&jointidxs, int *&cpcounts, int *&cpIdxs, float *&weights, int *&accuTable)
{
	if(data == NULL)
	{
		puts("fuck");
		exit(1);
	}
	char* fp = data;
	clustercnt = *((int*)fp);
	fp += sizeof(int);
    inverses = (matrix_float4x4*)fp;
    fp += sizeof(matrix_float4x4) * clustercnt;
	jointidxs = (int*)fp;
	fp += sizeof(int) * clustercnt;
	cpcounts = (int*)fp;
	fp += sizeof(int) * clustercnt;
	
	accuTable = pooli::getAvailable(clustercnt);
	int accu = 0;
	for(int i=0;i<clustercnt;i++)
	{
		accuTable[i] = accu;
		accu += cpcounts[i];
	}
	
	cpIdxs = (int*)fp;
	fp += sizeof(int) * (accuTable[clustercnt-1] + cpcounts[clustercnt-1]);
	weights = (float*)fp;
}

typedef jallocator<jcurvenode, 1024> poolcurvenode;
typedef jallocator<jcurve, 2000> poolcurve;
void jbinary_janim::getInfo(char *data, jcurvenode *&curvenodes, const int skelcnt)
{
    char* fp = data;

    curvenodes = poolcurvenode::getAvailable(skelcnt);
    
    for(int i=0;i<skelcnt;i++)
    {
        for(int j=0;j<3;j++)
        {
            int cnt = *((int*)fp);
            fp += sizeof(int);
            
            if(cnt ==0)
            {
                curvenodes[i].curves[j] = NULL;
                continue;
            }
            
            jcurve *curve = poolcurve::getAvailable(1);
            
            curve->cnt = cnt;
            
            curve->interpolations = (JCURVEINTERPOLATION*)fp;
            fp += sizeof(JCURVEINTERPOLATION) * cnt;
            
            curve->tangents_l = (simd::float2*)fp;
            fp += sizeof(simd::float2) * cnt;
            
            curve->tangents_r = (simd::float2*)fp;
            fp += sizeof(simd::float2) * cnt;
            
            curve->times = (float*)fp;
            fp += sizeof(float) * cnt;
            
            curve->values = (float*)fp;
            fp += sizeof(float) * cnt;
            
            curvenodes[i].curves[j] = curve;
        }
    }
}