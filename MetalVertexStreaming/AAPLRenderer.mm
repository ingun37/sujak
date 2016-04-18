/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 Metal Renderer for Metal Vertex Streaming sample. Acts as the update and render delegate for the view controller and performs rendering. Renders a simple basic triangle with and updates the vertices every frame using a shared CPU/GPU memory buffer.
 */

#include "jcore.hpp"
#include "jmath.hpp"

#import "AAPLRenderer.h"
#import "AAPLViewController.h"
#import "AAPLView.h"

#import <simd/simd.h>


static const long kMaxBufferBytesPerFrame = 30000*4;
static const long kInFlightCommandBuffers = 3;


id <MTLBuffer> _vertexPositionBuffer = nil;
id <MTLBuffer> _vertexNormalBuffer = nil;
id <MTLBuffer> _vertexColorBuffer = nil;
id <MTLBuffer> _indexBuffer = nil;
id <MTLRenderCommandEncoder> _renderEncoder = nil;
MTLPrimitiveType _primitiveTypeCurrent = MTLPrimitiveTypeTriangle;
id <MTLBuffer> _uniformBuffer = nil;

class jrendercontextpair
{
	bool inited;
public:
	id<MTLRenderPipelineState> color;
	id<MTLDepthStencilState> depthstencil;
	bool isInited(){ return inited; }
	void setPair(id<MTLRenderPipelineState> _color, id<MTLDepthStencilState> _depth)
	{
		if(color != nil || depthstencil != nil || inited == true)
		{
			NSLog(@"reinitializing renderContext");
			exit(1);
		}
		
		color = _color;
		depthstencil = _depth;
		inited = true;
	}
	jrendercontextpair()
	{
		inited = false;
		color = nil;
		depthstencil = nil;
	}
};

jrendercontextpair renderContextPairs[JRenderState::JRenderState_number];

// render stage
//id <MTLRenderPipelineState> _pipelineStateTriangle;
//id <MTLRenderPipelineState> _pipelineStateLine;
//id <MTLRenderPipelineState> _pipelineStateUI;

//id <MTLDepthStencilState> _depthStencilStateTriangle;
//id <MTLDepthStencilState> _depthStencilStateLine;
//id <MTLDepthStencilState> _depthStencilStateUI;

jcore core;

void withMetalLoadFile(const char* szFileName, const char* szExt, char* &file, unsigned long& size)
{
	NSURL* url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:szFileName] withExtension:[NSString stringWithUTF8String:szExt] subdirectory:@"meshes"];
	NSData* data = [NSData dataWithContentsOfURL:url];
	
	if(data == nil)
	{
		NSLog(@"no file : %@ %@", [NSString stringWithUTF8String:szFileName], [NSString stringWithUTF8String:szExt]);
		exit(1);
	}
	
	size = [data length];
	file = core.getFilePool().getAvailable(size);
	
	[data getBytes:file length:size];
}

void withMetalSetRenderState( JRenderState state )
{
	if(_renderEncoder == nil)
	{
		puts("no renderencoder");
		exit(1);
	}
	
#ifdef DEBUG
	for(int i=0;i<JRenderState::JRenderState_number;i++)
	{
		if(!renderContextPairs[i].isInited())
		{
			NSLog(@"one of rendercontext is not inited");
			exit(1);
		}
	}
#endif
	[_renderEncoder setRenderPipelineState:renderContextPairs[state].color];
	[_renderEncoder setDepthStencilState:renderContextPairs[state].depthstencil];
}

void withMetalSetPrimitive(JRenderPrimitive prim)
{
	switch (prim)
	{
		case JRenderPrimitive_triangle:
			_primitiveTypeCurrent = MTLPrimitiveTypeTriangle;
			break;
		case JRenderPrimitive_line:
			_primitiveTypeCurrent = MTLPrimitiveTypeLine;
			break;
		default:
			puts("unexpected primitivetype");
			exit(1);
			break;
	}
}

void withMetalDrawIndex(int offset, int cnt)
{
	if(_renderEncoder == nil)
	{
		puts("no encoder trying to draw");
		exit(1);
	}
	[_renderEncoder drawIndexedPrimitives:_primitiveTypeCurrent indexCount:cnt indexType:MTLIndexTypeUInt32 indexBuffer:_indexBuffer indexBufferOffset:offset * sizeof(int)];
}

