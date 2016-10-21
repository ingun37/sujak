//
//  jmetalbuffer.m
//  jmetal
//
//  Created by ingun on 9/6/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "jmtlBuffer.hpp"

void* jmtlBuffer::handle()
{
    return [buffer contents];
}

void jmtlBuffer::init(unsigned int reserve, id<MTLDevice> device, MTLResourceOptions option)
{
    jglBuffer::init(reserve);
    buffer = [device newBufferWithLength:reserve options:option];
}
