// CMemory.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

class A {
public:
	A():_a(0x0a0a0a0a){}
	int _a;
	virtual void f(){}
};

class B : public A 
{
public:
	B() :_b(0x0b0b0b0b) {}
	int _b;
	virtual void f() {}
	virtual void g() {}
	void test(){}
};

class C : public B
{
public:
	C() : _c(0x0c0c0c0c){}
	int _c;
	virtual void h(){}
};

class D : public A
{
public:
	D() : _d(0x0d0d0d0d){}
	int _c;
	virtual void h(){}
};
//���ǲ����ı�
//���ǲ����ı�2
//���ǲ����ı�3
//���ǲ����ı�4
//���ǲ����ı�5
//���ǲ����ı�6
//���ǲ����ı�7
//���ǲ����ı�8
//���ǲ����ı�9
//���ǲ����ı�10
//���ǲ����ı�11
class E : public A
{
public:
	E() : _e(1){}
	int _e;
	virtual void h(){}
};

int main()
{
	A a;
	B b;
	C c;
    return 0;
}

