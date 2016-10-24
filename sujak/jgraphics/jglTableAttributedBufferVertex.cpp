//
//  jglTableAttributedBufferVertex.cpp
//  sujak
//
//  Created by ingun on 24/10/2016.
//
//
#include "jconstants.hpp"
#include "jglTableAttributedBufferVertex.hpp"

using namespace sujak;
unsigned int jglTableAttributedBufferVertex::attributeGroupNumber()
{
	return JAttributeGroupVertex_number;
}

int jglTableAttributedBufferVertex::flagOfAttributeGroup(int attributeGroupIdx)
{
	return jconstant_attribute_group( (JAttributeGroupVertex)attributeGroupIdx );
}
