//
//  jlinearrow.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 7/12/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jlinearrow_hpp
#define jlinearrow_hpp
#include <vector>
#include <math.h>
using namespace std;

template <unsigned int N>
class jlinearrow {
    friend jlinearrow;
    float row[N];
    
public:
    bool operator< (const jlinearrow& r) const
    {
        return leadingEntry() < r.leadingEntry();
    }
    float& operator[](unsigned int i)
    {
        if(i >= N)
            throw "asefsefiiiiii";
        return row[i];
    }
    
    void operator*=(float f)
    {
        for(int i=0;i<N;i++)
            row[i] *= f;
    }
    void operator/=(float f)
    {
        if(f == 0)
            throw "devivision 0";
        (*this)*=(1.f/f);
    }

    void operator+=(const jlinearrow& r)
    {
        for(int i=0;i<N;i++)
        {
            row[i] += r.row[i];
            if(abs(row[i]) < 0.00001)
                row[i] = 0;
        }
    }

    void operator-=(const jlinearrow& r)
    {
        (*this)+=(-r);
    }
    
    jlinearrow operator*(float f) const
    {
        jlinearrow a(*this);
        a*=f;
        return a;
    }

    jlinearrow operator/(float f) const
    {
        if(f == 0)
            throw "zero division";
        return (*this)*(1.f/f);
    }

    jlinearrow operator+(const jlinearrow& r) const
    {
        jlinearrow a = *this;
        a += r;
        return a;
    }

    jlinearrow operator-() const
    {
        jlinearrow inversed;
        for(int i=0;i<N;i++)
            inversed.setAt(i, -row[i]);
        return inversed;
    }

    jlinearrow operator-(const jlinearrow& r) const
    {
        return (*this) + (-r);
    }

    
    void set(float e, ...)
    {
        row[0] = e;
        va_list vl;
        va_start(vl, e);
        for(int i=1;i<N;i++)
            row[i] = va_arg(vl, double);
        va_end(vl);
    }
    
    unsigned int leadingEntry() const
    {
        int cnt = 0;
        while(row[cnt] == 0)
        {
            if(cnt >= N)
                throw "idx sfjiesjifsijefsefsefsqqqq[[[[";
            cnt++;
        }
        return cnt;
    }

    void normalizeToLeadingEntry()
    {
        if(row[leadingEntry()] == 0)
            throw "aisejifsjeifjsifjsiefoiliil";
        (*this)/=row[leadingEntry()];
    }
    
    bool isZeroRow()
    {
        for(int i=0;i<N;i++)
            if(row[i] != 0)
                return false;
        return true;
    }
 
    bool isEntryZero(unsigned int c)
    {
        return row[c]==0;
    }
    
    void reduceBy(const jlinearrow<N> &r)
    {
        unsigned int entry = r.leadingEntry();
        (*this) -= r*(row[entry]/r.getAt(entry));
    }
    
    float getAt(unsigned int i) const
    {
        return row[i];
    }
    
    void setAt(unsigned int i, float v)
    {
        row[i] = v;
    }
    
    unsigned int coefficientNumber()
    {
        unsigned int d = 0;
        for(int i=0;i<N-1;i++)
            if(row[i] != 0)
                d++;
        return d;
    }
    
    float last()
    {
        return row[N-1];
    }
    
    jlinearrow()
    {
        for(int i=0;i<N;i++)
            row[i] = 0;
    }
    
    jlinearrow(float e1, float e2, float e3, float e4)
    {
        if(N != 4)
            throw "asefoo23233";
        set(e1, e2, e3, e4);
    }
    
    jlinearrow(float e1, float e2, float e3, float e4, float e5)
    {
        if(N != 5)
            throw "jlinearrow e5";
        set(e1, e2, e3, e4, e5);
    }
    
    jlinearrow(float* es)
    {
        for(int i=0;i<N;i++)
            row[i] = es;
    }
    jlinearrow( const vector<float>& ev )
    {
        if(ev.size() >= N)
            throw "soefsjiefse";
        for(int i=0;i<N;i++)
            row[i] = ev[i];
    }
};

#endif /* jlinearrow_hpp */
