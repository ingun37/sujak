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

@interface jmetalnontransients : NSObject

+(id<MTLRenderPipelineState>)pipelineWithDevice:(id<MTLDevice>)device library:(id<MTLLibrary>)library of:(sujak::JPipeline)p;

+(id<MTLTexture>)defaultDepthTexWithDevice:(id<MTLDevice>)device width:(NSUInteger)w height:(NSUInteger)h format:(MTLPixelFormat)f;
+(id<MTLTexture>)defaultStencilTexWithDevice:(id<MTLDevice>)device width:(NSUInteger)w height:(NSUInteger)h  format:(MTLPixelFormat)f;
@end
