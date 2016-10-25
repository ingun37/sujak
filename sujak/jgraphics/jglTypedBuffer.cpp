//
//  jglTypedBuffer.cpp
//  sujak
//
//  Created by ingun on 25/10/2016.
//
//
#include <iostream>
#include "jglTypedBuffer.hpp"
#include "jglBuffer.hpp"
jglTypedBuffer::jglTypedBuffer()
{
	this->buffer = NULL;
	this->typeSize = 0;
}

void jglTypedBuffer::init(unsigned int typeSize, jglBuffer* buffer)
{
	this->buffer = buffer;
	this->typeSize = typeSize;
}

void jglTypedBuffer::append(const void* src, unsigned int unitCnt)
{
	buffer->writeAndAdvance(src, unitCnt * typeSize);
}
