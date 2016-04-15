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
void jbinary_jskinner::getInfo(char *data, int &clustercnt, int *&jointidxs, int *&cpcounts, int *&cpIdxs, float *&weights, int *&accuTable)
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
    unsigned char* keycnts = (unsigned char*)fp;
    fp += 3*sizeof(unsigned char)*skelcnt;
    
    curvenodes = poolcurvenode::getAvailable(skelcnt);
    
    for(int i=0;i<skelcnt;i++)
    {
        memcpy(curvenodes[i].keycnt, &keycnts[i*3], sizeof(unsigned char)*3);
        //printf("keycnt : %d %d %d\n", curvenodes[i].keycnt[0], curvenodes[i].keycnt[1], curvenodes[i].keycnt[2]);
    }
    
    for(int i=0;i<skelcnt;i++)
    {
        for(int j=0;j<3;j++)
        {
            if (curvenodes[i].keycnt[j]==0)
            {
                curvenodes[i].curves[j] = NULL;
                continue;
            }
            
            jcurve *curve = poolcurve::getAvailable(1);
            
            curve->cnt = curvenodes[i].keycnt[j];
            
            curve->interpolations = (JCURVEINTERPOLATION*)fp;
            fp += sizeof(JCURVEINTERPOLATION) * curvenodes[i].keycnt[j];
            
            curve->times = (float*)fp;
            fp += sizeof(float) * curvenodes[i].keycnt[j];
            
            curve->values = (float*)fp;
            fp += sizeof(float) * curvenodes[i].keycnt[j];
            
            curve->tangents_l = (simd::float2*)fp;
            fp += sizeof(simd::float2) * curvenodes[i].keycnt[j];
            
            curve->tangents_r = (simd::float2*)fp;
            fp += sizeof(simd::float2) * curvenodes[i].keycnt[j];
            
            
            
            curvenodes[i].curves[j] = curve;
            
        }
    }
    /*
    for(int i=0;i<skelcnt;i++)
    {
        for(int j=0;j<3;j++)
        {
            jcurve* curve = curvenodes[i].curves[j];
            if(curve == NULL)
            {
                if (keycnts[i*3 + j]!= 0)
                {
                    printf("curve err\n");
                    exit(1);
                }
                continue;
            }
            else
            {
                if(keycnts[i*3 + j] == 0)
                {
                    printf("curve err..\n");
                    exit(1);
                }
            }
            
            if(curve->cnt==0)
            {
                puts("zero keycnt");
                exit(1);
            }
            float first_right = curve->tangents_r[0][0];
            float last_left = curve->tangents_l[curve->cnt-1][0];
            
            if(first_right < 0 || last_left < 0)
            {
                puts("curve err2");
                exit(1);
            }
            
            //printf("%f %f\n", first_right, last_left);
        }
    }
     */
}