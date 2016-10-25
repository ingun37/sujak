//
//  jglNonTransientPool.hpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglnontransienttable_hpp
#define jglnontransienttable_hpp
#include <map>

template <typename T>
class jglTableNonTransientMap
{
	bool inited;
	std::map<int, T> m;
protected:
	virtual T makeOf(int k) = 0;
	void initArbitrary(unsigned int num, const int* fixedkeys )
	{
		for(int i=0;i<num;i++)
			set( fixedkeys[i], makeOf(fixedkeys[i]) );
		inited = true;
	}
	void initOrdered(unsigned int num )
	{
		for(int i=0;i<num;i++)
			set( i, makeOf(i) );
		inited = true;
	}
public:
	jglTableNonTransientMap()
	{
		inited = false;
	}
	
	T get(int k)
	{
		if(!inited) throw "90239ildkljskflsdf";
		if(m.find(k) == m.end())
			throw "9w903490334t34t34t43t";
		return m[k];
	}
	void set(int k, T o)
	{
		if(!inited) throw "902sjlfsijldfijlsfsef";
		m[k] = o;
	}
};

#endif /* jglnontransienttable_hpp */
