//
//  main.cpp
//  fbxconverter
//
//  Created by Build on 2015. 10. 30..
//  Copyright © 2015년 Build. All rights reserved.
//

#include <simd/simd.h>
#include <iostream>
#include "fbxsdk.h"
#include <string.h>
#include <fstream>
#include <vector>
#include <list>
#include "jjoint.h"
#include "jrotation.hpp"
using namespace std;

typedef struct _vinfo
{
	int pos;
	int nor;
} vinfo;

std::ofstream *gFile = NULL;
std::ofstream *logfile = NULL;

void startlog(const char* filename)
{
	if(logfile != NULL)
	{
		cout << "log fil enerr not null" << endl;
		exit(1);
	}
	logfile = new std::ofstream(filename, std::ios::out );
}

void writelog(char* content)
{
	if(logfile == NULL)
	{
		cout << "try8ing to log to null" << endl;
		exit(1);
	}
	*logfile << content << endl;
}

void endlog()
{
	if (logfile == NULL)
	{
		cout <<"log file already null" <<endl;
		exit(1);
	}
	logfile->close();
	logfile = NULL;
}

void startfile(char* filename)
{
	if(gFile != NULL)
	{
		cout << "gFile is not null" << endl;
		exit(1);
	}
	gFile = new std::ofstream(filename,std::ios::out | std::ios::binary);
}

void writefile(void* content, unsigned long len)
{
	if(gFile == NULL)
	{
		cout << "gFile is null" << endl;
		exit(1);
	}
	gFile->write((const char *)content, len);
}

template<typename T>
void writefile_copy(T v)
{
	if(gFile == NULL)
	{
		cout << "gFile is null copy" << endl;
		exit(1);
	}
	gFile->write((const char *)&v, sizeof(T));
}

void endfile()
{
	if(gFile == NULL)
	{
		cout << "gFile is null no gfile to close" << endl;
		exit(1);
	}
	gFile->close();
	gFile = NULL;
}

char namebuff[128];
int makename(const char* szfbxname, const char* sznodename, const char* tail, char* buff, int bufflen)
{
	int lastidxdot = 0;
	int fbxfnameidx = 0;
	
	size_t len = strlen(szfbxname);
	
	for(int i=0;i<len;i++)
	{
		if(szfbxname[i] == '.')
			lastidxdot = i;
		if(szfbxname[i] == '\\' || szfbxname[i] == '/')
			fbxfnameidx = i+1;
	}
	
	char purename[64];
	
	memset(purename, 0, sizeof(purename));
	
	for(int i=fbxfnameidx ; i<lastidxdot ; i++)
	{
		purename[i - fbxfnameidx] = szfbxname[i];
	}
	
	
	
	memset(buff, 0, bufflen);
	sprintf(buff, "%s_%s%s", purename, sznodename, tail);
	
	for (char* cp = buff; *cp!=0 ; cp++)
	{
		if('a' <= *cp && *cp <= 'z')
		{
			
		}
		else if('A' <= *cp && *cp <= 'Z')
		{
			
		}
		else if ('0' <= *cp && *cp <= '9')
		{
			
		}
		else if(*cp == '_' || *cp == '.')
		{
			
		}
		else
		{
			*cp = '_';
		}
	}
	return 0;
}

void skelToArr(FbxNode* node, size_t upperIdx, vector<jjoint> &joints, vector<int> &table, vector<FbxNode*>& idxToSkel, int depth)
{
	jjoint joint;
	FbxDouble3 euler = node->LclRotation.Get();
	
	joint.rot.euler( static_cast<float>(euler[0]), static_cast<float>(euler[1]), static_cast<float>(euler[2]) );
	joint.pos.setPos( static_cast<float>(node->LclTranslation.Get()[0]), static_cast<float>(node->LclTranslation.Get()[1]), static_cast<float>(node->LclTranslation.Get()[2]) );
	
	//for(int i=0;i<depth;i++)
	//	cout << "  ";
	//printf("%-20s pos : %.2f, %.2f, %.2f,  rot : %.2f, %.2f, %.2f, %.2f\n", node->GetName(), joint.pos.getPos()[0], joint.pos.getPos()[1], joint.pos.getPos()[2], joint.rot.xyzw[0], joint.rot.xyzw[1], joint.rot.xyzw[2], joint.rot.xyzw[3]);

	size_t idx = joints.size();
	joints.push_back(joint);
	table.push_back( static_cast<int>( upperIdx ));
	idxToSkel.push_back(node);
	
	char jointinfo[128];
	sprintf(jointinfo, "%ld. %s", idx, node->GetName());
	writelog(jointinfo);
	for(int i=0;i<node->GetChildCount();i++)
	{
		skelToArr( node->GetChild(i) , idx, joints, table, idxToSkel, depth + 1);
	}
}

