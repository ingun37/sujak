//
//  jmetalinstancebuffer.m
//  jmetal
//
//  Created by ingun on 13/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "jmtlBufferInstance.hpp"
#import "jmetaldefinitions.hpp"
#import "jconstants.hpp"
#import "jmtlBuffer.hpp"

using namespace sujak;

unsigned int unitSizeOf(int exponentNum)
{
    JInstanceAttribute a = (JInstanceAttribute)exponentNum;
    return jmetalconstant_datatype_size(jconstant_attribute_datatype(a));
}
unsigned int reserveUnitCnt()
{
    //todo make it different for each flags
    return 8;
}
