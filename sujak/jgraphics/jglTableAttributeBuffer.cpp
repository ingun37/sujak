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
typedef jallocator<jglTypedBuffer, 16, jglTableAttributeBuffer> poolTypedBuffer;
jglTypedBuffer* jglTableAttributeBuffer::makeOf(unsigned int k)
{
    jglTypedBuffer *tb = poolTypedBuffer::getAvailable(1);
    tb->init(ofTypeSize(k), ofReservedCnt(k));
    return tb;
}