int doanim(FbxScene* scene, vector<FbxNode*>& idxToNodePointer, const char* fbxname, const char* skelname)
{
    int numStacks = scene->GetSrcObjectCount<FbxAnimStack>();
    cout << "animation stack cnt : " << numStacks << endl;
    for(int i=0;i<numStacks;i++)
    {
        FbxAnimStack *animstack = scene->GetSrcObject<FbxAnimStack>(i);
        cout << i << ". stack : " << animstack->GetName() << endl;
        int numLayers = animstack->GetMemberCount<FbxAnimLayer>();
        cout << "\tlayer cnt : " << numLayers << endl;
        for(int j=0;j<numLayers;j++)
        {
            FbxAnimLayer* layer = animstack->GetMember<FbxAnimLayer>(j);
            cout << "\t" << j << ". layer : " << layer->GetName() << endl;
        }
    }
    /*
    makename(fbxname, skelname, ".janim\0", namebuff, sizeof(namebuff));
    startfile(namebuff);
    const int ckeycnt = 4;
    int keycnt = ckeycnt;
    float times[ckeycnt] = {0,4,8,12};
    float values[ckeycnt] = {0,3.14f/2, -3.14f/2, 0};
    simd::float2 tangents_l[ckeycnt] = {{0,0},{1,1},{1,1},{1,1}};
    endfile();
     */
    return 0;
}

void doskel( FbxScene* scene, FbxNode* node, const char* fbxname, vector<FbxNode*>& idxToNodePointer)
{
	vector<jjoint> joints;
	vector<int> table;

	makename(fbxname, node->GetName(), ".log\0", namebuff, sizeof(namebuff));
	startlog(namebuff);
	skelToArr(node, -1, joints, table, idxToNodePointer, 0);
	endlog();
	if(joints.size() != table.size() || table.size() != idxToNodePointer.size())
	{
		cout << "joints and table not match " << joints.size() << " " << table.size() << " " << idxToNodePointer.size() << endl;
		exit(1);
	}

	cout << "joint cnt : " << joints.size() << endl;
	
	int tmp;
	makename(fbxname, node->GetName(),".jtable\0", namebuff, sizeof(namebuff));
	startfile(namebuff);
	tmp = (int)table.size();
	writefile(&tmp, sizeof(int));
	writefile(&table[0], sizeof(int)*table.size());
	endfile();
	
	makename(fbxname, node->GetName(), ".jjoints\0", namebuff, sizeof(namebuff));
	startfile(namebuff);
	writefile(&joints[0], sizeof(jjoint)*joints.size());
	endfile();
    
    doanim(scene, idxToNodePointer, fbxname, node->GetName());
}

/*
 //make sure dat vertexMap.len is equal to vertexlen before triangulation.
 //make sure that idxToNode.len is equal to jointlen.
 */
