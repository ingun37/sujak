//
//  jlinearsystem.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 7/12/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jlinearsystem_hpp
#define jlinearsystem_hpp
#include <simd/simd.h>
#include <vector>
#include <algorithm>
#include "jlinearrow.hpp"

template <unsigned int C, unsigned int R>
class jlinearsystem {
    std::vector<jlinearrow<R>> rows;
    void swapRow(int i1, int i2)
    {
        if(i1==i2)
            return;
        jlinearrow<R> tmp = rows[i1];
        rows[i1] = rows[i2];
        rows[i2] = tmp;
    }
    void topRow(int top, int targetEntry)
    {
        int min = top;
        for(int i=top;i<rows.size();i++)
        {
            if( rows[i].getAt(targetEntry) != 0 )
            {
                min = i;
                break;
            }
            if(rows[i] < rows[min])
                min = i;
        }
        swapRow(top, min);
    }
public:
    jlinearsystem()
    {
        rows.reserve(C);
        for(int i=0;i<C;i++)
            rows.push_back(jlinearrow<R>());
    }
    float getAt(unsigned int c, unsigned int r) const
    {
        if(c >= C)
            throw "sefqiqiqiqiooooo";
        return rows[c].getAt(r);
    }
    inline void setrow(unsigned int ri, float e1, float e2, float e3, float e4)
    {
        rows[ri].set(e1, e2, e3, e4);
    }
    inline void setrow(unsigned int ri, float e1, float e2, float e3, float e4, float e5)
    {
        rows[ri].set(e1, e2, e3, e4, e5);
    }
    void echelonize()
    {
        int lastEntry = -1;
        for(int i=0;i<C;i++)
        {
            topRow(i, lastEntry + 1);
            
            if(rows[i].isZeroRow())
                break;
            
            for(int j=i+1;j<C;j++)
                rows[j].reduceBy(rows[i]);
            
            lastEntry = rows[i].leadingEntry();
        }
    }
    void reducedEchelonize()
    {
        echelonize();
        for(int i=0;i<C;i++)
        {
            if(rows[i].isZeroRow())
                break;
            
            for(int j=0;j<i;j++)
                rows[j].reduceBy(rows[i]);
            
            rows[i].normalizeToLeadingEntry();
        }
    }
    
    bool hasSolution()
    {
        for(int i=0;i<rows.size();i++)
            if(rows[i].coefficientNumber() == 0 && rows[i].last() != 0)
                return false;
        return true;
    }
    
    bool has1Solution()
    {
        if(!hasSolution())
            return false;
        
        for(int i=0;i<rows.size();i++)
            if(rows[i].coefficientNumber() > 1)
                return false;
        return true;
    }
    
    void getSolution(float* solution)
    {
        if(!hasSolution())
            throw "no solution";
        if(!has1Solution())
            throw "more than 1 solution";
        for(int i=0;i<C;i++)
            solution[i] = rows[i].getAt(R-1);
    }
    
    void getColumnAt(int idx, float* column)
    {
        for(int i=0;i<C;i++)
            column[i] = rows[i].getAt(idx);
    }
};

#endif /* jlinearsystem_hpp */
