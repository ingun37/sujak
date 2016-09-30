//
//  jmetalstreambuffer.h
//  jmetal
//
//  Created by ingun on 9/27/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "jmetalindexbuffer.hpp"
#import "jmetalvertexbuffer.hpp"

@interface jmetalstreambuffer : NSObject
@property jmetalindexbuffer* index;
@property jmetalvertexbuffer* vertex;

-(id)initWithDevcie:(id<MTLDevice>)device vertextype:(int)vertextype reserveVertexCnt:(unsigned int)reserveVertexCnt reserveIndexCnt:(unsigned int)reserveIndexCnt;
-(void)appendVertexData:(void**)vdatas vertextype:(int)vertextype vertexCnt:(unsigned int)vcnt indexData:(void*)idata indexCnt:(unsigned int)icnt;
-(void)reset;
@end
