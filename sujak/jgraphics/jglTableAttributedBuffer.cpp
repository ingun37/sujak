//
//  jglTableAttributedBuffer.cpp
//  sujak
//
//  Created by ingun on 24/10/2016.
//
//

#include "jglTableAttributedBuffer.hpp"

jglTableAttributedBuffer::jglTableAttributedBuffer()
{
	flag = 0;
}

jglAttributedBuffer* jglTableAttributedBuffer::makeOf(int attributeGroupIdx)
{
	return makeAttributedBufferOf( flagOfAttributeGroup(attributeGroupIdx) );
}

void jglTableAttributedBuffer::init()
{
	jglTableNonTransient<jglAttributedBuffer*>::initOrdered(attributeGroupNumber());
}
