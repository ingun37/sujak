//
//  jcore.cpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 4..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//


#include <stdlib.h>
#include "jmath.hpp"
#include "jcore.hpp"
#include "jvideomemorymapper.hpp"
#include "jbinary.hpp"
#include "jrenderstategroup.hpp"
#include "jskeleton.hpp"
#include "jskinner.hpp"
#include "jrenderobject.hpp"
#include "jnode.hpp"

typedef jallocator<simd::float4, 256> jallocatorF4;
typedef jallocator<int, 256> jallocatorInt;

jrenderstategroup renderstateGroups[JRenderState_number];

void makeRenderObjFromSkeleton( jskeleton& skel, jrenderobject& renderobj )
{
	simd::float4* poolv = jallocatorF4::getAvailable(skel.getJointCnt());
	simd::float4* poolc = jallocatorF4::getAvailable(skel.getJointCnt());
	int* pooli = jallocatorInt::getAvailable(256);
	
	int cnt = 0;
	int jointcnt = skel.getJointCnt();
	simd::float4 origin = {0,0,0,1};
	for(int i=0;i<jointcnt;i++)
	{
		poolc[i] = { ((float)(rand()%255))/255,((float)(rand()%255))/255,((float)(rand()%255))/255,1};

		poolv[i] = matrix_multiply(skel.transOfJointAt(i), origin);
		
		if(skel.getTableArr()[i] == -1)
			continue;
		
		pooli[cnt] = i;
		pooli[++cnt] = skel.getTableArr()[i];
		++cnt;
	}
	renderobj.setData(poolv, poolv, poolc, skel.getJointCnt(), pooli, cnt);
}

bool inited = false;

static simd::float4 vLine[] =
{
	{0, 0, 0, 1},
	{40, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 40, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 40, 1}
};

static simd::float4 cLine[] =
{
	{1,0,0,1},
	{1,0,0,1},
	{0,1,0,1},
	{0,1,0,1},
	{0,0,1,1},
	{0,0,1,1}
};

static simd::float4 nLine[] =
{
	{0,0,1,0},
	{0,0,1,0},
	{0,0,1,0},
	{0,0,1,0},
	{0,0,1,0},
	{0,0,1,0},
};

static int iLine[]=
{
	
	0,1,2,3,4,5,
};




static simd::float4 vQuad[] = {
	
	{0.8,	0.8,	-1.01,	1},
	{0.8,	0.85,	-1.01,	1},
	{0.85,	0.85,	-1.01,	1},
	{0.85,	0.8,	-1.01,	1},
};

static simd::float4 nQuad[] = {
	
	{ 0.f, 0.f, 1.0f, 0.f },
	{ 0.f, 0.f, 1.0f, 0.f },
	{ 0.f, 0.f, 1.0f, 0.f },
	{ 0.f, 0.f, 1.0f, 0.f },
};

static simd::float4 cQuad[] = {
	
	{ 0.3f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.3f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.3f, 1.0f },
	{ 0.3f, 0.3f, 0.3f, 1.0f },
};

static int iQuad[] =
{
	0,1,2,
	0,2,3,
};


typedef jallocator<jrenderobject, 8> jallocatorRenderObjs;
typedef jallocator<jskeleton, 4> jallocatorSkeleton;
typedef jallocator<jskinner, 2> jallocatorSkinner;
typedef jallocator<jnode, 4> jallocatorJnode;

typedef jallocator<int, 6000> jallocatorInt6000;
typedef jallocator<float, 6000> jallocatorFloat6000;

jcore::jcore()
{
	if(inited)
	{
		printf("somethingwrong");
		exit(1);
	}
	inited = true;
}

void jcore::render(platformSpecificSetRenderState pfuncRenderState, platformSpecificSetPrimitive pfuncPrim, platformSpecificRenderIndexed pfuncRender)
{
	for(int i=0;i<JRenderState_number;i++)
	{
		(*pfuncRenderState)((JRenderState)i);
		for(int j=0;j<JRenderPrimitive_number;j++)
		{
			(*pfuncPrim)((JRenderPrimitive)j);
			for(int k=0;k<renderstateGroups[i].subPrimitiveGroups[j].objCnt;k++)
			{
				jrenderobject *obj = renderstateGroups[i].subPrimitiveGroups[j].objs[k]->getRenderObject();
				(*pfuncRender)( obj->getIBufferOffset(), obj->getIndexCnt() );
			}
		}
	}
}

typedef jallocator<jnode*, 6> jallocatorSkinnedMeshes;

