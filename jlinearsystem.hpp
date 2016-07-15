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
namespace jlinear
{
    enum JLINEAR_ELEMENTS{A,B,C,D,E,F,G};
    
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
        bool isRowRepresentingElementExist(unsigned int element) const
        {
            for(int i=0;i<C;i++)
                if(rows[i].leadingEntry()==element)
                    return true;
            return false;
        }
        const jlinearrow<R>& rowRepresentingElement(unsigned int element) const
        {
            for(int i=0;i<C;i++)
                if(rows[i].leadingEntry()==element)
                    return rows[i];
            throw "no row representing nth element";
        }
    public:
        jlinearsystem()
        {
            if(C > R)
                throw "C > R not supported";
            rows.reserve(C);
            for(int i=0;i<C;i++)
                rows.push_back(jlinearrow<R>());
        }
        double getAt(unsigned int c, unsigned int r) const
        {
            if(c >= C)
                throw "sefqiqiqiqiooooo";
            return rows[c].getAt(r);
        }
        inline void setrow(unsigned int ri, double e1, double e2, double e3, double e4)
        {
            rows[ri].set(e1, e2, e3, e4);
        }
        inline void setrow(unsigned int ri, double e1, double e2, double e3, double e4, double e5)
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
        
        bool hasSolution() const
        {
            for(int i=0;i<rows.size();i++)
                if(!rows[i].isValid())
                    return false;
            return true;
        }
        bool isElementFreeVariable(unsigned int element) const
        {
            return !isRowRepresentingElementExist(element);
        }
        
        double getCoefficientAt(unsigned int element, unsigned int nth)
        {
            return rowRepresentingElement(element).getCoefficientAt(nth);
        }
        unsigned int getNumberFreeVariableDependencyOfElement(unsigned int element)
        {
            return rowRepresentingElement(element).getNumberFreeVariableDependency();
        }
        double getSumOfFreeVariableCoefficientOfElement(unsigned int element)
        {
            return rowRepresentingElement(element).getSumOfFreeVariableCoefficients();
        }
    };
}
#endif /* jlinearsystem_hpp */
