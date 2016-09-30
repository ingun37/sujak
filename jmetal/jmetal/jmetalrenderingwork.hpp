//
//  jmetalrenderingwork.h
//  jmetal
//
//  Created by ingun on 9/30/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#import <vector>
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "jmetalrenderstategroups.hpp"

typedef std::vector<jmetalrenderstategroups> vec_jmetalrenderstategroups;
typedef struct _jmetalrenderingwork
{
    id<MTLTexture> colortarget;
    id<MTLTexture> depthtarget;
    id<MTLTexture> stenciltarget;
	id<MTLDrawable> drawable;
    vec_jmetalrenderstategroups renderstategroups;
    _jmetalrenderingwork(id<MTLTexture> colortarget, id<MTLTexture> depthtarget, id<MTLTexture> stenciltarget, id<MTLDrawable> drawable)
    {
        this->colortarget = colortarget;
        this->depthtarget = depthtarget;
        this->stenciltarget = stenciltarget;
		this->drawable = drawable;
    }
	jmetalrenderstategroups* getormakeone(id<MTLRenderPipelineState> pipeline)
	{
		jmetalrenderstategroups* state = NULL;
		
		for(vec_jmetalrenderstategroups::iterator istate = renderstategroups.begin();istate != renderstategroups.end();istate++)
		{
			if(istate->pipeline == pipeline)
			{
				state = &(*istate);
				break;
			}
		}
		
		if(state == NULL)
		{
			renderstategroups.push_back( jmetalrenderstategroups(pipeline) );
			state = &(renderstategroups[renderstategroups.size()-1]);
		}
		return state;
	}
}jmetalrenderingwork;
