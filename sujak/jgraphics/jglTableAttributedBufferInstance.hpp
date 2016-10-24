//
//  jglTableAttributedBufferInstance.hpp
//  sujak
//
//  Created by ingun on 24/10/2016.
//
//

#ifndef jglTableAttributedBufferInstance_hpp
#define jglTableAttributedBufferInstance_hpp

#include "jglTableAttributedBuffer.hpp"

class jglTableAttributedBufferInstance : public jglTableAttributedBuffer
{
protected:
	virtual unsigned int attributeGroupNumber();
	virtual int flagOfAttributeGroup(int attributeGroupIdx);

};

#endif /* jglTableAttributedBufferInstance_hpp */
