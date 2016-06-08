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
#include "jcurve.hpp"


typedef jallocator<simd::float4, 400> jallocatorF4;
typedef jallocator<int, 400> jallocatorInt;

jrenderstategroup renderstateGroups[JRenderState_number];

void makeRenderObjCurve(simd::float2 p1, simd::float2 p2, simd::float2 t1, simd::float2 t2, unsigned long smooth, jrenderobject& renderobj )
{
    simd::float4* poolv = jallocatorF4::getAvailable(smooth+1);
    simd::float4* poolc = jallocatorF4::getAvailable(smooth+1);
    int* pooli = jallocatorInt::getAvailable(smooth*2);
    
    
    float times[2] = {p1[0], p2[0]};
    float values[2] = {p1[1],p2[1]};
    simd::float2 tangents_l[2] = {{0,0},t2};
    simd::float2 tangents_r[2] = {t1,{0,0}};
    jcurve curve;
    curve.cnt = 2;
    curve.times = times;
    curve.values = values;
    curve.tangents_r = tangents_r;
    curve.tangents_l = tangents_l;
    
    for(int i=0;i<smooth;i++)
    {
        float t = i*((times[1]-times[0])/smooth) + times[0];
        simd::float2 valed = curve.evaluate(t);
        poolv[i][0] = valed[0];
        poolv[i][1] = valed[1];
        poolv[i][2] = 0;
        poolv[i][3] = 1;
        
        poolc[i] = {1,0,0,1};
        pooli[i*2 + 0] = i;
        pooli[i*2 + 1] = i+1;
    }
    
    simd::float2 valed = curve.evaluate(times[1]);
    poolv[smooth][0] = valed[0];
    poolv[smooth][1] = valed[1];
    poolv[smooth][2] = 0;
    poolv[smooth][3] = 1;
    
    poolc[smooth] = {1,0,0,1};
    
    renderobj.setData(poolv, poolv, poolc, NULL, (int)smooth+1, pooli, (int)smooth*2);
}

void makeRenderObjFromSkinner( jskinner& skinner, jskeleton& skel, jrenderobject& renderobj )
{
    simd::float4* poolv = jallocatorF4::getAvailable(skinner.clusterCnt);
    simd::float4* poolc = jallocatorF4::getAvailable(skinner.clusterCnt);
    int* pooli = jallocatorInt::getAvailable(skinner.clusterCnt * 2 + 5);
    
    int cnt = 0;
    
    simd::float4 origin = {0,0,0,1};
    for(int i=0;i<skinner.clusterCnt;i++)
    {
        poolc[i] = { 0,1,0,1};
        
        poolv[i] = matrix_multiply(matrix_invert( skinner.inverses[i]), origin);
        int skelidx = skinner.jointIdxs[i];
        int upperskelidx = skel.getTableArr()[skelidx];
        if(upperskelidx == -1)
            continue;
        
        int clusteridxWithUpperSkel = -1;
        for(int j=0;j<skinner.clusterCnt;j++)
            if(skinner.jointIdxs[j] == upperskelidx)
                clusteridxWithUpperSkel = j;
        if(clusteridxWithUpperSkel == -1)
        {
            puts("sdfsasdf1233123");
            exit(1);
        }
        
        pooli[cnt++] = i;
        pooli[cnt++] = clusteridxWithUpperSkel;
    }
    renderobj.setData(poolv, NULL, poolc, NULL, skinner.clusterCnt, pooli, cnt);
}
void mapSkeletonVertices(jskeleton& skel, simd::float4* dst)
{
    int jointcnt = skel.getJointCnt();
    simd::float4 origin = {0,0,0,1};
    matrix_float4x4* jointglobals = skel.computeglobals();
    for(int i=0;i<jointcnt;i++)
    {
        dst[i] = matrix_multiply(jointglobals[i], origin);
        
        if(skel.getTableArr()[i] == -1)
            continue;
    }
}
void makeRenderObjFromSkeleton( jskeleton& skel, jrenderobject& renderobj )
{
	simd::float4* poolv = jallocatorF4::getAvailable(skel.getJointCnt());
	simd::float4* poolc = jallocatorF4::getAvailable(skel.getJointCnt());
	int* pooli = jallocatorInt::getAvailable(256);
	
	int cnt = 0;
	int jointcnt = skel.getJointCnt();
	simd::float4 origin = {0,0,0,1};
    matrix_float4x4* jointglobals = skel.computeglobals();
	for(int i=0;i<jointcnt;i++)
	{
		poolc[i] = { 1,0,0,1};

		poolv[i] = matrix_multiply(jointglobals[i], origin);
		
		if(skel.getTableArr()[i] == -1)
			continue;
		
		pooli[cnt] = i;
		pooli[++cnt] = skel.getTableArr()[i];
		++cnt;
	}
	renderobj.setData(poolv, poolv, poolc, NULL, skel.getJointCnt(), pooli, cnt);
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
typedef jallocator<jnode, 100> jallocatorJnode;

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
            if(renderstateGroups[i].subPrimitiveGroups[j].total > 0)
                (*pfuncRender)( renderstateGroups[i].subPrimitiveGroups[j].min, renderstateGroups[i].subPrimitiveGroups[j].total );
			
		}
	}
}

