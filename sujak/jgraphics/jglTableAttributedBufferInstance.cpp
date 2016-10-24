//
//  jglTableAttributedBufferInstance.cpp
//  sujak
//
//  Created by ingun on 24/10/2016.
//
//

#include "jconstants.hpp"
#include "jglTableAttributedBufferInstance.hpp"

using namespace sujak;
unsigned int jglTableAttributedBufferInstance::attributeGroupNumber()
{
	return JAttributeGroupInstance_number;
}
int jglTableAttributedBufferInstance::flagOfAttributeGroup(int attributeGroupIdx)
{
	return jconstant_attribute_group((JAttributeGroupInstance)attributeGroupIdx);
}
