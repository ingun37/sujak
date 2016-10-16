//
//  jglattributebuffer.cpp
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include <iostream>
#include "jglattributebuffer.hpp"
#include "jglbuffer.hpp"

jglAttributeBuffer::jglAttributeBuffer()
{
	this->flag = 0;
}

void jglAttributeBuffer::init(int flag)
{
    this->flag = flag;

    for(int i=0;i<JGLATTRIBUTEBUFFER_MAXATT_CNT;i++)
    {
        if(!(flag & (1 << i)))
            continue;
        initAttributeInfo(attributes[i], i);
    }
}

void jglAttributeBuffer::appendTo(int a, const void *data, unsigned int unitCnt)
{
    if(!(flag & (1 << a)))
        throw "whwioerhwioefhowehfiwef";
    
    attributes[a].buffer->append(data, unitCnt * attributes[a].unitsize);
}
