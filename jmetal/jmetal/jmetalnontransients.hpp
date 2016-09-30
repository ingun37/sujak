//
//  jmetalnontransients.h
//  jmetal
//
//  Created by ingun on 9/30/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "jconstants.hpp"
#import "jmetalstreambuffer.hpp"
@interface jmetalnontransients : NSObject

+(id<MTLRenderPipelineState>)pipelineWithDevice:(id<MTLDevice>)device library:(id<MTLLibrary>)library of:(sujak::JPipeline)p;
+(jmetalstreambuffer*)bufferOfType:(int)type device:(id<MTLDevice>)device;
+(id<MTLTexture>)defaultDepthTexWithDevice:(id<MTLDevice>)device width:(NSUInteger)w height:(NSUInteger)h format:(MTLPixelFormat)f;
+(id<MTLTexture>)defaultStencilTexWithDevice:(id<MTLDevice>)device width:(NSUInteger)w height:(NSUInteger)h  format:(MTLPixelFormat)f;
@end
