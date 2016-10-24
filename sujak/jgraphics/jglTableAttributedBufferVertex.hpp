//
//  jglTableAttributedBufferVertex.hpp
//  sujak
//
//  Created by ingun on 24/10/2016.
//
//

#ifndef jglTableAttributedBufferVertex_hpp
#define jglTableAttributedBufferVertex_hpp

#include "jglTableAttributedBuffer.hpp"

class jglTableAttributedBufferVertex : public jglTableAttributedBuffer
{
protected:
	virtual unsigned int attributeGroupNumber();
	virtual int flagOfAttributeGroup(int attributeGroupIdx);
};

#endif /* jglTableAttributedBufferVertex_hpp */