@implementation AAPLRenderer
{
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
    id <MTLLibrary> _defaultLibrary;
    
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
    // find a usable Device
    _device = view.device;

    // setup view with drawable formats
    view.depthPixelFormat   = MTLPixelFormatDepth32Float_Stencil8;
    view.stencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    view.sampleCount        = 1;
    
    // create a new command queue
    _commandQueue = [_device newCommandQueue];
    
    _defaultLibrary = [_device newDefaultLibrary];
    if(!_defaultLibrary) {
        NSLog(@">> ERROR: Couldnt create a default shader library");
        // assert here becuase if the shader libary isn't loading, nothing good will happen
        assert(0);
    }
    
    if (![self preparePipelineState:view])
    {
        NSLog(@">> ERROR: Couldnt create a valid pipeline state");
        
        // cannot render anything without a valid compiled pipeline state object.
        assert(0);
    }
    
    // set the vertex shader and buffers defined in the shader source, in this case we have 2 inputs. A position buffer and a color buffer
    // Allocate a buffer to store vertex position data (we'll quad buffer this one)

    _vertexPositionBuffer = [_device newBufferWithLength:kMaxBufferBytesPerFrame options:0];
	_vertexNormalBuffer = [_device newBufferWithLength:kMaxBufferBytesPerFrame options:0];
	_vertexColorBuffer = [_device newBufferWithLength:kMaxBufferBytesPerFrame options:0];
	_indexBuffer = [_device newBufferWithLength:kMaxBufferBytesPerFrame options:0];
	
	core.loadAll(withMetalLoadFile);
	
	core.initVideoMemoryMapper((simd::float4*)[_vertexPositionBuffer contents], (simd::float4*)[_vertexColorBuffer contents], (simd::float4*)[_vertexNormalBuffer contents], (int*)[_indexBuffer contents]);
	core.layout();
	
	JUniformBlock b;

	matrix_float4x4 mv = jmath::GetViewMatrix({60,50,150}, {0,1,0}, {0,50,0});
	matrix_float4x4 mp = jmath::GetProjectionMatrixPerspective(1, 1, 1, 400);
	b.projview = matrix_multiply(mp, mv);
	b.orthoview = matrix_multiply(jmath::GetProjectionMatrixOrthogonal(1, 1, 1, 200), jmath::GetViewMatrix({0,0,0}, {0,1,0}, {0,0,-1}));
	_uniformBuffer = [_device newBufferWithBytes:&b length:sizeof(JUniformBlock) options:0];
}

