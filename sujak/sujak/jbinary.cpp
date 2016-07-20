#include <stdlib.h>
#include <iostream>
#include "jbinary.hpp"
#include "jallocator.hpp"
#include "jcurvenode.hpp"
#include "jcurve.hpp"
using namespace sujak;

typedef jallocator<simd::float4, 12000, jbinary_jmesh> poolf4;

using namespace std;
using namespace sujak;
void sujak::jbinary_jmesh::getInfo(char* data, int &vcnt, int &icnt, simd::float4* &poolPos, simd::float4* &poolNormal, simd::float2* &pooluv, int* &poolIdx)
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

void sujak::jbinary_jmesh::saveInfo(int &vcnt, int &icnt, simd::float4 *poolPos, simd::float4 *poolNormal, int *poolIdx)
{
	
}
typedef jallocator<int, 5000, jbinary_jskinner> pooli;

void sujak::jbinary_jskinner::getInfo(char *data, int &clustercnt, matrix_float4x4* &inverses, matrix_float4x4* &bindmeshes, int *&jointidxs, int *&cpcounts, int *&cpIdxs, float *&weights, int *&accuTable)
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
    
    printf("accu : %d\n", accu);
	
	cpIdxs = (int*)fp;
	fp += sizeof(int) * (accuTable[clustercnt-1] + cpcounts[clustercnt-1]);
	weights = (float*)fp;
    
}

typedef jallocator<jcurvenode, 1024, jbinary_janim> poolcurvenode;
typedef jallocator<jcurve, 2000, jbinary_janim> poolcurve;
void sujak::jbinary_janim::getInfo(char *data, jcurvenode *&curvenodes, const int skelcnt)
{
    char* fp = data;

    curvenodes = poolcurvenode::getAvailable(skelcnt);
    
    for(int i=0;i<skelcnt;i++)
    {
        for(int ip=0;ip<JCURVENODE_PROPERTY_NUMBER;ip++)
            for(int j=0;j<3;j++)
            {
                int cnt = *((int*)fp);
                fp += sizeof(int);
            
                if(cnt ==0)
                {
                    curvenodes[i].getcurveofproperty((JCURVENODE_PROPERTY)ip)[j] = NULL;
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
            
                curvenodes[i].getcurveofproperty((JCURVENODE_PROPERTY)ip)[j] = curve;
            }
        
    }
}
