//
//  jglTypedBuffer.cpp
//  sujak
//
//  Created by Ingun Jon on 29/10/2016.
//
//
#include "jdefinitions.hpp"
#include "jglTypedBuffer.hpp"
#include "jglPoolBuffer.hpp"
void jglTypedBuffer::init(unsigned int typeSize, unsigned int reservedCnt)
{
    this->typeSize = typeSize;
    this->reservedCnt = reservedCnt;
    buffer = jglPoolBuffer::pool->aInitedBuffer(reservedCnt * typeSize, sujak::JMemoryOption_managed_cpuwrite);
}
