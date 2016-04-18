//
//  jnode.cpp
//  MetalVertexStreaming
//
//  Created by Build on 12/7/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//
#include <strings.h>
#include <stdlib.h>
#include "jrenderobject.hpp"
#include "jskeleton.hpp"
#include "jskinner.hpp"
#include "jnode.hpp"



void jnode::computeAndStoreSkinnedPositionTo(simd::float4 *dest)
{
	if(skinner == NULL || skeleton == NULL)
	{
		puts("no skinner or skeleton");
		exit(1);
	}
	
	memset(dest, 0, sizeof(simd::float4) * renderobj->getVertexCnt());
	
	for(int i=0;i<skinner->clusterCnt;i++)
	{
		int jointidx = skinner->jointIdxs[i];

		//matrix_float4x4 inverse = skeleton->inverseOfJointAt( jointidx );
        matrix_float4x4 inverse = skinner->inverses[i];
		matrix_float4x4 trans = skeleton->transOfJointAt( jointidx );
		for(int j=0;j<skinner->linkCounts[i];j++)
		{
			int vidx = skinner->linkIdxs[ skinner->accuTable[i] + j];
			float weight = skinner->linkWeights[ skinner->accuTable[i] + j];

			//simd::float4 pos_bind = renderobj->getPositionAt(vidx);

			dest[vidx] += matrix_multiply(trans, matrix_multiply(inverse, renderobj->getPositionAt(vidx)) ) * weight;
		}
	}
}