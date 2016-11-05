//
//  jglTypedBuffer.cpp
//  sujak
//
//  Created by Ingun Jon on 29/10/2016.
//
//
#include "jdefinitions.hpp"
#include "jglTypedBuffer.hpp"
#include "jglPool.hpp"
#include "jglBuffer.hpp"
void jglTypedBuffer::init(unsigned int typeSize, unsigned int reservedCnt)
{
    this->typeSize = typeSize;
    this->reservedCnt = reservedCnt;
    buffer = jglPool::pool->aInitedBuffer(reservedCnt * typeSize, sujak::JMemoryOption_managed_cpuwrite);
}

unsigned int jglTypedBuffer::append(const void* src, unsigned int cnt)//returns offset in unit
{
	unsigned int bytes = buffer->writeAndAdvance(src, cnt * typeSize);
	if( bytes%typeSize != 0)
		throw "90 239ji";
	return bytes/typeSize;
}
