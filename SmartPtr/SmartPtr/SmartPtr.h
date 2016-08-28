#include<iostream>
using namespace std;


//AutoPtr����ģ��ԭ��ָ�� ����ֻ��������Ȩ����ת��
//���ڿ�������͸�ֵ �Ὣ�����͸�ֵ��ָ���ÿ�����֤��������ȷ��
//��û������������ģ��
template<class T>
class AutoPtr
{
public:
	//explicit��ֹ����������ʽת��  ���ڵ������Ĺ��캯��
	explicit AutoPtr(T* ptr = NULL) :_ptr(ptr)
	{}

	~AutoPtr()
	{
		delete _ptr;
	}

	//AutoPtr�汾������ָ��Ŀ�������͸�ֵ�ǽ���ָ��Ȩ��ת��
	explicit AutoPtr(AutoPtr<T>& ap) :_ptr(ap._ptr)
	{
		ap._ptr = NULL;
	}

	//�ִ�ʽд�� 
	//AutoPtr<T>& operator=(AutoPtr<T> ap)
	//{
	//	swap(_ptr, ap._ptr);
	//	return *this;
	//}

	AutoPtr<T>& operator=(AutoPtr<T>& ap)
	{
		//�Ը�ֵ
		if (*this != ap)   //this!=&ap
		{
			//�ͷ�ԭ����ָ��
			delete _ptr;
			_ptr = NULL;
			//Ȩ��ת��
			_ptr = ap._ptr;
			//�ÿ�
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

	//ʵ����Դ����Ȩ�ͷ�
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


//Ϊ�˸Ľ�AutoPtr��һ�ֱ����ֶ� ���ø�ֵ�Ϳ���
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

//ƫ�ػ���[]
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
	//����ʹ��ScopedPtr<int> sp4 = new int; û�п��õĹ��캯�� ��������Ϊexplicit
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


//���Ҫģ�³�������ԭ��ָ����ֵ�sharedptr
//ʹ�����ü��������Ȩ��ת�Ƶ�����
