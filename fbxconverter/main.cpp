//
//  main.cpp
//  fbxconverter
//
//  Created by Build on 2015. 10. 30..
//  Copyright © 2015년 Build. All rights reserved.
//

#include <math.h>
#include <simd/simd.h>
#include <iostream>
#include "fbxsdk.h"
#include <string.h>
#include <fstream>
#include <vector>
#include <list>
#include "jjoint.h"
#include "jrotation.hpp"
#include "jcurve.hpp"
#include "bitmap_image.hpp"

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

FbxAMatrix CalculateLocalTransform(FbxNode* pNode)
{
    FbxAMatrix lTranlationM, lScalingM, lScalingPivotM, lScalingOffsetM, lRotationOffsetM, lRotationPivotM, \
    lPreRotationM, lRotationM, lPostRotationM, lTransform;
    
    FbxAMatrix lLocalT, lLocalRS;
    
    if(!pNode)
    {
        lTransform.SetIdentity();
        return lTransform;
    }
    
    // Construct translation matrix
    FbxVector4 lTranslation = pNode->LclTranslation.Get();
    lTranlationM.SetT(lTranslation);
    
    // Construct rotation matrices
    FbxVector4 lRotation = pNode->LclRotation.Get();
    FbxVector4 lPreRotation = pNode->PreRotation.Get();
    FbxVector4 lPostRotation = pNode->PostRotation.Get();
    lRotationM.SetR(lRotation);
    lPreRotationM.SetR(lPreRotation);
    lPostRotationM.SetR(lPostRotation);
    
    // Construct scaling matrix
    FbxVector4 lScaling = pNode->LclScaling.Get();
    lScalingM.SetS(lScaling);
    
    // Construct offset and pivot matrices
    FbxVector4 lScalingOffset = pNode->ScalingOffset.Get();
    FbxVector4 lScalingPivot = pNode->ScalingPivot.Get();
    FbxVector4 lRotationOffset = pNode->RotationOffset.Get();
    FbxVector4 lRotationPivot = pNode->RotationPivot.Get();
    lScalingOffsetM.SetT(lScalingOffset);
    lScalingPivotM.SetT(lScalingPivot);
    lRotationOffsetM.SetT(lRotationOffset);
    lRotationPivotM.SetT(lRotationPivot);
    
    FbxAMatrix lLRM;
    lLRM = lPreRotationM * lRotationM * lPostRotationM;
    
    FbxAMatrix lLSM;
    lLSM = lScalingM;
    
    lLocalRS = lLRM * lLSM;
    
    // Construct translation matrix
    // Calculate the local transform matrix
    lTransform = lTranlationM * lRotationOffsetM * lRotationPivotM * lPreRotationM * lRotationM * lPostRotationM * lRotationPivotM.Inverse()\
    * lScalingOffsetM * lScalingPivotM * lScalingM * lScalingPivotM.Inverse();
    FbxVector4 lLocalTWithAllPivotAndOffsetInfo = lTransform.GetT();
    
    lLocalT.SetT(lLocalTWithAllPivotAndOffsetInfo);
    
    //Construct the whole global transform
    lTransform = lLocalT * lLocalRS;
    
    return lTransform;
}
void skelToArr(FbxNode* node, int upperIdx, vector<int> &table, vector<FbxNode*>& idxToSkel, int depth)
{
    //printf("rot : %f  %f  %f\n", node->LclRotation.Get()[0], node->LclRotation.Get()[1], node->LclRotation.Get()[2]);
    //if(node->LclTranslation.GetCurveNode() != NULL)
    //    printf("%s has curvenode\n", node->GetName());
		
	table.push_back( upperIdx );
	idxToSkel.push_back(node);
	
    int curridx = (int)idxToSkel.size() - 1;
	char jointinfo[128];
    sprintf(jointinfo, "%d. %s\n", curridx, node->GetName());
	writelog(jointinfo);
	for(int i=0;i<node->GetChildCount();i++)
	{
		skelToArr( node->GetChild(i) , curridx, table, idxToSkel, depth + 1);
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
            int nodecnt = layer->GetMemberCount<FbxAnimCurveNode>();
            for(int inode =0;inode<nodecnt;inode++)
            {
                FbxAnimCurveNode* node = layer->GetMember<FbxAnimCurveNode>(inode);
                FbxProperty prop = node->GetDstProperty();
                FbxAnimCurve* curve = prop.GetCurve(layer);
                if(curve)
                {
                    if(curve->KeyGetCount() > 0)
                    {
                        FbxString propname = prop.GetName();
                        FbxObject* obj = prop.GetFbxObject();
                        //printf("%s %s\n",propname.Buffer(), obj->GetName());
                    }
                }
            }
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

template <typename T>
void copyVectorTo(vector<T>& vec, T* p)
{
    memcpy(p, &vec[0], sizeof(T) * vec.size());
}

void makejcurveFromFbxcurve(jcurve& dst, FbxAnimCurve* src)
{
    dst.cnt = src->KeyGetCount();
    
    vector<JCURVEINTERPOLATION> myinterpolations;
    vector<float> mytimes;
    vector<float> myvalues;
    
    for(int j=0;j<dst.cnt;j++)
    {
        mytimes.push_back(src->KeyGetTime(j).GetSecondDouble());
        myvalues.push_back(src->KeyGetValue(j));
    }
    
    for(int j=0;j<dst.cnt-1;j++)
    {
        if(mytimes[j] == mytimes[j+1])
        {
            printf(" at %d time aliasing fail\n", j);
            exit(1);
        }
    }
    
    vector<simd::float2> mytangents_l;
    vector<simd::float2> mytangents_r;
    
    for(int j=0;j<dst.cnt;j++)
    {
        simd::float2 tin = {0,0};
        simd::float2 tout = {0,0};
        
        FbxAnimCurveKey key = src->KeyGet(j);
        
        if(key.GetInterpolation() == FbxAnimCurveDef::EInterpolationType::eInterpolationCubic)
        {
            myinterpolations.push_back(JCURVEINTERPOLATION_CUBIC);
            
            float tension = key.GetDataFloat(FbxAnimCurveDef::EDataIndex::eTCBTension);
            float continuity = key.GetDataFloat(FbxAnimCurveDef::EDataIndex::eTCBContinuity);
            float bias = key.GetDataFloat(FbxAnimCurveDef::EDataIndex::eTCBBias);
            
            auto relpos = [&](int idx)
            {
                int relidx = j + idx;
                if (0 > relidx || relidx >= src->KeyGetCount())
                {
                    return (simd::float2){0,0};
                }
                return (simd::float2){static_cast<float>(mytimes[relidx]), myvalues[relidx]};
            };
            
            simd::float2 prevp = relpos(-1);
            simd::float2 currp = relpos(0);
            simd::float2 nextp = relpos(1);
            
            float tcb1;
            float tcb2;
            
            if(j==0)
            {
                tout = nextp - currp;
            }
            else if(j==dst.cnt-1)
            {
                tin = currp - prevp;
            }
            else
            {
                tcb1 = (1-tension)*(1+continuity)*(1-bias)/2;
                tcb2 = (1-tension)*(1-continuity)*(1+bias)/2;
                
                tin = (tcb1*(nextp-currp))+(tcb2*(currp-prevp));
                
                tcb1 = (1-tension)*(1-continuity)*(1-bias)/2;
                tcb2 = (1-tension)*(1+continuity)*(1+bias)/2;
                
                tout = (tcb1*(nextp-currp))+(tcb2*(currp-prevp));
            }
            
            if(tin[0] < 0 || tout[0] < 0)
            {
                puts("negative x in tout and tin\n");
                exit(1);
            }
        }
        else if(key.GetInterpolation() == FbxAnimCurveDef::eInterpolationLinear)
        {
            myinterpolations.push_back(JCURVEINTERPOLATION_LINEAR);
        }
        else if(key.GetInterpolation() == FbxAnimCurveDef::eInterpolationConstant)
        {
            myinterpolations.push_back(JCURVEINTERPOLATION_CONSTANT);
        }
        mytangents_l.push_back(tin);
        mytangents_r.push_back(tout);
    }
    
    if( dst.cnt != myinterpolations.size() ||
       myinterpolations.size() != mytimes.size() ||
       mytimes.size() != myvalues.size() ||
       myvalues.size() != mytangents_l.size() ||
       mytangents_l.size() != mytangents_r.size() )
    {
        printf("len err\n");
        exit(1);
    }

    dst.interpolations = new JCURVEINTERPOLATION[dst.cnt];
    copyVectorTo(myinterpolations, dst.interpolations);
    
    dst.times = new float[dst.cnt];
    copyVectorTo(mytimes, dst.times);

    dst.values = new float[dst.cnt];
    copyVectorTo(myvalues, dst.values);

    dst.tangents_l = new simd::float2[dst.cnt];
    copyVectorTo(mytangents_l, dst.tangents_l);
    
    dst.tangents_r = new simd::float2[dst.cnt];
    copyVectorTo(mytangents_r, dst.tangents_r);
}

void writeCurveChannelKeyCnt(FbxNode* skel)
{
    char flag = 0;
    FbxAnimCurveNode* cnode = skel->LclRotation.GetCurveNode();
    FbxStringCompare comparer;
    const FbxString x("X"), y("Y"), z("Z");
    
    for(int i=0;i<cnode->GetChannelsCount();i++)
    {
        if (comparer( cnode->GetChannelName(i), x ) == 0)
            flag |= 1;
        else if(comparer( cnode->GetChannelName(i), y ) == 0)
            flag |= 2;
        else if(comparer( cnode->GetChannelName(i), z ) == 0)
            flag |= 4;
        else
        {
            printf("not ready for channel name : %s\n", cnode->GetChannelName(i).Buffer());
            exit(1);
        }
    }

    unsigned char keycnts[3] = {0,0,0};
    
    int channelid = 0;
    for(int i=0;i<3;i++)
    {
        if (flag & 1 << i)
        {
            FbxAnimCurve* curve = cnode->GetCurve(channelid++);
            if(curve == NULL)
                continue;
            
            if(curve->KeyGetCount() > 255)
            {
                puts("not ready for key over 255");
                exit(1);
            }
            
            keycnts[i] = (unsigned char)curve->KeyGetCount();
        }
    }
    writefile(keycnts, sizeof(unsigned char) * 3);
    //printf("%s : keycnt : %d %d %d\n", skel->GetName(), keycnts[0], keycnts[1], keycnts[2]);
}

void diffimage(jcurve& c1, FbxAnimCurve* c2, const char* imgname)
{
    const int w = 1600;
    const int h = 1600;
    const int hh = h/2;
    bitmap_image img(w+16,h);
    img.clear();
    image_drawer drawer(img);
    
    char cr = 244;
    char cg = 0;
    char cb = 0;
    
    int prevx = 0;
    int prevy = c1.evaluate(c1.times[0]) + hh;
    
    const int smooth = 140;
    
    //printf("curve keycnt : %d\n", curve->KeyGetCount());
    for(int j=1;j<=smooth;j++)
    {
        float ratio = ((float)j)/smooth;
        int currx = (float)w * ratio;
        
        int curry = (c1.evaluate( c1.getTimeInterval(ratio) ) * 10) + hh;
        curry = min(max(curry,10),h-10);
        
        drawer.pen_color(0, 244, 0);
        drawer.line_segment(prevx, prevy-4, currx, curry-4);
        
        prevx = currx;
        prevy = curry;
    }
    
    FbxTimeSpan span;
    c2->GetTimeInterval(span);
    
    
    prevx = 0;
    prevy = c2->Evaluate(span.GetStart()) + hh;
    //printf("curve keycnt : %d\n", curve->KeyGetCount());
    for(int j=1;j<=smooth;j++)
    {
        float ratio = ((float)j)/smooth;
        int currx = (float)w * ratio;
        
        double t_in_double = span.GetStart().GetSecondDouble() + (span.GetDuration().GetSecondDouble() * ratio);
        FbxTime t;
        t.SetSecondDouble(t_in_double);
        float val = c2->Evaluate(t);
        //printf("%.5f : %f, %f, %f * %f\n", t.GetSecondDouble(), val, span.GetStart().GetSecondDouble(), span.GetDuration().GetSecondDouble(), ratio);
        int curry = (val * 10) + hh;
        curry = min(max(curry,10),h-10);
        
        cb = ratio * 255;
        drawer.pen_color(cr, cg, cb);
        drawer.line_segment(prevx, prevy, currx, curry);
        
        prevx = currx;
        prevy = curry;
    }

    makename("graph", imgname, ".png", namebuff, sizeof(namebuff));
    img.save_image(namebuff);
}

void writeCurveInfo(FbxNode* skel)
{
    FbxAnimCurveNode* cnode = skel->LclRotation.GetCurveNode();
    for(int i=0;i<cnode->GetChannelsCount();i++)
    {
        FbxAnimCurve* curve = cnode->GetCurve(i);
        if(curve == NULL)
            continue;
        
        jcurve mycurve;
        
        makejcurveFromFbxcurve(mycurve, curve);
        
        writefile(mycurve.interpolations, sizeof(mycurve.interpolations[0]) * mycurve.cnt);
        writefile(mycurve.times, sizeof(mycurve.times[0]) * mycurve.cnt);
        writefile(mycurve.values, sizeof(mycurve.values[0]) * mycurve.cnt);
        writefile(mycurve.tangents_l, sizeof(mycurve.tangents_l[0]) * mycurve.cnt);
        writefile(mycurve.tangents_r, sizeof(mycurve.tangents_r[0]) * mycurve.cnt);
    }
}

void doskel( FbxScene* scene, FbxNode* node, const char* fbxname, vector<FbxNode*>& idxToNodePointer)
{
	vector<jjoint> joints;
	vector<int> table;

	makename(fbxname, node->GetName(), ".log\0", namebuff, sizeof(namebuff));
	startlog(namebuff);
	skelToArr(node, -1, table, idxToNodePointer, 0);
    endlog();
    
    for(int i=0;i<idxToNodePointer.size();i++)
    {
        jjoint joint;
        //FbxDouble3 euler = node->LclRotation.Get();
        FbxAMatrix realrotm = idxToNodePointer[i]->EvaluateLocalTransform(0);
        FbxVector4 realrotv = realrotm.GetR();
        FbxAMatrix rotm;
        rotm.SetR(idxToNodePointer[i]->LclRotation.EvaluateValue(0));
        FbxAMatrix scalm;
        scalm.SetS(idxToNodePointer[i]->GeometricScaling.EvaluateValue(0));
        
        FbxVector4 rotv = (rotm * scalm).GetR();
        printf("%d. %f %f %f -> %s\n", i, rotv[0] - realrotv[0], rotv[1] - realrotv[1], rotv[2] - realrotv[2], idxToNodePointer[i]->GetName());
        FbxVector4 grot = idxToNodePointer[i]->GeometricRotation.EvaluateValue(0);
        //printf("geo %f %f %f\n", grot[0],grot[1],grot[2]);
        FbxAMatrix localtrans = CalculateLocalTransform(idxToNodePointer[i]);
        
        FbxVector4 euler = localtrans.GetR();
        FbxVector4 trans = localtrans.GetT();
        FbxVector4 scale = localtrans.GetS();
        
        //todo scale
        if( 1.0001f < scale[0] || 1.0001f < scale[1] || 1.0001f < scale[2] ||
           0.9999f > scale[0] || 0.9999f > scale[1] || 0.9999f > scale[2] )
        {
            puts("not ready for... scale");
            exit(1);
        }
        
        joint.rot.euler( static_cast<float>(euler[0]*(3.141592/180)), static_cast<float>(euler[1]*(3.141592/180)), static_cast<float>(euler[2]*(3.141592/180)) );
        joint.pos.setPos( static_cast<float>(trans[0]), static_cast<float>(trans[1]), static_cast<float>(trans[2]) );
        
        joints.push_back(joint);
    }
	
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
    
    makename(fbxname, node->GetName(), ".janim", namebuff, sizeof(namebuff));
    startfile(namebuff);
    
    for(int i=0;i<idxToNodePointer.size();i++)
        writeCurveChannelKeyCnt(idxToNodePointer[i]);
    for(int i=0;i<idxToNodePointer.size();i++)
        writeCurveInfo(idxToNodePointer[i]);
    endfile();
}

/*
 //make sure dat vertexMap.len is equal to vertexlen before triangulation.
 //make sure that idxToNode.len is equal to jointlen.
 */
void doSkin( FbxNode* node, vector<FbxNode*> &idxToNode, const char* fbxname, vector< vector<int>* > &vertexMap, int vertexCntAfterTriangulation)
{
	
	FbxMesh* mesh = node->GetMesh();
	if(mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) > 1)
	{
		cout << node->GetName() << " has " << mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) << " skin deformer." << endl;
		exit(1);
	}
    else if(mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) ==0)
    {
        cout << node->GetName() << " has " << mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) << " skin deformer. skipping." << endl;
        return;
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
    vector<matrix_float4x4> xClusterCount_inverses;
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
        FbxAMatrix linktrans;
        cluster->GetTransformLinkMatrix(linktrans);
        FbxAMatrix meshtrans;
        cluster->GetTransformMatrix(meshtrans);
        //printf("%s link transform : %f %f %f\n",cluster->GetLink()->GetName(), linktrans.GetT()[0], linktrans.GetT()[1], linktrans.GetT()[2]);
        //printf("mesh trlation : %f %f %f : %s \n", meshtrans.GetT()[0], meshtrans.GetT()[1], meshtrans.GetT()[2],cluster->GetLink()->GetName());
        //printf("mesh rotation : %f %f %f : %s\n", meshtrans.GetR()[0], meshtrans.GetR()[1], meshtrans.GetR()[2],cluster->GetLink()->GetName());
        FbxVector4 bindt = linktrans.GetT();
        FbxVector4 bindr = linktrans.GetR();
        FbxVector4 binds = linktrans.GetS();
        if (binds[0] > 1.0001f || binds[1] > 1.0001f || binds[2] > 1.0001f ||
            binds[0] < 0.9999f || binds[1] < 0.9999f || binds[2] < 0.9999f)
        {
            puts("not ready for scale");
            exit(1);
        }
            
        jtranslation jt;
        jt.setPos(bindt[0], bindt[1], bindt[2]);
        jrotation jr;
        jr.euler(bindr[0], bindr[1], bindr[2]);
        //TODO : scale
        
        matrix_float4x4 bindx = matrix_multiply(jt.getMat(), jr.toMat());
        bindx = matrix_invert(bindx);
        
        xClusterCount_inverses.push_back(bindx);
        
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
    writefile(&xClusterCount_inverses[0], sizeof(xClusterCount_inverses[0]) * xClusterCount_inverses.size());
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
        else if(polysize == 5)
        {
            indicesFinal.push_back( polygonsIndices[0]);
            indicesFinal.push_back( polygonsIndices[1]);
            indicesFinal.push_back( polygonsIndices[2]);
            
            indicesFinal.push_back( polygonsIndices[0]);
            indicesFinal.push_back( polygonsIndices[2]);
            indicesFinal.push_back( polygonsIndices[3]);
            
            indicesFinal.push_back( polygonsIndices[0]);
            indicesFinal.push_back( polygonsIndices[3]);
            indicesFinal.push_back( polygonsIndices[4]);
        }
		else
		{
			cout << "not ready for " << (*it)->size() << " poly" << endl;
			exit(1);
		}
	}
	
    
    if(positions.size() != normals.size())
    {
        printf("len not same postio norm\n");
        exit(1);
    }
    
    FbxAMatrix localtrans = CalculateLocalTransform(node);
    
    for(int i=0;i<positions.size();i++)
    {
        FbxVector4 pos;
        pos.Set(positions[i][0], positions[i][1], positions[i][2]);
        pos = localtrans.MultT(pos);
        positions[i] = simd::float4{static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]), 1};
        
        FbxVector4 nor;
        nor.Set(normals[i][0], normals[i][1], normals[i][2], 0);
        FbxVector4 rot = localtrans.GetR();
        FbxAMatrix rM;
        rM.SetR(rot);
        nor = rM.MultT(nor);
        normals[i] = simd::float4{static_cast<float>(nor[0]), static_cast<float>(nor[1]), static_cast<float>(nor[2]), 0};
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
	char debugfilename[] = "fbxs/soldier.fbx\0";
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
