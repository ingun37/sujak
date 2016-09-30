//
//  jmetalindexbuffer.h
//  jmetal
//
//  Created by ingun on 9/22/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "jmetalbuffer.hpp"
@interface jmetalindexbuffer : NSObject

@property jmetalbuffer* buffer;
@property (readonly) unsigned int indexCnt;
-(id)initWithDevice:(id<MTLDevice>)device reserveIndexCnt:(unsigned int)reserveIndexCnt;
-(unsigned int)getUnitSize;
-(void)append:(void*)data cnt:(unsigned int)cnt;
-(void)reset;
@end
