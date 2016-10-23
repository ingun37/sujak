//
//  jmetalrenderstategroups.h
//  jmetal
//
//  Created by Ingun Jon on 10/4/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include <vector>
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "jmetalrenderobj.hpp"
typedef std::vector<jmetalrenderobj> vec_jmetalrenderobj;
typedef struct _jmetalrenderstategroups
{
    id<MTLRenderPipelineState> pipeline;
    vec_jmetalrenderobj objs;
    _jmetalrenderstategroups(id<MTLRenderPipelineState> pipeline)
    {
		if(pipeline == nil)
		{
			[NSException raise:@"pipeline is null" format:@""];
		}
        this->pipeline = pipeline;
    }
	jmetalrenderobj* getormakeone(int vertextype)
	{
		jmetalrenderobj* obj = NULL;
		for(vec_jmetalrenderobj::iterator iobj = objs.begin();iobj!=objs.end();iobj++)
		{
			if(iobj->vertextype == vertextype)
			{
				obj = &(*iobj);
				break;
			}
		}
		
		if(obj == NULL)
		{
			objs.push_back( jmetalrenderobj(vertextype) );
			obj = &(objs[objs.size()-1]);
		}
		return obj;
	}
}jmetalrenderstategroups;
