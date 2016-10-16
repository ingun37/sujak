//
//  jmtlattributebuffer.cpp
//  jmetal
//
//  Created by Ingun Jon on 16/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtlattributebuffer.hpp"
#include "jallocator.hpp"
#include "jmtlbuffer.hpp"
using namespace sujak;

void jmtlAttributeBuffer::init(int flag, id<MTLDevice> device)
{
    jglAttributeBuffer::init(flag);
    this->device = device;
}

typedef jallocator<jmtlBuffer, 16, jmtlAttributeBuffer> poolBuff;


void jmtlAttributeBuffer::initAttributeInfo( jglAttributeBufferAttributeInfo& info, int exponentNum )
{
    info.unitsize = unitSizeOf(exponentNum);
    jmtlBuffer* buff = poolBuff::getAvailable(1);
    buff->init(reserveUnitCnt() * info.unitsize, device, MTLResourceOptionCPUCacheModeWriteCombined);
    info.buffer = buff;
}
