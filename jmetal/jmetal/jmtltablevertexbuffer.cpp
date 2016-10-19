//
//  jmtltablevertexbuffer.cpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtltablevertexbuffer.hpp"
#include "jmtlvertexbuffer.hpp"
#include "jallocator.hpp"

using namespace sujak;

typedef jallocator<jmtlVertexBuffer, 4, jglTableVertexBuffer> bufferpool;
jmtlVertexBuffer* jglTableVertexBuffer::makeOf(int k)
{
	jmtlVertexBuffer* buffer = bufferpool::getAvailable(1);
	buffer->init( jconstant_attribute_group((JAttributeGroupVertex)k, device );
	return buffer;
}

void jglTableVertexBuffer::init(id<MTLDevice> device)
{
	this->device = device;
	jglNonTransientTable<jmtlVertexBuffer*>::initOrdered( JAttributeGroupVertex_number );
}
