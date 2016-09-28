#import "jmetal.hpp"

#import "jmetalrendercontextdata.hpp"

using namespace sujak;



void jmetal::loadObject(sujak::JRenderContext state, sujak::jrenderobject *obj)
{
	/*
	jmetalrendercontextdata* context = rendercontextdatas[state];
	jmetalvertexbuffer *vertexbuffer = context.att;
	
	for(int i=0;i<JVertexAttribute_number;i++)
	{
		JVertexAttribute eIdx = (JVertexAttribute)i;
		jmetalbuffer* attbuffer = [vertexbuffer getBufferOf:eIdx];
		const void* vertexattributedata = obj->getDataForCopy(eIdx);
		JDataTypeVertex jtype = jconstant_vertex_attributes[i].type;
		unsigned long len = obj->getVertexCnt() * jmetalconstanttypesize[jtype];
		
		[attbuffer append:vertexattributedata len:len];
	}
	 */
	//[context.index.buffer append:obj->getIndexDataForCopy() len:jmetalconstanttypesize[jconstant_index_type] * obj->getIndexCnt()];
}

void jmetal::render(id<MTLTexture> rendertarget)
{
	id<MTLCommandBuffer> cmdbuff = [commandqueue commandBuffer];
	renderpassdesc.colorAttachments[0].texture = rendertarget;
	
	id <MTLRenderCommandEncoder> rencoder = [cmdbuff renderCommandEncoderWithDescriptor:renderpassdesc];
	
	for(int i=0;i<JRenderContext_number;i++)
	{
		jmetalrendercontextdata* context = rendercontextdatas[i];
		
		[rencoder setRenderPipelineState:context.renderpipeline];
		/*
		[rencoder setVertexBuffer:[context.att getBufferOf:JVertexAttribute_position].buffer offset:0 atIndex:JBuffer_vertex_position];
		[rencoder setVertexBuffer:[context.att getBufferOf:JVertexAttribute_normal].buffer offset:0 atIndex:JBuffer_vertex_normal];
		[rencoder setVertexBuffer:[context.att getBufferOf:JVertexAttribute_color].buffer offset:0 atIndex:JBuffer_vertex_color];
		[rencoder setVertexBuffer:[context.att getBufferOf:JVertexAttribute_uv].buffer offset:0 atIndex:JBuffer_vertex_uv];
		*/
		//[rencoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:[context.index indexCnt] indexType:jmetalconstantindextype[jconstant_index_type] indexBuffer:context.index.buffer.buffer indexBufferOffset:0];
	}
	[rencoder endEncoding];
	[cmdbuff commit];
} 
void jmetal::addVertexAttribute(int bindbufferidx, MTLVertexFormat format)
{
    
}
void jmetal::init(int width, int height)
{
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    commandqueue = [device newCommandQueue];
    
    id<MTLLibrary> library = [device newDefaultLibrary];
    
    MTLVertexDescriptor *vdesc = [MTLVertexDescriptor new];
    
    for(int iva=0;iva<JVertexAttribute_number;iva++)
    {
        MTLVertexAttributeDescriptor* vadesc = [MTLVertexAttributeDescriptor new];
        vadesc.bufferIndex = jconstant_vertex_attributes[iva].bufferIdx;
        vadesc.offset = 0;
		vadesc.format = jmetalconstantmetaltype[jconstant_vertex_attributes[iva].type];
		
        MTLVertexBufferLayoutDescriptor* vldesc = [MTLVertexBufferLayoutDescriptor new];
        vldesc.stepFunction = MTLVertexStepFunctionPerVertex;
        vldesc.stride = 0;
        vldesc.stepRate = 1;
        
        [vdesc.layouts setObject:vldesc atIndexedSubscript:iva];
        [vdesc.attributes setObject:vadesc atIndexedSubscript:iva];
    }
 
	MTLTextureDescriptor *dtdesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float width:width height:height mipmapped:NO];
	dtdesc.textureType = MTLTextureType2D;
	dtdesc.sampleCount = 1;
	rtDepth = [device newTextureWithDescriptor:dtdesc];
	
	MTLTextureDescriptor *stdesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatStencil8 width:width height:height mipmapped:NO];
	stdesc.textureType = MTLTextureType2D;
	stdesc.sampleCount = 1;
	rtStencil = [device newTextureWithDescriptor:stdesc];
	
	renderpassdesc = [MTLRenderPassDescriptor new];
	renderpassdesc.colorAttachments[0].loadAction = MTLLoadActionClear;
	renderpassdesc.colorAttachments[0].storeAction = MTLStoreActionStore;
	renderpassdesc.colorAttachments[0].clearColor = MTLClearColorMake(0.4, 0.2, 0.1, 1);
	
	renderpassdesc.depthAttachment.loadAction = MTLLoadActionClear;
	renderpassdesc.depthAttachment.storeAction = MTLStoreActionStore;
	renderpassdesc.depthAttachment.clearDepth = 1;
	renderpassdesc.depthAttachment.texture = rtDepth;
	
	renderpassdesc.stencilAttachment.texture = rtStencil;
	
    for(int i=0;i<JRenderContext_number;i++)
    {
        NSString* nameV = [NSString stringWithUTF8String: jmetalconstantshaderentryvert[ jconstant_states_color[ jconstant_contexts[i].color ].shaderVert]];
        NSString* nameF = [NSString stringWithUTF8String: jmetalconstantshaderentryfrag[ jconstant_states_color[ jconstant_contexts[i].color ].shaderFrag]];
		
        id<MTLFunction> frag = [library newFunctionWithName:nameF];
        id<MTLFunction> vert = [library newFunctionWithName:nameV];
		
		if(frag == nil || vert == nil)
		{
			[NSException raise:@"no shader" format:@"no shader...."];
		}
		
		MTLRenderPipelineDescriptor* descPipe = [MTLRenderPipelineDescriptor new];
		
        descPipe.vertexFunction = vert;
        descPipe.fragmentFunction = frag;
        descPipe.vertexDescriptor = vdesc;
        descPipe.sampleCount = 1;
		descPipe.depthAttachmentPixelFormat = jmetalconstantpixeltype[jconstant_pixelformat_depth];
		descPipe.stencilAttachmentPixelFormat = jmetalconstantpixeltype[jconstant_pixelformat_stencil];
		descPipe.colorAttachments[0].pixelFormat = jmetalconstantpixeltype[jconstant_pixelformat_color];
		descPipe.colorAttachments[0].blendingEnabled = YES;
		
		rendercontextdatas[i].renderpipeline = [device newRenderPipelineStateWithDescriptor:descPipe error:nil];
		rendercontextdatas[i].stream = [[jmetalstreambuffer alloc]initWithDevcie:device];
		rendercontextdatas[i].uniform = [[jmetaluniformbuffer alloc]initWithDevice:device];
    }
}
