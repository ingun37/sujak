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
class jglNonTransientTable
{
	std::map<int, T> m;
public:
	T get(int k)
	{
		if(m.find(k) == m.end())
			throw "9w903490334t34t34t43t";
		return m[k];
	}
	void set(int k, T o)
	{
		m[k] = o;
	}
};

#endif /* jglnontransienttable_hpp */
