//
//  jglTableNonTransientArray.hpp
//  sujak
//
//  Created by ingun on 25/10/2016.
//
//

#ifndef jglTableNonTransientArray_hpp
#define jglTableNonTransientArray_hpp
#include <vector>

template <typename T>
class jglTableNonTransientArray
{
	bool inited;
	std::vector<T> m;
protected:
	virtual T makeOf(unsigned int k) = 0;
	void init(unsigned int num )
	{
		m.reserve(num);
		for(unsigned int i=0;i<num;i++)
			m.push_back(makeOf(i));
		inited = true;
	}
public:
	jglTableNonTransientArray()
	{
		inited = false;
	}
	
	T get(int k)
	{
		if(!inited) throw "90239ildkljskflsdf";
		if(m.size() <= k) throw "9w903490334t34t34t43t";
		return m[k];
	}
	void set(int k, T o)
	{
		if(!inited) throw "902sjlfsijldfijlsfsef";
		if(m.size() <= k) throw "923239239239239iek";
		m[k] = o;
	}
};

#endif /* jglTableNonTransientArray_hpp */
