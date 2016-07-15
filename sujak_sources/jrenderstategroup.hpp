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
