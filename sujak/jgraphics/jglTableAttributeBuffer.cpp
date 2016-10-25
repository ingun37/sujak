//
//  jglTableAttributeBuffer.cpp
//  sujak
//
//  Created by ingun on 25/10/2016.
//
//
#include "jconstants.hpp"
#include "jglTableAttributeBuffer.hpp"
#include "jglPoolBuffer.hpp"

using namespace sujak;

void jglTableAttributeBuffer::init()
{
	jglTableNonTransientArray::init( attributeNum() );
}

jglTypedBuffer jglTableAttributeBuffer::makeOf(unsigned int k)
{
    jglTypedBuffer r;
    r.typeSize = typeSizeOf(k);
    r.buffer = jglPoolBuffer::pool->aInitedBuffer( jconstant_starting_vertex_reserve_cnt * r.typeSize , JMemoryOption_managed_cpuwrite );
    return r;
}
