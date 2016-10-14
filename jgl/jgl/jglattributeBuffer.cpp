//
//  jglattributebuffer.cpp
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jglattributebuffer.hpp"

jglAttributeBuffer::jglAttributeBuffer(int flag, unsigned int cnt)
{
	this->flag = flag;
	this->cnt = cnt;
	
	for(int i=0;i<JGLATTRIBUTEBUFFER_MAXATT_CNT;i++)
	{
		if(!(flag & (1 << i)))
			continue;
		initAttributeInfo( attributes[i], i );
	}
}
