//
//  jmetalvertexbuffer.h
//  jmetal
//
//  Created by ingun on 9/21/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "jconstants.hpp"
#import "jmtlattributebuffer.hpp"

class jmtlVertexBuffer : public jmtlAttributeBuffer
{
    virtual unsigned int unitSizeOf(int exponentNum) = 0;
    virtual unsigned int reserveUnitCnt() = 0;
};
