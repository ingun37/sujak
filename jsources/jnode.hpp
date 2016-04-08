//
//  jnode.hpp
//  MetalVertexStreaming
//
//  Created by Build on 12/7/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef jnode_hpp
#define jnode_hpp

#include <stdio.h>
#include <simd/simd.h>
class jrenderobject;
class jskeleton;
class jskinner;
class jnode
{
	jrenderobject* renderobj;
	jskeleton* skeleton;
	jskinner* skinner;
	
public:
	bool shouldRemap;//todo : bitfield
	
	jnode()
	{
		renderobj = NULL;
		skeleton = NULL;
		skinner = NULL;
		shouldRemap = false;
	}
	
	inline void setData( jrenderobject* r, jskeleton* s, jskinner* k )
	{
		renderobj = r;
		skeleton = s;
		skinner = k;
	}
	inline jrenderobject* getRenderObject() const {return renderobj;}
	inline jskeleton* getSkeleton() const {return skeleton;}
	inline jskinner* getSkinner() const {return skinner;}

	void computeAndStoreSkinnedPositionTo(simd::float4* dest);
};
#endif /* jnode_hpp */
