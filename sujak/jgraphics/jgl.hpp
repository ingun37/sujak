//
//  jgl.hpp
//  sujak
//
//  Created by ingun on 24/10/2016.
//
//

#ifndef jgl_hpp
#define jgl_hpp

class jgl
{
public:
	unsigned int loadVertexAttributeData(int attribute, const void* data, unsigned int cnt);//retun offset in unit cnt
	unsigned int loadInstanceAttributeData(int attribute, const void* data, unsigned int cnt);//returns offset in unit cnt
};

#endif /* jgl_hpp */
