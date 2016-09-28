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
{
	jmetalbuffer* buffer;
}
-(id)initWithDevice:(id<MTLDevice>)device;
-(void)resetOffset;
-(void)loadObjectIndex:(const void*)data cnt:(unsigned int)cnt;
@end
