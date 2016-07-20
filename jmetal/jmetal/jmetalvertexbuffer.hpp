//
//  jmetalvertexbuffer.h
//  jmetal
//
//  Created by ingun on 9/21/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "jmetalbuffer.hpp"
#include "jdefinitions.hpp"
@interface jmetalvertexbuffer : NSObject
{
	jmetalbuffer* buffers[sujak::JVertexAttribute_number];
}

-(jmetalbuffer*)getBufferOf:(sujak::JVertexAttribute) att;
-(id)initWithDevice:(id<MTLDevice>)device;

@end
