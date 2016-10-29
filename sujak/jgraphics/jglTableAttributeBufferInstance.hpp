//
//  jglTableAttributeBufferInstance.hpp
//  sujak
//
//  Created by Ingun Jon on 26/10/2016.
//
//

#ifndef jglTableAttributeBufferInstance_hpp
#define jglTableAttributeBufferInstance_hpp

#include "jglTableAttributeBuffer.hpp"

class jglTableAttributeBufferInstance : jglTableAttributeBuffer
{
protected:
    virtual unsigned int attributeNum();
    virtual unsigned int ofTypeSize(int attribute) = 0;
    virtual unsigned int ofReservedCnt(int attribute) = 0;
};

#endif /* jglTableAttributeBufferInstance_hpp */
