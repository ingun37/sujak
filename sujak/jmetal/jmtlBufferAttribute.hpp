//
//  jmtlattributebuffer.hpp
//  jmetal
//
//  Created by Ingun Jon on 16/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtlattributebuffer_hpp
#define jmtlattributebuffer_hpp

#import <Metal/Metal.h>
#include "jglBufferAttribute.hpp"


class jmtlBufferAttribute : public jglBufferAttribute
{
    id<MTLDevice> device;
protected:
    virtual unsigned int unitSizeOf(int exponentNum) = 0;
    virtual unsigned int reserveUnitCnt() = 0;
    virtual void initAttributeInfo( jglAttributeBufferAttributeInfo& info, int exponentNum );
public:
    virtual void init(int flag, id<MTLDevice> device);
};
#endif /* jmtlattributebuffer_hpp */
