//
//  jmetaltransients.h
//  jmetal
//
//  Created by ingun on 9/30/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

@interface jmetalvertinfo : NSObject
@property NSUInteger buffidx;
@property MTLVertexFormat vertexformat;
@end


@interface jmetaltransients : NSObject 
+(MTLRenderPassDescriptor*) newRenderPass:(id<MTLTexture>)color depth:(id<MTLTexture>)depth stencil:(id<MTLTexture>)stencil;
+(MTLVertexDescriptor*) newVertexDescWithCnt:(NSUInteger)cnt buffidxs:(NSUInteger*)buffidxs vertexformats:(MTLVertexFormat*)vformats stride:(unsigned int*)strides;
@end
