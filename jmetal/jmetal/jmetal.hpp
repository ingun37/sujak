//
//  jmetal.h
//  jmetal
//
//  Created by ingun on 7/20/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "jmetalconstants.hpp"

@class jmetalrendercontextdata;

class jmetal
{
	MTLRenderPassDescriptor *renderpassdesc;
    jmetalrendercontextdata* rendercontextdatas[sujak::JRenderContext_number];
	id<MTLCommandQueue> commandqueue;
	id<MTLTexture> rtDepth;
	id<MTLTexture> rtStencil;
public:
    void addRenderContext(int bindbufferidx, MTLVertexFormat format);
    virtual void init(int width, int height);
	virtual void loadObject(sujak::JRenderContext state, sujak::jrenderobject* obj);
    virtual void render(id<MTLTexture> rendertarget);
};
