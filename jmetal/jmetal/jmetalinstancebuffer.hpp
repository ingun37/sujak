//
//  jmetalinstancebuffer.h
//  jmetal
//
//  Created by ingun on 13/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "jconstants.hpp"
#import "jmetalbuffer.hpp"
@interface jmetalinstancebuffer : NSObject
{
	sujak::JInstanceAttribute attribs[sujak::JInstanceAttribute_number];
}
@property NSMutableArray<jmetalbuffer*> *buffers;
@property (readonly) int type;
@property (readonly) size_t attnum;

@property (readonly) unsigned int num;
-(sujak::JInstanceAttribute) getAttribOfBufferAt:(int)idx;
-(unsigned int) getUnitSizeOfBufferAt:(int)idx;
-(id)initWithDevice:(id<MTLDevice>)device type:(int)type reserveCnt:(unsigned int)reserveCnt;
-(void)append:(void **)datas cnt:(unsigned int)cnt type:(int)type;
-(void)reset;
@end
