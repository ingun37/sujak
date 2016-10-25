//
//  jmtltablebuffervertex.cpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtlTableBufferVertex.hpp"
#include "jmtlBufferVertex.hpp"
#include "jallocator.hpp"

using namespace sujak;

typedef jallocator<jmtlBufferVertex, 4, jglTableBufferVertex> bufferpool;
jmtlBufferVertex* jglTableBufferVertex::makeOf(int k)
{
	jmtlBufferVertex* buffer = bufferpool::getAvailable(1);
	buffer->init( jconstant_attribute_group((JAttributeGroupVertex)k), device );
	return buffer;
}

void jglTableBufferVertex::init(id<MTLDevice> device)
{
	this->device = device;
	jglTableNonTransientMap<jmtlBufferVertex*>::initOrdered( JAttributeGroupVertex_number );
}
