//
//  jglTableAttributeBuffer.hpp
//  sujak
//
//  Created by ingun on 25/10/2016.
//
//

#ifndef jglTableAttributeBuffer_hpp
#define jglTableAttributeBuffer_hpp

#include "jglTableNonTransientArray.hpp"

class jglTypedBuffer;
class jglTableAttributeBuffer : jglTableNonTransientArray<jglTypedBuffer*>
{
protected:
	void init();
	virtual jglTypedBuffer* makeOf(unsigned int k);
    virtual unsigned int attributeNum() = 0;
    virtual unsigned int ofTypeSize(int attribute) = 0;
    virtual unsigned int ofReservedCnt(int attribute) = 0;
public:
	
};

#endif /* jglTableAttributeBuffer_hpp */
