//author：caocun
//function：STLlist

#ifndef _MYLIST_H
#define _MYLIST_H
#include"myalloc.h"
#include"myconstruct.h"
#include"myiterator.h"

namespace caocun   //添加命名空间，防止和系统的冲突
{
	template<typename _Ty, typename Alloc = myalloc::alloc>
	class list   //类模板
	{
	protected:
		struct _Node;                //声明结点类
		typedef struct _Node * _Nodeptr;  //重命名结点指针
		struct _Node                //定义结点类
		{
			_Ty _Value;                //结点的值
			_Nodeptr _Prev, _Next;   //指向结点的前驱和后继的指针
		};
		struct _Acc                  //访问结点的方法类
		{
			typedef struct _Node *& _Nodepref;
			typedef _Ty & _Vref;
			static _Vref _Value(_Nodeptr _P)  //获取结点的值的引用，可以取值，也可以赋值
			{
				return (*_P)._Value;
			}
			static _Nodepref _Prev(_Nodeptr _P)//获取指向结点前驱的指针
			{
				return (*_P)._Prev;
			}
			static _Nodepref _Next(_Nodeptr _P)//获取指向结点后驱的指针
			{
				return (*_P)._Next;
			}
		};
	public:
		typedef Alloc       allocator_type;/// 
		typedef typename myalloc::simple_alloc<_Node,Alloc> data_alloctor;
		typedef list<_Ty>       _Myt;   //list自身类型
		typedef _Ty                value_type;//值的类型
		typedef _Ty *              _Tptr;  //指针类型
		typedef const _Ty *    _Ctptr; //常指针类型
		typedef _Ty &            reference; //引用类型
		typedef const _Ty &  const_reference;//常引用类型

		typedef int                 difference_type;//元素之间的距离类型
		typedef unsigned int  size_type;   //元素个数类型
	public:
		class const_iterator;  //常迭代器
		class iterator;            //普通迭代器
		class const_iterator:public myiterator::_Bidit<_Ty, int>
		{
		public:
			const_iterator(_Nodeptr _P = NULL):_Ptr(_P){}
			const_reference  operator*() const// const_reference * const this
			{
				return _Acc::_Value(_Ptr);
			}
			_Ctptr operator->() const //list<student>::iterator it ; it->show() 等价于 (*it)->show()  => it == *this; *it == **this；
			{
				return &**this;  //因为我们关心的是容器中的值的地址，而不是容器的地址
			}
			const_iterator & operator++()
			{
				_Ptr = _Acc::_Next(_Ptr);		
				return *this;
			}
			const_iterator  operator++(int) //不能返回引用，因为_Tmp是局部变量。
			{
				const_iterator _Tmp = *this;
				++*this;
				return _Tmp;
			}
			const_iterator & operator--()
			{
				_Ptr = _Acc::_Prev(_Ptr);		
				return *this;
			}
			const_iterator  operator--(int) //不能返回引用，因为_Tmp是局部变量。
			{
				const_iterator _Tmp = *this;
				--*this;
				return _Tmp;
			}
			bool operator==(const_iterator &_X) const
			{
				return _Ptr == _X._Ptr ;
			}
			bool operator!=(const_iterator &_X) const
			{
				return !(*this == _X);
			}
			_Nodeptr _Mynode()const
			{
				return _Ptr;
			}
		protected:
			_Nodeptr _Ptr;
		};
		class iterator : public const_iterator
		{
		public:
			iterator(_Nodeptr _P = NULL):const_iterator(_P){}
			reference operator*()
			{
				return _Acc::_Value(_Ptr);
			}
			_Tptr operator->() const //list<student>::iterator it ; it->show() 等价于 (*it)->show()  => it == *this; *it == **this；
			{
				return &**this;  //因为我们关心的是容器中的值的地址，而不是容器的地址
			}
			iterator & operator++()
			{
				_Ptr = _Acc::_Next(_Ptr);		
				return *this;
			}
			iterator  operator++(int) //不能返回引用，因为_Tmp是局部变量。
			{
				iterator _Tmp = *this;
				++*this;
				return _Tmp;
			}
			iterator & operator--()
			{
				_Ptr = _Acc::_Prev(_Ptr);		
				return *this;
			}
			iterator  operator--(int) //不能返回引用，因为_Tmp是局部变量。
			{
				iterator _Tmp = *this;
				--*this;
				return _Tmp;
			}
			bool operator==(iterator &_X) const
			{
				return _Ptr == _X._Ptr ;
			}
			bool operator!=(iterator &_X) const
			{
				return !(*this == _X);
			}
		};
		iterator begin()
		{
			return iterator(_Acc::_Next(_Head));
		}
		iterator end()
		{
			return iterator(_Head);
		}
		const_iterator begin() const 
		{
			return const_iterator(_Acc::_Next(_Head));
		}
		const_iterator end() const
		{
			return const_iterator(_Head);
		}
	public:
		typedef const_iterator _It;
		list():_Head(_Buynode()),_Size(0){}
		list(const _Ty *_F,const _Ty *_L):_Head(_Buynode()),_Size(0)
		{
			insert(begin(),_F,_L);
		}
		list(size_type _N,const _Ty &_X):_Head(_Buynode()),_Size(0)
		{
			insert(begin(),_N,_X);
		}
		list( const _Myt & _X):_Head(_Buynode()),_Size(0)
		{
			insert(begin(),_X.begin(),_X.end());
		}
		list(_It _F,_It _L):_Head(_Buynode()),_Size(0)
		{
			insert(begin(),_F,_L);
		}
		~list()
		{
			clear();
			_Freenode(_Head);
			_Head =  NULL;
		}
	public:
		void push_back(const _Ty &_X)
		{
			insert(end(), _X);
		}
		void push_front(const _Ty & _X)
		{
			insert(begin(), _X);
		}
		void insert(iterator _P,const _Ty *_F,const _Ty *_L )
		{
			for(;_F != _L; ++_F)
			{
				insert(_P, *_F);
			}
		}
		void insert(iterator _P,size_type _N, const _Ty & _X )
		{
			for(; _N>0; --_N)
			{
				insert(_P, _X);
			}
		}
		void insert(iterator _P, _It  _F, _It  _L)
		{
			for(; _F != _L; ++_F)
			{
				insert(_P, *_F);
			}
		}

