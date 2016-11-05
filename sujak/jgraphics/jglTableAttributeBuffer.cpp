//
//  jglTableAttributeBuffer.cpp
//  sujak
//
//  Created by ingun on 25/10/2016.
//
//
#include "jallocator.hpp"
#include "jglTableAttributeBuffer.hpp"
#include "jglTypedBuffer.hpp"
using namespace sujak;

void jglTableAttributeBuffer::init()
{
	jglTableNonTransientArray::init( attributeNum() );
}
jallocator<jglTypedBuffer, 16> pool;
jglTypedBuffer* jglTableAttributeBuffer::makeOf(unsigned int k)
{
    jglTypedBuffer *tb = pool.getAvailable(1);
    tb->init(ofTypeSize(k), ofReservedCnt(k));
    return tb;
}
