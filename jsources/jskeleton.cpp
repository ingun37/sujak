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
    globals = NULL;
}

typedef jallocator<matrix_float4x4, 600> poolmat;
typedef jallocator<simd::float3, 128> jallocatorf3;
void jskeleton::setFromFile(char *tableBytes, char *jointBytes, char* animbytes)
{
	jointnum = *(int*)tableBytes;
	table = (int*)(tableBytes + sizeof(int));
	joints = (jjoint*)jointBytes;
    
    jbinary_janim::getInfo(animbytes, curvenodes, jointnum);
    
    globals = poolmat::getAvailable(jointnum);
}

void jskeleton::animate(float t) const
{
    //return;
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

typedef jallocator<jjoint, 600> jallocjoint;
void jskeleton::clone(jskeleton &skeleton)
{
    jjoint* newjoints = jallocjoint::getAvailable(jointnum);
    
    for(int i=0;i<jointnum;i++)
    {
        newjoints[i] = joints[i];
    }
    
    skeleton = *this;
    skeleton.joints = newjoints;
    skeleton.globals = poolmat::getAvailable(jointnum);
}

matrix_float4x4* jskeleton::computeglobals()
{
    //important : up->down ordered array must be guaranteed!!!!!!!!!!!!
    for(int i=0;i<jointnum;i++)
    {
        globals[i] = joints[i].getTransMat();
        if(table[i] == -1)
            continue;
        globals[i] = matrix_multiply( globals[ table[i] ] , globals[i]);
    }
    return globals;
}