/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 Metal Renderer for Metal Vertex Streaming sample. Acts as the update and render delegate for the view controller and performs rendering. Renders a simple basic triangle with and updates the vertices every frame using a shared CPU/GPU memory buffer.
 */

#include <jcore.hpp>
#include "jmath.hpp"

#import "AAPLRenderer.h"
#import "AAPLViewController.h"
#import "AAPLView.h"

#import <simd/simd.h>

using namespace sujak;

static const long kInFlightCommandBuffers = 1;

JUniformBlock _uniformb;
id <MTLTexture> _texture;




id <MTLBuffer> _uniformBuffer = nil;


jcore core;

void withMetalGetObjInfo(const char* jobjname, char* nameskel, char* namemesh, size_t namesize)
{
    NSURL* url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:jobjname] withExtension:@".jobj" subdirectory:@"meshes"];
    NSData* data = [NSData dataWithContentsOfURL:url];
    
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
    
    NSString *mesh = [dic objectForKey:@"mesh"];
    NSString *skel = [dic objectForKey:@"skeleton"];
    memset(nameskel, 0, namesize);
    memset(namemesh, 0, namesize);
    strncpy(nameskel, [skel UTF8String], strlen([skel UTF8String]));
    strncpy(namemesh, [mesh UTF8String], strlen([mesh UTF8String]));
}


void withMetalLoadFile(const char* szFileName, const char* szExt, char* &file, unsigned long& size)
{
    const unsigned int tmpfilebuffsize = (1024 * 1024);
    static char tmpfilebuff[tmpfilebuffsize];
	NSURL* url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:szFileName] withExtension:[NSString stringWithUTF8String:szExt] subdirectory:@"meshes"];
	NSData* data = [NSData dataWithContentsOfURL:url];
	
	if(data == nil)
        [NSException raise:@"render" format:@"no file at %@", [url absoluteString]];
	
	size = [data length];
    if(size >= tmpfilebuffsize)
        throw "f buff shortage";
	
	[data getBytes:tmpfilebuff length:size];
    file = tmpfilebuff;
}


@implementation AAPLRenderer
{
    
    
    dispatch_semaphore_t _inflight_semaphore;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        _inflight_semaphore = dispatch_semaphore_create(kInFlightCommandBuffers);
    }
    return self;
}

#pragma mark RENDER VIEW DELEGATE METHODS

