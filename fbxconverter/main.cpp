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

#include "bitmap_image.hpp"
#include "jfbxcustomizer_skin.hpp"
#include "jfbxcustomizer_curve.hpp"

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
    FbxNode* rootskel = NULL;
    FbxNode* skinnedmesh = NULL;
    for(int i=0;i<scene->GetNodeCount();i++)
    {
        FbxNode* node = scene->GetNode(i);
        if(node->GetNodeAttribute() == NULL)
            continue;
        if(node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton )
        {
            if(!node->GetSkeleton()->IsSkeletonRoot())
                continue;
            if(rootskel != NULL)
            {
                puts("jsiofjilsejlfsef");
                exit(1);
            }
            rootskel = node;
        }
        else if(node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            if(node->GetMesh()->GetDeformerCount() > 0)
            {
                if(skinnedmesh != NULL)
                {
                    puts("jaiosrghisgssss");
                    exit(1);
                }
                skinnedmesh = node;
                
            }
        }
    }
    
    
    
    if(rootskel && skinnedmesh)
    {
        jfbxcustomizer_curve curve_customizer(rootskel);
        
        vector<jcurvenode*> curvenodes = curve_customizer.getCurvenodes();
        
        makename("test\0", rootskel->GetName(), ".janim", namebuff, sizeof(namebuff));
        startfile(namebuff);
        for(int i=0;i<curvenodes.size();i++)
        {
            jcurvenode* cnode = curvenodes[i];
            for(int ip = 0;ip<JCURVENODE_PROPERTY_NUMBER;ip++)
                for(int j=0;j<3;j++)
                {
                    jcurve* curve = cnode->getcurveofproperty((JCURVENODE_PROPERTY)ip)[j];
                    if(curve == NULL)
                    {
                        writefile_copy(0);
                        continue;
                    }
                    else
                    {
                        writefile_copy(curve->cnt);
                    }
                
                    if(curve->cnt > 0)
                    {
                        writefile(curve->interpolations, sizeof(curve->interpolations[0]) * curve->cnt);
                        writefile(curve->tangents_l, sizeof(curve->tangents_l[0]) * curve->cnt);
                        writefile(curve->tangents_r, sizeof(curve->tangents_r[0]) * curve->cnt);
                        writefile(curve->times, sizeof(curve->times[0]) * curve->cnt);
                        writefile(curve->values, sizeof(curve->values[0]) * curve->cnt);
                    }
                }
        }
        endfile();
        
        vector<int>& table = curve_customizer.getUpperTable();
        
        makename("test\0", rootskel->GetName(), ".jtable\0", namebuff, sizeof(namebuff));
        startfile(namebuff);
        writefile_copy((int)table.size());
        writefile(&table[0], sizeof(table[0])*table.size());
        endfile();
        
        vector<jjoint>& joints = curve_customizer.getJoints();
        makename("test\0", rootskel->GetName(), ".jjoin\0", namebuff, sizeof(namebuff));
        startfile(namebuff);
        writefile(&joints[0], sizeof(joints[0]) * joints.size());
        endfile();
        
        jfbxcustomizer_skin skin_customizer(rootskel, skinnedmesh->GetMesh());
        
        vector<jvertex> vertices = skin_customizer.getvertices();
        vector<int> indices = skin_customizer.getindices();
        
        vector<simd::float4> positions;
        vector<simd::float4> normals;
        vector<simd::float2> uvs;
        for(int i=0;i<vertices.size();i++)
        {
            positions.push_back( simd::float4 { static_cast<float>(vertices[i].pos[0]),static_cast<float>(vertices[i].pos[1]),static_cast<float>(vertices[i].pos[2]),1 } );
            simd::float3 normal_before = simd::float3{ static_cast<float>(vertices[i].nor[0]), static_cast<float>(vertices[i].nor[1]), static_cast<float>(vertices[i].nor[2]) };
            normal_before = vector_normalize(normal_before);
            normals.push_back( simd::float4{ normal_before[0], normal_before[1], normal_before[2], 0 } );
            uvs.push_back(simd::float2 { static_cast<float>(vertices[i].uv[0]), 1 - static_cast<float>(vertices[i].uv[1]) } );
        }
        
        makename("test\0", skinnedmesh->GetName(), ".jmesh\0", namebuff, sizeof(namebuff));
        startfile(namebuff);
        writefile_copy((int)vertices.size());
        writefile_copy((int)indices.size());
        writefile(&positions[0], sizeof(positions[0]) * positions.size());
        writefile(&normals[0], sizeof(normals[0]) * normals.size());
        writefile(&uvs[0], sizeof(uvs[0]) * uvs.size());
        writefile(&indices[0], sizeof(indices[0]) * indices.size());
        endfile();
        
        vector<jskinjointinfo>skinjointinfos = skin_customizer.getjointinfos();
        
        vector<int> jointidxs;
        vector<matrix_float4x4> inverses;
        vector<matrix_float4x4> bindmeshes;
        vector<int> cpcnts;
        
        vector<int> cpindices;
        vector<float> weights;
        for(int i=0;i<skinjointinfos.size();i++)
        {
            jskinjointinfo& info = skinjointinfos[i];
            jointidxs.push_back(info.jointidx);
            inverses.push_back(info.inverse);
            bindmeshes.push_back(info.bindmesh);
            cpcnts.push_back((int)info.cpinfos.size());
            
            for(int j=0;j<info.cpinfos.size();j++)
            {
                cpindices.push_back(info.cpinfos[j].idx);
                weights.push_back(info.cpinfos[j].weight);
            }
        }
        
        makename("test\0", skinnedmesh->GetName(), ".jskin\0", namebuff, sizeof(namebuff));
        startfile(namebuff);
        writefile_copy((int)jointidxs.size());
        writefile(&jointidxs[0], sizeof(jointidxs[0]) * jointidxs.size());
        writefile(&inverses[0], sizeof(inverses[0]) * inverses.size());
        writefile(&bindmeshes[0], sizeof(bindmeshes[0]) * bindmeshes.size());
        writefile(&cpcnts[0], sizeof(cpcnts[0]) * cpcnts.size());
        writefile(&cpindices[0], sizeof(cpindices[0]) * cpindices.size());
        writefile(&weights[0], sizeof(weights[0]) * weights.size());
        endfile();
        
    }

	scene->Destroy();
	importer->Destroy();
	iosetting->Destroy();
	fm->Destroy();
    
	return 0;
}
