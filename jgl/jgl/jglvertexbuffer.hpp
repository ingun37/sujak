//
//  jglvertexbuffer.hpp
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglvertexbuffer_hpp
#define jglvertexbuffer_hpp

#include "jconstants.hpp"
#include "jglattributebuffer.hpp"
class jglVertexBuffer : jglAttributeBuffer
{
protected:
	virtual void initAttributeInfo( jglAttributeBufferAttributeInfo& info, int exponentNum );
};
#endif /* jglvertexbuffer_hpp */
