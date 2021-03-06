
#include "jskeleton.hpp"
#include "jallocator.hpp"
#include "jbinary.hpp"
#include "jcurvenode.hpp"
#include "jcurve.hpp"

using namespace sujak;
sujak::jskeleton::jskeleton()
{
	jointnum = 0;
	joints = NULL;
	table = NULL;
    globals = NULL;
    currentkeys = NULL;
    currentt = 0;
    animating = false;
}

typedef jallocator<matrix_float4x4, 3000, jskeleton> poolmat;
typedef jallocator<simd::float3, 128, jskeleton> jallocatorf3;

typedef jallocator<int, 20000, jskeleton> poolkey;

void sujak::jskeleton::setFromFile(char *tableBytes, char *jointBytes, char* animbytes)
{
	jointnum = *(int*)tableBytes;
	table = (int*)(tableBytes + sizeof(int));
	joints = (jjoint*)jointBytes;
    
    jbinary_janim::getInfo(animbytes, curvenodes, jointnum);
    
    globals = poolmat::getAvailable(jointnum);
    
    currentkeys = poolkey::getAvailable(jointnum * JCURVENODE_PROPERTY_NUMBER * 3);
}

typedef jallocator<jjoint, 3000, jskeleton> jallocjoint;
void sujak::jskeleton::clone(jskeleton &skeleton)
{
    jjoint* newjoints = jallocjoint::getAvailable(jointnum);
    
    for(int i=0;i<jointnum;i++)
    {
        newjoints[i] = joints[i];
    }
    
    skeleton = *this;
    skeleton.joints = newjoints;
    skeleton.globals = poolmat::getAvailable(jointnum);
    
    skeleton.currentkeys = poolkey::getAvailable(jointnum*JCURVENODE_PROPERTY_NUMBER * 3);
}

matrix_float4x4* sujak::jskeleton::computeglobals()
{
    //important : up->down ordered array must be guaranteed!!!!!!!!!!!!
    for(int i=0;i<jointnum;i++)
    {
        globals[i] = joints[i].getTransMat();
        if(table[i] == -1)
            continue;
        globals[i] = matrix_multiply( globals[ table[i] ] , globals[i]);
    }
    return globals;
}

void sujak::jskeleton::animateto(float at)
{
    currentt = at;
    for(int ij=0;ij<jointnum;ij++)
    {
        for(int ip=0;ip<JCURVENODE_PROPERTY_NUMBER;ip++)
        {
            for(int iaxis = 0;iaxis<3;iaxis++)
            {
                int idx = (ij * JCURVENODE_PROPERTY_NUMBER * 3) + (ip * 3) + iaxis;
                jcurve* curve = curvenodes[ij].getcurveofproperty((JCURVENODE_PROPERTY)ip)[iaxis];
                
                if(curve == NULL) continue;
                if(curve->cnt == 0) continue;
                
                if (at >  curve->times[curve->cnt-1])
                {
                    currentkeys[idx] = curve->cnt-1;
                }
                else if(at < curve->times[0])
                {
                    currentkeys[idx] = 0;
                }
                else
                {
                    int start;
                    int begin = 0;
                    int end = curve->cnt-1;
                    start = begin + (end-begin)/2;
                    while( !(curve->times[start] <= at && at < curve->times[start+1]) )
                    {
                        if(curve->times[start] < at)
                            begin = start+1;
                        else
                            end = start-1;
                        start = begin + (end-begin)/2;
                    }
                    
                    currentkeys[idx] = start;
                }
            }
        }
    }
}

void sujak::jskeleton::advance(float dt)
{
    animating = false;
    const float radtodeg = 180.f/3.141592;
    const float degtorad = 3.141592/180.f;
    currentt += dt;
    for(int ij=0;ij<jointnum;ij++)
    {
        for(int ip=0;ip<JCURVENODE_PROPERTY_NUMBER;ip++)
        {
            simd::float4 initialval;
            switch((JCURVENODE_PROPERTY)ip)
            {
                case JCURVENODE_PROPERTY_ROTATION:
                    
                    initialval = { joints[ij].rot.xyz[0] * radtodeg  ,joints[ij].rot.xyz[1] * radtodeg, joints[ij].rot.xyz[2] * radtodeg, 0};
                    break;
                case JCURVENODE_PROPERTY_TRANSLATION:
                    initialval = joints[ij].pos.pos;
                    break;
                default:
                    puts("unknown proper");
                    exit(1);
                    break;
            }
            
            for(int iaxis = 0;iaxis<3;iaxis++)
            {
                jcurve* curve = curvenodes[ij].getcurveofproperty((JCURVENODE_PROPERTY)ip)[iaxis];
                
                if(curve == NULL) continue;
                if(curve->cnt == 0) continue;
                if(currentt < curve->times[0]) continue;
                
                int idx = (ij * JCURVENODE_PROPERTY_NUMBER * 3) + (ip * 3) + iaxis;
                
                if ( currentkeys[idx] == curve->cnt-1) continue;
                
                if(curve->times[currentkeys[idx] + 1] < currentt)
                {
                    ++currentkeys[idx];
                }
                
                if ( currentkeys[idx] == curve->cnt-1) continue;
                
                animating = true;
                
                int ckey = currentkeys[idx];
                
                simd::float2 eval;
                float t1 = curve->times[ckey];
                float t2 = curve->times[ckey+1];
                float t;
                simd::float4 T;
                float v1 = curve->values[ckey];
                float v2 = curve->values[ckey+1];
                simd::float2 tan1 = curve->tangents_r[ckey];
                simd::float2 tan2 = curve->tangents_l[ckey+1];
                matrix_float4x2 G;
                
                switch(curve->interpolations[ckey])
                {
                    case JCURVEINTERPOLATION_CUBIC:
                        //todo : no float division. (use bresenham algorithm instead)
                        t = ((float)(currentt-t1))/(t2-t1);
                        
                        T = {1,t,t*t,t*t*t};
                        
                        G = {
                            (simd::float2){static_cast<float>(t1), v1},
                            (simd::float2){static_cast<float>(t2), v2},
                            tan1,
                            tan2,
                        };
                        
                        eval = matrix_multiply(G, matrix_multiply(hermite, T));
                        break;
                    case JCURVEINTERPOLATION_CONSTANT:
                        eval[1] = v1;
                        break;
                    case JCURVEINTERPOLATION_LINEAR:
                        t = ((float)(currentt-t1))/(t2-t1);
                        eval[1] = (v2*t) + (v1*(1-t));
                        break;
                }
                
                initialval[iaxis] = eval[1];
            }
            
            switch ((JCURVENODE_PROPERTY)ip)
            {
                case JCURVENODE_PROPERTY_ROTATION:
                    joints[ij].rot.xyz = {initialval[0] * degtorad, initialval[1] * degtorad, initialval[2] * degtorad};
                    break;
                case JCURVENODE_PROPERTY_TRANSLATION:
                    joints[ij].pos.pos = initialval;
                    break;
                default:
                    exit(1);
                    break;
            }
        }
    }
}
