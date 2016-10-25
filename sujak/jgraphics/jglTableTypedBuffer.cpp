//
//  jglTableTypedBuffer.cpp
//  sujak
//
//  Created by ingun on 25/10/2016.
//
//

#include "jglTableTypedBuffer.hpp"

void jglTableTypedBuffer::init()
{
	jglTableNonTransientArray::init( attributeNum() );
}

jglTypedBuffer* jglTableTypedBuffer::makeOf(unsigned int k)
{
	
}
