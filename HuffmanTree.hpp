#pragma once

#include"heap.hpp"
//哈弗曼树：最优二叉树，加权路径长度最短的二叉树【贪心算法】
template<class T>
struct HuffmanNode
{
	HuffmanNode* _left;
	HuffmanNode* _right;
	T _weight;
	HuffmanNode(const T& weight = 0)
		:_left(NULL)
		, _right(NULL)
		, _weight(weight)
	{}
};
template<class T>
class HuffmanTree
{
	typedef HuffmanNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}
	HuffmanTree(const T* a, size_t size, const T& invalid)//构造哈夫曼树,invalid为非法值
	{
		assert(a);
		_root = CreateTree(a, size, invalid);
	}
	Node* GetRootNode()//获得根节点
	{
		return _root;
	}
	~HuffmanTree()
	{
		Clear(_root);
	}
private:
	Node* CreateTree(const T* a, size_t size, const T& invalid)//构造哈夫曼树
	{
		assert(a);
		struct compare
		{
			bool operator()(const Node* r1, const Node* r2)
			{
				return r1->_weight < r2->_weight;
			}
		};
		Heap<Node*, compare> minHeap;//所有元素建成小堆，每次取出最小值进行
		for (size_t i = 0; i < size; ++i)
		{
			if (a[i] != invalid)
			{
				minHeap.Push(new Node(a[i]));
			}
		}
		Node* parent = new Node(0);
		while (minHeap.Size() > 1)
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			minHeap.Push(parent);
		}
		return parent;
	}
	void Clear(Node* root)
	{
		if (root->_left)
			Clear(root->_left);
		if (root->_right)
			Clear(root->_right);
		delete root->_left;
		delete root->_right;
	}
private:
	Node* _root;
};