typedef jallocator<jnode*, 80> jallocskinnedmeshes;

jvideomemorymapper mmapper;
void jcore::initVideoMemoryMapper(void* buffers[], int *_buffIndex)
{
	mmapper.init(buffers, _buffIndex);
}
void jcore::layout()
{
	for(int i=0;i<JRenderState_number;i++)
	{
		for(int j=0;j<JRenderPrimitive_number;j++)
		{
			for(int k=0;k<renderstateGroups[i].subPrimitiveGroups[j].objCnt;k++)
			{
				mmapper.mapToVideoMemory(renderstateGroups[i].subPrimitiveGroups[j].objs[k]->getRenderObject());
			}
            renderstateGroups[i].subPrimitiveGroups[j].calculateMinMax();
		}
	}
    
    for(int i=0;i<jallocskinnedmeshes::getCnt();i++)
    {
        jnode* nodeToSkin = *jallocskinnedmeshes::getAt(i);
        nodeToSkin->computeAndStoreSkinnedPositionTo(mmapper.getPositionMemoryOf(*(nodeToSkin->getRenderObject())));
    }
}

void loadobj(const char* objname, jnode& node, platformSpecificGetFile pgetfile, platformSpecificGetObjInfo pgetobj)
{
    char namemesh[32];
    char nameskel[32];
    const char* extMesh = ".jmesh";
    const char* extJoin = ".jjoin";
    const char* extAnim = ".janim";
    const char* extTable = ".jtable";
    const char* extSkin = ".jskin";
    (*pgetobj)(objname, nameskel, namemesh, 32);
    
    unsigned long size;
    
    char* fileMesh;
    (*pgetfile)(namemesh, extMesh, fileMesh, size);
    char* fileSkin;
    (*pgetfile)(namemesh, extSkin, fileSkin, size);
    
    int vcnt, icnt, *pooli;
    simd::float4 *poolp, *pooln;
    simd::float2 *poolu;
    
    jbinary_jmesh::getInfo(fileMesh, vcnt, icnt, poolp, pooln, poolu, pooli);
    jrenderobject *mesh = jallocatorRenderObjs::getAvailable(1);
    mesh->setData(poolp, pooln, NULL, poolu, vcnt, pooli, icnt);
    
    char* fileJoin;
    (*pgetfile)(nameskel, extJoin, fileJoin, size);
    char* fileAnim;
    (*pgetfile)(nameskel, extAnim, fileAnim, size);
    char* fileTable;
    (*pgetfile)(nameskel, extTable, fileTable, size);
    
    jskeleton* sk = jallocatorSkeleton::getAvailable(1);
    sk->setFromFile(fileTable, fileJoin, fileAnim);
    
    jskinner* skinner = jallocatorSkinner::getAvailable(1);
    jbinary_jskinner::getInfo(fileSkin, skinner->clusterCnt, skinner->inverses, skinner->bindmeshes, skinner->jointIdxs, skinner->linkCounts, skinner->linkIdxs, skinner->linkWeights, skinner->accuTable);
    
    node.setData(mesh, sk, skinner);
    node.precomputeLocals();
}

