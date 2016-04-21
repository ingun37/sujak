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
#include "jallocator.hpp"


void jnode::computeAndStoreSkinnedPositionTo(simd::float4 *dest)
{
	if(skinner == NULL || skeleton == NULL || !localsPrecomputed )
	{
		puts("no skinner or skeleton or locals not precomputed");
		exit(1);
	}
    int vcnt = renderobj->getVertexCnt();
	memset(dest, 0, sizeof(simd::float4) * vcnt);
	
	for(int i=0;i<skinner->clusterCnt;i++)
	{
		int jointidx = skinner->jointIdxs[i];

		//matrix_float4x4 inverse = skeleton->inverseOfJointAt( jointidx );
        //matrix_float4x4 inverse = skinner->inverses[i];
        //matrix_float4x4 bindmesh = skinner->bindmeshes[i];
        //matrix_float4x4 combine = matrix_multiply(inverse, bindmesh);
		matrix_float4x4 trans = skeleton->transOfJointAt( jointidx );
		for(int j=0;j<skinner->linkCounts[i];j++)
		{
            int lidx = skinner->accuTable[i] + j;
			int vidx = skinner->linkIdxs[ lidx ];
			float weight = skinner->linkWeights[ lidx ];

			dest[vidx] += matrix_multiply(trans, precomputelocalposes[lidx] ) * weight;
		}
	}
    
    for(int iv = 0;iv<vcnt;iv++)
    {
        dest[iv] = matrix_multiply(local, dest[iv]);
    }
}
typedef jallocator<jskeleton, 32> jallocskel;
typedef jallocator<jrenderobject, 32> jallocrobj;
typedef jallocator<jskinner, 32> jallocskin;
void jnode::clone(jnode &node)
{
    node = *this;
    jskeleton* newskel = jallocskel::getAvailable(1);
    jskinner* newskin = jallocskin::getAvailable(1);
    jrenderobject* newrobj = jallocrobj::getAvailable(1);
    
    skeleton->clone(*newskel);
    skinner->clone(*newskin);
    renderobj->clone(*newrobj);
    
    node.renderobj = newrobj;
    node.skinner = newskin;
    node.skeleton = newskel;
}
typedef jallocator<simd::float4, 60000> poolPrecompute;
void jnode::precomputeLocals()
{
    if(localsPrecomputed)
    {
        puts("already precomputed");
        exit(1);
    }
    
    precomputelocalposes = poolPrecompute::getAvailable( skinner->accuTable[ skinner->clusterCnt-1 ] + skinner->linkCounts[ skinner->clusterCnt-1 ] );
    for(int ci=0;ci<skinner->clusterCnt;ci++)
    {
        for(int vi=0;vi<skinner->linkCounts[ci];vi++)
        {
            int idx = skinner->accuTable[ci] + vi;
            precomputelocalposes[idx] = matrix_multiply(skinner->inverses[ci], matrix_multiply(skinner->bindmeshes[ci],
                                                                                                renderobj->getPositionAt( skinner->linkIdxs[idx] )
                                                                                               ));
        }
    }
    localsPrecomputed = true;
}