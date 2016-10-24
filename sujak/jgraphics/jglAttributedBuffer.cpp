//
//  jglattributebuffer.cpp
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include <iostream>
#include "jglAttributedBuffer.hpp"
#include "jglBuffer.hpp"

jglAttributedBuffer::jglAttributedBuffer()
{
	this->flag = 0;
	this->attributecnt = 0;
	this->cnt = 0;
}

void jglAttributedBuffer::init(int flag)
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

void jglAttributedBuffer::append(unsigned int &offsetCnt, unsigned int cnt, int attribute1, const void* src1,
					 int attribute2, const void* src2,
					 int attribute3, const void* src3)
{
	jglAttributedBufferData datas[3];
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
	append(offsetCnt, cnt, i, datas);
		
}

void jglAttributedBuffer::append(unsigned int &offsetCnt, unsigned int unitcnt, int datacnt, const jglAttributedBufferData datas[])
{
	if(datacnt != attributecnt)
		throw "9802wr092h934fh2f";
	
	//flag validate
	{
		int tmpflag = this->flag;
		for(int i=0;i<attributecnt;i++)
		{
			if(!(tmpflag & (1 << datas[i].attribute)))
				throw "98902039u023490349090iodifi";
			tmpflag ^= (1 << datas[i].attribute);
		}
		if(tmpflag != 0)
			throw "sildgihsdfgilhsdhilgilsdfgi";
	}
	
	offsetCnt = this->cnt;
	for(int i=0;i<attributecnt;i++)
	{
		jglAttributedBufferAttributeInfo& a = attributes[ datas[i].attribute ];
		a.buffer->writeAndAdvance(datas[i].src, a.unitsize * unitcnt);
	}
	this->cnt += unitcnt;
}
