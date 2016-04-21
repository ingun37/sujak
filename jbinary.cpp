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
void jbinary_jmesh::getInfo(char* data, int &vcnt, int &icnt, simd::float4* &poolPos, simd::float4* &poolNormal, simd::float2* &pooluv, int* &poolIdx)
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
	
    pooluv = (simd::float2*)fp;
    fp += sizeof(simd::float2) * vcnt;
    
	poolIdx = (int*)fp;
}

void jbinary_jmesh::saveInfo(int &vcnt, int &icnt, simd::float4 *poolPos, simd::float4 *poolNormal, int *poolIdx)
{
	
}
typedef jallocator<int, 5000> pooli;
void jbinary_jskinner::getInfo(char *data, int &clustercnt, matrix_float4x4* &inverses, matrix_float4x4* &bindmeshes, int *&jointidxs, int *&cpcounts, int *&cpIdxs, float *&weights, int *&accuTable)
{
	if(data == NULL)
	{
		puts("fuck");
		exit(1);
	}
	char* fp = data;
	clustercnt = *((int*)fp);
	fp += sizeof(int);
    
    jointidxs = (int*)fp;
    fp += sizeof(int) * clustercnt;
    
    inverses = (matrix_float4x4*)fp;
    fp += sizeof(matrix_float4x4) * clustercnt;
	
    bindmeshes = (matrix_float4x4*)fp;
    fp += sizeof(matrix_float4x4) * clustercnt;
    
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
    /*
    float* test = new float[2000];
    memset(test,0,sizeof(float)*2000);
    int testmax = 0;
    for(int i=0;i<clustercnt;i++)
    {
        int testcnt = cpcounts[i];
        for(int j=0;j<testcnt;j++)
        {
            int testidx = cpIdxs[accuTable[i] + j];
            test[testidx] += weights[accuTable[i] + j];
            printf("testing....%f\n",test[testidx]);
            if(testidx > testmax)
                testmax = testidx;
        }
    }
    printf("test max : %d\n", testmax);
    for(int i=0;i<=testmax;i++)
    {
        if(abs(test[i] - 1) > 0.0000005)
        {
            printf("fuck %f ", test[i]);
        }
    }
     */
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
                curvenodes[i].curves_r[j] = NULL;
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
            
            curvenodes[i].curves_r[j] = curve;
        }
    }
}