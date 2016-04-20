//
//  jrendercontext.hpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 4..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#ifndef jrendercontext_hpp
#define jrendercontext_hpp

#include <stdio.h>
#include "jconstants.h"
#include "jrenderprimitivegroup.hpp"
class jrenderstategroup
{
public:
	jrenderprimitivegroup subPrimitiveGroups[JRenderPrimitive_number];
	jrenderstategroup();
	
};
#endif /* jrendercontext_hpp */
