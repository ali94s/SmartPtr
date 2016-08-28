#define _CRT_SECURE_NO_WARNINGS
#include"SmartPtr.h"
#include"SharedPtr.h"
struct ListNode
{
	SharedPtr<ListNode > _prev;
	SharedPtr<ListNode > _next;

	//weak_ptr<ListNode > _prev;
	//weak_ptr<ListNode > _next;
	~ListNode()
	{
		cout << "~ListNode()" << endl;
	}
};

void TestShared()
{
	SharedPtr <ListNode> p1(new ListNode());
	SharedPtr <ListNode> p2(new ListNode());

	cout << "p1->Count:" << p1.usecount() << endl;
	cout << "p2->Count:" << p2.usecount() << endl;

	 //p1�ڵ��_nextָ�� p2�ڵ�
	p1->_next = p2;
	 //p2�ڵ��_prevָ�� p1�ڵ� 
 	p2->_prev = p1;

	cout << "p1->Count:" << p1.usecount() << endl;
	cout << "p2->Count:" << p2.usecount() << endl;
}
void TestDel()
{
	SharedPtr<FILE,FClose> p1(fopen("test.txt", "w"));
}
int  main()
{	
	//int *a = NULL;
	//free(NULL);
	//delete a;
	//TestAutoPtr();
	//TestScopedPtr();
	TestShared();
	TestDel();
	system("pause");
	return 0;
}