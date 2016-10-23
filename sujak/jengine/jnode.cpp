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

using namespace sujak;

void sujak::jnode::computeAndStoreSkinnedPositionTo(simd::float4 *dest)
{
	if(skinner == NULL || skeleton == NULL || !localsPrecomputed )
	{
		puts("no skinner or skeleton or locals not precomputed");
		exit(1);
	}
    int vcnt = renderobj->getVertexCnt();
	memset(dest, 0, sizeof(simd::float4) * vcnt);
	
    matrix_float4x4* jointglobals = skeleton->computeglobals();
	for(int i=0;i<skinner->clusterCnt;i++)
	{
		int jointidx = skinner->jointIdxs[i];
        matrix_float4x4 header = matrix_multiply(local, matrix_multiply(jointglobals[jointidx], precomputeClusterDependencies[i]));
		for(int j=0;j<skinner->linkCounts[i];j++)
		{
            int lidx = skinner->accuTable[i] + j;
			int vidx = skinner->linkIdxs[ lidx ];
			
			dest[vidx] += matrix_multiply( header , precomputeLinkDependencies[i][j] );
		}
	}
}
typedef jallocator<jskeleton, 80, jnode> jallocskel;
typedef jallocator<jrenderobject, 80, jnode> jallocrobj;
typedef jallocator<jskinner, 80, jnode> jallocskin;
void sujak::jnode::clone(jnode &node)
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
typedef jallocator<simd::float4*, 1024, jnode> poolpf4;
typedef jallocator<simd::float4, 60000, jnode> poolf4;
typedef jallocator<matrix_float4x4, 1024, jnode> poolM;
void sujak::jnode::precomputeLocals()//todo : share
{
    if(localsPrecomputed)
    {
        puts("already precomputed");
        exit(1);
    }
    
    precomputeClusterDependencies = poolM::getAvailable( skinner->clusterCnt);
    precomputeLinkDependencies = poolpf4::getAvailable( skinner->clusterCnt );
    
    for(int ci=0;ci<skinner->clusterCnt;ci++)
    {
        precomputeClusterDependencies[ci] = matrix_multiply(skinner->inverses[ci], skinner->bindmeshes[ci]);
        precomputeLinkDependencies[ci] = poolf4::getAvailable(skinner->linkCounts[ci]);
        for(int vi=0;vi<skinner->linkCounts[ci];vi++)
        {
            int idx = skinner->accuTable[ci] + vi;
            //precomputeLinkDependencies[ci][vi] = renderobj->getPositionAt( skinner->linkIdxs[idx] ) * skinner->linkWeights[idx];
        }
    }
    localsPrecomputed = true;
}
