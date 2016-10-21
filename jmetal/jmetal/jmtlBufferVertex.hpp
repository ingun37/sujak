//
//  jmetalvertexbuffer.h
//  jmetal
//
//  Created by ingun on 9/21/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "jconstants.hpp"
#import "jmtlBufferAttribute.hpp"

class jmtlBufferVertex : public jmtlBufferAttribute
{
    virtual unsigned int unitSizeOf(int exponentNum);
    virtual unsigned int reserveUnitCnt();
};
