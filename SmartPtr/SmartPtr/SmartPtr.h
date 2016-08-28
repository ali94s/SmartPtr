#include<iostream>
using namespace std;


//AutoPtr的是模仿原生指针 但是只是做到了权利的转移
//对于拷贝构造和赋值 会将拷贝和赋值的指针置空来保证析构的正确性
//并没有做到真正的模仿
template<class T>
class AutoPtr
{
public:
	//explicit防止编译器的隐式转换  对于单参数的构造函数
	explicit AutoPtr(T* ptr = NULL) :_ptr(ptr)
	{}

	~AutoPtr()
	{
		delete _ptr;
	}

	//AutoPtr版本的智能指针的拷贝构造和赋值是进行指向权的转移
	explicit AutoPtr(AutoPtr<T>& ap) :_ptr(ap._ptr)
	{
		ap._ptr = NULL;
	}

	//现代式写法 
	//AutoPtr<T>& operator=(AutoPtr<T> ap)
	//{
	//	swap(_ptr, ap._ptr);
	//	return *this;
	//}

	AutoPtr<T>& operator=(AutoPtr<T>& ap)
	{
		//自赋值
		if (*this != ap)   //this!=&ap
		{
			//释放原来的指向
			delete _ptr;
			_ptr = NULL;
			//权利转移
			_ptr = ap._ptr;
			//置空
			ap._ptr = NULL;
		}
		return *this;
	}

	T& operator	*()
	{
		return *_ptr;
	}

	T* operator->()
	{
		return _ptr;
	}

	operator bool()
	{
		return _ptr != NULL;
	}

	//实现资源所有权释放
	T* release()
	{
		T* ret = _ptr;
		_ptr = NULL;
		return ret;
	}
private:
	T* _ptr;
};


class TestC{
public:
	int a;
	void f()
	{
		cout << "TestC::f()" << endl;
	}
};

void TestAutoPtr()
{
	AutoPtr<int> auP1(new int(10));
	if (auP1)
	{
		cout << "test" << endl;
	}

	cout << *auP1 << endl;

	AutoPtr<int> auP2(auP1);
	AutoPtr<int> auP3;
	auP3 = auP2;
	//cout << *auP1 << endl;
	//cout << *auP2 << endl;
	cout << *auP3 << endl;

	AutoPtr<TestC> auT(new TestC);
	auT->a = 4;
	cout << auT->a << endl;
	auT->f();
}


//为了改进AutoPtr的一种暴力手段 不让赋值和拷贝
template<class T>
class ScopedPtr
{
public:
	explicit ScopedPtr(T* ptr) :_ptr(ptr)
	{}

	~ScopedPtr()
	{
		delete _ptr;
	}

	T& operator*()const
	{
		return *_ptr;
	}
	T* operator->()const
	{
		return _ptr;
	}

	T* Get()
	{
		return _ptr;
	}

	void Swap(ScopedPtr &b)
	{
		::swap(_ptr, b._ptr);
	}

	operator bool()
	{
		return _ptr != NULL;
	}


protected:
	ScopedPtr(ScopedPtr<T>& sp);
	ScopedPtr<T>& operator=(ScopedPtr<T>& sp);
private:
	T* _ptr;
};

//偏特化的[]
template<class T>
class ScopedPtr<T[]>
{
public:
	explicit ScopedPtr(T* ptr) :_ptr(ptr)
	{}

	~ScopedPtr()
	{
		delete []_ptr;
	}


	T& operator*()const
	{
		return *_ptr;
	}
	T* operator->()const
	{
		return _ptr;
	}

	T* Get()
	{
		return _ptr;
	}

	void Swap(ScopedPtr &b)
	{
		swap(_ptr, b._ptr);
	}

	operator bool()
	{
		return _ptr != NULL;
	}
private:
	ScopedPtr(const ScopedPtr&);
	ScopedPtr&operator=(const ScopedPtr&);
	bool operator==(const ScopedPtr)const;
	bool operator!=(const ScopedPtr)const;
	T* _ptr;
};

void TestScopedPtr()
{
	//不能使用ScopedPtr<int> sp4 = new int; 没有可用的构造函数 函数声明为explicit
	ScopedPtr<int[]> sp(new int[10]);
	ScopedPtr<int> Sp1(new int(10));
	ScopedPtr<int> Sp2(new int(20));
	ScopedPtr<TestC>Sp3(new TestC);

	cout << *Sp1 << endl;
	Sp3->a = 4;
	Sp3->f();

	AutoPtr<int> AuP(new int(4));
	//ScopedPtr<int> Sp(AuP);

	//cout << *Sp << endl;
}


//如果要模仿出真正的原生指针出现的sharedptr
//使用引用计数来解决权利转移的问题
