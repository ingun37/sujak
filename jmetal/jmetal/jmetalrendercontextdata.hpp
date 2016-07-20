//
//  jmetalrendercontextdata.h
//  jmetal
//
//  Created by Ingun Jon on 9/16/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

#include "jmetalconstants.hpp"
#import "jmetalstreambuffer.hpp"
#import "jmetaluniformbuffer.hpp"

@interface jmetalrendercontextdata : NSObject

@property NSString* frag;
@property NSString* vert;
@property id<MTLRenderPipelineState> renderpipeline;
@property jmetaluniformbuffer* uniform;
@property jmetalstreambuffer* stream;
@end
