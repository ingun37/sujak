//
//  jmetalvertexbuffer.m
//  jmetal
//
//  Created by ingun on 9/21/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jmtlBufferVertex.hpp"
#import "jmetaldefinitions.hpp"
#import "jmtlBuffer.hpp"

using namespace sujak;

unsigned int jmtlBufferVertex::unitSizeOf(int exponentNum)
{
    JVertexAttribute a = (JVertexAttribute)exponentNum;
    return jmetalconstant_datatype_size(jconstant_attribute_datatype(a));
}
unsigned int jmtlBufferVertex::reserveUnitCnt()
{
    //todo vary each flag
    return 64;
}
