#ifndef jrendercontext_hpp
#define jrendercontext_hpp

#include <stdio.h>
#include "jconstants.hpp"
#include "jrenderprimitivegroup.hpp"
namespace sujak
{
    class jrenderstategroup
    {
    public:
        sujak::jrenderprimitivegroup subPrimitiveGroups[JRenderPrimitive_number];
        jrenderstategroup();
        
    };
}
#endif /* jrendercontext_hpp */
