//
//  jskeleton.cpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 3..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#include "jskeleton.hpp"
#include "jallocator.hpp"



jskeleton::jskeleton()
{
	jointnum = 0;
	joints = NULL;
	table = NULL;
	inverseTable = NULL;
}

typedef jallocator<matrix_float4x4, 128> jallocatorMat44;

void jskeleton::setFromFile(char *tableBytes, char *jointBytes)
{
	jointnum = *(int*)tableBytes;
	table = (int*)(tableBytes + sizeof(int));
	joints = (jjoint*)jointBytes;
	
	inverseTable = jallocatorMat44::getAvailable(jointnum);
	for(int i=0;i<jointnum;i++)
	{
		matrix_float4x4 m = transOfJointAt(i);
		inverseTable[i] = matrix_invert(m);
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