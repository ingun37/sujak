//
//  jmetalrendertransients.h
//  jmetal
//
//  Created by ingun on 10/4/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "jmetalrenderingwork.hpp"
typedef std::vector<jmetalrenderingwork> vec_jmetalrenderingwork;

class jmetalrendertransients
{
public:
	vec_jmetalrenderingwork renderingworks;

	jmetalrenderingwork* getormake( id<MTLTexture> color, id<MTLTexture> depth, id<MTLTexture> stencil, id<MTLDrawable> drawable);
	void clear();
};
