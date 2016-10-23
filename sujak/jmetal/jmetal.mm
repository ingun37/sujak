#include <math.h>

#import "jmetal.hpp"

#import "jmtlBufferVertex.hpp"
#import "jmtlBufferInstance.hpp"
#import "jmtlBuffer.hpp"
#import "jmetaltransients.hpp"
#import "jmetalnontransients.hpp"
#import "jmetaldefinitions.hpp"
#import "jallocator.hpp"
#import "jmtlTableShader.hpp"
#import "jmtlTableBufferVertex.hpp"
#import "jmtlTableBufferInstance.hpp"

using namespace std;
using namespace sujak;


jmtlBuffer uniformbuffer;

void jmetal::loadVertexObjectOnMemory(unsigned int vcnt, simd::float4* positions, simd::float4* colors)
{
	int type = (1 << JVertexAttribute_color) | (1 << JVertexAttribute_position);
	
	if(jconstant_attribute_datatype(JVertexAttribute_color) != JDataType_f4)
		throw "902392903sksdjfksdf";
	if(jconstant_attribute_datatype(JVertexAttribute_position) != JDataType_f4)
		throw "9123232323233232sdfgxcv";
    if(vbuffers.find(type) == vbuffers.end())
    {
        jmtlBufferVertex* b = poolVBuffer::getAvailable(1);
        b->init(vtype, device);
        vbuffers[vtype] = b;
    }
	return vbuffers[type]->writeAndAdvance(vcnt, JVertexAttribute_position, positions, JVertexAttribute_color, colors);
}

void jmetal::loadInstanceObjectOnMemory(int cnt, matrix_float4x4* trans)
{
	int type = (1 << JInstanceAttribute_trans);
	
	if(jconstant_attribute_datatype(JInstanceAttribute_trans) != JDataType_f44)
		throw "ilkiojlkmijlkijlk";
		
    if(instancebuffers.find(type) == instancebuffers.end())
    {
        jmtlBufferInstance* b = poolInstanceBuffer::getAvailable(1);
        b->init(type, device);
        instancebuffers[itype] = b;
    }
    
    return instancebuffers[itype]->writeAndAdvance(cnt, JInstanceAttribute_trans, trans);
}

void jmetal::init(CAMetalLayer* metallayer, CGSize drawableSize, JUniform initUniform)
{
    device = MTLCreateSystemDefaultDevice();
    commandqueue = [device newCommandQueue];
    
    library = [device newDefaultLibrary];
	if(library == nil)
	{
		NSString *path = [[NSBundle mainBundle] pathForResource:@"jmetalshader" ofType:@"metallib"];
		
		library = [device newLibraryWithFile:path error:nil];
		if(library == nil)
		{
			[NSException raise:@"library is no" format:@""];
		}
	}

	metallayer.drawableSize = drawableSize;

	metallayer.device = device;
	metallayer.pixelFormat = jmetalconstant_pixelformat(jconstant_framebuffer_pixelformat);
	metallayer.framebufferOnly = YES;//todo : do something
	
    uniformbuffer.init(sizeof(JUniform), device, MTLResourceOptionCPUCacheModeWriteCombined);
	updateUniform(initUniform);
}

void jmetal::updateUniform(JUniform uni)
{
	uniformbuffer->reset();
	uniformbuffer->writeAndAdvance(&uni, sizeof(JUniform));
}

void jmetal::draw(void **vdatas, int vtype, unsigned int vcnt, void *idata, unsigned int icnt, id<MTLTexture> ctarget, JPipeline p, id<MTLDrawable> drawableToPresent)
{
	draw(vdatas, vtype, vcnt, idata, icnt, ctarget, p, drawableToPresent,
		 [jmetalnontransients defaultDepthTexWithDevice:device width:[ctarget width] height:[ctarget height] format: jmetalconstant_pixelformat(jconstant_depth_pixelformat) ],
		 [jmetalnontransients defaultStencilTexWithDevice:device width:[ctarget width] height:[ctarget height] format:jmetalconstant_pixelformat(jconstant_stencil_pixelformat)] );
}

void jmetal::draw(int vtype, unsigned int vcnt, void* idata, unsigned int icnt,
				  id<MTLTexture> ctarget, JPipeline p, id<MTLDrawable> drawableToPresent,
				  id<MTLTexture> dtarget,
				  id<MTLTexture> starget
				  )
{
	
	jmetalrenderingwork* work = rendertransients.getormake(ctarget, dtarget, starget, drawableToPresent);
    jmetalrenderstategroups* state = work->getormakeone([jmetalnontransients pipelineWithDevice:device library:library of:p]);
	jmetalrenderobj* obj = state->getormakeone(vtype);
	
    jmetalstreambuffer* streambuffer = [jmetalnontransients bufferOfType:vtype device:device];
    unsigned int ioffset = streambuffer.index.indexCnt;
    unsigned int voffset = streambuffer.vertex.vnum;
    [streambuffer appendVertexData:vdatas vertextype:vtype vertexCnt:vcnt indexData:idata indexCnt:icnt];
    
    jmetalbufferalign align(voffset, ioffset, vcnt, icnt);
	obj->aligns.push_back(align);
}

void jmetal::render()
{
    for(vec_jmetalrenderingwork::iterator work = rendertransients.renderingworks.begin() ; work!=rendertransients.renderingworks.end();work++)
    {
		MTLRenderPassDescriptor *renderpass = [jmetaltransients newRenderPass:work->colortarget depth:work->depthtarget stencil:work->stenciltarget];
		id<MTLCommandBuffer> cmdbuff = [commandqueue commandBuffer];
		id <MTLRenderCommandEncoder> rencoder = [cmdbuff renderCommandEncoderWithDescriptor:renderpass];
		[rencoder setCullMode:MTLCullModeNone];
		[rencoder setVertexBuffer:[jmetalnontransients uniformBufferWithDevice:device].buffer offset:0 atIndex:JBuffer_uniform];
        for(vec_jmetalrenderstategroups::iterator state = work->renderstategroups.begin();state != work->renderstategroups.end();state++)
        {
            [rencoder setRenderPipelineState:state->pipeline];
            
            for(vec_jmetalrenderobj::iterator robj = state->objs.begin();robj!=state->objs.end();robj++)
            {
                jmetalstreambuffer* sbuffer = [jmetalnontransients bufferOfType:robj->vertextype device:nil];
                for(int buffi=0;buffi<sbuffer.vertex.attnum;buffi++)
                {
                    [rencoder setVertexBuffer:sbuffer.vertex.buffers[buffi].buffer offset:0 atIndex: jconstant_bufferidx_of( [sbuffer.vertex getAttribOfBufferAt:buffi] ) ];
                }
				
                for( vec_jmetalbufferalign::iterator align = robj->aligns.begin() ; align != robj->aligns.end() ; align++ )
                {
                    for(int buffi=0;buffi<sbuffer.vertex.attnum;buffi++)
                        [rencoder setVertexBufferOffset:align->voffset * [sbuffer.vertex getUnitSizeOfBufferAt:buffi] atIndex:jconstant_bufferidx_of([sbuffer.vertex getAttribOfBufferAt:buffi])];
					
                    [rencoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:align->icnt indexType:jmetalconstant_indextype(jconstant_index_datatype) indexBuffer:sbuffer.index.buffer.buffer indexBufferOffset:align->ioffset * [sbuffer.index getUnitSize]];
					
                }
				[sbuffer reset];
            }
		}
		[rencoder endEncoding];
		
		[cmdbuff presentDrawable:work->drawable];
		[cmdbuff commit];
	}
	rendertransients.clear();
} 