jvideomemorymapper mmapper;
void jcore::initVideoMemoryMapper(simd::float4 *_buffPosition, simd::float4 *_buffColor, simd::float4 *_buffNormal, int *_buffIndex)
{
	mmapper.init(_buffPosition, _buffColor, _buffNormal, _buffIndex);
}
void jcore::layout()
{
	for(int i=0;i<JRenderState_number;i++)
	{
		for(int j=0;j<JRenderPrimitive_number;j++)
		{
			for(int k=0;k<renderstateGroups[i].subPrimitiveGroups[j].objCnt;k++)
			{
				mmapper.mapToVideoMemory(renderstateGroups[i].subPrimitiveGroups[j].objs[k]->getRenderObject(), renderstateGroups[i].subPrimitiveGroups[j].objs[k]->getRenderObject()->aligninfo);
			}
		}
	}

	for (int i=0; i<jallocatorSkinnedMeshes::getCnt(); i++)
	{
		jnode* meshToInitSkin = *jallocatorSkinnedMeshes::getAt(i);
		const jrenderobject* robj = meshToInitSkin->getRenderObject();
		meshToInitSkin->computeAndStoreSkinnedPositionTo(mmapper.getPositionMemoryOf(*robj));
	}
}

const char* fnameMesh = "girl_BodyGeo\0";
const char* extMesh = ".jmesh\0";
const char* extSkin = ".jskin\0";
const char* fnameSkel = "girl_Mia_Hips\0";
const char* extJoint = ".jjoints\0";
const char* extTable = ".jtable\0";


void jcore::loadAll(platformSpecificGetFile pfunc)
{
	char* file;
	unsigned long size;
	
	
	
	(*pfunc)(fnameSkel, extJoint, file, size);
	char* file2;
	(*pfunc)(fnameSkel, extTable, file2, size);
	jskeleton* sk = jallocatorSkeleton::getAvailable(1);
	sk->setFromFile(file2, file);
	
	(*pfunc)(fnameMesh, extMesh, file, size);
	int vcnt, icnt;
	simd::float4 *poolP, *poolN;
	int* poolI;
	jbinary_jmesh::getInfo(file, vcnt, icnt, poolP, poolN, poolI);
	
	jrenderobject* mesh = jallocatorRenderObjs::getAvailable(1);
	mesh->setData(poolP, poolN, poolN, vcnt, poolI, icnt);
	
	jnode* node = jallocatorJnode::getAvailable(1);
	(*pfunc)(fnameMesh, extSkin, file, size);
	jskinner* skinner = jallocatorSkinner::getAvailable(1);
	jbinary_jskinner::getInfo(file, skinner->clusterCnt, skinner->jointIdxs, skinner->linkCounts, skinner->linkIdxs, skinner->linkWeights, skinner->accuTable);
	node->setData(mesh, sk, skinner);
	renderstateGroups[JRenderState_light].subPrimitiveGroups[JRenderPrimitive_triangle].addObj(node);
	
	jallocatorSkinnedMeshes::getAvailable(1)[0] = node;
	
	mesh = jallocatorRenderObjs::getAvailable(1);
	makeRenderObjFromSkeleton(*sk, *mesh);
	
	node = jallocatorJnode::getAvailable(1);
	node->setData(mesh, sk, NULL);
	renderstateGroups[JRenderState_info].subPrimitiveGroups[JRenderPrimitive_line].addObj(node);
	
	
	jrenderobject* uiquad = jallocatorRenderObjs::getAvailable(1);
	uiquad->setData(vQuad, nQuad, cQuad, 4, iQuad, 6);
	jnode* uinode = jallocatorJnode::getAvailable(1);
	uinode->setData(uiquad, NULL, NULL);
	renderstateGroups[JRenderState_ui].subPrimitiveGroups[JRenderPrimitive_triangle].addObj(uinode);
}

void jcore::update()
{
	static float rot = 0;
	float s = sinf(rot);
	float c = cosf(rot);
	rot += 0.1f;

	jnode* mannode = *jallocatorSkinnedMeshes::getAt(0);
	mannode->getSkeleton()->getjointsArr()[80].rot.euler(c*0.2, s*0.3, 0);
	mannode->getSkeleton()->getjointsArr()[81].rot.euler(0, 0, s*0.4);
	mannode->getSkeleton()->getjointsArr()[1].rot.euler(0, 0, c*0.2);
	mannode->getSkeleton()->getjointsArr()[2].rot.euler(0, 0, s*0.14);
	mannode->getSkeleton()->getjointsArr()[3].rot.euler(0, 0, c*0.22);

	for(int i=0;i<jallocatorSkinnedMeshes::getCnt();i++)
	{
		jnode* nodeToSkin = *jallocatorSkinnedMeshes::getAt(i);
		nodeToSkin->computeAndStoreSkinnedPositionTo(mmapper.getPositionMemoryOf(*(mannode->getRenderObject())));
	}
}