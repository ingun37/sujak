//
//  jmtlshadertable.cpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtlshadertable.hpp"
#include "jmtlshader.hpp"
#include "jallocator.hpp"

using namespace sujak;
void jmtlShaderTable::init(id<MTLDevice> device)
{
	jglShaderTable::init();
	this->device = device;
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
	jglShaderTable::init();
}
typedef jallocator<jmtlShader, 4, jmtlShaderTable> shaderpool;
jglShader* jmtlShaderTable::makeOf(int key)
{
	jmtlShader* shader = shaderpool::getAvailable(1);
	shader->init((JPipeline)key, device, library);
	return shader;
}