- (BOOL)preparePipelineState:(AAPLView*)view
{


	
	MTLVertexDescriptor *mtlVertexDescriptor = [[MTLVertexDescriptor alloc] init];
	
	mtlVertexDescriptor.attributes[JVertexAttribute_position].format = MTLVertexFormatFloat4;
	mtlVertexDescriptor.attributes[JVertexAttribute_position].offset = 0;
	mtlVertexDescriptor.attributes[JVertexAttribute_position].bufferIndex = JBufferIndex_vertex_position;
	
	mtlVertexDescriptor.attributes[JVertexAttribute_normal].format = MTLVertexFormatFloat4;
	mtlVertexDescriptor.attributes[JVertexAttribute_normal].offset = 0;
	mtlVertexDescriptor.attributes[JVertexAttribute_normal].bufferIndex = JBufferIndex_vertex_normal;
	
	mtlVertexDescriptor.attributes[JVertexAttribute_color].format = MTLVertexFormatFloat4;
	mtlVertexDescriptor.attributes[JVertexAttribute_color].offset = 0;
	mtlVertexDescriptor.attributes[JVertexAttribute_color].bufferIndex = JBufferIndex_vertex_color;
	
	mtlVertexDescriptor.layouts[JBufferIndex_vertex_position].stride = sizeof(simd::float4);
	mtlVertexDescriptor.layouts[JBufferIndex_vertex_position].stepFunction = MTLVertexStepFunctionPerVertex;
	mtlVertexDescriptor.layouts[JBufferIndex_vertex_position].stepRate = 1;

	mtlVertexDescriptor.layouts[JBufferIndex_vertex_normal].stride = sizeof(simd::float4);
	mtlVertexDescriptor.layouts[JBufferIndex_vertex_normal].stepFunction = MTLVertexStepFunctionPerVertex;
	mtlVertexDescriptor.layouts[JBufferIndex_vertex_normal].stepRate = 1;
	
	mtlVertexDescriptor.layouts[JBufferIndex_vertex_color].stride = sizeof(simd::float4);
	mtlVertexDescriptor.layouts[JBufferIndex_vertex_color].stepFunction = MTLVertexStepFunctionPerVertex;
	mtlVertexDescriptor.layouts[JBufferIndex_vertex_color].stepRate = 1;
	
    //  create a reusable pipeline state
    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [MTLRenderPipelineDescriptor new];
	MTLDepthStencilDescriptor *depthStencilStateDesc = [[MTLDepthStencilDescriptor alloc]init];

	
	//////////////////////
	id <MTLFunction> vertexProgram = [_defaultLibrary newFunctionWithName:@"passThroughVertex"];
	id <MTLFunction> fragmentProgram = [_defaultLibrary newFunctionWithName:@"passThroughFragment"];
	
	if(vertexProgram == nil || fragmentProgram == nil)
	{
		NSLog(@"model program gen fail");
		exit(1);
	}
	
    pipelineStateDescriptor.label = @"MyPipeline";
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    pipelineStateDescriptor.sampleCount      = view.sampleCount;
	pipelineStateDescriptor.vertexDescriptor = mtlVertexDescriptor;
    pipelineStateDescriptor.vertexFunction   = vertexProgram;
    pipelineStateDescriptor.fragmentFunction = fragmentProgram;
	pipelineStateDescriptor.depthAttachmentPixelFormat = view.depthPixelFormat;
	pipelineStateDescriptor.stencilAttachmentPixelFormat = view.stencilPixelFormat;
	
	depthStencilStateDesc.depthCompareFunction = MTLCompareFunctionLess;
	depthStencilStateDesc.depthWriteEnabled = YES;
	
	renderContextPairs[JRenderState::JRenderState_light].setPair (
		[_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:nil],
		[_device newDepthStencilStateWithDescriptor:depthStencilStateDesc]);

	/////////////////////////////////////////////////
	
	id <MTLFunction> vertLine = [_defaultLibrary newFunctionWithName:@"vertLine"];
	id <MTLFunction> fragLine = [_defaultLibrary newFunctionWithName:@"fragLine"];
	
	if(vertLine == nil || fragLine == nil)
	{
		NSLog(@"line program gen fail");
		exit(1);
	}
    
	pipelineStateDescriptor.vertexFunction = vertLine;
	pipelineStateDescriptor.fragmentFunction = fragLine;
    depthStencilStateDesc.depthCompareFunction = MTLCompareFunctionAlways;
    
	renderContextPairs[JRenderState::JRenderState_info].setPair(
		[_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:nil],
		[_device newDepthStencilStateWithDescriptor:depthStencilStateDesc]);
	
	////////////////////////////////////////
	
	id <MTLFunction> vertUI = [_defaultLibrary newFunctionWithName:@"vertUI"];
	id <MTLFunction> fragUI = [_defaultLibrary newFunctionWithName:@"fragUI"];
	
	if(vertUI == nil || fragUI == nil)
	{
		NSLog(@"ui program gen fail");
		exit(1);
	}
	
	pipelineStateDescriptor.vertexFunction = vertUI;
	pipelineStateDescriptor.fragmentFunction = fragUI;
	
	depthStencilStateDesc.depthCompareFunction = MTLCompareFunctionAlways;
	depthStencilStateDesc.depthWriteEnabled = NO;
	
	renderContextPairs[JRenderState::JRenderState_ui].setPair(
		[_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:nil],
		[_device newDepthStencilStateWithDescriptor:depthStencilStateDesc]);

	/////////////////////////////////////////
	
	for(int i=0;i<JRenderState_number;i++)
	{
		if (!renderContextPairs[i].isInited())
		{
			NSLog(@"oneof rendercontextpairs not inited %d", i);
			exit(1);
		}
	}

    return YES;
}

- (void)render:(AAPLView *)view
{
	core.update();
    // Allow the renderer to preflight 3 frames on the CPU (using a semapore as a guard) and commit them to the GPU.
    // This semaphore will get signaled once the GPU completes a frame's work via addCompletedHandler callback below,
    // signifying the CPU can go ahead and prepare another frame.
    dispatch_semaphore_wait(_inflight_semaphore, DISPATCH_TIME_FOREVER);
	
	//core.remapPos(remapPosWithMetal);
    // create a new command buffer for each renderpass to the current drawable
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    
    // create a render command encoder so we can render into something
    MTLRenderPassDescriptor *renderPassDescriptor = view.renderPassDescriptor;
    if (renderPassDescriptor)
    {
        id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
		_renderEncoder = renderEncoder;
		
		[_renderEncoder setVertexBuffer:_vertexPositionBuffer
								 offset:0
								atIndex:JBufferIndex_vertex_position ];
		
		[_renderEncoder setVertexBuffer:_vertexNormalBuffer
								 offset:0
								atIndex:JBufferIndex_vertex_normal ];
		
		[_renderEncoder setVertexBuffer:_vertexColorBuffer
								 offset:0
								atIndex:JBufferIndex_vertex_color ];
		
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
