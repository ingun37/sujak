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
}

typedef jallocator<matrix_float4x4, 128> jallocatorMat44;
typedef jallocator<simd::float3, 128> jallocatorf3;
void jskeleton::setFromFile(char *tableBytes, char *jointBytes, char* animbytes)
{
	jointnum = *(int*)tableBytes;
	table = (int*)(tableBytes + sizeof(int));
	joints = (jjoint*)jointBytes;
    
    jbinary_janim::getInfo(animbytes, curvenodes, jointnum);
}

void jskeleton::animate(float t) const
{
    for(int i=0;i<jointnum;i++)
    {
        simd::float3 initeuler = simd::float3{0,0,0};
        for(int j=0;j<3;j++)
        {
            if(curvenodes[i].getcurveofproperty( JCURVENODE_PROPERTY_ROTATION )[j]==NULL)
                continue;
            initeuler[j] = curvenodes[i].getcurveofproperty(JCURVENODE_PROPERTY_ROTATION)[j]->evaluate(t) * (3.141592/180);
        }
        
        joints[i].rot.euler(initeuler[0], initeuler[1], initeuler[2], joints[i].order);
        
        simd::float4 initpos = joints[i].pos.getPos();
        for(int j=0;j<3;j++)
        {
            if(curvenodes[i].getcurveofproperty( JCURVENODE_PROPERTY_TRANSLATION )[j]==NULL)
                continue;
            
            initpos[j] = curvenodes[i].getcurveofproperty(JCURVENODE_PROPERTY_TRANSLATION)[j]->evaluate(t);
        }
        joints[i].pos.setPos(initpos);
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

typedef jallocator<jjoint, 400> jallocjoint;
void jskeleton::clone(jskeleton &skeleton)
{
    jjoint* newjoints = jallocjoint::getAvailable(jointnum);
    
    for(int i=0;i<jointnum;i++)
    {
        newjoints[i] = joints[i];
    }
    
    skeleton = *this;
    skeleton.joints = newjoints;
}