void doSkin( FbxNode* node, vector<FbxNode*> &idxToNode, const char* fbxname, vector< vector<int>* > &vertexMap, int vertexCntAfterTriangulation)
{
	
	FbxMesh* mesh = node->GetMesh();
	if(mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) != 1)
	{
		cout << node->GetName() << " has " << mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) << " skin deformer." << endl;
		exit(1);
	}
	
	cout << "mesh has " << mesh->GetDeformerCount() << " deformers, " << mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) << " skin deformers." << endl;
	
	
	FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(0, FbxDeformer::EDeformerType::eSkin);
	if(skin == NULL)
	{
		cout << "no skin" << endl;
		exit(1);
	}
	makename(fbxname, node->GetName(),".jskin\0", namebuff, sizeof(namebuff));
	
	vector<int> xClusterCount_jointIdxs;
	vector<int> xClusterCount_cpCnt;
	vector<int> xCpCount_cpIdx;
	vector<float> xCpCount_weights;
	for(int i=0;i<skin->GetClusterCount();i++)
	{
		FbxCluster* cluster = skin->GetCluster(i);
		int joint=-1;
		for (int j=0;j<idxToNode.size();j++)
		{
			if( idxToNode[j] == cluster->GetLink() )
			{
				joint = j;
				break;
			}
		}
		if(joint == -1)
		{
			cout << "link not found " << endl;
			exit(1);
		}
		
		int cpicnt = cluster->GetControlPointIndicesCount();
		
		//TODO : skip meaningless cluster
		//if(cpicnt == 0)
		//	continue;
		
		
		for(int j=0;j<cpicnt;j++)
		{
			if(cluster->GetControlPointIndices()[j] >= mesh->GetControlPointsCount())
			{
				cout << "9239345892359." << endl;
				exit(1);
			}
		}
		
		int vcntofthiscluster = 0;
		for(int j=0;j<cpicnt;j++)
		{
			vector<int>& vmap = *vertexMap[ cluster->GetControlPointIndices()[j] ];
			float weight = cluster->GetControlPointWeights()[j];
			for (vector<int>::iterator it = vmap.begin(); it != vmap.end(); it++)
			{
				vcntofthiscluster++;
				xCpCount_cpIdx.push_back( *it );
				xCpCount_weights.push_back( weight );
			}
		}
		
		xClusterCount_jointIdxs.push_back( joint );
		xClusterCount_cpCnt.push_back( vcntofthiscluster );
	}
	
	/////////////check if it is valid

	if(skin->GetClusterCount() <= 0 || skin->GetClusterCount() > idxToNode.size() || idxToNode.size() == 0)
	{
		cout << "invalid skin..." << endl;
		exit(1);
	}

	for(vector<int>::iterator it = xClusterCount_jointIdxs.begin();it != xClusterCount_jointIdxs.end();it++)
	{
		if(*it >= idxToNode.size())
		{
			cout << "invalid cluster joint" << endl;
			exit(1);
		}
	}

	int* indextable = new int[vertexCntAfterTriangulation];
	memset(indextable, 0, sizeof(int)*vertexCntAfterTriangulation);
	for(vector<int>::iterator it = xCpCount_cpIdx.begin();it!=xCpCount_cpIdx.end();it++)
	{
		indextable[*it]++;
	}
	
	for(int i=0;i<vertexCntAfterTriangulation;i++)
	{
		if(indextable[i] == 0)
		{
			cout << "some vertex are not linked" << endl;
			exit(1);
		}
	}
	
	delete[] indextable;
	vector<float> weighttable;
	for(int i=0;i<vertexCntAfterTriangulation;i++)
		weighttable.push_back(0);
	
	for(int i=0;i<xCpCount_cpIdx.size();i++)
	{
		weighttable[ xCpCount_cpIdx[i] ] += xCpCount_weights[i];
	}
	
	for(vector<float>::iterator it = weighttable.begin();it!=weighttable.end();it++)
	{
		if( 0.9995f > *it || *it > 1.0001f )
		{
			cout << "invalid weight... .. .. .. " << *it << endl;
			exit(1);
		}
	}
	
	/////////////
	
	startfile(namebuff);
	//cluster count, joint index of each cluster, cp count of each cluster, cp indexs, weights.
	writefile_copy<int>(skin->GetClusterCount());
	writefile(&xClusterCount_jointIdxs[0], sizeof(int) * xClusterCount_jointIdxs.size());
	writefile(&xClusterCount_cpCnt[0], sizeof(int) * xClusterCount_cpCnt.size());
	writefile(&xCpCount_cpIdx[0], sizeof(int) * xCpCount_cpIdx.size());
	writefile(&xCpCount_weights[0], sizeof(float) * xCpCount_weights.size());
	endfile();
}

