//
//  jskinner.hpp
//  MetalVertexStreaming
//
//  Created by Build on 12/17/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef jskinner_hpp
#define jskinner_hpp

#include <stdio.h>
#include <simd/simd.h>
class jskinner
{
public:
	int clusterCnt;
    matrix_float4x4 *inverses;
	int* jointIdxs;
	int* linkCounts;
	int* accuTable;
	int* linkIdxs;
	float* linkWeights;
	jskinner()
	{
		clusterCnt = 0;
		jointIdxs = NULL;
		linkCounts = NULL;
		accuTable = NULL;
		linkIdxs = NULL;
		linkWeights = NULL;
	}
};

#endif /* jskinner_hpp */
