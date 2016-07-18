#include <stdlib.h>
#include "jrenderprimitivegroup.hpp"
#include "jnode.hpp"
#include "jrenderobject.hpp"
sujak::jrenderprimitivegroup::jrenderprimitivegroup()
{
	for(int i=0;i<jrenderprimitivegroup_maxcap;i++)
		objs[i] = NULL;
	objCnt = 0;
    min = total = 0;
    
}

void sujak::jrenderprimitivegroup::addObj(jnode *obj)
{
	objs[objCnt++] = obj;
}

void sujak::jrenderprimitivegroup::calculateMinMax()
{
    if(objCnt == 0)
    {
        min=0;
        total=0;
        return;
    }
    unsigned long max=0;
    min = ~0;
    total = 0;
    for(int i=0;i<objCnt;i++)
    {
        unsigned long begin;
        unsigned long end;
        begin = objs[i]->getRenderObject()->aligninfo.iBufferOffset;
        end = begin + objs[i]->getRenderObject()->getIndexCnt();
        total += objs[i]->getRenderObject()->getIndexCnt();
        
        if (min > begin)
            min = begin;
        if (max < end)
            max = end;
    }
    
    if(max - min != total)
    {
        printf("%ld %ld render obj not align",max-min, total);
        exit(1);
    }
}