- (void)configure:(AAPLView *)view
{
    // setup view with drawable formats
    view.sampleCount        = 1;
    
    // create a new command queue
    
    
    // set the vertex shader and buffers defined in the shader source, in this case we have 2 inputs. A position buffer and a color buffer
    // Allocate a buffer to store vertex position data (we'll quad buffer this one)

    
	
    void* tmpbuffers[JVertexAttribute_number];
    for(int ib = 0;ib<JVertexAttribute_number;ib++)
    {
        tmpbuffers[ib] = [_vertexbuffers[ib] contents];
    }
    try
    {
        @try
        {
            core.init(tmpbuffers, (int*)[_indexBuffer contents], withMetalLoadFile, withMetalGetObjInfo);
            core.loadstatic(jobjinfo_static("soldier0", 0, 0, 0));
            core.doneloadstatic();
        }
        @catch(NSException *ex)
        {
            NSLog(@"%@", ex);
            throw;
        }
        @catch(...)
        {
            throw;
        }
    }
    catch(const char* msg)
    {
        NSLog(@"%s", msg);
    }
    catch(...)
    {
        throw;
    }
	matrix_float4x4 mv = jmath::GetViewMatrix({0,70,400}, {0,1,0}, {0,70,0});
	matrix_float4x4 mp = jmath::GetProjectionMatrixPerspective(1.4, 1, 1, 500);
    
	_uniformb.projview = matrix_multiply(mp, mv);
	_uniformb.orthoview = matrix_multiply(jmath::GetProjectionMatrixOrthogonal(1, 1, 1, 200), jmath::GetViewMatrix({0,0,0}, {0,1,0}, {0,0,-1}));
	_uniformBuffer = [_device newBufferWithBytes:&_uniformb length:sizeof(JUniformBlock) options:MTLResourceCPUCacheModeWriteCombined];
    
    
    //////hardcode texture
    NSURL* imgurl = [[NSBundle mainBundle] URLForResource:@"Soldier_D_1024_alt" withExtension:@".jpg" subdirectory:@"meshes"];
    UIImage *pImage = [UIImage imageWithContentsOfFile:[imgurl path]];
    CGColorSpaceRef pColorSpace = CGColorSpaceCreateDeviceRGB();
    uint32_t width  = uint32_t(CGImageGetWidth(pImage.CGImage));
    uint32_t height = uint32_t(CGImageGetHeight(pImage.CGImage));
    uint32_t rowBytes = width * 4;
    CGContextRef pContext = CGBitmapContextCreate(NULL,
                                                  width,
                                                  height,
                                                  8,
                                                  rowBytes,
                                                  pColorSpace,
                                                  CGBitmapInfo(kCGImageAlphaPremultipliedLast));
    
    CGColorSpaceRelease(pColorSpace);
    CGRect bounds = CGRectMake(0.0f, 0.0f, width, height);
    CGContextClearRect(pContext, bounds);
    CGContextDrawImage(pContext, bounds, pImage.CGImage);
    MTLTextureDescriptor *pTexDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                        width:width
                                                                                       height:height
                                                                                    mipmapped:NO];
    
    
    _texture = [_device newTextureWithDescriptor:pTexDesc];
    const void *pPixels = CGBitmapContextGetData(pContext);
    
    MTLRegion region = MTLRegionMake2D(0, 0, width, height);
        
    [_texture replaceRegion:region
                    mipmapLevel:0
                      withBytes:pPixels
                    bytesPerRow:rowBytes];
    
    
    CGContextRelease(pContext);
    
    
}


- (void)render:(AAPLView *)view
{
	
    // Allow the renderer to preflight 3 frames on the CPU (using a semapore as a guard) and commit them to the GPU.
    // This semaphore will get signaled once the GPU completes a frame's work via addCompletedHandler callback below,
    // signifying the CPU can go ahead and prepare another frame.
    dispatch_semaphore_wait(_inflight_semaphore, DISPATCH_TIME_FOREVER);
	core.update();
    
	//core.remapPos(remapPosWithMetal);
    // create a new command buffer for each renderpass to the current drawable
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    
    // create a render command encoder so we can render into something
    MTLRenderPassDescriptor *renderPassDescriptor = view.renderPassDescriptor;
    if (renderPassDescriptor)
    {
        id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
		_renderEncoder = renderEncoder;
		
        [_renderEncoder setCullMode:MTLCullModeFront];
        [_renderEncoder setFragmentTexture:_texture atIndex:0];
        
        for(int ivb=0;ivb<JVertexAttribute_number;ivb++)
        {
            [_renderEncoder setVertexBuffer:_vertexbuffers[ivb]
                                     offset:0
                                    atIndex:jvertexbufferindices[ivb] ];
        }
        
		[_renderEncoder setVertexBuffer:_uniformBuffer offset:0 atIndex:JBufferIndex_uniform];

		core.render( withMetalSetRenderState, withMetalSetPrimitive, withMetalDrawIndex );
		
        [renderEncoder endEncoding];

        // schedule a present once the framebuffer is complete
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    
    // call the view's completion handler which is required by the view since it will signal its semaphore and set up the next buffer
    __block dispatch_semaphore_t block_sema = _inflight_semaphore;
    [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
        
        // GPU has completed rendering the frame and is done using the contents of any buffers previously encoded on the CPU for that frame.
        // Signal the semaphore and allow the CPU to proceed and construct the next frame.
        dispatch_semaphore_signal(block_sema);
    }];
    
    // finalize rendering here. this will push the command buffer to the GPU
    [commandBuffer commit];
}


- (void)reshape:(AAPLView *)view
{
    // unused in this sample
}

@end
