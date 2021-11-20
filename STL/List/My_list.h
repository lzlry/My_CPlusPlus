#pragma once
#include<assert.h>
namespace lzryl {
	template<class T>
	//��Ϊlist��ÿ����㶼Ҫ���ʣ����Բ�Ӧ��д��list���ڲ���
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
		���߸�һ��ȫȱʡ��Ĭ�Ϲ��캯��
		__list_node(const T& t = T())
			:_next(nullptr)
			,_prev(nullptr)
			,_data(t)
		{}*/
	};

	//��һ����ȥ��װָ�룬����������������������++,--,!=���������ԭ��ָ������������һ�й���
	template<class T, class Ref, class Ptr>
	struct __list_iterator {
		typedef __list_iterator<T, Ref, Ptr> self;
		typedef __list_node<T> Node;//����Ҫ�õ�ģ����__list_node,������typedefһ��
		Node* _node;//�������϶���Ҫһ��������͵�ָ�룬��������ָ���㡣
		__list_iterator(Node* node)
			:_node = node
		{}//����������һ�����ָ�����������iterator
		//���濪ʼ����һϵ�е������
		Ref operator*() {
			return _node->_data;//����һ���������ͣ�����node����data������
		}
		
		//������++���Ǹ������������Է���ֵ���ͻ��ǵ�����������
		self& operator++() {
			_node = _node->_next;
			return *this;
		}

		self operator++(int) {
			self temp(*this);
			_node = _node->_next;
			return temp;//������ʱ���������ܷ������ã�����ֻ���ô�ֵ������
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
			//��ͷ����˫��ѭ������
			_head = new Node(T());//���ܸ�0����Ϊ��֪��������ʲô����
			//��һ����int,����ֱ�Ӵ���һ��T���͵���������
			_head->_next = _head;
			_head->_prev = _head;
		}

		~list(){
		}

		void pop_back() {
			erase(--end());//��--���ã���֤�������һ��Ԫ��
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
		Node* _head;//һ��ͷ�ڵ�Ϳ��Ա�������list��Ϊ�ײ���˫��ѭ������
		//������ֻ��һ��ָ�룬���Ա�֤�������ջ�Ͽռ�СһЩ��������Ǹ�ͷ�ڵ㵱ȻҲ���ԣ����ǲ�֪��
		//ͷ�ڵ����data��ʲô���ͣ����������ͱȽϴ���ô�����ͻ�ܴ�
	};
}