void domesh( FbxNode* node, vector<FbxNode *> &idxToNode, const char* fbxname )
{
	FbxMesh*  mesh = node->GetMesh();
	if(mesh == NULL)
	{
		cout << "mesh is null" << endl;
		exit(1);
	}
	FbxGeometryElementNormal *normalelement = mesh->GetElementNormal();
	
	if(!normalelement)
	{
		puts("no normal!!");
		exit(1);
	}
	
	vector<simd::float4> positions, normals;
	vector<vector<int>*> indices;
	vector<int> indicesFinal;
	vector<vector<int>*> vertexMap;
	
	for(int i=0;i<mesh->GetControlPointsCount();i++)
		vertexMap.push_back(new vector<int>());
	for(int i=0;i<normalelement->GetDirectArray().GetCount();i++)
	{
		
		
		normals.push_back((simd::float4)
						  {
							  static_cast<float>(normalelement->GetDirectArray().GetAt(i)[0]),
							  static_cast<float>(normalelement->GetDirectArray().GetAt(i)[1]),
							  static_cast<float>(normalelement->GetDirectArray().GetAt(i)[2]),
							  0
						  } );
	}
	
	if(normalelement->GetMappingMode() == FbxLayerElement::EMappingMode::eByPolygonVertex)
	{
		if(normalelement->GetReferenceMode() == FbxLayerElement::EReferenceMode::eDirect)
		{
			if(normalelement->GetDirectArray().GetCount() != mesh->GetPolygonVertexCount())
			{
				puts("err err not match idx len");
				exit( 1 );
			}
			
			for(int i=0;i<normalelement->GetDirectArray().GetCount();i++)
			{
				
				
				positions.push_back((simd::float4)
									{
										static_cast<float>(mesh->GetControlPoints()[ mesh->GetPolygonVertices()[i] ][0]),
										static_cast<float>(mesh->GetControlPoints()[ mesh->GetPolygonVertices()[i] ][1]),
										static_cast<float>(mesh->GetControlPoints()[ mesh->GetPolygonVertices()[i] ][2]),
										1
									}
									);
				vertexMap[ mesh->GetPolygonVertices()[i] ]->push_back(i);
			}
			int idxcnt = 0;
			for(int pi = 0;pi<mesh->GetPolygonCount();pi++)
			{
				
				indices.push_back(new vector<int>());
				for(int vi = 0;vi<mesh->GetPolygonSize(pi);vi++)
				{
					indices[pi]->push_back( idxcnt );
					idxcnt++;
				}
			}
		}
		else
		{
			puts("not ready for 34535");
			exit(1);
		}
	}
	else if(normalelement->GetMappingMode() == FbxLayerElement::EMappingMode::eByControlPoint)
	{
		if(normalelement->GetReferenceMode() == FbxLayerElement::EReferenceMode::eDirect)
		{
			if(normalelement->GetDirectArray().GetCount() != mesh->GetControlPointsCount())
			{
				puts("err err not match idx len sef");
				exit(1);
			}
			for(int i=0;i<mesh->GetControlPointsCount();i++)
			{
				positions.push_back((simd::float4)
									{
										static_cast<float>(mesh->GetControlPoints()[ i ][0]),
										static_cast<float>(mesh->GetControlPoints()[ i ][1]),
										static_cast<float>(mesh->GetControlPoints()[ i ][2]),
										1
									}
									);
				vertexMap[i]->push_back(i);//in this case, 1, 2, 3, 4, 5, 6, 7, ....
				
			}
			for(int pi = 0;pi<mesh->GetPolygonCount();pi++)
			{
				indices.push_back(new vector<int>());
				for(int vi = 0;vi<mesh->GetPolygonSize(pi);vi++)
				{
					indices[pi]->push_back( mesh->GetPolygonVertex(pi, vi) );
				}
			}
		}
		else
		{
			printf("not ready for ref mode : %d\n", (int)normalelement->GetReferenceMode());
			exit(1);
		}
	}

	doSkin(node, idxToNode, fbxname, vertexMap, (int)positions.size());
	
	for (vector<vector<int>*>::iterator it = indices.begin(); it!=indices.end(); it++)
	{
		int polysize = (int)(*it)->size();
		vector<int>& polygonsIndices = **it;
		
		if(polysize== 3)
		{
			indicesFinal.push_back( polygonsIndices[0]);
			indicesFinal.push_back( polygonsIndices[1]);
			indicesFinal.push_back( polygonsIndices[2]);
		}
		else if(polysize== 4)
		{
			indicesFinal.push_back( polygonsIndices[0]);
			indicesFinal.push_back( polygonsIndices[1]);
			indicesFinal.push_back( polygonsIndices[2]);
			indicesFinal.push_back( polygonsIndices[2]);
			indicesFinal.push_back( polygonsIndices[3]);
			indicesFinal.push_back( polygonsIndices[0]);
		}
		else
		{
			cout << "not ready for " << (*it)->size() << " poly" << endl;
			exit(1);
		}
	}
	
	makename(fbxname, node->GetName(),".jmesh\0", namebuff, sizeof(namebuff));
	startfile(namebuff);
	writefile_copy<int>((int)positions.size());
	writefile_copy<int>((int)indicesFinal.size());


	writefile(&positions[0], sizeof(simd::float4)*positions.size());
	writefile(&normals[0], sizeof(simd::float4)*normals.size());
	writefile(&indicesFinal[0], sizeof(int) * indicesFinal.size());
	endfile();
}

