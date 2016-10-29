//
//  jglTableAttributeBufferVertex.hpp
//  sujak
//
//  Created by Ingun Jon on 26/10/2016.
//
//

#ifndef jglTableAttributeBufferVertex_hpp
#define jglTableAttributeBufferVertex_hpp

#include "jglTableAttributeBuffer.hpp"

class jglTableAttributeBufferVertex : jglTableAttributeBuffer
{
protected:
    virtual unsigned int attributeNum();
    virtual unsigned int ofTypeSize(int attribute) = 0;
    virtual unsigned int ofReservedCnt(int attribute) = 0;
};

#endif /* jglTableAttributeBufferVertex_hpp */
