#pragma once

#include<iostream>
#include<vector>
#include<assert.h>
using namespace std;

template<class T>
struct Less
{
	bool operator()(const T& r1, const T& r2)
	{
		return r1 < r2;
	}
};
//template < class T, template <class>class Compare = Less>
template < class T, class Compare>
class Heap//堆
{
public:
	Heap()
		: _a(NULL)
	{}
	Heap(const T* a, size_t size)
	{
		CreateHeap(a, size);
	}
	void Push(const T& x)//堆尾插入
	{
		_a.push_back(x);
		Adjustup(_a.size() - 1);
	}
	void Pop()//删除堆顶
	{
		assert(_a.size() > 0);
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		Adjustdown(0);
	}
	void CreateHeap(const T* a, size_t size)
	{
		assert(a);
		_a.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			_a.push_back(a[i]);
		}
		//建小堆
		for (int i = (size- 2) / 2; i >= 0; --i)//注意用（size-2）/2
		{
			Adjustdown(i);
		}
	}
	T& Top()//访问堆顶元素
	{
		return _a[0];
	}
	size_t Size()//堆元素个数
	{
		return _a.size();
	}
	void Adjustdown(size_t parent)
	{
		//Compare<T> com;
		Compare com;
		size_t child = parent * 2 + 1;
		while (child < _a.size())
		{
			if (child + 1 < _a.size() && com(_a[child + 1], _a[child]))
			{
				++child;
			}
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	void Adjustup(size_t child)
	{
		//Compare<T> com;
		Compare com;
		size_t parent = (child - 1) / 2;
		while (child > 0)
		{
			if (com(_a[child], _a[parent]))
			{
				swap(_a[parent], _a[child]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}
private:
	vector<T> _a;
};
