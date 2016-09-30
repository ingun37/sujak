//
//  jmetalvertexbuffer.h
//  jmetal
//
//  Created by ingun on 9/21/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "jconstants.hpp"
#import "jmetalbuffer.hpp"

@interface jmetalvertexbuffer : NSObject
{
    sujak::JVertexAttribute attribs[sujak::JVertexAttribute_number];
}
@property NSMutableArray<jmetalbuffer*> *buffers;
@property (readonly) int vertextype;
@property (readonly) size_t attnum;

@property (readonly) unsigned int vnum;
-(sujak::JVertexAttribute) getAttribOfBufferAt:(int)idx;
-(unsigned int) getUnitSizeOfBufferAt:(int)idx;
-(id)initWithDevice:(id<MTLDevice>)device vertextype:(int)vertextype reserveVertexCnt:(unsigned int)reserveVertexCnt;
-(void)append:(void **)datas cnt:(unsigned int)cnt vertextype:(int)vertextype;
-(void)reset;
@end
