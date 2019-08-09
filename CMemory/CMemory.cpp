// CMemory.cpp : 定义控制台应用程序的入口点。
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

int main()
{
	A a;
	B b;
	C c;
    return 0;
}

