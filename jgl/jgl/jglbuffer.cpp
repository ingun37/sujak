//
//  jglbuffer.cpp
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include <iostream>
#include "jglBuffer.hpp"

void jglBuffer::append(const void *data, unsigned int len)
{
	if(data == NULL)
		throw "data is null";
	
    memcpy((char*)handle() + this->len, data, len);//todo safer
    this->len += len;
}

void jglBuffer::reset()
{
	this->len = 0;
}

jglBuffer::jglBuffer()
{
	len = 0;
}

void jglBuffer::init(unsigned int reserve)
{
    
}