		iterator insert(iterator _P, const _Ty &_X)//在_P前插入，返回迭代器
		{
			_Nodeptr _S = _P._Mynode();
			_Acc::_Prev(_S) = _Buynode(_Acc::_Prev(_S), _S);
			_S = _Acc::_Prev(_S);
			_Acc::_Next(_Acc::_Prev(_S)) = _S;
			//??error//_Acc::_Value(_S) = _X;  //不能直接把值送给空间
			//new (&_Acc::_Value(_S)) _Ty(_X);   //定位new，
			myconstruct::construct(&_Acc::_Value(_S),_X);
			_Size += 1;
			return iterator(_S);
		}
		//////////////////////////////////////////////////////////////////////////////
		void pop_back()
		{
			erase(--end());
		}
		void pop_front()
		{
			erase(begin());
		}
		void clear()
		{
			erase(begin(), end());
		}
		void erase(iterator _F,iterator _L)
		{
			for(; _F != _L; )
			{
				erase(_F++);  //一定要注意++，如果没有的话会引起迭代器失效
			}
		}
		iterator erase(iterator _P)
		{
			_Nodeptr _S = _P++._Mynode();  
			_Acc::_Next(_Acc::_Prev(_S)) = _Acc::_Next(_S);
			_Acc::_Prev(_Acc::_Next(_S)) = _Acc::_Prev(_S);
			//(&_Acc::_Value(_S))->~_Ty();  //调用它的值的析构函数
			myconstruct::destroy(&_Acc::_Value(_S));
			_Freenode(_S);
			_Size -= 1;
			return _P;
		}
	private:
		_Nodeptr _Buynode(_Nodeptr _Parg=NULL, _Nodeptr _Narg=NULL)
		{
			//_Nodeptr _S = (_Nodeptr)malloc(sizeof(struct _Node));
			_Nodeptr _S = data_alloctor::allocate();
			if(NULL == _S) exit(1);
			_Acc::_Prev(_S) = NULL == _Parg ? _S : _Parg;
			_Acc::_Next(_S) = NULL == _Narg ? _S : _Narg;
			return _S;
		}
		void _Freenode(_Nodeptr _P)
		{
			//free(_P);
			data_alloctor::deallocate(_P);
			_P = NULL;
		}
		_Nodeptr _Head;
		size_type _Size;
	};
};
#endif
