#pragma once

#include "../sdk.hpp"

template< class T, class A = int>
class c_utl_vector
{
public:
	int get_size() { return size; }
	int get_size_const() const { return size; }
	T get_element(int i) { return data[i]; }
	T get_element_const(int i) const { return data[i]; }
protected:
	int64_t size;
	T* data;
};