//jrenderobject* g_skelmesh = NULL;
void jcore::loadAll(platformSpecificGetFile pfunc, platformSpecificGetObjInfo pgetobjinfo)
{
	jnode* lod1 = jallocatorJnode::getAvailable(1);
    loadobj("soldier1", *lod1, pfunc, pgetobjinfo);
    lod1->testtrans(-30, 0, 0);
    
    
    const float term_h = 60;
    const float term_v = 140;
    const int clonenum_r = 16;
    const int clonenum_c = 5;
    for(int ir=0;ir<clonenum_r;ir++)
    {
        for(int ic=0;ic<clonenum_c;ic++)
        {
            jnode* clone = jallocatorJnode::getAvailable(1);
            lod1->clone(*clone);
            //clone->getSkeleton()->animateto((ir*clonenum_c + ic) * 0.06);
            clone->testtrans( ir*term_h - (term_h*(clonenum_r-1))/2, ic*term_v - (term_v*(clonenum_c-1))/2, 0);
            jallocskinnedmeshes::getAvailable(1)[0] = clone;
        }
    }
    
    for(int i=0;i<jallocskinnedmeshes::getCnt();i++)
        renderstateGroups[JRenderState_light].subPrimitiveGroups[JRenderPrimitive_triangle].addObj( *jallocskinnedmeshes::getAt(i) );
    
    
    
	//renderstateGroups[JRenderState_light].subPrimitiveGroups[JRenderPrimitive_triangle].addObj(objnode);
	//jallocatorSkinnedMeshes::getAvailable(1)[0] = objnode;
	
    /*
    jrenderobject* mesh = jallocatorRenderObjs::getAvailable(1);
    makeRenderObjFromSkinner(*(objnode->getSkinner()), *(objnode->getSkeleton()), *mesh);
    node = jallocatorJnode::getAvailable(1);
    node->setData(mesh, NULL, NULL);
    renderstateGroups[JRenderState_info].subPrimitiveGroups[JRenderPrimitive_line].addObj(node);
    
	mesh = jallocatorRenderObjs::getAvailable(1);
	makeRenderObjFromSkeleton(*(objnode->getSkeleton()), *mesh);
    g_skelmesh = mesh;
	node = jallocatorJnode::getAvailable(1);
	node->setData(mesh, NULL, NULL);
	renderstateGroups[JRenderState_info].subPrimitiveGroups[JRenderPrimitive_line].addObj(node);
	*/
	jrenderobject* uiquad = jallocatorRenderObjs::getAvailable(1);
	uiquad->setData(vQuad, nQuad, cQuad, NULL, 4, iQuad, 6);
	jnode* uinode = jallocatorJnode::getAvailable(1);
	uinode->setData(uiquad, NULL, NULL);
	renderstateGroups[JRenderState_ui].subPrimitiveGroups[JRenderPrimitive_triangle].addObj(uinode);
}

void jcore::update()
{
    static float t = 0;
	for(int i=0;i<jallocskinnedmeshes::getCnt();i++)
	{
        jnode* nodeToSkin = *jallocskinnedmeshes::getAt(i);
        if(t >= i*0.2 )
        {
            nodeToSkin->getSkeleton()->advance(0.006);
        }
        if(nodeToSkin->getSkeleton()->isanimating())
            nodeToSkin->computeAndStoreSkinnedPositionTo(mmapper.getPositionMemoryOf(*(nodeToSkin->getRenderObject())));
        //mapSkeletonVertices(*nodeToSkin->getSkeleton(), mmapper.getPositionMemoryOf(*(g_skelmesh)));
	}
    t+=0.1;
}