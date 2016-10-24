//
//  jmtlBufferInstancetable.cpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtlTableBufferInstance.hpp"
#include "jmtlBufferInstance.hpp"
#include "jallocator.hpp"
#include "jconstants.hpp"
using namespace sujak;
typedef jallocator<jmtlBufferInstance, 4, jmtlTableBufferInstance> bufferpool;
jmtlBufferInstance* jmtlTableBufferInstance::makeOf(int k)
{
	jmtlBufferInstance* buffer = bufferpool::getAvailable(1);
	buffer->init(jconstant_attribute_group((JAttributeGroupInstance)k), device);
	return buffer;
}

void jmtlTableBufferInstance::init(id<MTLDevice> device)
{
	this->device = device;
	jglTableNonTransient<jmtlBufferInstance*>::initOrdered(JAttributeGroupInstance_number);
}
