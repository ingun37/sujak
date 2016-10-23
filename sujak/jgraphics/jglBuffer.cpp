//
//  jglBuffer.cpp
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include <iostream>
#include "jglBuffer.hpp"

void* jglBuffer::handleAtPointer()
{
    return (char*)handle() + pointer;
}
void jglBuffer::reset()
{
	this->pointer = 0;
}

jglBuffer::jglBuffer()
{
	pointer = 0;
	reservedSize = 0;
}

void jglBuffer::init(unsigned int reserve)
{
	reservedSize = reserve;
}

void jglBuffer::writeAndAdvance(const void *src, unsigned int len)
{
	if(((unsigned int)-1) - this->pointer < len)
		throw "exceeee1";
	if(((unsigned int)-1) - len < this->pointer)
		throw "exceeeee2";
	if(this->pointer + len > reservedSize)
		throw "exceeeeeeeee";
	memcpy(handleAtPointer(), src, len);
	this->pointer += len;
}
