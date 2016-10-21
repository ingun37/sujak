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

#include "jmtlBufferAttribute.hpp"

class jmtlBufferInstance : public jmtlBufferAttribute
{
    virtual unsigned int unitSizeOf(int exponentNum);
    virtual unsigned int reserveUnitCnt();
};

