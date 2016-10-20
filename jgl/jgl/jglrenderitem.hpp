//
//  jglrenderitem.hpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglrenderitem_hpp
#define jglrenderitem_hpp

#include "jdefinitions.hpp"

typedef struct _jglBufferAlign
{
	unsigned int offset;
	unsigned int cnt;
}jglBufferAlign;

class jglRenderItem
{
	sujak::JAttributeGroupVertex vag;
	jglBufferAlign vAlign;
	sujak::JAttributeGroupInstance iag;
	jglBufferAlign instAlign;
	jglBufferAlign idxAlign;
	sujak::JPipeline shader;
	
public:

	jglRenderItem();
};

#endif /* jglrenderitem_hpp */
