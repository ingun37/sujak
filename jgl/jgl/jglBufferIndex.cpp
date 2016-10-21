//
//  jglBufferindex.cpp
//  jgl
//
//  Created by ingun on 20/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include <iostream>
#include "jglBufferIndex.hpp"
#include "jglBuffer.hpp"
//virtual unsigned int unitSize() = 0;
//virtual jglBuffer* makeIndexBuffer(reservecnt) = 0;
void jglBufferIndex::init(unsigned int reserveCnt)
{
	buffer = makeIndexBuffer(reserveCnt);
	cnt = 0;
}
jglBufferIndex::jglBufferIndex()
{
	buffer = NULL;
}

void jglBufferIndex::append(unsigned int cnt, void* src, unsigned int &offsetCnt)
{
	offsetCnt = this->cnt;
	buffer->writeAndAdvance(src, cnt * unitSize());
	this->cnt += cnt;
}
