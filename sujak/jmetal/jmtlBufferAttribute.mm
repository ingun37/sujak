//
//  jmtlattributebuffer.cpp
//  jmetal
//
//  Created by Ingun Jon on 16/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtlBufferAttribute.hpp"
#include "jallocator.hpp"
#include "jmtlBuffer.hpp"
using namespace sujak;

void jmtlBufferAttribute::init(int flag, id<MTLDevice> device)
{
    jglBufferAttribute::init(flag);
    this->device = device;
}

typedef jallocator<jmtlBuffer, 16, jmtlBufferAttribute> poolBuff;


void jmtlBufferAttribute::initAttributeInfo( jglAttributeBufferAttributeInfo& info, int exponentNum )
{
    info.unitsize = unitSizeOf(exponentNum);
    jmtlBuffer* buff = poolBuff::getAvailable(1);
    buff->init(reserveUnitCnt() * info.unitsize, device, MTLResourceOptionCPUCacheModeWriteCombined);
    info.buffer = buff;
}
