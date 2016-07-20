//
//  jmetalbuffer.h
//  jmetal
//
//  Created by ingun on 9/6/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmetalbuffer_h
#define jmetalbuffer_h

#import <Metal/Metal.h>

@interface jmetalbuffer : NSObject

@property (readonly) unsigned long offset;
@property id<MTLBuffer> buffer;
-(id)initWithDevice:(id<MTLDevice>)device initialSize:(unsigned int)initialSize option:(MTLResourceOptions)option;
-(void)append:(const void*)data len:(unsigned long)len;
@end

#endif /* jmetalbuffer_h */
