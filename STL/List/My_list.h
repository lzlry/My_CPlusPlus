#pragma once
#include<assert.h>
namespace lzryl {
	template<class T>
	//因为list中每个结点都要访问，所以不应该写成list的内部类
	struct __list_node
	{
		__list_node<T>* _next;
		__list_node<T>* _prev;
		T _data;

		__list_node(const T& t)
			:_next(nullptr)
			,_prev(nullptr)
			,_data(t)
		{}
		/*
		或者给一个全缺省的默认构造函数
		__list_node(const T& t = T())
			:_next(nullptr)
			,_prev(nullptr)
			,_data(t)
		{}*/
	};

	//用一个类去封装指针，可以在这个类里重载运算符++,--,!=···完成原生指针作迭代器的一切功能
	template<class T, class Ref, class Ptr>
	struct __list_iterator {
		typedef __list_iterator<T, Ref, Ptr> self;
		typedef __list_node<T> Node;//这里要用到模板类__list_node,所以先typedef一下
		Node* _node;//迭代器肯定需要一个结点类型的指针，帮助我们指向结点。
		__list_iterator(Node* node)
			:_node = node
		{}//构造器，用一个结点指针来构造这个iterator
		//下面开始重载一系列的运算符
		Ref operator*() {
			return _node->_data;//返回一个引用类型，就是node里面data的引用
		}
		
		//迭代器++后还是个迭代器，所以返回值类型还是迭代器的类型
		self& operator++() {
			_node = _node->_next;
			return *this;
		}

		self operator++(int) {
			self temp(*this);
			_node = _node->_next;
			return temp;//返回临时变量，不能返回引用，这里只能用传值返回了
		}

		self& operator--() {
			_node = _node->_next;
			return *this;
		}

		self operator--(int) {
			self temp(*this);
			_node = _node->_prev;
			return temp;
		}

		bool operator!=(const self& it)const {
			return _node != it._node;
		}
		bool operator==(const self& it)const {
			return _node == it._node;
		}
	};

	template<class T>
	class list {
		typedef __list_node<T> Node;
		typedef __list_iterator<T, T&, T*> iterator;
		typedef __list_iterator<T, const T&, const T*> const_iterator;
	public:
		list() {
			//带头结点的双向循环链表
			_head = new Node(T());//不能给0，因为不知道里面是什么对象
			//不一定是int,所以直接传递一个T类型的匿名对象
			_head->_next = _head;
			_head->_prev = _head;
		}

		~list(){
		}

		void pop_back() {
			erase(--end());//先--再用，保证是在最后一个元素
		}

		void pop_front() {
			erase(begin());
		}

		iterator begin() {
			return  iterator(_head->_next);
		}
		iterator end() {
			return iterator(_hear->prev);
		}

		void push_back(const T& t) {
			insert(end(), t);
		}

		void push_front(const T& t) {
			insert(begin(), t);
		}

		iterator erase(iterator pos) {
			assert(pos != end());
			Node* curr = pos->_node;
			Node* prev = curr->_prev;
			Node* next = curr->_next;

			prev->_next = next;
			next->_prev = prev;
			delete curr;

			return iterator(next);
			
		}

		size_t size() {
			size_t n = 0;
			iterator it = begin();
			while (it != end()) {
				++it;
				++n;
			}
			return n;
		}

		iterator insert(iterator pos, const T& t) {
			Node* curr = pos._node;
			Node* prev = curr->_prev;
			Node* newNode = new Node(t);

			prev->_next = newNode;
			curr->_prev = newNode;
			newNode->prev = prev;
			newNode->_next = curr;

			return iterator(newNode);
		}
	protected:
		Node* _head;//一个头节点就可以遍历整个list因为底层是双向循环链表
		//类里面只有一个指针，可以保证这个类在栈上空间小一些，如果这是个头节点当然也可以，但是不知道
		//头节点里的data是什么类型，如果这个类型比较大，那么这个类就会很大
	};
}