//
//  jglTableAttributedBuffer.hpp
//  sujak
//
//  Created by ingun on 24/10/2016.
//
//

#ifndef jglTableAttributedBuffer_hpp
#define jglTableAttributedBuffer_hpp
#include "jglTableNonTransient.hpp"
class jglAttributedBuffer;
class jglTableAttributedBuffer : public jglTableNonTransient<jglAttributedBuffer*>
{
	int flag;
protected:
	virtual unsigned int attributeGroupNumber() = 0;
	virtual jglAttributedBuffer* makeAttributedBufferOf(int flag) = 0;
	virtual int flagOfAttributeGroup(int attributeGroupIdx) = 0;
	
	jglTableAttributedBuffer();
	jglAttributedBuffer* makeOf(int attributeGroupIdx);
	void init();
};

#endif /* jglTableAttributedBuffer_hpp */
