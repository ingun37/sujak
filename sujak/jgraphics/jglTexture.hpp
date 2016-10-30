//
//  jglTexture.hpp
//  sujak
//
//  Created by Ingun Jon on 30/10/2016.
//
//

#ifndef jglTexture_hpp
#define jglTexture_hpp
#include "jdefinitions.hpp"
class jglTexture
{
	unsigned int width;
	unsigned int height;
	sujak::JPixelFormat format;
protected:
	void init(unsigned int width, unsigned int height, sujak::JPixelFormat format)
	{
		this->width = width;
		this->height = height;
		this->format = format;
	}
	jglTexture(){}
};

#endif /* jglTexture_hpp */
