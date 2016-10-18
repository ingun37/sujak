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
	this->attributecnt = 0;
}

void jglAttributeBuffer::init(int flag)
{
    this->flag = flag;

    for(int i=0;i<JGLATTRIBUTEBUFFER_MAXATT_CNT;i++)
    {
        if(!(flag & (1 << i)))
            continue;
        initAttributeInfo(attributes[i], i);
		attributecnt++;
    }
}

void jglAttributeBuffer::writeAndAdvance(unsigned int cnt, int attribute1, const void* src1,
					 int attribute2, const void* src2,
					 int attribute3, const void* src3)
{
	jglAttributeData datas[3];
	datas[0].attribute = attribute1;
	datas[0].src  = src1;
	
	datas[1].attribute = attribute2;
	datas[1].src  = src2;
	
	datas[2].attribute = attribute3;
	datas[2].src  = src3;
	
	int i = 0;
	
	for(i=0;i<3;i++)
	{
		if(datas[i].attribute == 0)
			break;
	}
	i++;
	writeAndAdvance(cnt, i, datas);
		
}

void jglAttributeBuffer::writeAndAdvance(unsigned int unitcnt, int datacnt, const jglAttributeData datas[])
{
	if(datacnt != attributecnt)
		throw "9802wr092h934fh2f";
	int flag = this->flag;
	for(int i=0;i<attributecnt;i++)
	{
		if(!(flag & (1 << datas[i].attribute)))
			throw "98902039u023490349090iodifi";
		flag ^= (1 << datas[i].attribute);
		jglAttributeBufferAttributeInfo& a = attributes[ datas[i].attribute ];
		
		a.buffer->writeAndAdvance(datas[i].src, a.unitsize * unitcnt);
	}
	if(flag != 0)
		throw "sildgihsdfgilhsdhilgilsdfgi";
}
