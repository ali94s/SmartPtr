#include<iostream>
using namespace std;

//�����ܵ�����ָ��

//ѭ������
//�̰߳�ȫ
//����ɾ��
//����ģʽ

//DΪ����ɾ����ģ�����
template<class T, class D=Delete>
class SharedPtr
{
public:
	SharedPtr() :_ptr(NULL)
	{}
	SharedPtr(T* ptr) :_ptr(ptr), _usecount(new int(1))
	{}

	~SharedPtr()
	{
		if (-- (*_usecount) == 0)
		{
			D()(_ptr);
			delete _usecount;
		}
	}

	SharedPtr(SharedPtr<T,D>& sp) :_ptr(sp._ptr), _usecount(sp._usecount)
	{
		++*(_usecount);
	}

	bool operator!=(SharedPtr<T, D>& sp)
	{
		return _ptr != sp._ptr;
	}

	SharedPtr<T,D>& operator=(SharedPtr<T,D>& sp)
	{
		if (*this != sp)
		{
			//cout << *_usecount << endl;
			if (_ptr != NULL)
			{
				if (--*_usecount == 0)
					this->~SharedPtr();
			}
				_ptr = sp._ptr;
				_usecount = sp._usecount;
				++*_usecount;
		}
		return *this;
	}

	//SharedPtr<T,D>& operator=(SharedPtr<T,D> sp)
	//{
	//	swap(_ptr, sp._ptr);
	//	swap(_usecount, sp._usecount);
	//	//�ִ�ʽд��  ���Ƚ��п������� ��sp����ָ������ü�������++
	//	//Ȼ�󽻻� ��֤���ü�����ȷ
	//	//������������Զ�����
	//	return *this;
	//}

	T& operator&()
	{
		return *_ptr.
	}

	T* operator->()
	{
		return _ptr;
	}

	int usecount()
	{
		return *_usecount;
	}

private:
	T* _ptr;
	int *_usecount;
};


// ���õ�ɾ�����º���
class Delete
{
public:
	void operator()(void* ptr)
	{
		cout << "delete" << endl;
		delete ptr;
	}
};
class FClose
{
public:
	void operator () (void* ptr)
	{
		cout << "fclose" << endl;
		fclose((FILE *)ptr);
	}
};
class Free
{
public:
	void operator () (void* ptr)
	{
		cout << "free" << endl;
		free(ptr);
	}
};