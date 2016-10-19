//
//  jmtlinstancebuffertable.cpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtlinstancebuffertable.hpp"
#include "jmtlinstancebuffer.hpp"
#include "jallocator.hpp"
#include "jconstants.hpp"
using namespace sujak;
typedef jallocator<jmtlInstanceBuffer, 4, jmtlInstanceBufferTable> bufferpool;
jmtlInstanceBuffer* jmtlInstanceBufferTable::makeOf(int k)
{
	jmtlInstanceBuffer* buffer = bufferpool::getAvailable(1);
	buffer->init(jconstant_attribute_group((JAttributeGroupInstance)k), device);
	return buffer;
}

void jmtlInstanceBufferTable::init(id<MTLDevice> device)
{
	this->device = device;
	jglNonTransientTable<jmtlInstanceBuffer*>::initOrdered(JAttributeGroupInstance_number);
}
