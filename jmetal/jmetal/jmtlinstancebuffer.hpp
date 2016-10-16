//
//  jmetalinstancebuffer.h
//  jmetal
//
//  Created by ingun on 13/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "jconstants.hpp"

#include "jmtlattributebuffer.hpp"

class jmtlInstanceBuffer : public jmtlAttributeBuffer
{
    virtual unsigned int unitSizeOf(int exponentNum);
    virtual unsigned int reserveUnitCnt();
};