FbxNode* getRootSkelNode(FbxScene* scene)
{
	for(int i=0;i<scene->GetNodeCount();i++)
	{
		FbxNode* node = scene->GetNode(i);
		
		if(node->GetNodeAttribute() == NULL)
			continue;
		switch (node->GetNodeAttribute()->GetAttributeType())
		{
			case FbxNodeAttribute::EType::eSkeleton:
				
				if(node->GetSkeleton()->IsSkeletonRoot())
					return node;
				break;
			default:
				break;
		}
	}
	
	cout << "no root skel in scene" << endl;
	exit(1);
	return NULL;
}


int doScene(FbxManager* fm, FbxScene* scene, const char* fbxname)
{
	//todo check if skinning also done;
	//do skeleton (make jjoints, jtable, fbxskel to index table)
	FbxNode* rootSkelNode = getRootSkelNode(scene);
	vector<FbxNode*> idxToNodePointer;
	doskel(scene, rootSkelNode, fbxname, idxToNodePointer);
	
	//do mesh. create jmesh and jskin
	for(int i=0;i<scene->GetNodeCount();i++)
	{
		FbxNode* node = scene->GetNode(i);
		
		if(node->GetNodeAttribute() == NULL)
			continue;
		FbxNodeAttribute::EType type = node->GetNodeAttribute()->GetAttributeType();
		
		if (type == FbxNodeAttribute::EType::eMesh)
		{
			domesh(node, idxToNodePointer, fbxname);
			
			
		}
	}
	return 0;
}

int main(int argc, const char * argv[])
{
	/*
	jrotation rotx((simd::float4){1,0,0,0},0.4f);
	jrotation roty((simd::float4){0,1,0,0},0.1f);
	jrotation rotz((simd::float4){0,0,1,0},0.9f);
	
	jrotation eulx, euly, eulz, eulxyz;
	eulx.euler(0.4f, 0, 0);
	euly.euler(0, 0.1f, 0);
	eulz.euler(0, 0, 0.9f);
	
	eulxyz.euler(0.4f, 0.1f, 0.9f);
	jrotation qmul1 = rotx * roty * rotz;
	jrotation qmul2 = rotz * (roty * rotx);
	
	jrotation drotx((simd::float4){1,0,0,0},0.2f);
	drotx = drotx * drotx;
	
	cout << sizeof(jrotation) << ", " << sizeof(simd::float4) << endl;
	*/
	cout << sizeof(jrotation) << endl << sizeof(jjoint) << endl;

#ifndef DEBUG
	cout << "release mode" << endl;
	if(argc != 2)
	{
		std::cout << "argc is not 2" << std::endl;
		return 1;
	}
#else
	cout << "debug mode" << endl;
	char debugfilename[] = "fbxs/test.fbx\0";
	argv[1] = debugfilename;
#endif
	
	std::cout << "filename : " << argv[1] << std::endl;
	
	FbxManager* fm = FbxManager::Create();
	FbxIOSettings *iosetting = FbxIOSettings::Create(fm, IOSROOT);
	
	iosetting->SetAllObjectFlags(true);
	fm->SetIOSettings(iosetting);
	
	FbxImporter *importer = FbxImporter::Create(fm,"");
	
	
	
	if(!importer->Initialize(argv[1],-1, fm->GetIOSettings()))
	{
		std::cout << "can't import" << std::endl;
		return 1;
	}
	
	FbxScene *scene = FbxScene::Create(fm, "scene");
	importer->Import(scene);
	
	char fname[128];
	
	memset(fname, 0, sizeof(fname));
	
	doScene(fm, scene, argv[1]);

	scene->Destroy();
	importer->Destroy();
	iosetting->Destroy();
	fm->Destroy();
	return 0;
}
