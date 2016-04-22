//
//  jfbxcustomizer_joint.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jfbxcustomizer_joint.hpp"



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

void setrotationorder(jjoint& joint, FbxNode* node)
{
    switch(node->RotationOrder.Get())
    {
        case FbxEuler::eOrderXYZ: joint.order = JROTATION_ORDER::XYZ; break;
        case FbxEuler::eOrderXZY: joint.order = JROTATION_ORDER::XZY; break;
        case FbxEuler::eOrderYXZ: joint.order = JROTATION_ORDER::YXZ; break;
        case FbxEuler::eOrderYZX: joint.order = JROTATION_ORDER::YZX; break;
        case FbxEuler::eOrderZXY: joint.order = JROTATION_ORDER::ZXY; break;
        case FbxEuler::eOrderZYX: joint.order = JROTATION_ORDER::ZYX; break;
        default: puts("unexpected order"); exit(1); break;
    }
}

vector<jjoint>& jfbxcustomizer_joint::getJoints()
{
    if(joints.size() == 0)
    {
        vector<FbxNode*> skels = getSkels();
        for(int i=0;i<skels.size();i++)
        {
            FbxNode* skel = skels[i];
            
            jjoint joint;
            
            FbxAMatrix localtrans = CalculateLocalTransform(skel);
            
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
            
            setrotationorder(joint, skel);
            
            joint.rot.degree( euler[0], euler[1], euler[2] );
            joint.pos.setPos( trans[0], trans[1], trans[2] );
            
            matrix_float4x4 m1 = joint.getTransMat();
            FbxAMatrix m2 = skel->EvaluateLocalTransform();
            
            for(int j=0;j<4;j++)
            {
                
                for(int k=0;k<4;k++)
                {
                    if(abs(m1.columns[j][k] - m2.GetRow(j)[k]) > 0.0001)
                    {
                        puts("caluclation went wrong");
                        exit(1);
                    }
                }
            }
            
            joints.push_back(joint);
        }
    }
    return joints;
}

jfbxcustomizer_joint::jfbxcustomizer_joint(FbxNode* rootskel) : jfbxcustomizer(rootskel)
{
    
}