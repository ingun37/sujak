//
//  jskeleton.cpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 3..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#include "jskeleton.hpp"
#include "jallocator.hpp"
#include "jbinary.hpp"
#include "jcurvenode.hpp"
#include "jcurve.hpp"
jskeleton::jskeleton()
{
	jointnum = 0;
	joints = NULL;
	table = NULL;
	inverseTable = NULL;
}

typedef jallocator<matrix_float4x4, 128> jallocatorMat44;
typedef jallocator<simd::float3, 128> jallocatorf3;
void jskeleton::setFromFile(char *tableBytes, char *jointBytes, char* animbytes)
{
	jointnum = *(int*)tableBytes;
	table = (int*)(tableBytes + sizeof(int));
	joints = (jjoint*)jointBytes;
    initeulers = jallocatorf3::getAvailable(jointnum);
    for(int i=0;i<jointnum;i++)
    {
        initeulers[i] = joints[i].rot.toEuler();
        //printf("rot : %f %f %f\n", initeulers[i][0]*(180/3.141592), initeulers[i][1]*(180/3.141592), initeulers[i][2]*(180/3.141592));
    }
    
	inverseTable = jallocatorMat44::getAvailable(jointnum);
	for(int i=0;i<jointnum;i++)
	{
		matrix_float4x4 m = transOfJointAt(i);
		inverseTable[i] = matrix_invert(m);
	}
    
    jbinary_janim::getInfo(animbytes, curvenodes, jointnum);
}

void jskeleton::animate(float t) const
{
    for(int i=0;i<jointnum;i++)
    {
        simd::float3 initeuler = simd::float3{0,0,0};
        for(int j=0;j<3;j++)
        {
            if(curvenodes[i].curves[j]==NULL)
                continue;
            initeuler[j] = curvenodes[i].curves[j]->evaluate(t) * (3.141592/180);
        }
        
        joints[i].rot.euler(initeuler[0], initeuler[1], initeuler[2]);
        //joints[i].rot.euler(0, 0, 0);
    }
}

matrix_float4x4 jskeleton::transOfJointAt(int i)
{
	int tmp = i;
	//TODO : optimize like start from getTransMat
	matrix_float4x4 identity = matrix_identity_float4x4;
	
	while(tmp != -1)
	{
		jjoint& j = joints[tmp];
		identity = matrix_multiply(j.getTransMat(), identity);
		tmp = table[tmp];
	}
	
	return identity;
}

matrix_float4x4 jskeleton::inverseOfJointAt(int i)
{
//TODO : make this function inline
	if(inverseTable  == NULL)
	{
		puts("no inverseTable");
		exit(1);
	}
	
	return inverseTable[i];
}